#include "pemUtils.h"

#include "logger.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

namespace PemUtils
{
    // Base64 character table
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    // Base64 encoding function
    std::string base64_encode(const std::string &in)
    {
        std::string out;
        int val = 0, valb = -6;
        for (unsigned char c : in)
        {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0)
            {
                out.push_back(base64_chars[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6)
            out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
        while (out.size() % 4)
            out.push_back('=');
        return out;
    }

    // Base64 decoding function
    std::string base64_decode(const std::string &in)
    {
        std::vector<int> T(256, -1);
        for (int i = 0; i < 64; i++)
            T[base64_chars[i]] = i;

        std::string out;
        int val = 0, valb = -8;
        for (unsigned char c : in)
        {
            if (T[c] == -1)
                break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0)
            {
                out.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return out;
    }

    // Function to check if user keys exist and prompt for overwrite confirmation
    bool validate(const std::string &user)
    {
        // Create 'keys' directory if it does not exist
        if (!std::filesystem::exists("keys"))
        {
            std::filesystem::create_directory("keys");
            Logger::log("FileCheck", "Created 'keys' directory as it did not exist.");
            return true; // Directory 'keys' was created, no user key exists yet
        }

        std::string pub_key_path = "../keys/public_key_" + user + ".pem";
        std::string priv_key_path = "../keys/private_key_" + user + ".pem";

        // Check if keys for the specified user already exist
        if (std::filesystem::exists(pub_key_path) || std::filesystem::exists(priv_key_path))
        {
            Logger::log("FileCheck", "Keys for user " + user + " already exist.");
            std::cout << "Keys for user " << user << " already exist. Do you want to overwrite? (y/n): ";
            char response;
            std::cin >> response;

            if (response == 'y' || response == 'Y')
            {
                Logger::log("UserResponse", "User chose to overwrite existing keys for " + user + ".");
                return true; // User chose to overwrite
            }
            else
            {
                Logger::log("UserResponse", "User chose not to overwrite existing keys for " + user + ".");
                std::cout << "Aborted key generation for user " << user << ".\n";
                return false; // User chose not to overwrite
            }
        }

        Logger::log("FileCheck", "No existing keys for user " + user + ". Proceeding with key generation.");
        return true; // No keys exist for the user, continue with key generation
    }

    // Function to save public and private keys into separate `.pem` files
    void save_keys(const std::string &user, const mpz_class &e, const mpz_class &d, const mpz_class &n)
    {
        std::string pub_key_path = "../keys/public_key_" + user + ".pem";
        std::string priv_key_path = "../keys/private_key_" + user + ".pem";

        // Write public key to file
        std::ofstream pub_file(pub_key_path);
        if (pub_file.is_open())
        {
            pub_file << "-----BEGIN PUBLIC KEY-----\n";
            pub_file << base64_encode(e.get_str(16)) << "\n"; // Encode `e` to Base64
            pub_file << base64_encode(n.get_str(16)) << "\n"; // Encode `n` to Base64
            pub_file << "-----END PUBLIC KEY-----\n";
            pub_file.close();
            std::string log_msg = "Public key saved successfully to " + pub_key_path;
            std::cout << log_msg << ".\n";
            Logger::log("KeySave", log_msg);
        }
        else
        {
            std::string error_msg = "Failed to open public key file for writing.";
            std::cerr << error_msg << "\n";
            Logger::log("KeySaveError", error_msg);
            return; // Stop if unable to open the public key file
        }

        // Write private key to file
        std::ofstream priv_file(priv_key_path);
        if (priv_file.is_open())
        {
            priv_file << "-----BEGIN PRIVATE KEY-----\n";
            priv_file << base64_encode(d.get_str(16)) << "\n"; // Encode `d` to Base64
            priv_file << base64_encode(n.get_str(16)) << "\n"; // Encode `n` to Base64
            priv_file << "-----END PRIVATE KEY-----\n";
            priv_file.close();
            std::string log_msg = "Private key saved successfully to " + priv_key_path;
            std::cout << log_msg << ".\n";
            Logger::log("KeySave", log_msg);
        }
        else
        {
            std::string error_msg = "Failed to open private key file for writing.";
            std::cerr << error_msg << "\n";
            Logger::log("KeySaveError", error_msg);
        }
    }

    // Function to load the public key for a specified user
    bool load_public_key(const std::string &user, mpz_class &e, mpz_class &n)
    {
        std::ifstream pub_file("../keys/public_key_" + user + ".pem");
        if (!pub_file.is_open())
        {
            std::string error_msg = "Failed to open public key file for user: " + user;
            std::cerr << error_msg << "\n";
            Logger::log("LoadPublicKeyError", error_msg);
            return false;
        }

        std::string line, encoded_e, encoded_n;
        // Skip the first line (header)
        std::getline(pub_file, line);

        // Read `e` and `n` values from the file
        std::getline(pub_file, encoded_e);
        std::getline(pub_file, encoded_n);

        // Decode Base64 to large number `mpz_class`
        e = mpz_class(base64_decode(encoded_e), 16);
        n = mpz_class(base64_decode(encoded_n), 16);

        pub_file.close();
        Logger::log("LoadPublicKey", "Loaded public key for user: " + user);
        return true;
    }

    // Function to load the private key for a specified user
    bool load_private_key(const std::string &user, mpz_class &d, mpz_class &n)
    {
        std::string priv_key_path = "../keys/private_key_" + user + ".pem";
        std::ifstream priv_file(priv_key_path);

        if (!priv_file.is_open())
        {
            std::string error_msg = "Failed to open private key file for user: " + user;
            std::cerr << error_msg << "\n";
            Logger::log("LoadPrivateKeyError", error_msg);
            return false;
        }

        std::string line, encoded_d, encoded_n;
        // Skip the first line (header)
        std::getline(priv_file, line);

        // Read `d` and `n` values from the file
        std::getline(priv_file, encoded_d);
        std::getline(priv_file, encoded_n);

        // Decode Base64 to large number `mpz_class`
        d = mpz_class(base64_decode(encoded_d), 16);
        n = mpz_class(base64_decode(encoded_n), 16);

        priv_file.close();
        Logger::log("LoadPrivateKey", "Loaded private key for user: " + user);
        return true;
    }
}
