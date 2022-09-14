#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"

int main(int argn, char *argc[]) {
  
  //Check arguments
  //enc_client plaintext# key randnum
	if (argn < 4) {
		fprintf(stderr,"USAGE:  %s  plaintext  key  port\n", argc[0]);
		exit(0);
	}
  //printf("IN ENC_C\n");
	//all con() and tell it to encode
  //printf("%s\n%s\n%s\n",argc[1], argc[2], argc[3]);
  //connects
	con(argc[1], argc[2], argc[3], "enc_server");

	return 0;
}
