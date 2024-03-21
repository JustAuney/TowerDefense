#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "utils/Coord.h"

typedef enum _direction {
	North,
	South,
	East,
	West
} Direction;

typedef struct _path_cell {
	Coord coord;
	bool isPath;
	Direction dir;
} Cell;

typedef struct _path {
	Cell board[BOARD_HEIGHT][BOARD_WIDTH];
	Coord monster_nest;
	Coord base;
	int turn;
	int length;
} Path;

void path_init(Path *p);

void path_gen(Path *p);

void path_print(const Path *p);
