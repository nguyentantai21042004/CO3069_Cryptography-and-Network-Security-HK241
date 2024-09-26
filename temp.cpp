#include <iostream>
#include <gmp.h>
#include <fstream>

using namespace std;

mpz_t a, b, n, result;

// // Hàm tính phi(n)
// void calculateEulerPhi(mpz_t phi_n, const mpz_t n) {
//     mpz_set(phi_n, n); // Bắt đầu với phi(n) = n
//     mpz_t i, temp_n, quotient;
//     mpz_init(i);
//     mpz_init(temp_n);
//     mpz_init(quotient);
//     mpz_set(temp_n, n); // Tạo một bản sao của n

//     for (mpz_set_ui(i, 2); mpz_cmp(i, temp_n) <= 0; mpz_add_ui(i, i, 1)) {
//         // Kiểm tra nếu i là ước số của temp_n
//         if (mpz_divisible_p(temp_n, i)) {
//             // Giảm phi(n) theo công thức
//             mpz_divexact(quotient, temp_n, i); // temp_n / i
//             mpz_sub(phi_n, phi_n, quotient);
//             // Giảm temp_n để tính cho số nguyên tố
//             while (mpz_divisible_p(temp_n, i)) {
//                 mpz_divexact(temp_n, temp_n, i);
//             }
//         }
//     }

//     // Nếu còn lại một số nguyên tố lớn hơn sqrt(n)
//     if (mpz_cmp_ui(temp_n, 1) > 0) {
//         mpz_sub(phi_n, phi_n, temp_n); // Tính phi(n) cho số nguyên tố còn lại
//     }

//     mpz_clear(i);
//     mpz_clear(temp_n);
//     mpz_clear(quotient);
// }

// // // Hàm sàng Eratosthenes để tạo danh sách các số nguyên tố
// // std::vector<bool> sieveOfEratosthenes(const mpz_t n)
// // {
// //     unsigned long num = mpz_get_ui(n);
// //     std::vector<bool> is_prime(num + 1, true);
// //     is_prime[0] = is_prime[1] = false;

// //     for (unsigned long i = 2; i * i <= num; ++i)
// //     {
// //         if (is_prime[i])
// //         {
// //             for (unsigned long j = i * i; j <= num; j += i)
// //             {
// //                 is_prime[j] = false;
// //             }
// //         }
// //     }
// //     return is_prime;
// // }

// // // Hàm tính phi(n)
// // void calculateEulerPhi(mpz_t phi_n, const mpz_t n)
// // {
// //     mpz_set(phi_n, n); // Khởi tạo phi(n) = n

// //     // Tạo mảng sàng số nguyên tố
// //     std::vector<bool> is_prime = sieveOfEratosthenes(n);

// //     mpz_t temp_n, i;
// //     mpz_init(temp_n);
// //     mpz_init(i);
// //     mpz_set(temp_n, n); // Create a copy of n

// //     for (mpz_set_ui(i, 2); mpz_cmp(i, temp_n) <= 0; mpz_add_ui(i, i, 1))
// //     {
// //         unsigned long ui_i = mpz_get_ui(i); // Convert to unsigned long
// //         if (ui_i < is_prime.size() && is_prime[ui_i])
// //         {
// //             if (mpz_divisible_p(temp_n, i))
// //             { // Use temp_n instead of n
// //                 // Giảm phi(n) theo công thức
// //                 mpz_submul(phi_n, phi_n, i);
// //                 do
// //                 {
// //                     mpz_divexact(temp_n, temp_n, i); // Again, using temp_n
// //                 } while (mpz_divisible_p(temp_n, i));
// //             }
// //         }
// //     }

// //     // Nếu temp_n còn lại là một số nguyên tố lớn hơn sqrt(n)
// //     if (mpz_cmp_ui(temp_n, 1) > 0)
// //     {
// //         mpz_sub(phi_n, phi_n, temp_n);
// //     }

// //     mpz_clear(i);
// //     mpz_clear(temp_n);
// // }

// void modular_exponentiation(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t n)
// {
//     mpz_t b_prime, phi_n;
//     mpz_init(b_prime);
//     mpz_init(phi_n);

//     // Tính phi(n)
//     calculateEulerPhi(phi_n, n); // Tính phi(n) chính xác

//     // Tính b' = b mod phi(n)
//     mpz_mod(b_prime, b, phi_n);

//     // Tính (a^b') mod n
//     mpz_powm(result, a, b_prime, n);

//     // Giải phóng bộ nhớ
//     mpz_clear(b_prime);
//     mpz_clear(phi_n);
// }

// // Hàm tính gcd
// long long gcd(long long a, long long b)
// {
//     return b == 0 ? a : gcd(b, a % b);
// }

// // Hàm tính phi(n)
// long long eulerPhi(long long n)
// {
//     long long result = n;
//     for (long long p = 2; p * p <= n; p++)
//     {
//         if (n % p == 0)
//         {
//             while (n % p == 0)
//             {
//                 n /= p;
//             }
//             result -= result / p;
//         }
//     }
//     if (n > 1)
//     {
//         result -= result / n;
//     }
//     return result;
// }

// // Hàm tính (a^b) mod n sử dụng phép nhân modulo
// long long modExp(long long a, long long b, long long n)
// {
//     long long result = 1;
//     a = a % n;
//     while (b > 0)
//     {
//         if (b % 2 == 1)
//         { // Nếu b là lẻ
//             result = (result * a) % n;
//         }
//         a = (a * a) % n; // a^2
//         b /= 2;          // Chia b cho 2
//     }
//     return result;
// }

