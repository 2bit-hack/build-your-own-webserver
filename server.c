#define _XOPEN_SOURCE 700

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define REQ_BACKLOG 5
#define REQ_BUF_SZ 5 * 1024
#define RES_BUF_SZ 5 * 1024

int main() {
  int server_fd, client_fd;

  char req_buffer[REQ_BUF_SZ];
  char res_buffer[RES_BUF_SZ];

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("Failed to create server socket\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr;
  socklen_t addr_len = sizeof addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
    perror("Failed to bind to host and port\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, REQ_BACKLOG) == -1) {
    perror("Failed to listen for clients\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server started...\n");

  client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
  if (client_fd == -1) {
    perror("Failed to connect to client\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Client connected...\n");

  ssize_t bytes;
  while ((bytes = recv(client_fd, req_buffer, sizeof(req_buffer) - 1, 0)) > 0) {
    req_buffer[bytes] = '\0';
    printf("Client: %s\n", req_buffer);

    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "Hello world!";
    send(client_fd, response, strlen(response), 0);
  }

  close(server_fd);
  return 0;
}