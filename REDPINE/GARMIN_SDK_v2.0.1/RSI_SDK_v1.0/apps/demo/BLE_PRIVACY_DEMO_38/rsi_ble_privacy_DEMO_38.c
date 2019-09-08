/**
 * @file    rsi_ble_privacy_DEMO_38.c
 * @version 0.2
 * @date    22 Feb 2019
 *
 *  Copyright(C) Redpine Signals 2019
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
#include "rsi_common_app.h"
#if BLE_PRIVACY
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

//! Memory length for the driver
#define BT_GLOBAL_BUFF_LEN                          10000
#define  RSI_BLE_SMP_PASSKEY                        0x0
//! Memory to initialize driver
uint8_t bt_global_buf[BT_GLOBAL_BUFF_LEN];

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint32_t passkey;
static uint8_t peer_irk[16] ;
static uint8_t local_irk[16] ;
static uint8_t dev_address_type;
static uint8_t str_remote_address[18];
static uint8_t resolv_str_remote_address[6];
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;
static uint8_t rsi_app_resp_resolvlist_size = 0;

static rsi_bt_event_encryption_enabled_t glbl_enc_enabled;
static rsi_bt_event_le_ltk_request_t temp_le_ltk_req;
static rsi_bt_event_le_security_keys_t temp_le_sec_keys;
static rsi_ble_event_enhance_conn_status_t rsi_app_enhance_connected_device = {0};
static rsi_ble_resolvlist_group_t  resolvlist[5];
static rsi_ble_resolve_key_t  resolve_key;
rsi_ble_dev_ltk_list_t ble_dev_ltk_list[RSI_MAX_LIST_SIZE];

uint8_t add_device_to_ltk_key_list(rsi_ble_dev_ltk_list_t *ble_dev_ltk_list, rsi_bt_event_encryption_enabled_t *enc_enabled);
uint8_t add_security_keys_to_device_list(rsi_ble_dev_ltk_list_t *ble_dev_ltk_list, rsi_bt_event_le_security_keys_t *le_sec_keys);
uint8_t rsi_get_ltk_list(rsi_ble_dev_ltk_list_t *ble_dev_ltk_list, rsi_bt_event_le_ltk_request_t *le_ltk_req);

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
  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
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
  memcpy (remote_dev_bd_addr, resp_conn->dev_addr, 6);
  LOG_PRINT ("connect - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii (str_remote_address, resp_conn->dev_addr));

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
  memcpy (remote_dev_bd_addr, resp_disconnect->dev_addr, 6);
  LOG_PRINT ("disconnect - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii (str_remote_address, resp_disconnect->dev_addr));

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
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_req - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
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
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_resp - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
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
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_passkey - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
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
  memcpy (remote_dev_bd_addr, smp_passkey_display->dev_addr, 6);
  LOG_PRINT ("remote addr: %s, passkey: %s \r\n", 
      rsi_6byte_dev_address_to_ascii (str_remote_address, smp_passkey_display->dev_addr), 
      smp_passkey_display->passkey );
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
  memcpy (remote_dev_bd_addr, sc_passkey->dev_addr, 6);
  LOG_PRINT ("remote addr: %s, passkey: %06d \r\n", 
      rsi_6byte_dev_address_to_ascii (str_remote_address, sc_passkey->dev_addr),
      sc_passkey->passkey );
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
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_failed status: 0x%x, str_remote_address: %s\r\n", status,
      rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
  rsi_ble_app_set_event (RSI_BLE_SMP_FAILED_EVENT);
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
  add_security_keys_to_device_list(ble_dev_ltk_list, le_sec_keys);
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
  memcpy (remote_dev_bd_addr, resp_enh_conn->dev_addr, 6);
  LOG_PRINT ("Enhance connection done");


  rsi_ble_app_set_event (RSI_BLE_ENHANCE_CONNECTED_EVENT);
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
  add_device_to_ltk_key_list(ble_dev_ltk_list, enc_enabled);
  rsi_ble_app_set_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
}

/*==============================================*/
/**
 * @fn         add_device_to_ltk_key_list
 * @brief      this function will add device to the ltk key list
 * @param[in]  ltk device list pointer
 * @param[in]  encrypt start event pointer
 * @param[out] status
 * @section description
 * add device to ltk key list
 * */
