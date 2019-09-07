/*
 * scheduler.h
 *
 *  Created on: Apr 14, 2019
 *
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include "native_gecko.h"
#include "mesh_generic_model_capi_types.h"
#include "gpio.h"
#include "mesh_lib.h"

// soft timer flags
#define DISPLAY_REFRESH 0x05
#define LOG_REFRESH 0x06
#define TIMER_ID_FACTORY_RESET 0x07
#define TIMER_ID_RESTART 0x08
#define TIMER_ID_FRIEND_FIND 0x09

// external event flag
#define BUTTON0_FLAG 			0x01
#define BUTTON1_FLAG 			0x02
#define GAS_FLAG                0x07
#define LUX_FLAG                0x05
#define PRESENCE_STATE          0x06

/// number of active Bluetooth connections
/// as shown in Silicon Labs switch example
static uint8 num_connections = 0;

uint32_t tickCount;
uint8_t trid;
uint16_t presence_value;

//function declarations
void set_device_name(bd_addr *pAddr);
static void init_models(void);
//static errorcode_t onoff_update_and_publish(uint16_t element_index,
//                                            uint32_t remaining_ms);
//static errorcode_t onoff_update(uint16_t element_index, uint32_t remaining_ms);
//static void onoff_request(uint16_t model_id,
//                          uint16_t element_index,
//                          uint16_t client_addr,
//                          uint16_t server_addr,
//                          uint16_t appkey_index,
//                          const struct mesh_generic_request *request,
//                          uint32_t transition_ms,
//                          uint16_t delay_ms,
//                          uint8_t request_flags);
//static void onoff_change(uint16_t model_id,
//                         uint16_t element_index,
//                         const struct mesh_generic_state *current,
//                         const struct mesh_generic_state *target,
//                         uint32_t remaining_ms);

static void pb_request(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
uint8_t request_flags);

static void pb_change(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
uint32_t remaining_ms);

static void br_request(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
uint8_t request_flags);

static void br_change(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
uint32_t remaining_ms);
static void sm_request(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
uint8_t request_flags);

static void sm_change(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
uint32_t remaining_ms);

int state_store_LPN(uint8_t state_index );
static int home_state_load_LPN(void);

#endif /* SRC_SCHEDULER_H_ */
