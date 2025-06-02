#include "oled_task.h"

static const char* TAG = "OLED_TASK";

static void oled_task_loop(void *pvParameters);

void oled_task_init() {
    //
    xTaskCreate(oled_task_loop, "oled_task", 4096, NULL, 5, NULL);
}

static void oled_task_loop(void *pvParameters) {
    while(true) {

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}