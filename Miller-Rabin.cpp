#include <iostream>
using namespace std;

// Hàm tính (base^exponent) mod modValue
long long modExp(long long base, long long exponent, long long modValue)
{
    long long result = 1;
    base = base % modValue;

    while (exponent > 0)
    {
        // Nếu exponent là lẻ, nhân base với kết quả
        if (exponent % 2 == 1)
        {
            result = (result * base) % modValue;
        }
        // exponent phải là chẵn
        exponent = exponent >> 1;        // Chia đôi exponent
        base = (base * base) % modValue; // Bình phương base
    }
    return result;
}

bool miller_rabin(int n, int iterations)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;

    /* 1. Find integers k, q, k > 0, q odd, so that (n–1) = 2^{k}.q */
    int k = 0;
    int q = n - 1;
    while (q % 2 == 0)
    {
        q /= 2;
        k++;
    }

    for (int i = 0; i < iterations; i++)
    {
        /* 2. Select a random integer a, 1 < a < n–1 */
        int a = 1 + rand() % (n - 1);
        cout << "Kiểm tra với a = " << a << ": ";

        /* 3. If a^{q} mod n == 1 then return ("maybe prime") */
        long long x = modExp(a, q, n);
        if (x == 1 || x == n - 1)
        {
            cout << "thành công" << endl;
            continue; // Có thể là số nguyên tố
        }

        /* 4. for j = 0 to k – 1 do */
        bool isComposite = true;
        for (int j = 0; j < k - 1; j++)
        {
            /* 5. If a^{2^{j}.q} mod n == n - 1 then return ("maybe prime") */
            x = modExp(x, 2, n);
            if (x == n - 1)
            {
                isComposite = false; // Có thể là số nguyên tố
                cout << "thành công" << endl;
                break;
            }
        }

        /* 6. return ("composite") */
        if (isComposite)
        {
            cout << "không thành công" << endl;
            return false; // Composite
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(0))); // Khởi tạo bộ sinh số ngẫu nhiên

    if (argc != 3)
    {
        cout << "Sử dụng: " << argv[0] << " <số nguyên n> <số lần lặp>" << endl;
        return 1;
    }

    int n = atoi(argv[1]);          // Chuyển đổi tham số thứ nhất thành số nguyên
    int iterations = atoi(argv[2]); // Chuyển đổi tham số thứ hai thành số nguyên

    if (miller_rabin(n, iterations))
    {
        cout << n << " có thể là số nguyên tố." << endl;
    }
    else
    {
        cout << n << " là số hợp số." << endl;
    }

    return 0;
}