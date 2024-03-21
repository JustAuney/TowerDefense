#pragma once

#include "utils/Coord.h"
#include "MonsterList.h"
#include "Gem.h"
#include "Shot.h"

typedef struct _monster_list MonsterList;
typedef struct _shot Shot;

typedef struct _tower {
	Coord pos;
	Gem *gem; // may be NULL
	time_t last_shot;
} Tower;

void tower_init(Tower *tower, Coord pos);
Shot *tower_shoot(Tower *t, MonsterList *ml);
// if there is already a gem, return it, else return NULL
Gem *tower_add_gem(Tower *t, Gem *gem);
Gem *tower_remove_gem(Tower *t);
