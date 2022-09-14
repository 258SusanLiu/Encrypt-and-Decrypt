#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"

int main(int argn, char *argc[]) {
  
  //check arguments
  //dec_client plaintext# key randnum
	if (argn < 4) {
		fprintf(stderr,"USAGE:  %s  plaintext  key  port\n", argc[0]);
		exit(0);
	}

	//call con() and tell it to decode
	con(argc[1], argc[2], argc[3], "dec_server");

	return 0;
}