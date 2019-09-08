/**
 * @file    rsi_bt_connected_sniff_DEMO_26.c
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
 *  @brief : This file contains example application for BT Classic connected sniff
 *
 *  @section Description  This application serves as a BT Slave.
 *
 */

/**
 * Include files
 * */

#include <rsi_common_app.h>
#if BT_SNIFF
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
static uint8_t data[RSI_BT_MAX_PAYLOAD_SIZE];
static uint16_t data_len;
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
	memcpy ((int8_t *)remote_dev_addr, conn_event->dev_addr, RSI_DEV_ADDR_LEN);
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
void rsi_bt_app_on_pincode_req(uint16_t resp_status, rsi_bt_event_user_pincode_request_t *user_pincode_request)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_PINCODE_REQ);
	memcpy ((int8_t *)remote_dev_addr, user_pincode_request->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("on_pin_coe_req: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_pincode_request->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_on_linkkey_req
 * @brief      invoked when linkkey request event is received
 * @param[out] user_linkkey_req, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey request from remote device
 */
void rsi_ble_app_on_linkkey_req (uint16_t status, rsi_bt_event_user_linkkey_request_t  *user_linkkey_req)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_REQ);
	memcpy ((int8_t *)remote_dev_addr, user_linkkey_req->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("linkkey_req: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_req->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_on_linkkey_save
 * @brief      invoked when linkkey save event is received
 * @param[out] user_linkkey_req, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey save from local device
 */
void rsi_ble_app_on_linkkey_save (uint16_t status, rsi_bt_event_user_linkkey_save_t *user_linkkey_save)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_SAVE);
	memcpy ((int8_t *)remote_dev_addr, user_linkkey_save->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("linkkey_save: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_save->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_auth_complete
 * @brief      invoked when authentication event is received
 * @param[out] resp_status, authentication status
 * @param[out] auth_complete, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the pairing status and remote device information
 */
void rsi_bt_app_on_auth_complete (uint16_t resp_status, rsi_bt_event_auth_complete_t *auth_complete)
{
  if(resp_status == 0)
  {
    rsi_bt_app_set_event (RSI_APP_EVENT_AUTH_COMPLT);
  }
	memcpy ((int8_t *)remote_dev_addr, auth_complete->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("auth_complete: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, auth_complete->dev_addr));
}

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
	memcpy ((int8_t *)remote_dev_addr, bt_disconnected->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("on_disconn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, bt_disconnected->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_spp_connect
 * @brief      invoked when spp profile connected event is received
 * @param[out] spp_connect, spp connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the spp connected remote device information
 */
void rsi_bt_app_on_spp_connect (uint16_t resp_status, rsi_bt_event_spp_connect_t *spp_connect)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_CONN);
	memcpy ((int8_t *)remote_dev_addr, spp_connect->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("spp_conn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, spp_connect->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_spp_disconnect
 * @brief      invoked when spp profile disconnected event is received
 * @param[out] spp_disconn, spp disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the spp disconnected event
 */
void rsi_bt_app_on_spp_disconnect (uint16_t resp_status, rsi_bt_event_spp_disconnect_t *spp_disconn)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_DISCONN);
	memcpy ((int8_t *)remote_dev_addr, spp_disconn->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("spp_disconn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, spp_disconn->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_passkey_display
 * @brief      invoked when passkey display event is received
 * @param[out] resp_status, event status
 * @param[out] bt_event_user_passkey_display, passkey information
 * @return     none.
 * @section description
 * This callback function indicates the passkey display event
 */

void rsi_bt_on_passkey_display (uint16_t resp_status, rsi_bt_event_user_passkey_display_t *bt_event_user_passkey_display)
{
	uint8_t ix;
	rsi_bt_app_set_event (RSI_APP_EVENT_PASSKEY_DISPLAY);
  //rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, bt_event_user_passkey_display->passkey);
  memcpy (data,bt_event_user_passkey_display->passkey ,4);
  for (ix = 0; ix < 4; ix++)
  {
    LOG_PRINT(" 0x%02x,", bt_event_user_passkey_display->passkey[ix]);
  }

  LOG_PRINT ("\r\n");
  LOG_PRINT ("passkey: %d", *((uint32_t *)bt_event_user_passkey_display->passkey));
  LOG_PRINT ("\r\n");
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_passkey_request
 * @brief      invoked when passkey request event is received
 * @param[out] resp_status, event status
 * @param[out] user_passkey_request, passkey information
 * @return     none.
 * @section description
 * This callback function indicates the passkey request event
 */
void rsi_bt_on_passkey_request (uint16_t resp_status, rsi_bt_event_user_passkey_request_t *user_passkey_request)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_PASSKEY_REQUEST);
	memcpy ((int8_t *)remote_dev_addr, user_passkey_request->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("passkey_request: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_passkey_request->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_ssp_complete
 * @brief      invoked when ssp complete event is received
 * @param[out] resp_status, event status
 * @param[out] ssp_complete, secure simple pairing information
 * @return     none.
 * @section description
 * This callback function indicates the spp status
 */
void rsi_bt_on_ssp_complete (uint16_t resp_status, rsi_bt_event_ssp_complete_t *ssp_complete)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_SSP_COMPLETE);
	memcpy ((int8_t *)remote_dev_addr, ssp_complete->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("ssp_complete: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, ssp_complete->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_confirm_request
 * @brief      invoked when ssp confirm request event is received
 * @param[out] resp_status, event status
 * @param[out] ssp_complete, secure simple pairing requested remote device information
 * @return     none.
 * @section description
 * This callback function indicates the ssp confirm event
 */
void rsi_bt_on_confirm_request (uint16_t resp_status, rsi_bt_event_user_confirmation_request_t *user_confirmation_request)
{
	uint8_t ix;
	rsi_bt_app_set_event (RSI_APP_EVENT_CONFIRM_REQUEST);

  memcpy (data,user_confirmation_request->confirmation_value ,4);
  for (ix = 0; ix < 4; ix++)
  {
    LOG_PRINT (" 0x%02x,", user_confirmation_request->confirmation_value[ix]);
  }
  LOG_PRINT ("\r\n");
  LOG_PRINT ("data: %s",user_confirmation_request->confirmation_value );
}



/*==============================================*/
/**
 * @fn         rsi_bt_on_mode_change
 * @brief      invoked when mode chande event is received
 * @param[out] mode change,mode change request to remote device
 * @return     none.
 * @section description
 * This callback function indicates the mode change event
 */
void rsi_bt_on_mode_change (uint16_t resp_status, rsi_bt_event_mode_change_t  *mode_change)
{
  memcpy ((int8_t *)remote_dev_addr, mode_change->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("mode_change_event: str_conn_bd_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, mode_change->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_MODE_CHANGED);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_sniff_subrating
 * @brief      invoked when sniff subrating event is received
 * @param[out] sniff subrating,shiff subrating request to remote device
 * @return     none.
 * @section description
 * This callback function indicates the shiff subrating event
 */
void rsi_bt_on_sniff_subrating (uint16_t resp_status,rsi_bt_event_sniff_subrating_t  *mode_change)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_SNIFF_SUBRATING);
  memcpy ((int8_t *)remote_dev_addr, mode_change->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("mode_change_event: str_conn_bd_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, mode_change->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_spp_data_rx
 * @brief      invoked when spp data rx event is received
 * @param[out] spp_receive, spp data from remote device
 * @return     none.
 * @section description
 * This callback function indicates the spp data received event
 */
void rsi_bt_app_on_spp_data_rx (uint16_t resp_status, rsi_bt_event_spp_receive_t *spp_receive)
{
  uint16_t ix;

	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_RX);
  data_len = spp_receive->data_len;
  memcpy (data, spp_receive->data, spp_receive->data_len);
  LOG_PRINT ("spp_rx: data_len: %d, data: ", spp_receive->data_len);
  for (ix = 0; ix < spp_receive->data_len; ix++)
  {
    LOG_PRINT (" 0x%02x,", spp_receive->data[ix]);
  }
  LOG_PRINT ("\r\n");
  LOG_PRINT ("data: %s", spp_receive->data);
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
int32_t rsi_app_task_bt_connected_sniff(void)
{
  int32_t status = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[18] = {0};
  //uint8_t eir_data[200] = {2,1,0};

  //! BT register GAP callbacks:
  rsi_bt_gap_register_callbacks(
      NULL, //role_change
      rsi_bt_app_on_conn,
      NULL, //
      rsi_bt_app_on_disconn,
      NULL,//scan_resp
      NULL,//remote_name_req
      rsi_bt_on_passkey_display,//passkey_display
      NULL,//remote_name_req+cancel
      rsi_bt_on_confirm_request,//confirm req
      NULL,//rsi_bt_app_on_pincode_req,
      rsi_bt_on_passkey_request,//passkey request
      NULL,//inquiry complete
      rsi_bt_app_on_auth_complete,
      rsi_ble_app_on_linkkey_req,//linkkey request
     	rsi_bt_on_ssp_complete,//ssp coplete
      rsi_ble_app_on_linkkey_save,
      NULL, //get services
      NULL,
	  rsi_bt_on_mode_change,
	  rsi_bt_on_sniff_subrating); //search service

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

#if 0
  //! prepare Extended Response Data
  eir_data[3] = strlen (RSI_BT_LOCAL_NAME) + 1;
  eir_data[4] = 9;
  strcpy ((char *)&eir_data[5], RSI_BT_LOCAL_NAME);
  //! set eir data
  rsi_bt_set_eir_data (eir_data, strlen (RSI_BT_LOCAL_NAME) + 5);
  //! start the discover mode
  status = rsi_bt_start_discoverable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

//Adding Device Identification
  status = rsi_bt_add_device_id(RSI_DID_SPEC_ID,
                                RSI_DID_VENDOR_ID,
                                RSI_DID_PRODUCT_ID,
                                RSI_DID_VERSION,
                                RSI_DID_PRIMARY_RECOED,
                                RSI_DID_VENDOR_ID_SOURCE
                                );
  if(status != RSI_SUCCESS){
    return status;
  }

  //! start the connectability mode
  status = rsi_bt_set_connectable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! start the ssp mode
  status = rsi_bt_set_ssp_mode(1,1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! initilize the SPP profile
  status = rsi_bt_spp_init ();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! register the SPP profile callback's
  rsi_bt_spp_register_callbacks (rsi_bt_app_on_spp_connect,
                                 rsi_bt_app_on_spp_disconnect,
                                 rsi_bt_app_on_spp_data_rx);

  status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr,REMOTE_BD_ADDR));
  if(status != RSI_SUCCESS)
  {
      return status;
  }

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
      case RSI_APP_EVENT_CONNECTED:
        {
          //! remote device connected event

          //! clear the connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_CONNECTED);
        }
        break;

      case RSI_APP_EVENT_PINCODE_REQ:
        {
          //! pincode request event
          uint8_t *pin_code = (uint8_t *)PIN_CODE;

          //! clear the pincode request event.
          rsi_bt_app_clear_event(RSI_APP_EVENT_PINCODE_REQ);

          //! sending the pincode requet reply
          status = rsi_bt_pincode_request_reply((int8_t *)remote_dev_addr, pin_code, 1);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
        }
        break;
      case RSI_APP_EVENT_LINKKEY_SAVE:
        {
          //! linkkey save event

          //! clear the likkey save event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_SAVE);
        }
        break;
      case RSI_APP_EVENT_AUTH_COMPLT:
        {
          //! authentication complete event

          //! clear the authentication complete event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AUTH_COMPLT);
        }
        break;
      case RSI_APP_EVENT_DISCONNECTED:
        {
          //! remote device connected event

          //! clear the disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_DISCONNECTED);
        }
        break;
      case RSI_APP_EVENT_LINKKEY_REQ:
        {
          //! linkkey request event

          //! clear the linkkey request event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_REQ);

          LOG_PRINT ("linkkey_req: %s\r\n", str_conn_bd_addr);
          //! sending the linkkey request negative reply
          rsi_bt_linkkey_request_reply ((int8_t *)remote_dev_addr, NULL, 0);
        }
        break;

      case RSI_APP_EVENT_SPP_CONN:
        {
          //! spp connected event

          //! clear the spp connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SPP_CONN);

          LOG_PRINT("\n initiating sniff mode command here \n");

          /* here we call the sniff_mode command*/
		  rsi_bt_sniff_mode(remote_dev_addr, SNIFF_INTV_MAX, SNIFF_INTV_MIN, SNIFF_ATTEPMT, SNIFF_TIMEOUT);


		  if (!power_save_given)
		  {
			status = rsi_bt_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
			if (status != RSI_SUCCESS)
			{
				return status;
			}
			power_save_given = 1;
		  }

        }
        break;

      case RSI_APP_EVENT_SPP_DISCONN:
        {
          //! spp disconnected event

          //! clear the spp disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SPP_DISCONN);
        }
        break;

      case RSI_APP_EVENT_SPP_RX:
        {
          //! spp receive event

          //! clear the spp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SPP_RX);

          LOG_PRINT("\nintiaiting sniff_stop command\n");
          rsi_bt_sniff_exit_mode(remote_dev_addr);


          //! SPP data transfer (loop back)
          rsi_bt_spp_transfer (remote_dev_addr, data, data_len);


          LOG_PRINT("\n initiating sniff mode command here \n");

          /* here we call the sniff_mode command*/
		  rsi_bt_sniff_mode(remote_dev_addr,0x32,0x32,0x04,0x02);
        }
        break;

      case RSI_APP_EVENT_PASSKEY_DISPLAY:
        {
          //! clear the passkey display event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_PASSKEY_DISPLAY);
        }
        break;

      case RSI_APP_EVENT_PASSKEY_REQUEST:
        {
          //! clear the passkey request event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_PASSKEY_REQUEST);
        }
        break;

      case RSI_APP_EVENT_SSP_COMPLETE:
        {
          //! clear the ssp complete event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SSP_COMPLETE);
					LOG_PRINT(" SSP conection completed\n");
        }
        break;

      case RSI_APP_EVENT_CONFIRM_REQUEST:
        {
          //! clear the ssp confirmation event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_CONFIRM_REQUEST);
					LOG_PRINT("Confirmation is requested\n");
					rsi_bt_accept_ssp_confirm((int8_t *)remote_dev_addr);
        }
        break;
      case RSI_APP_EVENT_MODE_CHANGED:
        {
          //! clear mode change event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_MODE_CHANGED);
					LOG_PRINT("MODE_CHANGE IS COMPLETED\n");
        }
        break;

      case RSI_APP_EVENT_SNIFF_SUBRATING:
        {
          //! clear the sniff subrating event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SNIFF_SUBRATING);
					LOG_PRINT("SNIFF SUBRATING IS COMPLETED\n");
        }
        break;
    }
  }

  return 0;
}
#endif
