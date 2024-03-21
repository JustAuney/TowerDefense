#include "utils/FCoord.h"

#include <math.h>
#include <stdio.h>

bool fcoord_valid(FCoord c) {
	return (
		c.x >= 0 && c.x < FBOARD_WIDTH
	 && c.y >= 0 && c.y < FBOARD_HEIGHT
	);
}

FCoord fcoord_add(FCoord a, FCoord b) {
	return (FCoord) {
		.x = a.x + b.x,
		.y = a.y + b.y
	};
}

FCoord fcoord_add_f(FCoord c, double x) {
	return (FCoord) {
		.x = c.x + x,
		.y = c.y + x,
	};
}

FCoord fcoord_sub(FCoord a, FCoord b) {
	return (FCoord) {
		.x = a.x - b.x,
		.y = a.y - b.y
	};
}

FCoord fcoord_mul(FCoord c, double x) {
	return (FCoord) {
		.x = c.x * x,
		.y = c.y * x,
	};
}

FCoord fcoord_from_coord(Coord c) {
	return (FCoord) {
		.x = (double)c.x,
		.y = (double)c.y,
	};
}

bool fcoord_is_near_center(FCoord c) {
	static const double epsilon = 0.01;

	return c.x - floor(c.x) - 0.5 < epsilon && c.y - floor(c.y) - 0.5 < epsilon;
}

FCoord fcoord_set_center(FCoord c) {
	return (FCoord) {
		.x = floor(c.x) + 0.5,
		.y = floor(c.y) + 0.5,
	};
}

double fcoord_squared_distance(FCoord a, FCoord b) {
	return ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));
}

void fcoord_debug(FCoord c) {
	fprintf(stderr, "(%f, %f)\n", c.x, c.y);
}
