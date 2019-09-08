/**
 * @file    rsi_bt_hid_master.c
 * @version 0.1
 * @date    15 April 2019
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BT Classic hid Slave Role
 *
 *  @section Description  This application serves as a BT hid Slave.
 *
 */

/**
 * Include files
 * */

//! BT include file to refer BT APIs
#include<rsi_bt_apis.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_bt_config.h>
#include<rsi_bt.h>
#include <stdio.h>

//!Common include file 
#include<rsi_common_apis.h>

//! application defines 
#if 1//def RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT
#endif
#define RSI_BT_LOCAL_NAME             "REDPINE_HID_KEYBOARD"
#define PIN_CODE                      "0000"
#define REMOTE_BD_ADDR                "00:1B:DC:F2:1C:53" //"E4:92:FB:F7:28:BA"

//! application events list
#define RSI_APP_EVENT_CONNECTED       1
#define RSI_APP_EVENT_PINCODE_REQ     2
#define RSI_APP_EVENT_LINKKEY_SAVE    3
#define RSI_APP_EVENT_AUTH_COMPLT     4
#define RSI_APP_EVENT_DISCONNECTED    5
#define RSI_APP_EVENT_LINKKEY_REQ     6
#define RSI_APP_EVENT_HID_CONN        7
#define RSI_APP_EVENT_HID_DISCONN     8
#define RSI_APP_EVENT_HID_RX          9
#define RSI_APP_EVENT_HID_TX          10
#define RSI_APP_EVENT_HID_MSG_CONTROL           11
#define RSI_APP_EVENT_HID_MSG_GET_REPORT        12
#define RSI_APP_EVENT_HID_MSG_SET_REPORT        13
#define RSI_APP_EVENT_HID_MSG_GET_PROTOCOL      14
#define RSI_APP_EVENT_HID_MSG_SET_PROTOCOL      15




#define KBD_EMULATOR 1

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN            10000

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE 1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY   1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  3000


int32_t rsi_bt_hid_service_initialize(char *service_name, char *service_description, 
                                      char *service_provider, uint8_t *desc_buf, uint16_t desc_buf_len);



void rsi_wireless_driver_task(void);
#endif


#if (KBD_EMULATOR == 1)
/*Example: Value from Descriptor to be stored in SDP >> 160 Bytes << */
uint8_t hid_desc_160_data[] =   {
0x35,0x9e,0x35,0x9c,0x08,0x22,0x25,0x98, 
0x05,0x01,0x09,0x06,0xa1,0x01,0x85,0x01,
0x75,0x01,0x95,0x08,0x05,0x07,0x19,0xe0,
0x29,0xe7,0x15,0x00,0x25,0x01,0x81,0x02,
0x95,0x01,0x75,0x08,0x81,0x03,0x95,0x05,
0x75,0x01,0x05,0x08,0x19,0x01,0x29,0x05,
0x91,0x02,0x95,0x01,0x75,0x03,0x91,0x03,
0x95,0x06,0x75,0x08,0x15,0x00,0x26,0xff,
0x00,0x05,0x07,0x19,0x00,0x29,0xff,0x81,
0x00,0xc0,0x05,0x0c,0x09,0x01,0xa1,0x01,
0x85,0x03,0x15,0x00,0x25,0x01,0x75,0x01,
0x95,0x0d,0x0a,0x23,0x02,0x0a,0x21,0x02,
0x0a,0x83,0x01,0x09,0xb6,0x09,0xb5,0x09,
0xe9,0x09,0xea,0x09,0xcd,0x09,0xb7,0x09,
0xe2,0x0a,0x8a,0x01,0x09,0x30,0x09,0xb8,
0x81,0x02,0x95,0x01,0x75,0x0b,0x81,0x03,
0xc0,0x05,0x01,0x09,0x80,0xa1,0x01,0x85,
0x04,0x15,0x00,0x25,0x01,0x75,0x01,0x95,
0x03,0x09,0x81,0x09,0x82,0x09,0x83,0x81,
0x02,0x95,0x01,0x75,0x05,0x81,0x03,0xc0};
#endif

