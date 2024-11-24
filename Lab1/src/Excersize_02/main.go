package main

import (
	"fmt"
	"strings"
)

func toNumberic(ciphertext string) []int {
	ciphertext = strings.ToLower(ciphertext)
	numericSlice := []int{}
	for _, char := range ciphertext {
		if char >= 'a' && char <= 'z' {
			numericSlice = append(numericSlice, int(char-'a'))
		}
	}
	return numericSlice
}

func decrypt(ciphertext []int, key int) string {
	plaintext := strings.Builder{}
	for _, num := range ciphertext {
		decryptedChar := (num - key + 26) % 26
		plaintext.WriteRune(rune(decryptedChar + 'a'))
	}
	return plaintext.String()
}

func main() {
	ciphertext := "asvphgyt"

	numericSlice := toNumberic(ciphertext)
	fmt.Println(numericSlice)

	fmt.Println("Testing all possible keys:")
	for key := 0; key < 26; key++ {
		plaintext := decrypt(numericSlice, key)
		fmt.Printf("Key %d: %s\n", key, plaintext)
	}
}
