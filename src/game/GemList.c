#include "game/GemList.h"

#include <assert.h>
#include <stdlib.h>

#include "utils/fatal.h"

void gl_init(GemList *gl) {
	*gl = (GemList) {
		.head = NULL,
		.size = 0,
	};
}

void gl_reset(GemList *gl) {
	if (gl->head) {
		GemNode *node = gl->head;
		while (node) {
			GemNode *to_free = node;
			node = node->next;
			free(to_free);
		}
		free(gl->head);
	}
	gl_init(gl);
}

static GemNode *_gl_make_node(Gem *gem) {
	GemNode *node = malloc(sizeof(GemNode));
	if (!node)
		system_fatal("malloc");
	node->gem = gem;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

void gl_add(GemList *gl, Gem *gem) {
	assert(gem);
	++gl->size;
	if (!gl->head) {
		gl->head = _gl_make_node(gem);
		return ;
	}

	GemNode *node = gl->head;
	while (node->next)
		node = node->next;
	node->next = _gl_make_node(gem);
	node->next->prev = node;
}

bool gl_remove(GemList *gl, Gem *gem) {
	assert(gem);
	GemNode *node = gl->head;
	while (node && node->gem != gem) {
		node = node->next;
	}
	if (node) {
		if (gl->head == node)
			gl->head = node->next;
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;
		free(node);
		--gl->size;
		return true;
	}
	return false;
}
