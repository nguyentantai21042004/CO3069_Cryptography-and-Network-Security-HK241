#include "RSA/KeyGenerator.h"
#include "RSA/RSAOperations.h"
#include "Utils/Logger.h"
#include <gmpxx.h>
#include <iostream>
#include <string>
#include <fstream>

void save_keys(const mpz_class &e, const mpz_class &d, const mpz_class &n);
bool load_keys(mpz_class &e, mpz_class &d, mpz_class &n);
void clear_console();
bool is_valid_bit_length(int bit_length);

int main()
{
    int choice;
    bool exit_program = false;
    mpz_class e, d, n;

    while (!exit_program)
    {
        std::cout << "Select an operation:\n";
        std::cout << "1. Generate Key Pair\n";
        std::cout << "2. Encrypt Message\n";
        std::cout << "3. Decrypt Message\n";
        std::cout << "4. Sign Message\n";
        std::cout << "5. Verify Signature\n";
        std::cout << "6. Save Keys to File\n";
        std::cout << "7. Load Keys from File\n";
        std::cout << "8. Demo RSA Workflow\n";
        std::cout << "0. Exit\n";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int bit_length;
            std::cout << "Enter bit length for the key: ";
            std::cin >> bit_length;

            if (!is_valid_bit_length(bit_length))
            {
                std::cout << "Invalid bit length. Must be between 512 and 4096.\n";
                break;
            }

            RSAOperations::generate_rsa_keys(bit_length, e, d, n);
            Logger::log("Key Generation", "Keys generated successfully.");
            std::cout << "Public Key (e, n): (" << e << ", " << n << ")\n";
            std::cout << "Private Key (d, n): (" << d << ", " << n << ")\n";
            break;
        }
        case 2:
        {
            if (e == 0 || n == 0)
            {
                std::cout << "Generate keys first.\n";
                break;
            }
            std::string message;
            std::cout << "Enter message to encrypt: ";
            std::cin.ignore();
            std::getline(std::cin, message);

            mpz_class message_mpz = RSAOperations::text_to_mpz(message);
            mpz_class encrypted_message = RSAOperations::encrypt(message_mpz, e, n);
            std::cout << "Encrypted Message: " << encrypted_message << std::endl;
            break;
        }
        case 3:
        {
            if (d == 0 || n == 0)
            {
                std::cout << "Generate keys first.\n";
                break;
            }

            mpz_class encrypted_message;
            std::cout << "Enter encrypted message as integer: ";
            std::cin >> encrypted_message;

            mpz_class decrypted_message_mpz = RSAOperations::decrypt(encrypted_message, d, n);
            std::string decrypted_text = RSAOperations::mpz_to_text(decrypted_message_mpz);
            std::cout << "Decrypted Message: " << decrypted_text << std::endl;
            break;
        }
        case 6:
        {
            save_keys(e, d, n);
            break;
        }
        case 7:
        {
            if (!load_keys(e, d, n))
            {
                std::cout << "Failed to load keys. Generate or save keys first.\n";
            }
            break;
        }
        case 8:
        {
            // Demo RSA workflow with sample message
            std::string sample_message = "Hello, RSA!";
            std::cout << "Sample Message: " << sample_message << "\n";

            mpz_class encoded_message = RSAOperations::text_to_mpz(sample_message);
            mpz_class encrypted_message = RSAOperations::encrypt(encoded_message, e, n);
            std::cout << "Encrypted: " << encrypted_message << "\n";

            mpz_class decrypted_message_mpz = RSAOperations::decrypt(encrypted_message, d, n);
            std::string decrypted_message = RSAOperations::mpz_to_text(decrypted_message_mpz);
            std::cout << "Decrypted: " << decrypted_message << "\n";

            mpz_class signature = RSAOperations::sign_message(encoded_message, d, n);
            bool is_valid = RSAOperations::verify_signature(encoded_message, signature, e, n);
            std::cout << "Signature verification " << (is_valid ? "successful" : "failed") << std::endl;
            break;
        }
        case 0:
        {
            exit_program = true;
            std::cout << "Exiting program." << std::endl;
            break;
        }
        default:
            std::cout << "Invalid choice, please try again." << std::endl;
        }
        clear_console();
    }
    return 0;
}

// Implement helper functions
