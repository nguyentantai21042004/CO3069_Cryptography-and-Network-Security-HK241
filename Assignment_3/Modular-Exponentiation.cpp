#include <iostream>
#include <fstream>
#include <gmp.h>

using namespace std;

/* Method 1: Using Exponentiation by Squaring */
void squared_exponentiation(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t n);

/* Method 2: Using Exponentiation by Binary Expansion */
void binary_expansion_exponentiation(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t n);

void readInputFile(const string &fileName, mpz_t a, mpz_t b, mpz_t n);

int main(int argc, char *argv[])
{
    // Declare mpz_t variables
    mpz_t a, b, n, result;
    mpz_init(a), mpz_init(b), mpz_init(n), mpz_init(result);

    string fileName = "";
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <input file name>" << endl;
        return 1;
    }
    fileName = argv[1];
    readInputFile(fileName, a, b, n);

    gmp_printf("Value of a = %Zd\n", a);
    gmp_printf("Value of b = %Zd\n", b);
    gmp_printf("Value of n = %Zd\n", n);

    /* Method 1: Using Exponentiation by Squaring */
    squared_exponentiation(result, a, b, n);
    gmp_printf("Result of (a^b) mod n = %Zd\n", result);

    // /* Method 2: Using Exponentiation by Binary Expansion */
    binary_expansion_exponentiation(result, a, b, n);
    gmp_printf("Result of (a^b) mod n = %Zd\n", result);

    // Free memory
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(n);
    mpz_clear(result);

    return 0;
}

void readInputFile(const string &fileName, mpz_t a, mpz_t b, mpz_t n)
{
    ifstream infile(fileName);
    string line;

    if (infile.is_open())
    {
        getline(infile, line);
        mpz_set_str(a, line.substr(line.find('=') + 1).c_str(), 10);

        getline(infile, line);
        mpz_set_str(b, line.substr(line.find('=') + 1).c_str(), 10);

        getline(infile, line);
        mpz_set_str(n, line.substr(line.find('=') + 1).c_str(), 10);

        infile.close();
    }
    else
    {
        cout << "Cannot open file." << endl;
    }
}

void squared_exponentiation(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t n)
{
    /* (base^exp) % mod */
    mpz_t base, exp;
    mpz_init_set(base, a);
    mpz_init_set(exp, b);
    mpz_set_ui(result, 1); // result = 1

    while (mpz_cmp_ui(exp, 0) > 0)
    {
        // Giả sử exp là số chẵn
        if (mpz_even_p(exp))
        {
            // Bình phương base
            mpz_mul(base, base, base);
            mpz_mod(base, base, n); // Thực hiện modulo

            // Chia exp cho 2
            mpz_fdiv_q_2exp(exp, exp, 1);
        }
        else
        {
            // Nếu exp là số lẻ, nhân result với base
            mpz_mul(result, result, base);
            mpz_mod(result, result, n); // Thực hiện modulo

            // Cập nhật exp
            mpz_sub_ui(exp, exp, 1); // giảm exp đi 1
        }
    }

    mpz_clear(base);
    mpz_clear(exp);
}

void binary_expansion_exponentiation(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t n)
{
    /* (base^exp) % mod */
    mpz_t base, exp;
    mpz_init_set(base, a);
    mpz_init_set(exp, b);
    mpz_set_ui(result, 1); // result = 1

    while (mpz_cmp_ui(exp, 0) > 0)
    {
        if (mpz_odd_p(exp))
        {
            // Nếu exp là số lẻ, nhân result với base
            mpz_mul(result, result, base);
            mpz_mod(result, result, n); // Thực hiện modulo
        }

        // Bình phương base
        mpz_mul(base, base, base);
        mpz_mod(base, base, n); // Thực hiện modulo

        // Chia exp cho 2
        mpz_fdiv_q_2exp(exp, exp, 1);
    }

    mpz_clear(base);
    mpz_clear(exp);
}

