**Hiện thực hệ mã RSA**

# **1\. Tìm số nguyên tố lớn (ít nhất 500 bits)**

**1.1. Thư viện GMP (GNU Multiple Precision Arithmetic Library)**

Thư viện GMP (GNU Multiple Precision Arithmetic Library) cung cấp các biến và cấu trúc cơ bản để làm việc với các số nguyên lớn, số thực lớn và số hợp số.

Các kiểu dữ liệu chính:

- mpz_t: Kiểu dữ liệu đại diện cho số nguyên lớn (big integer). Cú pháp: mpz_t var_name; Quản lý bộ nhớ: Cần khởi tạo với mpz_init(var_name) và giải phóng với mpz_clear(var_name).
- mpq_t: Kiểu dữ liệu đại diện cho số hữu tỷ (rational number), tức là tỷ lệ giữa hai số nguyên. Cú pháp: mpq_t var_name; Khởi tạo với mpq_init(var_name) và giải phóng với mpq_clear(var_name).
- mpf_t: Kiểu dữ liệu đại diện cho số thực lớn (floating-point number). Cú pháp: mpf_t var_name; Khởi tạo với mpf_init(var_name) và giải phóng với mpf_clear(var_name).

Lớp mpz_class trong C++ là một lớp trong thư viện GMP dành cho C++, cung cấp giao diện hướng đối tượng để làm việc với các số nguyên lớn.

Thư viện GMP cung cấp các kiểu dữ liệu mạnh mẽ để xử lý các số nguyên lớn, số hữu tỷ và số thực lớn. Với các phương thức dễ sử dụng và khả năng tự động quản lý bộ nhớ trong mpz_class, nó trở thành một công cụ tuyệt vời cho các ứng dụng cần tính toán chính xác và làm việc với các số lớn.

**1.2. Sinh số ngẫu nhiên lớn**

Để bắt đầu, chúng ta cần sinh ra một số ngẫu nhiên lớn với số lượng bit xác định (ít nhất 500 bits). Đây là bước cơ bản để tạo ra các số ứng cử viên cho việc kiểm tra tính nguyên tố.

Trong C++, bạn có thể sử dụng các hàm ngẫu nhiên như rand(), hoặc tốt hơn là srand() để tạo số ngẫu nhiên. Để tạo ra số ngẫu nhiên lớn, bạn sẽ phải làm việc với một thư viện hỗ trợ số lớn như GMP (GNU Multiple Precision Arithmetic Library) hoặc NTL (Number Theory Library).

Cách Sinh Số Ngẫu Nhiên Lớn Với 500 Bits: Ta cần đảm bảo rằng số được tạo ra có độ dài chính xác là 500 bits (hoặc hơn). Điều này có nghĩa là số được sinh ra phải nằm trong khoảng: 2^{499} <= p <= 2^{500}. Sau khi có số ngẫu nhiên, cần đặt bit cao nhất và bit thấp nhất để đảm bảo số sinh ra có đúng độ dài 500 bits và là số lẻ (số lẻ cần thiết vì số chẵn không thể là số nguyên tố trừ số 2).

Khởi Tạo Seed: Seed (hạt giống) là một giá trị khởi đầu được sử dụng để khởi động một thuật toán sinh số ngẫu nhiên. Khi bạn gọi std::srand(seed), bạn cung cấp một giá trị seed cho bộ sinh số ngẫu nhiên. Nếu bạn không cung cấp seed, hoặc nếu bạn sử dụng cùng một giá trị seed nhiều lần, thì chuỗi số ngẫu nhiên mà bạn tạo ra sẽ giống nhau mỗi lần bạn chạy chương trình.

Sau khi seed đã được thiết lập, bạn có thể gọi std::rand() để lấy các số ngẫu nhiên. Nếu bạn đã sử dụng một seed khác mỗi lần khởi động chương trình (thường là thời gian hiện tại), chuỗi số ngẫu nhiên sẽ khác nhau mỗi lần chạy.

Sự Quan Trọng Của Seed

Thay vì dùng std::time(nullptr), bạn sử dụng std::chrono::system_clock::now().time_since_epoch().count() để tạo ra một giá trị seed chính xác hơn. Điều này giúp đảm bảo rằng nếu chương trình chạy nhiều lần trong một giây, mỗi lần chạy sẽ có giá trị seed khác nhau.

- Tính Ngẫu Nhiên: Nếu bạn muốn có những số ngẫu nhiên khác nhau trong mỗi lần chạy chương trình, việc sử dụng thời gian hiện tại (như std::time(nullptr)) là một cách thông dụng để tạo ra một seed duy nhất.
- Tái Tạo: Trong một số trường hợp, bạn có thể muốn sinh ra cùng một chuỗi số ngẫu nhiên nhiều lần để phục vụ cho việc kiểm tra hoặc mô phỏng. Khi đó, bạn có thể sử dụng cùng một giá trị seed.

Hàm mpz_urandomb trong thư viện GMP được sử dụng để sinh ra một số nguyên ngẫu nhiên có kích thước cụ thể. Cú pháp của hàm này như sau:

