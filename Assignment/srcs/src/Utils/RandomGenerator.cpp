#include "RandomGenerator.h"
#include <chrono>
#include <random>
#include "Logger.h"

namespace RandomGenerator
{
    mpz_class generate_random_prime_candidate(int bit_length, unsigned int thread_id)
    {
        gmp_randclass rand_gen(gmp_randinit_default);
        auto now = std::chrono::high_resolution_clock::now();
        auto seed = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
        rand_gen.seed(seed + thread_id);

        mpz_class candidate = rand_gen.get_z_bits(bit_length);
        mpz_setbit(candidate.get_mpz_t(), bit_length - 1);
        mpz_setbit(candidate.get_mpz_t(), 0);

        // Logger::log("RandomGenerator", "Generated prime candidate: " + candidate.get_str());
        return candidate;
    }

    mpz_class generate_random_number(int bit_length)
    {
        gmp_randclass rand_gen(gmp_randinit_default);
        auto now = std::chrono::high_resolution_clock::now();
        auto seed = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
        rand_gen.seed(seed);

        return rand_gen.get_z_bits(bit_length);
    }

    mpz_class mod_exp(mpz_class base, mpz_class exp, mpz_class mod)
    {
        mpz_class result = 1;
        while (exp > 0)
        {
            if (exp.get_ui() & 1)
                result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result;
    }
}
