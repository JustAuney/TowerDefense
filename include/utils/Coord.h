#pragma once

#include <stdbool.h>

#include "constants.h"

typedef struct _coord {
	int x;
	int y;
} Coord;

bool coord_valid(Coord c);
bool coord_equal(Coord a, Coord b);
Coord coord_add(Coord a, Coord b);
Coord coord_sub(Coord a, Coord b);
void coord_debug(Coord c);
