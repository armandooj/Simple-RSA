#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"

#define N 80000000

int main() {

	// Using GMP
	mpz_t n;
	mpz_init(n);
	mpz_set_ui(n, N);

	char message[] = "Testing this out.";
	unsigned int *int_array = malloc(sizeof(unsigned int) * strlen(message));
	
	int no_elements = string_to_int(message, n, int_array);

	char buffer[strlen(message)];
	int_to_string(int_array, no_elements, n, buffer);
	
	printf("Message: %s\n", buffer);

	mpz_clear(n);
	return 0;
}