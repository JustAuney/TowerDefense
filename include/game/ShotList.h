#pragma once

#include "Shot.h"

typedef struct _mana_tank ManaTank;

typedef struct _shot_node {
	Shot *shot;
	struct _shot_node *prev;
	struct _shot_node *next;
} ShotNode;

typedef struct _shot_list {
	ShotNode *head;
	int size;
} ShotList;

void sl_init(ShotList *sl);
void sl_reset(ShotList *sl);
void sl_add(ShotList *sl, Shot *shot);
void sl_move(ShotList *sl);
void sl_update(ShotList *sl, MonsterList *ml, ManaTank *mt);
