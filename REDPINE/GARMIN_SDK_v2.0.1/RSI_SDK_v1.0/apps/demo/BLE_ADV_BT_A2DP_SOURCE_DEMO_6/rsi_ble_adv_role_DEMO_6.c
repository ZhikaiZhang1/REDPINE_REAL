/**
 * @file    rsi_ble_adv_role_DEMO_6.c
 * @version 0.1
 * @date    09 Sep 2018
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application to test the GATT client API's
 *
 *  @section Description  This application has covered most of the GATT client API's
 */

/*=======================================================================*/
//   ! INCLUDES
/*=======================================================================*/
#include <rsi_common_app.h>
#if BLE_ADV_BT_A2DP_SOURCE
//! BT include file to refer BT APIs
#include <rsi_ble_apis.h>
#include <rsi_ble_config.h>
#include <rsi_common_apis.h>
#include <rsi_bt_common.h>
#include <stdio.h>
#include <string.h>
#include "rsi_driver.h"


#ifdef RSI_WITH_OS
extern rsi_semaphore_handle_t coex_sem;
extern rsi_semaphore_handle_t suspend_sem;
#endif
/*=======================================================================*/
//   ! GLOBAL VARIABLES
/*=======================================================================*/
uint8_t           num_of_conn_masters = 0;
uint16_t          mtu_size;
static uint32_t   ble_app_event_map;
static rsi_ble_event_mtu_t app_ble_mtu_event;

/*=======================================================================*/
//   ! EXTERN VARIABLES
/*=======================================================================*/
/*
*/
/*========================================================================*/
//!  CALLBACK FUNCTIONS
/*=======================================================================*/
/*
*/
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
	ble_app_event_map = 0;
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_set_event
 * @brief      set the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
static void rsi_ble_app_set_event(uint32_t event_num) {
	ble_app_event_map |= BIT(event_num);
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
	ble_app_event_map &= ~BIT(event_num);
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
		if (ble_app_event_map & (1 << ix)) {
			return ix;
		}
	}

	return (-1);
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
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn)
{
	if(resp_conn->status != 0)
		return;
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
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(
		rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason) {

	rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
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
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
	if(resp_enh_conn->status != 0)
		return;
	rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
}



/*==============================================*/
/**
 * @fn         rsi_ble_on_mtu_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_mtu_event(rsi_ble_event_mtu_t *rsi_ble_mtu) {
	memcpy(&app_ble_mtu_event, rsi_ble_mtu, sizeof(rsi_ble_event_mtu_t));
	mtu_size = (uint16_t) app_ble_mtu_event.mtu_size;
	rsi_ble_app_set_event(RSI_BLE_MTU_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_gatt_test
 * @brief      this is the application of ble GATT client api's test cases.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used at application.
 */
int32_t rsi_ble_adv_role(void) {
	uint8_t adv[31] = { 2, 1, 6 };

	//! registering the GAP call back functions
	rsi_ble_gap_register_callbacks(NULL,//rsi_ble_simple_central_on_adv_report_event,
			rsi_ble_on_connect_event,
			rsi_ble_on_disconnect_event,
			NULL,
			NULL,
			NULL,
			rsi_ble_on_enhance_conn_status_event,
			NULL,
			NULL);

	//! registering the GATT call back functions
	rsi_ble_gatt_register_callbacks(NULL,
			NULL,//rsi_ble_profile,
			NULL,//rsi_ble_char_services,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,//rsi_ble_on_gatt_write_event,
			NULL,
			NULL,
			NULL,//rsi_ble_on_read_req_event,
			rsi_ble_on_mtu_event,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

	//!  initializing the application events map
	rsi_ble_app_init_events();

	//! Set local name
	rsi_bt_set_local_name(RSI_BLE_APP_GATT_TEST);

	//! prepare advertise data //local/device name
	adv[3] = strlen(RSI_BLE_APP_GATT_TEST) + 1;
	adv[4] = 9;
	strcpy((char *) &adv[5], RSI_BLE_APP_GATT_TEST);

	//! set advertise data
	rsi_ble_set_advertise_data(adv, strlen(RSI_BLE_APP_GATT_TEST) + 5);

	//! set device in advertising mode.
	rsi_ble_start_advertising();

	return 0;
}

void rsi_ble_app_task(void) {
	int32_t status = 0;
	int32_t event_id;
#ifndef RSI_WITH_OS
	//! Application main loop
	rsi_wireless_driver_task();
#else

	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	// Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! BLE dual role Initialization
	status = rsi_ble_adv_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("ble dual role init failed\n");
		return;
	}
	rsi_semaphore_post(&coex_sem);
while(1)
#endif
	{
#ifdef RSI_WITH_OS
	rsi_semaphore_wait(&suspend_sem,0);
#endif
	//! checking for events list
	event_id = rsi_ble_app_get_event();
	if (event_id == -1) {
		//continue;
	}

	switch (event_id) {

	case RSI_BLE_CONN_EVENT: {

		//! event invokes when connection was completed
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);
		LOG_PRINT("\n Device connected \n ");
		num_of_conn_masters++;

		if (num_of_conn_masters < 2) {
			status = rsi_ble_start_advertising();
			if (status != RSI_SUCCESS) {
				//return status;
			}
		}
	}
		break;

	case RSI_BLE_DISCONN_EVENT: {

		LOG_PRINT("\n Device disconnected\n ");
		rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);

		num_of_conn_masters--;

		if (num_of_conn_masters < 2) {
			status = rsi_ble_start_advertising();
			if (status != RSI_SUCCESS) {
				//return status;
			}
		}
	}
		break;

	case RSI_BLE_MTU_EVENT: {
		//! event invokes when write/notification events received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_MTU_EVENT);

	}
		break;

	}
	}
}
#endif
