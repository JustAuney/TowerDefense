#pragma once

#include <stdbool.h>

#include "utils/time.h"

typedef enum _effect_type {
	PARASITE = 1,
	SLOW = 1 << 2,
	WEAK_SLOW = 1 << 3,
	PARALYSE = 1 << 4
} DebuffType;

typedef struct _effect {
	time_t start;
	time_t last_dot;
	time_t duration;
	time_t interval;
	int base_damage;
	DebuffType type;
} Debuff;

Debuff *debuff_create(DebuffType type);
