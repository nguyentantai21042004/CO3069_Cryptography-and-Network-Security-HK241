#include <iostream>
#include <limits>
#include <gmpxx.h>
#include "PrimeGenerator.h"
#include "RSA.h"

// Pointer to store the receiver's key
RSAKey *receiver_key = nullptr;

// Function to create RSA keys for the receiver
void create_receiver_key(gmp_randclass &rand_gen)
{
    int n;
    std::cout << "Enter the required number of bits for prime numbers: ";
    std::cin >> n;

    // Validate the bit length input
    if (n <= 0)
    {
        std::cerr << "Error: The number of bits must be greater than zero!" << std::endl;
        return;
    }

    // Create keys for the receiver
    std::cout << "Creating keys for receiver..." << std::endl;

    // Generate prime number p for the receiver
    mpz_class p_receiver = Generate_Prime_Number(n, rand_gen);
    mpz_class q_receiver;

    // Ensure q is different from p
    do
    {
        q_receiver = Generate_Prime_Number(n, rand_gen);
    } while (q_receiver == p_receiver);

#ifdef DEBUG
    std::cout << "Prime number p: " << p_receiver.get_str() << std::endl; // Print value of p
    std::cout << "Prime number q: " << q_receiver.get_str() << std::endl; // Print value of q
#endif

    // Create the receiver's RSA keys
    receiver_key = new RSAKey(p_receiver, q_receiver);
    receiver_key->Print_Public_Key();  // Print the receiver's public key
    receiver_key->Print_Private_Key(); // Print the receiver's private key
}

// Function to encode a message to ASCII
std::string encodeToASCII(const std::string &message)
{
    return message; // For simplicity, return the message directly as it is already ASCII
}

// Function to convert ASCII-encoded string to a large integer
mpz_class stringToNumber(const std::string &encoded_message)
{
    mpz_class number = 0;
    // Convert each character in the encoded message to its numeric representation
    for (char c : encoded_message)
    {
        number = (number << 8) + static_cast<unsigned char>(c); // Shift left by 8 bits and add the character
    }
    return number;
}

// Function to send a message (encryption process)
void send_message()
{
    // Check if the receiver's keys have been generated
    if (receiver_key == nullptr)
    {
        std::cerr << "Error: Receiver's key is not generated yet!" << std::endl;
        return;
    }

    std::cin.ignore();   // Ignore any leftover newline characters
    std::string message; // Variable to store the user's message

    // Prompt the user for the message to send
    std::cout << "Enter a message to send: ";
    std::getline(std::cin, message); // Get the complete message from the user

    // Check if the message is empty
    if (message.empty())
    {
        std::cerr << "Error: Message cannot be empty!" << std::endl;
        return;
    }

    // Convert the message into an ASCII-encoded string
    std::string encoded_message = encodeToASCII(message);
    std::cout << "Encoded Message is: " << encoded_message << std::endl;

    // Convert the encoded message into a large integer
    mpz_class message_number = stringToNumber(encoded_message);

    // Get the value of n from the receiver's public key
    mpz_class maxSize = receiver_key->Get_Value_Of_N();

    // Check if the message size exceeds the maximum allowable size for encryption
    if (message_number >= maxSize)
    {
        std::cout << "This message is too large for the receiver's key," << std::endl
                  << "message size: " << message_number << ", receiver size: " << maxSize << std::endl
                  << "Try again with a smaller message or implement chunking." << std::endl;
        return;
    }

    // Create an encryption object using the receiver's public key
    RSAEncryption encryptor(receiver_key->Get_Value_Of_N(), receiver_key->Get_Public_Key());

    // Encrypt the message and store the ciphertext
    mpz_class ciphertext;
    try
    {
        ciphertext = encryptor.Encrypt(encoded_message); // Attempt to encrypt the message
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error during encryption: " << e.what() << std::endl; // Handle encryption errors
        return;
    }

    // Display the ciphertext in numeric format
    std::cout << "Encrypted ciphertext: " << ciphertext << std::endl;
}

// Function to decode ASCII to original text
std::string decodeFromASCII(const std::string &numeric_message)
{
    return numeric_message; // For simplicity, assume the numeric message is ASCII
}