#if (KBD_EMULATOR == 1) /* for testing KBD */
uint8_t hid_int_data_kb_down[] = {0x01 ,0x00 ,0x00 ,0x51 ,0x00 ,0x00 ,0x00 ,0x00, 0x00};/* KBD Down arrow */
uint8_t hid_int_data_kb_reset[] = {0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, 0x00};/* KBD reset arrow */
uint8_t hid_int_data_kb_b_key[] = {0x01 ,0x00 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00, 0x00};/* KBD b key press */
#endif
//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN] = {0};


rsi_bt_event_hid_receive_t hid_rx_data;


//! Application global parameters.
static uint32_t rsi_app_async_event_map = 0;
static rsi_bt_resp_get_local_name_t   local_name = {0};
static uint8_t str_conn_bd_addr[18];
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t remote_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t data[RSI_BT_MAX_PAYLOAD_SIZE];
static uint16_t data_len;
static uint8_t  linkkey[16];

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
  LOG_PRINT ("on conn: str_conn_bd_addr: %s\r\n",
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
  LOG_PRINT ("on pincode_req: str_conn_bd_addr: %s\r\n",
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
	memcpy (remote_dev_addr, user_linkkey_req->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("linkkey_req: str_conn_bd_addr: %s\r\n",
	  rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, user_linkkey_req->dev_addr));
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
	    rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, user_linkkey_save->dev_addr));
  memcpy (linkkey, user_linkkey_save->linkKey, 16);

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
  } else {
    memset (linkkey, 0, 16);
  }
  memcpy ((int8_t *)remote_dev_addr, auth_complete->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("auth_complete: str_conn_bd_addr: %s\r\n",
	  rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, auth_complete->dev_addr));
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
	  rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, bt_disconnected->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_hid_connect
 * @brief      invoked when hid profile connected event is received
 * @param[out] hid_connect, hid connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the hid connected remote device information
 */
