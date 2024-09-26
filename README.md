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

