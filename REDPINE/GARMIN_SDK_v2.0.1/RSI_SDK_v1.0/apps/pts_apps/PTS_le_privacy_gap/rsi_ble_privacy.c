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

#include<rsi_pts_privacy_inc.h>

//! Common include file
#include<rsi_common_apis.h>
#include<stdio.h>
#include<string.h>

#if 1//def RSI_DEBUG_PRINTS
#define LOG_PRINT                                   printf 
#else 
#define LOG_PRINT
#endif

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME                      "BLE_SIMPLE_PRIVACY"

//! Memory length for the driver
#define BT_GLOBAL_BUFF_LEN                          10000

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];


#define RSI_SLAVE                                   0
#define RSI_MASTER                                  1

//!local device role
#if (GAP_BROB_BCST_BV_03_C || GAP_CONN_PRDA_BV_01_C || GAP_PRIV_CONN_BV_10_C || GAP_PRIV_CONN_BI_01_C || GAP_CONN_UCON_BV_06_C)
#define RSI_DEVICE_ROLE                             RSI_SLAVE //RSI_MASTER
#elif (GAP_BROB_OBSV_BV_05_C || GAP_BROB_OBSV_BV_06_C )
#define RSI_DEVICE_ROLE                             RSI_MASTER
#else
#define RSI_DEVICE_ROLE                             RSI_SLAVE //RSI_MASTER
#endif

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
//#define RSI_BLE_REMOTE_ADDR                          "00:23:A7:56:77:77"
#define RSI_BLE_REMOTE_ADDR                          "00:1B:DC:F2:1C:54"

//!address resolution enable
#define RSI_BLE_SET_RESOLVABLE_PRIV_ADDR_TOUT        60

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

#define MITM_REQ                                   0x01
#define RSI_MAX_LIST_SIZE                          0x05

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

//LE resolvlist.
typedef struct rsi_ble_dev_ltk_list_s {
  uint8_t  sc_enable;
  uint8_t  remote_dev_addr_type;
  uint8_t  remote_dev_addr[6];
  uint8_t  peer_irk[16];
  uint8_t  local_irk[16];
  uint16_t remote_ediv;
  uint16_t local_ediv;
  uint8_t  remote_rand[8];
  uint8_t localrand[8];
  uint8_t  remote_ltk[16];
  uint8_t  localltk[16];
  uint8_t  Identity_addr_type;
  uint8_t  Identity_addr[6];
  uint8_t  used;
} rsi_ble_dev_ltk_list_t;

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
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t remote_dev_bd_addr_1[6] = {0};
static uint8_t device_found=0;
static uint8_t rsi_app_resp_resolvlist_size = 0;

static rsi_bt_event_encryption_enabled_t glbl_enc_enabled;
static rsi_bt_event_le_ltk_request_t temp_le_ltk_req;
static rsi_bt_event_le_security_keys_t temp_le_sec_keys;
static rsi_ble_event_enhance_conn_status_t rsi_app_enhance_connected_device = {0};
static rsi_ble_resolvlist_group_t  resolvlist[5];
static rsi_ble_resolve_key_t  resolve_key;
rsi_ble_dev_ltk_list_t ble_dev_ltk_list[RSI_MAX_LIST_SIZE];

static uint8_t paired_already = 0;
static uint8_t enc_started_count = 0;
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
 * @fn         BT_LE_ADPacketExtractLocal
 * @brief      This function used to extract device name in advertise report data.
 * @param[in]  pbuf advertise data packet buffer pointer, buf_len buffer length
 * @return     none.
 * @section description
 * This function is used to extract the advertiseed packet.
 */
