/**
 * @file    rsi_bt_spp_slave.c
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

/**
 * Include files
 * */

//! BT include file to refer BT APIs
#include<rsi_bt_apis.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_bt_config.h>
#include<rsi_bt.h>
#include<stdio.h>
#include<rsi_pts_inc.h>

//! application defines 
#if 1//def RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#define LOG_SCAN                      scanf
#else 
#define LOG_PRINT
#define LOG_SCAN                      
#endif
#define RSI_BT_LOCAL_NAME             "BT_DUAL_MODE"
#define PIN_CODE                      "0000"

#define INITIATOR        0x01
#define RESPONDER        0x02
#if ( GAP_DM_GIN_BV_01_C || GAP_DM_LIN_BV_01_C || GAP_DM_NAD_BV_01_C || GAP_DM_LEP_BV_04_C || GAP_DM_LEP_BV_05_C \
    || GAP_DM_LEP_BV_09_C || GAP_DM_LEP_BV_10_C || GATT_CL_GAC_BV_01_C || GATT_CL_GAD_BV_01_C || GATT_CL_GAD_BV_02_C \
    || GATT_CL_GAD_BV_03_C || GATT_CL_GAD_BV_04_C || GATT_CL_GAT_BV_01_C || GATT_CL_GAR_BI_13_C || GATT_CL_GAR_BI_18_C \
    || GATT_CL_GAR_BI_19_C || GATT_CL_GAW_BI_09_C || GATT_CL_GAW_BI_32_C || GATT_CL_GAW_BV_06_C || GATT_CL_GAD_BV_05_C \
    || GATT_CL_GAD_BV_06_C || GATT_CL_GAR_BV_01_C || GATT_CL_GAR_BI_01_C || GATT_CL_GAR_BI_02_C || GATT_CL_GAR_BV_03_C \
    || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_07_C || GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BI_12_C || GATT_CL_GAR_BI_14_C \
    || GATT_CL_GAR_BV_05_C || GATT_CL_GAR_BV_06_C || GATT_CL_GAR_BV_07_C || GATT_CL_GAR_BI_34_C || GATT_CL_GAW_BV_01_C \
    || GATT_CL_GAW_BV_03_C || GATT_CL_GAW_BI_02_C || GATT_CL_GAW_BI_03_C || GATT_CL_GAW_BV_05_C || GATT_CL_GAW_BI_07_C \
    || GATT_CL_GAW_BI_08_C || GATT_CL_GAW_BV_08_C || GATT_CL_GAW_BV_09_C || GATT_CL_GAW_BI_33_C || GATT_CL_GAW_BI_34_C \
    || GATT_CL_GAN_BV_01_C || GATT_CL_GAI_BV_01_C || GATT_CL_GAS_BV_01_C || GATT_CL_GAT_BV_02_C)
#define ROLE             INITIATOR 
#else
#define ROLE             RESPONDER 
#endif
#define REMOTE_BD_ADDR                          "00:1B:DC:F2:1C:54"
//#define REMOTE_BD_ADDR                          "00:1B:DC:07:31:E5"

//! application events list
#define RSI_APP_EVENT_CONNECTED       1
#define RSI_APP_EVENT_PINCODE_REQ     2
#define RSI_APP_EVENT_LINKKEY_SAVE    3
#define RSI_APP_EVENT_AUTH_COMPLT     4
#define RSI_APP_EVENT_DISCONNECTED    5
#define RSI_APP_EVENT_LINKKEY_REQ     6
#define RSI_APP_EVENT_SPP_CONN        7
#define RSI_APP_EVENT_SPP_DISCONN     8
#define RSI_APP_EVENT_SPP_RX          9

#define RSI_APP_EVENT_INQUIRY_COMPLETE 10
#define RSI_APP_EVENT_INQUIRY_RESP     11

/** ssp related defines********/
#define RSI_APP_EVENT_PASSKEY_DISPLAY 12
#define RSI_APP_EVENT_PASSKEY_REQUEST 13
#define RSI_APP_EVENT_SSP_COMPLETE    14
#define RSI_APP_EVENT_CONFIRM_REQUEST 15


#define RSI_APP_BT_CONNECT            16
#define RSI_APP_BT_DISCONNECT         17


//! Memory length for driver
//#define BT_GLOBAL_BUFF_LEN            10000
#define BT_DATA                       0

