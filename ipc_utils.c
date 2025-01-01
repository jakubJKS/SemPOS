#include "ipc_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int create_shared_memory(size_t size) {
    int shmid = shmget(SHM_KEY, size, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    return shmid;
}

void* attach_shared_memory(int shmid) {
    void* shm = shmat(shmid, NULL, 0);
    if (shm == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }
    return shm;
}

int create_message_queue() {
    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    return msgid;
}

int send_message(int msgid, Message* msg) {
    if (msgsnd(msgid, msg, sizeof(msg->mtext), 0) == -1) {
        perror("msgsnd failed");
        return -1;
    }
    return 0;
}

int receive_message(int msgid, Message* msg) {
    if (msgrcv(msgid, msg, sizeof(msg->mtext), 0, 0) == -1) {
        perror("msgrcv failed");
        return -1;
    }
    return 0;
}
