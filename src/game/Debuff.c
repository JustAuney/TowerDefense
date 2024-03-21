#include "game/Debuff.h"

#include <stdlib.h>

#include "utils/fatal.h"

Debuff *debuff_create(DebuffType type) {
	Debuff *debuff = malloc(sizeof(Debuff));
	if (!debuff)
		system_fatal("malloc");
	*debuff = (Debuff) {
		.start = timestamp(),
		.last_dot = 0,
		.interval = __LONG_MAX__,
		.base_damage = 0,
		.type = type,
	};
	switch (type) {
		case PARASITE:
			debuff->duration = 1000;
			debuff->interval = 500;
			break ;
		case SLOW:
			debuff->duration = 1000;
			break ;
		case WEAK_SLOW:
			debuff->duration = 5000;
			break ;
		case PARALYSE:
			debuff->duration = 3000;
	}
	return debuff;
}
