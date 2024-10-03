#include "Utility.h"

std::string replaceSpaces(const std::string &input)
{
    std::string result = input;
    // Loop through each character in the string
    for (char &c : result)
    {
        if (c == ' ')
        {
            c = '_'; // Replace space with underscore
        }
    }
    return result;
}

std::string encodeToASCII(const std::string &input)
{
    std::string encoded;
    // Loop through each character in the string
    for (char c : input)
    {
        // Convert each character to its ASCII code and append to the result
        encoded += std::to_string(static_cast<int>(c));
    }
    return encoded;
}

std::string decodeFromASCII(const std::string &input)
{
    std::string decoded;
    int i = 0;
    // Loop through the input string to parse ASCII values
    while (i < input.length())
    {
        int asciiValue = 0;
        // Try to decode a 3-digit ASCII value first
        if (i + 3 <= input.length())
        {
            asciiValue = std::stoi(input.substr(i, 3)); // Get a 3-digit substring
            if (asciiValue >= 32 && asciiValue <= 126)  // Check if it's a printable character
            {
                decoded += static_cast<char>(asciiValue); // Convert to character
                i += 3;                                   // Move to the next ASCII code
                continue;
            }
        }
        // If 3-digit decoding fails, try to decode a 2-digit ASCII value
        if (i + 2 <= input.length())
        {
            asciiValue = std::stoi(input.substr(i, 2)); // Get a 2-digit substring
            if (asciiValue >= 32 && asciiValue <= 126)  // Check if it's a printable character
            {
                decoded += static_cast<char>(asciiValue); // Convert to character
                i += 2;                                   // Move to the next ASCII code
            }
        }
    }
    return decoded;
}

std::string restoreSpaces(const std::string &input)
{
    std::string result = input;
    // Loop through each character in the string
    for (char &c : result)
    {
        if (c == '_')
        {
            c = ' '; // Replace underscore with space
        }
    }
    return result;
}
