#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

char* httpget() {
  int socket_fd;
  struct sockaddr_in server_connection_info;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  server_connection_info.sin_family = AF_INET;
  // TODO: configurable server URL somewhere
  server_connection_info.sin_addr.s_addr = inet_addr("10.0.0.3");
  server_connection_info.sin_port = htons(80);

  if(0 != connect(socket_fd, &server_connection_info, sizeof(server_connection_info))) {
    // TODO: handle failure
    printf("Failed...\n");
    return NULL;
  }

  // TODO: error check the operations. Should be able to timeout
  char* response = calloc(1024, sizeof(char));
  // write to write data to socket
  write(socket_fd, "GET /environment.json HTTP/1.1", 30);
  
  // read data from socket
  read(socket_fd, response, 1024);
  close(socket_fd);

  return response;
}
