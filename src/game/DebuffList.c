#include "game/DebuffList.h"

#include <assert.h>
#include <stdlib.h>

#include "utils/fatal.h"

void dl_reset(DebuffList *dl) {
	if (dl->head) {
		DebuffNode *node = dl->head;
		while (node) {
			DebuffNode *to_free = node;
			node = node->next;
			free(to_free);
		}
		free(dl->head);
		dl->head = NULL;
	}
	dl->size = 0;
}

static DebuffNode *_dl_make_node(Debuff *debuff) {
	DebuffNode *node = malloc(sizeof(DebuffNode));
	if (!node)
		system_fatal("malloc");
	node->debuff = debuff;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

void dl_add(DebuffList *dl, Debuff *debuff) {
	assert(debuff);

	if (!dl->head) {
		dl->head = _dl_make_node(debuff);
		return ;
	}

	DebuffNode *node = dl->head;
	while (node->next)
		node = node->next;
	node->next = _dl_make_node(debuff);
	node->next->prev = node;
}

bool dl_remove(DebuffList *dl, Debuff *debuff) {
	assert(debuff);

	DebuffNode *node = dl->head;
	while (node && node->debuff != debuff) {
		node = node->next;
	}
	if (node) {
		if (dl->head == node)
			dl->head = node->next;
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;
		free(node);
		return true;
	}
	return false;
}
