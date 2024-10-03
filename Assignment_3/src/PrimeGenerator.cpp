#include "PrimeGenerator.h"
#include <iostream>
#include <cstdlib> // For random number functions
#include <ctime>   // For random seed generation
#include <chrono>  // For measuring time
#include <thread>  // For multithreading
#include <mutex>   // For thread synchronization
#include <atomic>
#include <iomanip>       // For formatting output
#include <random>        // For random number generation using mt19937
#include <cmath>         // For logarithmic functions
#include <unordered_set> // For storing unique large integers
#include <shared_mutex>  // For shared and exclusive access to data structures

// Global variables used across threads
std::atomic<bool> found(false);
mpz_class big_prime_integer;
std::mutex mtx;

// Constants used in primality testing
const int NUM_PRIME_SMALLER_10_000 = 1229;
const int MIN_QUICK_CHECK_ITERATIONS = 100;
const double C_Quick_Check = 0.75;
const double C_Fermat = 0.5;
const double C_Miller_Rabin = 1.0;

// Unordered set to store generated large integers (to avoid duplicates)
std::unordered_set<std::string> taken;
std::shared_mutex taken_mtx; // Shared mutex for managing access to the set

// Converts a `mpz_class` number to a string
std::string mpz_to_string(const mpz_class &num)
{
    return num.get_str();
}

// Generates a large random integer with a specified bit length
mpz_class Generate_Big_Integer(int bit_length)
{
    mpz_class big_integer;

    // Use `std::mt19937` with a random device to generate a strong random seed
    std::random_device rd;
    std::mt19937 mt(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<unsigned long> dist(0, ULONG_MAX);
    unsigned long seed_value = dist(mt); // Generate the random seed

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed_value); // Initialize the GMP random state

    while (true)
    {
        // Generate a random number of specified bit length
        mpz_urandomb(big_integer.get_mpz_t(), state, bit_length);
        mpz_setbit(big_integer.get_mpz_t(), bit_length - 1); // Ensure the number is `bit_length` bits long

        std::string big_integer_str = mpz_to_string(big_integer); // Convert the large integer to a string

        {
            // Use shared lock for multiple thread reading access
            std::shared_lock<std::shared_mutex> lock(taken_mtx);
            if (taken.find(big_integer_str) == taken.end()) // If the number is not already in the set
            {
                lock.unlock(); // Unlock shared lock before acquiring exclusive lock

                // Use unique lock to exclusively insert into the set
                std::unique_lock<std::shared_mutex> write_lock(taken_mtx);
                taken.insert(big_integer_str); // Insert the number into the set
                break;                         // Break loop when a valid number is found
            }
        }

        // Increment the number to reduce time spent generating random numbers
        big_integer += 2; // Increment by 2 to maintain odd numbers (efficient for prime testing)
    }

    gmp_randclear(state);
    return big_integer;
}

// Performs squared exponentiation: Computes (a^b) % n
void Squared_Exponentiation(mpz_class &result, const mpz_class &a, const mpz_class &b, const mpz_class &n)
{
    mpz_class base = a;
    mpz_class exp = b;
    result = 1;

    while (exp > 0)
    {
        if (exp % 2 == 0)
        {
            base = (base * base) % n;
            exp /= 2;
        }
        else
        {
            result = (result * base) % n;
            exp -= 1;
        }
    }
}

// Performs Montgomery reduction to optimize modular operations
void Montgomery_Reduce(mpz_class &target, const mpz_class &n, const mpz_class &n_inv, const mpz_class &R)
{
    mpz_class m = (target * n_inv) % R; // Compute intermediate value for reduction
    target = (target + m * n) / R;      // Perform Montgomery reduction

    // Ensure the result is less than n
    if (target >= n)
        target -= n;
}

