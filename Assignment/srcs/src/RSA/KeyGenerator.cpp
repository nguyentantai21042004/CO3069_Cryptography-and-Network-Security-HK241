#include "KeyGenerator.h"
#include "../Core/PrimeUtils.h"
#include "../Utils/RandomGenerator.h"
#include "../Utils/Logger.h"

namespace KeyGenerator
{
    mpz_class generate_large_prime(int bit_length)
    {
        Logger::startTimer("Large Prime Generation");
        mpz_class prime;
        while (true)
        {
            unsigned int thread_id = 0; // Temporarily fixed to 0 for debugging
            prime = RandomGenerator::generate_random_prime_candidate(bit_length, thread_id);
            Logger::log("KeyGenerator", "Generated candidate for large prime: " + prime.get_str());

            if (PrimeUtils::is_prime(prime, bit_length))
                break;
        }
        Logger::endTimer("Large Prime Generation");
        return prime;
    }

    mpz_class generate_strong_prime(int bit_length)
    {
        Logger::startTimer("Strong Prime Generation");
        mpz_class prime;
        while (true)
        {
            unsigned int thread_id = 0; // Temporarily fixed to 0 for debugging
            prime = RandomGenerator::generate_random_prime_candidate(bit_length, thread_id);
            mpz_class q = (prime - 1) / 2;

            Logger::log("KeyGenerator", "Generated candidate for strong prime: " + prime.get_str());
            Logger::log("KeyGenerator", "Checking if q = (prime - 1) / 2 is prime: " + q.get_str());

            if (PrimeUtils::is_prime(prime, bit_length) && PrimeUtils::is_prime(q, bit_length / 2))
                break;
        }
        Logger::endTimer("Strong Prime Generation");
        return prime;
    }
}
