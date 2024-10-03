#ifndef PRIME_GENERATOR_H
#define PRIME_GENERATOR_H

#include <gmpxx.h> // GNU MP library for large integers
#include <vector>
#include <chrono>
#include <mutex> // For thread synchronization
#include <atomic> // For atomic boolean flag

// Atomic flag to indicate if a prime number has been found
extern std::atomic<bool> found;
// Global variable to store the found prime number
extern mpz_class big_prime_integer;
// Mutex to prevent race conditions when accessing the global prime number
extern std::mutex mtx;

// Generates a random large integer of specified bit length
mpz_class Generate_Big_Integer(int bit_length);

// Computes (a^b) % n using the squared exponentiation method
void Squared_Exponentiation(mpz_class &result, const mpz_class &a, const mpz_class &b, const mpz_class &n);

// Performs Montgomery reduction, used for efficient modular arithmetic
void Montgomery_Reduce(mpz_class &t, const mpz_class &n, const mpz_class &n_inv, const mpz_class &R);

// Montgomery exponentiation algorithm for efficient modular exponentiation
void Montgomery_Exponentiation(mpz_class &result, const mpz_class &a, const mpz_class &b, const mpz_class &n);

// Returns the number of quick test iterations based on bit length and a constant factor
int Quick_Test_Iteration(int bit_length, double C);

// Performs a quick primality check by testing divisibility with small prime numbers
bool Quick_Check(const mpz_class &bigInteger, int iterations);

// Returns the number of Fermat test iterations based on bit length and a constant factor
int Fermat_Iterations(int bit_length, double C);

// Fermat primality test to check if a number is likely prime
bool Fermat_Test(const mpz_class &p, int iterations);

// Returns the number of Miller-Rabin iterations based on bit length and a constant factor
int Miller_Rabin_Iterations(int bit_length, double C);

// Miller-Rabin primality test to check if a number is likely prime
bool Miller_Rabin(const mpz_class &bigInteger, int iterations);

// Multi-threaded function to find a large prime number
void Find_Big_Prime(int n, std::chrono::steady_clock::time_point start_time);

// Generates and returns a large prime number using multiple threads
mpz_class Generate_Prime_Number(int n);

#endif // PRIME_GENERATOR_H
