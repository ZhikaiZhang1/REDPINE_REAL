/**
 * @file    rsi_ble_central_app.c
 * @version 0.1
 * @date    05 Sep 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BLE Central mode.
 *
 *  @section Description  This application connects as a Central/Master.
 *
 */

/**
 * Include files
 * */

#include<rsi_common_app.h>

#if  ANT_BLE_CENT_BT_A2DP_SRC_APP
//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>s
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_ble.h>
#include<rsi_ant.h>
#include<rsi_bt_common.h>

#include <string.h>
//! Common include file 
#define RSI_BLE_LOCAL_NAME             "ANT+BLE_CENT+BT_A2DP_SRC"
//! application defines 
static rsi_bt_resp_get_local_name_t  rsi_ant_app_resp_get_local_name = {0};
static uint8_t rsi_ant_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = {0};

static uint32_t ble_ant_app_event_map;
static uint32_t ble_ant_app_event_mask;
rsi_ant_atm_mode_t atm_mode;
rsi_ant_get_ver_t  ant_get_version;
rsi_ant_get_stats_t ant_get_stats;                  
uint8_t rsi_ant_get_ver_resp[6] = {0};
uint8_t rsi_ant_get_stats[16] = {0};



/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_ant_app_init_events()
{
  ble_ant_app_event_map = 0;
 //! ble_ant_app_event_mask = 0xFFFFFFFF;
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
static void rsi_ble_ant_app_set_event(uint32_t event_num)
{
  ble_ant_app_event_map |= BIT(event_num);
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
static void rsi_ble_ant_app_clear_event(uint32_t event_num)
{
  ble_ant_app_event_map &= ~BIT(event_num);
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
static int32_t rsi_ble_ant_app_get_event(void)
{
  uint32_t  ix;

  for (ix = 0; ix < 32; ix++)
  {
    if (ble_ant_app_event_map & (1 << ix))
    {
      return ix;
    }
  }

  return (-1);
}

/*==============================================*/
/**
 * @fn         rsi_ant_async_resp_handler
 * @brief      invoked when asyncronous ant stack packets are received.
 * @return     none.
 * @section description
 */
void rsi_ant_async_resp_handler(uint8_t *payload)
{
	uint16_t event_num;
	event_num = *(uint16_t *)&payload[0];
	rsi_ant_schedule_stats_t *rsi_ant_schedule_stats;
	rsi_ant_activity_stats_t *rsi_ant_activity_stats;

	switch(event_num)
	{
		case RSI_ANT_SCHED_STATS:
			rsi_ant_schedule_stats = (rsi_ant_schedule_stats_t *)payload;
			LOG_PRINT("activities_blocked : %d  activities_aborted : %d  schedules_blocked : %d, radio_ops_complete : %d. schedules_attempted : %d", 
					rsi_ant_schedule_stats->activities_blocked,
					rsi_ant_schedule_stats->activities_aborted,
					rsi_ant_schedule_stats->schedules_blocked,
					rsi_ant_schedule_stats->radio_ops_complete,
					rsi_ant_schedule_stats->schedules_attempted);
			break;
		case RSI_ANT_ACTIVITY_STATS:
			rsi_ant_activity_stats = (rsi_ant_activity_stats_t *)payload;
			LOG_PRINT("activity_status_ok : %d  activities_aborted : %d  activity_status_too_late : %d, activity_status_no_sync : %d, activity_status_incomplete : %d  activity_status_bad_crc  : %d, activity_status_vnd_error_start : %d",
					rsi_ant_activity_stats->activity_status_ok,
					rsi_ant_activity_stats->activities_aborted,
					rsi_ant_activity_stats->activity_status_too_late,
					rsi_ant_activity_stats->activity_status_no_sync,
					rsi_ant_activity_stats->activity_status_incomplete,
					rsi_ant_activity_stats->activity_status_bad_crc,
					rsi_ant_activity_stats->activity_status_vnd_error_start);
			break;
	}
}

uint32_t rsi_ble_ant_app(void) 
{
	int32_t status = 0;

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(rsi_ant_app_resp_get_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! set the local device name
  status = rsi_bt_set_local_name(RSI_BLE_LOCAL_NAME);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! get the local device name
  status = rsi_bt_get_local_name(&rsi_ant_app_resp_get_local_name);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //!  initializing the application events map
  rsi_ble_ant_app_init_events ();

	//! BLE register GAP callbacks
	rsi_ant_register_callbacks(rsi_ant_async_resp_handler);

	ant_get_version.cmd_ix = ANT_GET_VERSION;

	//! get the ANT Stack Local Version.
	LOG_PRINT("RSI GET ANT VERSION REQUEST CMD\n");
	status = rsi_ant_send_cmd(&ant_get_version, rsi_ant_get_ver_resp);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	atm_mode.cmd_ix = ANT_ATM_CMD;
	atm_mode.flags = 0x01;
	LOG_PRINT("RSI SEND ANT START ATM COMMAND \n");
	status = rsi_ant_send_cmd (&atm_mode);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	ant_get_stats.cmd_ix = ANT_GET_STATS;
	//! get the ANT Stack Local Version.
	LOG_PRINT("RSI ANT GET GET STATS COMMAND \n");
	status = rsi_ant_send_cmd(&ant_get_stats, rsi_ant_get_stats);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_task
 * @brief      this function will execute when BLE events are raised.
 * @param[in]  none.
 * @return     none.
 * @section description
 */
void rsi_ble_ant_app_task(void) 
{
	int32_t status = 0;
	int32_t  event_id;
	//! checking for events list
	event_id = rsi_ble_ant_app_get_event ();
	if (event_id == -1) {
		return;
	}
	switch (event_id) {
	case 0 : 
					break;
	}
		return;
}
#endif