//! Enumeration for commands used in application
typedef enum rsi_app_cmd_e
{
  RSI_DATA = 0
}rsi_app_cmd_t;

//! Memory to initialize driver
//uint8_t bt_global_buf[BT_GLOBAL_BUFF_LEN] = {0};

//! Application global parameters.
static uint32_t rsi_app_async_event_map = 0;
static rsi_bt_resp_get_local_name_t   local_name = {0};
static uint8_t str_conn_bd_addr[18];
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t spp_data[200];
static uint16_t spp_data_len;
static uint8_t bt_device_found=0;
rsi_bt_req_remote_name_t remote_device_info = {0};
static uint8_t linkkey[16]= {0};

//! global_variable
extern bt_done;
extern ble_done;
extern void intimate_ble();
extern void intimate_gatt_ble(uint8_t *);
extern void rsi_bt_app_send_to_ble (uint8_t  data_type, uint8_t *data, uint16_t data_len);
extern last_handle;

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
  LOG_PRINT ("on_conn\r\n");
  if(resp_status == RSI_SUCCESS) {
    rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
    
    memcpy (remote_dev_bd_addr, conn_event->dev_addr, RSI_DEV_ADDR_LEN);
    LOG_PRINT ("\n\n on_conn - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, conn_event->dev_addr));


  } else {
//    rsi_bt_app_set_event (RSI_APP_EVENT_INQUIRY_COMPLETE);
    rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
  }
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
  memcpy (remote_dev_bd_addr, user_pincode_request->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n on_pin_code_req - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, user_pincode_request->dev_addr));

  rsi_bt_app_set_event (RSI_APP_EVENT_PINCODE_REQ);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_linkkey_req
 * @brief      invoked when linkkey request event is received
 * @param[out] user_linkkey_req, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey request from remote device
 */
void rsi_bt_app_on_linkkey_req (uint16_t status, rsi_bt_event_user_linkkey_request_t  *user_linkkey_req)
{
  memcpy (remote_dev_bd_addr, user_linkkey_req->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n linkkey_req - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, user_linkkey_req->dev_addr));

  rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_REQ);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_linkkey_save
 * @brief      invoked when linkkey save event is received
 * @param[out] user_linkkey_req, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey save from local device
 */
void rsi_bt_app_on_linkkey_save (uint16_t status, rsi_bt_event_user_linkkey_save_t *user_linkkey_save)
{
  memcpy (linkkey, user_linkkey_save->linkKey, 16);
  memcpy (remote_dev_bd_addr, user_linkkey_save->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n linkkey_save - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, user_linkkey_save->dev_addr));

  rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_SAVE);
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
  memcpy (remote_dev_bd_addr, auth_complete->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n auth_complete - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, auth_complete->dev_addr));

  rsi_bt_app_set_event (RSI_APP_EVENT_AUTH_COMPLT);
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
  memcpy (remote_dev_bd_addr, bt_disconnected->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n on_disconn - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, bt_disconnected->dev_addr));

  LOG_PRINT ("\n rsi_bt_app_on_disconn : remote device disconnected \r\n");
  rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_unbond
 * @brief      invoked when disconnect/ unboned event is received
 * @param[out] resp_status, disconnect/ unbond status/error
 * @param[out] bt_unbond, disconnected and unbond remote device information
 * @return     none.
 * @section description
 * This callback function indicates the unbond device information
 */
void rsi_bt_app_on_unbond (uint16_t resp_status, rsi_bt_event_unbond_t *bt_unbond)
{
  memcpy (remote_dev_bd_addr, bt_unbond->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n on_unbond - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, bt_unbond->dev_addr));

  LOG_PRINT ("\n rsi_bt_app_on_unbond : remote device disconnected \r\n");
  rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
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
  memcpy (remote_dev_bd_addr, spp_connect->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n spp_conn - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, spp_connect->dev_addr));

  rsi_bt_app_set_event (RSI_APP_EVENT_SPP_CONN);
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
  memcpy (remote_dev_bd_addr, spp_disconn->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n spp_disconn- str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, spp_disconn->dev_addr));

  rsi_bt_app_set_event (RSI_APP_EVENT_SPP_DISCONN);

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
  //rsi_bt_app_set_event (RSI_APP_EVENT_SPP_RX);
  //spp_data_len = spp_receive->data_len;
  //memcpy (spp_data, spp_receive->data, spp_receive->data_len);
  rsi_bt_app_send_to_ble ( BT_DATA, spp_receive->data, spp_receive->data_len);
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_send_to_bt
 * @brief      data transfer from BT module to BLE module.
 * @param[in]  data_type, data type, now we are using sample data type '0'.
 * @param[in]  data, raw data received from connected BT device..
 * @param[in]  data_len, raw data length
 * @return     none.
 * @section description
 * This function is used to initialize the BLE module
 */