uint8_t add_device_to_ltk_key_list(rsi_ble_dev_ltk_list_t *ble_dev_ltk_list, rsi_bt_event_encryption_enabled_t *enc_enabled)
{
  uint8_t status;
  uint8_t ix;

  for  (ix = 0; ix < RSI_MAX_LIST_SIZE; ix++) {

    if(ble_dev_ltk_list[ix].used == 1) 
    {
      if((enc_enabled->dev_addr_type > 1)
          && (!memcmp(enc_enabled->dev_addr,ble_dev_ltk_list[ix].Identity_addr,6)))
      {
        break;
      }
      
      if((enc_enabled->dev_addr_type <= 1)
          && (!memcmp(enc_enabled->dev_addr,ble_dev_ltk_list[ix].remote_dev_addr,6)))
      {
        break;
      }
    }

    if (ble_dev_ltk_list[ix].used == 0) {
      ble_dev_ltk_list[ix].used = 1;
      ble_dev_ltk_list[ix].sc_enable = enc_enabled->sc_enable;
      memcpy (ble_dev_ltk_list[ix].remote_dev_addr,enc_enabled->dev_addr, 6);
      memcpy (ble_dev_ltk_list[ix].localltk,enc_enabled->localltk,16);
      memcpy (ble_dev_ltk_list[ix].localrand,enc_enabled->localrand, 8);
      ble_dev_ltk_list[ix].local_ediv = enc_enabled->localediv;
      break;
    }
  }
  if (ix >= RSI_MAX_LIST_SIZE) {
    return -1;
  }
  return status;
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
uint8_t add_security_keys_to_device_list(rsi_ble_dev_ltk_list_t *ble_dev_ltk_list, rsi_bt_event_le_security_keys_t *le_sec_keys)
{
  uint8_t status;
  uint8_t ix;
  for  (ix = 0; ix < RSI_MAX_LIST_SIZE; ix++) {

    if ((ble_dev_ltk_list[ix].used == 1) 
        && (!memcmp(ble_dev_ltk_list[ix].remote_dev_addr, le_sec_keys->dev_addr,6))
        && (ble_dev_ltk_list[ix].remote_dev_addr_type == le_sec_keys->dev_addr_type)) {
      memcpy (ble_dev_ltk_list[ix].local_irk,le_sec_keys->local_irk, 16);
      memcpy (ble_dev_ltk_list[ix].peer_irk,le_sec_keys->remote_irk,16);
      memcpy (ble_dev_ltk_list[ix].remote_rand,le_sec_keys->remote_rand, 8);
      memcpy (ble_dev_ltk_list[ix].remote_ltk,le_sec_keys->remote_ltk, 16);
      memcpy (ble_dev_ltk_list[ix].Identity_addr,le_sec_keys->Identity_addr, 6);
      ble_dev_ltk_list[ix].remote_ediv = le_sec_keys->remote_ediv;
      ble_dev_ltk_list[ix].Identity_addr_type = le_sec_keys->Identity_addr_type;
      break;
    }
  }
  if (ix >= RSI_MAX_LIST_SIZE) {
    return -1;
  }

  return status;

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
      temp_le_sec_keys.Identity_addr, 
      resolve_key_p->peer_irk ,
      resolve_key_p->local_irk );
  if(status != RSI_SUCCESS)
  {
    return status;
  } 
}

