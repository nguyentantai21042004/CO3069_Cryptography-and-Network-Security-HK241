#include "RSA.h"
#include "PrimeGenerator.h"

RSAKey::RSAKey(mpz_class p, mpz_class q)
{
    this->p = p;
    this->q = q;
    this->n = p * q;
    this->phi_n = (p - 1) * (q - 1);
    this->e = Generate_Public_Key();
    this->d = Generate_Private_Key();
}

void RSAKey::Print_Public_Key()
{
    std::cout << "Public Key = (n, e) = (" << n << ", " << e << ")\n";
}

void RSAKey::Print_Private_Key()
{
    if (d != -1)
    {
        std::cout << "Private Key = (n, d) = (" << n << ", " << d << ")\n";
    }
    else
    {
        std::cerr << "Error: Unable to generate private key!\n";
    }
}

mpz_class RSAKey::Generate_Public_Key()
{
    mpz_class public_key = 65537; // Common values ​​for public key e

    /* Check if gcd(e, phi_n) = 1 */
    mpz_class result = this->GCD(public_key, phi_n);

    if (result != 1)
    {
        // If 65537 is not coprime with phi(n), choose a different value for e
        std::cerr << "Error: 65537 is not coprime with phi(n). Choosing a different e.\n";
        public_key = 3;
        while (this->GCD(public_key, phi_n) != 1)
        {
            public_key += 2; // Increment e by 2 (next odd number) until a suitable e is found
        }
    }

    return public_key;
}

mpz_class RSAKey::Generate_Private_Key()
{
    mpz_class x, y;
    if (By_Extended_Euclid(x, y, e, phi_n) != 1)
    {
        // If e and phi(n) are not coprime, private key d does not exist
        std::cerr << "Error: e is not coprime with phi(n). Private Key d does not exist.\n";
        return -1;
    }

    mpz_class private_key = (x % phi_n + phi_n) % phi_n; // Calculate d from e and phi_n
    return private_key;
}

mpz_class RSAKey::GCD(const mpz_class &a, const mpz_class &b)
{
    mpz_class a_copy = a;
    mpz_class b_copy = b;

    while (b_copy != 0)
    {
        mpz_class temp = b_copy;
        b_copy = a_copy % b_copy;
        a_copy = temp;
    }

    return a_copy;
}

mpz_class RSAKey::By_Extended_Euclid(mpz_class &x, mpz_class &y, const mpz_class &a, const mpz_class &b)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a; // Base case: if b is 0, gcd(a, b) = a
    }
    else
    {
        mpz_class x1, y1;
        mpz_class q = a / b; // Quotient
        mpz_class r = a % b; // Remainder

        // Recursive call
        mpz_class result = By_Extended_Euclid(x1, y1, b, r);

        x = y1;
        y = x1 - q * y1;

        return result;
    }
}

mpz_class RSAEncryption::Encrypt(const std::string &encoded_message)
{
    mpz_class m(encoded_message, 10); // Convert the ASCII-encoded string to a large number
    mpz_class ciphertext;

    // Perform Montgomery Exponentiation for encryption: ciphertext = m^e mod n
    Montgomery_Exponentiation(ciphertext, m, e, n);

    return ciphertext; // Return the encrypted ciphertext
}

std::string RSADecryption::Decrypt(const mpz_class &ciphertext)
{
    mpz_class m;
    // Perform Montgomery Exponentiation for decryption: m = ciphertext^d mod n
    Montgomery_Exponentiation(m, ciphertext, d, n);
    return m.get_str(); // Convert the decrypted value back to a string and return it
}