/*
* Socket Chat connection
* @author - Sparsh Agarwal
*
* client.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3490"
#define MAXDATASIZE 100
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {

  int sockFD, portNo, n;
  struct sockaddr_in servAddr;
  char buffer[BUFFER_SIZE];

  if (argc < 2) {
    printf("provide required command-line inputs as: <filename.c> <server_IP_addr>\n");
    exit(1);
  }

  // portNo = atoi(argv[2]);
  portNo = atoi(PORT);
  sockFD = socket(AF_INET, SOCK_STREAM, 0); // Starts Socket

  if (sockFD < 0) {
    printf("Error in opening socket\n");
    exit(1);
  }

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(argv[1]);
  servAddr.sin_port = htons(portNo);

  // Requests connection to the server
  if (connect(sockFD, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
    printf("Connection failed...\n");
    exit(1);
  }

  printf("Connection Established . . . \n\n------------------------------------------\n\n");

  // Loops indefinitely until a message triggers break
  while (1) {
    bzero(buffer, BUFFER_SIZE);
    printf("> ");
    fgets(buffer, BUFFER_SIZE, stdin);
    n = write(sockFD, buffer, BUFFER_SIZE);
    if (n < 0) {
      printf("Error in Writing...\n");
      exit(1);
    }
    if (!strncmp("terminate connection", buffer, 20)) {
      break;
    }

    bzero(buffer, BUFFER_SIZE);
    n = read(sockFD, buffer, BUFFER_SIZE);
    if (n < 0) {
      printf("Error in Reading...\n");
      exit(1);
    }
    printf("server> %s\n", buffer);

    if (!strncmp("terminate connection", buffer, 20)) {
      break;
    }
  }

  close(sockFD); // Closes socket
  printf("\n------------------------------------------\nConnection Terminated . . . \n\n");

  return 0;
}
