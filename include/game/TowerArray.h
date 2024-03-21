#pragma once

#include "MonsterList.h"
#include "Tower.h"
#include "utils/Coord.h"
#include "ShotList.h"

#define TA_SIZE BOARD_HEIGHT * BOARD_WIDTH

typedef struct _tower_array {
	Tower towers[TA_SIZE];
	int size;
} TowerArray;

void ta_init(TowerArray *ta);
bool ta_add(TowerArray *ta, Coord pos, const Path *path);
void ta_shoot(TowerArray *ta, MonsterList *ml, ShotList *sl);
