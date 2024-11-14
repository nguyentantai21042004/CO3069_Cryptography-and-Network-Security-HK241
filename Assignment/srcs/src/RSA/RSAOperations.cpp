#include "rsaOperations.h"
#include "KeyGenerator.h"
#include "../utils/pemUtils.h"
#include "../utils/randomGenerator.h"
#include "../utils/logger.h"

#include <gmpxx.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>

namespace RSAOperations
{
    // Generate RSA key pair and save as `.pem` files
    void generate_rsa_keys(const std::string &user, int bit_length)
    {
        mpz_class e, d, n;

        // Step 1: Generate two large primes p and q
        mpz_class p = KeyGenerator::generate_large_prime(bit_length / 2);
        mpz_class q = KeyGenerator::generate_large_prime(bit_length / 2);

        // Step 2: Compute n = p * q and phi = (p - 1) * (q - 1)
        n = p * q;
        mpz_class phi = (p - 1) * (q - 1);

        // Step 3: Choose e, a common public exponent, ensuring e and phi are coprime
        e = 65537; // Common choice
        if (gcd(e, phi) != 1)
        {
            do
            {
                e += 2; // Increment e to ensure it is coprime with phi
            } while (gcd(e, phi) != 1);
        }

        // Step 4: Compute private key d as modular inverse of e mod phi
        if (mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t()) == 0)
        {
            throw std::runtime_error("Failed to compute modular inverse for private key");
        }

        // Step 5: Save the key values into `.pem` files named according to user
        PemUtils::save_keys(user, e, d, n);

        Logger::log("RSAOperations", "Keys generated and saved successfully for user: " + user);
    }

    // Convert message string to a large number
    void convert_message_to_number(const std::string &message, mpz_class &message_mpz)
    {
        std::ostringstream hex_stream;
        for (unsigned char ch : message)
        {
            hex_stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
        }
        message_mpz.set_str(hex_stream.str(), 16);
    }

    // Convert a large number back to a message string
    std::string convert_number_to_message(const mpz_class &decrypted_message)
    {
        // Convert large number to hexadecimal string
        std::string hex_str = decrypted_message.get_str(16);

        // If odd length, add '0' to the beginning for full pairs
        if (hex_str.size() % 2 != 0)
        {
            hex_str = "0" + hex_str;
        }

        // Convert hex string to text string
        std::string text;
        for (size_t i = 0; i < hex_str.size(); i += 2)
        {
            // Take each pair of hexadecimal characters
            std::string byte_hex = hex_str.substr(i, 2);
            // Convert hex pair to character
            char ch = static_cast<char>(std::stoi(byte_hex, nullptr, 16));
            text.push_back(ch);
        }

        return text;
    }

    // Encrypt a large number using public exponent e and modulus n
    void encrypt_number(const mpz_class &message_mpz, const mpz_class &e, const mpz_class &n, mpz_class &encrypted_message)
    {
        encrypted_message = RandomGenerator::mod_exp(message_mpz, e, n);
    }

    // Decrypt a large number using private exponent d and modulus n
    void decrypt_number(const mpz_class &encrypted_message, const mpz_class &d, const mpz_class &n, mpz_class &decrypted_message)
    {
        decrypted_message = RandomGenerator::mod_exp(encrypted_message, d, n);
    }

    // Save encrypted message to file in hexadecimal format
    void save_encrypted_message(const std::string &user, const mpz_class &encrypted_message)
    {
        std::string folder_path = "../encrypted_message";
        std::string filename = folder_path + "/encrypted_message_" + user + ".txt";

        // Check if folder exists, create if it does not
        if (!std::filesystem::exists(folder_path))
        {
            if (!std::filesystem::create_directory(folder_path))
            {
                Logger::log("RSAOperations", "Failed to create directory: " + folder_path);
                return;
            }
        }

        // Open file to write encrypted message
        std::ofstream out_file(filename);
        if (out_file.is_open())
        {
            out_file << encrypted_message.get_str(16); // Save as hexadecimal
            out_file.close();
            Logger::log("RSAOperations", "Encrypted message saved to " + filename);
        }
        else
        {
            Logger::log("RSAOperations", "Failed to save encrypted message to " + filename);
        }
    }

    // Load encrypted message from file
    bool load_encrypted_message(const std::string &user, mpz_class &encrypted_message)
    {
        std::string filename = "../encrypted_message/encrypted_message_" + user + ".txt";
        std::ifstream in_file(filename);

        if (!in_file.is_open())
        {
            Logger::log("RSAOperations", "Failed to open encrypted message file for user: " + user);
            return false;
        }

        std::string encrypted_hex;
        in_file >> encrypted_hex;                         // Read the encrypted content in hex
        encrypted_message = mpz_class(encrypted_hex, 16); // Convert from hex to large number
        in_file.close();

        return true;
    }

    // Sign a large number (message) with a private key
    void sign_number(const mpz_class &message, const mpz_class &private_key, const mpz_class &n, mpz_class &signature)
    {
        // Signature = message^private_key mod n
        mpz_powm(signature.get_mpz_t(), message.get_mpz_t(), private_key.get_mpz_t(), n.get_mpz_t());
    }

    // Verify a digital signature against the original message and public key
    bool verify_number_signature(const mpz_class &message, const mpz_class &signature, const mpz_class &public_key, const mpz_class &n)
    {
        // Decrypt signature: verified_message = signature^public_key mod n
        mpz_class verified_message;
        mpz_powm(verified_message.get_mpz_t(), signature.get_mpz_t(), public_key.get_mpz_t(), n.get_mpz_t());

        // Compare result with the original message
        return message == verified_message;
    }

    // Save digital signature to file in hexadecimal format
    void save_signature(const std::string &user, const mpz_class &signature)
    {
        std::string folder_path = "../signatures";
        std::string filename = folder_path + "/signature_" + user + ".txt";

        // Check and create folder if it does not exist
        if (!std::filesystem::exists(folder_path))
        {
            std::filesystem::create_directory(folder_path);
        }

        // Save the signature to file
        std::ofstream out_file(filename);
        if (out_file.is_open())
        {
            out_file << signature.get_str(16); // Save signature as hexadecimal
            out_file.close();
            Logger::log("RSAOperations", "Signature saved to " + filename);
        }
        else
        {
            Logger::log("RSAOperations", "Failed to save signature to " + filename);
        }
    }

    // Load digital signature from file
    bool load_signature(const std::string &user, mpz_class &signature)
    {
        std::string filename = "../signatures/signature_" + user + ".txt";

        // Open signature file
        std::ifstream in_file(filename);
        if (!in_file.is_open())
        {
            Logger::log("RSAOperations", "Failed to open signature file: " + filename);
            return false;
        }

        // Read hexadecimal signature from file
        std::string signature_hex;
        in_file >> signature_hex;
        signature = mpz_class(signature_hex, 16); // Convert from hex to large number
        in_file.close();

        return true;
    }
}
