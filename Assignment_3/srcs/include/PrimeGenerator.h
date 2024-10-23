#ifndef PRIME_GENERATOR_H
#define PRIME_GENERATOR_H

#include <gmpxx.h>
#include <random>
#include <ctime>

// Function to perform modular exponentiation using the method of exponentiation by squaring
// Time Complexity: O(log(exp))
mpz_class Exponentiation_By_Squaring(const mpz_class &base, const mpz_class &exp, const mpz_class &mod);

// Function to generate a random large integer with a specified number of bits
// Time Complexity: O(bits_length)
mpz_class Generate_Random_Large_Integer(int bits_length, gmp_randclass &rand_gen);

// Function to perform primality testing using the Fermat method
// Time Complexity: O(k * log(n)), where k is the number of iterations and n is the size of the number
bool Fermat_Test(const mpz_class &bigInteger, int iterations);

// Function to perform primality testing using the Miller-Rabin method
// Time Complexity: O(k * log(n)), where k is the number of iterations and n is the size of the number
bool Miller_Rabin(const mpz_class &bigInteger, int iterations, gmp_randclass &rand_gen);

// General function to check if a number is prime
// Time Complexity: O(log(n) * (k + m)), where n is the number, k is the number of iterations for Fermat, and m is for Miller-Rabin
bool Is_Prime_Integer(const mpz_class &bigInteger, int bits_length, gmp_randclass &rand_gen);

// BigInteger class for handling large integers
class BigInteger
{
public:
    // Constructor
    BigInteger();

    // Function to find a large prime number with a specified number of bits
    // Time Complexity: Depends on primality tests, typically O(log(n) * (k + m))
    static BigInteger Find_Prime_Number(int bits, gmp_randclass &rand_gen);

    // Getter for the value of the BigInteger
    mpz_class get_value() const;

private:
    mpz_class value; // Using mpz_class from GMP for large integer representation
};

// Function to generate a prime number for the receiver
// Time Complexity: O(log(n) * (k + m))
mpz_class Generate_Prime_Number(int bits_length, gmp_randclass &rand_gen);

#endif // PRIME_GENERATOR_H
