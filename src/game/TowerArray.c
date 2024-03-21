#include "game/TowerArray.h"

#include <string.h>
#include <assert.h>

void ta_init(TowerArray *ta) {
	ta->size = 0;
	memset(ta->towers, 0, sizeof(Tower) * TA_SIZE);
}

bool ta_add(TowerArray *ta, Coord pos, const Path *path) {
	assert(ta->size < TA_SIZE);

	for (int i = 0; i < ta->size; ++i) {
		if (coord_equal(ta->towers[i].pos, pos) || path->board[pos.y][pos.x].isPath)
			return false;
	}
	tower_init(&ta->towers[ta->size++], pos);
	return true;
}

void ta_shoot(TowerArray *ta, MonsterList *ml, ShotList *sl) {
	for (int i = 0; i < ta->size; ++i) {
		Shot *shot = tower_shoot(&ta->towers[i], ml);
		if (shot)
			sl_add(sl, shot);
	}
}
