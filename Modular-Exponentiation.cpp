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

// void montgomery_multiply(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t n)
// {
//     mpz_t R, R_inv, n_inv, a_prime, b_prime, t, m, c;

//     mpz_init(R);
//     mpz_init(R_inv);
//     mpz_init(n_inv);
//     mpz_init(a_prime);
//     mpz_init(b_prime);
//     mpz_init(t);
//     mpz_init(m);
//     mpz_init(c);

//     // Choose R = 2^k, with k large enough
//     mpz_ui_pow_ui(R, 2, mpz_sizeinbase(n, 2) + 1); // R = 2^(k)

//     // Calculate R^(-1) mod n and n^(-1) mod R
//     mpz_invert(R_inv, R, n);
//     mpz_invert(n_inv, n, R);

//     // Convert a and b to Montgomery form
//     mpz_mul(a_prime, a, R);
//     mpz_mod(a_prime, a_prime, n);

//     mpz_mul(b_prime, b, R);
//     mpz_mod(b_prime, b_prime, n);

//     // Perform multiplication in Montgomery space
//     mpz_mul(t, a_prime, b_prime);
//     mpz_mod(t, t, n);

//     mpz_mul(m, t, n_inv);
//     mpz_mod(m, m, R);

//     mpz_mul(c, m, n);
//     mpz_add(c, c, t);
//     mpz_fdiv_q(result, c, R); // c / R

//     // If result is greater than n, calculate mod n
//     if (mpz_cmp(result, n) >= 0)
//     {
//         mpz_sub(result, result, n);
//     }

//     // Free memory
//     mpz_clear(R);
//     mpz_clear(R_inv);
//     mpz_clear(n_inv);
//     mpz_clear(a_prime);
//     mpz_clear(b_prime);
//     mpz_clear(t);
//     mpz_clear(m);
//     mpz_clear(c);
// }

// void montgomery_exponentiation(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t n)
// {
//     mpz_t R, R_inv, n_inv, a_montgomery, t, m, c;
//     mpz_init(R);
//     mpz_init(R_inv);
//     mpz_init(n_inv);
//     mpz_init(a_montgomery);
//     mpz_init(t);
//     mpz_init(m);
//     mpz_init(c);

//     // Choose R = 2^k, with k large enough
//     mpz_ui_pow_ui(R, 2, mpz_sizeinbase(n, 2) + 1); // R = 2^(k)
//     mpz_invert(R_inv, R, n);
//     mpz_invert(n_inv, n, R);

//     // Convert a to Montgomery form
//     mpz_mul(a_montgomery, a, R);
//     mpz_mod(a_montgomery, a_montgomery, n);

//     // Initialize result to 1 (in Montgomery form)
//     mpz_set_ui(result, 1);
//     mpz_mul(result, result, R);
//     mpz_mod(result, result, n);

//     // Perform exponentiation by squaring
//     mpz_t b_copy;
//     mpz_init(b_copy);
//     mpz_set(b_copy, b); // Set b_copy to b
//     while (mpz_cmp_ui(b_copy, 0) > 0)
//     {
//         if (mpz_odd_p(b_copy))
//         {
//             montgomery_multiply(t, result, a_montgomery, n);
//             mpz_set(result, t);
//         }
//         montgomery_multiply(t, result, result, n);
//         mpz_set(result, t);
//         mpz_fdiv_q_ui(b_copy, b_copy, 2);
//     }
//     mpz_clear(b_copy); // Free memory after use

//     // Convert back to standard form
//     if (mpz_cmp(result, n) >= 0)
//     {
//         mpz_sub(result, result, n);
//     }

//     // Free memory
//     mpz_clear(R);
//     mpz_clear(R_inv);
//     mpz_clear(n_inv);
//     mpz_clear(a_montgomery);
//     mpz_clear(t);
//     mpz_clear(m);
//     mpz_clear(c);
// }