// Montgomery exponentiation: Optimized modular exponentiation using Montgomery reduction
void Montgomery_Exponentiation(mpz_class &result, const mpz_class &a, const mpz_class &b, const mpz_class &n)
{
    /* Initialize necessary variables for Montgomery */
    mpz_class R, R_inv, n_inv;
    int k = mpz_sizeinbase(n.get_mpz_t(), 2); // Get bit-length of n
    R = mpz_class(1) << k;                    // R = 2^k

    /* Calculate the modular inverse of R and n */
    mpz_invert(R_inv.get_mpz_t(), R.get_mpz_t(), n.get_mpz_t());
    mpz_invert(n_inv.get_mpz_t(), n.get_mpz_t(), R.get_mpz_t());

    /* Convert a to Montgomery form */
    mpz_class a_mont = (a * R) % n;
    result = R % n; // Start with Montgomery form of 1 (i.e. R mod n)

    mpz_class base = a_mont;
    mpz_class exp = b;

    /* Perform Montgomery exponentiation */
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result = (result * base) % n;
            Montgomery_Reduce(result, n, n_inv, R); // Apply Montgomery reduction
        }
        base = (base * base) % n;
        Montgomery_Reduce(base, n, n_inv, R); // Apply Montgomery reduction
        exp /= 2;                             // Halve the exponent
    }

    /* Convert result from Montgomery form back to normal form */
    result = (result * R_inv) % n;
}

// Determines the number of iterations for quick primality testing
int Quick_Test_Iteration(int bit_length, double C)
{
    int iterations = std::ceil(C * (static_cast<double>(bit_length) / 1024) * NUM_PRIME_SMALLER_10_000);
    iterations = std::min(iterations, static_cast<int>(NUM_PRIME_SMALLER_10_000));
    return iterations;
}

// Performs a quick check for primality by testing divisibility against small primes
bool Quick_Check(const mpz_class &bigInteger, int iterations)
{
    /* List of small prime numbers */
    const std::vector<int> small_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997,
                                           1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999,
                                           2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129, 2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287, 2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617, 2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741, 2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903, 2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999,
                                           3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181, 3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413, 3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571, 3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643, 3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727, 3733, 3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823, 3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907, 3911, 3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989,
                                           4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057, 4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139, 4153, 4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231, 4241, 4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297, 4327, 4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409, 4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493, 4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583, 4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657, 4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751, 4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831, 4861, 4871, 4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937, 4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999,
                                           5003, 5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087, 5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179, 5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279, 5281, 5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387, 5393, 5399, 5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443, 5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521, 5527, 5531, 5557, 5563, 5569, 5573, 5581, 5591, 5623, 5639, 5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693, 5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791, 5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857, 5861, 5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939, 5953, 5981, 5987,
                                           6007, 6011, 6029, 6037, 6043, 6047, 6053, 6067, 6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133, 6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217, 6221, 6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301, 6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373, 6379, 6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473, 6481, 6491, 6521, 6529, 6547, 6551, 6553, 6563, 6569, 6571, 6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673, 6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737, 6761, 6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833, 6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917, 6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991, 6997,
                                           7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103, 7109, 7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207, 7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297, 7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411, 7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499, 7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561, 7573, 7577, 7583, 7589, 7591, 7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699, 7703, 7717, 7723, 7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829, 7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919, 7927, 7933, 7937, 7949, 7951, 7963, 7993,
                                           8009, 8011, 8017, 8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093, 8101, 8111, 8117, 8123, 8147, 8161, 8167, 8171, 8179, 8191, 8209, 8219, 8221, 8231, 8233, 8237, 8243, 8263, 8269, 8273, 8287, 8291, 8293, 8297, 8311, 8317, 8329, 8353, 8363, 8369, 8377, 8387, 8389, 8419, 8423, 8429, 8431, 8443, 8447, 8461, 8467, 8501, 8513, 8521, 8527, 8537, 8539, 8543, 8563, 8573, 8581, 8597, 8599, 8609, 8623, 8627, 8629, 8641, 8647, 8663, 8669, 8677, 8681, 8689, 8693, 8699, 8707, 8713, 8719, 8731, 8737, 8741, 8747, 8753, 8761, 8779, 8783, 8803, 8807, 8819, 8821, 8831, 8837, 8839, 8849, 8861, 8863, 8867, 8887, 8893, 8923, 8929, 8933, 8941, 8951, 8963, 8969, 8971, 8999,
                                           9001, 9007, 9011, 9013, 9029, 9041, 9043, 9049, 9059, 9067, 9091, 9103, 9109, 9127, 9133, 9137, 9151, 9157, 9161, 9173, 9181, 9187, 9199, 9203, 9209, 9221, 9227, 9239, 9241, 9257, 9277, 9281, 9283, 9293, 9311, 9319, 9323, 9337, 9341, 9343, 9349, 9371, 9377, 9391, 9397, 9403, 9413, 9419, 9421, 9431, 9433, 9437, 9439, 9461, 9463, 9467, 9473, 9479, 9491, 9497, 9511, 9521, 9533, 9539, 9547, 9551, 9587, 9601, 9613, 9619, 9623, 9629, 9631, 9643, 9649, 9661, 9677, 9679, 9689, 9697, 9719, 9721, 9733, 9739, 9743, 9749, 9767, 9769, 9781, 9787, 9791, 9803, 9811, 9817, 9829, 9833, 9839, 9851, 9857, 9859, 9871, 9883, 9887, 9901, 9907, 9923, 9929, 9931, 9941, 9949, 9967, 9973};

    // Performs a quick check for primality by testing divisibility against small primes
    for (int i = 0; i < iterations; i++)
    {
        int prime = small_primes[i];
        if (mpz_divisible_ui_p(bigInteger.get_mpz_t(), prime))
        {
            return false; // It's not prime
        }
    }

    return true; // Passes the quick check, might be prime
}

