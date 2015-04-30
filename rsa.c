#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "rsa.h"

//#define NUMBER_SIZE 8000000000000000
#define NUMBER_SIZE 100000000
#define STRONG_PRIME_SIZE 5
#define MILLER_RABIN_PROB 5

void inverse_mod(mpz_t x, mpz_t n, mpz_t *out) {
	// Returns non-zero if an inverse exists, zero otherwise
	mpz_invert(*out, x, n);
	// gmp_printf("%Zd ^ -1 mod %Zd =  %Zd \n", x, n, out);
}

void exp_mod(mpz_t x, mpz_t k, mpz_t n, mpz_t *out) {
	mpz_powm(*out, x, k, n);
	// gmp_printf("%Zd ^ %Zd mod %Zd = %Zd \n", x, k, n, out);
}

/*
Primality check
Input: n > 3, an odd integer to be tested for primality
Input: k, accuracy of the test -> 0 < k < 1
Output: 0 if n is composite, 1 probably prime
*/
int miller_rabin_test(mpz_t n, int k) {
	int s = 1;
	long aux;
	mpz_t d, n_1, temp;
	mpz_init(d);
	mpz_init(n_1);
	mpz_init(temp);
	mpz_set_ui(temp, 0);
	mpz_sub_ui(n_1, n, 1);
	
	// Find an odd d by factoring powers of 2 from n − 1 
	// S.t n − 1 = 2^s * d 
	while (mpz_get_ui(temp) != 1) {
		aux = pow(2, s);
		// d = (n - 1) / 2^r
		mpz_tdiv_q_ui(d, n_1, aux);		
		mpz_mod_ui(temp, d, 2);
		s++;
	}
	s--;

	gmp_randstate_t r_state;
	gmp_randinit_default(r_state);
	gmp_randseed_ui(r_state, time(NULL));

	int i, j;	
	for (i = 0; i < k; i++) {
	    mpz_t a;
	    mpz_init(a);
	    mpz_set_ui(a, 0);
		
		// Pick a random integer a in the range [2, n − 2]		
		while (mpz_cmp_ui(a, 1) < 0) {
			mpz_urandomm(a, r_state, n_1);
		}

		short flag;
		// x ← a^d mod n
		mpz_t x;
		mpz_init(x);
		exp_mod(a, d, n, &x);

		 // if x = 1 or x = n − 1 then do the for loop again
		if (!(mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, n_1) == 0)) {
			flag = 1;
			// S - 1 times
			for (j = 0; j < s; j++) {
				// x ← x^2 mod n
				mpz_powm_ui(x, x, 2, n);
				// if x = 1 then return composite      
				if (mpz_cmp_ui(x, 1) == 0) {
					// Composite
					return 0;
				} 
				// if x = n − 1 then do the for loop again
				else if (mpz_cmp(x, n_1) == 0) {
					j = s;
					flag = 0;
				}      			
			}
			// Composite
			if (flag) return 0;
		}

		mpz_clear(a);
		mpz_clear(x);
	}

	mpz_clear(d);
	mpz_clear(n_1);
	mpz_clear(temp);
	// Probably prime
	return 1;
}

// Ensures n is > 3 by generating random numbers until the condition is met
void miller_rabin_check(gmp_randstate_t *state, unsigned long b, mpz_t *n) {		
	while ((mpz_cmp_ui(*n, 3) <= 0)) {
		// Generates a random from 0 to b bits
		mpz_urandomb(*n, *state, b);
	}
}

// Randomly finds a prime number of b bits
void get_prime(gmp_randstate_t *state, unsigned long b, mpz_t *p) {
	miller_rabin_check(state, b, p);
	while (miller_rabin_test(*p, MILLER_RABIN_PROB) != 1) {
		mpz_urandomb(*p, *state, b);
		miller_rabin_check(state, b, p);
	}
}

