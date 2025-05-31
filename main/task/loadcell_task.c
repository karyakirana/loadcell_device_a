#include "loadcell_task.h"
#include <hx711_driver.h>

static const char* TAG = "LOADCELL_TASK";

static weight_t weight_data;

// forward declaration
static void loadcell_task_loop(void *pvParameters);
static void loadcell_state_dispatcher(void);
static void loadcell_cal_state_dispatcher(void);

static void loadcell_normalmode(void);
static void loadcell_sleepmode(void);
static void loadcell_deepsleepmode(void);
static void loadcell_wakeupmode(void);
static void loadcell_errormode(void);

static void loadcell_calinit(void);
static void loadcell_calloadweights(void);
static void loadcell_calwaiting(void);
static void loadcell_calinput(void);
static void loadcell_calconfirmation(void);
static void loadcell_calerror(void);

static void queue_from_main_handler(void);
static void queue_to_main_handler(void);

void loadcell_task_init(void) {
    // init hx711

    xTaskCreate(loadcell_task_loop, "loadcell_task", 8192, NULL, 4, NULL);
}

static void loadcell_task_loop(void *pvParameters) {
    while(true) {

        loadcell_state_dispatcher();

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

static void loadcell_state_dispatcher(void) {
    if (main_state == CALIBRATION_MODE) {
        loadcell_cal_state_dispatcher();
        return;
    }

    switch(main_state) {
        case NORMAL_MODE:
            break;
        case TARE_MODE:
            break;
        case SLEEP_MODE:
            break;
        case DEEPSLEEP_MODE:
            break;
        case WAKEUP_MODE:
            break;
        case ERROR_MODE:
            break;
        default: // IDLE_MODE
            break;
    }
}

static void loadcell_cal_state_dispatcher() {
    switch(calibration_state) {
        case CAL_INIT_MODE:
            break;
        case CAL_LOAD_WEIGHTS:
            break;
        case CAL_WAITING:
            break;
        case CAL_INPUT_MODE:
            break;
        case CAL_CONFIRMATION_MODE:
            break;
        case CAL_ERROR_MODE:
            break;
        default: // CAL_IDLE
            break;
    } 
}

static void loadcell_normalmode(void) {}
static void loadcell_sleepmode(void) {}
static void loadcell_deepsleepmode(void) {}
static void loadcell_wakeupmode(void) {}
static void loadcell_errormode(void) {}

static void loadcell_calinit(void) {}
static void loadcell_calloadweights(void) {}
static void loadcell_calwaiting(void) {}
static void loadcell_calinput(void) {}
static void loadcell_calconfirmation(void) {}
static void loadcell_calerror(void) {}

// QueueHandler
static void queue_from_main_handler(void) {}

static void queue_to_main_handler(void) {}