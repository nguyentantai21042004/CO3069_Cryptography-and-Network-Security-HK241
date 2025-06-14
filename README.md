# RSA Encryption Implementation

## Project Overview
This project is part of the CO3069 - Cryptography and Network Security course at Ho Chi Minh City University of Technology (HCMUT). The implementation focuses on RSA encryption with a maximum key size of 1024 bits.

## Features
- RSA key generation (up to 1024 bits)
- Message encryption and decryption
- Digital signature generation and verification
- Secure key storage and management

## Project Structure
```
src/
├── bin/              # Compiled executables
├── obj/              # Object files
├── keys/             # Generated RSA keys
├── signatures/       # Digital signatures
├── encrypted_message/# Encrypted messages
└── Makefile         # Build configuration
```

## Requirements
- C/C++ compiler (gcc/g++)
- OpenSSL library
- Make

## Building the Project
```bash
cd src
make
```

## Usage
1. Generate RSA keys:
```bash
./bin/keygen
```

2. Encrypt a message:
```bash
./bin/encrypt <message>
```

3. Decrypt a message:
```bash
./bin/decrypt <encrypted_message>
```

4. Generate digital signature:
```bash
./bin/sign <message>
```

5. Verify digital signature:
```bash
./bin/verify <message> <signature>
```

## Security Notes
- This implementation uses RSA with a maximum key size of 1024 bits
- Keys are stored securely in the `keys/` directory
- Encrypted messages are stored in the `encrypted_message/` directory
- Digital signatures are stored in the `signatures/` directory

## Course Information
- Course: CO3069 - Cryptography and Network Security
- Institution: Ho Chi Minh City University of Technology (HCMUT)
- Semester: HK241

## Author
- Student ID: [Your Student ID]
- Name: [Your Name]

## License
This project is created for educational purposes as part of the course requirements.