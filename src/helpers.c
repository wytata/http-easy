#include <ctype.h>

#include "helpers.h"
#include "log.h"

int parse_request_target(char *request_buf, char *request_target) {
	int request_target_len = 0;
	char *iter = request_target;
	while (!isspace((*iter++ = *request_buf++))) {
		request_target_len++;
		if (request_target_len == MAX_TARGET_LEN + 1) {
			LOG_ERROR("Request target length greater than maximum");
			return 1;
		}
	}
	request_target[request_target_len] = 0;
	return 0;
}

