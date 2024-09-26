# CO3069_Cryptography-and-Network-Security
# Định Lý Euler

Trong lý thuyết số, định lý Euler (còn được gọi là định lý Fermat–Euler hoặc định lý totient của Euler) phát biểu rằng, nếu \( n \) và \( a \) là hai số nguyên dương nguyên tố cùng nhau, thì 

$$
a^{\varphi(n)} \equiv 1 \mod n,
$$

trong đó \( \varphi \) là hàm totient của Euler.

Năm 1736, Leonhard Euler đã công bố một bằng chứng cho định lý nhỏ của Fermat (được Fermat phát biểu mà không có bằng chứng), mà là trường hợp đặc biệt của định lý Euler khi \( n \) là một số nguyên tố. Sau đó, Euler đã trình bày các bằng chứng khác cho định lý này, culminating with his 1763 paper, trong đó ông đã chứng minh một sự tổng quát cho trường hợp \( n \) không phải là số nguyên tố.

Định lý cũng có thể được sử dụng để giảm bậc của các số lớn theo mô-đun \( n \). Ví dụ, hãy xem xét việc tìm chữ số hàng đơn vị của \( 7^{222} \), tức là \( 7^{222} \mod 10 \). Các số nguyên \( 7 \) và \( 10 \) là nguyên tố cùng nhau, và 

$$
\varphi(10) = 4.
$$

Vì vậy, định lý Euler cho phép ta viết 

$$
7^4 \equiv 1 \mod 10.
$$

Do đó,

$$
7^{222} \equiv 7^{4 \cdot 55 + 2} \equiv (7^4)^{55} \cdot 7^2 \equiv 1^{55} \cdot 7^2 \equiv 49 \equiv 9 \mod 10.
$$

Nói chung, khi giảm bậc của \( a^b \) theo mô-đun \( n \) (trong đó \( a \) và \( n \) là nguyên tố cùng nhau), ta cần làm việc theo mô-đun \( \varphi(n) \) trong bậc của \( a \):

Nếu 

$$
x \equiv y \mod \varphi(n),
$$

thì 

$$
a^x \equiv a^y \mod n.
$$

Định lý Euler là cơ sở của hệ thống mã hóa RSA, được sử dụng rộng rãi trong các giao tiếp Internet. Trong hệ thống này, \( n \) là một tích của hai số nguyên tố lớn, và sự an toàn của hệ thống dựa vào độ khó trong việc phân tích số nguyên này.

# Phương Pháp Bình Phương Lặp
Phương pháp bình phương lặp lại là một kỹ thuật toán học được sử dụng để tính nhanh lũy thừa của một số. Nó hoạt động bằng cách liên tục bình phương một số rồi nhân kết quả với số ban đầu. Quá trình này được lặp lại cho đến khi đạt được công suất mong muốn. Phương pháp này đặc biệt hữu ích khi xử lý số lượng lớn, vì nó có thể được thực hiện nhanh hơn nhiều so với các phương pháp truyền thống. Nó cũng hữu ích để tính lũy thừa của các số không phải là số nguyên, chẳng hạn như phân số hoặc số vô tỷ.

## Nguyên Tắc

Phép lũy thừa có thể được phân tích thành các phép nhân đơn giản hơn bằng cách sử dụng thuộc tính của lũy thừa:

- Nếu \( b \) là số chẵn:
  \[
  a^b = (a^{b/2})^2
  \]

- Nếu \( b \) là số lẻ:
  \[
  a^b = a \cdot a^{b-1}
  \]

## Quy Trình

1. Khởi tạo `result` bằng 1 (bởi vì bất kỳ số nào mũ 0 đều bằng 1).
2. Trong khi \( b > 0 \):
   - Nếu \( b \) là số lẻ, nhân `result` với `base` và lấy modulo \( n \).
   - Luôn bình phương `base` và lấy modulo \( n \).
   - Chia \( b \) cho 2 để cập nhật số mũ.
3. Khi \( b \) bằng 0, `result` sẽ chứa giá trị của \( (a^b) \mod n \).


Giả sử chúng ta muốn tính \( 3^{13} \mod 7 \).

### Tham số đầu vào
- \( a = 3 \)
- \( b = 13 \)
- \( n = 7 \)

### Bước 1: Khởi tạo giá trị
- Kết quả (result) ban đầu được khởi tạo bằng 1.
- Base (base) sẽ được khởi tạo bằng \( a \) (3).
- Exponent (exp) sẽ được khởi tạo bằng \( b \) (13).
- Mod (mod) sẽ được khởi tạo bằng \( n \) (7).

### Bước 2: Thực hiện các phép toán
Chúng ta sẽ theo dõi từng bước trong vòng lặp của hàm `modular_exponentiation`.

#### Vòng lặp đầu tiên:
- `exp = 13` (lẻ), nên ta thực hiện:
  - \( \text{result} = 1 \cdot 3 = 3 \)
  - \( \text{result} \mod 7 = 3 \)

- Cập nhật base:
  - \( \text{base} = 3^2 = 9 \)
  - \( 9 \mod 7 = 2 \)

- Cập nhật exp:
  - \( \text{exp} = \frac{13}{2} = 6 \)

#### Vòng lặp thứ hai:
- `exp = 6` (chẵn), không thay đổi `result`.
  
- Cập nhật base:
  - \( \text{base} = 2^2 = 4 \)
  - \( 4 \mod 7 = 4 \)

- Cập nhật exp:
  - \( \text{exp} = \frac{6}{2} = 3 \)

#### Vòng lặp thứ ba:
- `exp = 3` (lẻ), thực hiện:
  - \( \text{result} = 3 \cdot 4 = 12 \)
  - \( 12 \mod 7 = 5 \)

- Cập nhật base:
  - \( \text{base} = 4^2 = 16 \)
  - \( 16 \mod 7 = 2 \)

- Cập nhật exp:
  - \( \text{exp} = \frac{3}{2} = 1 \)

#### Vòng lặp thứ tư:
- `exp = 1` (lẻ), thực hiện:
  - \( \text{result} = 5 \cdot 2 = 10 \)
  - \( 10 \mod 7 = 3 \)

- Cập nhật base:
  - \( \text{base} = 2^2 = 4 \)
  - \( 4 \mod 7 = 4 \)

- Cập nhật exp:
  - \( \text{exp} = \frac{1}{2} = 0 \)

### Kết quả
Khi `exp = 0`, vòng lặp kết thúc. Giá trị cuối cùng của `result` là 3. Do đó, 

\[
3^{13} \mod 7 = 3.
\]
