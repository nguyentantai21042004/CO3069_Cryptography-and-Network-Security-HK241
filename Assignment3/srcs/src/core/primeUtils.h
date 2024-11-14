#ifndef PRIME_UTILS_H
#define PRIME_UTILS_H

#include <gmpxx.h>

namespace PrimeUtils
{
    int calculate_miller_rabin_rounds(int bit_length); // Add this line

    bool quick_test(const mpz_class &n);
    bool miller_rabin_test(const mpz_class &n, int rounds);
    bool is_prime(const mpz_class &n, int bit_length);
}

#endif // PRIME_UTILS_H
