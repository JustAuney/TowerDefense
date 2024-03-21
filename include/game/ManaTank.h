#pragma once

#include <stdbool.h>

#include "Monster.h"
#include "Gem.h"
#include "Tower.h"

typedef struct _mana_tank {
	int mana;
	int capacity;
	int lvl;
} ManaTank;

void mt_init(ManaTank *mt);
bool mt_lvlup(ManaTank *mt);
void mt_kill_monster(ManaTank *mt, const Monster *m);
bool mt_ban_monster(ManaTank *mt, const Monster *m);
bool mt_buy_tower(ManaTank *mt);
bool mt_buy_gem(ManaTank *mt, int lvl);
bool mt_fuse_gems(ManaTank *mt);
void mt_refound_next_wave(ManaTank *mt, time_t time_of_last_wave);
