#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <stdint.h>
#include <stdbool.h>

// state
typedef enum {
  IDLE_MODE,
  NORMAL_MODE,
  TARE_MODE,
  CALIBRATION_MODE,
  SLEEP_MODE,
  DEEPSLEEP_MODE,
  WAKEUP_MODE,
  ERROR_MODE
} main_state_t;

typedef enum {
  CAL_IDLE,
  CAL_INIT_MODE,
  CAL_LOAD_WEIGHTS,
  CAL_WAITING,
  CAL_INPUT_MODE,
  CAL_CONFIRMATION_MODE,
  CAL_ERROR_MODE
} calibration_state_t;

typedef enum {
  IDLE,
  FAILED,
  RECEIVED,
  RESPONSE,
  TIMEOUT
} msg_state_t;

typedef enum {
  IDLE_MSG,
  COMMON_MSG,
  COMMAND_MSG,
  RESPONSE_MSG,
  ACK_MSG
} msg_type_t;

typedef enum {
  CMD_IDLE,
  CMD_NORMAL,
  CMD_CALIBRATION,
  CMD_CAL_INIT,
  CMD_CAL_LOAD_WEIGHTS,
  CMD_CAL_WAITING,
  CMD_CAL_INPUT,
  CMD_CAL_CONFIRMATION,
  CMD_CAL_ERROR,
  CMD_SLEEP,
  CMD_DEEPSLEEP,
  CMD_WAKEUP,
  CMD_ERROR
} cmd_t;

// command state
// state ketika command sudah dikirim
// STATE KETIKA CMD DIKIRIM SAMPAI ACK DITERIMA
typedef enum {
  RET_CMD_IDLE, // TIDAK NGAPA-NGAPAIN
  RET_CMD_WAIT, // MENUGGGU RESPONSE 
  RET_CMD_RETRY, // MENGULANG PENGIRIMAN
  RET_CMD_SUCCESS, // CMD DIRESPONSE 
} ret_cmd_t;

// struct yang dikirim
typedef struct {
  int64_t raw_val;
  float filtered_val;
  float known_weight;
} weight_t;

// pesan yang dikirim ke device B
typedef struct {
  uint8_t     transaction_id;
  msg_type_t  msg_type;
  msg_state_t msg_state;
  cmd_t       cmd;
  weight_t    weight_data;
  uint8_t     status; // response code 
} msg_data_t;

// pesan yang diterima dari device B
typedef struct {
  uint8_t     transaction_id;
  msg_type_t  msg_type;
  msg_state_t msg_state;
  cmd_t       cmd;
  float       val;
  uint8_t     status; // response code
} response_msg_t;

// --- INTERNAL DATA ---

// oled_data
typedef struct {
  const char* line_1;
  const char* line_2;
  const char* line_3;
  const char* line_4;
} oled_data_t;

#endif