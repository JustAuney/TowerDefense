#include "utils/fatal.h"

#include <stdio.h>
#include <stdlib.h>

static const char *FATAL_MSG = "tower_defense: fatal error: ";

void system_fatal(const char *msg) {
	fprintf(stderr, "%s", FATAL_MSG);
	perror(msg);
	exit(1);
}

void fatal(const char *msg) {
	fprintf(stderr, "%s %s\n", FATAL_MSG, msg);
	exit(1);
}
