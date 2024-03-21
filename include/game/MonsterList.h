#pragma once

#include "Monster.h"
#include "Path.h"
#include "SplashEffect.h"

typedef struct _monster Monster;
typedef struct _gem Gem;
typedef struct _mana_tank ManaTank;

typedef struct _monster_node {
	Monster *monster;
	struct _monster_node *prev;
	struct _monster_node *next;
} MonsterNode;

typedef struct _monster_list {
	MonsterNode *head;
	int nb_monsters;
	time_t time_of_last_wave;
	int i; // indice of wave
} MonsterList;

typedef struct _monster_wave {
	int nb_monster;
	int health;
	double speed;
} MonsterWave;

typedef enum _wave_type {
	BOSS,
	CROWD,
	FAST,
	NORMAL
} WaveType;

void ml_init(MonsterList *ml);
void ml_generate_wave(MonsterList *ml, Coord path);
void ml_apply_effect(MonsterList *ml, ManaTank *mt, FCoord pos, Gem *gem, SplashEffect effect);
void ml_add(MonsterList *ml, Monster *m);
void ml_remove(MonsterList *ml, MonsterNode *mn);
void ml_update(MonsterList *ml, ManaTank *mt, const Path *path);
void ml_reset(MonsterList *ml);
void ml_print(const MonsterList *ml);
