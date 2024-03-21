#include "game/Path.h"
#include "game/MonsterList.h"
#include "game/Game.h"
#include "graphics/Engine.h"

int main(void) {
    Game game;
    game_init(&game);
    engine_init(&game);
    game_loop(&game);
    return 0;
}
