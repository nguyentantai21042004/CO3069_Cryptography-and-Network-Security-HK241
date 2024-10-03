#ifndef GREATEST_COMMON_DIVISOR_H
#define GREATEST_COMMON_DIVISOR_H

#include <gmpxx.h>

// Method 1: Find GCD using subtraction
// Complexity: O(max(a, b))
mpz_class By_Subtraction(mpz_class a, mpz_class b);

// Method 2: Find GCD using remainder division
// Complexity: O(log(min(a, b)))
mpz_class By_Remainder_Division(mpz_class a, mpz_class b);

// Method 3: Find GCD using Euclid's algorithm (optimized)
// Complexity: O(log(min(a, b)))
mpz_class By_Euclid_Algorithm(mpz_class a, mpz_class b);

// Method 4: Find GCD using the Extended Euclidean Algorithm
// Complexity: O(log(min(a, b)))
mpz_class By_Extended_Euclid(mpz_class &x, mpz_class &y, const mpz_class &a, const mpz_class &b);

// Main function: Computes the GCD of two numbers using the extended Euclidean algorithm
// Complexity: O(log(min(a, b)))
mpz_class GCD(const mpz_class &a, const mpz_class &b);

#endif // GREATEST_COMMON_DIVISOR_H