void BT_LE_ADPacketExtractLocal (uint8_t *remote_name,uint8_t *pbuf,uint8_t buf_len)
{
  uint8_t   adv_len;
  uint8_t   adv_type;
  uint8_t   ix;
  uint8_t   name_len;
  uint8_t   flags;

  ix = 0;

  while (ix < buf_len) {
    adv_len  = pbuf[ix];
    ix     += sizeof (uint8_t);
    adv_type = pbuf[ix];

    switch (adv_type) {
      case 0x01:
        {
          flags = pbuf[ix+1]; 
          LOG_PRINT (" \n ********** \n AD Data Flags =0X%x \n\n ********** \n",flags);
          if(flags & BIT(0))
          {
            LOG_PRINT ("\n LE limited discoverble mode");
          }
        }
        break;
      case 0x08:
      case 0x09:
        {
          name_len =adv_len - 1;
          memcpy (remote_name, &pbuf [ix + 1], name_len);
          remote_name[name_len] = 0;
          LOG_PRINT ("\n ********** \n Remote_dev name = %s \n\n ********** \n ",remote_name);
          return;
        }
        break;
      default:
        break;
    }

    ix += adv_len;
  }
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

  BT_LE_ADPacketExtractLocal (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  if((device_found==0) && ((strcmp(remote_name, RSI_REMOTE_DEVICE_NAME))==0) ||
      ((remote_addr_type == RSI_BLE_REMOTE_ADDR_TYPE)&&((strcmp(remote_dev_addr,RSI_BLE_REMOTE_ADDR)==0))))
  { 
    LOG_PRINT ("\n device found");
    LOG_PRINT ("\n adv report type %x: \n",adv_report->report_type);
#if (GAP_BROB_OBSV_BV_05_C )
    if (adv_report->report_type > 2)
    while(1);
#else
    device_found=1;
    memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
    rsi_ble_app_set_event (RSI_APP_EVENT_ADV_REPORT);
#endif
  }
  else if ((remote_addr_type == LE_RESOLVABLE_PUBLIC_ADDRESS)&&((strcmp(remote_dev_addr,RSI_BLE_REMOTE_ADDR)==0)))
  {
    LOG_PRINT ("\n device found");
    LOG_PRINT ("\n adv report type %x: \n",adv_report->report_type);
    while (1);
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
  LOG_PRINT ("Connected - str_remote_address : %s\r\n",
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
  LOG_PRINT ("Dis-connected - str_remote_address : %s\r\n",
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

  LOG_PRINT ("Connected - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii (str_remote_address, resp_enh_conn->dev_addr));
  rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
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

    if (ble_dev_ltk_list[ix].used == 0) 
    {
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
  uint8_t adv[31] = {2,1,0},ix, first_connect=0;
  uint8_t  adv_data[31] = {0x05, 0x03, 0x00, 0x18, 0x01, 0x18, 0x0D, 0x09, 0x50, 0x54, 0x53, 0x2D, 0x47, 0x41, 0x50, 0x2D, 0x30, 0x36, 0x42, 0x38, 0x03, 0x19, 0x00, 0x00};
  rsi_ble_dev_ltk_list_t *ble_dev_ltk = NULL;

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
#if (GAP_BROB_BCST_BV_03_C)

  rsi_ble_set_scan_response_data (adv_data, 24);
  
  rsi_ble_set_advertise_data (adv_data, 24);
#else
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
#endif
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

          status = rsi_ble_connect(remote_addr_type, remote_dev_bd_addr);
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

          if (!paired_already) {
            LOG_PRINT ("\n Initiating Security Request \n");
            status = rsi_ble_smp_pair_request (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
          } else {
#if (!(GAP_CONN_UCON_BV_06_C || GAP_CONN_PRDA_BV_01_C))
            rsi_ble_disconnect (remote_dev_bd_addr);
#endif
          }
        }
        break;

      case RSI_BLE_DISCONN_EVENT:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
          device_found=0;

#if(RSI_DEVICE_ROLE == RSI_MASTER)

          status = rsi_ble_start_scanning ();
#if 0          
          status = rsi_ble_connect(resolve_key.Identity_addr_type + 2, (int8_t *)resolve_key.Identity_addr);
#endif
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("connect status: 0x%X\r\n", status);
          }

#else
          //! set advertise data
          sleep(5);
          status = rsi_ble_set_advertise_data (adv_data, 24);
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("\n setting adv data failed \n Reason code : %x", status);
//            return status;
          }

#if 0
          status = rsi_ble_set_random_address(); 
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("\n setting random address failed \n Reason code : %x", status);
//            return status;
          }
#endif
          rsi_ble_req_adv_t adv_params = {0};
          adv_params.status = RSI_BLE_START_ADV;
#if (GAP_BROB_BCST_BV_03_C)
          adv_params.adv_type = UNDIR_NON_CONN;
#elif (GAP_PRIV_CONN_BI_01_C || GAP_PRIV_CONN_BV_10_C)
          adv_params.adv_type = UNDIR_CONN;
#else
          adv_params.adv_type = UNDIR_CONN;
#endif
          adv_params.filter_type = RSI_BLE_ADV_FILTER_TYPE;
          adv_params.direct_addr_type = LE_PUBLIC_ADDRESS;
  
          rsi_ascii_dev_address_to_6bytes_rev(adv_params.direct_addr,resolve_key.Identity_addr);
          //rsi_ascii_dev_address_to_6bytes_rev(adv_params.direct_addr,RSI_BLE_REMOTE_ADDR);
          adv_params.adv_int_min = RSI_BLE_ADV_INT_MIN;
          adv_params.adv_int_max = RSI_BLE_ADV_INT_MAX;
#if (GAP_CONN_UCON_BV_06_C || GAP_PRIV_CONN_BI_01_C || GAP_PRIV_CONN_BV_10_C)
          adv_params.own_addr_type = LE_RESOLVABLE_RANDOM_ADDRESS;
#else
          adv_params.own_addr_type = LE_PUBLIC_ADDRESS;
#endif
          adv_params.adv_channel_map = RSI_BLE_ADV_CHANNEL_MAP;

          status = rsi_ble_start_advertising_with_values(&adv_params);
          if(status != RSI_SUCCESS)
          {
            //return status;
            LOG_PRINT ("\n Start advertising failed \n Reason code : %x \n", status);
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
          //rsi_6byte_dev_address_to_ascii (str_remote_address,temp_le_ltk_req.dev_addr);
          ix =  rsi_get_ltk_list(ble_dev_ltk_list, &temp_le_ltk_req);

          ble_dev_ltk = &ble_dev_ltk_list[ix];

          if((temp_le_ltk_req.localediv == ble_dev_ltk->local_ediv) && 
              !((memcmp(temp_le_ltk_req.localrand, ble_dev_ltk->localrand,8)))
            )
          {
            LOG_PRINT("\n positive reply\n");
            //! give le ltk req reply cmd with positive reply
            //status = rsi_ble_ltk_req_reply(str_remote_address,(1 && (ble_dev_ltk->sc_enable << 7)), ble_dev_ltk->localltk);
            status = rsi_ble_ltk_req_reply(temp_le_ltk_req.dev_addr, (1), ble_dev_ltk->localltk);
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
          rsi_ascii_dev_address_to_6bytes_rev(remote_dev_bd_addr_1, resolve_key.Identity_addr);
          status = rsi_ble_set_privacy_mode(resolve_key.Identity_addr_type, remote_dev_bd_addr_1, RSI_BLE_PRIVACY_MODE);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
          rsi_ble_update_directed_address(remote_dev_bd_addr);

#if(RSI_DEVICE_ROLE == RSI_SLAVE)

          sleep(3);

          rsi_ascii_dev_address_to_6bytes_rev(remote_dev_bd_addr_1, resolve_key.Identity_addr);
          //status = rsi_ble_disconnect (resolve_key.Identity_addr);
          status = rsi_ble_disconnect (remote_dev_bd_addr);
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
         
          LOG_PRINT ("\n Updating the variable \n");
          paired_already = 1;  /* updated the global_varible */
          enc_started_count++;

          if (enc_started_count > 1) {

            sleep(3);
            //status = rsi_ble_disconnect (resolve_key.remote_dev_addr);
            rsi_ascii_dev_address_to_6bytes_rev(remote_dev_bd_addr_1, resolve_key.Identity_addr);
            status = rsi_ble_disconnect (remote_dev_bd_addr_1);
            if(status != RSI_SUCCESS) {
              LOG_PRINT ("\n Reason code : %x\n", status);
            }
          }

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
