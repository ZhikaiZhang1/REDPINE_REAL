/**
 * @file    rsi_bt_a2dp_sink.c
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
 *  @brief : This file contains example application for BT Classic A2DP Slave Role
 *
 *  @section Description  This application serves as a BT A2DP Slave.
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

#ifdef RSI_M4_INTERFACE
#include <rsi_board.h>
#endif
#include "rsi_a2dp.h"

//! Common include file 
#include<rsi_common_apis.h>

//! application defines 
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT
#endif
#define RSI_BT_LOCAL_NAME             "A2DP_SINK"
#define PIN_CODE                      "0000"

//! application events list
#define RSI_APP_EVENT_CONNECTED       1
#define RSI_APP_EVENT_PINCODE_REQ     2
#define RSI_APP_EVENT_LINKKEY_SAVE    3
#define RSI_APP_EVENT_AUTH_COMPLT     4
#define RSI_APP_EVENT_DISCONNECTED    5
#define RSI_APP_EVENT_LINKKEY_REQ     6
/** ssp related defines********/
#define RSI_APP_EVENT_PASSKEY_DISPLAY 10
#define RSI_APP_EVENT_PASSKEY_REQUEST 11
#define RSI_APP_EVENT_SSP_COMPLETE    12
#define RSI_APP_EVENT_CONFIRM_REQUEST 13

#define RSI_APP_EVENT_A2DP_CONN        14
#define RSI_APP_EVENT_A2DP_DISCONN     15
#define RSI_APP_EVENT_A2DP_CONFIGURE   16
#define RSI_APP_EVENT_A2DP_OPEN        17
#define RSI_APP_EVENT_A2DP_START       18
#define RSI_APP_EVENT_A2DP_SUSPEND     19
#define RSI_APP_EVENT_A2DP_ABORT       20
#define RSI_APP_EVENT_A2DP_CLOSE       21

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN            10000

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE 3000

//! BT task priority
#define RSI_BT_TASK_PRIORITY   1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  3000

void rsi_wireless_driver_task(void);

#endif

//! Memory to initialize driver
uint8_t bt_global_buf[BT_GLOBAL_BUFF_LEN] = {0};

//! Application global parameters.
static uint32_t rsi_app_async_event_map = 0;
static rsi_bt_resp_get_local_name_t   local_name = {0};
static uint8_t str_conn_bd_addr[18];
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t data[6];
//static uint16_t data_len;
static uint32_t sample_freq;
uint32_t pcm_packet_missed;
uint32_t pcm_packet;
uint32_t pcm_packet_loop;
uint32_t i2s_module_idel;
uint32_t pcm_packets_completed;
uint32_t decode_fail;
volatile uint8_t pcm_mutex;

data_pkt_t *sbc_queue_head = NULL;
data_pkt_t  encode_data_pkts[MAX_DATA_PKTS] = {0};

data_pkt_t *pcm_queue_head = NULL;
data_pkt_t  pcm_data_pkts[MAX_DATA_PKTS] = {0};

