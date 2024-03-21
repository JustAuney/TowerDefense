#include "game/ManaTank.h"

#include <stdlib.h>
#include <math.h>
#include <assert.h>

void mt_init(ManaTank *mt) {
	mt->lvl = 0;
	mt->capacity = 2000;
	mt->mana = 150;
}

bool mt_lvlup(ManaTank *mt) {
	int cost = mt->capacity / 4;
	if (mt->mana <= cost)
		return false;
	mt->mana -= cost;
	mt->lvl += 1;
	mt->capacity = 2000 * pow(1.4, mt->lvl);
	return true;
}

void mt_kill_monster(ManaTank *mt, const Monster *m) {
	assert(m);

	mt->mana += m->hp_max * 0.1 * pow(1.3, mt->lvl);
	if (mt->mana > mt->capacity)
		mt->mana = mt->capacity;
}

bool mt_ban_monster(ManaTank *mt, const Monster *m) {
	assert(m);

	int cost = m->hp_max * 0.15 * pow(1.3, mt->lvl);
	if (mt->mana <= cost)
		return false;
	mt->mana -= cost;
	return true;
}

bool mt_buy_tower(ManaTank *mt) {
	static int count = 0;
	static int last_cost = 50;

	if (count++ < 3)
		return true;
	else {
		int cost = last_cost * 2;
		if (mt->mana <= cost)
			return false;
		mt->mana -= last_cost;
		last_cost *= 2;
		return true;
	}
}

bool mt_buy_gem(ManaTank *mt, int lvl) {
	int cost = 100 * pow(2, lvl);
	if (mt->mana <= cost)
		return false;
	mt->mana -= cost;
	return true;
}

bool mt_fuse_gems(ManaTank *mt) {
	if (mt->mana <= 100)
		return false;
	mt->mana -= 100;
	return true;
}

void mt_refound_next_wave(ManaTank *mt, time_t time_of_last_wave) {
	mt->mana += mt->capacity * ((timestamp() - time_of_last_wave + TIME_BETWEEN_WAVES) / 1000) / 100;
	if (mt->mana > mt->capacity)
		mt->mana = mt->capacity;
}
