/**
 * @file    rsi_ble_peripheral_DEMO_4.c
 * @version 0.1
 * @date    xx Oct 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BT Classic SPP Slave Role
 *
 *  @section Description  This application serves as a BT SPP Slave.
 *
 */

/*=======================================================================*/
//   ! INCLUDES
/*=======================================================================*/

#include <rsi_common_app.h>
#if BLE_POWERSAVE
//! BT include file to refer BT APIs
#include <rsi_driver.h>
#include <rsi_bt_common_apis.h>
#include <rsi_ble_apis.h>
#include <rsi_bt.h>
#include <stdio.h>
#include <rsi_ble.h>

/*=======================================================================*/
//   ! MACROS
/*=======================================================================*/

/*=======================================================================*/
//   ! GLOBAL VARIABLES
/*=======================================================================*/

uint8_t         conn_dev_addr[18] = { 0 };
uint16_t        num_of_conn_slaves = 0;
uint8_t         num_of_conn_masters = 0;
uint16_t        conn_req_pending = 0;
uint8_t         rsi_conn_update = 0;

static uint8_t  str_remote_address[18];
static uint8_t  remote_dev_address[6];
static uint32_t rsi_app_async_event_map = 0;
static uint32_t rsi_ble_async_event_mask = 0;
static uint8_t  rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = { 0 };

static rsi_bt_resp_get_local_name_t       rsi_app_resp_get_local_name = { 0 };
static rsi_ble_event_remote_features_t 	  remote_dev_feature;

/*=======================================================================*/
//   ! EXTERN VARIABLES
/*=======================================================================*/

/*=======================================================================*/
//   ! EXTERN FUNCTIONS
/*=======================================================================*/

/*========================================================================*/
//!  CALLBACK FUNCTIONS
/*=======================================================================*/

/*=======================================================================*/
//   ! PROCEDURES
/*=======================================================================*/

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
 * @fn         rsi_ble_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_ble_app_clear_event(uint32_t event_num) {
	rsi_app_async_event_map &= ~BIT(event_num);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
static int32_t rsi_ble_app_get_event(void) {
	uint32_t ix;

	for (ix = 0; ix < 32; ix++) {
		if (rsi_app_async_event_map & (1 << ix)) {
			return ix;
		}
	}

	return (RSI_FAILURE);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_enhance_conn_status_event(
		rsi_ble_event_enhance_conn_status_t *resp_enh_conn) {
	memcpy ((int8_t *) remote_dev_address,	resp_enh_conn->dev_addr, 6);
	LOG_PRINT("\nconnect - str_remote_address : %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *) str_remote_address,	resp_enh_conn->dev_addr));
	rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_connect_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn) {
	memcpy ((int8_t *) remote_dev_address,	resp_conn->dev_addr, 6);
	LOG_PRINT("\nconnect - str_remote_address : %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *) str_remote_address,	resp_conn->dev_addr));
	rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This Callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(
		rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason) {
	//uint8_t str_disconn_device[18] = { 0 };

	//rsi_6byte_dev_address_to_ascii((int8_t *) str_disconn_device,	(uint8_t *) resp_disconnect->dev_addr);
	rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
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
	LOG_PRINT("\nFeature received is %d\n",
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
 * @fn         rsi_ble_dual_role
 * @brief      Tests the BLE GAP peripheral role.
 * @param[in]  none
 * @return    none.
 * @section description
 * This function is used to test the BLE peripheral role and simple GAP API's.
 */
int32_t rsi_ble_dual_role(void) {
	int32_t status = 0;
	uint8_t adv_data[31] = { 0 };
	int8_t adv_data_length = 0;

	//! initialize the event map
	rsi_ble_app_init_events();

	//! registering the GAP callback functions
	rsi_ble_gap_register_callbacks(NULL,
			rsi_ble_on_connect_event, rsi_ble_on_disconnect_event,
			NULL, NULL,
			NULL,
			rsi_ble_on_enhance_conn_status_event,
			NULL, rsi_ble_on_conn_update_complete_event);

	rsi_ble_gap_extended_register_callbacks(
			rsi_ble_simple_peripheral_on_remote_features_event,
			NULL);


	//! get the local device address(MAC address).
	status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
	if (status != RSI_SUCCESS) {
		return status;
	}

	//! set the local device name
	status = rsi_bt_set_local_name(RSI_BLE_LOCAL_NAME);
	if (status != RSI_SUCCESS) {
		return status;
	}

	//! get the local device name
	status = rsi_bt_get_local_name(&rsi_app_resp_get_local_name);
	if (status != RSI_SUCCESS) {
		return status;
	}

	adv_data[adv_data_length] = 2;
	adv_data_length += 1;

	adv_data[adv_data_length] = 1;
	adv_data_length += 1;

	adv_data[adv_data_length] = 6;
	adv_data_length += 1;

	//Name
	adv_data[adv_data_length] = strlen(RSI_ADV_DATA_NAME) + 1;
	adv_data_length += 1;

	adv_data[adv_data_length] = 9;
	adv_data_length += 1;

	strcpy((char *) &adv_data[adv_data_length], RSI_ADV_DATA_NAME);
	adv_data_length += strlen(RSI_ADV_DATA_NAME);

	//! set advertise data
	rsi_ble_set_advertise_data(adv_data, adv_data_length);

	//! start the advertising
	status = rsi_ble_start_advertising();
	if (status != RSI_SUCCESS) {
		return status;
	}

	//! initiateing power save in BLE mode
	status = rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE);
	if (status != RSI_SUCCESS) {
		return status;
	}
	return 0;
}

int32_t rsi_ble_app_task() {
	int32_t status = 0;
	int32_t temp_event_map = 0;

	//! checking for received events
	temp_event_map = rsi_ble_app_get_event();
	if (temp_event_map == RSI_FAILURE) {
		//! if events are not received loop will be continued.
		//return 0;
	}

	//! if any event is received, it will be served.
	switch (temp_event_map) {
	case RSI_BLE_CONN_EVENT: {
		LOG_PRINT("\n Device connected \n ");
		LOG_PRINT("\n Number of devices connected:%d \n", num_of_conn_slaves);
		conn_req_pending = 0;
		rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);
	}
		break;

	case RSI_BLE_DISCONN_EVENT: {
		LOG_PRINT("\n Device disconnected\n ");

		//! initiateing power save in BLE mode
		status = rsi_bt_power_save_profile(0, 0);
		if (status != RSI_SUCCESS) {
			return status;
		}
		status = rsi_ble_start_advertising();
		//! initiateing power save in BLE mode
		status = rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE);
		if (status != RSI_SUCCESS) {
			return status;
		}
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);
	}
		break;

	case RSI_BLE_RECEIVE_REMOTE_FEATURES: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);

		status = rsi_ble_conn_params_update((int8_t *) remote_dev_address, 200,
				200, 0, 400);
		if (status == RSI_SUCCESS) {
			return status;
		}
	}
		break;

	case RSI_BLE_CONN_UPDATE_EVENT: {

		rsi_ble_app_clear_event(RSI_BLE_CONN_UPDATE_EVENT);
	}
		break;
	}
	return 0;
}
#endif

