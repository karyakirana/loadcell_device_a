#include "sensor.h"
#include "helpers/hardware_config.h"
#include "hx711_driver.h"
#include "nvs_interface.h"

static const char* TAG = "SENSOR";

static float calibration_factor = 1.0f;
static long offset_val = 0;
static uint8_t sensor_read_time = 10;
static float known_weight_value = 10000.0f; // default value

static int32_t raw_val = 0;
static float units_val = 0.0f;
static float filtered_units_val = 0.0f;
static bool is_stable_val = false;

// forward declaration -- start
static void filtered_weight(void);
// forward declaration -- end

void sensor_init(void) {

    // nvs init
    app_nvs_init();

    // check offset from nvs
    if (app_nvs_get_int32(TAG, "offset", &offset_val) != ESP_OK) {
        ESP_LOGW(TAG, "GET offset_val from NVS failed");
        offset_val = 0;
    }

    // check calibration_factor from nvs
    if (app_nvs_get_float(TAG, "calibration_factor", &calibration_factor) != ESP_OK) {
        ESP_LOGW(TAG, "GET calibration_factor from NVS failed");
        calibration_factor = 1.0f;
    }

    hx711_config_t hx711_config = {
        .dout_pin = HX711_DOUT_PIN,
        .sck_pin = HX711_SCK_PIN,
        .calibration_factor = calibration_factor,
        .offset = offset_val,
        .gain = HX711_GAIN
    };

    hx711_wrapper_init(&hx711_config);
}

void sensor_update(void) {
    // update val
    raw_val = hx711_wrapper_get_averages(sensor_read_time); // read raw
    units_val = hx711_wrapper_get_units(sensor_read_time);
}

void sensor_set_read_time(uint8_t times) {
    sensor_read_time = times;
}

float sensor_get_raw_value(void) {
    return raw_val;
}

float sensor_get_units_value(void) {
    return units_val;
}

float sensor_get_filtered_units_value(void) {
    return filtered_units_val;
}

bool sensor_is_table(void) {
    return is_stable_val;
}

esp_err_t sensor_tare_process(void) {
    // last offset_val
    long last_offset_val = offset_val;
    // tare proses
    hx711_wrapper_tare(10);
    // get offset_vall from hx711
    long new_offset_val = hx711_wrapper_get_offset(); 
    // save to nvs
    if (app_nvs_set_int32(TAG, "offset", new_offset_val) == ESP_OK) {
        offset_val = new_offset_val;
        return ESP_OK;
    }
    // if fail save to nvs, rollback val
    offset_val = last_offset_val;
    hx711_wrapper_set_offset(last_offset_val);
    return ESP_FAIL;
}

esp_err_t sensor_calibration_init(void) {
    // check and set known weight value
    return ESP_FAIL;
}

esp_err_t sensor_calibration_input(float known_weight_val) {
    // save nvs
    return ESP_FAIL;
}

esp_err_t sensor_sleep_mode(void) {
    return ESP_FAIL;
}

esp_err_t sensor_wakeup_mode(void) {
    return ESP_FAIL;
}