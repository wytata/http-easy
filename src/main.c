#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "connection.h"
#include "util.h"

#define MAX_CONNECTIONS 10

int main(int argc, char *argv[])
{
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		exit_on_error("Failure in socket()");
	} 

	srand(time(NULL));
	int rand_port = (rand() % 5000) + 1000;
	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(rand_port),
		.sin_addr = {
			.s_addr = inet_addr("0.0.0.0"),
		}
	};
	socklen_t server_addr_len = sizeof(server_addr);
	if (bind(sockfd, (const struct sockaddr *)&server_addr, server_addr_len) == -1) {
		exit_on_error("Failure in bind()");
	}

	if (listen(sockfd, MAX_CONNECTIONS) ) {
		exit_on_error("Failure in listen()");
	}
	printf("Listening on port %d...\n", rand_port);

	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	int client_fd;
	for (;;) {
		if ((client_fd = accept(sockfd , (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len)) == -1) {
			exit_on_error("Failure in accept()");
		}
		pid_t responding_proc = fork();
		if (responding_proc == 0) {
			new_connection_entrypoint(client_fd);
		} else if (responding_proc == -1) {
			exit_on_error("Failure to fork and create responding process");
		} else {
			continue;
		}
	}
	return 0;
}
