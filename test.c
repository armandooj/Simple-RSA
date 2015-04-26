#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"

#define N 80000000000

int main() {

	// Using GMP
	mpz_t n;
	mpz_init(n);
	mpz_set_ui(n, N);

	char message[] = "I'm normally not a praying man, but if you're up there, please save me, Superman. Oh, a *sarcasm* detector. Oh, that's a *really* useful invention!";
	mpz_t *mpz_t_array = malloc(sizeof(mpz_t) * strlen(message));
	
	int no_elements = string_to_int(message, n, mpz_t_array);
	char buffer[strlen(message)];	

	int_to_string(mpz_t_array, no_elements, n, buffer);	
	printf("Message: %s\n", buffer);	

	mpz_clear(n);
	return 0;
}