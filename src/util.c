#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "util.h"

void exit_on_error(char *descriptor) {
	printf("Error (%s): %s\n", descriptor, strerror(errno));
	exit(1);
}

