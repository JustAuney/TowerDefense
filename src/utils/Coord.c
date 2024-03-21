#include "utils/Coord.h"

#include <stdio.h>

bool coord_valid(Coord c) {
	return (
		c.x >= 0 && c.x < BOARD_WIDTH
	 && c.y >= 0 && c.y < BOARD_HEIGHT
	);
}

bool coord_equal(Coord a, Coord b) {
	return a.x == b.x && a.y == b.y;
}

Coord coord_add(Coord a, Coord b) {
	return (Coord) {
		.x = a.x + b.x,
		.y = a.y + b.y
	};
}

Coord coord_sub(Coord a, Coord b) {
	return (Coord) {
		.x = a.x - b.x,
		.y = a.y - b.y
	};
}

void coord_debug(Coord c) {
	fprintf(stderr, "(%d, %d)\n", c.x, c.y);
}
