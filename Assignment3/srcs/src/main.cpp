#include <iostream>
#include <string>
#include <filesystem>

#include "rsa/rsaOperations.h"
#include "utils/pemUtils.h"
#include "utils/logger.h"

void generate_keys();
void encrypt_message();
void decrypt_message();
void check_keys();
void list_keys();

void display_menu()
{
    std::cout << "\nSelect an operation:\n";
    std::cout << "1. Generate Key Pair\n";
    std::cout << "2. Encrypt Message\n";
    std::cout << "3. Decrypt Message\n";
    std::cout << "4. Check if Keys Exist\n";
    std::cout << "5. List All Keys\n";
    std::cout << "0. Exit\n";
}

int main()
{
    int choice;
    bool exit_program = false;

    while (!exit_program)
    {
        display_menu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Logger::log("Main", "Starting key pair generation...");
            generate_keys();
            break;
        }
        case 2:
        {
            Logger::log("Main", "Starting message encryption...");
            encrypt_message();
            break;
        }
        case 3:
        {
            Logger::log("Main", "Starting message decryption...");
            decrypt_message();
            break;
        }
        case 4:
        {
            Logger::log("Main", "Checking if keys exist...");
            check_keys();
            break;
        }
        case 5:
            Logger::log("Main", "Listing all keys...");
            list_keys();
            break;
        case 0:
            exit_program = true;
            Logger::log("Main", "Exiting program.");
            break;
        default:
            Logger::log("Main", "Invalid choice, please try again.");
            break;
        }
    }

    return 0;
}

// Define the function for generating a key pair
void generate_keys()
{
    int keysize;
    std::string user;
    std::cout << "Enter key size (at least 500 bits): ";
    std::cin >> keysize;
    std::cout << "Enter username for the key pair: ";
    std::cin >> user;

    if (!PemUtils::validate(user))
    {
        Logger::log("Main", "Failed to create new user with username: " + user);
        return;
    }

    RSAOperations::generate_rsa_keys(user, keysize);
    Logger::log("Main", "Key pair generated successfully for user: " + user);
}

// Define the function for encrypting a message
void encrypt_message()
{
    std::string message, user;
    std::cout << "Enter message to encrypt: ";
    std::cin.ignore(); // Clear remaining newline after input
    std::getline(std::cin, message);
    std::cout << "Enter recipient username for encryption: ";
    std::cin >> user;

    // Load the recipient's public key
    mpz_class e, n;
    if (!PemUtils::load_public_key(user, e, n))
    {
        std::cerr << "Failed to load public key for encryption.\n";
        Logger::log("Encrypt", "Failed to load recipient's public key.");
        return;
    }

    // Convert the message to a large number
    mpz_class message_mpz;
    RSAOperations::convert_message_to_number(message, message_mpz);

    // Encrypt the message
    mpz_class encrypted_message;
    RSAOperations::encrypt_number(message_mpz, e, n, encrypted_message);

    // Create a digital signature
    std::string sender;
    std::cout << "Enter sender username for signing: ";
    std::cin >> sender;

    mpz_class d_sender, n_sender;
    if (!PemUtils::load_private_key(sender, d_sender, n_sender))
    {
        std::cerr << "Failed to load private key for signing.\n";
        Logger::log("Encrypt", "Failed to load sender's private key.");
        return;
    }

    mpz_class signature;
    RSAOperations::sign_number(message_mpz, d_sender, n_sender, signature);

    // Save the encrypted message and signature to files
    RSAOperations::save_encrypted_message(user, encrypted_message);
    RSAOperations::save_signature(user, signature);

    Logger::log("Encrypt", "Message encrypted and signed successfully.");
}

// Define the function for decrypting a message
void decrypt_message()
{
    std::string user;
    std::cout << "Enter username to decrypt the message: ";
    std::cin >> user;

    // Load the recipient's private key
    mpz_class d, n;
    if (!PemUtils::load_private_key(user, d, n))
    {
        std::cerr << "Failed to load private key for user: " << user << "\n";
        Logger::log("Decrypt", "Failed to load private key.");
        return;
    }

    // Load the encrypted message from file
    mpz_class encrypted_message;
    if (!RSAOperations::load_encrypted_message(user, encrypted_message))
    {
        std::cerr << "Failed to load encrypted message for user: " << user << "\n";
        Logger::log("Decrypt", "Failed to load encrypted message.");
        return;
    }

    // Decrypt the message
    mpz_class decrypted_message;
    RSAOperations::decrypt_number(encrypted_message, d, n, decrypted_message);

    // Convert the large number back to text
    std::string message = RSAOperations::convert_number_to_message(decrypted_message);
    std::cout << "Decrypted Message: " << message << "\n";

    // Verify the digital signature
    std::string sender;
    std::cout << "Enter sender username to verify signature: ";
    std::cin >> sender;

    mpz_class e_sender, n_sender;
    if (!PemUtils::load_public_key(sender, e_sender, n_sender))
    {
        std::cerr << "Failed to load public key for verification.\n";
        Logger::log("Decrypt", "Failed to load sender's public key.");
        return;
    }

    mpz_class signature;
    if (!RSAOperations::load_signature(user, signature))
    {
        std::cerr << "Failed to load signature for verification.\n";
        Logger::log("Decrypt", "Failed to load signature for verification.");
        return;
    }

    mpz_class message_mpz;
    RSAOperations::convert_message_to_number(message, message_mpz);

    bool is_valid = RSAOperations::verify_number_signature(message_mpz, signature, e_sender, n_sender);

    if (is_valid)
    {
        std::cout << "Signature is valid. Message verified successfully.\n";
        Logger::log("Decrypt", "Signature is valid, message verified successfully.");
    }
    else
    {
        std::cout << "Signature is invalid. Message verification failed.\n";
        Logger::log("Decrypt", "Signature is invalid, verification failed.");
    }
}

// Check if the user's key files exist
void check_keys()
{
    std::string user;
    std::cout << "Enter username to check keys: ";
    std::cin >> user;

    // Path to the public and private key files
    std::string pub_key_path = "../keys/public_key_" + user + ".pem";
    std::string priv_key_path = "../keys/private_key_" + user + ".pem";

    // Check for the existence of the key files
    bool pub_key_exists = std::filesystem::exists(pub_key_path);
    bool priv_key_exists = std::filesystem::exists(priv_key_path);

    if (pub_key_exists && priv_key_exists)
    {
        std::cout << "Keys for user " << user << " exist.\n";
        Logger::log("CheckKeys", "Key pair exists for user: " + user);
    }
    else
    {
        if (!pub_key_exists)
            std::cout << "Public key for user " << user << " does not exist.\n";
        if (!priv_key_exists)
            std::cout << "Private key for user " << user << " does not exist.\n";

        Logger::log("CheckKeys", "Incomplete key pair for user: " + user);
    }
}

// List all the keys in the directory
void list_keys()
{
    std::string keys_directory = "../keys";
    bool keys_found = false;

    std::cout << "Listing all keys in the 'keys' directory:\n";
    Logger::log("ListKeys", "Starting to list keys in 'keys' directory.");

    for (const auto &entry : std::filesystem::directory_iterator(keys_directory))
    {
        if (entry.path().extension() == ".pem")
        {
            std::cout << entry.path().filename().string() << "\n";
            keys_found = true;
        }
    }

    if (!keys_found)
    {
        std::cout << "No keys found in the 'keys' directory.\n";
        Logger::log("ListKeys", "No keys found.");
    }
    else
    {
        Logger::log("ListKeys", "Key listing completed.");
    }
}
