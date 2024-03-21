#include "game/Shot.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "utils/time.h"

bool shot_move(Shot *shot) {
	FCoord m = shot->target->monster->pos;
	FCoord s = shot->coord;
	double distance = sqrt(fcoord_squared_distance(s, m));
	FCoord direction = (FCoord) {
		.x = (m.x - s.x) / distance,
		.y = (m.y - s.y) / distance,
	};
	if (distance < SHOT_SPEED)
		return true;
	shot->coord = fcoord_add(shot->coord, fcoord_mul(direction, SHOT_SPEED));
	return false;
}

SplashEffect shot_hit(Shot *shot) {
	Gem *gem = shot->gem;
	Monster *monster = shot->target->monster;
	SplashEffect effect = NONE;
	Debuff *debuff = NULL;
	assert(gem && monster);

	int damage = gem_base_damage(gem, monster->color);
	if (gem->type == MIXED) {
		damage *= 2;
		if (rand() % 10 == 0)
			damage *= 2;
	}
	else if (!monster->elemental_residu) {
		monster->elemental_residu = gem_copy(gem);
	} else if (monster->elemental_residu->type == gem->type) {
		switch (gem->type) {
			case RED:
				effect = PYRO;
				break ;
			case GREEN:
				debuff = debuff_create(PARASITE);
				debuff->base_damage = damage;
				break ;
			case BLUE:
				debuff = debuff_create(SLOW);
				break ;
			default: __builtin_unreachable();
		}
		free(monster->elemental_residu);
		monster->elemental_residu = gem_copy(gem);
	} else {
		int combination = monster->elemental_residu->type | gem->type;
		switch (combination) {
			case RED|BLUE:
				effect = VAPORIZATION;
				debuff = debuff_create(WEAK_SLOW);
				break ;
			case RED|GREEN:
				damage *= 3;
				break ;
			case BLUE|GREEN:
				debuff = debuff_create(PARALYSE);
				break ;
			default: __builtin_unreachable();
		}
		free(monster->elemental_residu);
		monster->elemental_residu = NULL;
	}
	if (debuff)
		dl_add(&monster->debuffs, debuff);
	monster->hp -= damage;
	return effect;
}
