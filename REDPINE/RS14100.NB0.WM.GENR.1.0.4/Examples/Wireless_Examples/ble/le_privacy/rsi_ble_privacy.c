/**
 * @file    rsi_ble_privacy.c
 * @version 0.2
 * @date    27 April 2018
 *
 *  Copyright(C) Redpine Signals 2018
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BLE LL PRIVACY
 *
 *  @section Description this application executes ll privacy feature
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
#include<rsi_ble.h>

//! Common include file
#include<rsi_common_apis.h>
#include<stdio.h>
#include<string.h>

#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                                   printf 
#else 
#define LOG_PRINT
#endif

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME                      "BLE_SIMPLE_PRIVACY"

//! Memory length for the driver
#define BT_GLOBAL_BUFF_LEN                          10000

//! Memory to initialize driver
uint8_t bt_global_buf[BT_GLOBAL_BUFF_LEN];


#define RSI_SLAVE                                   0
#define RSI_MASTER                                  1

//!local device role
#define RSI_DEVICE_ROLE                             RSI_MASTER

//! local device name
#define RSI_BLE_DEVICE_NAME                        "BLE_SIMPLE_PRIVACY"

//! local device IO capability
#define  RSI_BLE_SMP_IO_CAPABILITY                  0x01
#define  RSI_BLE_SMP_PASSKEY                        0

//! application event list
#define RSI_APP_EVENT_ADV_REPORT       				      0x00
#define  RSI_BLE_CONN_EVENT                         0x01
#define  RSI_BLE_DISCONN_EVENT                      0x02
#define  RSI_BLE_SMP_REQ_EVENT                      0x03
#define  RSI_BLE_SMP_RESP_EVENT                     0x04
#define  RSI_BLE_SMP_PASSKEY_EVENT                  0x05
#define  RSI_BLE_SMP_FAILED_EVENT                   0x06
#define  RSI_BLE_ENCRYPT_STARTED_EVENT              0x07
#define  RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT          0x08
#define  RSI_BLE_SC_PASSKEY_EVENT                   0x09
#define  RSI_BLE_LTK_REQ_EVENT                      0x0A
#define  RSI_BLE_SECURITY_KEYS_EVENT                0x0B
#define  RSI_BLE_ENHANCE_CONNECTED_EVENT            0x0C

//! Address type of the device to connect
#define RSI_BLE_REMOTE_ADDR_TYPE                    LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_REMOTE_ADDR                          "00:23:A7:56:77:77"

//!address resolution enable
#define RSI_BLE_SET_RESOLVABLE_PRIV_ADDR_TOUT        120

//! process type 1-add device to resolvlist, 2-remove device from resolvlist, 3-clear the resolvlist
#define RSI_BLE_ADD_TO_RESOLVE_LIST                  1
#define RSI_BLE_REMOVE_FROM_RESOLVE_LIST             2
#define RSI_BLE_CLEAR_RESOLVE_LIST                   3

//! privacy mode 0-Network privacy mode 1-Device privacy mode
#define RSI_BLE_NETWORK_PRIVACY_MODE                0 
#define RSI_BLE_DEVICE_PRIVACY_MODE                 1

#define RSI_BLE_PRIVACY_MODE                        RSI_BLE_DEVICE_PRIVACY_MODE

#define RSI_BLE_RESOLVING_LIST_SIZE                 5

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE                      1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                        1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS                           1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                    2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE                  3000

void rsi_wireless_driver_task(void);

#endif

//! user defined structure
//LE resolvlist group.
typedef struct rsi_ble_resolvlist_group_s {
    uint8_t     used;
    uint8_t     remote_dev_addr_type;
    uint8_t     remote_dev_addr[18];
    uint8_t     peer_irk[16];
    uint8_t     local_irk[16];
    uint8_t  Identity_addr_type;
    uint8_t  Identity_addr[18];
} rsi_ble_resolvlist_group_t;

//LE resolvlist.
typedef struct rsi_ble_resolve_key_s {
  uint8_t  remote_dev_addr_type;
  uint8_t  remote_dev_addr[18];
  uint8_t  peer_irk[16];
  uint8_t  local_irk[16];
  uint16_t remote_ediv;
  uint8_t  remote_rand[16];
  uint8_t  remote_ltk[16];
  uint8_t  Identity_addr_type;
  uint8_t  Identity_addr[18];
} rsi_ble_resolve_key_t;

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint32_t passkey;
static uint8_t peer_irk[16] ;
static uint8_t local_irk[16] ;
static uint8_t dev_address_type;
static uint8_t str_remote_address[18];
static uint8_t resolv_str_remote_address[18];
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t device_found=0;
static uint8_t rsi_app_resp_resolvlist_size = 0;

static rsi_bt_event_encryption_enabled_t glbl_enc_enabled;
static rsi_bt_event_le_ltk_request_t temp_le_ltk_req;
static rsi_bt_event_le_security_keys_t temp_le_sec_keys;
static rsi_ble_event_enhance_conn_status_t rsi_app_enhance_connected_device = {0};
static rsi_ble_resolvlist_group_t  resolvlist[5];
static rsi_ble_resolve_key_t  resolve_key;


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
 * @brief      set the specific event.
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
 * @fn         rsi_ble_simple_central_on_adv_report_event
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
void rsi_ble_simple_central_on_adv_report_event(rsi_ble_event_adv_report_t *adv_report)
{

  if (device_found == 1) {
    return;
  }

  BT_LE_ADPacketExtract (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  if((device_found==0) && ((strcmp(remote_name, RSI_REMOTE_DEVICE_NAME))==0) ||
      ((remote_addr_type == RSI_BLE_REMOTE_ADDR_TYPE)&&((strcmp(remote_dev_addr,RSI_BLE_REMOTE_ADDR)==0))))
  { 
    device_found=1;
    rsi_ble_app_set_event (RSI_APP_EVENT_ADV_REPORT);
  }
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
  dev_address_type = resp_conn->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (str_remote_address, resp_conn->dev_addr);
  LOG_PRINT ("connect - str_remote_address : %s\r\n", str_remote_address);

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
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  rsi_6byte_dev_address_to_ascii (str_remote_address, resp_disconnect->dev_addr);
  LOG_PRINT ("connect - str_remote_address : %s\r\n", str_remote_address);

  rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_request 
 * @brief      its invoked when smp request event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP request events is received(we are in Master mode)
 * Note: slave requested to start SMP request, we have to send SMP request command
 */
