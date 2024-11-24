package main

import (
	"fmt"
	"sort"
	"strings"
)

// frequency returns a map of the frequency of each character in the string
func frequency(s string) map[rune]int {
	frequency := make(map[rune]int)

	for _, char := range s {
		frequency[char]++
	}

	return frequency
}

// topFrequencies returns the top N most frequent characters in the frequency map
func topFrequencies(freqMap map[rune]int, topN int) []rune {
	type kv struct {
		Key   rune
		Value int
	}
	var freqSlice []kv
	for k, v := range freqMap {
		freqSlice = append(freqSlice, kv{k, v})
	}
	sort.Slice(freqSlice, func(i, j int) bool {
		return freqSlice[i].Value > freqSlice[j].Value
	})
	topKeys := []rune{}
	for i := 0; i < topN && i < len(freqSlice); i++ {
		topKeys = append(topKeys, freqSlice[i].Key)
	}
	return topKeys
}

// decryptCaesar decrypts a Caesar cipher with the given shift
func decryptCaesar(ciphertext string, shift int) string {
	plaintext := strings.Builder{}
	for _, char := range ciphertext {
		if char >= 'A' && char <= 'Z' {
			decryptedChar := (char-'A'-rune(shift)+26)%26 + 'A'
			plaintext.WriteRune(decryptedChar)
		} else {
			plaintext.WriteRune(char)
		}
	}
	return plaintext.String()
}

func main() {
	input := "KNXMNSLKWJXMBFYJWGJSIXFIRNYXBTWIKNXMWFSITAJWMJQRNSLFSDIFD"

	frequencyMap := frequency(input)

	topKeys := topFrequencies(frequencyMap, 5)
	fmt.Println("Top 5 characters with highest frequency:")
	for _, char := range topKeys {
		fmt.Printf("Character: '%c', Frequency: %d\n", char, frequencyMap[char])
	}

	fmt.Println("Top 5 characters with highest frequency and assumed decryption:")
	for _, char := range topKeys {
		shift := (int(char-'A') - int('E'-'A') + 26) % 26
		fmt.Printf("Assuming '%c' is 'E' (shift %d):\n", char, shift)
		plaintext := decryptCaesar(input, shift)
		fmt.Println("Decrypted text:", plaintext)
		fmt.Println()
	}
}
