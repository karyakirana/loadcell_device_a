#include "comm_task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_now.h"
#include "helpers/hardware_config.h"

static const char* TAG = "COMM_TASK";

typedef enum {
    COMM_SENT_IDLE,
    COMM_SENT_FAIL,
    COMM_SENT_SUCESS
} comm_internal_state_t;

// mac address
static uint8_t receiver_mac[ESP_NOW_ETH_ALEN] = MAC_RECV;

// main_to_comm_data
static main_to_comm_t main_data;
static weight_t weight;

// data yang dikirim ke device B
static msg_data_t msg_data;

// status menunggu balasan dari pesan yang dikirim
// tidak semua perlu ini
static ret_cmd_t ret_cmd = RET_CMD_IDLE;

// jenis pesan
static msg_type_t msg_type = IDLE_MSG;

// state response pesan
static msg_state_t msg_state = IDLE;

// transaction id
static uint8_t trancaction_id;

// variabel-variabel untuk status pengiriman data khusus
static uint8_t sent_transaction_id = 0;
static bool awaiting_send_confirm = false;
static comm_internal_state_t internal_state = COMM_SENT_IDLE;

// forward declaration
static void comm_task_loop(void *pvParameters);
static esp_err_t comm_espnow_init(void);

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
    // ESP INIT
    ESP_ERROR_CHECK(comm_espnow_init());

    xTaskCreate(comm_task_loop, "comm_task", 8192, NULL, 5, NULL);
}

static void comm_task_loop(void *pvParameters) {
    while(true) {
        // loop
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

static esp_err_t comm_espnow_init(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "ESP WIFI_MODE_STA");

    // --- KRUSIAL: INISIALISASI ESP-NOW DI SINI ---
    esp_err_t esp_now_init_ret = esp_now_init();
    if (esp_now_init_ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize ESP-NOW: %s", esp_err_to_name(esp_now_init_ret));
        return esp_now_init_ret; // Kembalikan error jika inisialisasi ESP-NOW gagal
    }
    ESP_LOGI(TAG, "ESP-NOW initialized.");

    // tambahkan peer (penerima)
    esp_now_peer_info_t peer_info = {};
    memcpy(peer_info.peer_addr, receiver_mac, ESP_NOW_ETH_ALEN);
    peer_info.channel = 0; // channel wifi saat ini
    peer_info.encrypt = false;
    peer_info.ifidx = WIFI_IF_STA;

    // daftarkan callback
    ESP_ERROR_CHECK(esp_now_register_send_cb(esp_now_send_cb)); // untuk status pengiriman
    ESP_ERROR_CHECK(esp_now_register_recv_cb(esp_now_recv_cb)); // untuk penerimaan

    ESP_LOGI(TAG, "ADDING PPER: %02x:%02x:%02x:%02x:%02x:%02x", 
            receiver_mac[0], receiver_mac[1], receiver_mac[2], receiver_mac[3],
            receiver_mac[4], receiver_mac[5]
    );

    // check peer
    if (!esp_now_is_peer_exist(receiver_mac)) {
        esp_err_t add_peer_ret = esp_now_add_peer(&peer_info);
        if (add_peer_ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to add peer: %s", esp_err_to_name(add_peer_ret));
            return ESP_FAIL;
        }
        ESP_LOGI(TAG, "Successfully added PEER");
    } else {
        ESP_LOGI(TAG, "PEER already exist");
    }

    return ESP_OK;
}

static void 
IRAM_ATTR esp_now_send_cb(const uint8_t *mac_address, esp_now_send_status_t status) {
    char mac_str[18];
    snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]
            );

    if (awaiting_send_confirm) {
        ESP_LOGI(TAG, "Status transaction ID: %du", sent_transaction_id);
        awaiting_send_confirm = false;
        
        if (status == ESP_NOW_SEND_SUCCESS) {
            internal_state = COMM_SENT_SUCESS;
        }

        if (status == ESP_NOW_SEND_FAIL) {
            internal_state = COMM_SENT_FAIL;
        }
    }

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

static void comm_from_main_queue_handler(void) {
    if (xQueueReceive(main_to_comm_queue, &main_data, pdMS_TO_TICKS(200)) != pdPASS) {
        ESP_LOGW(TAG, "Failed to receive main_to_comm_queue");
    }
}

static void comm_to_main_queue_handler(void) {}

static void transaction_id_received(void) {}

// --- function by state ---
static void comm_normalmode(void) {
    // memberikan nilai weight
    comm_from_main_queue_handler();
    

    if (sizeof(main_data.weight_data) == sizeof(weight_t)) {
        weight = main_data.weight_data;
    } else {
        weight.filtered_val = 0.0f;
        weight.raw_val = 0;
    }

    // fill msg_data
    msg_data.transaction_id = 0;
    msg_data.msg_type = COMMON_MSG;
    msg_data.cmd = CMD_IDLE;
    msg_data.weight_data = weight;
    msg_data.status = true;
    
    // send via esp-now
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