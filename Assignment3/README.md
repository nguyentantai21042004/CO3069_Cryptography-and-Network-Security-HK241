# Dự án Mã hóa RSA và Chữ ký số
Dự án này là một triển khai của hệ thống mã hóa RSA kết hợp với chữ ký số, nhằm hỗ trợ các thao tác như tạo khóa RSA, mã hóa và giải mã thông điệp, ký và xác thực chữ ký số. Dự án được viết bằng C++ và sử dụng thư viện GMP để xử lý số học lớn.

## Cấu trúc thư mục
├── src                # Thư mục chứa mã nguồn \
│   ├── core           # Các file nguồn liên quan đến thuật toán số nguyên tố \
│   ├── rsa            # Các file nguồn liên quan đến RSA và các thao tác  
│   ├── utils          # Các hàm tiện ích như ghi log, sinh số ngẫu nhiên, xử lý file pem \
│   └── main.cpp       # File main để chạy chương trình \
├── bin                # Thư mục chứa file thực thi \
├── obj                # Thư mục chứa các file đối tượng (.o) \
├── Makefile           # File Makefile để tự động hóa quá trình biên dịch \
└── README.md          # Hướng dẫn sử dụng và cấu hình dự án \
## Yêu cầu cài đặt
1. C++ Compiler hỗ trợ C++17 (Ví dụ: g++)
2. Thư viện GMP: Hỗ trợ xử lý số học lớn, cần thiết để tính toán các giá trị lớn trong RSA.
3. Thư viện FileSystem: C++17 (std::filesystem) để quản lý và kiểm tra các file và thư mục.

## Cài đặt và biên dịch
### Clone dự án về máy:
```bash
git clone <url_repo>
cd <ten_thu_muc_dự_an>
```

### Cài đặt thư viện GMP (nếu chưa cài đặt):

Trên Ubuntu:
```bash
Copy code
sudo apt-get update
sudo apt-get install libgmp3-dev
```
Sau khi biên dịch thành công, file thực thi sẽ nằm trong thư mục bin với tên là main.

### Chạy chương trình: 
Để chạy chương trình, sử dụng lệnh:
```bash
make run
```

### Hướng dẫn sử dụng
Khi chạy chương trình, một menu sẽ xuất hiện với các lựa chọn sau:

1. Generate Key Pair: Tạo một cặp khóa công khai và khóa bí mật RSA, lưu vào thư mục keys.
2. Encrypt Message: Mã hóa một thông điệp bằng khóa công khai của người nhận.
3. Decrypt Message: Giải mã một thông điệp bằng khóa bí mật của người nhận.
4. Check if Keys Exist: Kiểm tra xem cặp khóa của người dùng có tồn tại trong thư mục keys hay không.
5. List All Keys: Hiển thị danh sách tất cả các khóa .pem hiện có trong thư mục keys. \

Các tệp và thư mục được tạo trong quá trình sử dụng:
Thư mục keys: Lưu trữ các cặp khóa công khai và khóa bí mật dưới định dạng .pem.
Thư mục encrypted_message: Lưu trữ các thông điệp đã được mã hóa.
Thư mục signatures: Lưu trữ các chữ ký số để xác thực các thông điệp.
Chi tiết các chức năng chính
Tạo cặp khóa RSA:

Chương trình tạo hai số nguyên tố lớn p và q, sau đó tính n = p * q và phi(n) = (p - 1) * (q - 1).
Chọn số e nguyên tố với phi(n), tính toán d là nghịch đảo modular của e mod phi(n).
Lưu trữ cặp khóa công khai (e, n) và khóa bí mật (d, n) trong các file .pem.
Mã hóa thông điệp:

Chuyển đổi thông điệp thành số nguyên lớn, sau đó mã hóa bằng khóa công khai của người nhận.
Lưu trữ thông điệp đã mã hóa trong thư mục encrypted_message.
Giải mã thông điệp:

Tải thông điệp đã mã hóa và sử dụng khóa bí mật của người nhận để giải mã.
Chuyển đổi số nguyên lớn về dạng văn bản ban đầu và hiển thị.
Ký và xác thực chữ ký số:

Ký số một thông điệp bằng khóa bí mật của người gửi và lưu chữ ký vào thư mục signatures.
Xác thực chữ ký số bằng cách so sánh thông điệp giải mã từ chữ ký với thông điệp gốc.
Xóa các file biên dịch
Để xóa các file .o và file thực thi sau khi biên dịch xong, dùng lệnh:

```bash
make clean
```
Thông tin bổ sung
Mã nguồn có sử dụng std::filesystem từ C++17 để quản lý các thư mục và kiểm tra sự tồn tại của file. \
Logger được tích hợp trong mã để ghi lại các sự kiện quan trọng, hỗ trợ việc gỡ lỗi và kiểm tra.

Tác giả \
Tên tác giả: Nguyễn Tấn Tài - 2212990 \
Email: tai.nguyentantai21042004@hcmut.edu.vn \
Khóa học: CO3069 - Mã hóa và An ninh Mạng