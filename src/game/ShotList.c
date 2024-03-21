#include "game/ShotList.h"

#include <assert.h>
#include <stdlib.h>

#include "game/ManaTank.h"
#include "utils/fatal.h"

void sl_init(ShotList *sl) {
	*sl = (ShotList) {
		.head = NULL,
		.size = 0,
	};
}

void sl_reset(ShotList *sl) {
	if (sl->head) {
		ShotNode *node = sl->head;
		while (node) {
			ShotNode *to_free = node;
			node = node->next;
			free(to_free);
		}
		free(sl->head);
	}
	sl_init(sl);
}

static ShotNode *_sl_make_node(Shot *shot) {
	ShotNode *node = malloc(sizeof(ShotNode));
	if (!node)
		system_fatal("malloc");
	node->shot = shot;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

void sl_add(ShotList *sl, Shot *shot) {
	assert(shot);
	++sl->size;
	if (!sl->head) {
		sl->head = _sl_make_node(shot);
		return ;
	}

	ShotNode *node = sl->head;
	while (node->next)
		node = node->next;
	node->next = _sl_make_node(shot);
	node->next->prev = node;
}

void sl_remove(ShotList *sl, ShotNode *node) {
	--sl->size;
	if (node == sl->head)
		sl->head = node->next;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	free(node->shot);
	free(node);
}

void _cleanup_kills(ShotList *sl) {
	for (ShotNode *node = sl->head; node;) {
		if (node->shot->target->monster == NULL) {
			ShotNode *to_remove = node;
			node = node->next;
			sl_remove(sl, to_remove);
		} else
			node = node->next;
	}
}

void sl_update(ShotList *sl, MonsterList *ml, ManaTank *mt) {
	for (ShotNode *node = sl->head; node;) {
		Shot *shot = node->shot;
		if (!shot->target->monster) {
			node = node->next;
			continue;
		}
		if (shot_move(shot)) {
			SplashEffect effect = shot_hit(shot);
			if (effect != NONE)
				ml_apply_effect(ml, mt, shot->coord, shot->gem, effect);
			if (shot->target->monster && shot->target->monster->hp <= 0) {
				mt_kill_monster(mt, node->shot->target->monster);
				free(node->shot->target->monster);
				node->shot->target->monster = NULL;
				node = node->next;
				continue ;
			}
			ShotNode *to_remove = node;
			node = node->next;
			sl_remove(sl, to_remove);
		}
		else
			node = node->next;
	}
	_cleanup_kills(sl);
}
