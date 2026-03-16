#include "ao_led.h"

#include "board.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"

#define TASK_PERIOD_MS_           (1000)

#define QUEUE_LENGTH            (1)
#define QUEUE_ITEM_SIZE         (sizeof(aoLedMessageT))

static GPIO_TypeDef* led_port_[] = {LED_RED_PORT, LED_GREEN_PORT,  LED_BLUE_PORT};
static uint16_t led_pin_[] = {LED_RED_PIN,  LED_GREEN_PIN, LED_BLUE_PIN };


static void task(void *argument)
{
  aoLedHandleT* hao = (aoLedHandleT*)argument;
  while (true) {
    aoLedMessageT msg;
    if (pdPASS == xQueueReceive(hao->hqueue, &msg, portMAX_DELAY)) {
      uint16_t ttl = msg.ttl;
      HAL_GPIO_WritePin(led_port_[hao->color], led_pin_[hao->color], GPIO_PIN_SET);
      vTaskDelay((TickType_t)(ttl / portTICK_PERIOD_MS));
      HAL_GPIO_WritePin(led_port_[hao->color], led_pin_[hao->color], GPIO_PIN_RESET);
    }
  }
}

/********************** external functions definition ************************/

bool ao_led_send(aoLedHandleT* hao, aoLedMessageT* msg) {
  return (pdPASS == xQueueSend(hao->hqueue, (void*)msg, 0));
}

void ao_led_init(aoLedHandleT* hao, aoLedColorT color) {
  hao->color = color;

  hao->hqueue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
  while(NULL == hao->hqueue) {
    // error
  }

  BaseType_t status = xTaskCreate(task, "task_ao_led", 128, (void * const) hao, tskIDLE_PRIORITY, NULL);
  while (pdPASS != status) {
    // error
  }
}

/********************** end of file ******************************************/
