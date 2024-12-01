#ifndef PEM_UTILS_H
#define PEM_UTILS_H

#include <gmpxx.h>
#include <string>

struct RSAKey
{
    std::string type;
    mpz_class e;
    mpz_class d;
    mpz_class n;
};

namespace PemUtils
{
    std::string base64_encode(const std::string &in);
    std::string base64_decode(const std::string &in);
    bool validate(const std::string &user);
    void save_keys(const std::string &user, const mpz_class &e, const mpz_class &d, const mpz_class &n);
    bool load_public_key(const std::string &user, mpz_class &e, mpz_class &n);
    bool load_private_key(const std::string &user, mpz_class &d, mpz_class &n);
    // void read_key(const std::string &user);

    void read_key(const std::string &user);
    bool read_pem_key(const std::string &filepath, RSAKey &key);
}

#endif // PEM_UTILS_H