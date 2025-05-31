#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include <helpers/common_headers.h>
#include <helpers/common_data.h>

extern QueueHandle_t comm_to_main_queue;
extern QueueHandle_t main_to_comm_queue;
extern QueueHandle_t main_to_oled_queue;
extern QueueHandle_t loadcell_to_main_queue;

void main_task_init(void);

#endif // MAIN_TASK_H