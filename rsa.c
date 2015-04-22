#include <stdio.h>
#include <string.h>
#include <math.h>

// Converts a string to an array of integers bewteen 0 and n - 1
int string_to_int(char *s, int n) {	
	int s_len = strlen(s);
	int num = 0;
	int i;
	for (i = 0; i < s_len; i++) {
		num += ((int) s[i]) * pow(256, s_len - i - 1);
	}
	printf("%d\n", num);
	return 0;
}