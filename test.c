#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"

#define N 8000000000000000

int main() {

	/*
	// Using GMP
	mpz_t n;
	mpz_init(n);
	mpz_set_ui(n, N);

	char message[] = "I'm normally not a praying man, but if you're up there, please save me, Superman. Oh, a *sarcasm* detector. Oh, that's a *really* useful invention!";
	//char message[] = "Test";
	mpz_t *mpz_t_array = malloc(sizeof(mpz_t) * (strlen(message) + 1));
	
	int no_elements = string_to_int(message, n, mpz_t_array);
	
	char *buffer = malloc(sizeof(char) * strlen(message));
	int_to_string(mpz_t_array, n, buffer);
	
	printf("\nMessage: %s\n", buffer);

	mpz_t x, n2, inv;
	mpz_init(x);
	mpz_init(n2);
	mpz_init(inv);
	mpz_set_ui(x, 197);
	mpz_set_ui(n2, 3000);
	inverse_mod(x, n2, &inv);

	mpz_t p;
	mpz_init(p);
	mpz_set_ui(p, 919);
	// reasonable values vary from 5 to 10
	int prime = miller_rabin_test(p, 3);
	gmp_printf("%Zd prime? = %d\n", p, prime);

	// Gordon's
	generate_robust_prime(130, &p);

	free(mpz_t_array);
	free(buffer);
	mpz_clear(n);
	*/

	// Alice generates private and public keys
	mpz_t e, n, d;
	mpz_init(e);
	mpz_init(n);
	mpz_init(d);
	generate_keys(&e, &n, &d);

	// Bob wants to send a message. Use Alice's public key
	//char message[] = "I'm normally not a praying man, but if you're up there, please save me, Superman. Oh, a *sarcasm* detector. Oh, that's a *really* useful invention!";
	char message[] = "I'm normally not a praying man";
	mpz_t *c = malloc(sizeof(mpz_t) * strlen(message) + 1);	
	rsa_encrypt(e, n, message, c);

	// Alice decrypts the message with her private key
	char *buffer = malloc(sizeof(char) * strlen(message));
	rsa_decrypt(d, n, c, buffer);
	printf("Message: %s\n", buffer);

	return 0;
}