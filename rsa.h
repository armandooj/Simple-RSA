#include <gmp.h>

int string_to_int(char *s, mpz_t n, unsigned int *out);
void int_to_string(unsigned int *integers, int i_size, mpz_t n, char *out);