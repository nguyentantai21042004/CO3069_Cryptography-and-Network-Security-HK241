#include <iostream>
#include <gmp.h>
#include <fstream>

using namespace std;

mpz_t a, b, n, result;

// Hàm tính phi(n)
void calculateEulerPhi(mpz_t phi_n, const mpz_t n) {
    mpz_set(phi_n, n); // Bắt đầu với phi(n) = n
    mpz_t i, temp_n, quotient;
    mpz_init(i);
    mpz_init(temp_n);
    mpz_init(quotient);
    mpz_set(temp_n, n); // Tạo một bản sao của n

    for (mpz_set_ui(i, 2); mpz_cmp(i, temp_n) <= 0; mpz_add_ui(i, i, 1)) {
        // Kiểm tra nếu i là ước số của temp_n
        if (mpz_divisible_p(temp_n, i)) {
            // Giảm phi(n) theo công thức
            mpz_divexact(quotient, temp_n, i); // temp_n / i
            mpz_sub(phi_n, phi_n, quotient);
            // Giảm temp_n để tính cho số nguyên tố
            while (mpz_divisible_p(temp_n, i)) {
                mpz_divexact(temp_n, temp_n, i);
            }
        }
    }

    // Nếu còn lại một số nguyên tố lớn hơn sqrt(n)
    if (mpz_cmp_ui(temp_n, 1) > 0) {
        mpz_sub(phi_n, phi_n, temp_n); // Tính phi(n) cho số nguyên tố còn lại
    }

    mpz_clear(i);
    mpz_clear(temp_n);
    mpz_clear(quotient);
}

// // Hàm sàng Eratosthenes để tạo danh sách các số nguyên tố
// std::vector<bool> sieveOfEratosthenes(const mpz_t n)
// {
//     unsigned long num = mpz_get_ui(n);
//     std::vector<bool> is_prime(num + 1, true);
//     is_prime[0] = is_prime[1] = false;

//     for (unsigned long i = 2; i * i <= num; ++i)
//     {
//         if (is_prime[i])
//         {
//             for (unsigned long j = i * i; j <= num; j += i)
//             {
//                 is_prime[j] = false;
//             }
//         }
//     }
//     return is_prime;
// }

// // Hàm tính phi(n)
// void calculateEulerPhi(mpz_t phi_n, const mpz_t n)
// {
//     mpz_set(phi_n, n); // Khởi tạo phi(n) = n

//     // Tạo mảng sàng số nguyên tố
//     std::vector<bool> is_prime = sieveOfEratosthenes(n);

//     mpz_t temp_n, i;
//     mpz_init(temp_n);
//     mpz_init(i);
//     mpz_set(temp_n, n); // Create a copy of n

//     for (mpz_set_ui(i, 2); mpz_cmp(i, temp_n) <= 0; mpz_add_ui(i, i, 1))
//     {
//         unsigned long ui_i = mpz_get_ui(i); // Convert to unsigned long
//         if (ui_i < is_prime.size() && is_prime[ui_i])
//         {
//             if (mpz_divisible_p(temp_n, i))
//             { // Use temp_n instead of n
//                 // Giảm phi(n) theo công thức
//                 mpz_submul(phi_n, phi_n, i);
//                 do
//                 {
//                     mpz_divexact(temp_n, temp_n, i); // Again, using temp_n
//                 } while (mpz_divisible_p(temp_n, i));
//             }
//         }
//     }

//     // Nếu temp_n còn lại là một số nguyên tố lớn hơn sqrt(n)
//     if (mpz_cmp_ui(temp_n, 1) > 0)
//     {
//         mpz_sub(phi_n, phi_n, temp_n);
//     }

//     mpz_clear(i);
//     mpz_clear(temp_n);
// }

void modular_exponentiation(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t n)
{
    mpz_t b_prime, phi_n;
    mpz_init(b_prime);
    mpz_init(phi_n);

    // Tính phi(n)
    calculateEulerPhi(phi_n, n); // Tính phi(n) chính xác

    // Tính b' = b mod phi(n)
    mpz_mod(b_prime, b, phi_n);

    // Tính (a^b') mod n
    mpz_powm(result, a, b_prime, n);

    // Giải phóng bộ nhớ
    mpz_clear(b_prime);
    mpz_clear(phi_n);
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
        cout << "Không thể mở file." << endl;
    }
}

int main(int argc, char *argv[])
{
    mpz_init(a), mpz_init(b), mpz_init(n);
    mpz_init(result);

    string fileName = "";
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " Name of input file contain value of a, b, n !!!" << endl;
        return 1;
    }
    fileName = argv[1];
    readInputFile(fileName, a, b, n);

    gmp_printf("Value of a = %Zd\n", a);
    gmp_printf("Value of b = %Zd\n", b);
    gmp_printf("Value of n = %Zd\n", n);

    // Tính (a^b) mod n
    modular_exponentiation(result, a, b, n);
    gmp_printf("Result of (a^b) mod n = %Zd\n", result);

    // Giải phóng bộ nhớ
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(n);
    mpz_clear(result);

    return 0;
}
