#ifndef OLED_TASK_H
#define OLED_TASK_H

#include <helpers/common_data.h>
#include <helpers/common_headers.h>

extern main_state_t main_state;
extern calibration_state_t calibration_state;

extern QueueHandle_t main_to_oled_queue;

void oled_task_init(void);

#endif // OLED_TASK_H