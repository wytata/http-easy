#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <ctype.h>

#include "connection.h"
#include "log.h"

#define MAX_TARGET_LEN 256
#define REALLOC_THRESHOLD 1024 // Experiment with this???
#define TIMEOUT_SECONDS 5
#define TIMEOUT_MICROSECONDS 0

void new_connection_entrypoint(int client_fd) {
	int rc = 0, buf_size = 512, bytes_read = 0, cur_pos = 0;
	char *client_buf = malloc(buf_size); // TODO - decide on sensible request buf size
	
	int select_result;
	fd_set read_fds;
	struct timeval timeout = {TIMEOUT_SECONDS, TIMEOUT_MICROSECONDS};
	while (1) {
		FD_ZERO(&read_fds);
		FD_SET(client_fd, &read_fds);
		if ((select_result = select(client_fd + 1, &read_fds, NULL, NULL, &timeout)) > 0) {
			bytes_read = recv(client_fd, client_buf + cur_pos, buf_size - cur_pos, 0);
			if (bytes_read == -1) {
				LOG_ERROR("Failure in new_connection_entrypoint()");
				return;
			} else if (bytes_read == 0) {
				break;
			} else {
				if (buf_size - cur_pos < REALLOC_THRESHOLD) {
					buf_size *= 2;
					char *new_buf = realloc(client_buf, buf_size);
					if (!new_buf) {
						LOG_ERROR("Failed to reallocate client buffer while processing client request");
						return;
					}
					client_buf = new_buf;
				}
				cur_pos += bytes_read;
			}
			if (strncmp(client_buf + cur_pos - 4, "\r\n\r\n", 4) == 0) {
				break;
			}
		} else if (select_result == 0) {
			break;
		} else {
			LOG_ERROR("Call to select() failed while processing client request");
			return;
		}
		
	}
	client_buf[cur_pos] = 0;
	process_request(client_buf, cur_pos);

	LOG_WARN("Child proc %d exiting", getpid());
	exit(rc);
}

void process_request(char *request_buf, int request_len) {
	char request_target[MAX_TARGET_LEN + 1];
	if (strncmp(request_buf, "GET ", 4) == 0) {
		LOG_INFO("Received GET request");
		request_buf += 4;
	} else if (strncmp(request_buf, "HEAD ", 5) == 0) {
		LOG_INFO("Received HEAD request");
		request_buf += 5;
	} else if (strncmp(request_buf, "OPTIONS ", 8) == 0) {
		LOG_INFO("Received OPTIONS request");
		request_buf += 8;
	} else if (strncmp(request_buf, "TRACE ", 6) == 0) {
		LOG_INFO("Received TRACE request");
		request_buf += 6;
	} else if (strncmp(request_buf, "PUT ", 4) == 0) {
		LOG_INFO("Received PUT request");
		request_buf += 4;
	} else if (strncmp(request_buf, "DELETE ", 7) == 0) {
		LOG_INFO("Received DELETE request");
		request_buf += 7;
	} else if (strncmp(request_buf, "POST ", 5) == 0) {
		LOG_INFO("Received POST request");
		request_buf += 5;
	} else if (strncmp(request_buf, "PATCH ", 6) == 0) {
		LOG_INFO("Received PATCH request");
		request_buf += 6;
	} else if (strncmp(request_buf, "CONNECT ", 8) == 0) {
		LOG_INFO("Received CONNECT request");
		request_buf += 8;
	} else {
		LOG_ERROR("Received request of invalid type");
		return;
	}

	int request_target_len = 0;
	char *iter = request_target;
	while (!isspace((*iter++ = *request_buf++))) {
		request_target_len++;
		if (request_target_len == MAX_TARGET_LEN + 1) {
			LOG_ERROR("Request target length greater than maximum");
			return;
		}
	}
	request_target[request_target_len] = 0;
	LOG_INFO("Request target - %s", request_target);

	//write(STDOUT_FILENO, request_buf, request_len);
}
