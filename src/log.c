#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "log.h"

const char * log_type[4] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
}; 

const char * colors[4] = {
    "\x1b[36m",
    "\x1b[32m",
    "\x1b[1;33m",
    "\x1b[31m"
};

void log_message(LOG_LEVEL ll, char *file, int line, const char *function, char *log_message_fmt, ...) {
	time_t unformatted_time;
	time(&unformatted_time);
	struct tm *result_time = localtime(&unformatted_time);

	printf("%s[%d-%.2d-%.2d %.2d:%.2d:%.2d at %s:%d in function %s()] %s: ", 
		colors[ll],
		result_time->tm_year + 1900, 
		result_time->tm_mon + 1, 
		result_time->tm_mday, 
		result_time->tm_hour,
		result_time->tm_min,
		result_time->tm_sec,
		file,
		line,
		function,
		log_type[ll]
	);

	va_list args;
	va_start(args, log_message_fmt);
	vfprintf(stdout, log_message_fmt, args);
	va_end(args);
	printf("\x1b[0m\n");
}
