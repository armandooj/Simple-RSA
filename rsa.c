#include <stdio.h>
#include <string.h>
#include <math.h>
#include "rsa.h"

// Finds the number of letters that would fit in n
int calculate_int_size(mpz_t n) {
	// The smallest a part can be is of 1 (one char)
	if (mpz_cmp_d(n, 256) < 0) {
		return 1;
	}
	mpz_t aux;
	mpz_init(aux);
	mpz_set_ui(aux, 2);
	int size = 0;
	int ex = 2;
	// Equivalent to: while (n >= (256 << aux))
	while (mpz_cmp(n, aux) > 0) {	
		mpz_ui_pow_ui(aux, 16, ex);
		size++;
		ex += 2;
	}
	return size - 1;
}

// Converts a string to an array of integers bewteen 0 and n - 1
int string_to_int(char *s, mpz_t n, unsigned int *out) {	
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

void int_to_string(unsigned int *integers, int i_size, mpz_t n, char *out) {	
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