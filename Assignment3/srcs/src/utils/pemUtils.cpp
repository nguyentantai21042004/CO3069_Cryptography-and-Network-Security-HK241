#include "pemUtils.h"
#include "logger.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace PemUtils
{
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

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
        Logger::log("Base64Decode", "Base64 decoded data: " + out);
        return out;
    }

    bool validate(const std::string &user)
    {
        if (!std::filesystem::exists("keys"))
        {
            std::filesystem::create_directory("keys");
            Logger::log("FileCheck", "Created 'keys' directory as it did not exist.");
            return true;
        }

        std::string pub_key_path = "../keys/public_key_" + user + ".pem";
        std::string priv_key_path = "../keys/private_key_" + user + ".pem";

        if (std::filesystem::exists(pub_key_path) || std::filesystem::exists(priv_key_path))
        {
            std::cout << "Keys for user " << user << " already exist. Do you want to overwrite? (y/n): ";
            char response;
            std::cin >> response;
            if (response == 'y' || response == 'Y')
            {
                return true;
            }
            else
            {
                std::cout << "Aborted key generation for user " << user << ".\n";
                return false;
            }
        }
        return true;
    }

    void save_keys(const std::string &user, const mpz_class &e, const mpz_class &d, const mpz_class &n)
    {
        if (e == 0 || d == 0 || n == 0)
        {
            std::cerr << "Error: One or more key components are zero or invalid.\n";
            return;
        }

        std::string pub_key_path = "../keys/public_key_" + user + ".pem";
        std::string priv_key_path = "../keys/private_key_" + user + ".pem";

        std::ofstream pub_file(pub_key_path);
        if (pub_file.is_open())
        {
            pub_file << "-----BEGIN PUBLIC KEY-----\n";
            pub_file << base64_encode(e.get_str(16)) << "\n";
            pub_file << base64_encode(n.get_str(16)) << "\n";
            pub_file << "-----END PUBLIC KEY-----\n";
            pub_file.close();
        }
        else
        {
            std::cerr << "Failed to open file for saving public key.\n";
        }

        std::ofstream priv_file(priv_key_path);
        if (priv_file.is_open())
        {
            priv_file << "-----BEGIN PRIVATE KEY-----\n";
            priv_file << base64_encode(d.get_str(16)) << "\n";
            priv_file << base64_encode(n.get_str(16)) << "\n";
            priv_file << "-----END PRIVATE KEY-----\n";
            priv_file.close();
        }
        else
        {
            std::cerr << "Failed to open file for saving private key.\n";
        }
    }

    bool load_public_key(const std::string &user, mpz_class &e, mpz_class &n)
    {
        std::ifstream pub_file("../keys/public_key_" + user + ".pem");
        if (!pub_file.is_open())
        {
            std::cerr << "Failed to open public key file for user: " << user << "\n";
            return false;
        }

        std::string line, encoded_e, encoded_n;
        std::getline(pub_file, line);
        std::getline(pub_file, encoded_e);
        std::getline(pub_file, encoded_n);

        if (encoded_e.empty() || encoded_n.empty())
        {
            std::cerr << "Error: Missing public key components (e or n).\n";
            return false;
        }

        try
        {
            e = mpz_class(base64_decode(encoded_e), 16);
            n = mpz_class(base64_decode(encoded_n), 16);
        }
        catch (const std::invalid_argument &ex)
        {
            std::cerr << "Error converting public key components: " << ex.what() << "\n";
            return false;
        }

        pub_file.close();
        return true;
    }

    bool load_private_key(const std::string &user, mpz_class &d, mpz_class &n)
    {
        std::ifstream priv_file("../keys/private_key_" + user + ".pem");
        if (!priv_file.is_open())
        {
            return false;
        }

        std::string line, encoded_d, encoded_n;
        std::getline(priv_file, line);
        std::getline(priv_file, encoded_d);
        std::getline(priv_file, encoded_n);

        d = mpz_class(base64_decode(encoded_d), 16);
        n = mpz_class(base64_decode(encoded_n), 16);

        priv_file.close();
        return true;
    }

    void read_key(const std::string &user)
    {
        std::string pub_key_path = "../keys/public_key_" + user + ".pem";
        std::string priv_key_path = "../keys/private_key_" + user + ".pem";

        RSAKey key;
        if (read_pem_key(pub_key_path, key))
        {
            std::cout << "Public Key: " << key.e.get_str(10) << " " << key.n.get_str(10) << "\n";
        }
        else
        {
            std::cerr << "Failed to read public key for user " << user << "\n";
        }
    }

    bool read_pem_key(const std::string &filepath, RSAKey &key)
    {
        std::ifstream pem_file(filepath);
        if (!pem_file.is_open())
        {
            std::cerr << "Failed to open key file: " << filepath << "\n";
            return false;
        }

        std::string line, key_data;
        bool is_public = false, is_private = false;

        while (std::getline(pem_file, line))
        {
            if (line.find("PUBLIC KEY") != std::string::npos)
            {
                is_public = true;
            }
            else if (line.find("PRIVATE KEY") != std::string::npos)
            {
                is_private = true;
            }
            else if (!line.empty() && line.find("-----") == std::string::npos)
            {
                key_data += line;
            }
        }
        pem_file.close();

        std::string decoded_data = base64_decode(key_data);
        std::cout << "Decoded Base64 data: " << decoded_data << "\n";

        std::istringstream decoded_stream(decoded_data);
        try
        {
            if (is_public)
            {
                std::string e_str, n_str;
                std::getline(decoded_stream, e_str);
                std::getline(decoded_stream, n_str);

                if (e_str.empty() || n_str.empty())
                {
                    std::cerr << "Error: Missing public key components (e or n).\n";
                    return false;
                }

                key.type = "Public";
                key.e = mpz_class(e_str, 16);
                key.n = mpz_class(n_str, 16);
            }
            else if (is_private)
            {
                std::string d_str, n_str;
                std::getline(decoded_stream, d_str);
                std::getline(decoded_stream, n_str);

                if (d_str.empty() || n_str.empty())
                {
                    std::cerr << "Error: Missing private key components (d or n).\n";
                    return false;
                }

                key.type = "Private";
                key.d = mpz_class(d_str, 16);
                key.n = mpz_class(n_str, 16);
            }
            else
            {
                std::cerr << "Invalid PEM format.\n";
                return false;
            }
        }
        catch (const std::invalid_argument &ex)
        {
            std::cerr << "Error converting PEM data to mpz_class: " << ex.what() << "\n";
            return false;
        }

        return true;
    }
}
