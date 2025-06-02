#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <helpers/common_headers.h>
#include <helpers/common_data.h>
#include <task/main_task.h>
#include <task/comm_task.h>
#include <task/loadcell_task.h>
#include <task/oled_task.h>

static const char* TAG = "MAIN";

// Global variable
QueueHandle_t comm_to_main_queue;
QueueHandle_t main_to_comm_queue;
QueueHandle_t main_to_oled_queue;
QueueHandle_t loadcell_to_main_queue;
QueueHandle_t main_to_loadcell_queue;

// 

void app_main(void)
{
  // config debug
  esp_log_level_set("*", ESP_LOG_DEBUG);
  
  // inisiasi awal QueueHandle_t
  comm_to_main_queue = xQueueCreate(5, sizeof(comm_to_main_t));
  if (comm_to_main_queue == NULL) {
    ESP_LOGE(TAG, "Failed to create comm_to_main_queue");
  }

  main_to_comm_queue = xQueueCreate(5, sizeof(main_to_comm_t));
  if (main_to_comm_queue == NULL) {
    ESP_LOGE(TAG, "Failed to create main_to_comm_queue");
  }

  main_to_oled_queue = xQueueCreate(5, sizeof(oled_data_t));
  if (main_to_oled_queue == NULL) {
    ESP_LOGE(TAG, "Failed to create main_to_oled_queue");
  }

  loadcell_to_main_queue = xQueueCreate(5, sizeof(loadcell_data_t));
  if (loadcell_to_main_queue == NULL) {
    ESP_LOGE(TAG, "FAILED to create loadcell_to_main_queue");
  }

  main_to_loadcell_queue = xQueueCreate(2, sizeof(loadcell_recv_data_t));
  if (main_to_loadcell_queue == NULL) {
    ESP_LOGE(TAG, "FAILED to create main_to_loadcell_queue");
  }

  loadcell_task_init(); // harus di atas
  comm_task_init();
  oled_task_init();
  main_task_init();
}