#pragma once

#include "GemList.h"
#include "ManaTank.h"
#include "TowerArray.h"
#include "ShotList.h"

typedef struct _game {
	ManaTank mt;
	GemList gl;
	MonsterList ml;
	Path path;
	TowerArray ta;
	ShotList sl;
	Gem * selected;
	bool createTower;
	time_t start_time;
} Game;

void game_init(Game *game);
void game_tick(Game *game);
void game_loop(Game *game);
bool game_upgrade_mana_tank(Game *game);
bool game_build_tower(Game *game, Coord coord);
bool game_gen_gem(Game *game, int lvl);
Gem *game_fuse_gems(Game *game, Gem *a, Gem *b);
void game_anticipate_next_wave(Game *game);
