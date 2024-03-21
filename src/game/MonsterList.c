#include "game/MonsterList.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#include "game/ShotList.h"
#include "utils/fatal.h"
#include "game/ManaTank.h"

static MonsterNode *_ml_make_node(Monster *monster) {
	MonsterNode *newNode = (MonsterNode *)malloc(sizeof(MonsterNode));
	if(!newNode)
		system_fatal("malloc");
	newNode->monster = monster;
	newNode->prev = NULL;
	newNode->next = NULL;

	return newNode;
}

void ml_init(MonsterList *ml) {
	*ml = (MonsterList) {
		.head = NULL,
		.nb_monsters = 0,
		.i = 0,
		.time_of_last_wave = 0
	};
}

void ml_generate_wave(MonsterList *ml, Coord monster_nest) {
	MonsterWave types[4];
	WaveType distribution[10] = {BOSS, CROWD, CROWD, FAST, FAST, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL};
	
	++ml->i;
	ml->time_of_last_wave = timestamp();

	int base_health = HEALTH_CONSTANT*pow(1.2, ml->i);
	types[BOSS] = (MonsterWave) {2, base_health * 12, 1. / FPS};
	types[CROWD] = (MonsterWave) {24, base_health, 1. / FPS};
	types[FAST] = (MonsterWave) {12, base_health, 2. / FPS};
	types[NORMAL] = (MonsterWave) {12, base_health, 1. / FPS};

	if (ml->i <= 5)
		distribution[0] = NORMAL; // no boss before wave 6;

	WaveType type = distribution[rand() % 10];
	MonsterWave wave = types[type];
	// wave.nb_monster = 4;
	for(int i = 0; i < wave.nb_monster; i++) {
		Monster *m = monster_create(wave.health, wave.speed, rand() % 360, monster_nest);
		Debuff *debuff = debuff_create(PARALYSE); // paralyse monster to get a line of monster instead of a pack
		debuff->duration = (type != FAST) ? i * 1000 : i * 500; 
		dl_add(&m->debuffs, debuff);
		ml_add(ml, m);
	}
}

void ml_apply_effect(MonsterList *ml, ManaTank *mt, FCoord pos, Gem *gem, SplashEffect effect) {
	for (MonsterNode *node = ml->head; node; node = node->next) {
		Monster *monster = node->monster;
		if (splash_should_apply(effect, fcoord_squared_distance(monster->pos, pos)))
			if (!monster_apply_effect(monster, gem, effect)) {
				mt_kill_monster(mt, node->monster);
				free(node->monster);
				node->monster = NULL;
			}
	}
}

void ml_add(MonsterList *ml, Monster *m) {
	assert(m);
	ml->nb_monsters++;
	if (!ml->head) {
		ml->head = _ml_make_node(m);
		return ;
	}
	MonsterNode *node = ml->head;
	while (node->next)
		node = node->next;
	node->next = _ml_make_node(m);
	node->next->prev = node;
}

void ml_remove(MonsterList *ml, MonsterNode *node) {
	--ml->nb_monsters;
	if (node == ml->head)
		ml->head = node->next;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	free(node);
}

void ml_update(MonsterList *ml, ManaTank *mt, const Path *path) {
	if (ml->time_of_last_wave != 0 && timestamp() - ml->time_of_last_wave > TIME_BETWEEN_WAVES)
		ml_generate_wave(ml, path->monster_nest);
	for(MonsterNode *node = ml->head; node;) {
		if (!node->monster || !monster_update(node->monster, mt, path)) {
			MonsterNode *to_remove = node;
			node = node->next;
			ml_remove(ml, to_remove);
			continue ;
		}
		node = node->next;
	}
}

void ml_reset(MonsterList *ml) {
	MonsterNode *next;
	for(MonsterNode *current = ml->head; current; current = next) {
		next = current->next;
		free(current);
	}
	ml_init(ml);
}

void ml_print(const MonsterList *ml) {
	for(MonsterNode *node = ml->head; node; node = node->next)
		monster_print(node->monster);
}