void rsi_bt_app_on_hid_connect (uint16_t resp_status, rsi_bt_event_hid_connect_t *hid_connect)
{
  if(hid_connect->conn_status == BT_HID_CONNECTED) 
  {
    rsi_bt_app_set_event (RSI_APP_EVENT_HID_CONN);
  }else
  {
    rsi_bt_app_set_event (RSI_APP_EVENT_HID_DISCONN);
  }
  memcpy ((int8_t *)remote_dev_addr, hid_connect->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("hid_conn: str_conn_bd_addr: %s\r\n",
    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, hid_connect->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_hid_disconnect
 * @brief      invoked when hid profile disconnected event is received
 * @param[out] hid_disconn, hid disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the hid disconnected event
 */
void rsi_bt_app_on_hid_disconnect (uint16_t resp_status, rsi_bt_event_hid_disconnect_t *hid_disconn)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_HID_DISCONN);
  //memcpy ((int8_t *)remote_dev_addr, hid_disconn->dev_addr, RSI_DEV_ADDR_LEN);
  //LOG_PRINT ("hid_disconn: str_conn_bd_addr: %s\r\n",
    //rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, hid_disconn->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_hid_data_rx
 * @brief      invoked when hid data rx event is received
 * @param[out] hid_receive, hid data from remote device
 * @return     none.
 * @section description
 * This callback function indicates the hid data received event
 */
void rsi_bt_app_on_hid_data_rx (uint16_t resp_status, rsi_bt_event_hid_receive_t *hid_receive)
{
  uint16_t ix;

	rsi_bt_app_set_event (RSI_APP_EVENT_HID_RX);
  *(uint16_t*)&hid_rx_data.data_len = *(uint16_t*)&hid_receive->data_len;
 memcpy (&hid_rx_data, hid_receive, (sizeof(rsi_bt_event_hid_receive_t)- RSI_BT_MAX_PAYLOAD_SIZE + *(uint16_t*)&hid_receive->data_len));
}


void rsi_bt_app_on_hid_control_msg (uint16_t resp_status, rsi_bt_event_hid_receive_t *hid_receive)
{
  memcpy (&hid_rx_data, hid_receive, (sizeof(rsi_bt_event_hid_receive_t)- RSI_BT_MAX_PAYLOAD_SIZE + hid_receive->data_len));
  rsi_bt_app_set_event (RSI_APP_EVENT_HID_MSG_CONTROL);
}
void rsi_bt_app_on_hid_get_report_msg (uint16_t resp_status, rsi_bt_event_hid_receive_t *hid_receive)
{
  memcpy (&hid_rx_data, hid_receive, (sizeof(rsi_bt_event_hid_receive_t)- RSI_BT_MAX_PAYLOAD_SIZE + hid_receive->data_len));
  rsi_bt_app_set_event (RSI_APP_EVENT_HID_MSG_GET_REPORT);
}
void rsi_bt_app_on_hid_set_report_msg (uint16_t resp_status, rsi_bt_event_hid_receive_t *hid_receive)
{
  memcpy (&hid_rx_data, hid_receive, (sizeof(rsi_bt_event_hid_receive_t)- RSI_BT_MAX_PAYLOAD_SIZE + hid_receive->data_len));
  rsi_bt_app_set_event (RSI_APP_EVENT_HID_MSG_SET_REPORT);
}
void rsi_bt_app_on_hid_get_protocol_msg (uint16_t resp_status, rsi_bt_event_hid_receive_t *hid_receive)
{
  memcpy (&hid_rx_data, hid_receive, (sizeof(rsi_bt_event_hid_receive_t)- RSI_BT_MAX_PAYLOAD_SIZE + hid_receive->data_len));
  rsi_bt_app_set_event (RSI_APP_EVENT_HID_MSG_GET_PROTOCOL);
}
void rsi_bt_app_on_hid_set_protocol_msg (uint16_t resp_status, rsi_bt_event_hid_receive_t *hid_receive)
{
  memcpy (&hid_rx_data, hid_receive, (sizeof(rsi_bt_event_hid_receive_t)- RSI_BT_MAX_PAYLOAD_SIZE + hid_receive->data_len));
  rsi_bt_app_set_event (RSI_APP_EVENT_HID_MSG_SET_PROTOCOL);
}






/*==============================================*/
/**
 * @fn         rsi_bt_hid_master
 * @brief      Tests the BT Classic hid Slave role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the hid Slave role.
 */
int32_t rsi_bt_hid_master (void)
{
  int32_t status = 0;
  int32_t tx_ix = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[18] = {0};
  uint8_t hid_tx_data_len = 0;
  uint32_t class_of_device;
  uint8_t action=0;
  uint8_t msg_count=0;
  
  uint32_t timer_1=0;
  uint8_t send_periodic_interrupt=0;

#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! Redpine module intialisation 
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BT_CLASSIC);

  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! BT register GAP callbacks:
  rsi_bt_gap_register_callbacks(
      NULL, //role_change
      rsi_bt_app_on_conn, 
      NULL, //
      rsi_bt_app_on_disconn,
      NULL,//scan_resp
      NULL,//remote_name_req
      NULL,//passkey_display
      NULL,//remote_name_req+cancel
      NULL,//confirm req
      rsi_bt_app_on_pincode_req,
      NULL,//passkey request
      NULL,//inquiry complete
      rsi_bt_app_on_auth_complete,
      rsi_ble_app_on_linkkey_req,//linkkey request
      NULL,//ssp coplete
      rsi_ble_app_on_linkkey_save,
      NULL, //get services
      NULL,
      NULL,
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
  status = rsi_bt_set_local_name(RSI_BT_LOCAL_NAME);
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
 
  class_of_device = 0x002580;   //! HID

  status = rsi_bt_set_local_class_of_device(class_of_device);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\n class_of_device = 0x%x", class_of_device);

  
  status = rsi_bt_hid_service_initialize("Keyboard2", "BT Widget", "REDPINE", hid_desc_160_data, 160);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! initilize the hid profile
  status = rsi_bt_hid_init ();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_spp_init();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

//  status = rsi_bt_a2dp_init();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
 // status = rsi_bt_avrcp_init();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  //! start the discover mode
  status = rsi_bt_start_discoverable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! start the connectability mode
  status = rsi_bt_set_connectable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! register the hid profile callback's
  rsi_bt_hid_register_callbacks (rsi_bt_app_on_hid_connect,
                                 rsi_bt_app_on_hid_data_rx,
                                 NULL,
                                 rsi_bt_app_on_hid_control_msg,
                                 rsi_bt_app_on_hid_get_report_msg,
                                 rsi_bt_app_on_hid_set_report_msg,
                                 rsi_bt_app_on_hid_get_protocol_msg,
                                 rsi_bt_app_on_hid_set_protocol_msg
                                 );


  while(1)
  {
    //! Application main loop

#ifndef RSI_WITH_OS
    //! run the non OS scheduler
    rsi_wireless_driver_task();
#endif
    //! checking for received events;
    temp_event_map = rsi_bt_app_get_event ();
    if (temp_event_map == RSI_FAILURE) {
      //! if events are not received loop will be continued.
      
#if (KBD_EMULATOR == 1) /* for testing KBD */
      if(send_periodic_interrupt == 1)
      {
        /* 20000000  ->  2 sec  */
        if(timer_1++ == (20000000   * 5))
        {
          timer_1 = 0;
          rsi_bt_app_set_event (RSI_APP_EVENT_HID_TX);
        }
      }
#endif

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
          uint8_t *pin_code = PIN_CODE;

          //! clear the pincode request event.
          rsi_bt_app_clear_event(RSI_APP_EVENT_PINCODE_REQ);

          //! sending the pincode requet reply
          status = rsi_bt_pincode_request_reply((int8_t *)remote_dev_addr, pin_code, 1);
          if(status != RSI_SUCCESS)
          {
            //return status;
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

          status = rsi_bt_hid_connect(remote_dev_addr);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
        }
        break;
      case RSI_APP_EVENT_DISCONNECTED:
        {
          //! remote device connected event

#if (KBD_EMULATOR == 1) /* for testing KBD */
          send_periodic_interrupt = 0;
#endif
          //! clear the disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_DISCONNECTED);
        }
        break;
      case RSI_APP_EVENT_LINKKEY_REQ:
        {
          //! linkkey request event

          //! clear the linkkey request event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_REQ);

          if (linkkey[0] == 0) {
          //! sending the linkkey request negative reply
          rsi_bt_linkkey_request_reply ((int8_t *)remote_dev_addr, NULL, 0);
          } else {
            rsi_bt_linkkey_request_reply ((int8_t *)remote_dev_addr, linkkey, 1);
          }
        }
        break;

      case RSI_APP_EVENT_HID_CONN:
        {
          //! hid connected event

          //! clear the hid connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_HID_CONN);
          LOG_PRINT("\nHID CONNECTION EVENT \n");
#if (KBD_EMULATOR == 1) /* for testing KBD */
          send_periodic_interrupt = 1;
#endif
        }
        break;

      case RSI_APP_EVENT_HID_DISCONN:
        {
          //! hid disconnected event
#if (KBD_EMULATOR == 1) /* for testing KBD */
          send_periodic_interrupt = 0;
#endif

          LOG_PRINT("\nHID DIS-CONNECTED EVENT \n");
          //! clear the hid disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_HID_DISCONN);
        }
        break;

      case RSI_APP_EVENT_HID_RX:
        {
          //! hid receive event

          rsi_bt_app_clear_event (RSI_APP_EVENT_HID_RX);
          //! clear the hid receive event.
          uint16_t ix;
          uint16_t len  = *(uint16_t*)&hid_rx_data.data_len;
          LOG_PRINT ("\r\nhid_rx: HID Report Data Received");
          LOG_PRINT ("hid_rx: Channel: >%d<, \n", hid_rx_data.hid_channel);

          LOG_PRINT ("hid_rx: data_len: %d, data: ",len );
          for (ix = 0; ix < len; ix++)
          {
            LOG_PRINT (" 0x%02x,", hid_rx_data.data[ix]);
          }
          LOG_PRINT ("\r\n");
        }
        break;
     case RSI_APP_EVENT_HID_TX:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_HID_TX);
 
          uint8_t hid_int_data_len = 9; 
          switch(action)
          {
            case 0:
              {
                printf("\nTransmitting Keyboard Down arrow\n");
                rsi_bt_hid_send_interrupt_data(remote_dev_addr, hid_int_data_kb_down, hid_int_data_len);
                break;
              }
            case 1:
              {
                printf("\nTransmitting Keyboard ACtion Key up\n");
                rsi_bt_hid_send_interrupt_data(remote_dev_addr, hid_int_data_kb_reset, hid_int_data_len);
                break;
              }
            case 2:
              {
                printf("\nTransmitting Keyboard >>B<< Key up\n");
                rsi_bt_hid_send_interrupt_data(remote_dev_addr, hid_int_data_kb_b_key, hid_int_data_len);
                break;
              }
            case 3:
              {
                printf("\nTransmitting Keyboard ACtion Key up\n");
                rsi_bt_hid_send_interrupt_data(remote_dev_addr, hid_int_data_kb_reset, hid_int_data_len);
                break;
              }
            default:
              {
                printf("\n ENTERED DEFAULT \n");
              }
          }
             if(++action >= 4)
             {
               action = 0;
             }
        }
        break;

      case RSI_APP_EVENT_HID_MSG_CONTROL:
        {
          LOG_PRINT("\nHID Received Control EVENT \n");
          rsi_bt_app_clear_event (RSI_APP_EVENT_HID_MSG_CONTROL);
        }
        break;
      case RSI_APP_EVENT_HID_MSG_GET_REPORT:
        {
          LOG_PRINT("\nHID  Received Get Report EVENT \n");
          rsi_bt_app_clear_event (RSI_APP_EVENT_HID_MSG_GET_REPORT);
        }
        break;
      case RSI_APP_EVENT_HID_MSG_SET_REPORT:
        {
          LOG_PRINT("\nHID  Received Set Report EVENT \n");
          rsi_bt_app_clear_event (RSI_APP_EVENT_HID_MSG_SET_REPORT);
        }
        break;
      case RSI_APP_EVENT_HID_MSG_GET_PROTOCOL:
        {
          LOG_PRINT("\nHID  Received Get Protocol EVENT \n");
          rsi_bt_app_clear_event (RSI_APP_EVENT_HID_MSG_GET_PROTOCOL);
        }
        break;
      case RSI_APP_EVENT_HID_MSG_SET_PROTOCOL:
        {
          LOG_PRINT("\nHID  Received Set Protocol EVENT \n");
          rsi_bt_app_clear_event (RSI_APP_EVENT_HID_MSG_SET_PROTOCOL);
        }
        break;

    }
  }

  return 0;
}

/*==============================================*/
/**
 * @fn         main_loop
 * @brief      Schedules the Driver task.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function schedules the Driver task.
 */
void main_loop (void)
{
	while (1) {
		rsi_wireless_driver_task ();
	}
}

/*==============================================*/
/**
 * @fn         main
 * @brief      main function
 * @param[in]  none.
 * @return     none.
 * @section description
 * This is the main function to call Application
 */
int main (void)
{
  int32_t status;
#ifdef RSI_WITH_OS
  rsi_task_handle_t bt_task_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;
#endif



#ifndef RSI_WITH_OS
  //Start BT Stack
  intialize_bt_stack(STACK_BT_MODE);
  
  //! Call BLE Peripheral application
  status = rsi_bt_hid_master();

  //! Application main loop
  main_loop();

  return status;
#endif
#ifdef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN );
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! Redpine module intialisation
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT Stack
  intialize_bt_stack(STACK_BT_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_bt_hid_master, "bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  rsi_start_os_scheduler();
  return status;
#endif
}
