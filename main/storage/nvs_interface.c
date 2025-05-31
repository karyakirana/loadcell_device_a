#include "nvs_interface.h"
#include <nvs_flash.h>
#include <nvs.h>

static const char* TAG = "NVS_INTERFACE";

void app_nvs_init() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK( ret );
  ESP_LOGI(TAG, "NVS initialized");
}

esp_err_t app_nvs_set_int32(const char* namespace_name, const char* key, int32_t value) {
  nvs_handle nvs_handle;

  // 1. Open NVS namespace
  esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error opening NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
    return err;
  }

  // 2. Set the integer value
  err = nvs_set_i32(nvs_handle, key, value);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error setting NVS key '%s': %s", key, esp_err_to_name(err));
    return err;
  }

  // 3.Commit changes to flash
  err = nvs_commit(nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error committing NVS value '%s': %s", key, esp_err_to_name(err));
    nvs_close(nvs_handle);
    return err;
  }

  // 4. Close namespace
  nvs_close(nvs_handle);

  // ESP_LOGI(TAG, "Successfully set NVS value '%s/%s'", namespace_name, key);
  return ESP_OK;
}

esp_err_t app_nvs_get_int32(const char* namespace_name, const char* key, int32_t* out_value) {
  nvs_handle nvs_handle;

  // ensure ouput pointer is valid
  if (!out_value) {
    return ESP_ERR_INVALID_ARG;
  }

  // 1. Open NVS namespace (readonly is sufficient
  esp_err_t err = nvs_open(namespace_name, NVS_READONLY, &nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error opening NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
    return err;
  }

  // 2. Get the integer value
  err = nvs_get_i32(nvs_handle, key, out_value);
  if (err != ESP_OK) {
    // log error unless it's NOT_FOUND, which might be expected.
    if (err == ESP_ERR_NVS_NOT_FOUND) {
      ESP_LOGE(TAG, "Error getting NVS key '%s': %d", key, err);
    }
    nvs_close(nvs_handle); // close before returning
    return err;
  }

  // 4. Close namespace
  nvs_close(nvs_handle);

  // ESP_LOGI(TAG, "Successfully got NVS key '%s/%s', value: %ld", namespace_name, key, *out_value);
  return ESP_OK;
}

esp_err_t app_nvs_set_float(const char* namespace_name, const char* key, float value) {
  nvs_handle nvs_handle;

  // 1. Open NVS namespace
  esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error opening NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
    return err;
  }

  // 2. Set the float value via blob
  err = nvs_set_blob(nvs_handle, key, &value, sizeof(value));
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error setting NVS key '%s': %s", key, esp_err_to_name(err));
    nvs_close(nvs_handle);
    return err;
  }

  // 3.Commit changes to flash
  err = nvs_commit(nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error committing NVS value '%s': %s", key, esp_err_to_name(err));
    nvs_close(nvs_handle);
    return err;
  }

  // 4. Close namespace
  nvs_close(nvs_handle);

  // ESP_LOGI(TAG, "Successfully set NVS value '%s/%s'", namespace_name, key);
  return ESP_OK;
}

esp_err_t app_nvs_get_float(const char* namespace_name, const char* key, float* out_value) {
  nvs_handle nvs_handle;

  // ensure ouput pointer is valid
  if (!out_value) {
    return ESP_ERR_INVALID_ARG;
  }

  // 1. Open NVS namespace
  esp_err_t err = nvs_open(namespace_name, NVS_READONLY, &nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error opening NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
    return err;
  }

  // 2. Get the blob value
  size_t required_size = sizeof(float);
  err = nvs_get_blob(nvs_handle, key, out_value, &required_size);
  if (err != ESP_OK) {
    // log error unless it's NOT_FOUND, which might be expected.
    if (err == ESP_ERR_NVS_NOT_FOUND) {
      ESP_LOGW(TAG, "NVS key '%s/%s' not found.", namespace_name, key);
    } else {
      ESP_LOGE(TAG, "Error getting NVS key '%s/%s': %s", namespace_name, key, esp_err_to_name(err));
    }
    nvs_close(nvs_handle); // close before returning
    return err;
  }

  // 4. Close namespace
  nvs_close(nvs_handle);

  // ESP_LOGI(TAG, "Successfully got NVS key '%s/%s', value: %f", namespace_name, key, *out_value);
  return ESP_OK;
}