// // Hàm chính
// long long modPow(long long a, long long b, long long n)
// {
//     if (gcd(a, n) != 1)
//     {
//         // Nếu a không nguyên tố cùng nhau với n, không thể áp dụng định lý Euler
//         return -1; // Hoặc xử lý theo cách khác
//     }

//     long long phi_n = eulerPhi(n);
//     long long b_prime = b % phi_n; // b' = b mod phi(n)
//     return modExp(a, b_prime, n);
// }

// Hàm Montgomery Reduction
void montgomery_reduce(mpz_t result, const mpz_t T, const mpz_t N, const mpz_t N_prime, const mpz_t R)
{
    mpz_t m, t;
    mpz_inits(m, t, NULL);

    // m = (T * N_prime) % R
    mpz_mul(m, T, N_prime);
    mpz_mod(m, m, R);

    // t = (T + m * N) / R
    mpz_mul(t, m, N);
    mpz_add(t, T, t);
    mpz_fdiv_q(t, t, R);

    // Nếu t >= N, ta trừ N
    if (mpz_cmp(t, N) >= 0)
    {
        mpz_sub(t, t, N);
    }

    // Kết quả sau khi Montgomery Reduction
    mpz_set(result, t);

    mpz_clears(m, t, NULL);
}

// Hàm thực hiện phép nhân Montgomery
void montgomery_multiplication(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t N)
{
    mpz_t R, R_inv, N_prime, A, B, T;
    mpz_inits(R, R_inv, N_prime, A, B, T, NULL);

    // Bước 1: Xác định R lớn hơn N và là lũy thừa của 2
    mpz_setbit(R, mpz_sizeinbase(N, 2) + 1); // R = 2^(bit_length(N) + 1)

    // Tính R^(-1) mod N
    mpz_invert(R_inv, R, N);

    // Tính N' = -N^(-1) mod R
    mpz_invert(N_prime, N, R);
    mpz_neg(N_prime, N_prime);
    mpz_mod(N_prime, N_prime, R);

    // Bước 2: Chuyển đổi a, b về đại diện Montgomery
    mpz_mul(A, a, R);
    mpz_mod(A, A, N);

    mpz_mul(B, b, R);
    mpz_mod(B, B, N);

    // Bước 3: Nhân Montgomery
    mpz_mul(T, A, B); // T = A * B
    montgomery_reduce(result, T, N, N_prime, R);

    // Bước 4: Chuyển đổi ngược về hệ bình thường
    mpz_mul(result, result, R_inv);
    mpz_mod(result, result, N);

    mpz_clears(R, R_inv, N_prime, A, B, T, NULL);
}

// Hàm lũy thừa sử dụng Montgomery
void montgomery_exponentiation(mpz_t result, const mpz_t a, const mpz_t exp, const mpz_t N)
{
    mpz_t R, N_prime, A, base, temp;
    mpz_inits(R, N_prime, A, base, temp, NULL);

    // Bước 1: Tính R = 2^(bit_length(N) + 1)
    mpz_setbit(R, mpz_sizeinbase(N, 2) + 1); // R = 2^(bit_length(N) + 1)

    // Bước 2: Tính N' = -N^(-1) mod R
    mpz_invert(N_prime, N, R);
    mpz_neg(N_prime, N_prime);
    mpz_mod(N_prime, N_prime, R);

    // Bước 3: Chuyển a vào hệ Montgomery
    mpz_mul(A, a, R);
    mpz_mod(A, A, N);

    // Bước 4: Khởi tạo kết quả là Montgomery(1)
    mpz_set_ui(result, 1);
    mpz_mul(result, result, R);
    mpz_mod(result, result, N);

    // Bước 5: Khởi tạo base = Montgomery(a)
    mpz_set(base, A);

    // Bước 6: Tính lũy thừa bằng bình phương lặp kết hợp Montgomery Multiplication
    mpz_t exp_copy;
    mpz_init_set(exp_copy, exp);

    while (mpz_cmp_ui(exp_copy, 0) > 0)
    {
        if (mpz_odd_p(exp_copy)) // nếu lẻ
        {
            montgomery_multiplication(result, result, base, N);
        }
        montgomery_multiplication(base, base, base, N); // bình phương base
        mpz_fdiv_q_2exp(exp_copy, exp_copy, 1);         // chia exp cho 2
    }

    // Bước 7: Chuyển kết quả về hệ bình thường
    mpz_mul(temp, result, R);
    mpz_mod(result, temp, N);

    mpz_clears(R, N_prime, A, base, temp, exp_copy, NULL);
}

/* Case 1: */
// Hàm tính (a^b) % n sử dụng phương pháp bình phương lặp
void modular_exponentiation(mpz_t result, const mpz_t a, const mpz_t b, const mpz_t n)
{
    mpz_t base, exp, mod;
    mpz_init_set(base, a);
    mpz_init_set(exp, b);
    mpz_init_set(mod, n);

    mpz_set_ui(result, 1); // result = 1

    while (mpz_cmp_ui(exp, 0) > 0)
    {
        if (mpz_odd_p(exp))
        {
            mpz_mul(result, result, base); // result = result * base
            mpz_mod(result, result, mod);  // result = result % mod
        }

        mpz_mul(base, base, base);    // base = base^2
        mpz_mod(base, base, mod);     // base = base % mod
        mpz_fdiv_q_2exp(exp, exp, 1); // exp = exp / 2
    }

    mpz_clears(base, exp, mod, NULL);
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
    // montgomery_exponentiation(result, a, b, n);
    gmp_printf("Result of (a^b) mod n = %Zd\n", result);

    // Giải phóng bộ nhớ
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(n);
    mpz_clear(result);

    return 0;
}