#ifndef PRIME_UTILS_H
#define PRIME_UTILS_H

#include <gmpxx.h>

namespace PrimeUtils
{
    bool is_prime(const mpz_class &n, int bit_length);
    bool miller_rabin_test(const mpz_class &n, int rounds);
    int calculate_miller_rabin_rounds(int bit_length); // Add this line
}

#endif // PRIME_UTILS_H
