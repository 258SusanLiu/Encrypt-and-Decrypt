#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "code.h"

int main (int argn, char* argc[]) {
	
	int i, length, letter;
	
  length = atoi(argc[1]);
	char key[length+1];
	srand(time(0));

	//assign random value to each char
	for (i=0; i<length; i++) {
		letter = rand() % 27;
		key[i] = letters[letter];
	}
 
	key[length] = '\0';
	
	printf("%s\n", key);

	return 0;
}