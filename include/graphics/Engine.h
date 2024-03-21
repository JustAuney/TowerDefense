#pragma once

#include <MLV/MLV_all.h>
#include <math.h>

#include "game/Game.h"
#include "utils/Coord.h"
#include "game/Path.h"

void engine_init(const Game *game);
void engine_draw(const Game *game);
void engine_event(Game *game);
