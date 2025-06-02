#include "comm_esp_now.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_now.h"
#include "esp_log.h"
#include "helpers/hardware_config.h"
#include <string.h>

static const char* TAG = "COMM_ESP_NOW";

// variabel-variabel untuk status pengiriman data khusus
static uint8_t sent_transaction_id = 0;
static bool awaiting_send_confirm = false;
static comm_internal_state_t internal_state = COMM_SENT_IDLE;

static void 
IRAM_ATTR esp_now_send_cb(const uint8_t *mac_address, esp_now_send_status_t status);
static void 
IRAM_ATTR esp_now_recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *incoming_data, int data_len);


esp_err_t comm_esp_now_init(const uint8_t receiver_mac[ESP_NOW_ETH_ALEN]) {
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

esp_err_t comm_esp_send_msg(void) {
    return ESP_FAIL;
}

esp_err_t comm_esp_rcv_msg(void) {
    return ESP_FAIL;
}

esp_err_t send_esp_now_data(msg_data_t msg_data) {
    return ESP_FAIL;
}