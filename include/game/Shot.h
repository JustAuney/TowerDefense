#pragma once

#include <stdbool.h>

#include "utils/FCoord.h"
#include "Gem.h"
#include "MonsterList.h"
#include "SplashEffect.h"

typedef struct _gem Gem;
typedef struct _monster_node MonsterNode;

typedef struct _shot {
	FCoord coord;
	Gem *gem;
	MonsterNode *target;
} Shot;

// return true if it hits
bool shot_move(Shot *shot);
SplashEffect shot_hit(Shot *shot);
