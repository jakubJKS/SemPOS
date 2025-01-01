#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "ipc_utils.h"

void enable_raw_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disable_raw_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void display_menu() {
    printf("Welcome to Snake Game!\n");
    printf("1. Start a new single-player game\n");
    printf("2. Join a multiplayer game\n");
    printf("Enter your choice: ");
}

int main() {
    printf("Starting client...\n");

    int msgid = msgget(MSG_KEY, 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    display_menu();
    int choice;
    scanf("%d", &choice);

    Message msg;
    msg.mtype = 1;
    if (choice == 1) {
        strcpy(msg.mtext, "NEW_SINGLE");
    } else if (choice == 2) {
        strcpy(msg.mtext, "NEW_MULTI");
    } else {
        printf("Invalid choice\n");
        return 1;
    }

    if (send_message(msgid, &msg) == -1) {
        printf("Failed to send request\n");
        exit(1);
    }

    if (receive_message(msgid, &msg) == -1) {
        printf("Failed to receive response\n");
        exit(1);
    }

    int player_id = atoi(msg.mtext);
    printf("Assigned player ID: %d\n", player_id);

    enable_raw_mode();
    atexit(disable_raw_mode);

    char last_direction = 'w';
    printf("Use 'w', 'a', 's', 'd' to control the snake or 'q' to quit:\n");

    while (1) {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            if (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd') {
                last_direction = ch;
                msg.mtext[0] = '0' + player_id;
                msg.mtext[1] = ch;
                msg.mtext[2] = '\0';
                if (send_message(msgid, &msg) == -1) {
                    printf("Failed to send message\n");
                }
            } else if (ch == 'q') {
                printf("\nExiting client...\n");
                break;
            }
        }

        usleep(200000);
        msg.mtext[0] = '0' + player_id;
        msg.mtext[1] = last_direction;
        msg.mtext[2] = '\0';
        if (send_message(msgid, &msg) == -1) {
            printf("Failed to send message\n");
        }
    }

    return 0;
}
