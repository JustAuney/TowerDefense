#include "utils/time.h"

#include <stdlib.h>

time_t timestamp() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return 1000 * tv.tv_sec + tv.tv_usec / 1000;
}
