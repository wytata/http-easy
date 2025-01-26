#include <asm-generic/socket.h>
#include <signal.h>
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
#include "log.h"

#define MAX_CONNECTIONS 10

void usage() {
	printf("Usage: ./http-easy [OPTIONS]\n" \
		"Options:\n" \
		"-p <PORT_NO> (Bind server socket to port_no)\n" \
		"-h Get this usage information\n" \
	);
}

int main(int argc, char *argv[])
{
	int c;
	int port = -1;
	char *port_str;
	while ((c = getopt(argc, argv, "hp:")) != -1) {
		switch (c) {
			case 'h':
				usage();
				return 0;
			case 'p':
				port_str = optarg;
				port = atoi(port_str);
				break;
			case '?':
				usage();
				return 1;
		}
	}

	LOG_INFO("Starting server");
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		LOG_ERROR("Failure to obtain socket file descriptor - %s", strerror(errno));
		return 1;
	} 

	if (port != -1) {
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
			LOG_ERROR("Failed to set SO_REUSEADDR sockopt");
			return 1;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int)) < 0) {
			LOG_ERROR("Failed to set SO_REUSEPORT sockopt");
			return 1;
		}
	}

	srand(time(NULL));
	if (port == -1) port = (rand() % 5000) + 1000;
	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr = {
			.s_addr = inet_addr("0.0.0.0"),
		}
	};
	socklen_t server_addr_len = sizeof(server_addr);
	if (bind(sockfd, (const struct sockaddr *)&server_addr, server_addr_len) == -1) {
		LOG_ERROR("Failure to bind socket - %s", strerror(errno));
		return 1;
	}

	if (listen(sockfd, MAX_CONNECTIONS) ) {
		LOG_ERROR("Failure to listen on socket - %s", strerror(errno));
		return 1;
	}
	LOG_INFO("Server listening on port %d", port);

	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	int client_fd;
	for (;;) {
		if ((client_fd = accept(sockfd , (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len)) == -1) {
			LOG_ERROR("Failure to accept incoming request - %s", strerror(errno));
			return 1;
		}
		pid_t responding_proc = fork();
		if (responding_proc == 0) {
			new_connection_entrypoint(client_fd);
		} else if (responding_proc == -1) {
			LOG_ERROR("Failure to fork and create responding process - %s", strerror(errno));
			return 1;
		} else {
			continue;
		}
	}
	return 0;
}
