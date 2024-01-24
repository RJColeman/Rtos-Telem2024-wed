/*
 * Display messages thread.  Modules can use the helper function
 * to send information to any displays connected to the thing.
 *
 */
#include "mbed.h"
#include "config.h"
#include "display.h"
#include <string.h>
#include "vt100.h"

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
    //RIS; // reset terminal
    //ThisThread::sleep_for(1000);
    CLS; // clear the vt100 terminal screen
    ThisThread::sleep_for(10);
    BLUE_BOLD;

    printf("\033[1;10HCity1082 Telemetry"); //Title at top middle
    NORMAL;
    while (true) {
        osEvent evt = queue.get();
        if (evt.status == osEventMessage) {
            message_t *message = (message_t*)evt.value.p;
            switch(message->displayType) {
                case TEMPERATURE_READING: {
                    printf("\033[3;20H%s", message->buffer);
                    break;
                }
                default: {
                    printf("\033[20;1HNo definition detected %d %s\n", message->displayType, message->buffer);
                }
            }

            mpool.free(message);
        }
        ThisThread::sleep_for(10);
    }
}