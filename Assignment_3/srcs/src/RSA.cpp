#include "RSA.h"

// Constructor for RSAKey
RSAKey::RSAKey(mpz_class p, mpz_class q) : p(p), q(q)
{
    if (p <= 1 || q <= 1 || p == q)
    {
        throw std::invalid_argument("Both p and q must be distinct prime numbers greater than 1.");
    }

    this->n = p * q;
    this->phi_n = (p - 1) * (q - 1);
    this->e = Generate_Public_Key();
    this->d = Generate_Private_Key();
}

// Print Public Key (n, e)
void RSAKey::Print_Public_Key()
{
    std::cout << "Public Key: (n: " << n << ", e: " << e << ")" << std::endl;
}

// Print Private Key (n, d)
void RSAKey::Print_Private_Key()
{
    std::cout << "Private Key: (n: " << n << ", d: " << d << ")" << std::endl;
}

// Generate Public Key
mpz_class RSAKey::Generate_Public_Key()
{
    mpz_class e_candidate = 65537; // Common choice for e
    if (GCD(e_candidate, phi_n) == 1)
    {
        return e_candidate;
    }
    throw std::runtime_error("Failed to find a suitable public exponent e.");
}

// Generate Private Key
mpz_class RSAKey::Generate_Private_Key()
{
    mpz_class x, y;
    By_Extended_Euclid(x, y, e, phi_n);
    // Ensure d is positive
    return (x < 0) ? (x + phi_n) : x;
}

// Simple GCD function
mpz_class RSAKey::GCD(const mpz_class &a, const mpz_class &b)
{
    return (b == 0) ? a : GCD(b, a % b);
}

// Extended Euclidean Algorithm
mpz_class RSAKey::By_Extended_Euclid(mpz_class &x, mpz_class &y, const mpz_class &a, const mpz_class &b)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    mpz_class x1, y1;
    mpz_class gcd = By_Extended_Euclid(x1, y1, b, a % b);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

// Encrypt a message using the public key (n, e)
mpz_class RSAEncryption::Encrypt(const std::string &message)
{
    // Convert the string message to a numerical representation
    mpz_class message_int = 0;

    // Iterate through each character in the message
    for (size_t i = 0; i < message.size(); ++i)
    {
        // Shift the existing value to the left by 8 bits (1 byte) and add the new character
        message_int = (message_int << 8) + static_cast<unsigned char>(message[i]);
    }

    // Encrypt the numerical representation
    mpz_class ciphertext;
    mpz_powm(ciphertext.get_mpz_t(), message_int.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
    return ciphertext;
}

// Decrypt a ciphertext using the private key (n, d)
std::string RSADecryption::Decrypt(const mpz_class &ciphertext)
{
    // Decrypt the ciphertext to get the numerical representation
    mpz_class decrypted_int;
    mpz_powm(decrypted_int.get_mpz_t(), ciphertext.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());

    // Convert the decrypted number back to a string
    std::string decrypted_message;
    // We will convert the number back to a string by extracting each byte
    while (decrypted_int > 0)
    {
        // Get the last byte (the last 8 bits) and append to the result
        unsigned char byte = decrypted_int.get_ui() & 0xFF; // Get the last byte
        decrypted_message.push_back(byte);
        // Shift right by 8 bits to remove the last byte
        decrypted_int >>= 8;
    }

    // Since we added bytes in reverse order, reverse the string
    std::reverse(decrypted_message.begin(), decrypted_message.end());
    return decrypted_message;
}
