#pragma once

#include <stdbool.h>

#include "Coord.h"

typedef struct _fcoord {
	double x;
	double y;
} FCoord;

bool fcoord_valid(FCoord c);
FCoord fcoord_add(FCoord a, FCoord b);
FCoord fcoord_add_f(FCoord c, double x);
FCoord fcoord_sub(FCoord a, FCoord b);
FCoord fcoord_mul(FCoord c, double x);
FCoord fcoord_from_coord(Coord c);
bool fcoord_is_near_center(FCoord c);
FCoord fcoord_set_center(FCoord c);
double fcoord_squared_distance(FCoord a, FCoord b);
void fcoord_debug(FCoord c);