void mpz_urandomb(mpz_t rop, gmp_randstate_t state, mp_bitcnt_t n);

- mpz_t rop: Đây là biến để lưu trữ số nguyên lớn ngẫu nhiên được sinh ra. Biến này phải được khởi tạo trước khi truyền vào hàm. Trong trường hợp của bạn, bạn sử dụng result.get_mpz_t() để lấy đối tượng mpz_t từ đối tượng mpz_class result. mpz_t là kiểu dữ liệu dùng để lưu trữ số nguyên lớn trong thư viện GMP.
- gmp_randstate_t state: Đây là biến đại diện cho trạng thái của bộ sinh số ngẫu nhiên. Bạn cần khởi tạo và cấu hình trạng thái này trước khi sử dụng nó với mpz_urandomb. Trạng thái này quản lý các tham số liên quan đến thuật toán sinh số ngẫu nhiên mà bạn sử dụng. Bạn có thể sử dụng gmp_randinit_default() để khởi tạo một trạng thái ngẫu nhiên mặc định.
- mp_bitcnt_t n: Đây là kích thước của số ngẫu nhiên mà bạn muốn sinh ra, tính bằng số bit. Điều này có nghĩa là số ngẫu nhiên sẽ có độ dài n bit. Ví dụ: nếu bạn truyền vào giá trị 500 cho tham số này, hàm sẽ sinh ra một số ngẫu nhiên có độ dài 500 bit.

Đặt Bit Cao Nhất: Câu lệnh này đặt bit cao nhất (bit nhất bên trái) của số nguyên result thành 1. Điều này giúp đảm bảo rằng số nguyên có chính xác n bit, bởi vì bit cao nhất của một số nguyên n-bit không thể là 0 (nếu có bit cao nhất là 0, số đó sẽ có ít hơn n bit).

Đặt Bit Thấp Nhất Thành 0: Câu lệnh này đảm bảo rằng bit thấp nhất (bit 0) của số nguyên result được đặt thành 0. Điều này là cần thiết vì bạn muốn số này là số lẻ (số lẻ có bit thấp nhất là 1).

**1.3. Kiểm tra tính nguyên tố**

**a. Kiểm Tra Nhanh**

Trước khi áp dụng các phương pháp kiểm tra tính nguyên tố phức tạp, chương trình sử dụng hàm Quick_Check(const mpz_class &bigInteger) để kiểm tra nhanh bằng cách xác định tính chia hết của số nguyên lớn với một danh sách các số nguyên tố nhỏ (đến 97). Nếu số này chia hết cho bất kỳ số nào trong danh sách, nó sẽ bị loại bỏ ngay lập tức.

**b. Kiểm Tra Tính Nguyên Tố Bằng Thuật Toán Fermat**

Nếu số nguyên lớn vượt qua bước kiểm tra nhanh, chương trình sẽ áp dụng thuật toán Fermat để kiểm tra tính nguyên tố. Hàm Fermat_Test(const mpz_class &p, int iterations) chọn ngẫu nhiên các số nguyên a trong khoảng \[2, p-2\] và kiểm tra điều kiện a(p−1)mod  p=1a^{(p-1)} \\mod p = 1a(p−1)modp=1. Nếu điều kiện này không được thỏa mãn trong một số lần lặp nhất định, số nguyên được xem là không nguyên tố. Nếu tất cả các lần lặp đều thỏa mãn điều kiện này, số đó có khả năng là nguyên tố.

**c. Kiểm Tra Tính Nguyên Tố Bằng Thuật Toán Miller-Rabin**

Sau khi vượt qua kiểm tra Fermat, thuật toán Miller-Rabin sẽ được sử dụng để thực hiện kiểm tra tính nguyên tố mạnh mẽ hơn.

**d. Chạy Đa Luồng**

Chương trình được thiết kế để chạy đồng thời qua nhiều luồng, mỗi luồng sẽ tìm kiếm số nguyên tố độc lập. Biến toàn cục found và mutex mtx được sử dụng để quản lý truy cập vào số nguyên tố tìm được, đảm bảo rằng không có luồng nào ghi đè lên giá trị đã được tìm thấy.

# **2\. Tính ước số chung lớn nhất (GCD)**



# **3.** **Tính khóa giải mã d**

# **4\. Tạo bộ khóa ngẫu nhiên**

# **5\. Mã hóa thông điệp**

# **6\. Giải mã thông điệp mã hóa**

**MỤC LỤC**

[**1\. Tìm số nguyên tố lớn (ít nhất 500 bits)** 1](#_Toc178547453)

[**2\. Tính ước số chung lớn nhất (GCD)** 1](#_Toc178547454)

[**3.** **Tính khóa giải mã d** 1](#_Toc178547455)

[**4\. Tạo bộ khóa ngẫu nhiên** 1](#_Toc178547456)

[**5\. Mã hóa thông điệp** 1](#_Toc178547457)

[**6\. Giải mã thông điệp mã hóa** 1](#_Toc178547458)