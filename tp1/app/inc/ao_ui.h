#ifndef TP1_AO_UI_H
#define TP1_AO_UI_H

#include <stdbool.h>

typedef enum {
    MSG_EVENT_BUTTON_PULSE,
    MSG_EVENT_BUTTON_SHORT,
    MSG_EVENT_BUTTON_LONG,
} msg_event_t;

bool ao_ui_send_event(msg_event_t msg);

void ao_ui_init();

#endif //TP1_AO_UI_H