esp_err_t app_nvs_set_string(const char* namespace_name, const char* key, const char* value) {
  nvs_handle nvs_handle;

  // 1. Open NVS namespace (readonly is sufficient
  esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error opening NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
    return err;
  }

  // 2. Set the string value
  err = nvs_set_str(nvs_handle, key, value);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error setting NVS key '%s': %s", key, esp_err_to_name(err));
    nvs_close(nvs_handle); // Close before returning
    return err;
  }

  // 3. commit the changes of flash
  err = nvs_commit(nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error committing NVS changes %s", esp_err_to_name(err));
    nvs_close(nvs_handle);
    return err;
  }

  // 4. Close namespace
  nvs_close(nvs_handle);

  // ESP_LOGI(TAG, "Successfully set NVS key '%s/%s'", namespace_name, key);
  return ESP_OK;
}

esp_err_t app_nvs_get_string(const char* namespace_name, const char* key, char** out_value_ptr) {
  nvs_handle nvs_handle;
  size_t required_size;

  // Ensure output pointer to pointer is valid
  if (!out_value_ptr) {
    return ESP_ERR_INVALID_ARG;
  }
  // Ensure the output pointer is set to NULL initially
  *out_value_ptr = NULL;

  // 1. Open NVS namespace (read only)
  esp_err_t err = nvs_open(namespace_name, NVS_READONLY, &nvs_handle);
  if (err != ESP_OK) {
    if (err != ESP_ERR_NVS_NOT_FOUND) {
      ESP_LOGE(TAG, "Error opening NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
    }
    return err;
  }

  // 2. Get the required size for the string
  err = nvs_get_str(nvs_handle, key, NULL, &required_size);
  if (err != ESP_OK) {
    if (err != ESP_ERR_NVS_NOT_FOUND) {
      ESP_LOGE(TAG, "Error getting NVS string size for key '%s': %s", key, esp_err_to_name(err));
    }
    nvs_close(nvs_handle); // Close before returning
    return err;
  }

  // 3. Allocate memory for the string
  *out_value_ptr = malloc(required_size);
  if (*out_value_ptr == NULL) {
    ESP_LOGE(TAG, "Failed to allocate memory for NVS string value");
    nvs_close(nvs_handle); // Close before returning
    return ESP_ERR_NO_MEM;
  }

  // 4. Get the actual string value
  err = nvs_get_str(nvs_handle, key, *out_value_ptr, &required_size);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error getting NVS string value for key '%s': %s", key, esp_err_to_name(err));
    free(*out_value_ptr); // Free allocated memory on failure
    *out_value_ptr = NULL;
    nvs_close(nvs_handle); // Close before returning
    return err;
  }


  // 5. Close NVS namespace
  nvs_close(nvs_handle);

  // ESP_LOGI(TAG, "Successfully got NVS key '%s/%s', value: '%s'", namespace_name, key, *out_value_ptr);
  return ESP_OK;
}

