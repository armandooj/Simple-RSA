#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"
 
#define N 50000

int main() {

	char message[] = "Testing";
	int size = string_to_int(message, N);

	//int_to_string(integer, strlen(message));
	//printf("%s\n", test);

	return 0;
}