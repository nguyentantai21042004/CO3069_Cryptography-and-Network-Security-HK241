#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <gmpxx.h>
#include <string>

namespace FileUtils
{
    std::string read_message_from_file(const std::string &filename);
    void save_to_file(const std::string &filename, const mpz_class &data);
    void save_text_to_file(const std::string &filename, const std::string &data);
}

#endif // FILE_UTILS_H
