#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdio.h>

#define MAX_PLAYERS 10
#define WORLD_SIZE 100

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position body[WORLD_SIZE];
    int length;
    int is_active;
    char direction;
} Snake;

extern Snake snakes[MAX_PLAYERS];
extern Position food[MAX_PLAYERS];

void initialize_game(int world_width, int world_height);
void generate_food();
int move_snake(int player_id, char direction);
void render_world();
int is_occupied(int x, int y);

#endif // GAME_LOGIC_H
