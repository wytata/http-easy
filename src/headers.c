#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "headers.h"
#include "log.h"
#include "table.h"

table *build_header_table(char *request_buf, int request_len) {	
	LOG_INFO("Length without request line: %d\n", request_len);
	write(STDOUT_FILENO, request_buf, request_len);
	table *header_table = table_create();
	if (!header_table) {
		return NULL;
	}

	char *header_pos = request_buf;
	while (1) {
		char *key_start = header_pos;
		while (*header_pos++ != ':'); 
		char *key_end = strst
	}

	return header_table;
}
