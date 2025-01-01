#ifndef IPC_UTILS_H
#define IPC_UTILS_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <pthread.h>

#define SHM_KEY 1234
#define MSG_KEY 5678

typedef struct {
    long mtype;
    char mtext[256];
} Message;

int create_shared_memory(size_t size);
void* attach_shared_memory(int shmid);
int create_message_queue();
int send_message(int msgid, Message* msg);
int receive_message(int msgid, Message* msg);

#endif // IPC_UTILS_H
