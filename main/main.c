#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <helpers/common_headers.h>
#include <helpers/common_data.h>

static const char* TAG = "MAIN";

// Global variable
QueueHandle_t comm_to_main_queue;
QueueHandle_t main_to_comm_queue;
QueueHandle_t main_to_oled_queue;
QueueHandle_t loadcell_to_main_queue;

// 

void app_main(void)
{
  // config debug
  esp_log_level_set("*", ESP_LOG_DEBUG);
  
  // inisiasi awal QueueHandle_t
  comm_to_main_queue = xQueueCreate(5, sizeof(response_msg_t));
  if (comm_to_main_queue == NULL) {
    ESP_LOGE(TAG, "Failed to create comm_to_main_queue");
  }

  main_to_comm_queue = xQueueCreate(5, sizeof(msg_data_t));
  if (main_to_comm_queue == NULL) {
    ESP_LOGE(TAG, "Failed to create main_to_comm_queue");
  }

  main_to_oled_queue = xQueueCreate(5, sizeof(oled_data_t));
  if (main_to_oled_queue == NULL) {
    ESP_LOGE(TAG, "Failed to create main_to_oled_queue");
  }

  loadcell_to_main_queue = xQueueCreate(5, sizeof(weight_t));
  if (loadcell_to_main_queue == NULL) {
    ESP_LOGE(TAG, "FAILED to create loadcell_to_main_queue");
  }
}