#ifdef RSI_M4_INTERFACE
extern volatile uint32_t tx_done, rx_done;
extern ARM_DRIVER_SAI *SAIdrv;
#endif



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
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, conn_event->dev_addr);
  LOG_PRINT ("on_conn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_pincode_request->dev_addr);
  LOG_PRINT ("on_pin_coe_req: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_req->dev_addr);
  LOG_PRINT ("linkkey_req: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_save->dev_addr);
  LOG_PRINT ("linkkey_save: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, auth_complete->dev_addr);
  LOG_PRINT ("auth_complete: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, bt_disconnected->dev_addr);
  LOG_PRINT ("on_disconn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_connect
 * @brief      invoked when a2dp profile connected event is received
 * @param[out] a2dp_connect, a2dp connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp connected remote device information
 */
void rsi_bt_app_on_a2dp_connect (uint16_t resp_status, rsi_bt_event_a2dp_connect_t *a2dp_connect)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_CONN);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_connect->dev_addr);
  LOG_PRINT ("a2dp_conn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_disconnect
 * @brief      invoked when a2dp profile disconnected event is received
 * @param[out] a2dp_disconnect, a2dp disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp disconnected remote device information
 */
void rsi_bt_app_on_a2dp_disconnect (uint16_t resp_status, rsi_bt_event_a2dp_disconnect_t *a2dp_disconnect)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_DISCONN);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_disconnect->dev_addr);
  LOG_PRINT ("a2dp_disconn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_configure
 * @brief      invoked when a2dp profile configured event is received
 * @param[out] a2dp_connect, a2dp configured remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp configured remote device information
 */
void rsi_bt_app_on_a2dp_configure (uint16_t resp_status, rsi_bt_event_a2dp_configure_t *a2dp_configure)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_CONFIGURE);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_configure->dev_addr);
  LOG_PRINT ("a2dp_configure: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_open
 * @brief      invoked when a2dp profile open event is received
 * @param[out] a2dp_open, a2dp open remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp open remote device information
 */
void rsi_bt_app_on_a2dp_open (uint16_t resp_status, rsi_bt_event_a2dp_open_t *a2dp_oen)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_OPEN);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_oen->dev_addr);
  LOG_PRINT ("a2dp_open: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_start
 * @brief      invoked when a2dp profile start event is received
 * @param[out] a2dp_start, a2dp start remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp start remote device information
 */
void rsi_bt_app_on_a2dp_start (uint16_t resp_status, rsi_bt_event_a2dp_start_t *a2dp_start)
{
    uint32_t freq;
    
	sample_freq = freq = a2dp_start->sample_freq;
	rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_START);

  LOG_PRINT ("a2dp_start:sample freq : %d\r\n", freq);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_suspend
 * @brief      invoked when a2dp profile suspend event is received
 * @param[out] a2dp_suspend, a2dp suspend remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp suspend remote device information
 */
void rsi_bt_app_on_a2dp_suspend (uint16_t resp_status, rsi_bt_event_a2dp_suspend_t *a2dp_suspend)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_SUSPEND);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_suspend->dev_addr);
  LOG_PRINT ("a2dp_suspend: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_abort
 * @brief      invoked when a2dp profile abort event is received
 * @param[out] a2dp_abort, a2dp abort remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp abort remote device information
 */
void rsi_bt_app_on_a2dp_abort (uint16_t resp_status, rsi_bt_event_a2dp_abort_t *a2dp_abort)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_ABORT);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_abort->dev_addr);
  LOG_PRINT ("a2dp_abort: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_close
 * @brief      invoked when a2dp profile close event is received
 * @param[out] a2dp_close, a2dp close remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp close remote device information
 */
void rsi_bt_app_on_a2dp_close (uint16_t resp_status, rsi_bt_event_a2dp_close_t *a2dp_close)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_CLOSE);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_close->dev_addr);
  LOG_PRINT ("a2dp_close: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         allocate_data_pkt
 * @brief      This function is used to allocate free data packet from given pool.
 * @param[in] p_data_pool, pool data structure.
 * @return     data_pkt_t, return with free data packet or NULL
 * @section description
 * This function is used to allocate free data packet from given pool,
   if free packet is not available in given pool return with NULL packet. 
 */
data_pkt_t  *allocate_data_pkt (data_pkt_t *p_data_pool)
{
  uint8_t ix;
  data_pkt_t *p_pool = p_data_pool;

  for (ix = 0; ix < MAX_DATA_PKTS; ix++) {
      if (p_pool[ix].used == 0) {
          p_pool[ix].used = 1;
          return (&p_pool[ix]);
      }
  }
  //LOG_PRINT ("SBC packet are not available\r\n");
  return NULL;
}

/*==============================================*/
/**
 * @fn         free_data_pkt
 * @brief      This function is used to free the data packet in given pool.
 * @param[in] p_data_pool, pool data structure.
 * @param[in] p_data_pkt, data packet.
 * @return     None.
 * @section description
 * This function is used to allocate free data packet from given pool,
   if free packet is not available in given pool return with NULL packet. 
 */
void free_data_pkt (data_pkt_t  *p_data_pool, data_pkt_t *p_data_pkt)
{
  uint8_t ix;
  data_pkt_t  *p_pool = p_data_pool;

  if (p_data_pkt->used == 1) {
    for (ix = 0; ix < MAX_DATA_PKTS; ix++) {
        if (&p_pool[ix] == p_data_pkt) {
            p_pool[ix].used = 0;
            return;
        }
    }
  } else {
  }

  if (ix >= MAX_DATA_PKTS) {
		while (1);
  }

  return;
}

/*==============================================*/
/**
 * @fn         add_packet_to_queue
 * @brief      This function is used to add the data packet to specific queue. 
 * @param[in]  queue, data packet queue head.
 * @param[in]  data_pkt, data packet.
 * @return     None.
 * @section description
 * * This function is used to add the data packet to given queue.
 */
void add_packet_to_queue (data_pkt_t **queue, data_pkt_t  *data_pkt)
{
	data_pkt_t *temp;
	
	
  data_pkt->next = NULL;

  if (*queue == NULL) {
      *queue = data_pkt;
  } else {
		 temp = *queue;
		 while (temp->next != NULL) {
			 temp = temp->next;
		 }
      temp->next = data_pkt;
      temp = data_pkt;
  }

  return;
}

/*==============================================*/
/**
 * @fn         get_packet_from_queue
 * @brief      This function is used to get the data packet from the specific queue. 
 * @param[in]  queue, data packet queue head.
 * @return     data_pkt_t, Data packet from the given queue.
 * @section description
 * * This function is used to get the data packet from the given queue.
 */
data_pkt_t  *get_packet_from_queue (data_pkt_t **queue)
{
  data_pkt_t  *pkt;

  if (*queue != NULL) {
      pkt = *queue;
      *queue = (*queue)->next;
     /* if (*queue == NULL) {
        *queue = NULL;
      }*/
      return pkt;
  }

  return NULL;
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
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_passkey_request->dev_addr);
  LOG_PRINT ("passkey_request: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, ssp_complete->dev_addr);
  LOG_PRINT ("ssp_complete: str_conn_bd_addr: %s\r\n",str_conn_bd_addr);
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
 * @fn         rsi_bt_a2dp_sink
 * @brief      Tests the BT Classic A2DP sink Slave role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the A2DP sink Slave role.
 */

int32_t rsi_bt_a2dp_sink (void)
{
  int32_t status = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[18] = {0};
  uint8_t eir_data[200] = {2,1,0};
	uint16_t    pcm_data_len;
	data_pkt_t  *pcm_pkt;


#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(bt_global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! module intialisation 
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
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
      rsi_bt_on_passkey_display,//passkey_display
      NULL,//remote_name_req+cancel
      rsi_bt_on_confirm_request,//confirm req
      rsi_bt_app_on_pincode_req,
      rsi_bt_on_passkey_request,//passkey request
      NULL,//inquiry complete
      rsi_bt_app_on_auth_complete,
      rsi_ble_app_on_linkkey_req,//linkkey request
      rsi_bt_on_ssp_complete,//ssp coplete
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
  rsi_6byte_dev_address_to_ascii ((int8_t *)str_bd_addr, local_dev_addr);
  LOG_PRINT ("\r\nlocal_bd_address: %s\r\n", str_bd_addr);

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

  //! start the connectability mode
  status = rsi_bt_set_connectable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! initilize the A2DP profile
  status = rsi_bt_a2dp_init();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#ifdef RSI_M4_INTERFACE 
	//I2S Initiallization
	rsi_i2s_slave_init ();
#endif

  //! register the A2DP profile callback's
  rsi_bt_a2dp_register_callbacks (rsi_bt_app_on_a2dp_connect,
                                 rsi_bt_app_on_a2dp_disconnect,
                                 rsi_bt_app_on_a2dp_configure,
                                 rsi_bt_app_on_a2dp_open,
                                 rsi_bt_app_on_a2dp_start,
                                 rsi_bt_app_on_a2dp_suspend,
                                 rsi_bt_app_on_a2dp_abort,
                                 rsi_bt_app_on_a2dp_close,
                                 NULL,//rsi_bt_on_a2dp_encode_data,
                                 NULL,
                                 NULL
                                 );

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
          status = rsi_bt_pincode_request_reply((int8_t *)str_conn_bd_addr, pin_code, 1);
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
          rsi_bt_linkkey_request_reply ((int8_t *)str_conn_bd_addr, NULL, 0);
        }
        break;

      case RSI_APP_EVENT_A2DP_CONN:
        {
          //! a2dp connected event

          //! clear the a2dp connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONN);
        }
        break;

      case RSI_APP_EVENT_A2DP_DISCONN:
        {
          //! a2dp disconnected event

          //! clear the a2dp disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_DISCONN);
        }
        break;

      case RSI_APP_EVENT_A2DP_CONFIGURE:
        {
          //! a2dp configured event

          //! clear the a2dp configured event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONFIGURE);
        }
        break;

      case RSI_APP_EVENT_A2DP_OPEN:
        {
          //! a2dp open event

          //! clear the a2dp open event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_OPEN);
        }
        break;

      case RSI_APP_EVENT_A2DP_START:
        {
          //! a2dp start event

          //! clear the a2dp start event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_START);
		  
        }
        break;

      case RSI_APP_EVENT_A2DP_SUSPEND:
        {
          //! a2dp suspend event

          //! clear the a2dp suspend event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_SUSPEND);
        }
        break;

      case RSI_APP_EVENT_A2DP_ABORT:
        {
          //! a2dp abort event

          //! clear the a2dp abort event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_ABORT);
        }
        break;

      case RSI_APP_EVENT_A2DP_CLOSE:
        {
          //! a2dp close event

          //! clear the a2dp close event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CLOSE);
        }
        break;

      case RSI_APP_EVENT_PASSKEY_DISPLAY:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_PASSKEY_DISPLAY);
        }
        break;

      case RSI_APP_EVENT_PASSKEY_REQUEST:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_PASSKEY_REQUEST);
        }
        break;

      case RSI_APP_EVENT_SSP_COMPLETE:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SSP_COMPLETE);
					LOG_PRINT(" SSP conection completed\n");
        }
        break;

      case RSI_APP_EVENT_CONFIRM_REQUEST:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_CONFIRM_REQUEST);
					LOG_PRINT("Confirmation is requested\n");
					rsi_bt_accept_ssp_confirm((int8_t *)str_conn_bd_addr);

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
#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

#ifndef RSI_WITH_OS
  //Start BT-BLE Stack
  intialize_bt_stack(STACK_BT_MODE);
  
  //! Call BLE Peripheral application
  status = rsi_bt_a2dp_sink();

  //! Application main loop
  main_loop();

  return status;
#endif

#ifdef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(bt_global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! RS9113 initialization
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT-BLE Stack
  intialize_bt_stack(STACK_BT_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_bt_a2dp_sink, "bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  rsi_start_os_scheduler();
  return status;
#endif
}
