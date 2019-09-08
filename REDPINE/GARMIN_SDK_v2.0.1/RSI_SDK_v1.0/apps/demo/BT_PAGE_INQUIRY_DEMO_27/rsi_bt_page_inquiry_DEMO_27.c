/**
 * @file    rsi_bt_page_inquiry_DEMO_27.c
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
 *  @brief : This file contains example application for BT Classic Page Inquiry
 *
 *  @section Description  This application serves as a BT Master.
 *
 */

/**
 * Include files
 * */

#include <rsi_common_app.h>
#if BT_PAGE_INQUIRY

#include<rsi_driver.h>
//! BT include file to refer BT APIs
#include<rsi_bt_apis.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_bt_config.h>
#include<rsi_bt.h>
#include <stdio.h>

#define RSI_DID_SPEC_ID                0x0200
#define RSI_DID_VENDOR_ID              0x0201
#define RSI_DID_PRODUCT_ID             0x0202
#define RSI_DID_VERSION                0x0203
#define RSI_DID_PRIMARY_RECOED         0x0001
#define RSI_DID_VENDOR_ID_SOURCE       0x0002

//! Application global parameters.
static uint32_t rsi_app_async_event_map = 0;
static rsi_bt_resp_get_local_name_t   local_name = {0};
static uint8_t str_conn_bd_addr[18];
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t remote_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t rsi_app_resp_get_bond_dev_addr[18] = {0};
static rsi_bt_event_inquiry_response_t rsi_bt_inquiry_response = {0};
uint8_t power_save_given = 0;

extern int32_t rsi_bt_power_save_profile(uint8_t psp_mode, uint8_t psp_type);

/*==============================================*/
/**
 * @fn         rsi_bt_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BT initialization.
 */
static void rsi_bt_app_init_events()
{
	rsi_app_async_event_map = 0;
	return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_set_event
 * @brief      sets the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
static void rsi_bt_app_set_event(uint32_t event_num)
{
	rsi_app_async_event_map |= BIT(event_num);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_bt_app_clear_event(uint32_t event_num)
{
	rsi_app_async_event_map &= ~BIT(event_num);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
static int32_t rsi_bt_app_get_event(void)
{
	uint32_t  ix;

	for (ix = 0; ix < 32; ix++)
	{
		if (rsi_app_async_event_map & (1 << ix))
		{
		   return ix;
		}
	}

	return (RSI_FAILURE);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_conn
 * @brief      invoked when connection complete event is received
 * @param[out] resp_status, connection status of the connected device.
 * @param[out] conn_event, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_bt_app_on_conn (uint16_t resp_status, rsi_bt_event_bond_t *conn_event)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
	//memcpy ((int8_t *)remote_dev_addr, conn_event->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("on_conn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, conn_event->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_pincode_req
 * @brief      invoked when pincode request event is received
 * @param[out] user_pincode_request, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the pincode request from remote device
 */

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_disconn
 * @brief      invoked when disconnect event is received
 * @param[out] resp_status, disconnect status/error
 * @param[out] bt_disconnected, disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the disconnected device information
 */
void rsi_bt_app_on_disconn (uint16_t resp_status, rsi_bt_event_disconnect_t *bt_disconnected)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
	//memcpy ((int8_t *)remote_dev_addr, bt_disconnected->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("on_disconn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, bt_disconnected->dev_addr));
}


/*==============================================*/
/**
 * @fn         rsi_bt_app_on_inq_complete_event
 * @brief      invoked when inquiry complete event is received
 * @return     none.
 * @section description
 * This callback function indicates inquiry complete.
 */
void rsi_bt_app_on_inq_complete_event(uint16_t resp_status)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_INQ_COMPLT);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_inq_resp_event
 * @brief      invoked when inquiry response is received
 * @param[out] inq_resp, inquiry response information
 * @return     none.
 * @section description
 * This callback function is called when response to do inquiry command is recieved.
 */
void rsi_bt_app_on_inq_resp_event(uint16_t resp_status, rsi_bt_event_inquiry_response_t *inq_resp)
{
	memcpy(&rsi_bt_inquiry_response, inq_resp, sizeof(rsi_bt_event_inquiry_response_t));
	rsi_bt_app_set_event (RSI_APP_EVENT_INQ_RESP);
}

/*==============================================*/
/**
 * @fn         rsi_bt_spp_slave
 * @brief      Tests the BT Classic SPP Slave role with connected Sniff.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the SPP Slave role with connected Sniff.
 */
int32_t rsi_app_task_bt_page_inquiry (void)
{
  int32_t status = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[18] = {0};
  uint8_t eir_data[200] = {2,1,0};
  uint8_t i = 0;

  //! BT register GAP callbacks:
  rsi_bt_gap_register_callbacks(
      NULL, //role_change
      rsi_bt_app_on_conn,
      NULL, //
      rsi_bt_app_on_disconn,
	  rsi_bt_app_on_inq_resp_event,
      NULL,//remote_name_req
      NULL,////passkey_display
      NULL,//remote_name_req+cancel
      NULL,//confirm req
      NULL,//rsi_bt_app_on_pincode_req,
      NULL,//passkey request
	  rsi_bt_app_on_inq_complete_event,
      NULL,//rsi_bt_app_on_auth_complete,
      NULL,//rsi_ble_app_on_linkkey_req,//linkkey request
	  NULL,//rsi_bt_on_ssp_complete,//ssp coplete
      NULL,//rsi_ble_app_on_linkkey_save,
      NULL, //get services
      NULL,
	  NULL,//rsi_bt_on_mode_change,
	  NULL); //search service

  //! initialize the event map
  rsi_bt_app_init_events ();

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(local_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\r\nlocal_bd_address: %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_bd_addr, local_dev_addr));

  //! set the local device name
  status = rsi_bt_set_local_name((int8_t *)RSI_BT_LOCAL_NAME);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! get the local device name
  status = rsi_bt_get_local_name(&local_name);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\r\nlocal_name: %s\r\n", local_name.name);

  //! prepare Extended Response Data
  eir_data[3] = strlen (RSI_BT_LOCAL_NAME) + 1;
  eir_data[4] = 9;
  strcpy ((char *)&eir_data[5], RSI_BT_LOCAL_NAME);
  //! set eir data
  rsi_bt_set_eir_data (eir_data, strlen (RSI_BT_LOCAL_NAME) + 5);


	status = rsi_bt_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
	if (status != RSI_SUCCESS)
	{
		return status;
	}

#if BT_PAGE_ENABLE
	status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr,RSI_BT_REMOTE_BD_ADDR));
	if(status != RSI_SUCCESS)
	{
	         //return status;
	}
