#include <iostream>
#include <cmath>

using namespace std;

// Hàm tính gcd
int gcd(int a, int b)
{
    return (b == 0) ? a : gcd(b, a % b);
}

// Hàm kiểm tra tính nguyên tố bằng thuật toán AKS
bool isPrimeAKS(int n)
{
    if (n <= 1)
        return false; // 0 và 1 không phải số nguyên tố
    if (n == 2 || n == 3)
        return true; // 2 và 3 là số nguyên tố
    if (n % 2 == 0)
        return false; // Số chẵn lớn hơn 2 là hợp số

    // Bước 2: Tìm giá trị r
    int r = 2;
    while (r * r <= n)
    {
        if (n % r == 0)
            return false; // n chia hết cho r
        r++;
    }

    // Bước 3: Kiểm tra các điều kiện
    for (int a = 2; a <= r; a++)
    {
        if (gcd(a, n) != 1)
            return false;        // Nếu gcd(a, n) khác 1
        long long lhs = 1;       // (x - a)^n
        long long rhs = (a % n); // x^n - a

        for (int i = 1; i <= n; i++)
        {
            lhs = (lhs * (1 - a)) % n; // Tính (x - a)^n mod n
        }
        if (lhs != rhs)
            return false; // Kiểm tra tính chất
    }

    return true; // n là số nguyên tố
}

int main()
{
    int n;
    cout << "Nhập một số nguyên dương n: ";
    cin >> n;

    if (isPrimeAKS(n))
    {
        cout << n << " là số nguyên tố." << endl;
    }
    else
    {
        cout << n << " là hợp số." << endl;
    }

    return 0;
}
