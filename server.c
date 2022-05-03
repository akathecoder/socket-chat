/*
* Socket Chat connection
* @author - Sparsh Agarwal
*
* server.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"
#define BACKLOG 10
#define BUFFER_SIZE 1024
#define QUEUE_SIZE 1


int main(int argc, char const *argv[]) {

  int sockFD, newSockFD, portNo, n;
  struct sockaddr_in servAddr, clientAddr;
  char buffer[BUFFER_SIZE];
  socklen_t len; // stores sizeof address

  portNo = atoi(PORT);
  sockFD = socket(AF_INET, SOCK_STREAM, 0); // Starts Socket

  if (sockFD < 0) {
    printf("Error in opening socket\n");
    exit(1);
  }

  printf("Server Started\n");

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons(portNo);

  // Closes old connections in TIME_WAIT or CLOSE_WAIT state.
  if (setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
    herror("setsockopt(SO_REUSEADDR) failed");
  }

  // Bind Socket to Port
  if (bind(sockFD, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
    printf("Error in binding socket\n");
    exit(1);
  }

  listen(sockFD, QUEUE_SIZE);
  // printf("IP Address: %s\n", (char *) &servAddr.);
  printf("Started Listening for Client...\n");

  len = sizeof(clientAddr);
  newSockFD = accept(sockFD, (struct sockaddr *) &clientAddr, &len); // Creates new socket for connection

  if (newSockFD < 0) {
    printf("Error in accepting request...\n");
    exit(1);
  }

  printf("Connection Established . . . \n\n------------------------------------------\n\n");

  // Loops indefinitely until a message triggers break
  while (1) {
    bzero(buffer, BUFFER_SIZE);
    n = read(newSockFD, buffer, BUFFER_SIZE);
    if (n < 0) {
      printf("Error in Reading...\n");
      exit(1);
    }
    printf("client> %s\n", buffer);
    if (!strncmp("terminate connection", buffer, 20)) {
      break;
    }

    bzero(buffer, BUFFER_SIZE);
    printf("> ");
    fgets(buffer, BUFFER_SIZE, stdin);
    n = write(newSockFD, buffer, BUFFER_SIZE);
    if (n < 0) {
      printf("Error in Writing...\n");
      exit(1);
    }

    if (!strncmp("terminate connection", buffer, 20)) {
      break;
    }
  }

  close(newSockFD); // Closes new connection socket
  printf("\n------------------------------------------\nConnection Terminated . . . \n\n");

  close(sockFD); // Closes original socket

  return 0;
}
