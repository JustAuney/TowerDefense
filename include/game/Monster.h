#pragma once

#include "DebuffList.h"
#include "utils/FCoord.h"
#include "Gem.h"
#include "Path.h"
#include "Gem.h"
#include "SplashEffect.h"
#include "utils/time.h"

typedef struct _gem Gem;
typedef struct _mana_tank ManaTank;

typedef struct _monster {
	int hp;
	int hp_max;
	double speed;
	int paralyzed;
	int color;
	FCoord pos;
	Direction direction;
	Gem *elemental_residu; //copy in case the original gem is removed
	DebuffList debuffs;
} Monster;

Monster *monster_create(int hp_max, double speed, int color, Coord monster_nest);
void monster_print(const Monster *m);

// return false if monster died
bool monster_update(Monster *m, ManaTank *mt, const Path *path);
bool monster_apply_effect(Monster *m, Gem *gem, SplashEffect effect);
