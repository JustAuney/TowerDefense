#include "graphics/Engine.h"

#define L 0.5
#define S 1

static MLV_Color HSL_to_RGB(int H) {
    float C = (1 - fabs(2*L - 1)) * S;
    float X = C * (1 - fabs(fmod(H / 60., 2) - 1));
    float m = L - C / 2.0;
    C *= 255;
    X *= 255;
    m*= 255;
    int Hp = H/60;
    if(Hp == 0) return MLV_convert_rgba_to_color(C+m, X+m, m, 255);
    else if(Hp == 1) return MLV_convert_rgba_to_color(X+m, C+m, m, 255);
    else if(Hp == 2) return MLV_convert_rgba_to_color(m, C+m, X+m, 255);
    else if(Hp == 3) return MLV_convert_rgba_to_color(m, X+m, C+m, 255);
    else if(Hp == 4) return MLV_convert_rgba_to_color(X+m, m, C+m, 255);
    else return MLV_convert_rgba_to_color(C+m, m, X+m, 255);
}

static Coord f2pix(FCoord coord) {
	return (Coord) {
		.x = 11 + 30 * coord.x,
		.y = 51 + 30 * coord.y,
	};
} 

static void engine_draw_inventory(const Game *g) {
    GemNode *node = g->gl.head;
    for(int i = 0; i < g->gl.size; i++) {
        MLV_draw_filled_circle(895+30*(i%12), 65+30*(i/12), 14, HSL_to_RGB(node->gem->color));
        MLV_draw_text(895+30*(i%12), 65+30*(i/12), "%d", MLV_COLOR_WHITE, node->gem->lvl);
        if(node->gem  == g->selected)
            MLV_draw_circle(895+30*(i%12), 65+30*(i/12), 14, MLV_COLOR_BLACK);
        node = node->next;
    }
}

static void engine_draw_towers(const Game *g) {
    for(int i = 0; i < g->ta.size; i++) {
        MLV_draw_filled_rectangle(11+30*g->ta.towers[i].pos.x, 51+30*g->ta.towers[i].pos.y, 28, 28, MLV_convert_rgba_to_color(100, 100, 100, 255));
		if (g->ta.towers[i].gem) {
			MLV_draw_filled_circle(25+30*g->ta.towers[i].pos.x, 65+30*g->ta.towers[i].pos.y, 14, HSL_to_RGB(g->ta.towers[i].gem->color));
			MLV_draw_text(25+30*g->ta.towers[i].pos.x, 65+30*g->ta.towers[i].pos.y, "%d", MLV_COLOR_WHITE, g->ta.towers[i].gem->lvl);
			if(g->ta.towers[i].gem  == g->selected)
				MLV_draw_circle(25+30*g->ta.towers[i].pos.x, 65+30*g->ta.towers[i].pos.y, 14, MLV_COLOR_BLACK);
		}
    }    
}

static void engine_draw_waves(const MonsterList *ml) {
	MonsterNode *node = ml->head;
	for(int i = 0; i < ml->nb_monsters; i++) {
		Coord win = f2pix(node->monster->pos);
		MLV_draw_filled_rectangle(win.x - 15, win.y + 20, (node->monster->hp * 100 / node->monster->hp_max) / 3, 5, MLV_COLOR_GREEN);
		MLV_draw_rectangle(win.x - 15, win.y + 20, 33, 5, MLV_COLOR_BLACK);
		MLV_draw_filled_circle(win.x, win.y, 14, HSL_to_RGB(node->monster->color));
		node = node->next;
	}
}

static void engine_draw_shots(const ShotList *sl) {
	ShotNode *node = sl->head;
	for(int i = 0; i < sl->size; i++) {
		Coord win = f2pix(node->shot->coord);
		MLV_draw_filled_circle(win.x, win.y, 3, HSL_to_RGB(node->shot->gem->color));
		node = node->next;
	}
}

