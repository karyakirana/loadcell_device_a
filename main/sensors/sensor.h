#ifndef SENSOR_H
#define SENSOR_H

#include <helpers/common_headers.h>

void sensor_init(void);

void sensor_update(void);

void sensor_set_read_time(uint8_t times);

float sensor_get_raw_value(void);

float sensor_get_units_value(void);

float sensor_get_filtered_units_value(void);

bool sensor_is_table(void);

/**
 * @brief Melakukan tare process
 * @return esp_err_t ESP_OK jika berhasil
 */
esp_err_t sensor_tare_process(void);

esp_err_t sensor_calibration_init(void);

esp_err_t sensor_calibration_input(float known_weight_val);

esp_err_t sensor_sleep_mode(void);

esp_err_t sensor_wakeup_mode(void);

#endif // SENSOR_H