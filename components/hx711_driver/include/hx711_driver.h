#ifndef HX711_DRIVER_H
#define HX711_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int dout_pin; // Pin Data HX711 (DT)
  int sck_pin;  // Pin Clock HX711 (SCK)
  float calibration_factor; // Faktor kalibrasi (misalnya, pulsa per gram)
  long offset;              // Offset nol
  uint8_t gain;             // Gain (128, 64, atau 32)
} hx711_config_t;

// -- Fungsi-fungsi C wrapper untuk HX711 --

/**
 * @brief Menginisialisasi HX711.
 * @param config Pointer ke struktur konfigurasi hx711_config_t.
 * @return true jika berhasil, false jika gagal (misalnya, pin tidak valid).
 */
bool hx711_wrapper_init(const hx711_config_t *config);

/**
 * @brief Mengatur gain HX711.
 * @param gain Gain yang diinginkan (128, 64, atau 32).
 */
void hx711_wrapper_set_gain(uint8_t gain);

/**
 * @brief Mengatur faktor kalibrasi.
 * @param scale_factor Faktor kalibrasi (misalnya, pulsa per unit berat).
 */
void hx711_wrapper_set_scale(float scale_factor);

/**
 *
 * @return Nilai dari scale_factor yang digunakan hx711
 */
float hx711_wrapper_get_scale(void);

/**
 * @brief Mengatur offset (nilai nol).
 * @param offset_val Nilai offset.
 */
void hx711_wrapper_set_offset(long offset_val);

/**
 *
 * @return Nilai dari offset_val
 */
long hx711_wrapper_get_offset(void);

/**
 * @brief Membaca nilai mentah dari HX711.
 * @return Nilai mentah dari ADC.
 */
long hx711_wrapper_read_raw(void);

/**
 * @brief Membaca nilai berat setelah apply scale dan offset.
 * @param times Jumlah pembacaan rata-rata (default 1).
 * @return Nilai berat dalam unit yang dikalibrasi.
 */
float hx711_wrapper_get_units(int times);

/**
 * @brief Melakukan tare (mengatur nol) pada HX711.
 * @param times Jumlah pembacaan rata-rata untuk tare (default 1).
 */
void hx711_wrapper_tare(int times);

/**
 * @brief Memeriksa apakah HX711 siap untuk membaca.
 * @return true jika siap, false jika tidak.
 */
bool hx711_wrapper_is_ready(void);

#ifdef __cplusplus
}
#endif

#endif //HX711_DRIVER_H