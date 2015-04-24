#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"

// #define N 50000
#define N 80000

int main() {

	char message[] = "Testing";
	unsigned int *int_array = malloc(sizeof(unsigned int) * strlen(message));
	
	int no_elements = string_to_int(message, N, int_array);

	char buffer[strlen(message)];
	int_to_string(int_array, no_elements, N, buffer);
	
	printf("Message: %s\n", buffer);

	return 0;
}