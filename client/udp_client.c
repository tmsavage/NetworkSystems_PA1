/*
 * udpclient.c - A simple UDP client
 * usage: udpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 1024


/*
 * create function to send file
 */


/*
 * create function to receive file
 */


/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
  int sockfd, portno, n;
	int serverlen;
	struct sockaddr_in serveraddr;
	struct hostent *server;
	char *hostname;
	char buf[BUFSIZE];
	int flag = 1;
	char c;
	//char fileName[256];
	int b;


	/* check command line arguments */
	if (argc != 3) {
	fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]); // hostname: localhost
	exit(0);
	}
	hostname = argv[1];
	portno = atoi(argv[2]);


	/* socket: create the socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	error("ERROR opening socket");


	/* gethostbyname: get the server's DNS entry */
	server = gethostbyname(hostname);
	if (server == NULL) {
	fprintf(stderr,"ERROR, no such host as %s\n", hostname);
	exit(0);
	}

	/* build the server's Internet address */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(portno);


  while(1){
    /* get a message from the user */
    bzero(buf, BUFSIZE);
    printf("Please enter any of the following commands: \n");
    printf("\"get <filename>\" retrieves file from server \n");
    printf("\"put <filename>\" puts file to the server \n");
    printf("\"delete <filename>\" deletes file on the server \n");
    printf("\"ls\" displays the directory on the server \n");
    printf("\"exit\" quits the program \n");
    printf("\n");
    printf("Command: ");
    fgets(buf, BUFSIZE, stdin);


    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
    if (n < 0)
      error("ERROR in sendto");

    // Replace while loop for invalid commands around packet header if possible
    /*
    * Packet Header orientation
    */
    char* option = strtok(buf, " ");  // command option from user
    char* fileName = strtok(NULL, " "); // file name from user
    option = strtok(option, "\n");
    char* newFileName = strtok(fileName, "\n"); //eliminates '\n' character at end of fileName string

    // printf("IN BUFFER BEFORE FILE: ");
    // fwrite(buf, BUFSIZE, 1, stdout);

    // send file from client to server
    if(strcmp("put", option) == 0){
      printf("%s \n", newFileName);

      int sentChecker = 0;
      int byteSize = 0;
      char fileBuf[BUFSIZE];
      int whileFlag = 1;

      FILE *thisFile = fopen(newFileName, "rb");
      if(thisFile == NULL){
          printf("Failed to read file :( \n");
          printf("ERRNP: %s", strerror(errno));
      }
      else{

        while(whileFlag == 1){
          byteSize = fread(fileBuf, sizeof(char), BUFSIZE, thisFile);
          printf("ByteSize: %d \n", byteSize);

          sentChecker = sendto(sockfd, fileBuf, byteSize, 0, (struct sockaddr *) &serveraddr, serverlen);

          if(sentChecker < 0) {
            printf("ERROR sending file! ABORT! \n \n");
            exit(1);
          }
          else {
            printf("File packet successfully sent! WOOHOO!! \n \n");
            //exit(1);  // remove this
          }

          if(byteSize != BUFSIZE){
            whileFlag = 0;
          }
        }
      }

      fclose(thisFile);
    }

    // send file from client to server
    else if(strcmp("get", option) == 0 && newFileName != NULL){

      int receivedChecker = 0;
      char fileBuf[BUFSIZE];

      sleep(1); //give time before client overwrites file on localhost
      FILE *thisFile = fopen(newFileName, "wb");
      if(thisFile == NULL){
        printf("Failed to open file :( \n");
      }
      else {
        receivedChecker = BUFSIZE;
        while(receivedChecker == BUFSIZE){
          receivedChecker = recvfrom(sockfd, fileBuf, BUFSIZE, 0, (struct sockaddr *) &serveraddr, &serverlen);

          //printf("RECEIVED CHECKER %d \n", receivedChecker);
          if(receivedChecker < 0){
            printf("Error receiving file! \n \n");
          }
          else {

            printf("File packet successfully received WOOHOO!! \n \n");
            //printf("File Buf: %s \n", fileBuf);
            fwrite(fileBuf, sizeof(char), receivedChecker, thisFile);

          }

        }
      }
      fclose(thisFile);
    }



    else if(strcmp("delete", option) == 0){

      int x =  sizeof("File deleted successfully") + 1;
      char deleteBuf[x];
      int receivedChecker = 0;
      bzero(deleteBuf, x);
      receivedChecker = recvfrom(sockfd, deleteBuf, x, 0, (struct sockaddr *) &serveraddr, &serverlen);
      printf("After receive \n");
      if(receivedChecker < 0){
        printf("ERRNP: %s", strerror(errno));

      }
      else {
        printf("%s \n", deleteBuf);
      }
      printf("\n");
    }


    else if(strcmp("ls", option) == 0) {

      char lsBuf[BUFSIZE];
      int receivedChecker = BUFSIZE;

      while(receivedChecker == BUFSIZE) {
        bzero(lsBuf, BUFSIZE);
        receivedChecker = recvfrom(sockfd, lsBuf, BUFSIZE, 0, (struct sockaddr *) &serveraddr, &serverlen);
        // printf("Received Checker: %d \n", receivedChecker);

        if(receivedChecker < 0){
          printf("Error receiving file! \n \n");
        }
        else if(strcmp(lsBuf, "Completed file list") == 0){
          receivedChecker = 0;
        }
        else {
          printf("%s \n", lsBuf);
        }
      }
      printf("\n");
    }

    else if(strcmp("exit", option) == 0){
      printf("Quitting program! \n");
      exit(0);
    }

    else {
      printf("Invalid command! Try again.. \n");
    }

    //printf("Here 1 \n");
  }
  //   /* print the server's reply */
  // n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
  // //printf("N THING: %d \n", n);
  // if (n < 0)
  // error("ERROR in recvfrom");
  // printf("Echo from server: %s \n", buf);
  // return 0;

}
