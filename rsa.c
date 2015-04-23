#include <stdio.h>
#include <string.h>
#include <math.h>

// Converts a string to an array of integers bewteen 0 and n - 1
int string_to_int(char *s, int n) {	
	int s_len = strlen(s);
	long aux;
	unsigned int num, i, j = 0;

	// Calculate the the size of the chunks
	aux = 0;
	int size = 1;
	while (n >= (256 << aux)) {
		printf("-> %d\n", 256 << aux);
		size++;
		aux += 8;
	}

	int integers[size];
	printf("Chunk size: %d\n", size);

	// Extract 'size' characters from the string
	int letters;
	for (i = 0; i < s_len; i++) {
		letters = 0;
		num = 0;
		aux = 0;
		for (j = i; j < ((i + size) < s_len ? (i + size) : s_len); j++) {
			printf("(%c)", s[j]);
			num += ((int) s[j]) * pow(256, size - aux - 1);
			letters++;
			aux++;
		}
		i += letters - 1;
		printf(" Num: %d\n", num);
	}	
	
	return size;
}

void int_to_string(int i, int n) {
	char s[n];
	int aux = n;
	int j;
	for (j = 0; j < n; j++) {
		// Unshift the bits
		s[--aux] = (char) (i >> 8 * j) % 256;
	}

	// printf("-> %s\n", s);
	// return &s;
}