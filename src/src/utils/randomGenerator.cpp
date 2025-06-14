#include "randomGenerator.h"
#include <chrono>
#include <random>
#include "Logger.h"

namespace RandomGenerator
{
    // Function to generate a random prime candidate of specified bit length
    mpz_class generate_random_prime_candidate(int bit_length, unsigned int thread_id)
    {
        gmp_randclass rand_gen(gmp_randinit_default);
        auto now = std::chrono::high_resolution_clock::now();
        auto seed = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
        rand_gen.seed(seed + thread_id);

        // Generate a random candidate with the given bit length
        mpz_class candidate = rand_gen.get_z_bits(bit_length);

        // Ensure the candidate has the specified bit length and is odd
        mpz_setbit(candidate.get_mpz_t(), bit_length - 1); // Set the most significant bit to 1
        mpz_setbit(candidate.get_mpz_t(), 0);              // Set the least significant bit to 1 (odd number)

        Logger::log("RandomGenerator", "Generated prime candidate with bit length: " + std::to_string(bit_length));
        return candidate;
    }

    // Function to generate a random number of specified bit length
    mpz_class generate_random_number(int bit_length)
    {
        gmp_randclass rand_gen(gmp_randinit_default);
        auto now = std::chrono::high_resolution_clock::now();
        auto seed = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
        rand_gen.seed(seed);

        Logger::log("RandomGenerator", "Generated random number with bit length: " + std::to_string(bit_length));
        return rand_gen.get_z_bits(bit_length);
    }

    // Function to perform modular exponentiation: (base^exp) % mod
    mpz_class mod_exp(mpz_class base, mpz_class exp, mpz_class mod)
    {
        mpz_class result = 1;
        while (exp > 0)
        {
            if (exp.get_ui() & 1) // Check if the current bit of the exponent is 1
                result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1; // Shift exponent right by 1 bit
        }
        return result;
    }
}
