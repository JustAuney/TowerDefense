#pragma once

#include "Debuff.h"

typedef struct _debuff_node {
	Debuff *debuff;
	struct _debuff_node *prev;
	struct _debuff_node *next;
} DebuffNode;

typedef struct _debuff_list {
	DebuffNode *head;
	int size;
} DebuffList;

void dl_reset(DebuffList *dl);
void dl_add(DebuffList *dl, Debuff *debuff);
bool dl_remove(DebuffList *dl, Debuff *debuff);
