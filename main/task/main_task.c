#include <main_task.h>

static const char* TAG = "MAIN_TASK";

// global variable untuk di check
main_state_t main_state;
calibration_state_t calibration_state;

static cmd_t cmd_state;
static msg_data_t msg;
static response_msg_t response_msg;
static weight_t weight_data;
static oled_data_t oled_data;

// forward declaration
static void main_task_loop(void *pvParameters);
static void main_task_dispatcher(void);
static void calibration_dispatcher(void);

esp_err_t rcv_msg_from_comm_handler(void);
esp_err_t send_msg_to_comm_handler(void);
esp_err_t send_to_oled_handler(void);

void main_task_init(void) {
    // inisasi variabel

    xTaskCreate(main_task_loop, "main_task", 8192, NULL, 5, NULL);
}

static void main_task_loop(void *pvParameters) {
    while (1) {
        //
        main_task_dispatcher();
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

static void main_task_dispatcher(void) {

    if (main_state == CALIBRATION_MODE) {
        calibration_dispatcher();
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

static void calibration_dispatcher(void) {

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