void rsi_ble_on_smp_request (rsi_bt_event_smp_req_t  *remote_dev_address)
{
  rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr);
  LOG_PRINT ("smp_req - str_remote_address : %s\r\n", str_remote_address);
  rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_response 
 * @brief      its invoked when smp response event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP response events is received(we are in slave mode) 
 * Note: Master initiated SMP protocol, we have to send SMP response command
 */
void rsi_ble_on_smp_response (rsi_bt_event_smp_resp_t  *remote_dev_address)
{
  rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr);
  LOG_PRINT ("smp_resp - str_remote_address : %s\r\n", str_remote_address);
  rsi_ble_app_set_event (RSI_BLE_SMP_RESP_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_passkey 
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_smp_passkey (rsi_bt_event_smp_passkey_t  *remote_dev_address)
{
  rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr);
  LOG_PRINT ("smp_passkey - str_remote_address : %s\r\n", str_remote_address);
  rsi_ble_app_set_event (RSI_BLE_SMP_PASSKEY_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_passkey_display 
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_smp_passkey_display (rsi_bt_event_smp_passkey_display_t *smp_passkey_display)
{
  rsi_6byte_dev_address_to_ascii (str_remote_address, smp_passkey_display->dev_addr);
  LOG_PRINT ("remote addr: %s, passkey: %s \r\n", 
      str_remote_address, smp_passkey_display->passkey );
  rsi_ble_app_set_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);

}
/*==============================================*/
/**
 * @fn         rsi_ble_on_sc_passkey 
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_sc_passkey (rsi_bt_event_sc_passkey_t *sc_passkey)
{
  rsi_6byte_dev_address_to_ascii (str_remote_address, sc_passkey->dev_addr);
  LOG_PRINT ("remote addr: %s, passkey: %06d \r\n", 
      str_remote_address, sc_passkey->passkey );
  passkey = sc_passkey->passkey;
  rsi_ble_app_set_event (RSI_BLE_SC_PASSKEY_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_failed 
 * @brief      its invoked when smp failed event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP failed events is received
 */
void rsi_ble_on_smp_failed (uint16_t  status, rsi_bt_event_smp_failed_t  *remote_dev_address)
{
  rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr);
  LOG_PRINT ("smp_failed status: 0x%x, str_remote_address: %s\r\n", status, str_remote_address);
  rsi_ble_app_set_event (RSI_BLE_SMP_FAILED_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_encrypt_started 
 * @brief      its invoked when encryption started event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when encryption started events is received
 */
void rsi_ble_on_encrypt_started (uint16_t  status, rsi_bt_event_encryption_enabled_t *enc_enabled)
{
  LOG_PRINT ("start encrypt status: %d \r\n", status);
  memcpy (&glbl_enc_enabled, enc_enabled, sizeof (rsi_bt_event_encryption_enabled_t));
  rsi_ble_app_set_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_le_ltk_req_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_le_ltk_req_event(rsi_bt_event_le_ltk_request_t *le_ltk_req)
{
  LOG_PRINT (" \r\n rsi_ble_on_le_ltk_req_event \r\n");
  memcpy(&temp_le_ltk_req, le_ltk_req, sizeof(rsi_bt_event_le_ltk_request_t));
  rsi_ble_app_set_event (RSI_BLE_LTK_REQ_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_le_security_keys_event
 * @brief      invoked when security keys event is received
 * @param[in]  rsi_bt_event_le_security_keys_t, security keys information
 * @return     none.
 * @section description
 * This callback function indicates security keys information
 */
static void rsi_ble_on_le_security_keys_event(rsi_bt_event_le_security_keys_t *le_sec_keys)
{
  LOG_PRINT (" \r\n rsi_ble_on_le_security_keys_event \r\n");
  memcpy(&temp_le_sec_keys, le_sec_keys, sizeof(rsi_bt_event_le_security_keys_t));
  rsi_ble_app_set_event (RSI_BLE_SECURITY_KEYS_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_enh_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the enhanced connection
 */
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  memcpy(&rsi_app_enhance_connected_device, resp_enh_conn, sizeof(rsi_ble_event_enhance_conn_status_t));
  dev_address_type = resp_enh_conn->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (str_remote_address, resp_enh_conn->dev_addr);

  rsi_ble_app_set_event (RSI_BLE_ENHANCE_CONNECTED_EVENT);
}

/*==============================================*/
/**
 * @fn         add_device_to_resolvlist
 * @brief      this function will add device to resolvlistwith updated irks 
 * @param[out] ix, index of resolvlist
 * @return     none.
 * @section description
 * add device to resolvlistwith updated irks 
 * */
uint32_t add_device_to_resolvlist(rsi_ble_resolvlist_group_t *resolvlist_p, rsi_ble_resolve_key_t  *resolve_key_p)
{
  uint8_t status;
  uint8_t ix;

  for  (ix = 0; ix < RSI_BLE_RESOLVING_LIST_SIZE; ix++) {
    if (resolvlist_p[ix].used == 0) {
      resolvlist_p[ix].used = 1;
      resolvlist_p[ix].Identity_addr_type = resolve_key_p->Identity_addr_type;
      memcpy (resolvlist_p[ix].Identity_addr,resolve_key_p->Identity_addr,sizeof(resolve_key_p->Identity_addr));
      memcpy (resolvlist_p[ix].peer_irk,resolve_key_p->peer_irk,sizeof(resolve_key_p->peer_irk));
      memcpy (resolvlist_p[ix].local_irk,resolve_key_p->local_irk,sizeof(resolve_key_p->local_irk));
      break;
    }
  }
  if (ix >= RSI_BLE_RESOLVING_LIST_SIZE) {
    return -1;
  }

  //add device to resolvlist or remove from resolvlist or clear the resolvlist
  status = rsi_ble_resolvlist(RSI_BLE_ADD_TO_RESOLVE_LIST,  
      resolve_key_p->Identity_addr_type, 
      resolve_key_p->Identity_addr, 
      resolve_key_p->peer_irk ,
      resolve_key_p->local_irk );
  if(status != RSI_SUCCESS)
  {
    return status;
  } 
}

/*==============================================*/
/**
 * @fn         update_resolvlist
 * @brief      this function will update the resolvlist if different remote address or different irks for same address received
 * @param[out] none.
 * @return     none.
 * @section description
 * update the resolvlist if different remote address or different irks for same address received
 * */
void update_resolvlist(rsi_ble_resolvlist_group_t *resolvlist_p, rsi_ble_resolve_key_t  *resolve_key_p)
{
  uint8_t ix = 0;
  while(ix<RSI_BLE_RESOLVING_LIST_SIZE){
    if(!strcmp(resolvlist_p[ix].Identity_addr , resolve_key_p->Identity_addr)){
      if(memcmp(resolvlist_p[ix].peer_irk, resolve_key_p->peer_irk, sizeof(resolve_key_p->peer_irk)))
      {
        add_device_to_resolvlist(resolvlist_p, resolve_key_p);
        break;
      }
    }else{
      strcpy(resolvlist_p[ix].Identity_addr , resolve_key_p->Identity_addr);
      add_device_to_resolvlist(resolvlist_p, resolve_key_p);
      break;
    }
    ix++;
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_privacy_app
 * @brief      This application is used for testing the Privacy feature.
 * @param[in]  none. 
 * @return     none.
 * @section description
 * This function is used to test the LL Privacy Protocol.
 */
int32_t rsi_ble_privacy_app (void)
{
  int32_t status = 0 ,first_smp_req = 1;
  int32_t event_id;
  uint8_t adv[31] = {2,1,6},ix, first_connect=0;

#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(bt_global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! Redpine module intialisation 
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(
      rsi_ble_simple_central_on_adv_report_event,
      rsi_ble_on_connect_event,
      rsi_ble_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_on_enhance_conn_status_event,
      NULL,
      NULL);

  //! registering the SMP callback functions
  rsi_ble_smp_register_callbacks(rsi_ble_on_smp_request,
      rsi_ble_on_smp_response,
      rsi_ble_on_smp_passkey,
      rsi_ble_on_smp_failed,
      rsi_ble_on_encrypt_started,
      rsi_ble_on_smp_passkey_display,
      rsi_ble_on_sc_passkey,
      rsi_ble_on_le_ltk_req_event,
      rsi_ble_on_le_security_keys_event);

  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! Set local name
  status = rsi_bt_set_local_name(RSI_BLE_DEVICE_NAME);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#if(RSI_DEVICE_ROLE == RSI_SLAVE)
  //!preparing scan response data
  adv[3] = strlen (RSI_BLE_DEVICE_NAME) + 1;
  adv[4] = 9;
  strcpy (&adv[5], RSI_BLE_DEVICE_NAME);

  //!set scan response data
  status = rsi_ble_set_scan_response_data (adv, strlen (RSI_BLE_DEVICE_NAME) + 5);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! set advertise data
  status = rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_DEVICE_NAME) + 5);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! start the advertising
  status = rsi_ble_start_advertising();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#else
  status = rsi_ble_start_scanning();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#endif

  //! waiting for events from controller.
  while (1) {

    //! Application main loop
#ifndef RSI_WITH_OS
    rsi_wireless_driver_task();
#endif
    //! checking for events list
    event_id = rsi_ble_app_get_event ();

    if (event_id == -1) {
      continue;
    }

    switch (event_id) {
      case RSI_APP_EVENT_ADV_REPORT:
        {
          //! advertise report event.
          //! clear the advertise report event.
          rsi_ble_app_clear_event (RSI_APP_EVENT_ADV_REPORT);

          status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_addr);
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("connect status: 0x%X\r\n", status);
          }
        }
        break;
      case RSI_BLE_CONN_EVENT:
        {
          //! event invokes when connection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);

          status = rsi_ble_smp_pair_request (str_remote_address, RSI_BLE_SMP_IO_CAPABILITY);
        }
        break;

      case RSI_BLE_DISCONN_EVENT:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
          device_found=0;

#if(RSI_DEVICE_ROLE == RSI_MASTER)

          status = rsi_ble_connect(resolve_key.Identity_addr_type + 2, (int8_t *)resolve_key.Identity_addr);
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("connect status: 0x%X\r\n", status);
          }

#else
          //! start the advertising
          status = rsi_ble_start_advertising();
          if(status != RSI_SUCCESS)
          {
            return status;
          }
#endif
        }
        break;

      case RSI_BLE_SMP_REQ_EVENT:
        {

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_REQ_EVENT);
          if(first_connect == 0){
          //! initiating the SMP pairing process
          status = rsi_ble_smp_pair_request (str_remote_address, RSI_BLE_SMP_IO_CAPABILITY);
          }
        }
        break;

      case RSI_BLE_SMP_RESP_EVENT:
        {

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_RESP_EVENT);

          //! initiating the SMP pairing process
          status = rsi_ble_smp_pair_response (str_remote_address, RSI_BLE_SMP_IO_CAPABILITY);
        }
        break;

      case RSI_BLE_SMP_PASSKEY_EVENT:
        {

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_EVENT);

          //! initiating the SMP pairing process
          status = rsi_ble_smp_passkey (str_remote_address, RSI_BLE_SMP_PASSKEY);
        }
        break;
      case RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
        }
        break;
      case RSI_BLE_LTK_REQ_EVENT:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_LTK_REQ_EVENT);
          rsi_6byte_dev_address_to_ascii (str_remote_address,temp_le_ltk_req.dev_addr);

          if((temp_le_ltk_req.localediv == glbl_enc_enabled.localediv) && 
              !((memcmp(temp_le_ltk_req.localrand, glbl_enc_enabled.localrand,8)))
            )
          {
            LOG_PRINT("\n positive reply\n");
            //! give le ltk req reply cmd with positive reply
            status = rsi_ble_ltk_req_reply(str_remote_address,1,glbl_enc_enabled.localltk);
          }
          else
          {
            LOG_PRINT("\n negative reply\n");
            //! give le ltk req reply cmd with negative reply
            status = rsi_ble_ltk_req_reply(str_remote_address,0,NULL);
          }
        }
        break;

      case RSI_BLE_SC_PASSKEY_EVENT:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SC_PASSKEY_EVENT);
          rsi_ble_smp_passkey (str_remote_address, passkey);
        }
        break;

      case RSI_BLE_SECURITY_KEYS_EVENT:
        {
          //! event invokes when security keys are received
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SECURITY_KEYS_EVENT);
          rsi_6byte_dev_address_to_ascii (str_remote_address,temp_le_sec_keys.dev_addr);
          resolve_key.remote_dev_addr_type = dev_address_type; 
          
          resolve_key.remote_ediv = temp_le_sec_keys.remote_ediv; 
          memcpy(resolve_key.remote_rand,temp_le_sec_keys.remote_rand,sizeof(temp_le_sec_keys.remote_rand)); 
          memcpy(resolve_key.remote_ltk,temp_le_sec_keys.remote_ltk,sizeof(temp_le_sec_keys.remote_ltk)); 
          
          rsi_6byte_dev_address_to_ascii (resolve_key.remote_dev_addr,temp_le_sec_keys.dev_addr);
          memcpy(resolve_key.peer_irk,temp_le_sec_keys.remote_irk,sizeof(temp_le_sec_keys.remote_irk)); 
          memcpy(resolve_key.local_irk,temp_le_sec_keys.local_irk,sizeof(temp_le_sec_keys.local_irk)); 

          resolve_key.Identity_addr_type = temp_le_sec_keys.Identity_addr_type; 
          rsi_6byte_dev_address_to_ascii (resolve_key.Identity_addr,temp_le_sec_keys.Identity_addr);

          update_resolvlist ((rsi_ble_resolvlist_group_t *)&resolvlist, &resolve_key);

          //get resolvlist size
          status = rsi_ble_get_resolving_list_size(&rsi_app_resp_resolvlist_size);
          if(status != RSI_SUCCESS)
          {
            return status;
          }

          //set address resolution enable and resolvable private address timeout
          status = rsi_ble_set_addr_resolution_enable(RSI_BLE_DEV_ADDR_RESOLUTION_ENABLE,RSI_BLE_SET_RESOLVABLE_PRIV_ADDR_TOUT);
          if(status != RSI_SUCCESS)
          {
            return status;
          }

          //set privacy mode
          status = rsi_ble_set_privacy_mode(resolve_key.Identity_addr_type,resolve_key.Identity_addr,RSI_BLE_PRIVACY_MODE);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
          rsi_ble_update_directed_address(resolve_key.Identity_addr);