// Determines the number of iterations for Fermat primality testing
int Fermat_Iterations(int bit_length, double C)
{
    int iterations = std::ceil(C * std::log2(bit_length));
    return iterations;
}

// Fermat primality test to determine if a number is prime
bool Fermat_Test(const mpz_class &p, int iterations)
{
    if (p < 2)
        return false;

    if (p % 2 == 0)
        return p == 2;

    /* Random state initialization */
    gmp_randstate_t state;
    gmp_randinit_default(state);

    // Use `std::random_device` for stronger entropy
    std::random_device rd;
    unsigned long seed_value = rd();    // Generate random seed
    gmp_randseed_ui(state, seed_value); // Set seed for GMP random generator

    for (int i = 0; i < iterations; ++i)
    {
        // Generate a random number `a` in the range [2, p-2]
        mpz_class a;
        mpz_class upper_limit = p - 3;
        mpz_urandomm(a.get_mpz_t(), state, upper_limit.get_mpz_t());
        a += 2;

        // Calculate a^(p-1) % p using Montgomery exponentiation
        mpz_class result;
        Montgomery_Exponentiation(result, a, p - 1, p);

        if (result != 1)
        {
            gmp_randclear(state);
            return false; // Definitely not prime
        }
    }

    gmp_randclear(state);
    return true; // Passes Fermat test, may be prime
}

// Determines the number of iterations for Miller-Rabin primality testing
int Miller_Rabin_Iterations(int bit_length, double C)
{
    int iterations = std::ceil(C * std::log2(bit_length));
    return iterations;
}

