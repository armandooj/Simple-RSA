#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"

int main() {

	// Alice generates private and public keys
	mpz_t e, n, d;
	mpz_init(e);
	mpz_init(n);
	mpz_init(d);
	generate_keys(&e, &n, &d);

	// Bob wants to send a message. Use Alice's public key
	//char message[] = "Here you go, Alice!";
	char message[] = "I'm normally not a praying man, but if you're up there, please save me, Superman. Oh, a *sarcasm* detector. Oh, that's a *really* useful invention!";
	//char message[] = "I'm normally not a praying man";
	mpz_t *c = malloc(sizeof(mpz_t) * strlen(message) + 1);	
	rsa_encrypt(e, n, message, c);

	// Alice decrypts the message with her private key
	char *buffer = malloc(sizeof(char) * strlen(message));
	rsa_decrypt(d, n, c, buffer);
	printf("Message: %s\n", buffer);

	free(c);
	free(buffer);
	mpz_clear(e);
	mpz_clear(n);
	mpz_clear(d);
	return 0;
}