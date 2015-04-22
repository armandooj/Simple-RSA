#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"

#define N 100

int main() {

	char message[] = "Test";
	int *integers;

	integers = string_to_int(message, N);
	printf("%s\n", message);

	// http://math.stackexchange.com/questions/217816/converting-integer-to-ascii-string

	return 0;
}