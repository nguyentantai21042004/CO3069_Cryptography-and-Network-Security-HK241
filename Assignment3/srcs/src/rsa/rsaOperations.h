#ifndef RSA_OPERATIONS_H
#define RSA_OPERATIONS_H

#include <string>
#include <gmpxx.h>

namespace RSAOperations
{
    void generate_rsa_keys(const std::string &user, int bit_length);
    void convert_message_to_number(const std::string &message, mpz_class &message_mpz);
    std::string convert_number_to_message(const mpz_class &decrypted_message);
    void encrypt_number(const mpz_class &message_mpz, const mpz_class &e, const mpz_class &n, mpz_class &encrypted_message);
    void decrypt_number(const mpz_class &encrypted_message, const mpz_class &d, const mpz_class &n, mpz_class &decrypted_message);
    void save_encrypted_message(const std::string &user, const mpz_class &encrypted_message);
    bool load_encrypted_message(const std::string &user, mpz_class &encrypted_message);
    void sign_number(const mpz_class &message, const mpz_class &private_key, const mpz_class &n, mpz_class &signature);
    bool verify_number_signature(const mpz_class &message, const mpz_class &signature, const mpz_class &public_key, const mpz_class &n);
    void save_signature(const std::string &user, const mpz_class &signature);
    bool load_signature(const std::string &user, mpz_class &signature);
}

#endif // RSA_OPERATIONS_H
