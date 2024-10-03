#ifndef UTILITY_H
#define UTILITY_H

#include <string>

// Replace all spaces in the input string with underscores ('_')
std::string replaceSpaces(const std::string &input);

// Encode a string into its ASCII numeric representation
std::string encodeToASCII(const std::string &input);

// Decode a string from its ASCII numeric representation back to the original characters
std::string decodeFromASCII(const std::string &input);

// Restore underscores ('_') back to spaces (' ')
std::string restoreSpaces(const std::string &input);

#endif
