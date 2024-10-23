#ifndef RSAKEY_H
#define RSAKEY_H

#include <iostream>
#include <string>
#include <gmpxx.h>

class RSAKey
{
public:
    // Constructor to generate Public and Private keys
    RSAKey(mpz_class p, mpz_class q);

    // Print Public Key (n, e)
    void Print_Public_Key();

    // Print Private Key (n, d)
    void Print_Private_Key();

    // Getters for public and private key components
    mpz_class Get_Value_Of_N() const { return this->n; }
    mpz_class Get_Value_Of_Phi_Euler() const { return this->phi_n; }
    mpz_class Get_Value_Of_P() const { return this->p; }
    mpz_class Get_Value_Of_Q() const { return this->q; }
    mpz_class Get_Public_Key() const { return this->e; }
    mpz_class Get_Private_Key() const { return this->d; }

protected:
    // Generate Public Key
    mpz_class Generate_Public_Key();

    // Generate Private Key
    mpz_class Generate_Private_Key();

private:
    mpz_class p, q;     // Prime numbers
    mpz_class n, phi_n; // n and value of Phi Euler
    mpz_class e, d;     // Public key and Private key

    // Simple GCD function to find the greatest common divisor of a and b
    mpz_class GCD(const mpz_class &a, const mpz_class &b);

    // Extended Euclidean Algorithm to find the gcd and coefficients
    mpz_class By_Extended_Euclid(mpz_class &x, mpz_class &y, const mpz_class &a, const mpz_class &b);
};

class RSAEncryption
{
public:
    RSAEncryption(const mpz_class &n, const mpz_class &e) : n(n), e(e) {}

    // Encrypt a message using the public key (n, e)
    mpz_class Encrypt(const std::string &message);

private:
    mpz_class n, e; // Public Key of Sender
};

class RSADecryption
{
public:
    RSADecryption(const mpz_class &n, const mpz_class &d) : n(n), d(d) {}

    // Decrypt a ciphertext using the private key (n, d)
    std::string Decrypt(const mpz_class &ciphertext);

private:
    mpz_class n, d; // Private Key of Sender
};

#endif
