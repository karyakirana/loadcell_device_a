#ifndef LOADCELL_TASK_H
#define LOADCELL_TASK_H

#include <helpers/common_headers.h>
#include <helpers/common_data.h>

extern QueueHandle_t loadcell_to_main_queue;
extern QueueHandle_t main_to_loadcell_queue;

extern main_state_t main_state;
extern calibration_state_t calibration_state;

void loadcell_task_init(void);

#endif // LOADCELL_TASK_H