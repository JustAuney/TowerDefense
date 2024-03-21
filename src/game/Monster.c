#include "game/Monster.h"

#include <stdlib.h>
#include <assert.h>

#include "game/Path.h"
#include "utils/fatal.h"
#include "game/ManaTank.h"

static bool _apply_debuffs(Monster *m) {
    for (DebuffNode *node = m->debuffs.head; node;) {
        Debuff *debuff = node->debuff;
        time_t time = timestamp();
        if (time - debuff->start >= debuff->duration) {
            switch (debuff->type) {
                case SLOW:
                    m->speed *= 1.5;
                    break ;
                case WEAK_SLOW:
                    m->speed *= 1.25;
                    break ;
                case PARALYSE:
                    --m->paralyzed;
                    break ;
                default:
                    break ;
            }
            node = node->next;
            dl_remove(&m->debuffs, debuff);
            free(debuff);
            continue ;
        }
        if (debuff->last_dot > 0 && time - debuff->last_dot < debuff->interval) {
            node = node->next;
            continue ;
        }
        debuff->last_dot = time;
        switch (debuff->type) {
            case PARASITE:
                m->hp -= debuff->base_damage * 0.025;
                break ;
            case SLOW:
                m->speed /= 1.5;
                break ;
            case WEAK_SLOW:
                m->speed /= 1.25;
                break ;
            case PARALYSE:
                ++m->paralyzed;
                break ;
        }
        node = node->next;
    }
    return m->hp > 0;
}

static bool _move(Monster *m, const Path *path) {
    static const FCoord DIRECTIONS[] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};

    const Cell *cell = &path->board[(int)m->pos.y][(int)m->pos.x];
    if (coord_equal(cell->coord, path->base)) {
        return false;
    }
    assert(cell->isPath);
    if (cell->dir != m->direction && fcoord_is_near_center(m->pos)) { // turn toward the middle
        m->direction = cell->dir;
        m->pos = fcoord_set_center(m->pos); // reset pos to middle of cell to prevent degradation
    }
    if (!m->paralyzed) {
        FCoord direction = DIRECTIONS[m->direction];
        FCoord noise = fcoord_mul(direction, 0.1 - ((double)(rand() % 2)) / 10.); // 10% variance
        m->pos = fcoord_add(m->pos, fcoord_mul(fcoord_add(direction, noise), m->speed));
    }
    return true;
}

bool monster_update(Monster *m, ManaTank *mt, const Path *path) {
    if (!_move(m, path)) {
        if (!mt_ban_monster(mt, m))
            exit(0);
        return false;
    }
    if (!_apply_debuffs(m)) {
        mt_kill_monster(mt, m);
        return false;
    }
    return true;
}

bool monster_apply_effect(Monster *m, Gem *gem, SplashEffect effect) {
    int damage_factor = gem_base_damage(gem, m->color);
    switch (effect) {
        case PYRO:
            m->hp -= damage_factor * 0.15;
            break;
        case VAPORIZATION:
            m->hp -= damage_factor * 0.05;
            break ;
        default: __builtin_unreachable();
    }
    return m->hp > 0;
}

Monster *monster_create(int hp_max,	double speed, int color, Coord monster_nest) {
    Monster *m = malloc(sizeof(Monster));
    if (!m)
        system_fatal("malloc");
    *m = (Monster) {
        .hp = hp_max,
        .hp_max = hp_max,
        .speed = speed,
        .paralyzed = false,
        .color = color,
        .pos = fcoord_add(fcoord_from_coord(monster_nest), (FCoord){0.5, 0.5}),
        .elemental_residu = NULL
    };
    dl_reset(&m->debuffs);
    return m;
}

void monster_print(const Monster *m) {
    printf("%p\n", m);
    printf("HP %d/%d\n", m->hp, m->hp_max);
    printf("Speed %f\n", m->speed);
    printf("Color %d\n", m->color);
    fcoord_debug(m->pos);
}
