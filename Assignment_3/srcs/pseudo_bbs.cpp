// #include <iostream>
// #include <ctime>
// #include <gmpxx.h> // Sử dụng GMP cho các số lớn

// class PseudoBlumBlumShub
// {
// private:
//     mpz_class n;     // Số mô-đun
//     mpz_class state; // Trạng thái hiện tại

// public:
//     // Khởi tạo với hai số nguyên tố lớn p và q, và một seed
//     PseudoBlumBlumShub(const mpz_class &p, const mpz_class &q, const mpz_class &seed)
//     {
//         n = p * q;
//         state = seed % n; // Đảm bảo seed trong phạm vi n
//     }

//     // Sinh ra một "chunk" ngẫu nhiên với số bit cố định (ví dụ 64 bit)
//     uint64_t next_chunk()
//     {
//         state = (state * state) % n; // Bình phương mô-đun
//         return state.get_ui();       // Lấy 64 bit từ state
//     }

//     // Sinh số ngẫu nhiên với số bit chỉ định
//     mpz_class get_random_bits(int bit_length)
//     {
//         mpz_class random_number = 0;
//         int bits_generated = 0;

//         while (bits_generated < bit_length)
//         {
//             uint64_t chunk = next_chunk();
//             int bits_to_add = std::min(64, bit_length - bits_generated);

//             random_number = (random_number << bits_to_add) | (chunk >> (64 - bits_to_add));
//             bits_generated += bits_to_add;
//         }

//         // Đảm bảo số sinh ra có bit cao nhất được đặt (bit_length - 1) để đủ bit yêu cầu
//         mpz_class one = 1;
//         random_number |= (one << (bit_length - 1));

//         return random_number;
//     }
// };

// int main(int argc, char *argv[])
// {
//     // Kiểm tra tham số đầu vào
//     if (argc < 2)
//     {
//         std::cerr << "Vui lòng cung cấp độ dài bit dưới dạng tham số đầu vào.\n";
//         return 1;
//     }

//     int bit_length = std::stoi(argv[1]); // Đọc độ dài bit từ tham số dòng lệnh

//     // Khởi tạo hai số nguyên tố lớn
//     mpz_class p, q, seed;
//     mpz_nextprime(p.get_mpz_t(), mpz_class("18446744073709551629").get_mpz_t()); // Một số nguyên tố lớn gần 2^64
//     mpz_nextprime(q.get_mpz_t(), mpz_class("18446744073709551557").get_mpz_t()); // Một số nguyên tố lớn khác

//     seed = time(nullptr); // Sử dụng thời gian hiện tại làm seed

//     PseudoBlumBlumShub rand_gen(p, q, seed);

//     mpz_class random_number = rand_gen.get_random_bits(bit_length);

//     std::cout << "Số ngẫu nhiên " << bit_length << " bit: " << random_number << std::endl;

//     return 0;
// }

#include <iostream>
#include <gmpxx.h>
#include <random>
#include <ctime>

class PseudoBlumBlumShub
{
private:
    mpz_class n;     // Số mô-đun
    mpz_class state; // Trạng thái hiện tại

public:
    // Khởi tạo với hai số nguyên tố lớn p và q, và một seed ngẫu nhiên
    PseudoBlumBlumShub(const mpz_class &p, const mpz_class &q)
    {
        n = p * q;

        // Tạo seed ngẫu nhiên bằng cách kết hợp giá trị thời gian và thiết bị ngẫu nhiên
        std::random_device rd;
        mpz_class random_seed = rd() ^ static_cast<unsigned>(time(nullptr));
        state = random_seed % n; // Đảm bảo seed trong phạm vi n
    }

    // Sinh ra một "chunk" ngẫu nhiên với số bit cố định (ví dụ 64 bit)
    uint64_t next_chunk()
    {
        state = (state * state) % n; // Bình phương mô-đun
        return state.get_ui();       // Lấy 64 bit từ state
    }

    // Sinh số ngẫu nhiên với số bit chỉ định
    mpz_class get_random_bits(int bit_length)
    {
        mpz_class random_number = 0;
        int bits_generated = 0;

        while (bits_generated < bit_length)
        {
            uint64_t chunk = next_chunk();
            int bits_to_add = std::min(64, bit_length - bits_generated);

            random_number = (random_number << bits_to_add) | (chunk >> (64 - bits_to_add));
            bits_generated += bits_to_add;
        }

        // Đảm bảo số sinh ra có bit cao nhất được đặt (bit_length - 1) để đủ bit yêu cầu
        mpz_class one = 1;
        random_number |= (one << (bit_length - 1));

        return random_number;
    }
};

int main()
{
    // Chọn hai số nguyên tố lớn cho khởi tạo duy nhất
    mpz_class p, q;
    mpz_nextprime(p.get_mpz_t(), mpz_class("1298074214633706835075030044377087").get_mpz_t()); // Số nguyên tố lớn
    mpz_nextprime(q.get_mpz_t(), mpz_class("1298074214633706835075030044376991").get_mpz_t()); // Số nguyên tố lớn khác

    PseudoBlumBlumShub rand_gen(p, q); // Khởi tạo với seed ngẫu nhiên

    int bit_length = 1024; // Ví dụ: sinh số ngẫu nhiên 1024 bit
    mpz_class random_number = rand_gen.get_random_bits(bit_length);

    std::cout << "Số ngẫu nhiên " << bit_length << " bit: " << random_number << std::endl;

    return 0;
}
