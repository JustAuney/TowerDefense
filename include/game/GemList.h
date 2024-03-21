#pragma once

#include "Gem.h"

typedef struct _gem_node {
	Gem *gem;
	struct _gem_node *prev;
	struct _gem_node *next;
} GemNode;

typedef struct _gem_list {
	GemNode *head;
	int size;
} GemList;

void gl_init(GemList *gl);
void gl_reset(GemList *gl);
void gl_add(GemList *gl, Gem *gem);
bool gl_remove(GemList *gl, Gem *gem);
