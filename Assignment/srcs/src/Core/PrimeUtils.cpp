#include "primeUtils.h"
#include "../utils/logger.h"
#include "../utils/randomGenerator.h" // For generating random test numbers

namespace PrimeUtils
{
    // Determine the number of Miller-Rabin rounds based on bit length
    int calculate_miller_rabin_rounds(int bit_length)
    {
        if (bit_length >= 2048)
            return 40;
        if (bit_length >= 1024)
            return 30;
        if (bit_length >= 512)
            return 25;
        if (bit_length >= 256)
            return 20;
        return 15;
    }

    // Helper function: Checks if n is divisible by any small primes for quick elimination
    bool is_divisible_by_small_primes(const mpz_class &n)
    {
        static const int small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
        for (int prime : small_primes)
        {
            if (mpz_divisible_ui_p(n.get_mpz_t(), prime))
            {
                Logger::log("PrimeUtils", "Candidate divisible by small prime: " + std::to_string(prime));
                return true;
            }
        }
        return false;
    }

    // Miller-Rabin probabilistic primality test
    bool miller_rabin_test(const mpz_class &n, int rounds)
    {
        // Return false immediately for numbers <= 1, even numbers, and numbers 2 and 3
        if (n <= 1)
            return false;
        if (n == 2 || n == 3)
            return true;
        if (mpz_even_p(n.get_mpz_t()))
            return false;

        mpz_class d = n - 1;
        int r = 0;

        // Decompose n-1 as d * 2^r
        while (mpz_divisible_2exp_p(d.get_mpz_t(), 1))
        {
            d /= 2;
            r++;
        }

        gmp_randclass rand_gen(gmp_randinit_default);
        rand_gen.seed(RandomGenerator::generate_random_number(32).get_ui());

        // Perform the specified number of Miller-Rabin test rounds
        for (int i = 0; i < rounds; ++i)
        {
            mpz_class a = rand_gen.get_z_range(n - 3) + 2; // Random base a in [2, n-2]
            mpz_class x = RandomGenerator::mod_exp(a, d, n);

            if (x == 1 || x == n - 1)
                continue;

            bool passed = false;
            for (int j = 0; j < r - 1; j++)
            {
                x = (x * x) % n;
                if (x == n - 1)
                {
                    passed = true;
                    break;
                }
            }
            if (!passed)
            {
                Logger::log("PrimeUtils", "Failed Miller-Rabin test with base a = " + a.get_str());
                return false;
            }
        }
        Logger::log("PrimeUtils", "Passed Miller-Rabin test for candidate.");
        return true;
    }

    // Main function to check if a number is prime
    bool is_prime(const mpz_class &n, int bit_length)
    {
        // Logger::log("PrimeUtils", "Checking primality of candidate: " + n.get_str());

        // Check divisibility by small primes first
        if (is_divisible_by_small_primes(n))
        {
            Logger::log("PrimeUtils", "Candidate failed small prime divisibility test.");
            return false;
        }

        // Determine the number of Miller-Rabin rounds and perform the test
        int rounds = calculate_miller_rabin_rounds(bit_length);
        bool miller_rabin_result = miller_rabin_test(n, rounds);

        // Optionally use GMP's primality test as an additional check
        int gmp_result = mpz_probab_prime_p(n.get_mpz_t(), 25); // 25 rounds for GMP test

        if (miller_rabin_result && gmp_result > 0)
        {
            // Logger::log("PrimeUtils", "Candidate passed both Miller-Rabin and GMP tests and is prime.");
            return true;
        }

        // Logger::log("PrimeUtils", "Candidate failed primality checks.");
        return false;
    }
}