#else
	status = rsi_bt_inquiry(0, INQUIRY_DURATION, MAX_DEVICES);
	if(status != RSI_SUCCESS)
	{
	//   goto do_inquiry;
	}
#endif




while(1)
  {
    //! Application main loop

#ifndef RSI_WITH_OS
    //! run the non OS scheduler
    rsi_wireless_driver_task();
#endif
    //! checking for received events
    temp_event_map = rsi_bt_app_get_event ();
    if (temp_event_map == RSI_FAILURE) {
      //! if events are not received loop will be continued.
      continue;
    }

    //! if any event is received, it will be served.
    switch(temp_event_map)
    {
    case RSI_APP_EVENT_INQ_RESP:
	   {
		LOG_PRINT (" \n *********************Inquiry_resp_event*****************");
		 rsi_bt_app_clear_event (RSI_APP_EVENT_INQ_RESP);
		 LOG_PRINT ("\n inquiry_type = %d \n",rsi_bt_inquiry_response.inquiry_type );
		 LOG_PRINT (" remote_device_addr : ");
		 for(i = 0; i< 6; i++)
		 {
		  LOG_PRINT (":0x%x",rsi_bt_inquiry_response.dev_addr[i]);
		 }

		 //! converts the 6 byte MAC address to string format
		 //rsi_6byte_dev_address_to_ascii ((int8_t *)rsi_app_resp_get_bond_dev_addr, rsi_bt_inquiry_response.dev_addr);
	   }
	   break;

    case RSI_APP_EVENT_INQ_COMPLT:
      {
      	rsi_bt_app_clear_event (RSI_APP_EVENT_INQ_COMPLT);

      	status = rsi_bt_inquiry(0, INQUIRY_DURATION, MAX_DEVICES);
        //status = rsi_bt_connect((int8_t *)REMOTE_BD_ADDR);
        if(status != RSI_SUCCESS)
        {
          //return status;
        }
      }
      case RSI_APP_EVENT_CONNECTED:
        {
          //! remote device connected event

          //! clear the connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_CONNECTED);
        }
        break;

          case RSI_APP_EVENT_DISCONNECTED:
        {
          //! remote device connected event

          //! clear the disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_DISCONNECTED);
        }
        break;
    }
  }

  return 0;
}
#endif
