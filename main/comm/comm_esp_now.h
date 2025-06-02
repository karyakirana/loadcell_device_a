#ifndef COMM_ESP_NOW_H
#define COMM_ESP_NOW_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_now.h"
#include "helpers/common_data.h"

typedef enum {
    COMM_SENT_IDLE,
    COMM_SENT_FAIL,
    COMM_SENT_SUCESS
} comm_internal_state_t;

typedef struct{
    uint8_t transaction_id;
    uint8_t mac_address;
    esp_now_send_status_t status;
} send_callback_t;

typedef struct{
    esp_now_recv_info_t *rcv_info;
    uint8_t *incoming_data;
    int data_len;
} rcv_callback_t;

extern QueueHandle_t send_callback_queue;
extern QueueHandle_t rcv_callback_queue;

esp_err_t comm_esp_now_init(const uint8_t receiver_mac[ESP_NOW_ETH_ALEN]);

esp_err_t comm_esp_send_msg(void);
esp_err_t comm_esp_rcv_msg(void);

esp_err_t send_esp_now_data(msg_data_t msg_data);

#endif // COMM_ESP_NOW_H