/* 
Implementation of Gordon's algorithm for generating a strong prime
A prime number is said to be a strong primeif integers r, s and t exist s.t.:
 * p - 1 has a large prime factor, r
 * p + 1 has a large prime factor, s
 * r - 1 has a large prime factor, t
*/
void generate_robust_prime(unsigned long b, mpz_t *p, gmp_randstate_t *r_state) {
	// Generate two prime numbers s, t of b bits
	mpz_t s, t;
	mpz_init(s);
	mpz_init(t);
	get_prime(r_state, b, &s);
	get_prime(r_state, b, &t);

	// gmp_printf("s: %Zd\n", s);
	// gmp_printf("t: %Zd\n", t);

	// Find the smallest k s.t. r = 2kt + 1 is prime
	mpz_t r, temp;
	mpz_init(r);
	mpz_init(temp);

	mpz_mul_ui(r, t, 1);
	mpz_mul_ui(r, r, 2);
	mpz_add_ui(r, r, 1);
	mpz_mul_ui(temp, t, 2);	

	while (miller_rabin_test(r, MILLER_RABIN_PROB) != 1) {
		// r += 2 * t
		mpz_add(r, r, temp);
	}

	// Compute l = 2(s^(r − 2) mod r)s − 1
	mpz_t l;
	mpz_init(l);

	mpz_sub_ui(l, r, 2);
	mpz_powm(l, s, l, r);
	mpz_mul_ui(l, l, 2);
	mpz_mul(l, l, s);
	mpz_sub_ui(l, l, 1);

	// Find the smallest h s.t. p = l + 2hrs is prime
	mpz_mul_ui(*p, r, 2);
	mpz_mul(*p, *p, s);
	mpz_add(*p, *p, l);

	while (miller_rabin_test(*p, MILLER_RABIN_PROB) != 1) {
		// p += 2 * r * s
		mpz_t aux;
		mpz_init(aux);
		mpz_mul_ui(aux, r, 2);
		mpz_mul(aux, aux, s);		
		mpz_add(*p, *p, aux);
		mpz_clear(aux);
	}

	// gmp_printf("Strong p: %Zd\n", p);

	mpz_clear(s);
	mpz_clear(t);
	mpz_clear(r);	
	mpz_clear(l);

	// Make sure the size is at least 2b + 1
	mpz_set_ui(temp, 2);
	mpz_pow_ui(temp, temp, STRONG_PRIME_SIZE * 2 + 1);
	if (mpz_cmp(*p, temp) <= 0) {		
		generate_robust_prime(b, p, r_state);
	}

	mpz_clear(temp);
}

// Finds the number of letters that would fit in n
// TODO Should it be ceil(log_2(n)) ?
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

	mpz_clear(aux);
	return size - 1;
}

// Converts a string to an array of integers bewteen 0 and n - 1
int string_to_int(char *s, mpz_t n, mpz_t *out) {	
	int s_len = strlen(s);
	int aux;
	unsigned i, j = 0;
	
	mpz_t num, temp, base_256;
	mpz_init(num);
	mpz_init(temp);
	mpz_init(base_256);
	mpz_set_ui(base_256, 256);

	// Calculate the the size of the chunks
	int size = calculate_int_size(n);

	int integers[size];
	printf("Chunk size: %d\n", size);

	// Extract 'size' characters from the string
	int pos = 0;
	int letters;
	for (i = 0; i < s_len; i++) {
		letters = 0;
		mpz_set_ui(num, 0);
		aux = 0;
		// num += ((int) s[j]) * pow(256, size - aux - 1);
		for (j = i; j < ((i + size) < s_len ? (i + size) : s_len); j++) {
			printf("[%c]", s[j]);
			// temp = pow(256, size - aux - 1)
			mpz_pow_ui(temp, base_256, size - aux - 1);
			// temp *= (int) s[j];
			mpz_mul_ui(temp, temp, (int) s[j]);
			// num += temp;
			mpz_add(num, num, temp);			
			letters++;
			aux++;
		}
		i += letters - 1;
		mpz_init(out[pos]);
		mpz_set(out[pos], num);
		pos++;
		gmp_printf(" Num: %Zd\n", num);
	}

	// Write a 0 a the end so that the conversion to string knows when to stop
	mpz_init(out[pos]);
	mpz_set_ui(out[pos], 0);
	
	mpz_clear(num);
	mpz_clear(temp);
	mpz_clear(base_256);
	return pos;
}

