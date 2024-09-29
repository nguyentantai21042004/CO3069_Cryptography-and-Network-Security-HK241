#include <iostream>
#include <gmpxx.h> // This one is for big integer
#include <ctime>   // This one is for time
#include <cstdlib> // This one is for rand
#include <chrono>  // This one is for calculating time
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

std::atomic<bool> found(false); // Variable to check if a prime number has been found
mpz_class big_prime_integer;    // Global variable to store the found prime number
std::mutex mtx;                 // Mutex to protect access to big_prime_integer

// Generate random number with require bit length
mpz_class generate_random_big_integer(int bit_nums);

// Check Prime with all Primes < 100
bool Quick_Check(const mpz_class &bigInteger);

// Hàm kiểm tra nguyên tố Fermat
bool Fermat_Test(const mpz_class &p, int iterations);

// Modular Exponentiation Function
void squared_exponentiation(mpz_class &result, const mpz_class &a, const mpz_class &b, const mpz_class &n);

// Miller-Rabin Primality Test Function
bool Miller_Rabin(const mpz_class &bigInteger, int iterations);

// Main Function in each Thread
void findBigPrime(int n);

int main()
{
    int n;

    std::cout << "Enter the required number of bits (greater than 500): ";
    std::cin >> n;

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Number of threads based on hardware capabilities
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    // Create and start streams
    for (int i = 0; i < numThreads; ++i)
        threads.emplace_back(findBigPrime, n);

    // Wait for all threads to complete
    for (auto &thread : threads)
        thread.join();

    // End of time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate running time
    std::chrono::duration<double> duration = end - start;

    if (found)
        std::cout << "Found it !!! Big Prime Integer is " << big_prime_integer << std::endl;
    else
        std::cout << "Không tìm thấy số nguyên tố nào." << std::endl;

    std::cout << "Search time: " << duration.count() << " seconds" << std::endl;

    return 0;
}

mpz_class generate_random_big_integer(int bit_nums)
{
    mpz_class result;

    /* Random state initialization.*/
    /* Using the current time (like std::time(nullptr)) is a common way to create a unique seed. */
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, static_cast<unsigned int>(std::time(nullptr)));

    mpz_urandomb(result.get_mpz_t(), state, bit_nums);

    /* Make sure that the integer has exactly n bits, because the highest bit of an n-bit integer cannot be 0 */
    mpz_setbit(result.get_mpz_t(), bit_nums - 1);

    /* Make sure that the lowest bit (bit 0) of the result integer is set to 0 ==> be odd big integer */
    mpz_setbit(result.get_mpz_t(), 0);

    gmp_randclear(state);
    return result;
}

bool Fermat_Test(const mpz_class &p, int iterations)
{
    if (p < 2)
        return false;
    for (int i = 0; i < iterations; ++i)
    {
        // Chọn số ngẫu nhiên a từ [2, p-2]
        mpz_class a = generate_random_big_integer(p.get_str().length() - 1) + 2;
        if (mpz_cmp(a.get_mpz_t(), p.get_mpz_t()) >= 0)
            continue; // Bỏ qua nếu a >= p
        mpz_class result;
        mpz_powm(result.get_mpz_t(), a.get_mpz_t(), p.get_mpz_t() - 1, p.get_mpz_t());
        if (result != 1)
            return false;
    }
    return true; // Có khả năng là nguyên tố
}

bool Quick_Check(const mpz_class &bigInteger)
{
    /* List of small prime numbers */
    const std::vector<int> small_primes = {2, 3, 5, 7, 11,
                                           13, 17, 19, 23, 29,
                                           31, 37, 41, 43, 47,
                                           53, 59, 61, 67, 71,
                                           73, 79, 83, 89, 97};

    /* Check divisibility for each small prime number */
    for (int prime : small_primes)
    {
        if (mpz_divisible_ui_p(bigInteger.get_mpz_t(), prime))
        {
            std::cout << "Divisible by " << prime << ". Remove this number.\n";
            return false; // If not divisible by any small prime, continue checking with Miller_Rabin
        }
    }

    return true;
}

// Modular Exponentiation Function
void squared_exponentiation(mpz_class &result, const mpz_class &a, const mpz_class &b, const mpz_class &n)
{
    mpz_class base = a;
    mpz_class exp = b;
    result = 1;

    while (exp > 0)
    {
        if (exp % 2 == 0)
        { // exp là số chẵn
            base = (base * base) % n;
            exp /= 2;
        }
        else
        { // exp là số lẻ
            result = (result * base) % n;
            exp -= 1;
        }
    }
}

// Miller-Rabin Primality Test Function
bool Miller_Rabin(const mpz_class &bigInteger, int iterations)
{
    if (bigInteger <= 1)
        return false;
    if (bigInteger <= 3)
        return true;

    /* 1. Find k, q such that (n - 1) = 2^k * q */ unsigned long k = 0;
    mpz_class q = bigInteger - 1;
    while (q % 2 == 0)
    {
        q /= 2;
        k++;
    }

    /* Random state initialization.*/
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < iterations; i++)
    {
        /* 2. Choose a random number a, 1 < a < bigInteger - 1 */
        mpz_class a;
        mpz_class upper_limit = bigInteger - 2; // Upper bound for random numbers

        // Check if bigInteger is greater than 2
        if (upper_limit <= 0)
        {
            gmp_randclear(state);
            std::cerr << "Error: The number must be greater than 2." << std::endl;
            return false;
        }

        mpz_urandomm(a.get_mpz_t(), state, upper_limit.get_mpz_t()); // a belongs to [0, bigInteger - 2]
        a += 2;                                                      // Make sure a belongs to [1, bigInteger - 1]

        std::cout << "Testing with random value a = " << a << ": ";

        // 3. Compute a^q mod bigInteger
        mpz_class x;
        squared_exponentiation(x, a, q, bigInteger);
        std::cout << "Value of x: " << x << std::endl;

        if (x == 1 || x == bigInteger - 1)
        {
            std::cout << "Successfully" << std::endl;
            continue; // Maybe bigInteger is a Prime number
        }

        // 4. Checking tra k lần
        bool isComposite = true;
        for (unsigned long j = 0; j < k - 1; j++)
        {
            // 5. Compute x = x^2 mod bigInteger
            x = (x * x) % bigInteger;
            if (x == bigInteger - 1)
            {
                isComposite = false; // Maybe bigInteger is a Prime number
                std::cout << "Successfully" << std::endl;
                break;
            }
        }

        /* 6. If composite */
        if (isComposite)
        {
            std::cout << "Unsuccessfully" << std::endl;
            gmp_randclear(state); // Release random state
            return false;         // bigInteger is a Composite number
        }
    }

    gmp_randclear(state); // Release random state
    return true;          // Maybe bigInteger is a Prime number
}

void findBigPrime(int n)
{
    while (!found)
    {
        mpz_class randomBigInteger = generate_random_big_integer(n);
        std::cout << "Checking with randomBigInteger = " << randomBigInteger << std::endl;

        if (Quick_Check(randomBigInteger))
        {
            if (Fermat_Test(randomBigInteger, 5))
            {
                if (Miller_Rabin(randomBigInteger, 15))
                {
                    std::lock_guard<std::mutex> lock(mtx); // Khóa mutex
                    if (!found)
                    { // Kiểm tra lại một lần nữa trước khi cập nhật
                        big_prime_integer = randomBigInteger;
                        found = true;
                    }
                }
            }
        }
    }
}