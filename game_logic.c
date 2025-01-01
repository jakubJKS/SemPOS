#include "game_logic.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

static int world_width, world_height;
Snake snakes[MAX_PLAYERS];
Position food[MAX_PLAYERS];

void initialize_game(int width, int height) {
    srand(time(NULL));
    world_width = width;
    world_height = height;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        snakes[i].length = 1;
        snakes[i].is_active = 0;
        snakes[i].direction = 'd';
        snakes[i].body[0].x = rand() % world_width;
        snakes[i].body[0].y = rand() % world_height;

        food[i].x = -1;
        food[i].y = -1;
    }
}

void generate_food() {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (snakes[i].is_active && (food[i].x == -1 || food[i].y == -1)) {
            int x, y;
            do {
                x = rand() % world_width;
                y = rand() % world_height;
            } while (is_occupied(x, y));
            food[i].x = x;
            food[i].y = y;
        }
    }
}

int is_occupied(int x, int y) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (snakes[i].is_active) {
            for (int j = 0; j < snakes[i].length; j++) {
                if (snakes[i].body[j].x == x && snakes[i].body[j].y == y) {
                    return 1;
                }
            }
        }
        if (food[i].x == x && food[i].y == y) {
            return 1;
        }
    }
    return 0;
}

int move_snake(int player_id, char direction) {
    if (!snakes[player_id].is_active) return -1;

    if (direction != '\0') {
        snakes[player_id].direction = direction;
    }

    Position new_head = snakes[player_id].body[0];
    switch (snakes[player_id].direction) {
        case 'w': new_head.y = (new_head.y - 1 + world_height) % world_height; break;
        case 's': new_head.y = (new_head.y + 1) % world_height; break;
        case 'a': new_head.x = (new_head.x - 1 + world_width) % world_width; break;
        case 'd': new_head.x = (new_head.x + 1) % world_width; break;
        default: return -1;
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (food[i].x == new_head.x && food[i].y == new_head.y) {
            if (snakes[player_id].length < WORLD_SIZE) {
                snakes[player_id].length++;
            }
            food[i].x = -1;
            food[i].y = -1;
            generate_food();
        }
    }

    for (int i = snakes[player_id].length - 1; i > 0; i--) {
        snakes[player_id].body[i] = snakes[player_id].body[i - 1];
    }

    snakes[player_id].body[0] = new_head;

    return 0;
}

void render_world() {
    printf("\033[H\033[J"); // Clear terminal
    for (int x = 0; x < world_width + 2; x++) printf("#");
    printf("\n");

    for (int y = 0; y < world_height; y++) {
        printf("#");
        for (int x = 0; x < world_width; x++) {
            char symbol = ' ';
            for (int i = 0; i < MAX_PLAYERS; i++) {
                if (food[i].x == x && food[i].y == y) {
                    symbol = 'F';
                }
                if (snakes[i].is_active) {
                    for (int j = 0; j < snakes[i].length; j++) {
                        if (snakes[i].body[j].x == x && snakes[i].body[j].y == y) {
                            symbol = (j == 0) ? 'H' : 'h';
                        }
                    }
                }
            }
            printf("%c", symbol);
        }
        printf("#\n");
    }

    for (int x = 0; x < world_width + 2; x++) printf("#");
    printf("\n");
}
