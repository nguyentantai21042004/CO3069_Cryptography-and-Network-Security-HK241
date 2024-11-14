#ifndef KEY_GENERATOR_H
#define KEY_GENERATOR_H

#include <gmpxx.h>
#include <omp.h>
namespace KeyGenerator
{
    mpz_class generate_large_prime(int bit_length);
    mpz_class generate_strong_prime(int bit_length);
}

#endif // KEY_GENERATOR_H
