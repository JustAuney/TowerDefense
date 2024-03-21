#include "game/Gem.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "utils/fatal.h"

static const GemType TYPES[] = {RED, BLUE, GREEN};
static const int CENTERS[] = {0, 120, 240};

Gem *gem_create(int lvl) {
	Gem *gem = malloc(sizeof(Gem));
	if (!gem)
		system_fatal("malloc");
	gem->type = TYPES[rand() % 3];
	gem->color = CENTERS[gem->type] - 30 + rand() % 60;
	gem->lvl = lvl;
	return gem;
}

Gem *gem_copy(Gem *gem) {
	assert(gem);

	Gem *copy = malloc(sizeof(Gem));
	if (!copy)
		system_fatal("malloc");
	*copy = *gem;
	return copy;
}

int gem_base_damage(Gem *gem, int monster_color) {
	return DAMAGE_FACTOR * pow(2, gem->lvl) * (1 - cos(gem->color - monster_color) / 2);
}

static int _min(int a, int b) {
	return a < b ? a : b;
}

static int _max(int a, int b) {
	return a > b ? a : b;
}

Gem *gem_fuse(Gem *a, Gem *b) {
	assert(a && b);
	assert(a->lvl == b->lvl);

	Gem *gem = malloc(sizeof(Gem));
	if (!gem)
		system_fatal("malloc");
	gem->lvl = a->lvl + 1;
	if (a->color > b->color) {
		Gem *tmp = a;
		a = b;
		b = tmp;
	}
	if (abs(b->color - a->color) <= 180) {
		gem->color = _min(a->color, b->color) + abs(b->color - a->color) / 2;
	} else {
		gem->color = (_max(a->color, b->color) + (360 - b->color + a->color) / 2) % 360;
	}
	gem->type = MIXED;
	for (int i = 0; i < 3; ++i) {
		if (gem->color - CENTERS[i] - 30 < 60)
			gem->type = i;
	}
	return gem;
}
