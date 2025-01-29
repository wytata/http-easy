#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "handlers.h"
#include "helpers.h"
#include "log.h"
#include "config.h"

int handle_get(int client_fd, char *request_buf, int request_len) {
	char request_target[MAX_TARGET_LEN + 1] = {0};
	int request_target_len = parse_request_target(request_buf, request_target);
	char *query = strchr(request_target, '?');
	if (query) {
		*query = 0;
		query++;
		LOG_INFO("Request query: %s", query);
	}

	LOG_INFO("Request target: %s", request_target);
	char resource[DOCUMENT_ROOT_SIZE + MAX_TARGET_LEN + 12] = {0};

	if (request_target_len == 1) { 		
		sprintf(resource, "%s/index.html", DOCUMENT_ROOT);
	} else if (0) { // TODO - figure out if index.html needs to be appended to end
		sprintf(resource, "%s%s/index.html", DOCUMENT_ROOT, request_target);
	} else {
		sprintf(resource, "%s%s", DOCUMENT_ROOT, request_target);
	}
	LOG_INFO("Full resource path: %s", resource);

	// Optimize response using TCP_CORK option described in man page (tcp(7))
	setsockopt(client_fd, IPPROTO_TCP, TCP_CORK, &(int){1}, sizeof(int));
	int resource_fd = open(resource, O_RDONLY);
	if (resource_fd == -1) {
		LOG_ERROR("Error reading resource to serve request: %s", strerror(errno)); 
	}
	struct stat file_stats;
	int rc = fstat(resource_fd, &file_stats);
	if (rc == -1) {
		LOG_ERROR("Error retrieving file_stats from descriptor: %s", strerror(errno)); 
	}

	char response_header[MAX_RESPONSE_HEADER_SIZE] = {0};
	sprintf(
		response_header,
		"HTTP/1.2 200 OK\r\n"
		"Content-Length: %ld\r\n\r\n",
		file_stats.st_size
	);
	int response_header_length = strlen(response_header);
	send(client_fd, response_header, response_header_length, 0);
	sendfile(client_fd, resource_fd, NULL, file_stats.st_size);
	setsockopt(client_fd, IPPROTO_TCP, TCP_CORK, &(int){0}, sizeof(int));
	
	return 0;
}
