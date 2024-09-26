#include <iostream>
using namespace std;

// Hàm tính gcd
long long gcd(long long a, long long b)
{
    return b == 0 ? a : gcd(b, a % b);
}

// Hàm tính phi(n)
long long eulerPhi(long long n)
{
    long long result = n;
    for (long long p = 2; p * p <= n; p++)
    {
        if (n % p == 0)
        {
            while (n % p == 0)
            {
                n /= p;
            }
            result -= result / p;
        }
    }
    if (n > 1)
    {
        result -= result / n;
    }
    return result;
}

// Hàm tính (a^b) mod n sử dụng phép nhân modulo
long long modExp(long long a, long long b, long long n)
{
    long long result = 1;
    a = a % n;
    while (b > 0)
    {
        if (b % 2 == 1)
        { // Nếu b là lẻ
            result = (result * a) % n;
        }
        a = (a * a) % n; // a^2
        b /= 2;          // Chia b cho 2
    }
    return result;
}

// Hàm chính
long long modPow(long long a, long long b, long long n)
{
    if (gcd(a, n) != 1)
    {
        // Nếu a không nguyên tố cùng nhau với n, không thể áp dụng định lý Euler
        return -1; // Hoặc xử lý theo cách khác
    }

    long long phi_n = eulerPhi(n);
    long long b_prime = b % phi_n; // b' = b mod phi(n)
    return modExp(a, b_prime, n);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Usage: " << argv[0] << " <a> <b> <n>" << endl;
        return 1;
    }

    long long a = atoll(argv[1]);
    long long b = atoll(argv[2]);
    long long n = atoll(argv[3]);

    long long result = modPow(a, b, n);
    if (result != -1)
    {
        cout << a << "^" << b << " mod " << n << " = " << result << endl;
    }
    else
    {
        cout << "Không thể tính toán do a không nguyên tố cùng nhau với n." << endl;
    }

    return 0;
}