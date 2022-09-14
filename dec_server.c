#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"

int main(int argn, char *argc[]) {
  
  //check arguments
  //dec_server randnum
	if (argn < 2) {
		fprintf(stderr,"USAGE:  %s  port\n", argc[0]);
		exit(1);
	}

	//call cond() and tell it to decode
	cond(argc[1], "dec_server");

	return 0;
}