#if(RSI_DEVICE_ROLE == RSI_SLAVE)
          status = rsi_ble_disconnect (resolve_key.Identity_addr);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
#endif
        }
        break;

      case RSI_BLE_SMP_FAILED_EVENT:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_FAILED_EVENT);
        }
        break;

      case RSI_BLE_ENCRYPT_STARTED_EVENT:
        {
          //! start the encrypt event

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
        }
        break;
      case RSI_BLE_ENHANCE_CONNECTED_EVENT:
        {
          //! remote device enhnace connected event
          //! clear the enhance connected event.
          rsi_ble_app_clear_event (RSI_BLE_ENHANCE_CONNECTED_EVENT);
          if(first_connect == 0){
            first_connect = 1;
            status = rsi_ble_smp_pair_request (str_remote_address, RSI_BLE_SMP_IO_CAPABILITY);
          }
          else{
            status = rsi_ble_start_encryption (resolve_key.Identity_addr,resolve_key.remote_ediv ,resolve_key.remote_rand ,glbl_enc_enabled.localltk);
            if(status != RSI_SUCCESS)
            {
              return status;
            }
          }
#if(RSI_DEVICE_ROLE == RSI_SLAVE)
          if(first_smp_req){
            //! initiating the SMP pairing process
            status = rsi_ble_smp_pair_request (str_remote_address, RSI_BLE_SMP_IO_CAPABILITY);
            if(status != RSI_SUCCESS)
            {
              return status;
            }
            first_smp_req = 0;
          }
#endif

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
#ifdef RSI_WITH_OS
  int32_t status;
  rsi_task_handle_t bt_task_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;
#endif

#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

#ifndef RSI_WITH_OS

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);
  //! Call LL Privacy protocol test Application
  rsi_ble_privacy_app();

  //! Application main loop
  main_loop();

  return 0;
#endif
#ifdef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(bt_global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! Redpine module intialisation
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_ble_smp_test_app, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
