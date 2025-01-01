#include "game_logic.h"
#include "ipc_utils.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int msgid;
static pthread_t game_thread;

void* game_loop(void* arg) {
    while (1) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (snakes[i].is_active) {
                move_snake(i, snakes[i].direction);
            }
        }
        render_world();
        usleep(200000); // 200 ms delay
    }
    return NULL;
}

void handle_client_messages() {
    Message msg;
    while (1) {
        if (receive_message(msgid, &msg) == 0) {
            if (strcmp(msg.mtext, "NEW_SINGLE") == 0 || strcmp(msg.mtext, "NEW_MULTI") == 0) {
                for (int i = 0; i < MAX_PLAYERS; i++) {
                    if (!snakes[i].is_active) {
                        snakes[i].is_active = 1;
                        snprintf(msg.mtext, sizeof(msg.mtext), "%d", i);
                        if (send_message(msgid, &msg) == -1) {
                            printf("Failed to send ID to client\n");
                        }
                        generate_food();
                        break;
                    }
                }
            } else {
                int player_id = msg.mtext[0] - '0';
                char direction = msg.mtext[1];

                if (player_id >= 0 && player_id < MAX_PLAYERS) {
                    snakes[player_id].direction = direction;
                }
            }
        }
    }
}

int main() {
    printf("Starting server...\n");
    initialize_game(20, 20);

    msgid = create_message_queue();

    pthread_create(&game_thread, NULL, game_loop, NULL);
    handle_client_messages();
    pthread_join(game_thread, NULL);

    return 0;
}
