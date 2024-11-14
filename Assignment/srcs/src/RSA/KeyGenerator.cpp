#include "KeyGenerator.h"
#include "../core/primeUtils.h"
#include "../utils/randomGenerator.h"
#include "../utils/logger.h"

namespace KeyGenerator
{
    // Function to generate a large prime number of specified bit length
    mpz_class generate_large_prime(int bit_length)
    {
        Logger::startTimer("Large Prime Generation"); // Start timer for large prime generation
        mpz_class prime;

        while (true)
        {
            unsigned int thread_id = 0; // Temporarily fixed to 0 for debugging
            prime = RandomGenerator::generate_random_prime_candidate(bit_length, thread_id);

            // Log candidate generation only once per loop to avoid excessive logs
            Logger::log("KeyGenerator", "Generated candidate for large prime of bit length " + std::to_string(bit_length));

            // Check if the generated candidate is prime
            if (PrimeUtils::is_prime(prime, bit_length))
                break; // Exit loop if a prime is found
        }

        Logger::endTimer("Large Prime Generation"); // End timer
        return prime;
    }

    // Function to generate a strong prime number of specified bit length
    mpz_class generate_strong_prime(int bit_length)
    {
        Logger::startTimer("Strong Prime Generation"); // Start timer for strong prime generation
        mpz_class prime;

        while (true)
        {
            unsigned int thread_id = 0; // Temporarily fixed to 0 for debugging
            prime = RandomGenerator::generate_random_prime_candidate(bit_length, thread_id);
            mpz_class q = (prime - 1) / 2; // Calculate q for strong prime test

            // Log candidate generation and the corresponding `q` value
            Logger::log("KeyGenerator", "Generated candidate for strong prime: " + prime.get_str());
            Logger::log("KeyGenerator", "Checking if q = (prime - 1) / 2 is prime, q = " + q.get_str());

            // Check if both `prime` and `q` are prime for a strong prime
            if (PrimeUtils::is_prime(prime, bit_length) && PrimeUtils::is_prime(q, bit_length / 2))
                break; // Exit loop if a strong prime is found
        }

        Logger::endTimer("Strong Prime Generation"); // End timer
        return prime;
    }
}
