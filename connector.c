#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>

#include "code.h"

#define MAX 80000

//error function for reporting issues
void error(const char *m){
  perror(m);
  exit(1);
}

//encdecS came from enc_server or dec_server
int cond(char* port, char* encdecS){
  int WrongF = 0;
  int FlistenSocket, ConnectionEstab, portNum, charsRead;
  int childExitMethod = -5;
  int NumofChild = 0;
  int childPID[260];
  int uniquePid, complete, bb, ls;
  socklen_t ClientSize;
  pid_t pid, sid;
  char buff[512];
  char inFName[256];
  char keyFName[256];
  char inText[MAX];
  char key[MAX];
  char outText[MAX];
  char uniqueF[32];
  struct sockaddr_in SAddress, CAddress;
 
  //set address struct for server and clear address struct
  memset((char *)&SAddress, '\0', sizeof(SAddress));
  //convert to an integer from a string to get the port number
  portNum = atoi(port);
  //store port number in network-capable socket
  SAddress.sin_family = AF_INET;
  SAddress.sin_port = htons(portNum);
  //any address is allowed for connection
  SAddress.sin_addr.s_addr = INADDR_ANY;
  
  //create working socket
  FlistenSocket = socket(AF_INET, SOCK_STREAM, 0);
  //check if socket has opened
  if(FlistenSocket < 0){
    error("ERROR opening socket");
  }
  
  //bind socket and check if bound
  bb=bind(FlistenSocket, (struct sockaddr *)&SAddress, sizeof(struct sockaddr_in));
  if(bb< 0){
  	error("ERROR on binding");
  }//else if(bb >= 0){printf("Socked it Binded\n");}
  
  //let socket start listening, and allow max of 5 connections
  ls = listen(FlistenSocket, 5); /*if(ls==0){printf("Listening to socket\n");}else{printf("Not Listening to socket\n");}*/
  
  while(1){
    //make sure there's no zombie child
		if(NumofChild){
      complete = waitpid(childPID[NumofChild], &childExitMethod, WNOHANG);
		  //remove top element when finished
      if(complete){
        NumofChild--;
      }
    }
    
        
    //find size of address for connecting client
    ClientSize = sizeof(CAddress);
		//Accept
    /*printf("HIH\n");printf("Socket %d\n", FlistenSocket);printf("Address %p\n", &CAddress); printf("Size %d\n", ClientSize);*/
    
    ConnectionEstab = accept(FlistenSocket, (struct sockaddr*)&CAddress, &ClientSize);
    //printf("HI???? %d", ConnectionEstab);
    
    //check if connection is Established
    if(ConnectionEstab < 0){
      error("ERROR on accept");
    }//else{printf("Pass through Connection!!\n");}

 		//fork program
		pid = fork();
		switch(pid){
			case -1:{
        error("ERROR with creating fork");
				exit(0);
				break;
      }
			
			//works
			case 0:{
				memset(buff, '\0', sizeof(buff));
				//from socket read client message
				charsRead = recv(ConnectionEstab, buff, sizeof(buff)-1, 0);
				if(charsRead < 0){
          error("ERROR reading from socket");
        }
					
				//convert buff into message and key
				const char newline[2] = {'\n', '\0'};
				
				//plaintext
				char *txt = strtok(buff, newline);
				strcpy(inFName, txt);
				
				//key
				txt = strtok(NULL, newline);
				strcpy(keyFName, txt);

				//check connecting is right client/server
				txt = strtok(NULL, newline);
				if(strcmp(encdecS, txt)){
					fprintf(stderr, "ERROR %s cannot use %s", txt, encdecS );
					WrongF = 1;
				}

				if(!WrongF){
					
          //open input and key and put them into strings and remove newlines at end
					FILE* inputFile = fopen(inFName, "r");
					fgets(inText, MAX, inputFile);
					fclose(inputFile);
					inText[strcspn(inText, "\n")] = '\0';

					FILE* keyFile = fopen(keyFName, "r");
					fgets(key, MAX, keyFile);
					fclose(keyFile);
					key[strcspn(key, "\n")] = '\0';
					
					//decode or encode text
					if(strcmp(encdecS, "dec_server")){
						decode(key, inText, outText, strlen(inText));
					}
					else if(strcmp(encdecS, "enc_server")){
						encode(key, inText, outText, strlen(inText));
					}
				}	
				//write outText to new file and create new name
        uniquePid= getpid();
				sprintf(uniqueF, "%s_f.%d", encdecS, uniquePid);

				//open and write to output text or write nothing if  wrong program connects
				FILE* uniqueFD = fopen(uniqueF, "w+");
				
        if(WrongF){
    			fprintf(uniqueFD, "");
				}
        else{
  				fprintf(uniqueFD, "%s", outText);
				}
				fclose(uniqueFD);

				//send uniqueF(data) from socket with ConnectionEstab socket 
				charsRead = send(ConnectionEstab, uniqueF, strlen(uniqueF), 0);
				
        if(charsRead < 0){
          error("ERROR writing to socket");
        }
				
				//connection is closed
				close(ConnectionEstab);
				ConnectionEstab = -1;
				exit(0);

				break;
      }

			default:{
        //check child process in check
        complete = waitpid(pid, &childExitMethod, WNOHANG);
				//if child process is incomplete, store its PID in array called childPID[] to deal with later
        if(!complete){
					childPID[NumofChild++] = pid;
				}
				break;
      }
    }
    //kill zombie programs after waiting
    close(ConnectionEstab);
    ConnectionEstab = -1;
    wait(NULL);
  }

	//finished! now close socket!
	close(FlistenSocket);   
	return 0;
}