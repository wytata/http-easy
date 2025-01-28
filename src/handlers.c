#include "handlers.h"
#include "helpers.h"
#include "log.h"

int handle_get(char *request_buf, int request_len) {
	char request_target[MAX_TARGET_LEN + 1] = {0};
	parse_request_target(request_buf, request_target);
	LOG_INFO("Got request target %s", request_target);
	
	return 0;
}
