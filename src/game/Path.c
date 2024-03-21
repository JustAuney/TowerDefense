#include "game/Path.h"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static const Coord CARDINALS[] = {
    {0, -1},
    {0, 1},
    {1, 0},
    {-1, 0},
};

static const Coord MANHATTAN[4][5] = {
    { {0, -2}, {1, -1}, {-1, -1}, {-2, 0}, {2, 0} },
    { {0, 2}, {1, 1}, {-1, 1}, {-2, 0}, {2, 0} },
    { {2, 0}, {1, 1}, {1, -1}, {0, -2}, {0, 2}},
    { {-2, 0}, {-1, 1}, {-1, -1}, {0, -2}, {0, 2} },
};

// Renvoie le tableau des cases à au plus 2 de distance de Manhattan de la coordonnée c
bool isClear(Path *p, Coord coord, Direction direction) {
    for (int i = 0; i < 5; ++i) {
        Coord new_coord = coord_add(coord, MANHATTAN[direction][i]);
        if (!coord_valid(new_coord) || p->board[new_coord.y][new_coord.x].isPath)
            return false;
    }
    return true;
}

int extended(Path *p, Coord coord, Direction direction) {
    coord = coord_add(coord, CARDINALS[direction]);
    if (!isClear(p, coord, direction))
        return 0;
    return extended(p, coord, direction) + 1;
}

void path_init(Path* p) {
    // Initialise le chemin p à vide
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            p->board[i][j].coord.x = j;
            p->board[i][j].coord.y = i;
            p->board[i][j].isPath = false;
            p->board[i][j].dir = North;
            p->turn = 0;
	        p->length = 0;
        }
    }
}

int max(int a, int b) {
    return a > b ? a : b;
}

void newDir(Path *p, Coord currentPos) { 
    // Calcul les étendus dans les deux directions en tournant a gauche ou droite
    Direction *dir = &p->board[currentPos.y][currentPos.x].dir;
    int a, b;
    if(*dir == North || *dir == South) {
        a = extended(p, currentPos, East);
        b = extended(p, currentPos, West);
    } else {
        a = extended(p, currentPos, North);
        b = extended(p, currentPos, South);
    }
    // Choisis aléatoirement la direction avec une probabiltié proportionnelle à l'étendu de chacune
    int sum = a + b;
    if(sum == 0) 
         return;
    int randDir = rand() % sum + 1;
    if(randDir <= a && (*dir == North || *dir == South))
        *dir = East;
    else if(randDir > a && (*dir == North || *dir == South))
        *dir = West;
    else if(randDir <= a) {
        *dir = North;
    } else {
        *dir = South;
    }
}

void extendPath(Path* p, Coord *currentPos) {
    Direction dir = p->board[currentPos->y][currentPos->x].dir;
    int n = extended(p, *currentPos, dir);
    if(n <= 2) {
        return;
    } else {
        int s = 0;
        for(int i = 0; i < n; i++)
            s += rand() % 4 > 0 ? 1 : 0;
        Coord coord = CARDINALS[dir];
        for(int i = 0; i < max(s, 3); i++) {
            currentPos->x += coord.x;
            currentPos->y += coord.y;
            Cell *cell = &p->board[currentPos->y][currentPos->x];
            cell->dir = dir;
            cell->isPath = true;
            p->length++;
        }
        newDir(p, *currentPos);
        extendPath(p, currentPos);
        p->turn++;
    }

}

bool checkPath(Path* p) {
    return (p->turn >= 7 && p->length >= 75);
}

void path_gen(Path* p) {
    path_init(p);
    // Positionne le nid aléatoirement à au moins 3 cases des bords
    p->monster_nest.x = (rand() % (BOARD_WIDTH - 4)) + 2;
    p->monster_nest.y = (rand() % (BOARD_HEIGHT - 4)) + 2;
    p->board[p->monster_nest.y][p->monster_nest.x].isPath = true;
    p->length ++;
    // Calcul les étendus dans chaque direction du nid;
    int n = extended(p, p->monster_nest, North);
    int s = extended(p, p->monster_nest, South);
    int e = extended(p, p->monster_nest, East);
    int w = extended(p, p->monster_nest, West);
    // Choisis aléatoirement la direction initiale avec une probabiltié proportionnelle à l'étendu de chacune
    int sum = n + s + e + w;
    if(sum == 0) {
        fprintf(stderr, "Toutes les étendus valent 0 -> Division par 0");
        exit(1);
    }
    int randDir = rand() % sum + 1;
    Direction *dir = &p->board[p->monster_nest.y][p->monster_nest.x].dir;
    if(randDir <= n)
        *dir = North;
    else if (randDir <= n + s)
        *dir = South;
    else if (randDir <= n + s + e)
        *dir = East;
    else
        *dir = West;
    Coord currentPos = p->monster_nest;
    extendPath(p, &currentPos);
    if(!checkPath(p)) {
        path_gen(p);
    } else {
        Coord currentPos = p->monster_nest;
        for(int i = 0; i < p->length; i++) {
            switch (p->board[currentPos.y][currentPos.x].dir) {
                case North: currentPos.y --;
                    if(!p->board[currentPos.y][currentPos.x].isPath)
                        currentPos.y ++;
                    break;
                case South: currentPos.y ++;
                    if(!p->board[currentPos.y][currentPos.x].isPath)
                        currentPos.y --;
                    break;
                case East: currentPos.x ++;
                    if(!p->board[currentPos.y][currentPos.x].isPath)
                        currentPos.x --;
                    break;
                case West: currentPos.x --;
                    if(!p->board[currentPos.y][currentPos.x].isPath)
                        currentPos.x ++;
                    break;
            }
        }
        p->base = currentPos;
    }
}

void path_print(const Path *p) {
    printf("Turn : %d | Length : %d\n", p->turn, p->length);
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            if(p->board[i][j].isPath) {
                switch(p->board[i][j].dir) {
                    case North:  printf("N");
                        break;
                    case South: printf("S");
                        break;
                    case East: printf("E");
                        break;
                    case West: printf("W");
                        break; 
                }
            } else {
                printf("_");
            }
               
        }
        printf("\n");
    }
}
