#include <stdio.h>
#include <string.h>

#include "code.h"

//convert a character to integer based on letters[] (in code.h)
void char2Int(char in[], int out[], int size) {
	for (int i=0; i<size; i++) {
		for (int j=0; j<27; j++) {
			if (in[i] == letters[j]) {
				out[i] = j;
				break;
			}
		}
	}
}

//covert an integer to character based on letters[] (in code.h)
void int2Char(int in[], char out[], int size) {
	for (int i=0; i<size; i++) {
		out[i] = letters[in[i]];
	}
}

//these two functions are used in connector in cond()
void encode(char key[], char in[], char out[], int size) {

	//convert input array to integers
	int intIn[size];
	char2Int(in, intIn, size);

	//convert key to integers
	int intKey[size];
	char2Int(key, intKey, size);

	//generate output in integer form
	int intOut[size];
	for (int i=0; i<size; i++) {
		intOut[i] = (intIn[i] + intKey[i]) % 27;
	}

	//convert output to characters
	int2Char(intOut, out, size);
	out[size] = '\0';
}

void decode(char key[], char in[], char out[], int size) {

	//convert input array to integers
	int intIn[size];
	char2Int(in, intIn, size);

	//convert key to integers
	int intKey[size];
	char2Int(key, intKey, size);

	//generate output in integer form
	int intOut[size];
	for (int i=0; i<size; i++) {
		intOut[i] = intIn[i] - intKey[i];
		if (intOut[i] < 0) intOut[i] += 27;
	}

	//convert output to characters
	int2Char(intOut, out, size);
	out[size] = '\0';
}
