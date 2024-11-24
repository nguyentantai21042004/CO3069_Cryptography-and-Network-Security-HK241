package main

import (
	"crypto/sha1"
	"encoding/hex"
	"fmt"
)

func main() {
	// Duyệt qua từng ký tự trong bảng chữ cái tiếng Anh
	for char := 'A'; char <= 'Z'; char++ {
		// Chuyển ký tự thành chuỗi
		text := string(char)

		// Tạo SHA-1 hash của ký tự
		hash := sha1.New()
		hash.Write([]byte(text))
		hashInBytes := hash.Sum(nil)

		// Chuyển hash thành chuỗi hexadecimal
		hashString := hex.EncodeToString(hashInBytes)

		// In ra ký tự và giá trị SHA-1 của nó
		fmt.Printf("%c: %s\n", char, hashString)
	}
}
