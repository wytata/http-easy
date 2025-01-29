#ifndef HELPERS_H
#define HELPERS_H

#define MAX_TARGET_LEN 512
#define MAX_RESPONSE_HEADER_SIZE 1024

int parse_request_target(char *request_buf, char *request_target);

#endif // !HELPERS_H
