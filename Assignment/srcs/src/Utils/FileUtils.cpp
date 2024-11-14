#include "FileUtils.h"
#include <fstream>
#include <iostream>

namespace FileUtils
{

    std::string read_message_from_file(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file)
        {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return "";
        }
        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

    void save_to_file(const std::string &filename, const mpz_class &data)
    {
        std::ofstream file(filename);
        if (file)
        {
            file << data.get_str();
            file.close();
        }
        else
        {
            std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        }
    }

    void save_text_to_file(const std::string &filename, const std::string &data)
    {
        std::ofstream file(filename);
        if (file)
        {
            file << data;
            file.close();
        }
        else
        {
            std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        }
    }
}
