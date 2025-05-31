#include <stdio.h>
#include "hx711_driver.h"
#include <hx711_bogde.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

static const char* TAG = "HX711_WRAPPER";

static HX711 scale;

#ifdef __cplusplus
extern "C" {
#endif

bool hx711_wrapper_init(const hx711_config_t* config) {
  if (config == NULL) {
    ESP_LOGE(TAG, "config pointer is NULL");
    return false;
  }

  ESP_LOGI(TAG, "Initializing HX711 on DOUT: %d, SCK: %d, GAIN: %d",
            config->dout_pin, config->sck_pin, config->gain);

  // Inisialisasi objek HX711 (memanggil method C++ dari objek 'scale')
  scale.begin(config->dout_pin, config->sck_pin);

  // Atur gain
  if (config->gain == 128) {
    scale.set_gain(128);
  } else if (config->gain == 64) {
    scale.set_gain(64);
  } else if (config->gain == 32) {
    scale.set_gain(32);
  } else {
    ESP_LOGW(TAG, "Invalid gain specified (%d). Using default 128.", config->gain);
    scale.set_gain(128);
  }

  vTaskDelay(pdMS_TO_TICKS(50));

  scale.set_scale(config->calibration_factor);
  scale.set_offset(config->offset);

  if (scale.is_ready()) {
    ESP_LOGI(TAG, "HX711 wrapper initialized succesfully");
    return true;
  } else {
    ESP_LOGE(TAG, "HX711 not ready after initialization.");
    return false;
  }
}

void hx711_wrapper_set_gain(uint8_t gain) {
  scale.set_gain(gain);
}

void hx711_wrapper_set_scale(float scale_factor) {
  scale.set_scale(scale_factor);
}

float hx711_wrapper_get_scale() {
  return scale.get_scale();
}

void hx711_wrapper_set_offset(long offset_val) {
  scale.set_offset(offset_val);
}

long hx711_wrapper_get_offset(void) {
  return scale.get_offset();
}

long hx711_wrapper_read_raw(void) {
  return scale.read();
}

float hx711_wrapper_get_units(int times) {
  if (times <= 0) times = 1;
  return scale.get_units(times);
}

void hx711_wrapper_tare(int times) {
  if (times <= 0) times = 1;
  scale.tare(times);
  ESP_LOGI(TAG, "HX711 wrapper tare done");
}

bool hx711_wrapper_is_ready(void) {
  return scale.is_ready();
}

#ifdef __cplusplus
}
#endif