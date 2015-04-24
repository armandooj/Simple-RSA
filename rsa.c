#include <stdio.h>
#include <string.h>
#include <math.h>

int calculate_int_size(int n) {
	int aux = 0;
	int size = 1;
	while (n >= (256 << aux)) {
		// printf("-> %d\n", 256 << aux);
		size++;
		aux += 8;
	}
	return size;
}

// Converts a string to an array of integers bewteen 0 and n - 1
int string_to_int(char *s, int n, unsigned int *out) {	
	int s_len = strlen(s);
	long aux;
	unsigned int num, i, j = 0;

	// Calculate the the size of the chunks
	int size = calculate_int_size(n);

	int integers[size];
	printf("Chunk size: %d\n", size);

	// Extract 'size' characters from the string
	int pos = 0;
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
		out[pos++] = num;
		printf(" Num: %d\n", num);
	}	
	
	return pos;
}

void int_to_string(unsigned int *integers, int i_size, int n, char *out) {	
	int size = calculate_int_size(n);
	int i, aux, pos = 0;	
	for (i = 0; i < i_size; i++) {
		int j;
		for (j = size - 1; j >= 0; j--) {
			// Unshift the bits
			char c = (char) (integers[i] >> 8 * j) % 256;
			out[pos++] = c;
		}	
	}
}