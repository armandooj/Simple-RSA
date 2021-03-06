#include <gmp.h>

void exp_mod(mpz_t x, mpz_t k, mpz_t n, mpz_t *out);
void inverse_mod(mpz_t x, mpz_t n, mpz_t *out);
int miller_rabin_test(mpz_t n, double k);
void generate_robust_prime(unsigned long b, mpz_t *p, gmp_randstate_t *r_state);
int string_to_int(char *s, mpz_t n, mpz_t *out);
void int_to_string(mpz_t *integers, mpz_t n, char *out);
void generate_keys(mpz_t *e, mpz_t *n, mpz_t *d);
void rsa_encrypt(mpz_t e, mpz_t n, char *s, mpz_t *c);
void rsa_decrypt(mpz_t d, mpz_t n, mpz_t *c, char *out);