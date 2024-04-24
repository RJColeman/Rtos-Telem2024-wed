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

Semaphore displayUse(1);

/* clang has a problem with strcpy so implemented our own function */
int stringlen(char* a) {
    int i = 0;
    while (a[i] != NULL) i++;
    return i;
}

int nstringcpy(char* b, char* a, int last) { // local version of strcpy
    int i = 0;

    while (i < last) {
        b[i] = a[i];
        i++;
    }
    return i;
}
int stringcpy(char* b, char* a) { // local version of strcpy
    int i = 0;
    int last = stringlen(a);
    while (i < last) {
        b[i] = a[i];
        i++;
    }
    while ((i < 80)) {
        b[i] = 0;
        i++;
    }
    return i;
}


static MemoryPool<message_t, 32> mpool;
static Queue<message_t, 32> queue;

void displayMessage(message_t msg){
    while (displayUse.try_acquire() == false) {
        ThisThread::sleep_for(1);
    }
    message_t *message = mpool.alloc();
    if(message) {
        stringcpy ( message->buffer, msg.buffer);
        message->displayType = msg.displayType;
        queue.put(message);
    }
    displayUse.release();
}    
void displayPanel() {
    while (displayUse.try_acquire() == false) {
        ThisThread::sleep_for(1);
    }
    CLS;
    ThisThread::sleep_for(100);
    HOME;
    ThisThread::sleep_for(10);

    printf("┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                                          City1082 Telemetry                                                     │\n");
    printf("├───────────────────────────┬─────────┬───────────────────────────┬─────────┬───────────────────────────┬─────────┤\n");
    printf("│ Temperature Reading       │         │ Light Level               │         │ Humidity Level            │         │\n");
    printf("├───────────────────────────┼─────────┼───────────────────────────┼─────────┼───────────────────────────┼─────────┤\n");
    printf("│ Temperature Setting       │         │ Light Level Setting       │         │ Humidity Level Setting    │         │\n");
    printf("├───────────────────────────┼─────────┼───────────────────────────┼─────────┼───────────────────────────┼─────────┤\n");
    printf("│ Heater State              │         │ Light State               │         │ Fan State                 │         │\n");
    printf("├───────────────────────────┴─────────┴───────────────────────────┴─────────┴───────────────────────────┴─────────┤\n");
    printf("│                                                                                                                 │\n");
    printf("└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘\n");
    ThisThread::sleep_for(10);

    displayUse.release();

}
void displayTask() {
    //RIS; // reset terminal
    //ThisThread::sleep_for(1000);
    CLS; // clear the vt100 terminal screen
    ThisThread::sleep_for(10);
    BLUE_BOLD;
    HIDE_CURSOR;
    displayPanel();
//    printf("\033[2;25HCity1082 Telemetry"); //Title at top middle
    NORMAL;
    while (true) {
        osEvent evt = queue.get();
        if (evt.status == osEventMessage) {
            message_t *message = (message_t*)evt.value.p;
            while (displayUse.try_acquire() == false) {
                ThisThread::sleep_for(1);
            }
            switch(message->displayType) {
                case TEMPERATURE_READING: {
                    printf("\033[4;31H%s", message->buffer);
                    break;
                }
                case TEMPERATURE_SETTING: {
                    printf("\033[6;31H%s", message->buffer);
                    break;
                }
                case HEATER_STATE: {
                    printf("\033[8;32H%s", message->buffer);
                    break;
                }
                case LIGHT_READING: {
                    printf("\033[4;69H%s", message->buffer);
                    break;
                }
                case LIGHT_SETTING: {
                    printf("\033[6;69H%s", message->buffer);
                    break;
                }
                case LIGHT_STATE: {
                    printf("\033[8;70H%s", message->buffer);
                    break;
                }
                case HUMIDITY_READING: {
                    printf("\033[4;107H%s", message->buffer);
                    break;
                }
                case HUMIDITY_SETTING: {
                    printf("\033[6;108H%s", message->buffer);
                    break;
                }
                case FAN_STATE: {
                    printf("\033[8;108H%s", message->buffer);
                    break;
                }
                case STATUS_DISPLAY: {
                    printf("\033[10;3H%s", message->buffer);
                    break;
                }
                default: {
                    printf("\033[20;3HNo definition detected %d %s\n", message->displayType, message->buffer);
                }
            }

            mpool.free(message);
            displayUse.release();

        }
        ThisThread::sleep_for(10);
    }
}