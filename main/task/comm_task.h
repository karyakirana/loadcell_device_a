#ifndef COMM_TASK_H
#define COMM_TASK_H

#include <helpers/common_headers.h>
#include <helpers/common_data.h>

extern main_state_t main_state;
extern calibration_state_t calibration_state;

extern QueueHandle_t comm_to_main_queue;
extern QueueHandle_t main_to_comm_queue;

void comm_task_init(void);

#endif // COMM_TASK_H