#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <gmpxx.h>

namespace RandomGenerator
{
    mpz_class generate_random_prime_candidate(int bit_length, unsigned int thread_id);
    mpz_class generate_random_number(int bit_length);
    mpz_class mod_exp(mpz_class base, mpz_class exp, mpz_class mod);
}

#endif // RANDOM_GENERATOR_H