// Miller-Rabin primality test to determine if a number is prime
bool Miller_Rabin(const mpz_class &bigInteger, int iterations)
{
    if (bigInteger <= 1)
        return false;
    if (bigInteger <= 3)
        return true;

    // Find k, q such that (n - 1) = 2^k * q
    unsigned long k = 0;
    mpz_class q = bigInteger - 1;
    while (q % 2 == 0)
    {
        q /= 2;
        k++;
    }

    /* Montgomery setup */
    mpz_class R, R_inv, n_inv;
    int n_bits = mpz_sizeinbase(bigInteger.get_mpz_t(), 2);
    R = mpz_class(1) << n_bits; // R = 2^n_bits

    mpz_invert(R_inv.get_mpz_t(), R.get_mpz_t(), bigInteger.get_mpz_t()); // Inverse of R mod bigInteger
    mpz_invert(n_inv.get_mpz_t(), bigInteger.get_mpz_t(), R.get_mpz_t()); // Inverse of bigInteger mod R

    /* Random state initialization */
    gmp_randstate_t state;
    gmp_randinit_default(state);
    std::random_device rd;
    unsigned long seed_value = rd();
    gmp_randseed_ui(state, seed_value);

    for (int i = 0; i < iterations; i++)
    {
        // Generate random number `a` in range [2, bigInteger - 2]
        mpz_class a;
        mpz_class upper_limit = bigInteger - 2;

        mpz_urandomm(a.get_mpz_t(), state, upper_limit.get_mpz_t());
        a += 2;

        // Calculate a^q % bigInteger using Montgomery exponentiation
        mpz_class x;
        Montgomery_Exponentiation(x, a, q, bigInteger);

        if (x == 1 || x == bigInteger - 1)
            continue; // If x == 1 or bigInteger - 1, continue

        // Square x repeatedly to test if composite
        bool isComposite = true;
        for (unsigned long j = 0; j < k - 1; j++)
        {
            x = (x * x) % bigInteger;                   // Square x mod bigInteger
            Montgomery_Reduce(x, bigInteger, n_inv, R); // Apply Montgomery reduction

            if (x == bigInteger - 1)
            {
                isComposite = false;
                break;
            }
        }

        if (isComposite)
        {
            gmp_randclear(state);
            return false; // Composite number
        }
    }

    gmp_randclear(state);
    return true; // Passes Miller-Rabin test, may be prime
}

// Multi-threaded function to search for a large prime number
void Find_Big_Prime(int n, std::chrono::steady_clock::time_point start_time)
{
    while (found != true)
    {
        if (found == true)
            return;

        // Generate a large random integer
        mpz_class randomBigInteger = Generate_Big_Integer(n);
        mpz_setbit(randomBigInteger.get_mpz_t(), 0); // Ensure the number is odd

        // Perform primality tests
        int quick_check_iteration = Quick_Test_Iteration(n, C_Quick_Check);
        int fermat_iteration = Fermat_Iterations(n, C_Fermat);
        int miller_rabin_iteration = Miller_Rabin_Iterations(n, C_Miller_Rabin);

        if (Quick_Check(randomBigInteger, quick_check_iteration))
        {
            if (Fermat_Test(randomBigInteger, fermat_iteration))
            {
                if (Miller_Rabin(randomBigInteger, miller_rabin_iteration))
                {
                    // Use mutex to update global prime number
                    std::lock_guard<std::mutex> lock(mtx);
                    if (!found)
                    {
                        big_prime_integer = randomBigInteger;
                        found = true; // Signal threads to stop
                    }
                    return; // Exit after finding prime
                }
            }
        }
    }
}

// Generates a large prime number using multiple threads
mpz_class Generate_Prime_Number(int n)
{
    found = false; // Reset global flag
    big_prime_integer = 0;

    auto start = std::chrono::steady_clock::now(); // Start timer

    // Get number of threads supported by hardware
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    // Launch threads to search for a prime number
    for (int i = 0; i < numThreads; ++i)
        threads.emplace_back(Find_Big_Prime, n, start);

    // Wait for all threads to finish
    for (auto &thread : threads)
        thread.join();

    // End timer
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    if (found == true)
    {
        std::cout << "Prime Number Found!" << std::endl;
        std::cout << "Bits: " << n << std::endl;
        std::cout << "Value: " << big_prime_integer << std::endl;
        std::cout << "Time taken: " << std::fixed << std::setprecision(3) << duration.count() << " seconds" << std::endl;
        return big_prime_integer;
    }
    else
    {
        std::cerr << "No prime number found." << std::endl;
        return mpz_class(0); // Return 0 if no prime was found
    }
}
