/*
 * Display messages thread.  Modules can use the helper function
 * to send information to any displays connected to the thing.
 *
 */
#include "mbed.h"
#include "config.h"
#include "display.h"
#include <string.h>

//char* strcpy(char*, char*);  // fool syntax checker


static MemoryPool<message_t, 32> mpool;
static Queue<message_t, 32> queue;

void displayMessage(message_t msg){
    message_t *message = mpool.alloc();
    if(message) {
        strcpy ( message->buffer, msg.buffer);
        message->displayType = msg.displayType;
        queue.put(message);
    }
}    

void displayTask() {
    while (true) {
        osEvent evt = queue.get();
        if (evt.status == osEventMessage) {
            message_t *message = (message_t*)evt.value.p;
            printf("%d %s\n", message->displayType, message->buffer);
            mpool.free(message);
        }
        ThisThread::sleep_for(10);
    }
}