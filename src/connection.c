#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/signal.h>

#include "connection.h"
#include "util.h"

#define REALLOC_THRESHOLD 1024 // Experiment with this???
#define TIMEOUT_SECONDS 5
#define TIMEOUT_MICROSECONDS 0

void new_connection_entrypoint(int client_fd) {
	printf("Processing new connection with fd %d\n", client_fd);
	int rc = 0, buf_size = 512, bytes_read = 0, cur_pos = 0;
	char *client_buf = malloc(buf_size); // TODO - decide on sensible request buf size
	
	int select_result;
	fd_set read_fds;
	struct timeval timeout = {TIMEOUT_SECONDS, TIMEOUT_MICROSECONDS};
	printf("a\n");
	while (1) {
		printf("b\n");
		FD_ZERO(&read_fds);
		FD_SET(client_fd, &read_fds);
		if ((select_result = select(client_fd + 1, &read_fds, NULL, NULL, &timeout)) > 0) {
			bytes_read = recv(client_fd, client_buf + cur_pos, buf_size - cur_pos, 0);
			printf("c\n");
			if (bytes_read == -1) {
				exit_on_error("Failure in new_connection_entrypoint()");
			} else if (bytes_read == 0) {
				break;
			} else {
				if (buf_size - cur_pos < REALLOC_THRESHOLD) {
					printf("d\n");
					buf_size *= 2;
					char *new_buf = realloc(client_buf, buf_size);
					if (!new_buf) {
						printf("Couldn't reallocate client_buf :(");
						return;
					}
					client_buf = new_buf;
				}
				cur_pos += bytes_read;
			}
			if (strncmp(client_buf + cur_pos - 4, "\r\n\r\n", 4) == 0) {
				printf("Early return: End of message delimiter \\r\\n\\r\\n detected.\n");
				break;
			}
		} else if (select_result == 0) {
			break;
		} else {
			exit_on_error("select() failed");
		}
		
	}
	client_buf[cur_pos] = 0;
	process_request(client_buf, cur_pos);

	printf("Child proc exiting...\n");
	exit(rc);
}

void process_request(char *request_buf, int request_len) {
	write(STDOUT_FILENO, request_buf, request_len);
}
