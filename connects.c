#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "code.h"

#define MAX 80000

//error function used for reporting issues
void error(const char *m, int value){
  perror(m);
  //printf("ERROREEE\n");
  exit(value);
}

//encdecC came from enc_client or dec_client
int con(char* plaintext, char* key, char* port, char* encdecC){
	int socketCreate, portNum, Writtenchars, charsRead;
  int keyLength, textLength, p;
	char txt_plaintext[MAX];
	char textkey[MAX];
	char output[MAX];
	char buff[512];
  char filename[50];
  
	struct sockaddr_in serverAdd;
	struct hostent* serverHInfo;
  
  
	p = atoi(port);
  //printf("%d\n", port);

	//check size of key isnt shorter than size of plaintext, then get text from plaintext file
	FILE* textFile = fopen(plaintext, "r");
  if(textFile == 0){
    printf("Did not open file %s",plaintext);
  }
  fgets(txt_plaintext, MAX, textFile);
	fclose(textFile);
	txt_plaintext[strcspn(txt_plaintext, "\n")] = '\0';

	//get text from key file
	FILE* keyFile = fopen(key, "r");
  if(keyFile == 0){
    printf("Did not open file %s",key);
  }
	fgets(textkey, MAX, keyFile);
	fclose(keyFile);
	textkey[strcspn(textkey, "\n")] = '\0';
	
	//check string length
	textLength = strlen(txt_plaintext);
	keyLength = strlen(textkey);
  
	if(keyLength < textLength){
		fprintf(stderr, "ERROR key is shorter than plaintext\n");
		exit(1);
	}
	
	//loop and check for valid characters	
	//plaintext
	for(int i=0; i<textLength; i++){
		for(int j=0; j<28; j++){
			if(j == 27){
				fprintf(stderr, "ERROR %s contains invalid characters\n", plaintext);
				exit(1);
			}
			if(txt_plaintext[i] == letters[j]){
    		break;
			}
		}
	}

	//dey
	for(int i=0; i<keyLength; i++){
		for(int j=0; j<28; j++){
			if(j == 27){
				fprintf(stderr, "%s contains invalid characters\n", key);
			}
			if(textkey[i] == letters[j]){
				break;
			}
		}
	}
  
  //set up the server address struct clear out the address struct
  memset((char*)&serverAdd, '\0', sizeof(serverAdd));
	//get the port number and convert integer from string
  portNum = p;
	//printf("%d f\n", portNum);
 
	//convert the machine name into a special form of address
  serverHInfo = gethostbyname("localhost");
  if(serverHInfo == NULL){
	  fprintf(stderr, "CLIENT: ERROR no such host\n");
		exit(0);
	}
  //else{printf("LOL \n");}
  
	//copy in the address
	memcpy((char*)&serverAdd.sin_addr.s_addr, (char*)serverHInfo->h_addr, serverHInfo->h_length);
  
  //create network-capable socket and store port number
  serverAdd.sin_family = AF_INET;
  serverAdd.sin_addr.s_addr = INADDR_ANY;
  serverAdd.sin_port = htons(p);
 

  //set up and create socket
	socketCreate = socket(AF_INET, SOCK_STREAM, 0); 
	if(socketCreate < 0){
    error("CLIENT: ERROR opening socket", 1);
  }
	//printf("%s\n%s\n camefrom %s\n", plaintext, key, encdecC);
 
	//connect to server and connect socket to address
	if(connect(socketCreate, (struct sockaddr *)&serverAdd, sizeof(serverAdd)) < 0){
    error("CLIENT: ERROR connecting", -1);
  }
  //else{printf("HIHI\n");}
  
  
	//splice the message together will be separated by newline 
	memset(buff, '\0', sizeof(buff));
  sprintf(buff, "%s\n%s\n%s", plaintext, key, encdecC);
  
  //printf("%s\n",buff); printf("PIEEE\n");
  //send the message over write to the server
	Writtenchars = send(socketCreate, buff, sizeof(buff), 0);
  //printf("h %s   h\n",buff);
  if(Writtenchars < 0){
    error("CLIENT: ERROR writing to socket", 1);
  }
 	if(Writtenchars < sizeof(buff)){
    printf("CLIENT: WARNING not all data written to socket!\n");
  }
  
  
	//get message from server and clear the buff for reuse
	memset(buff, '\0', sizeof(buff));
  //sprintf(buff, "%s\n%s\n%s", plaintext, key, encdecC);
  //printf("s %s s\n", buff);
  //printf("PIEEE\n");
  
	//Read data from the socket leaving \0 at end
	charsRead = recv(socketCreate, buff, sizeof(buff), 0);

	//printf("%d\n", charsRead);
  if(charsRead< 0){
    error("CLIENT: ERROR reading from socket\n", 1);
  }
  
	//Open file returned and print it out!
	FILE* recvFile = fopen(buff, "r");
  fgets(output, MAX, recvFile);
	printf("%s\n", output);
  fclose(recvFile);
  //printf("HId\n");
  
	//Delete the file
	remove(buff);

	//Print it out and wrap up.
	//printf("outs %s\n", output);
	//printf("HI %s\n",buff);
  close(socketCreate);
	return 0;
}
