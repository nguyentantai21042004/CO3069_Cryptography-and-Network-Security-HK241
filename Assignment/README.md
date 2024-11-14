**Hệ thống mã hóa RSA**

**Giới thiệu**

Dự án này là hiện thực thuật toán mã hóa **RSA** trên **C++** sử dụng thư viện **GNU Multiple Precision Arithmetic Library (GMP)** để xử lý các số nguyên lớn. Hệ thống bao gồm các chức năng:

- Sinh số nguyên tố lớn (ít nhất 500 bit).
- Tạo cặp khóa RSA (khóa công khai và khóa riêng).
- Mã hóa thông điệp bằng khóa công khai.
- Giải mã thông điệp bằng khóa riêng.

Hệ thống không sử dụng bất kỳ thư viện RSA nào đã có sẵn. Tất cả các chức năng như sinh số nguyên tố, tạo khóa, mã hóa và giải mã đều được hiện thực từ đầu.

**Tính năng**

- **Sinh số nguyên tố lớn**: Sử dụng các thuật toán kiểm tra nguyên tố như Fermat và Miller-Rabin để đảm bảo các số nguyên sinh ra là nguyên tố.
- **Tạo khóa**: Tạo cặp khóa RSA bao gồm khóa công khai (n, e) và khóa riêng (n, d).
- **Mã hóa thông điệp**: Mã hóa thông điệp sử dụng khóa công khai (n, e).
- **Giải mã thông điệp**: Giải mã thông điệp sử dụng khóa riêng (n, d).
- **Xử lý đa luồng**: Tối ưu quá trình sinh số nguyên tố lớn bằng cách sử dụng đa luồng.
- **Tối ưu Montgomery**: Tối ưu hóa các phép toán mô-đun lớn bằng phương pháp Montgomery reduction.

**Yêu cầu**

**Phần mềm**

- **Trình biên dịch GNU C++**
- **Thư viện GMP** (GNU Multiple Precision Arithmetic Library)

Cài đặt GMP trên Linux:

sudo apt-get install libgmp-dev

**Biên dịch**

Để biên dịch chương trình, sử dụng lệnh sau:

g++ -DDEBUG -std=c++17 -Iinclude main.cpp src/PrimeGenerator.cpp src/RSA.cpp -lgmp -lgmpxx -o mclear

**Hướng dẫn sử dụng**

**1\. Tạo khóa RSA**

Để tạo khóa RSA, bạn cần nhập độ dài bit của các số nguyên tố. Chạy chương trình và chọn tùy chọn tạo khóa.

./mclear

- Chọn **1** để tạo khóa RSA.
- Nhập số bit cho các số nguyên tố (ví dụ: 512, 1024).

Hệ thống sẽ tạo ra hai số nguyên tố lớn và tính toán khóa công khai và khóa riêng.

**2\. Mã hóa thông điệp**

Để gửi thông điệp, sử dụng khóa công khai để mã hóa:

- Chọn **2** trong menu.
- Nhập thông điệp mà bạn muốn mã hóa.

Chương trình sẽ chuyển đổi thông điệp thành dạng số nguyên, mã hóa nó và hiển thị bản mã.

**3\. Giải mã thông điệp**

Để giải mã thông điệp bằng khóa riêng:

- Chọn **3** trong menu.
- Nhập bản mã (ciphertext) đã được mã hóa.

Chương trình sẽ giải mã và hiển thị lại thông điệp gốc.

**4\. Xem khóa đã tạo**

Để xem cặp khóa RSA đã được tạo, chọn **4** từ menu. Hệ thống sẽ hiển thị khóa công khai và khóa riêng.

**Mô tả các tệp**

**Các tệp nguồn**

- main.cpp: Tệp nguồn chính của chương trình, quản lý menu và tương tác với người dùng.
- PrimeGenerator.cpp: Hiện thực các chức năng để sinh số nguyên tố lớn, sử dụng các thuật toán Fermat và Miller-Rabin.
- PrimeGenerator.h: Tệp tiêu đề định nghĩa các hàm sinh số nguyên tố.
- RSA.cpp: Hiện thực thuật toán RSA, bao gồm tạo khóa, mã hóa và giải mã.
- RSA.h: Tệp tiêu đề định nghĩa các chức năng chính của RSA.

**Các hàm**

- **Generate_Prime_Number**: Sinh ra số nguyên tố lớn sử dụng đa luồng và kiểm tra tính nguyên tố.
- **Encrypt**: Mã hóa một thông điệp sử dụng khóa công khai.
- **Decrypt**: Giải mã một bản mã sử dụng khóa riêng.

**Ví dụ sử dụng**

Dưới đây là ví dụ về cách chạy chương trình và mã hóa/giải mã một thông điệp:

1. Chạy chương trình: ./mclear
2. Chọn tùy chọn **1** để tạo khóa RSA: Enter the required number of bits for prime numbers: 512; Khóa công khai: (n: ... , e: 65537); Khóa riêng: (n: ... , d: ...);
3. Chọn tùy chọn **2** để mã hóa một thông điệp: Enter a message to send: Nguyen Tan Tai 2212990; Encrypted ciphertext: ...;
4. Chọn tùy chọn **3** để giải mã thông điệp: Enter the ciphertext: ...; Decrypted message: Nguyen Tan Tai 2212990;

**Hướng phát triển tương lai**

- **Hỗ trợ mã hóa thông điệp dài**: Hiện tại, chương trình chỉ hỗ trợ mã hóa thông điệp ngắn. Cần triển khai các kỹ thuật chia nhỏ thông điệp để mã hóa các thông điệp dài hơn mô-đun nnn.
- **Cải thiện hiệu suất**: Tối ưu thêm đa luồng và cải thiện tốc độ tính toán cho các kích thước khóa lớn hơn.
- **Tăng cường bảo mật**: Triển khai thêm các phương pháp padding như **RSA-OAEP** để tăng cường bảo mật cho hệ thống.