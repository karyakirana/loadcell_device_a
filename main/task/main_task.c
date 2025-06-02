#include <main_task.h>

static const char* TAG = "MAIN_TASK";

// global variable untuk di check
main_state_t main_state;
calibration_state_t calibration_state;

typedef enum {
    TASK_RECEIVED,
    TASK_PROCESS,
    TASK_RETURN,
    TASK_NONE
} main_task_state_t;

static main_task_state_t main_task_state = TASK_NONE; 

static cmd_t cmd_state;
static main_to_comm_t main_to_comm_data;
static comm_to_main_t comm_to_main_data;
static response_msg_t response_msg;
static weight_t weight_data;
static oled_data_t oled_data;
static loadcell_recv_data_t main_to_loadcell_data;
static loadcell_data_t loadcell_data;

// forward declaration
static void main_task_loop(void *pvParameters);
static void main_task_dispatcher(void);
static void calibration_dispatcher(void);

esp_err_t rcv_from_comm_handler(void);
esp_err_t rcv_from_loadcell_handler(void);
esp_err_t send_to_comm_handler(void);
esp_err_t send_to_oled_handler(void);
esp_err_t send_to_loadcell_handler(void);

static void main_task_normalmode(void);
static void main_task_taremode(void);
static void main_task_calibrationmode(void);
static void main_task_sleepmode(void);
static void main_task_deepsleepmode(void);
static void main_task_wakeupmode(void);
static void main_task_errormode(void);

static void main_calinit(void);
static void main_calloadweights(void);
static void main_calwaiting(void);
static void main_calinput(void);
static void main_calconfirmation(void);
static void main_calerror(void);

void main_task_init(void) {
    // inisasi variabel

    // oled initiate data
    oled_data.line_1 = "";
    oled_data.line_2 = "";
    oled_data.line_3 = "";
    oled_data.line_4 = "";

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

esp_err_t rcv_from_comm_handler(void) {
    if (xQueueReceive(comm_to_main_queue, &comm_to_main_data, pdMS_TO_TICKS(200)) != pdPASS) {
        ESP_LOGW(TAG, "rcv comm_to_main_queue failed");
        return ESP_FAIL;
    }
    return ESP_FAIL;
}

esp_err_t rcv_from_loadcell_handler(void) {
    if (xQueueReceive(loadcell_to_main_queue, &loadcell_data, pdMS_TO_TICKS(200)) != pdPASS) {
        ESP_LOGW(TAG, "rcv loadcell_to_main_queue failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t send_to_comm_handler(void) {
    if (xQueueSend(main_to_comm_queue, &main_to_comm_data, pdMS_TO_TICKS(200)) != pdPASS) {
        ESP_LOGW(TAG, "rcv main_to_comm_queue failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t send_to_oled_handler(void) {
    if (xQueueSend(main_to_oled_queue, &oled_data, pdMS_TO_TICKS(200)) != pdPASS) {
        ESP_LOGW(TAG, "rcv main_to_oled_queue failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t send_to_loadcell_handler(void) {
    if (xQueueSend(main_to_loadcell_queue, &main_to_loadcell_data, pdMS_TO_TICKS(200)) != pdPASS) {
        ESP_LOGW(TAG, "rcv main_to_loadcell_queue failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

static void main_task_normalmode(void) {
    if (rcv_from_loadcell_handler() != ESP_OK) {
        return;
    }

    // weight_data filling
    weight_data.filtered_val = (sizeof(loadcell_data.filtered_val) == sizeof(float)) ? loadcell_data.filtered_val : 0.0f;
    weight_data.raw_val = (sizeof(loadcell_data.raw_val) == sizeof(int64_t)) ? loadcell_data.raw_val : 0;

    // main_to_comm_data filling
    main_to_comm_data.transaction_id = 0;
    main_to_comm_data.msg_type = COMMON_MSG;
    main_to_comm_data.is_return = true;
    main_to_comm_data.weight_data = weight_data;
    main_to_comm_data.command = CMD_NONE;

    // main_to_oled_data filling
    char buffer_filtered_val[16];
    char buffer_raw_val[16];
    snprintf(buffer_raw_val, sizeof(buffer_raw_val), "%lld", weight_data.raw_val);
    snprintf(buffer_filtered_val, sizeof(buffer_filtered_val), "%.2f", weight_data.filtered_val);
    oled_data.line_1 = "NORMAL_MODE";
    oled_data.line_2 = buffer_raw_val;
    oled_data.line_3 = buffer_filtered_val;
    oled_data.line_4 = "CMD_IDLE";

    send_to_comm_handler();
    send_to_oled_handler();
}

static void main_task_taremode(void) {
    if (comm_to_main_data.transaction_id == 0) {
        ESP_LOGE(TAG, "TRANSACTION ID = 0");
        return;
    }
    // task initiate
    if (main_task_state == TASK_NONE) {
        // main_to_loadcell filling
        main_to_loadcell_data.command = comm_to_main_data.command;

        // main_to_oled filling
        oled_data.line_1 = "TARE MODE";
        oled_data.line_2 = "";
        oled_data.line_3 = "";
        oled_data.line_4 = "CMD_TARE";

        send_to_loadcell_handler();
        main_task_state = TASK_RECEIVED;
    }
    // task received
    if (main_task_state == TASK_RECEIVED) {
        if (rcv_from_loadcell_handler() != ESP_OK) {
            return;
        }

        // weight data filling
        weight_data.filtered_val = 0.0f;
        weight_data.raw_val = 0;

        // main_to_comm data filling
        main_to_comm_data.transaction_id = loadcell_data.transaction_id;
        main_to_comm_data.msg_type = RESPONSE_MSG;
        main_to_comm_data.is_return = loadcell_data.is_success;
        main_to_comm_data.weight_data = weight_data;
        main_to_comm_data.command = CMD_NONE;

        send_to_comm_handler();
        main_task_state = TASK_NONE;
    }
}

static void main_task_calibrationmode(void) {
    //
}

static void main_task_sleepmode(void) {
    //
}

static void main_task_deepsleepmode(void) {}

static void main_task_wakeupmode(void) {}

static void main_task_errormode(void) {}


static void main_calinit(void) {
    // task initiated
    // task received response
}

static void main_calloadweights(void) {}

static void main_calwaiting(void) {}

static void main_calinput(void) {}

static void main_calconfirmation(void) {}

static void main_calerror(void) {}