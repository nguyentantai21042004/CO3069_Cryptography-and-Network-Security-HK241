#ifndef RSA_OPERATIONS_H
#define RSA_OPERATIONS_H

#include <gmpxx.h>
#include <string>

namespace RSAOperations
{
    void generate_rsa_keys(int bit_length, mpz_class &e, mpz_class &d, mpz_class &n);
    mpz_class sign_message(const mpz_class &message, const mpz_class &private_key, const mpz_class &n);
    bool verify_signature(const mpz_class &message, const mpz_class &signature, const mpz_class &public_key, const mpz_class &n);
    mpz_class encrypt(const mpz_class &message, const mpz_class &public_key, const mpz_class &n);
    mpz_class decrypt(const mpz_class &ciphertext, const mpz_class &private_key, const mpz_class &n);
    mpz_class text_to_mpz(const std::string &text);
    std::string mpz_to_text(const mpz_class &number);
}

#endif // RSA_OPERATIONS_H
