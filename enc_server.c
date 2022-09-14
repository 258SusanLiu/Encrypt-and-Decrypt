#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"

int main(int argn, char *argc[]) {
  
  //Check arguments
  //enc_server randnum
	if (argn < 2) {
		fprintf(stderr,"USAGE:  %s  port\n", argc[0]);
		exit(1);
  }

  //printf("IN ENC_S\n");
	//Call cond() and tell it to encode
	cond(argc[1], "enc_server");

	return 0;
}