esp_err_t app_nvs_key_exists(const char* namespace_name, const char* key, bool* out_exists) {
  nvs_handle nvs_handle;
  int32_t dummy_value; // Use a dummy value to check existence for any type

    // Ensure output pointer is valid
    if (!out_exists) {
        return ESP_ERR_INVALID_ARG;
    }
    *out_exists = false; // Default to false

    // 1. Open NVS namespace (read only)
    esp_err_t err = nvs_open(namespace_name, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        // If namespace doesn't exist, the key definitely doesn't exist in it
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            return ESP_OK; // Namespace not found is not an *error* for key existence check
        }
        ESP_LOGE(TAG, "Error opening NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
        return err;
    }

    // 2. Try to get the key (doesn't matter the type, just checking existence)
    // Using nvs_get_i32 is simple, but might not work for non-int keys.
    // A more robust check involves querying keys, but that's more complex.
    // A common simple way is to just try to get *any* known type or use nvs_get_stats and iterate (complex).
    // Let's stick to the simple check with i32; if the key exists as a different type, this will fail
    // with a type mismatch error, which we can interpret as "not found as this type".
    // To truly check *if key exists AT ALL regardless of type*, you need NVS iteration API.
    // For this wrapper's simplicity, let's refine: Key exists if nvs_get_i32 returns ESP_OK or ESP_ERR_NVS_TYPE_MISMATCH.
    // Actually, nvs_get_i32 will return ESP_ERR_NVS_NOT_FOUND if the key doesn't exist *at all*.
    // It will return ESP_ERR_NVS_TYPE_MISMATCH if it exists but is not an i32.
    // So, key exists if err is NOT ESP_ERR_NVS_NOT_FOUND after the get attempt.

    err = nvs_get_i32(nvs_handle, key, &dummy_value);

    nvs_close(nvs_handle); // Close before checking err

    if (err == ESP_OK || err == ESP_ERR_NVS_TYPE_MISMATCH) {
        *out_exists = true; // Key exists (either as i32 or another type)
        return ESP_OK; // Return OK if we successfully determined existence
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        *out_exists = false; // Key does not exist
        return ESP_OK; // Return OK as we successfully determined non-existence
    } else {
        // Some other read error occurred
        ESP_LOGE(TAG, "Error checking NVS key '%s': %s", key, esp_err_to_name(err));
        return err; // Return the actual error
    }
}

esp_err_t app_nvs_erase_key(const char* namespace_name, const char* key) {
  nvs_handle nvs_handle;

  // 1. Open NVS namespace
  esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &nvs_handle);
  if (err != ESP_OK) {
    if (err != ESP_ERR_NVS_NOT_FOUND) { // Namespace not found is not an error for erase
      ESP_LOGE(TAG, "Error opening NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
    }
    return err; // Return original error including NOT_FOUND
  }

  // 2. Erase the key
  err = nvs_erase_key(nvs_handle, key);
  if (err != ESP_OK) {
    if (err != ESP_ERR_NVS_NOT_FOUND) { // Key not found is not an error for erase
      ESP_LOGE(TAG, "Error erasing NVS key '%s': %s", key, esp_err_to_name(err));
    }
    nvs_close(nvs_handle); // Close before returning
    return err; // Return original error including NOT_FOUND
  }

  // 3. Commit changes
  err = nvs_commit(nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error committing NVS erase: %s", esp_err_to_name(err));
    nvs_close(nvs_handle); // Close before returning
    return err;
  }

  // 4. Close NVS namespace
  nvs_close(nvs_handle);

  // ESP_LOGI(TAG, "Successfully erased NVS key '%s/%s'", namespace_name, key);
  return ESP_OK;
}

esp_err_t app_nvs_erase_namespace(const char* namespace_name) {
  nvs_handle nvs_handle;

  // 1. Open NVS namespace
  esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &nvs_handle);
  if (err != ESP_OK) {
    if (err != ESP_ERR_NVS_NOT_FOUND) { // Namespace not found is not an error for erase
      ESP_LOGE(TAG, "Error opening NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
    }
    return err; // Return original error including NOT_FOUND
  }

  // 2. Erase all keys in the namespace
  err = nvs_erase_all(nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error erasing NVS namespace '%s': %s", namespace_name, esp_err_to_name(err));
    nvs_close(nvs_handle); // Close before returning
    return err;
  }

  // 3. Commit changes
  err = nvs_commit(nvs_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error committing NVS erase all: %s", esp_err_to_name(err));
    nvs_close(nvs_handle); // Close before returning
    return err;
  }

  // 4. Close NVS namespace
  nvs_close(nvs_handle);

  // ESP_LOGI(TAG, "Successfully erased NVS namespace '%s'", namespace_name);
  return ESP_OK;
}