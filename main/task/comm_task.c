#include "comm_task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_now.h"

static const char* TAG = "COMM_TASK";

// status menunggu balasan dari pesan yang dikirim
// tidak semua perlu ini
static ret_cmd_t ret_cmd = RET_CMD_IDLE;

// jenis pesan
static msg_type_t msg_type = IDLE_MSG;

// state response pesan
static msg_state_t msg_state = IDLE;

// transaction id
static uint8_t trancaction_id;

// forward declaration
static void comm_task_loop(void *pvParameters);

static void 
IRAM_ATTR esp_now_send_cb(const uint8_t *mac_address, esp_now_send_status_t status);
static void 
IRAM_ATTR esp_now_recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *incoming_data, int data_len);

static void comm_state_dispatcher(void);
static void comm_cal_state_dispatcher(void);

static void comm_from_main_queue_handler(void);
static void comm_to_main_queue_handler(void);

static void transaction_id_received(void);

// --- function by state ---
static void comm_normalmode(void);
static void comm_taremode(void);
static void comm_sleepmode(void);
static void comm_deepsleepmode(void);
static void comm_wakeupmode(void);
static void comm_errormode(void);

static void comm_calinit(void);
static void comm_calloadweights(void);
static void comm_calwaiting(void);
static void comm_calinput(void);
static void comm_calconfirmation(void);
static void comm_calerror(void);

void comm_task_init(void) {
    //

    // daftarkan callback
    ESP_ERROR_CHECK(esp_now_register_send_cb(esp_now_send_cb)); // untuk status pengiriman
    ESP_ERROR_CHECK(esp_now_register_recv_cb(esp_now_recv_cb)); // untuk penerimaan

    xTaskCreate(comm_task_loop, "comm_task", 8192, NULL, 5, NULL);
}

static void comm_task_loop(void *pvParameters) {
    while(true) {
        // loop
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

static void 
IRAM_ATTR esp_now_send_cb(const uint8_t *mac_address, esp_now_send_status_t status) {
    //
}

static void 
IRAM_ATTR esp_now_recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *incoming_data, int data_len) {
    //
}

static void comm_state_dispatcher(void) {
    if (main_state == CALIBRATION_MODE) {
        comm_cal_state_dispatcher();
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

static void comm_cal_state_dispatcher(void) {
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

static void comm_from_main_queue_handler(void) {}
static void comm_to_main_queue_handler(void) {}

static void transaction_id_received(void) {}

// --- function by state ---
static void comm_normalmode(void) {
    // memberikan nilai weight
}

static void comm_taremode(void) {
    // menyimpan transaction_id sebagai variabel lokal
    // melakukan tare
    // mengirimkan hasil response dg transaction_id
    // ubah state menjadi normal
}

static void comm_sleepmode(void) {}
static void comm_deepsleepmode(void) {}
static void comm_wakeupmode(void) {}
static void comm_errormode(void) {}

static void comm_calinit(void) {
    // menyimpan transaction_id sebagai variabel lokal
    // inisiasi calibration init
    // mengirimkan hasi calibration init dg transaction_id
    // mengirimkan juga perubahan state
    // ubah state menjadi cal_load_weights
}

static void comm_calloadweights(void) {
    // menunggu sampai ada CAL_LOADWEIGHT_DONE
    // konfirmasi dari user sudah meletakkan berat
    // ubah state menjadi CAL_WAITING
}

static void comm_calwaiting(void) {
    // memberikan terus nilai raw dan is_stable
    // state akan berubah ketika ada input CAL_INPUT
    // ubah state
}

static void comm_calinput(void) {
    // mengirimkan data last_known_weight
    // menunggu status apakah data sudah diterima
    // jika belum, lakukan pengiriman lagi
    // jika sudah, tidak mengirimkan apa-apa
}
static void comm_calconfirmation(void) {
    // menerima CMD_CAL_CONFIRMATION
    // menyimpan nilai kalibrasi
    // memberikan response atas penyimpanan kalibrasi
    // jika error maka merubah main_state menjadi CAL_ERROR
    // jika berhasil akan kembali ke mode normal
}

static void comm_calerror(void) {}