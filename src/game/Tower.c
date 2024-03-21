#include "game/Tower.h"

#include <assert.h>
#include <stdlib.h>

#include "utils/constants.h"
#include "utils/fatal.h"

void tower_init(Tower *t, Coord pos) {
	t->pos = pos;
	t->gem = NULL;
	t->last_shot = 0;
}

Shot *tower_shoot(Tower *t, MonsterList *ml) {
	assert(ml);

	if (!t->gem || timestamp() - t->last_shot < SHOT_INTERVAL)
		return NULL;
	t->last_shot = timestamp();
	MonsterNode *closest = NULL;
	double closest_d = SQUARED_TOWER_RADIUS;
	FCoord tower_pos = fcoord_set_center(fcoord_from_coord(t->pos));
	for (MonsterNode *node = ml->head; node; node = node->next) {
		double d = fcoord_squared_distance(tower_pos, node->monster->pos);
		if (d < closest_d) {
			closest_d = d;
			closest = node;
		}
	}
	if (!closest)
		return NULL;
	Shot *shot = malloc(sizeof(Shot));
	if (!shot)
		system_fatal("malloc");
	*shot = (Shot) {
		.coord = tower_pos,
		.gem = gem_copy(t->gem),
		.target = closest,
	};
	return shot;
}

Gem *tower_add_gem(Tower *t, Gem *gem) {
	assert(gem);

	Gem *old = t->gem;
	t->gem = gem;
	t->last_shot = timestamp() + 2000;
	return old;
}

Gem *tower_remove_gem(Tower *t) {
	Gem *old = t->gem;
	t->gem = NULL;
	return old;
}
