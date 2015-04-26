#include <gmp.h>

void inverse_mod(mpz_t x, mpz_t n, mpz_t *out);
int miller_rabin_test(mpz_t n, int k);
int string_to_int(char *s, mpz_t n, mpz_t *out);
void int_to_string(mpz_t *integers, int i_size, mpz_t n, char *out);