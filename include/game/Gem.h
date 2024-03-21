#pragma once

#include "Shot.h"

typedef enum _gem_type {
	RED,
	GREEN,
	BLUE,
	MIXED
} GemType;

typedef struct _gem {
	GemType type;
	int color;
	int lvl;
} Gem;

// malloc a gem
Gem *gem_create(int lvl);
Gem *gem_copy(Gem *gem);
Gem *gem_fuse(Gem *a, Gem *b);
int gem_base_damage(Gem *gem, int monster_color);

