#include "loadcell_task.h"
#include "sensors/sensor.h"

static const char* TAG = "LOADCELL_TASK";

static loadcell_data_t weight_data;
static loadcell_recv_data_t recv_data;

// forward declaration
static void loadcell_task_loop(void *pvParameters);
static void loadcell_state_dispatcher(void);
static void loadcell_cal_state_dispatcher(void);

static void loadcell_normalmode(void);
static void loadcell_normaltaremode(void);
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

static bool queue_from_main_handler(void);
static void queue_to_main_handler(void);

void loadcell_task_init(void) {
    // init sensor
    sensor_init();

    // init loadcell_data_t val
    weight_data.raw_val = 0;
    weight_data.units_val = 0.0f;
    weight_data.filtered_val = 0.0f;
    weight_data.known_weight_val = 0.0f;

    xTaskCreate(loadcell_task_loop, "loadcell_task", 8192, NULL, 4, NULL);
}

static void loadcell_task_loop(void *pvParameters) {
    while(true) {

        sensor_update();

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
            loadcell_normalmode();
            break;
        case TARE_MODE:
            loadcell_normaltaremode();
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
            loadcell_calinit();
            break;
        case CAL_LOAD_WEIGHTS:
            loadcell_calloadweights();
            break;
        case CAL_WAITING:
            loadcell_calwaiting();
            break;
        case CAL_INPUT_MODE:
            loadcell_calwaiting();
            break;
        case CAL_CONFIRMATION_MODE:
            loadcell_calconfirmation();
            break;
        case CAL_ERROR_MODE:
            break;
        default: // CAL_IDLE
            break;
    } 
}

static void loadcell_normalmode(void) {
    weight_data.raw_val = sensor_get_raw_value();
    weight_data.units_val = sensor_get_units_value();
    weight_data.filtered_val = sensor_get_filtered_units_value();
    queue_to_main_handler();
}

static void loadcell_normaltaremode(void) {
    if (!queue_from_main_handler()) return;

    if (recv_data.command != CMD_TARE_NORMAL) {
        ESP_LOGW(TAG, "NOT CMD_TARE_NORMAL command");
        return;
    }

    bool is_success = true;

    // jalankan tare
    if (sensor_tare_process() != ESP_OK) {
        ESP_LOGW(TAG, "TARE PROCESSING FAILED");
        is_success = false;
    }

    queue_to_main_handler();
}

static void loadcell_sleepmode(void) {}
static void loadcell_deepsleepmode(void) {}
static void loadcell_wakeupmode(void) {}
static void loadcell_errormode(void) {}

static void loadcell_calinit(void) {
    // get last known weight
    // tare processing
    // cal init status
}

static void loadcell_calloadweights(void) {
    // menunggu perintah CAL_LOAD_WEIGHTS_OK
    // cal load weights status
}

static void loadcell_calwaiting(void) {
    // mengirimkan terus status berat stabil
}

static void loadcell_calinput(void) {
    // proses perintah CAL_INPUT_VALUE
    // set last_known_weight
    // cal input status
}
static void loadcell_calconfirmation(void) {
    // simpan kalibrasi jika CMD_CAL_CONFIRMATION_OK
    // cal calibration status
}

static void loadcell_calerror(void) {
    // memberikan status terus menerus false
}

// QueueHandler
static bool queue_from_main_handler(void) {
    if (xQueueReceive(main_to_loadcell_queue, &recv_data, pdMS_TO_TICKS(200)) != pdPASS) {
        ESP_LOGW(TAG, "rcv main_to_loadcell_queue failed");
        return false;
    }
    return true;
}

static void queue_to_main_handler(void) {
    if (xQueueSend(loadcell_to_main_queue, &weight_data, pdMS_TO_TICKS(200)) != pdPASS) {
        ESP_LOGE(TAG, "FAILED to send loadcell_data_t to main queue");
    }
}