uint8_t rsi_get_ltk_list(rsi_ble_dev_ltk_list_t *ble_dev_ltk_list, rsi_bt_event_le_ltk_request_t *le_ltk_req)
{
  uint8_t status;
  uint8_t ix;

  for  (ix = 0; ix < RSI_MAX_LIST_SIZE; ix++) {

    if(ble_dev_ltk_list[ix].used == 1) 
    {
      if((le_ltk_req->dev_addr_type > 1)
          && (!(memcmp(le_ltk_req->dev_addr,ble_dev_ltk_list[ix].Identity_addr,6))))
      {
        return ix;
      }
      
      if((le_ltk_req->dev_addr_type <= 1)
          && (!(memcmp(le_ltk_req->dev_addr,ble_dev_ltk_list[ix].remote_dev_addr,6))))
      {
        return ix;
      }
    }
  }
  return -1;
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
 * @fn         rsi_ble_dual_role
 * @brief      BLE initialization .
 * @param[in]  none. 
 * @return     none.
 * @section description
 * This function is used to initialize the BLE.
 */
int32_t rsi_ble_dual_role(void){
	  int32_t status = 0 ,first_smp_req = 1;
	  int32_t event_id;
	  uint8_t adv[31] = {2,1,6},ix, first_connect=0;
	  rsi_ble_dev_ltk_list_t *ble_dev_ltk = NULL;

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

#if ENABLE_POWER_SAVE
	//! initiating power save in BLE mode
	status = rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif
	return 0;
}

int32_t rsi_ble_app_task(){

	 int32_t status = 0 ,first_smp_req = 1;
	 uint8_t ix, first_connect=0;
     int32_t temp_event_map = 0;
	 rsi_ble_dev_ltk_list_t *ble_dev_ltk = NULL;
	 //! checking for received events
		temp_event_map = rsi_ble_app_get_event();
		if (temp_event_map == RSI_FAILURE) {
			//! if events are not received loop will be continued.
#ifndef RSI_WITH_OS
			return 0;
#endif
		}

	    switch (temp_event_map) {
	      case RSI_APP_EVENT_ADV_REPORT:
	        {
	          //! advertise report event.
	          //! clear the advertise report event.
	          rsi_ble_app_clear_event (RSI_APP_EVENT_ADV_REPORT);

	          status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_bd_addr);
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

	          status = rsi_ble_smp_pair_request (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
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
	          status = rsi_ble_smp_pair_request (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
	          }
	        }
	        break;

	      case RSI_BLE_SMP_RESP_EVENT:
	        {

	          //! clear the served event
	          rsi_ble_app_clear_event (RSI_BLE_SMP_RESP_EVENT);

	          //! initiating the SMP pairing process
	          status = rsi_ble_smp_pair_response (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
	        }
	        break;

	      case RSI_BLE_SMP_PASSKEY_EVENT:
	        {

	          //! clear the served event
	          rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_EVENT);

	          //! initiating the SMP pairing process
	          status = rsi_ble_smp_passkey (remote_dev_bd_addr, RSI_BLE_SMP_PASSKEY);
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

	          ix =  rsi_get_ltk_list(ble_dev_ltk_list, &temp_le_ltk_req);

	          ble_dev_ltk = &ble_dev_ltk_list[ix];

	          if((temp_le_ltk_req.localediv == ble_dev_ltk->local_ediv) &&
	              !((memcmp(temp_le_ltk_req.localrand, ble_dev_ltk->localrand,8)))
	            )
	          {
	            LOG_PRINT("\n positive reply\n");
	            //! give le ltk req reply cmd with positive reply
	            status = rsi_ble_ltk_req_reply(temp_le_ltk_req.dev_addr, (1 && (ble_dev_ltk->sc_enable << 7)), ble_dev_ltk->localltk);
	          }
	          else
	          {
	            LOG_PRINT("\n negative reply\n");
	            //! give le ltk req reply cmd with negative reply
	            status = rsi_ble_ltk_req_reply(temp_le_ltk_req.dev_addr, 0, NULL);
	          }
	        }
	        break;

	      case RSI_BLE_SC_PASSKEY_EVENT:
	        {
	          //! clear the served event
	          rsi_ble_app_clear_event (RSI_BLE_SC_PASSKEY_EVENT);
	          rsi_ble_smp_passkey (remote_dev_bd_addr, passkey);
	        }
	        break;

	      case RSI_BLE_SECURITY_KEYS_EVENT:
	        {
	          //! event invokes when security keys are received
	          //! clear the served event
	          rsi_ble_app_clear_event (RSI_BLE_SECURITY_KEYS_EVENT);
	          //rsi_6byte_dev_address_to_ascii (str_remote_address,temp_le_sec_keys.dev_addr);
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
	          status = rsi_ble_set_privacy_mode(resolve_key.Identity_addr_type, temp_le_sec_keys.Identity_addr, RSI_BLE_PRIVACY_MODE);
	          if(status != RSI_SUCCESS)
	          {
	            return status;
	          }
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

	        }
	        break;
	    }
		return 0;
}

#endif
