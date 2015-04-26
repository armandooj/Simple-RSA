#include <gmp.h>

int string_to_int(char *s, mpz_t n, mpz_t *out);
void int_to_string(mpz_t *integers, int i_size, mpz_t n, char *out);