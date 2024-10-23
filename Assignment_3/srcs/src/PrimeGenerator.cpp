#include "PrimeGenerator.h"

const double C_Fermat = 0.05;
const double C_Miller_Rabin = 0.4;

// Hàm tính lũy thừa mô-đun
mpz_class Exponentiation_By_Squaring(const mpz_class &base, const mpz_class &exp, const mpz_class &mod)
{
    mpz_class result = 1;
    mpz_class b = base;
    mpz_class e = exp;

    while (e > 0)
    {
        if (e % 2 == 1)
        {
            result = (result * b) % mod; // Update result when exponent is odd
        }
        b = (b * b) % mod; // Square the base
        e /= 2;            // Divide exponent by 2
    }
    return result;
}

mpz_class Generate_Random_Large_Integer(int bit_length, gmp_randclass &rand_gen)
{
    mpz_class candidate = rand_gen.get_z_bits(bit_length - 1);
    mpz_setbit(candidate.get_mpz_t(), bit_length - 1); // Ensure the highest bit is set to 1
    return candidate;
}

int Fermat_Iterations(int bit_length, double C)
{
    return std::min(5, static_cast<int>(std::ceil(C * std::log2(bit_length))));
}

bool Fermat_Test(const mpz_class &bigInteger, int iterations)
{
    if (bigInteger <= 1)
        return false; // Numbers less than or equal to 1 are not prime
    if (bigInteger <= 3)
        return true; // 2 and 3 are prime numbers

    gmp_randclass rand_gen(gmp_randinit_mt); // Use Mersenne Twister
    rand_gen.seed(time(NULL));

    for (int i = 0; i < iterations; i++)
    {
        mpz_class a = 2 + rand_gen.get_z_bits(bigInteger.get_str().length() - 3);
        if (Exponentiation_By_Squaring(a, bigInteger - 1, bigInteger) != 1)
        {
            return false; // If Fermat's condition fails, the number is not prime
        }
    }
    return true; // If all tests pass, the number is likely prime
}

int Miller_Rabin_Iterations(int bit_length, double C)
{
    return std::min(20, static_cast<int>(std::ceil(C * std::log2(bit_length))));
}

bool Miller_Rabin(const mpz_class &bigInteger, int iterations, gmp_randclass &rand_gen)
{
    if (bigInteger <= 1)
        return false;
    if (bigInteger <= 3)
        return true;

    mpz_class d = bigInteger - 1;
    int r = 0;
    while (d % 2 == 0)
    {
        d /= 2;
        r++;
    }

    for (int i = 0; i < iterations; i++)
    {
        mpz_class a = 2 + rand_gen.get_z_bits(bigInteger.get_str().length() - 3);
        mpz_class x = Exponentiation_By_Squaring(a, d, bigInteger);
        if (x == 1 || x == bigInteger - 1)
            continue; // Continue if x is a trivial witness

        bool continue_outer_loop = false;
        for (int j = 1; j < r; j++)
        {
            x = Exponentiation_By_Squaring(x * x, 1, bigInteger);
            if (x == bigInteger - 1)
            {
                continue_outer_loop = true; // Found a witness, continue with next iteration
                break;
            }
        }
        if (!continue_outer_loop)
        {
            return false; // If no witness found, the number is composite
        }
    }
    return true; // If all tests pass, the number is likely prime
}

bool Is_Prime_Integer(const mpz_class &bigInteger, int bits_length, gmp_randclass &rand_gen)
{
    int fermat_iterations = Fermat_Iterations(bits_length, C_Fermat);
    if (!Fermat_Test(bigInteger, fermat_iterations))
        return false; // If Fermat test fails, return false

    int miller_iterations = Miller_Rabin_Iterations(bits_length, C_Miller_Rabin);
    return Miller_Rabin(bigInteger, miller_iterations, rand_gen); // Perform Miller-Rabin test
}

BigInteger::BigInteger() : value(0) {}

BigInteger BigInteger::Find_Prime_Number(int bits_length, gmp_randclass &rand_gen)
{
    BigInteger candidate;
    do
    {
        candidate.value = Generate_Random_Large_Integer(bits_length, rand_gen);
    } while (!Is_Prime_Integer(candidate.value, bits_length, rand_gen)); // Check for primality

    return candidate; // Return the found prime number
}

mpz_class BigInteger::get_value() const
{
    return value; // Return the value as mpz_class
}

mpz_class Generate_Prime_Number(int bits_length, gmp_randclass &rand_gen)
{
    BigInteger prime = BigInteger::Find_Prime_Number(bits_length, rand_gen);
    return prime.get_value(); // Return the value from the BigInteger object
}
