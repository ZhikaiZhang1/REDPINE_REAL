/**
 * @file    rsi_ble_app.c
 * @version 0.1
 * @date    17 Nov 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BLE simple chat
 *
 *  @section Description  This file contains example application for BLE simple chat
 *
 */

/**
 * Include files
 * */


//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<stdio.h>
#include<string.h>
#include<rsi_ble.h>
#include<rsi_ant.h>
#include<rsi_bt_common.h>
#include<rsi_ble_apis.h>

//! Common include file 
#include<rsi_common_apis.h>



//! application defines 
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT                     
#endif


#ifndef BLE_CENTRAL
#define BLE_DATA             0

//! max data length
#define  RSI_BLE_MAX_DATA_LEN           20

//! local device name
#define RSI_BLE_APP_DEVICE_NAME        "ANT+BT_BLE_DUAL_MODE"

//! application event list
#define  RSI_BLE_CONN_EVENT             0x01
#define  RSI_BLE_DISCONN_EVENT          0x02

//! Enumeration for commands used in application
typedef enum rsi_app_cmd_e
{
  RSI_DATA = 0
}rsi_app_cmd_t;

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t rsi_app_resp_device_state = 0;
static rsi_ble_event_conn_status_t conn_event_to_app = {0};
static rsi_ble_event_disconnect_t disconn_event_to_app = {0};
static uint8_t rsi_ble_app_data[RSI_BLE_MAX_DATA_LEN];

/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events()
{
  ble_app_event_map = 0;
  ble_app_event_mask = 0xFFFFFFFF;
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
static void rsi_ble_app_set_event(uint32_t event_num)
{
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
static void rsi_ble_app_clear_event(uint32_t event_num)
{
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
static int32_t rsi_ble_app_get_event(void)
{
  uint32_t  ix;

  for (ix = 0; ix < 32; ix++)
  {
    if (ble_app_event_map & (1 << ix))
    {
      return ix;
    }
  }

  return (-1);
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
	conn_event_to_app.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (conn_event_to_app.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  conn_event_to_app.status = resp_enh_conn->status;
	rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
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
  memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
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
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  LOG_PRINT ("rsi_ble_on_disconnect_event \r\n");
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_send_to_ble
 * @brief      data transfer from BT module to BLE module.
 * @param[in]  data_type, data type, now we are using sample data type '0'.
 * @param[in]  data, raw data received from connected BT device..
 * @param[in]  data_len, raw data length
 * @return     none.
 * @section description
 * This function is used to initialize the BLE module
 */
#if 0
void rsi_bt_app_send_to_ble (uint8_t  data_type, uint8_t *data, uint16_t data_len) 
{
  uint16_t rx_len;
  if ((data == NULL) || 
      (data_len == 0)) {
    return;
  }
  //app_ble_write_event.length = RSI_MIN (RSI_BLE_MAX_DATA_LEN, data_len);
  //memset (app_ble_write_event.att_value, 0, sizeof (app_ble_write_event.att_value));
	//memcpy (&app_ble_write_event.att_value, data, app_ble_write_event.length);
  rx_len = RSI_MIN (RSI_BLE_MAX_DATA_LEN, data_len);
  memset (rsi_ble_app_data, 0, RSI_BLE_MAX_DATA_LEN);
  memcpy (rsi_ble_app_data, data, rx_len);
  rsi_ble_app_set_event (RSI_BLE_GATT_WRITE_EVENT);
}
#endif

/*==============================================*/
/**
 * @fn         rsi_ble_app_init
 * @brief      initialize the BLE module.
 * @param[in]  none
 * @return     none.
 * @section description
 * This function is used to initialize the BLE module
 */
uint32_t rsi_ble_app_init (void)
{
	uint8_t adv[31] = {2,1,6};
  
  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(
      NULL,
      rsi_ble_on_connect_event,
      rsi_ble_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_on_enhance_conn_status_event,
      NULL,
      NULL);

  //! Set local name
  rsi_bt_set_local_name (RSI_BLE_APP_DEVICE_NAME);

	//! prepare advertise data //local/device name
  adv[3] = strlen (RSI_BLE_APP_DEVICE_NAME) + 1;
  adv[4] = 9;
  strcpy ((char *)&adv[5], RSI_BLE_APP_DEVICE_NAME);

	//! set advertise data
	rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_APP_DEVICE_NAME) + 5);

  //! set device in advertising mode.
  rsi_ble_start_advertising ();
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_task
 * @brief      this function will execute when BLE events are raised.
 * @param[in]  none.
 * @return     none.
 * @section description
 */
void rsi_ble_app_task (void)
{
	int32_t status = 0;
	int32_t  event_id;
	int8_t   data[20] = {0};
	uint8_t  remote_dev_addr[18] = {0};
	uuid_t   search_serv;
	rsi_ble_resp_local_att_value_t  local_att_val;
	static uint8_t loop = 0;


	//! checking for events list
	event_id = rsi_ble_app_get_event ();
	if (event_id == -1) {
		return;
	}

	switch (event_id) {
		case RSI_BLE_CONN_EVENT:
			{
				//! event invokes when connection was completed

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);

				//! get the local device state.
				status = rsi_ble_get_device_state(&rsi_app_resp_device_state);
				if(status != RSI_SUCCESS)
				{
					return status;
				}
			}
			break;

		case RSI_BLE_DISCONN_EVENT:
			{
				//! event invokes when disconnection was completed

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);

				//! get the local device state.
				status = rsi_ble_get_device_state(&rsi_app_resp_device_state);
				if(status != RSI_SUCCESS)
				{
					return status;
				}

				//! set device in advertising mode.
adv:
				status = rsi_ble_start_advertising ();
				if (status != RSI_SUCCESS) {
					goto adv;
				}
			}
			break;
	}

	return ;
}
#endif
