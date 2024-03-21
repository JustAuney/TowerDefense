#include "game/Game.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "utils/time.h"
#include "graphics/Engine.h"

void game_init(Game *game) {
	srand(timestamp());
	path_gen(&game->path);
	mt_init(&game->mt);
	gl_init(&game->gl);
	ml_init(&game->ml);
	ta_init(&game->ta);
	sl_init(&game->sl); // free(): invalid pointer
	game->selected = NULL;
	game->createTower = false;
	game->start_time = timestamp();
}

bool game_upgrade_mana_tank(Game *game) {
	return mt_lvlup(&game->mt);
}

bool game_build_tower(Game *game, Coord pos) {
	if (!coord_valid(pos))
		return false;
	if (!mt_buy_tower(&game->mt))
		return false;
	ta_add(&game->ta, pos, &game->path);
	game->createTower = false;
	return true;
}

bool game_gen_gem(Game *game, int lvl) {
	if (!mt_buy_gem(&game->mt, lvl))
		return false;
	gl_add(&game->gl, gem_create(lvl));
	return true;
}

Gem *game_fuse_gems(Game *game, Gem *a, Gem *b) {
	assert(a && b);
	if(a == b)
		return NULL;
	if (a->lvl != b->lvl)
		return NULL;
	if (!mt_fuse_gems(&game->mt))
		return NULL;
	return gem_fuse(a, b);
}

void game_anticipate_next_wave(Game *game) {
	static bool first_wave = true;
	if (!first_wave)
		mt_refound_next_wave(&game->mt, game->ml.time_of_last_wave);
	else first_wave = false;
	ml_generate_wave(&game->ml, game->path.monster_nest);
}

void game_tick(Game *game) {
	time_t begin = timestamp();
	sl_update(&game->sl, &game->ml, &game->mt);
	ml_update(&game->ml, &game->mt, &game->path);
	ta_shoot(&game->ta, &game->ml, &game->sl);
	usleep(1000000 / FPS - (timestamp() - begin) * 1000);
}

void game_loop(Game *game) {
	while (true) {
		engine_draw(game);
		engine_event(game);
		game_tick(game);
	}
}
