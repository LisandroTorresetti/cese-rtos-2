/*
 * Copyright (c) 2024 Sebastian Bedin <sebabedin@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : ao_led.c
 * @date   : Feb 17, 2023
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"

#include "ao_ui.h"
#include "ao_led.h"

/********************** macros and definitions *******************************/

#define QUEUE_LENGTH            (1)
#define QUEUE_ITEM_SIZE         (sizeof(msg_event_t))

/********************** internal data declaration ****************************/

typedef struct {
  QueueHandle_t hqueue;
  //ao_led_handler_t colours[3];
} ao_ui_handle_t;

extern ao_led_handler_t led_red;
extern ao_led_handler_t led_green;
extern ao_led_handler_t led_blue;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

static ao_ui_handle_t hao;

/********************** internal functions definition ************************/

static void task(void *argument) {
  while (true) {
    ao_led_message_t msg;
    msg.ttl = 10000;
    msg_event_t event_msg;

    if (pdPASS == xQueueReceive(hao.hqueue, &event_msg, portMAX_DELAY)) {
     // ao_led_handler_t haoLed = hao.colours[event_msg];
      if (event_msg == 0) {
        ao_led_send(&led_green, &msg);
      } else if (event_msg == 1) {
        ao_led_send(&led_blue, &msg);
      } else if (event_msg == 2) {
        ao_led_send(&led_red, &msg);
      }
    }
  }
}

/********************** external functions PULSE definition ************************/

bool ao_ui_send_event(msg_event_t msg) {
  return (pdPASS == xQueueSend(hao.hqueue, &msg, 0));
}

void ao_ui_init() {
  hao.hqueue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
  while(NULL == hao.hqueue) {
    // error
  }

  BaseType_t status = xTaskCreate(task, "task_ao_ui", 128, NULL, tskIDLE_PRIORITY, NULL);
  while (pdPASS != status) {
    // error
  }
  //for (uint8_t i = 0; i < 3; i++) {
  //  hao.colours[i] = colours[i];
  //}
}

/********************** end of file ******************************************/