void engine_init(const Game *game) {
	MLV_create_window("tower_defense", "tower_defense", WINDOW_WIDTH, WINDOW_HEIGHT);
	MLV_clear_window(MLV_COLOR_BLACK);
	MLV_draw_filled_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, MLV_COLOR_BLACK);
	for(int i = 0; i < BOARD_WIDTH; i++) {
		for(int j = 0; j < BOARD_HEIGHT; j++) {
			if(game->path.board[j][i].isPath) {
				MLV_Color col = MLV_COLOR_WHITE;
				if(coord_equal(game->path.monster_nest, (Coord) {.x = i, .y = j}))
					col = MLV_COLOR_RED;
				else if(coord_equal(game->path.base, (Coord) {.x = i, .y = j}))
					col = MLV_COLOR_GREEN;
				MLV_draw_filled_rectangle(11+30*i, 51+30*j, 28, 28, col);
			} else {
				MLV_draw_filled_rectangle(11+30*i, 51+30*j, 28, 28, MLV_COLOR_DARK_GRAY);
			}
		}
	}
	MLV_draw_text(1030, 10, "Inventory", MLV_COLOR_WHITE);
	for(int i = 0; i < 12; i++) {
		for(int j = 0; j < 12; j++)
			MLV_draw_filled_rectangle(881+30*i, 51+30*j, 28, 28, MLV_COLOR_DARK_GRAY);
	}
	MLV_draw_text_box(1000, 450, 120, 40, "Buy Gem", 1, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_PURPLE,
		MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(1000, 500, 120, 40, "Buy Tower", 1, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_DARK_GRAY,
		MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(1000, 550, 120, 40, "Level Up", 1, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLUE,
		MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(1000, 600, 120, 40, "Next Wave", 1, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_RED,
        MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_save_screen();
}

void engine_draw(const Game *game) {
	MLV_load_screen();
	MLV_draw_filled_rectangle(0, 0, 840, 10, MLV_COLOR_BLACK);
	MLV_draw_text(10, 10, "Mana : %d / %d", MLV_COLOR_BLUE, game->mt.mana, game->mt.capacity);
	MLV_draw_text(780, 10, "Wave : %d", MLV_COLOR_WHITE, game->ml.i);
	engine_draw_inventory(game);
	engine_draw_waves(&game->ml);
	engine_draw_shots(&game->sl);
	engine_draw_towers(game);
	if(game->createTower)
		MLV_draw_rectangle(1000, 500, 120, 40, MLV_COLOR_GREEN);
	MLV_update_window();
}

static bool engine_check_mouse_on_board(Game *game, Coord mouse) {
	if(mouse.x >= 10 && mouse.x <= 850 && mouse.y >= 50 && mouse.y <= 710) {
		// Click on the board
		Coord board;
		board.x = (mouse.x - 10) / 30;
		board.y = (mouse.y - 50) / 30;
		if(game->createTower) {
			// If we want to create a tower
			game_build_tower(game, board);
			game->createTower = false;
		} else {
			if (game->selected)
			for(int i = 0; i < game->ta.size; i++) {
				if(coord_equal(game->ta.towers[i].pos, board) && game->ta.towers[i].gem) {
					if(game->selected) {
						Gem *fused = game_fuse_gems(game, game->selected, game->ta.towers[i].gem);
						gl_remove(&game->gl, game->selected);
						if (fused) {
							free(game->selected);
							free(game->ta.towers[i].gem);
							tower_add_gem(&game->ta.towers[i], fused);
						} else {
							gl_add(&game->gl, game->ta.towers[i].gem);
							tower_add_gem(&game->ta.towers[i], game->selected);
						}
						game->selected = NULL;
					}
				} else if(game->selected && coord_equal(game->ta.towers[i].pos, board) && !game->ta.towers[i].gem) {
					tower_add_gem(&game->ta.towers[i], game->selected);
					gl_remove(&game->gl, game->selected);
					game->selected = NULL;
				}
			}
		}
		return true;
	}
	return false;
}

static bool engine_check_mouse_on_inventory(Game *game, Coord mouse) {
	if(mouse.x >= 880 && mouse.x <= 1240 && mouse.y >= 50 && mouse.y <= 410) {
		Coord board;
		board.x = (mouse.x - 880) / 30;
		board.y = (mouse.y - 50) / 30;
		if(board.x + board.y * 12 < game->gl.size) {
			GemNode *node = game->gl.head;
			for(int i = 0; i < board.x + board.y * 12; i++) {
				if (!node)
					return false;
				node = node->next;
			}
			if(game->selected) {
				Gem *a = game->selected;
				Gem *b = node->gem;
				Gem *fused = game_fuse_gems(game, a, b);
				if (fused) {
					gl_add(&game->gl, fused);
					gl_remove(&game->gl, a);
					gl_remove(&game->gl, b);
					free(a);
					free(b);
				}
				game->selected = NULL;
			} else {
				game->selected = node->gem;
			}
		}
		return true;
	}
	return false;
}

static bool engine_check_mouse_on_button(Game *game, Coord mouse) {
	if(mouse.x >= 1000 && mouse.x <= 1120 && mouse.y >= 450 && mouse.y <= 490)
		game_gen_gem(game, 0);
	else if(mouse.x >= 1000 && mouse.x <= 1120 && mouse.y >= 500 && mouse.y <= 540)
		game->createTower = true;
	else if(mouse.x >= 1000 && mouse.x <= 1120 && mouse.y >= 550 && mouse.y <= 590)
		game_upgrade_mana_tank(game);
	else if(mouse.x >= 1000 && mouse.x <= 1120 && mouse.y >= 600 && mouse.y <= 640)
		game_anticipate_next_wave(game);
	return false;
}

void engine_event(Game *game) {
	static bool pressed = false;
	if (pressed == false && MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED) {
		pressed = true;
		int x, y;
		MLV_get_mouse_position(&x, &y);
		Coord mouse = {x, y};
		if(!engine_check_mouse_on_board(game, mouse)) {
			if(!engine_check_mouse_on_inventory(game, mouse))
				engine_check_mouse_on_button(game, mouse);
		}
	}
	if (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_RELEASED)
		pressed = false;
}