// Function to receive a message (decryption process)
void receive_message()
{
    // Check if receiver keys have been generated
    if (receiver_key == nullptr)
    {
        std::cerr << "Error: Receiver keys are not generated yet!" << std::endl;
        return;
    }

    mpz_class ciphertext; // Variable to store the ciphertext input from the user
    std::cout << "Enter the ciphertext: ";

    // Attempt to read the ciphertext from the user
    if (!(std::cin >> ciphertext))
    {
        std::cerr << "Error: Failed to read ciphertext! Please ensure you enter a valid number." << std::endl;
        std::cin.clear();                                                   // Clear the error flag on cin
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        return;
    }

    // Check if the entered ciphertext is valid (greater than zero)
    if (ciphertext <= 0)
    {
        std::cerr << "Error: Invalid ciphertext entered! Ciphertext must be a positive number." << std::endl;
        return;
    }

    // Create a decryption object using the receiver's private key
    RSADecryption decryptor(receiver_key->Get_Value_Of_N(), receiver_key->Get_Private_Key());

    // Decrypt the ciphertext
    std::string decrypted_message;
    try
    {
        decrypted_message = decryptor.Decrypt(ciphertext);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error during decryption: " << e.what() << std::endl;
        return;
    }

    // Display the decrypted message as a number (the integer representation)
    std::cout << "Decrypted message (numeric): " << decrypted_message << std::endl;

    // Decode the decrypted message from ASCII to the original text
    std::string decoded_message = decodeFromASCII(decrypted_message);

    // Display the final restored message
    std::cout << "Restored message: " << decoded_message << std::endl;
}

// Function to print the receiver's keys
void print_receiver_key()
{
    if (receiver_key == nullptr)
    {
        std::cerr << "Error: Receiver keys are not generated yet!" << std::endl;
        return;
    }

    std::cout << "===== Receiver's Keys =====" << std::endl;

    // Print the public key of the receiver
    std::cout << "Public Key:" << std::endl;
    receiver_key->Print_Public_Key();

    // Optionally print the private key (only for debugging or educational purposes)
    std::cout << "Private Key (for debugging purposes):" << std::endl;
    receiver_key->Print_Private_Key();

    // Print additional key information (p, q, and Euler's Phi)
    std::cout << "Value of P (Prime 1): " << receiver_key->Get_Value_Of_P() << std::endl;
    std::cout << "Value of Q (Prime 2): " << receiver_key->Get_Value_Of_Q() << std::endl;
    std::cout << "Value of Phi Euler: " << receiver_key->Get_Value_Of_Phi_Euler() << std::endl;
}

// Function to display the main menu options
void display_menu()
{
    std::cout << "======= RSA CLI Menu =======" << std::endl;
    std::cout << "==1. Create receiver keys ==" << std::endl;
    std::cout << "==2. Send message         ==" << std::endl;
    std::cout << "==3. Receive message      ==" << std::endl;
    std::cout << "==4. Print Receiver's Keys==" << std::endl;
    std::cout << "==5. Exit                 ==" << std::endl;
    std::cout << "============================" << std::endl;
}

// Main function to run the RSA program
int main()
{
    // Initialize the random number generator
    gmp_randclass rand_gen(gmp_randinit_mt); // Use Mersenne Twister
    rand_gen.seed(time(NULL));               // Seed the generator

    int choice;
    while (true)
    {
        display_menu(); // Display the menu
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            create_receiver_key(rand_gen); // Create keys for receiver
            break;
        case 2:
            send_message(); // Send a message (encryption)
            break;
        case 3:
            receive_message(); // Receive a message (decryption)
            break;
        case 4:
            print_receiver_key(); // Print the receiver's keys
            break;
        case 5:
            std::cout << "Exiting..." << std::endl;
            // Clean up dynamically allocated memory
            delete receiver_key;
            receiver_key = nullptr; // Prevent dangling pointer
            return 0;
        default:
            std::cout << "Invalid choice, please try again." << std::endl;
            break;
        }
    }
    return 0;
}
