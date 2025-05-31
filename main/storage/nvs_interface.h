#ifndef NVS_INTERFACE_H
#define NVS_INTERFACE_H

#include <helpers/common_headers.h>

/**
 *
 * @brief INIT NVS harus di main setup
 */
void app_nvs_init(void);

/**
 * @brief Sets an integer value from NVS
 *
 * Opens the specified namespace, sets key-value pair, and closes the namespace.
 *
 * @param namespace_name The NVS namespace name (string). Max length 15 chars.
 * @param key The key (string). Max length 15 chars.
 * @param value The int32_t value to store.
 * @return ESP_OK on success, or an error code from esp_err_t
 */
esp_err_t app_nvs_set_int32(const char* namespace_name, const char* key, int32_t value);

/**
 * @brief Gets an integer value from NVS.
 *
 * Opens the specified namespace, reads the key-value pair, and closes the namespace.
 *
 * @param namespace_name The NVS namespace name (string).
 * @param key The key (string).
 * @param out_value Pointer to an int32_t variable to store the read value.
 * @return ESP_OK on success, ESP_ERR_NVS_NOT_FOUND if key not found, or other error code.
 */
esp_err_t app_nvs_get_int32(const char* namespace_name, const char* key, int32_t* out_value);

/**
 * @brief Sets an float value from nvs
 *
 * Opens the specified namespace, sets key-value pair, and close the namespace.
 *
 * @param namespace_name The NVS namespace name (string). Max length 15 chars.
 * @param key he key (string). Max length 15 chars.
 * @param value The float value to store
 * @return ESP_OK on success, or an error code from esp_err_t
 */
esp_err_t app_nvs_set_float(const char* namespace_name, const char* key, float value);

/**
 * @brief Gets on float value from nvs
 * @param namespace_name
 * @param key
 * @param out_value
 * @return
 */
esp_err_t app_nvs_get_float(const char* namespace_name, const char* key, float* out_value);

/**
 * @brief Sets a string value in NVS
 *
 * Opens the specified namespace, sets the key-value pair, and close the namespace.
 *
 * @param namespace_name The NVS namespace name (string).
 * @param key The key (string)
 * @param value The string value to store. Must be null-terminated.
 * @return ESP_OK on success, or an error code from esp_err_t
 */
esp_err_t app_nvs_set_string(const char* namespace_name, const char* key, const char* value);

/**
 * @brief Gets a string value from NVS.
 *
 * Opens the specified namespace, reads the value for the key, and closes the namespace.
 * Allocates memory for the string internally.
 *
 * @param namespace_name The NVS namespace name (string).
 * @param key The key (string).
 * @param out_value_ptr Pointer to a char* pointer.
 * On success, this will point to a null-terminated string on the heap.
 * ** THE user must free() this memory when no longer needed.**
 * @return ESP_OK on success, ESP_ERR_NVS_NOT_FOUND if the key not found,
 * ESP_ERR_NO_MEM if memory allocation fails, or other error code.
 */
esp_err_t app_nvs_get_string(const char* namespace_name, const char* key, char** out_value_ptr);

/**
 * @brief Checks if a key exists in the specified namespace.
 *
 * Opens the namespace, checks for the key, and closes the namespace.
 *
 * @param namespace_name The NVS namespace name (string).
 * @param key The key (string).
 * @param out_exists Pointer to a bool to store the result (true if exists, false otherwise).
 * @return ESP_OK on success, or an error code (e.g., ESP_ERR_NVS_NOT_FOUND if namespace doesn't exists).
 */
esp_err_t app_nvs_key_exists(const char* namespace_name, const char* key, bool* out_exists);

/**
 * @brief Erases a specific key-value pair from NVS.
 *
 * Opens the namespace, erases the key, and closes the namespace.
 *
 * @param namespace_name The NVS namespace name (string).
 * @param key The key (string).
 * @return ESP_OK on success, ESP_ERR_NVS_NOT_FOUND if key not found, or other error code.
 */
esp_err_t app_nvs_erase_key(const char* namespace_name, const char* key);

/**
 * @brief Erase all key-value pairs in the specified namespace.
 *
 * Opens the namespace, erases all keys within it, and closes the namespace.
 *
 * @param namespace_name The NVS namespace name (string).
 * @return ESP_OK on success, or an error code (e.g., ESP_ERR_NVS_NOT_FOUND if namespace doesn't exist).
 */
esp_err_t app_nvs_erase_namespace(const char* namespace_name);

#endif // NVS_INTERFACE_H