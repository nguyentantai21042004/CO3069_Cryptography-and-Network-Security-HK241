#include <iostream>
#include "PrimeGenerator.h"
#include "GCD.h"
#include "RSA.h"
#include "Utility.h"

// Pointers to store the sender's and receiver's keys
RSAKey *sender_key = nullptr;
RSAKey *receiver_key = nullptr;

// Create RSA keys for both the sender and the receiver
void create_keys()
{
    int n;
    std::cout << "Enter the required number of bits for prime numbers: ";
    std::cin >> n;

    // Create keys for the sender
    std::cout << "Creating keys for sender..." << std::endl;
    mpz_class p_sender = Generate_Prime_Number(n); // Generate a prime number p for the sender
    mpz_class q_sender;
    do
    {
        q_sender = Generate_Prime_Number(n); // Generate a different prime number q for the sender
    } while (q_sender == p_sender);
    sender_key = new RSAKey(p_sender, q_sender); // Create the sender's RSA keys
    sender_key->Print_Public_Key();              // Print the sender's public key
    sender_key->Print_Private_Key();             // Print the sender's private key

    // Create keys for the receiver
    std::cout << "Creating keys for receiver..." << std::endl;
    mpz_class p_receiver = Generate_Prime_Number(n); // Generate a prime number p for the receiver
    mpz_class q_receiver;
    do
    {
        q_receiver = Generate_Prime_Number(n); // Generate a different prime number q for the receiver
    } while (q_receiver == p_receiver);
    receiver_key = new RSAKey(p_receiver, q_receiver); // Create the receiver's RSA keys
    receiver_key->Print_Public_Key();                  // Print the receiver's public key
    receiver_key->Print_Private_Key();                 // Print the receiver's private key
}

// Send a message (encryption process)
void send_message()
{
    if (sender_key == nullptr || receiver_key == nullptr)
    {
        std::cerr << "Error: Keys for sender or receiver are not generated yet!" << std::endl;
        return;
    }

    std::cin.ignore(); // Ignore any leftover newline characters
    std::string message;
    std::cout << "Enter a message to send: ";
    std::getline(std::cin, message); // Get the message from the user

    std::string encoded_message = encodeToASCII(message); // Encode the message into ASCII format
    std::cout << "Encoded Message is: " << encoded_message << std::endl;

    mpz_class size(encoded_message, 10);                // Convert the encoded message into a large integer
    mpz_class maxSize = receiver_key->Get_Value_Of_N(); // Get the value of n from the receiver's public key

    if (size >= maxSize) // Check if the message size is larger than n
    {
        std::cout << "This message is too large for the receiver's key," << std::endl
                  << "message size: " << size << ", receiver size:" << maxSize
                  << " try again with a smaller message." << std::endl;
        return;
    }

    RSAEncryption encryptor(receiver_key->Get_Value_Of_N(), receiver_key->Get_Public_Key()); // Create an encryption object
    mpz_class ciphertext = encryptor.Encrypt(encoded_message);                               // Encrypt the message
    std::cout << "Encrypted ciphertext: " << ciphertext << std::endl;                        // Display the ciphertext
}

// Receive a message (decryption process)
void receive_message()
{
    if (receiver_key == nullptr)
    {
        std::cerr << "Error: Receiver keys are not generated yet!" << std::endl;
        return;
    }

    mpz_class ciphertext;
    std::cout << "Enter the ciphertext: ";
    std::cin >> ciphertext; // Get the ciphertext from the user

    RSADecryption decryptor(receiver_key->Get_Value_Of_N(), receiver_key->Get_Private_Key()); // Create a decryption object
    std::string decrypted_message = decryptor.Decrypt(ciphertext);                            // Decrypt the ciphertext
    std::cout << "Decrypted message: " << decrypted_message << std::endl;                     // Display the decrypted message

    std::string decoded_message = decodeFromASCII(decrypted_message);   // Decode the message from ASCII to original text
    std::string restored_message = restoreSpaces(decoded_message);      // Restore spaces in the message
    std::cout << "Restored message: " << restored_message << std::endl; // Display the final message
}

// Print the sender's keys
void print_sender_key()
{
    if (sender_key == nullptr)
    {
        std::cerr << "Error: Sender keys are not generated yet!" << std::endl;
        return;
    }

    std::cout << "===== Sender's Keys =====" << std::endl;
    sender_key->Print_Public_Key();  // Print the public key of the sender
    sender_key->Print_Private_Key(); // Print the private key of the sender

    std::cout << "Value of P: " << sender_key->Get_Value_Of_P() << std::endl;
    std::cout << "Value of Q: " << sender_key->Get_Value_Of_Q() << std::endl;
    std::cout << "Value of Phi Euler: " << sender_key->Get_Value_Of_Phi_Euler() << std::endl;
}

// Print the receiver's keys
void print_receiver_key()
{
    if (receiver_key == nullptr)
    {
        std::cerr << "Error: Receiver keys are not generated yet!" << std::endl;
        return;
    }

    std::cout << "===== Receiver's Keys =====" << std::endl;
    receiver_key->Print_Public_Key();  // Print the public key of the receiver
    receiver_key->Print_Private_Key(); // Print the private key of the receiver

    std::cout << "Value of P: " << receiver_key->Get_Value_Of_P() << std::endl;
    std::cout << "Value of Q: " << receiver_key->Get_Value_Of_Q() << std::endl;
    std::cout << "Value of Phi Euler: " << receiver_key->Get_Value_Of_Phi_Euler() << std::endl;
}

// Display the main menu options
void display_menu()
{
    std::cout << "======= RSA CLI Menu =======" << std::endl;
    std::cout << "==1. Create keys          ==" << std::endl;
    std::cout << "==2. Send message         ==" << std::endl;
    std::cout << "==3. Receive message      ==" << std::endl;
    std::cout << "==4. Print Sender's Keys  ==" << std::endl;
    std::cout << "==5. Print Receiver's Keys==" << std::endl;
    std::cout << "==6. Exit                 ==" << std::endl;
    std::cout << "============================" << std::endl;
}

// Main function to run the RSA program
int main()
{
    int choice;
    while (true)
    {
        display_menu(); // Display the menu
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            create_keys(); // Create keys for sender and receiver
            break;
        case 2:
            send_message(); // Send a message (encryption)
            break;
        case 3:
            receive_message(); // Receive a message (decryption)
            break;
        case 4:
            print_sender_key(); // Print the sender's keys
            break;
        case 5:
            print_receiver_key(); // Print the receiver's keys
            break;
        case 6:
            std::cout << "Exiting..." << std::endl;
            return 0; // Exit the program
        default:
            std::cout << "Invalid choice, please try again." << std::endl;
            break;
        }
    }
    return 0;
}