void rsi_ble_app_send_to_bt (uint8_t  data_type, uint8_t *data, uint16_t data_len) 
{
  if ((data == NULL) || 
      (data_len == 0)) {
    return;
  }
  spp_data_len = RSI_MIN (sizeof (spp_data), data_len);
  memset (spp_data, 0, sizeof (spp_data));
	memcpy (spp_data, data, spp_data_len);
  rsi_bt_app_set_event (RSI_APP_EVENT_SPP_RX);
}

/*==============================================*/
/**
 * @fn         rsi_bt_inquiry_resp_event
 * @brief      invoked when inquiry response event is received
 * @param[out] inquiry_resp,inquiry response from remote device
 * @return     none.
 * @section description
 * This callback function indicates the inquiry response received event
 */
void rsi_bt_inquiry_resp_event (uint16_t resp_status, rsi_bt_event_inquiry_response_t *inquiry_resp)
{
  LOG_PRINT ("\n\n inquiry resp - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, inquiry_resp->dev_addr));

  if ((strcmp(str_conn_bd_addr,REMOTE_BD_ADDR)==0))
  {
    memcpy (remote_dev_bd_addr, inquiry_resp->dev_addr, RSI_DEV_ADDR_LEN);
    bt_device_found = 1;
    //rsi_bt_app_set_event (RSI_APP_EVENT_INQUIRY_RESP);
  }
}

/*==============================================*/
/**
 * @fn         rsi_bt_inquiry_complete_event
 * @brief      invoked when invoked complete event is received
 * @return     none.
 * @section description
 * This callback function indicates the indicates complete received event
 */

void rsi_bt_inquiry_complete_event (uint16_t resp_status) 
{
  LOG_PRINT ("inquiry complete event:  %X\r\n", resp_status);
	rsi_bt_app_set_event (RSI_APP_EVENT_INQUIRY_COMPLETE);
}

/*==============================================*/
/**
 * @fn         rsi_bt_remote_name_event
 * @brief      invoked when remote name request event is received
 * @param[out] name_resp, name of the remote device
 * @return     none.
 * @section description
 * This callback function indicates the remote name received event
 */
void rsi_bt_remote_name_event (uint16_t resp_status, rsi_bt_event_remote_device_name_t *name_resp)
{
  if (resp_status == RSI_SUCCESS)
  {
    LOG_PRINT ("Remote dev name : %s \n", name_resp->remote_device_name);
  } else {
    rsi_bt_app_set_event (RSI_APP_EVENT_INQUIRY_COMPLETE);
  }
}
/*==============================================*/
/**
 * @fn         rsi_bt_on_passkey_display
 * @brief      invoked when passkey diaplay event is received
 * @param[out] passkey display, remote device passkey information
 * @return     none.
 * @section description
 * This callback function indicates the passkey display event
 */
void rsi_bt_on_passkey_display (uint16_t resp_status, rsi_bt_event_user_passkey_display_t *bt_event_user_passkey_display)
{
  LOG_PRINT ("passkey: %d", bt_event_user_passkey_display->passkey );
	rsi_bt_app_set_event (RSI_APP_EVENT_PASSKEY_DISPLAY);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_passkey_request
 * @brief      invoked when passkey request event is received
 * @param[out] passkey request, passkey request to remote device 
 * @return     none.
 * @section description
 * This callback function indicates the passkey request event
 */

void rsi_bt_on_passkey_request (uint16_t resp_status, rsi_bt_event_user_passkey_request_t *user_passkey_request)
{
  memcpy (remote_dev_bd_addr, user_passkey_request->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n on passkey_request - str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, user_passkey_request->dev_addr));

  rsi_bt_app_set_event (RSI_APP_EVENT_PASSKEY_REQUEST);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_ssp_complete
 * @brief      invoked when ssp complete event is received
 * @param[out] ssp complete, ssp completed remote device information 
 * @return     none.
 * @section description
 * This callback function indicates the ssp complete event
 */
void rsi_bt_on_ssp_complete (uint16_t resp_status, rsi_bt_event_ssp_complete_t *ssp_complete)
{
  memcpy (remote_dev_bd_addr, ssp_complete->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n ssp_complete- str_conn_remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, ssp_complete->dev_addr));

  rsi_bt_app_set_event (RSI_APP_EVENT_SSP_COMPLETE);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_confirm_request
 * @brief      invoked when confirmation request event is received
 * @param[out] confirmation request,confirmation request to remote device  
 * @return     none.
 * @section description
 * This callback function indicates the confirmation request event
 */
void rsi_bt_on_confirm_request (uint16_t resp_status, rsi_bt_event_user_confirmation_request_t *user_confirmation_request)
{
  LOG_PRINT ("data: %s",user_confirmation_request->confirmation_value );
	rsi_bt_app_set_event (RSI_APP_EVENT_CONFIRM_REQUEST);
}

/*==============================================*/
/**
 * @fn         intimate_bt
 * @brief      invoked when intimation requested from BLE
 * @return     none.
 * @section description
 * This callback function indicates the intimation request from the BLE app
 */
void intimate_bt(uint8_t *rem_addr) 
{
  memcpy (remote_dev_bd_addr, rem_addr, 6);
#if (GAP_DM_LEP_BV_04_C)
  bt_device_found = 1;
#else
  bt_device_found = 0;
#endif
  rsi_bt_app_set_event (RSI_APP_EVENT_INQUIRY_COMPLETE);
}


/*==============================================*/
/**
 * @fn         rsi_bt_app_gatt_connected
 * @brief      invoked when gatt connected event is received
 * @param[out] gatt_con,gatt connection to remote device  
 * @return     none.
 * @section description
 * This callback function indicates the gatt connected event
 */
void rsi_bt_app_gatt_connected (uint16_t status, rsi_bt_event_gatt_connect_t *gatt_conn)
{
  if(status == RSI_SUCCESS) {
    LOG_PRINT (" \n GATT Connected over Classic ACL link \n");
    memcpy (remote_dev_bd_addr, gatt_conn->dev_addr, RSI_DEV_ADDR_LEN);
    LOG_PRINT ("\n\n on_gatt_conn - str_conn_remote_dev_addr : %s\r\n",
        rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_bd_addr, gatt_conn->dev_addr));
  }
  intimate_gatt_ble (remote_dev_bd_addr);
}
/*==============================================*/
/**
 * @fn         rsi_bt_app_gatt_disconnected
 * @brief      invoked when gatt disconnected event is received
 * @param[out] gatt_disconn, gatt disconnected from remote device  
 * @return     none.
 * @section description
 * This callback function indicates the gatt disconnected event
 */

void rsi_bt_app_gatt_disconnected (uint16_t status, rsi_bt_event_gatt_disconnect_t *gatt_disconn)
{
  if(status == RSI_SUCCESS) {
    LOG_PRINT (" \n GATT Dis-Connected over Classic ACL link \n");
  }
  last_handle = 1;
}
/*==============================================*/
/**
 * @fn         rsi_bt_app_init
 * @brief      Tests the BT Classic SPP Slave role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the SPP Slave role.
 */
int32_t rsi_bt_app_init (void)
{
  int32_t status = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[18] = {0};
  
#if (GAP_MOD_LDIS_BV_01_C )
  int32_t time_out_ms = 120000; //60000 /* expecting test case error */
  uint8_t iac_lap[3]={0x00,0x8B,0x9E};
#elif (GAP_MOD_LDIS_BV_03_C)
  int32_t time_out_ms = 60000;  
  uint8_t iac_lap[3]={0x00,0x8B,0x9E};
#elif (GAP_MOD_LDIS_BV_02_C )
  int32_t time_out_ms = 30720;
  uint8_t iac_lap[3]={0x33,0x8B,0x9E};
#endif

  //! BT register GAP callbacks:
  rsi_bt_gap_register_callbacks(
      NULL, //role_change
      rsi_bt_app_on_conn, 
      rsi_bt_app_on_unbond, 
      rsi_bt_app_on_disconn,
      rsi_bt_inquiry_resp_event,//scan_resp
      rsi_bt_remote_name_event,//remote_name_req
      rsi_bt_on_passkey_display,//passkey_display
      NULL,//remote_name_req+cancel
      rsi_bt_on_confirm_request,//confirm req
      rsi_bt_app_on_pincode_req,
      rsi_bt_on_passkey_request,//passkey request
      rsi_bt_inquiry_complete_event,//inquiry complete
      rsi_bt_app_on_auth_complete,
      rsi_bt_app_on_linkkey_req,//linkkey request
      rsi_bt_on_ssp_complete,//ssp coplete
      rsi_bt_app_on_linkkey_save,
      NULL, //get services
      NULL,
      NULL,
      NULL); //search service
  
  //! BT register GATT callbacks:
  rsi_bt_gatt_extended_register_callbacks (
      rsi_bt_app_gatt_connected,
      rsi_bt_app_gatt_disconnected);


  LOG_PRINT ("rsi_bt_app_on_conn: %x\r\n", rsi_bt_app_on_conn);


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

  rsi_bt_set_ssp_mode (1, 1);
 
#if (ROLE == RESPONDER) 

#if (GAP_MOD_NDIS_BV_01_C)
  //! keep the device in non-discoverable mode
  status = rsi_bt_stop_discoverable ();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#elif (GAP_MOD_LDIS_BV_01_C || GAP_MOD_LDIS_BV_02_C || GAP_MOD_LDIS_BV_03_C) 
  status = rsi_bt_start_limited_discoverable (time_out_ms);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Write current IAC LAP
  status = rsi_bt_write_current_iac_lap (1,iac_lap);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#else
  //! start the discover mode
  status = rsi_bt_start_discoverable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif
#if (GAP_DM_NCON_BV_01_C || GAP_MOD_NCON_BV_01_C)
  //! start the Non - connectability mode
  status = rsi_bt_set_non_connectable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#else
  //! start the connectability mode
  status = rsi_bt_set_connectable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif
#else
#if (GATT_CL_GAC_BV_01_C || GATT_CL_GAD_BV_01_C || GATT_CL_GAD_BV_02_C || GATT_CL_GAD_BV_03_C \
    || GATT_CL_GAT_BV_01_C || GATT_CL_GAR_BI_13_C || GATT_CL_GAR_BI_18_C || GATT_CL_GAR_BI_19_C \
    || GATT_CL_GAW_BI_09_C || GATT_CL_GAW_BI_32_C || GATT_CL_GAW_BV_06_C || GATT_CL_GAD_BV_04_C \
    || GATT_CL_GAD_BV_05_C || GATT_CL_GAD_BV_06_C || GATT_CL_GAR_BV_01_C || GATT_CL_GAR_BI_01_C \
    || GATT_CL_GAR_BI_02_C || GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_07_C \
    || GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BI_12_C || GATT_CL_GAR_BI_14_C || GATT_CL_GAR_BV_05_C \
    || GATT_CL_GAR_BV_06_C || GATT_CL_GAR_BV_07_C || GATT_CL_GAR_BI_34_C || GATT_CL_GAW_BV_01_C \
    || GATT_CL_GAW_BV_03_C || GATT_CL_GAW_BI_02_C || GATT_CL_GAW_BI_03_C || GATT_CL_GAW_BV_05_C \
    || GATT_CL_GAW_BI_07_C || GATT_CL_GAW_BI_08_C || GATT_CL_GAW_BV_08_C || GATT_CL_GAW_BV_09_C \
    || GATT_CL_GAW_BI_33_C || GATT_CL_GAW_BI_34_C || GATT_CL_GAN_BV_01_C || GATT_CL_GAI_BV_01_C \
    || GATT_CL_GAS_BV_01_C || GATT_CL_GAT_BV_02_C)

  rsi_ascii_dev_address_to_6bytes_rev (remote_dev_bd_addr, REMOTE_BD_ADDR);
  status = rsi_bt_connect(remote_dev_bd_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#else
  
//! start the inquiry
#if ((GAP_DM_NAD_BV_01_C || GAP_DM_NAD_BV_02_C || GAP_DM_LEP_BV_04_C || GAP_DM_LEP_BV_05_C  || GAP_DM_LEP_BV_10_C || GAP_DM_GIN_BV_01_C))
  status = rsi_bt_inquiry( 2, 20000, 0);
#elif (GAP_DM_LIN_BV_01_C)
  status = rsi_bt_inquiry( 0x81, 20000, 0);  /* doing limited discovery */
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif
#endif
#endif
#if 1
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
#endif

#if (GATT_CL_GAR_BI_04_C || GATT_CL_GAR_BI_05_C || GATT_CL_GAR_BI_10_C || GATT_CL_GAR_BI_11_C || GATT_CL_GAR_BI_17_C || GATT_CL_GAR_BI_21_C || GATT_CL_GAR_BI_22_C || GATT_CL_GAR_BI_35_C || GATT_CL_GAW_BI_05_C || GATT_CL_GAW_BI_06_C || GATT_CL_GAW_BI_12_C || GATT_CL_GAW_BI_13_C)
  bt_done =1;
  intimate_ble();
#endif

  return status;
}

int32_t rsi_bt_app_task ()
{
  int32_t status = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[18] = {0};


  //! Application main loop

  //! checking for received events
  temp_event_map = rsi_bt_app_get_event ();
  if (temp_event_map == RSI_FAILURE) {
    //! if events are not received loop will be continued.
    return 0;
  }

  //! if any event is received, it will be served.
  switch(temp_event_map)
  {
    case RSI_APP_EVENT_CONNECTED:
      {
        //! remote device connected event

        //! clear the connected event.
        rsi_bt_app_clear_event (RSI_APP_EVENT_CONNECTED);
#if (GAP_DM_LEP_BV_11_C)
        sleep(20);
        rsi_bt_app_set_event (RSI_APP_BT_DISCONNECT);     
#endif                                                   
      }
      break;

    case RSI_APP_EVENT_PINCODE_REQ:
      {
        //! pincode request event
        uint8_t *pin_code = PIN_CODE;

        //! clear the pincode request event.
        rsi_bt_app_clear_event(RSI_APP_EVENT_PINCODE_REQ);

        //! sending the pincode requet reply
        status = rsi_bt_pincode_request_reply((int8_t *)remote_dev_bd_addr, pin_code, 1);
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
#if (GAP_DM_LEP_BV_04_C || GAP_DM_LEP_BV_05_C || GAP_DM_LEP_BV_09_C || GAP_DM_LEP_BV_10_C)
        sleep(5);
        rsi_bt_disconnect (remote_dev_bd_addr);
#else
//        status =rsi_bt_a2dp_connect(remote_dev_bd_addr);
        status = rsi_bt_gatt_connect (remote_dev_bd_addr);
        if (status != RSI_SUCCESS) {
          LOG_PRINT ("status = %x", status);
        }
#endif
      }
      break;
    case RSI_APP_EVENT_DISCONNECTED:
      {
        //! remote device connected event
        uint8_t option = 0;

        //! clear the disconnected event.
        rsi_bt_app_clear_event (RSI_APP_EVENT_DISCONNECTED);
#if (GATT_CL_GAR_BI_34_C || GAP_DM_LEP_BV_10_C)
        break;
#endif

#if (ROLE == INITIATOR )
        LOG_PRINT ("\n enter option : 1.BT_connect 2.BLE_connect \n");
        LOG_SCAN  ("%d",&option);
 
        if( option == 1 ) {
          rsi_bt_app_set_event (RSI_APP_BT_CONNECT);
        } else if ( option == 2) {
#if (GATT_CL_GAC_BV_01_C || GATT_CL_GAD_BV_01_C || GATT_CL_GAD_BV_02_C || GATT_CL_GAD_BV_03_C || GATT_CL_GAD_BV_04_C \
    || GATT_CL_GAT_BV_01_C || GATT_CL_GAR_BI_13_C || GATT_CL_GAR_BI_18_C || GATT_CL_GAR_BI_19_C || GATT_CL_GAW_BI_09_C \
    || GATT_CL_GAW_BI_32_C || GATT_CL_GAW_BV_06_C || GATT_CL_GAD_BV_05_C || GATT_CL_GAD_BV_06_C || GATT_CL_GAR_BV_01_C \
    || GATT_CL_GAR_BI_01_C || GATT_CL_GAR_BI_02_C || GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_07_C \
    || GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BI_12_C || GATT_CL_GAR_BI_14_C || GATT_CL_GAR_BV_05_C || GATT_CL_GAR_BV_06_C \
    || GATT_CL_GAR_BV_07_C || GATT_CL_GAW_BV_01_C || GATT_CL_GAW_BV_03_C || GATT_CL_GAW_BI_02_C || GATT_CL_GAW_BI_03_C \
    || GATT_CL_GAW_BV_05_C || GATT_CL_GAW_BI_07_C || GATT_CL_GAW_BI_08_C || GATT_CL_GAW_BV_08_C || GATT_CL_GAW_BV_09_C \
    || GATT_CL_GAW_BI_33_C || GATT_CL_GAW_BI_34_C || GATT_CL_GAN_BV_01_C || GATT_CL_GAI_BV_01_C || GATT_CL_GAS_BV_01_C \
    || GATT_CL_GAT_BV_02_C)
          bt_done =1;
          intimate_ble();
#endif
        }
#else
#if (GAP_DM_BON_BV_01_C)
          bt_done =1;
          intimate_ble();
#endif
#endif
      }
      break;
    case RSI_APP_EVENT_LINKKEY_REQ:
      {
        //! linkkey request event

        //! clear the linkkey request event.
        rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_REQ);

        //! sending the linkkey request negative reply
        if((linkkey[0] != 0x00) && (linkkey[15] != 0x00)) {
          rsi_bt_linkkey_request_reply (remote_dev_bd_addr, linkkey, 1);
        }else {
          rsi_bt_linkkey_request_reply (remote_dev_bd_addr, NULL, 0);
        }
      }
      break;

    case RSI_APP_EVENT_SPP_CONN:
      {
        //! spp connected event

        //! clear the spp connected event.
        rsi_bt_app_clear_event (RSI_APP_EVENT_SPP_CONN);
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

        //! SPP data transfer (loop back)
        rsi_bt_spp_transfer (remote_dev_bd_addr, spp_data, spp_data_len);
      }
      break;
    case RSI_APP_EVENT_INQUIRY_COMPLETE:
      {
        //! Inquiry complete event

        //! clear the Inquiry complete event.
        rsi_bt_app_clear_event (RSI_APP_EVENT_INQUIRY_COMPLETE);

#if (GAP_DM_GIN_BV_01_C || GAP_DM_LIN_BV_01_C || GAP_DM_NAD_BV_01_C || GAP_DM_LEP_BV_04_C || GAP_DM_LEP_BV_05_C || GAP_DM_LEP_BV_09_C || GAP_DM_LEP_BV_10_C)
        if(!bt_device_found)
        {
          //! start the inquiry
#if (GAP_DM_LIN_BV_01_C )
          status = rsi_bt_inquiry( 0x81, 20000, 0);  /* doing limited discovery */
#else
          status = rsi_bt_inquiry( 2, 20000, 0);
#endif
          if(status != RSI_SUCCESS)
          {
            return status;
          }
        }
        else 
        {
#if (GAP_DM_NAD_BV_01_C)
          status = rsi_bt_remote_name_request_async(remote_dev_bd_addr, &remote_device_info);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
#elif (GAP_DM_LEP_BV_04_C || GAP_DM_LEP_BV_05_C || GAP_DM_LEP_BV_09_C || GAP_DM_LEP_BV_10_C)
          status = rsi_bt_connect(remote_dev_bd_addr);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
#else
          bt_device_found = 0;
          bt_done = 1;
          intimate_ble();
          break;
#endif
        }
#endif
      }
      break;
    case RSI_APP_EVENT_INQUIRY_RESP:
        {
          //! Inquiry resp event

          //! clear the Inquiry resp event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_INQUIRY_RESP);
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
					rsi_bt_accept_ssp_confirm(remote_dev_bd_addr);

        }
        break;

      case RSI_APP_BT_CONNECT:
        {
          //! clear the ssp confirmation event.
          rsi_bt_app_clear_event (RSI_APP_BT_CONNECT);
          status = rsi_bt_connect(remote_dev_bd_addr);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
        }
        break;

      case RSI_APP_BT_DISCONNECT:
        {
          //! clear the ssp confirmation event.
          rsi_bt_app_clear_event (RSI_APP_BT_DISCONNECT);
          status = rsi_bt_disconnect(remote_dev_bd_addr);
          if(status != RSI_SUCCESS)
          {
            return status;                                              
          }
        }
        break;


  }

  return 0;
}


