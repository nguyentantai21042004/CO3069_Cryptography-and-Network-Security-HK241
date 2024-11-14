#include "RSAOperations.h"
#include "KeyGenerator.h"
#include <gmpxx.h>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace RSAOperations
{

    void generate_rsa_keys(int bit_length, mpz_class &e, mpz_class &d, mpz_class &n)
    {
        // Generate two large primes p and q
        mpz_class p = KeyGenerator::generate_large_prime(bit_length / 2);
        mpz_class q = KeyGenerator::generate_large_prime(bit_length / 2);

        n = p * q;
        mpz_class phi = (p - 1) * (q - 1);

        // Choose a commonly used public exponent
        e = 65537;
        if (gcd(e, phi) != 1)
        {
            do
            {
                e += 2; // Ensure 'e' is coprime with 'phi'
            } while (gcd(e, phi) != 1);
        }

        // Compute private key 'd' as the modular inverse of 'e' modulo 'phi'
        if (mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t()) == 0)
        {
            throw std::runtime_error("Failed to compute modular inverse for private key");
        }
    }

    mpz_class sign_message(const mpz_class &message, const mpz_class &private_key, const mpz_class &n)
    {
        mpz_class signature;
        mpz_powm(signature.get_mpz_t(), message.get_mpz_t(), private_key.get_mpz_t(), n.get_mpz_t());
        return signature;
    }

    bool verify_signature(const mpz_class &message, const mpz_class &signature, const mpz_class &public_key, const mpz_class &n)
    {
        mpz_class verified_message;
        mpz_powm(verified_message.get_mpz_t(), signature.get_mpz_t(), public_key.get_mpz_t(), n.get_mpz_t());
        return message == verified_message;
    }

    mpz_class encrypt(const mpz_class &message, const mpz_class &public_key, const mpz_class &n)
    {
        mpz_class ciphertext;
        mpz_powm(ciphertext.get_mpz_t(), message.get_mpz_t(), public_key.get_mpz_t(), n.get_mpz_t());
        return ciphertext;
    }

    mpz_class decrypt(const mpz_class &ciphertext, const mpz_class &private_key, const mpz_class &n)
    {
        mpz_class message;
        mpz_powm(message.get_mpz_t(), ciphertext.get_mpz_t(), private_key.get_mpz_t(), n.get_mpz_t());
        return message;
    }

    mpz_class text_to_mpz(const std::string &text)
    {
        std::ostringstream hex_stream;
        for (unsigned char ch : text)
        {
            hex_stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
        }
        return mpz_class(hex_stream.str(), 16);
    }

    std::string mpz_to_text(const mpz_class &number)
    {
        std::string hex_str = number.get_str(16);
        if (hex_str.size() % 2 != 0)
            hex_str = "0" + hex_str;

        std::string text;
        for (size_t i = 0; i < hex_str.size(); i += 2)
        {
            std::string byte_hex = hex_str.substr(i, 2);
            char ch = static_cast<char>(std::stoi(byte_hex, nullptr, 16));
            text.push_back(ch);
        }
        return text;
    }
}
