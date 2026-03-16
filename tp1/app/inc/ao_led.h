//
// Created by Luis Carol Lugones on 15/03/2026.
//

#ifndef TP1_AO_LED_H
#define TP1_AO_LED_H
#include <stdbool.h>
#include <stdint.h>
#include "cmsis_os.h"

typedef struct {
    uint16_t ttl;
} aoLedMessageT;

typedef enum {
    AO_LED_COLOR_RED,
    AO_LED_COLOR_GREEN,
    AO_LED_COLOR_BLUE,
} aoLedColorT;

typedef struct {
    aoLedColorT color;
    QueueHandle_t hqueue;
} aoLedHandleT;

bool ao_led_send(aoLedHandleT* hao, aoLedMessageT* msg);

void ao_led_init(aoLedHandleT* hao, aoLedColorT color);


#endif //TP1_AO_LED_H