void int_to_string(mpz_t *integers, mpz_t n, char *out) {	
	int size = calculate_int_size(n);
	int aux, i = 0, pos = 0;	

	while (mpz_cmp_ui(integers[i], 0) != 0) {
		int j;
		for (j = size - 1; j >= 0; j--) {
			// Unshift the bits
			mpz_t temp;
			mpz_init(temp);
			// temp = integers[i] >> (8 * j)
			mpz_fdiv_q_2exp(temp, integers[i], 8 * j);
			// temp %= 256;
			mpz_mod_ui(temp, temp, 256);			
			// Store the letter in the char array
			out[pos++] = (char) mpz_get_ui(temp);
			mpz_clear(temp);
		}
		i++;	
	}
}

void generate_keys(mpz_t *e, mpz_t *n, mpz_t *d) {
	// Generate 2 prime numbers, p and q
	mpz_t p, q, p_1, q_1, phi_n;
	mpz_init(p);
	mpz_init(q);
	mpz_init(p_1);
	mpz_init(q_1);
	mpz_init(phi_n);

	gmp_randstate_t r_state;
	gmp_randinit_default(r_state);
	gmp_randseed_ui(r_state, time(NULL));

	generate_robust_prime(STRONG_PRIME_SIZE, &p, &r_state);
	generate_robust_prime(STRONG_PRIME_SIZE, &q, &r_state);

	gmp_printf("Strong p: %Zd\n", p);
	gmp_printf("Strong q: %Zd\n", q);

	// n = p * q
	mpz_mul(*n, p, q);
	// phi_n = (p - 1)(q - 1)
	mpz_sub_ui(p_1, p, 1);
	mpz_sub_ui(q_1, q, 1);
	mpz_mul(phi_n, p_1, q_1);

	// Find a public exponent
	mpz_set_ui(*e, 17);
	mpz_t temp;
	mpz_init(temp);
	mpz_gcd(temp, *e, phi_n);
	while (mpz_cmp_ui(temp, 1) != 0) {
		gmp_printf("e not prime %Zd\n", e);
		mpz_add_ui(*e, *e , 2);
	}

	// d = e^-1 mod phi_n
	mpz_invert(*d, *e, phi_n);

	gmp_printf("Public key (e, n) -> (%Zd, %Zd)\n", *e, *n);
	gmp_printf("Private key d -> %Zd\n", *d);

	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(p_1);
	mpz_clear(q_1);
	mpz_clear(phi_n);
	mpz_clear(temp);
}

// Encrypts a message with the given public key and writes the ciphered message to out
void rsa_encrypt(mpz_t e, mpz_t n, char *s, mpz_t *c) {
	// Conver to an array of integers
	mpz_t num_size;
	mpz_init(num_size);
	mpz_set_ui(num_size, NUMBER_SIZE);
	mpz_t *mpz_t_array = malloc(sizeof(mpz_t) * strlen(s));	
	string_to_int(s, num_size, mpz_t_array);	

	// Encrypt each part -> c = M^e mod n
	int i = 0;
	while (mpz_cmp_ui(mpz_t_array[i], 0) != 0) {
		mpz_init(c[i]);
		mpz_powm(c[i], mpz_t_array[i], e, n);
		gmp_printf("c -> %Zd of (%Zd)\n", c[i], mpz_t_array[i]);
		i++;
	}

	mpz_init(c[i]);
	mpz_set_ui(c[i], 0);
	free(mpz_t_array);
	mpz_clear(num_size);
}

void rsa_decrypt(mpz_t d, mpz_t n, mpz_t *c, char *out) {
	// Decrypt each part -> M = c^d mod n
	int i = 0;
	mpz_t temp;
	// TODO Use the right size
	mpz_t *integers = malloc(sizeof(mpz_t) * 1000);
	mpz_init(temp);
	// Write the numbers to an array
	while (mpz_cmp_ui(c[i], 0) != 0) {
		mpz_powm(temp, c[i], d, n);
		mpz_set(integers[i], temp);
		//integers[i] = mpz_get_ui(temp);
		gmp_printf("M -> %Zd\n", temp);
		i++;
	}

	// Convert back to string
	mpz_set_ui(temp, NUMBER_SIZE);
	int_to_string(integers, temp, out);

	free(integers);
	mpz_clear(temp);
}
