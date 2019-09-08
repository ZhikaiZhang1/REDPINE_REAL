/**
 * @file    rsi_ble_idle_powersave_DEMO_24.c
 * @version 0.1
 * @date    xx Feb 2019
 *
 *  Copyright(C) Redpine Signals 2019
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BLE powersave during idle conditions
 *
 *  @section Description  This application serves as a BLE.
 *
 */

/**
 * Include files
 * */
#include <rsi_common_app.h>
#if BLE_IDLE_POWERSAVE
//! BT include file to refer BT APIs
#include<rsi_driver.h>
#include<rsi_bt_common_apis.h>
#include<rsi_ble_apis.h>
#include<rsi_bt.h>
#include <stdio.h>
#include<rsi_ble.h>

//! Application global parameters
static uint32_t rsi_app_async_event_map = 0;
static uint32_t rsi_ble_async_event_mask = 0;

uint8_t conn_dev_addr[18] = { 0 };
uint16_t num_of_conn_slaves = 0;
uint8_t num_of_conn_masters = 0;
uint16_t conn_req_pending = 0;
uint8_t rsi_conn_update = 0;

static rsi_ble_event_remote_features_t 		remote_dev_feature;

extern uint8_t ble_powersave_d;
/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events() {
	rsi_app_async_event_map = 0;
	rsi_ble_async_event_mask = 0xFFFFFFFF;
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_set_event
 * @brief      sets the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
static void rsi_ble_app_set_event(uint32_t event_num) {
	rsi_app_async_event_map |= BIT(event_num);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_peripheral_on_remote_features_event
 * @brief      invoked when LE remote features event is received.
 * @param[in] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_peripheral_on_remote_features_event(
		rsi_ble_event_remote_features_t *rsi_ble_event_remote_features) {
	LOG_PRINT("Feature received is %d",
			rsi_ble_event_remote_features->remote_features);
	memcpy(&remote_dev_feature, rsi_ble_event_remote_features,
			sizeof(rsi_ble_event_remote_features_t));
	rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
}

void rsi_ble_on_conn_update_complete_event(
		rsi_ble_event_conn_update_t *rsi_ble_event_conn_update_complete,
		uint16_t resp_status) {
	rsi_ble_app_set_event(RSI_BLE_CONN_UPDATE_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_app_task_ble_ilde_powersave
 * @brief      Tests the BLE powersave.
 * @param[in]  none
 * @return    none.
 * @section description
 * This function is used to test the BLE powersave.
 */
int32_t rsi_app_task_ble_ilde_powersave(void) {
	int32_t status = 0;

	//! initialize the event map
	rsi_ble_app_init_events();

	//! initiating power save in BLE mode
	status = rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE);
	if (status != RSI_SUCCESS) {
		return status;
	}

	ble_powersave_d = 1;
	return 0;
}
#endif

