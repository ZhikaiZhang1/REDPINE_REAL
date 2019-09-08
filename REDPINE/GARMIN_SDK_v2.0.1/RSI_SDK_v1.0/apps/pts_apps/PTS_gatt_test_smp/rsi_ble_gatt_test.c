/**
 * @file    rsi_ble_gatt_test.c
 * @version 0.1
 * @date    09 Sep 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application to test the GATT client API's
 *
 *  @section Description  This application has covered most of the GATT client API's
 */

/**
 * Include files
 * */

//! BLE include files to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_ble.h>
#include<string.h>
#include<stdio.h>
#include<rsi_pts_smp_inc.h>

//! Common include file 
#include<rsi_common_apis.h>
#define   RSI_DEBUG_PRINTS
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#define LOG_SCAN                      scanf
#else 
#define LOG_PRINT
#define LOG_SCAN                      
#endif
//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN   10000

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! Devide role
#define MASTER    0
#define SLAVE     1

#if (SM_SLA_PROT_BV_02_C || SM_SLA_JW_BV_02_C || SM_SLA_JW_BI_02_C || SM_SLA_JW_BI_03_C || SM_SLA_PKE_BV_02_C \
    || SM_SLA_PKE_BV_05_C || SM_SLA_PKE_BI_03_C || SM_SLA_OOB_BV_06_C || SM_SLA_OOB_BV_08_C || SM_SLA_EKS_BI_02_C \
    || SM_SLA_KDU_BV_01_C || SM_SLA_KDU_BV_02_C || SM_SLA_KDU_BV_03_C || SM_SLA_KDU_BV_07_C  || SM_SLA_KDU_BI_01_C \
    || SM_SLA_SIP_BV_01_C || SM_SLA_SIE_BV_01_C || SM_SLA_EKS_BV_02_C)
#define ROLE      SLAVE
#elif ( SM_MAS_PROT_BV_01_C || SM_MAS_JW_BV_05_C || SM_MAS_JW_BI_01_C || SM_MAS_JW_BI_04_C || SM_MAS_JW_BV_01_C \
    || SM_MAS_PKE_BV_01_C || SM_MAS_PKE_BI_01_C || SM_MAS_PKE_BV_04_C || SM_MAS_PKE_BI_02_C || SM_MAS_OOB_BV_05_C \
    || SM_MAS_OOB_BV_07_C || SM_MAS_EKS_BV_01_C || SM_MAS_EKS_BI_01_C || SM_MAS_KDU_BV_04_C || SM_MAS_KDU_BV_05_C \
    || SM_MAS_KDU_BV_06_C || SM_MAS_KDU_BI_01_C || SM_MAS_SIP_BV_02_C )
#define ROLE      MASTER
#else
#define ROLE      MASTER //SLAVE
#endif
//! maximum advertise list
#define  NO_OF_ADV_REPORTS  5
#define  RSI_BLE_DEV_ADDR_TYPE                           LE_PUBLIC_ADDRESS 
#define  RSI_BLE_DEV_ADDR                               "00:1B:DC:F2:1C:54"
//#define RSI_BLE_DEV_ADDR                               "00:1B:DC:07:2C:F0"

//! local device IO capability
#define  RSI_BLE_SMP_IO_CAPABILITY      0x00
#define  RSI_BLE_SMP_PASSKEY            0

#if (SM_MAS_JW_BV_01_C || SM_MAS_PKE_BV_01_C || SM_MAS_PKE_BI_01_C || SM_MAS_PKE_BV_04_C || SM_MAS_PKE_BI_02_C \
    || SM_SLA_PKE_BV_02_C || SM_SLA_SIP_BV_01_C || SM_SLA_SIE_BV_01_C || SM_MAS_SIP_BV_02_C || SM_SLA_PROT_BV_02_C)
#define  MITM_REQ                       0
#elif (SM_SLA_PKE_BV_05_C || SM_SLA_PKE_BI_03_C)
#define  MITM_REQ                       1
#else
#define  MITM_REQ                       1
#endif

/*=========================================================================*/
//! SMP parameters
/*=========================================================================*/

#define AUTH_REQ                                0x09 /*default bonding and SC enable*/ // 0x01 for only bonding
#define OOB_DATA_FLAG                           0x00
#define ENC_KEY_SIZE                            0x10
#define INI_KEY_DIST                            0x07 /*default EncKey, IdKey and SignKey */ // BIT(0)-> EncKey // BIT(1) -> IdKey // BIT(2) -> SignKey // BTI(3) -> LinkKey
#if (SM_SLA_KDU_BV_07_C)
#define RSP_KEY_DIST                            0x01 /*default EncKey, IdKey and SignKey */ // BIT(0)-> EncKey // BIT(1) -> IdKey // BIT(2) -> SignKey // BTI(3) -> LinkKey
#elif (SM_SLA_KDU_BV_02_C)
#define RSP_KEY_DIST                            0x02 /*default EncKey, IdKey and SignKey */ // BIT(0)-> EncKey // BIT(1) -> IdKey // BIT(2) -> SignKey // BTI(3) -> LinkKey
#elif (SM_SLA_KDU_BV_03_C)
#define RSP_KEY_DIST                            0x04 /*default EncKey, IdKey and SignKey */ // BIT(0)-> EncKey // BIT(1) -> IdKey // BIT(2) -> SignKey // BTI(3) -> LinkKey
#else
#define RSP_KEY_DIST                            0x07 /*default EncKey, IdKey and SignKey */ // BIT(0)-> EncKey // BIT(1) -> IdKey // BIT(2) -> SignKey // BTI(3) -> LinkKey
#endif



//! local device name
#define RSI_BLE_APP_GATT_TEST  "RSI_BLE_GATT_TEST_SM"

//! application events list
#define  RSI_BLE_ADV_REPORT_EVENT     0x01
#define  RSI_BLE_CONN_EVENT           0x02
#define  RSI_BLE_DISCONN_EVENT        0x03
#define  RSI_BLE_GATT_WRITE_EVENT     0x04
#define  RSI_BLE_GATT_PROFILES        0x05
#define  RSI_BLE_GATT_PROFILE         0x06
#define  RSI_BLE_GATT_CHAR_SERVICES   0x07
#define  RSI_BLE_GATT_INC_SERVICES    0x08
#define  RSI_BLE_GATT_DESC_SERVICES   0x09
#define  RSI_BLE_GATT_READ            0x0A
#define  RSI_BLE_GATT_WRITE           0x0B
#define  RSI_BLE_GATT_ERROR           0x0C
#define  RSI_BLE_GATT_DESC_VAL        0x0D

#define  RSI_BLE_SMP_REQ_EVENT          0x0E
#define  RSI_BLE_SMP_RESP_EVENT         0x10
#define  RSI_BLE_SMP_PASSKEY_EVENT      0x11
#define  RSI_BLE_SMP_FAILED_EVENT       0x12
#define  RSI_BLE_ENCRYPT_STARTED_EVENT  0x13
#define  RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT 0x14
#define  RSI_BLE_LTK_REQ_EVENT          0x15
#define  RSI_BLE_SC_PASSKEY_EVENT       0x16
#define  RSI_BLE_GATT_LONG_WRITE        0x17

#define  RSI_BLE_DISCONN                0x0F

#define  RSI_BLE_ATTRIBUTE_3_128UUID    { \
                                          {0x33,0x22,0x11,0x00}, \
                                          {0x55,0x44}, \
                                          {0x77,0x66}, \
                                          {0x99,0x88,0xbb,0xaa,0xff,0xee,0xdd,0xcc} \
                                        }

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

//! application global variables list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;
static app_num_scan_list;
static rsi_ble_event_adv_report_t app_scan_list[NO_OF_ADV_REPORTS];
static rsi_ble_event_conn_status_t app_conn_event;
static rsi_ble_event_disconnect_t app_disconn_event;
static rsi_ble_event_write_t app_ble_write_event;
static rsi_ble_resp_profiles_list_t  ble_profiles;
static rsi_ble_resp_query_profile_descriptor_t  ble_profile;
static rsi_ble_resp_char_services_t ble_char_services;
static rsi_ble_resp_inc_services_t  ble_inc_serv;
static rsi_ble_resp_att_descs_t  ble_desc_services;
static rsi_ble_resp_att_value_t  ble_read_val;
static rsi_bt_event_encryption_enabled_t glbl_enc_enabled;
static rsi_bt_event_le_ltk_request_t temp_le_ltk_req;
static rsi_bt_event_le_security_keys_t app_ble_sec_keys;
static uint32_t  numeric_value;
static uint8_t already_paired = 0;
static uint32_t smp_passkey=0;
uint16_t last_handle = 1;
rsi_ble_set_smp_pairing_capabilty_data_t  smp_pairing_cap_data;

/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events(void)
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
 * @brief      clear the specific event.
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
static int32_t rsi_ble_app_get_event()
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
 * @fn         rsi_ble_on_adv_reports
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
static void rsi_ble_on_adv_reports (rsi_ble_event_adv_report_t *adv_report)
{
  if (device_found == 1) {
    return;
  }

  BT_LE_ADPacketExtract (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);

  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, RSI_DEV_ADDR_LEN);
  
  LOG_PRINT("\nreport_type = %d",adv_report->report_type);
  
  if((device_found==0) && ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)&&((strcmp(remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  {
    device_found=1;
    rsi_ble_app_set_event (RSI_BLE_ADV_REPORT_EVENT);
  }
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
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n smp_req - remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, remote_dev_address->dev_addr));

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
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n smp_resp - remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, remote_dev_address->dev_addr));

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
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n smp_passkey - remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, remote_dev_address->dev_addr));

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
	LOG_PRINT ("\n remote_dev_address : %s - \t passkey display: %06s \n\n", 
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, smp_passkey_display->dev_addr),
        smp_passkey_display->passkey);

  rsi_ble_app_set_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
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
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n smp_failed_status: 0x%x,  remote_dev_addr : %s\r\n", status,
      rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, remote_dev_address->dev_addr));
  
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
  memcpy(&glbl_enc_enabled, enc_enabled, sizeof(rsi_bt_event_encryption_enabled_t));
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
 * @fn         rsi_ble_on_le_security_keys
 * @brief      its invoked when le secure keys events is received.
 * @param[in]  rsi_ble_event_le_security_keys, secure event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when security keys event is received after encription started. 
 */
void rsi_ble_on_le_security_keys  (rsi_bt_event_le_security_keys_t *rsi_ble_event_le_security_keys)
{
  already_paired = 1;
  memcpy (&app_ble_sec_keys, rsi_ble_event_le_security_keys, sizeof (rsi_bt_event_le_security_keys_t));
  LOG_PRINT ("remote_ediv: 0x%x\r\n", app_ble_sec_keys.remote_ediv);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_sc_passkey 
 * @brief      its invoked when sc passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SC passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_sc_passkey (rsi_bt_event_sc_passkey_t *sc_passkey)
{
  memcpy (remote_dev_bd_addr, sc_passkey->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n remote_dev_addr : %s\t \t passkey: %06d \r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, sc_passkey->dev_addr), sc_passkey->passkey);

  numeric_value = sc_passkey->passkey;
  rsi_ble_app_set_event (RSI_BLE_SC_PASSKEY_EVENT);
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
	app_conn_event.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (app_conn_event.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  app_conn_event.status = resp_enh_conn->status;
  LOG_PRINT ("rsi_ble_on_enhance_conn_status_event \r\n");
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
	memcpy(&app_conn_event, resp_conn, sizeof(rsi_ble_event_conn_status_t));
  LOG_PRINT ("rsi_ble_on_connect_event\r\n");
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
	memcpy(&app_disconn_event, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
  LOG_PRINT ("rsi_ble_on_disconnect_event \r\n");
	rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      this is call back function, it invokes when write/notify events received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This is a callback function
 */
static void rsi_ble_on_gatt_write_event (uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{
  memcpy (&app_ble_write_event, rsi_ble_write, sizeof (rsi_ble_event_write_t));
  LOG_PRINT (" write event id = %x \n",event_id);
  rsi_ble_app_set_event (RSI_BLE_GATT_WRITE_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_profiles_list_event
 * @brief      invoked when response is received for get list of services.
 * @param[out] p_ble_resp_profiles, profiles list details
 * @return     none
 * @section description
 */
static void rsi_ble_profiles_list (uint16_t resp_status, rsi_ble_resp_profiles_list_t  *rsi_ble_resp_profiles)
{
  uint8_t ix;

  LOG_PRINT("ble_profile_list resp_status %x \n\n",resp_status);
  if(resp_status != RSI_SUCCESS) {
    LOG_PRINT(" error = %x \n\n",resp_status);
  }else {
    memcpy(&ble_profiles, rsi_ble_resp_profiles, sizeof(rsi_ble_resp_profiles_list_t));
    LOG_PRINT("no of profiles = %d \n",ble_profiles.number_of_profiles);
    LOG_PRINT("reserved value = %d \n",*(uint32_t *)ble_profiles.reserved);

    for(ix=0;ix < ble_profiles.number_of_profiles; ix++)
    {
      LOG_PRINT ("profile uuid size = 0X%x \n",ble_profiles.profile_desc[ix].profile_uuid.size);
      LOG_PRINT ("profile uuid = 0X%x \n",ble_profiles.profile_desc[ix].profile_uuid.val);
      LOG_PRINT ("profile start handle = 0X%x \n",*(uint16_t *)ble_profiles.profile_desc[ix].start_handle);
      LOG_PRINT ("profile end handle = 0X%x \n\n",*(uint16_t *)ble_profiles.profile_desc[ix].end_handle);
    }
    rsi_ble_app_set_event (RSI_BLE_GATT_PROFILES);
  }

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_profile_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] rsi_ble_event_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_profile (uint16_t resp_status, profile_descriptors_t *rsi_ble_resp_profile)
{
  LOG_PRINT("ble_profile resp_status %x \n\n",resp_status);
  if(resp_status != RSI_SUCCESS) {
    LOG_PRINT("\n error = %x \n\n",resp_status);
  }else {
    LOG_PRINT ("profile uuid = 0X%x \n",rsi_ble_resp_profile->profile_uuid.val);
    LOG_PRINT ("profile start handle = 0X%x \n",*(uint16_t *)rsi_ble_resp_profile->start_handle);
    LOG_PRINT ("profile end handle = 0X%x \n\n",*(uint16_t *)rsi_ble_resp_profile->end_handle);
    rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
  }
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_char_services_event
 * @brief      invoked when response is received for characteristic services details
 * @param[out] rsi_ble_event_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_char_services (uint16_t resp_status, rsi_ble_resp_char_services_t *rsi_ble_resp_char_services)
{
  uint8_t ix;
  LOG_PRINT(" \n ble_char services resp_status %x",resp_status);
  if(resp_status != RSI_SUCCESS) {
  }else {
    memcpy(&ble_char_services, rsi_ble_resp_char_services, sizeof(rsi_ble_resp_char_services_t));
    LOG_PRINT("\n ble_char number of services : 0X%x",ble_char_services.num_of_services);
    for(ix=0; ix<ble_char_services.num_of_services;ix++) {
      LOG_PRINT ("\n char serv handle : 0X%x",(uint16_t )ble_char_services.char_services[ix].handle);
      LOG_PRINT ("\n char handle : 0X%x", (uint16_t )ble_char_services.char_services[ix].char_data.char_handle);
      LOG_PRINT ("\n char properties : 0x%x",(uint8_t )ble_char_services.char_services[ix].char_data.char_property);
      //      if(ble_char_services.char_services[ix].char_data.char_property & (1 << 1)) // checking read property
      //      {
      //        LOG_PRINT("\n\n ************* \n\n found read property \n\n ***************** \n\n");
      //        rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
      //        return;
      //      }

    }
    rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
  }
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_inc_services_event
 * @brief      invoked when response is received for the include services details
 * @param[out] rsi_ble_event_inc_services, list of include  services.
 * @return     none
 * @section description
 */
static void rsi_ble_inc_services (uint16_t resp_status, rsi_ble_resp_inc_services_t *rsi_ble_resp_inc_services)
{
  uint8_t ix;
  LOG_PRINT ("\n ble inc service resp =%x",resp_status);
  if(resp_status != RSI_SUCCESS) {
    LOG_PRINT ("\n error = %x",resp_status);
  }else {
    LOG_PRINT ("\n no.of include services = %d",rsi_ble_resp_inc_services->num_of_services );
    for(ix=0;ix<rsi_ble_resp_inc_services->num_of_services; ix++)
    {
      LOG_PRINT ("\n inc service handle = %x",rsi_ble_resp_inc_services->services[ix].handle);
      LOG_PRINT ("\n inc service handle uuid  = %x",rsi_ble_resp_inc_services->services[ix].inc_serv.uuid.val);
      LOG_PRINT ("\n inc service start handle = %x",rsi_ble_resp_inc_services->services[ix].inc_serv.start_handle);
      LOG_PRINT ("\n inc service end handle   = %x",rsi_ble_resp_inc_services->services[ix].inc_serv.end_handle);
    }
    rsi_ble_app_set_event (RSI_BLE_GATT_INC_SERVICES);
  }
    return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_att_desc_event
 * @brief      invoked when response is received for the characteristic descriptors details
 * @param[out] rsi_ble_event_att_desc, list of descriptors.
 * @return     none
 * @section description
 */
static  void rsi_ble_on_att_desc (uint16_t resp_status, rsi_ble_resp_att_descs_t *rsi_ble_resp_att_desc)
{

  LOG_PRINT("\n ble_on_att_desc resp_status %x",resp_status);
  if(resp_status != RSI_SUCCESS) {
    LOG_PRINT("\n error = %x",resp_status);
  }else {
    rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
  }
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_read_event
 * @brief      invoked when response is received for get attribute value
 * @param[out] resp_id, received response id.
 * @param[out] rsi_ble_event_att_val, attribute value details
 * @return     none
 * @section description
 */
static void rsi_ble_on_read (uint16_t resp_status, uint16_t resp_id, rsi_ble_resp_att_value_t *rsi_ble_resp_att_val)
{
  LOG_PRINT("\nble_on_read resp_status %x",resp_status);
  if(resp_status != RSI_SUCCESS) {
    LOG_PRINT ("\n err = %x", resp_status); 
  }else { 
    memcpy(&ble_read_val, rsi_ble_resp_att_val, sizeof(rsi_ble_resp_att_value_t));
    rsi_ble_app_set_event (RSI_BLE_GATT_READ);
  }
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_write_event
 * @brief      this function will invoke when set the attribute value complete
 * @param[out] resp_id, response id
 * @param[out] status, status of the response
 * @return     none
 * @section description
 */
static void rsi_ble_write (uint16_t resp_status, uint16_t resp_id)
{
  LOG_PRINT (" \n ble write rsp_id = %x",resp_id);

  rsi_ble_app_set_event (RSI_BLE_GATT_WRITE);
  return;
}
/*==============================================*/
/**
 * @fn         rsi_ble_gatt_error_event
 * @brief      this function will invoke when set the attribute value complete
 * @param[out] rsi_ble_gatt_error, event structure
 * @param[out] status, status of the response
 * @return     none
 * @section description
 */
static void rsi_ble_gatt_error_event (uint16_t resp_status, rsi_ble_event_error_resp_t *rsi_ble_gatt_error)
{
  rsi_ble_app_set_event (RSI_BLE_GATT_ERROR);
}
/*==============================================*/
/**
 * @fn         rsi_ble_gatt_error_event
 * @brief      this function will invoke when set the attribute value complete
 * @param[out] rsi_ble_gatt_desc_val, event structure
 * @param[out] status, status of the response
 * @return     none
 * @section description
 */
static void rsi_ble_gatt_desc_val_event (uint16_t resp_status, rsi_ble_event_gatt_desc_t *rsi_ble_gatt_desc_val)
{
  rsi_ble_app_set_event (RSI_BLE_GATT_DESC_VAL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_gatt_test
 * @brief      this is the application of ble GATT client api's test cases.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used at application.
 */
int32_t rsi_ble_simple_gatt_test (void)
{
  int32_t status = 0;
  int32_t  event_id;
  int8_t   data[20] = {0};
  uuid_t   search_serv = {0};
  uint8_t loop;
  uint8_t adv_data[31] = {2,1,2};

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

  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! registering the GAP call back functions
  rsi_ble_gap_register_callbacks(
      rsi_ble_on_adv_reports,
      rsi_ble_on_connect_event,
      rsi_ble_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_on_enhance_conn_status_event,
      NULL,
      NULL);

  //! registering the SMP callback functions
  rsi_ble_smp_register_callbacks(
      rsi_ble_on_smp_request,
      rsi_ble_on_smp_response,
      rsi_ble_on_smp_passkey,
      rsi_ble_on_smp_failed,
      rsi_ble_on_encrypt_started,
      rsi_ble_on_smp_passkey_display,
      rsi_ble_on_sc_passkey,
      rsi_ble_on_le_ltk_req_event,
      rsi_ble_on_le_security_keys);

  //! registering the GATT call back functions
  rsi_ble_gatt_register_callbacks (rsi_ble_profiles_list,
      rsi_ble_profile,
      rsi_ble_char_services,
      rsi_ble_inc_services,
      rsi_ble_on_att_desc,
      rsi_ble_on_read,
      rsi_ble_write,
      rsi_ble_on_gatt_write_event,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,//  rsi_ble_gatt_error_event,
      NULL,//  rsi_ble_gatt_desc_val_event,
      NULL,//  rsi_ble_profiles_list_event,
      NULL,//  rsi_ble_profile_event,
      NULL,//  rsi_ble_char_services_event,
      NULL,//  rsi_ble_inc_services_event,
      NULL,//  rsi_ble_on_att_desc_event,
      NULL,//  rsi_ble_on_read_event,
      NULL,//  rsi_ble_write_event,
      NULL,
      NULL);

  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! Set local name
  rsi_bt_set_local_name(RSI_BLE_APP_GATT_TEST);

#if 1
  smp_pairing_cap_data.io_capability = RSI_BLE_SMP_IO_CAPABILITY;
  smp_pairing_cap_data.oob_data_flag = OOB_DATA_FLAG;
  smp_pairing_cap_data.auth_req      = AUTH_REQ | (MITM_REQ << 2) ;
  smp_pairing_cap_data.enc_key_size  = ENC_KEY_SIZE;
  smp_pairing_cap_data.ini_key_distribution = INI_KEY_DIST;
  smp_pairing_cap_data.rsp_key_distribution = RSP_KEY_DIST;


  //! set smp pairing capability data
  status = rsi_ble_set_smp_pairing_cap_data(&smp_pairing_cap_data);
  if(status != RSI_SUCCESS)
  {
    LOG_PRINT ("smp_pairing_cap_data cmd failed = %x",status);
    return status;
  }
#endif


  //! add device to white-list
  rsi_ascii_dev_address_to_6bytes_rev (remote_dev_bd_addr, RSI_BLE_DEV_ADDR);
  status = rsi_ble_addto_whitelist(remote_dev_bd_addr, RSI_BLE_DEV_ADDR_TYPE);
  if(status != RSI_SUCCESS){
    LOG_PRINT ("\n add device to white list failed"); 
    return status;
  }

#if (ROLE == SLAVE)

  //! prepare advertise data //local/device name
  adv_data[3] = strlen (RSI_BLE_APP_GATT_TEST) + 1;
  adv_data[4] = 9;
  strcpy ((char *)&adv_data[5], RSI_BLE_APP_GATT_TEST);
  //! set advertising data
  rsi_ble_set_advertise_data (adv_data, strlen (RSI_BLE_APP_GATT_TEST) + 5);

  //! set device in advertising mode.
  status = rsi_ble_start_advertising ();
  if(status != RSI_SUCCESS)
  {
    return;
  }
#else
  //! start scanning
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
      case RSI_BLE_ADV_REPORT_EVENT:
        {
          //! advertise report event.
          //! clear the advertise report event.
          rsi_ble_app_clear_event (RSI_BLE_ADV_REPORT_EVENT);

          //status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_addr);
          status = rsi_ble_connect(remote_addr_type, remote_dev_bd_addr);
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("\n connect status: 0x%X", status);
          }     
        }
        break;

      case RSI_BLE_CONN_EVENT:
        {
          //! event invokes when connection was completed
          LOG_PRINT ("\n received connection event from remote_dev_bd_addr : %s \n\n",
              rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_bd_addr, app_conn_event.dev_addr));
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);
          sleep(5);
          device_found = 0;
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
#if (SM_MAS_PROT_BV_01_C || SM_MAS_JW_BV_05_C || SM_MAS_JW_BI_01_C || SM_MAS_JW_BI_04_C || SM_MAS_JW_BV_01_C \
    || SM_MAS_PKE_BV_01_C || SM_MAS_PKE_BI_01_C || SM_MAS_PKE_BV_04_C || SM_MAS_PKE_BI_02_C || SM_MAS_OOB_BV_05_C \
    || SM_MAS_OOB_BV_07_C || SM_MAS_EKS_BV_01_C || SM_MAS_EKS_BI_01_C || SM_MAS_KDU_BV_04_C || SM_MAS_KDU_BV_05_C \
    || SM_MAS_KDU_BV_06_C || SM_SLA_SIP_BV_01_C ) 
          if (1)
#elif (SM_SLA_SIE_BV_01_C )
            if(already_paired)
#else
              if(0)
#endif
              {
                LOG_PRINT ("\n send smp pair request");
                //! initiating the SMP pairing process
                status = rsi_ble_smp_pair_request (app_conn_event.dev_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
              }
#if (GAP_BOND_BON_BV_04_C )
              else if(already_paired) {
                rsi_ble_start_encryption(app_conn_event.dev_addr,app_ble_sec_keys.remote_ediv, app_ble_sec_keys.remote_rand, app_ble_sec_keys.remote_ltk);
                break;
              }
#endif
        }
        break;

      case RSI_BLE_DISCONN_EVENT:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
#if (ROLE == SLAVE)
          //! set device in advertising mode.
adv:
          status = rsi_ble_start_advertising ();
          if (status != RSI_SUCCESS) {
            sleep(5);
            goto adv;
          }
#else

          //! start scanning
scan:
          status = rsi_ble_start_scanning();
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("\nscan status");
            sleep(5);
            goto scan;
            //return status;
          }
#endif
        }
        break;

      case RSI_BLE_GATT_WRITE_EVENT:
        {
          //! event invokes when write/notification events received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE_EVENT);
        }
        break;

      case RSI_BLE_GATT_PROFILES:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILES);
          sleep(5);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
          rsi_ble_get_char_services_async(app_conn_event.dev_addr, 1, 0x10, NULL);
        }
        break;

      case RSI_BLE_GATT_PROFILE:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILE);

          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
          rsi_ble_get_char_services_async (app_conn_event.dev_addr, 0x1, 0xFFFF, &ble_char_services);
        }
        break;

      case RSI_BLE_GATT_CHAR_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_SERVICES);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
          rsi_ble_get_inc_services_async (app_conn_event.dev_addr, 1, 0xFFFF, &ble_inc_serv);

        }
        break;

      case RSI_BLE_GATT_INC_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_INC_SERVICES);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);

          rsi_ble_get_att_descriptors_async (app_conn_event.dev_addr, 1, 0xFFFF, &ble_desc_services);

        }
        break;

      case RSI_BLE_GATT_DESC_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_SERVICES);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
          rsi_ble_get_att_value_async (app_conn_event.dev_addr, 3, &ble_read_val);
          loop = 0;
        }
        break;

      case RSI_BLE_GATT_READ:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_READ);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);

          rsi_ble_get_char_value_by_uuid_async (app_conn_event.dev_addr, 0x01, 0xffff, search_serv, &ble_read_val);

          if (loop == 0)
          {
            search_serv.size = 2;
            search_serv.val.val16 = 0x2A00;
            rsi_ble_get_char_value_by_uuid_async(app_conn_event.dev_addr, 0x01, 0x05, search_serv, NULL);
          } else if (loop == 1) {
            uint16_t handles[2];

            handles[0] = 0x03;
            handles[1] = 0x05;
            rsi_ble_get_multiple_att_values_async(app_conn_event.dev_addr, 2, handles, NULL);
          } else if (loop == 2) {
            rsi_ble_get_long_att_value_async(app_conn_event.dev_addr, 3, 0, NULL);
          } else {
            rsi_ble_set_att_value_async (app_conn_event.dev_addr, 3, 12, "REDPINE_GATT");
          }
          loop++;
        }
        break;

      case RSI_BLE_GATT_WRITE:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE);
          rsi_ble_set_att_cmd (app_conn_event.dev_addr, 3, 12, "REDPINE_GATT");
        }
        break;
      case RSI_BLE_GATT_ERROR:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_ERROR);
          rsi_ble_set_att_cmd (app_conn_event.dev_addr, 3, 12, "REDPINE_DUT");
        }
        break;

      case RSI_BLE_GATT_DESC_VAL:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_VAL);
          rsi_ble_get_att_value_async(app_conn_event.dev_addr, 3, NULL);
          loop = 0;
        }
        break;
      case RSI_BLE_GATT_LONG_WRITE:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_LONG_WRITE);
        }
        break;
      case RSI_BLE_DISCONN:
        {
          sleep(6);
          LOG_PRINT ("\n giving disconnect");
          rsi_ble_disconnect(app_conn_event.dev_addr);
          rsi_ble_app_clear_event (RSI_BLE_DISCONN);
        }
        break;
      case RSI_BLE_LTK_REQ_EVENT:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_LTK_REQ_EVENT);
          //rsi_6byte_dev_address_to_ascii (remote_dev_addr,temp_le_ltk_req.dev_addr);

#if (ROLE == SLAVE)
          if((temp_le_ltk_req.localediv == glbl_enc_enabled.localediv) && 
              !((memcmp(temp_le_ltk_req.localrand, glbl_enc_enabled.localrand,8)))
            )
          {
            LOG_PRINT("\n positive reply\n");
            //! give le ltk req reply cmd with positive reply
            status = rsi_ble_ltk_req_reply(app_conn_event.dev_addr,1,glbl_enc_enabled.localltk);
          }
          else
          {
            LOG_PRINT("\n negative reply\n");
            //! give le ltk req reply cmd with negative reply
            status = rsi_ble_ltk_req_reply(app_conn_event.dev_addr,0,NULL);
          }
#endif
        }
        break;

      case RSI_BLE_SMP_REQ_EVENT:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_REQ_EVENT);

          //! initiate SMP protocol pairing request
          status = rsi_ble_smp_pair_request (app_conn_event.dev_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
        }
        break;

      case RSI_BLE_SMP_RESP_EVENT:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_RESP_EVENT);

          //! initiating the SMP pairing process
          status = rsi_ble_smp_pair_response (app_conn_event.dev_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
        }
        break;

      case RSI_BLE_SMP_PASSKEY_EVENT:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_EVENT);

          //! initiating the SMP pairing process
          if(RSI_BLE_SMP_IO_CAPABILITY == 2 || RSI_BLE_SMP_IO_CAPABILITY == 4) {

            LOG_PRINT ("\nenter 6 digit passkey");
            LOG_SCAN ("%d",&smp_passkey);
            status = rsi_ble_smp_passkey (app_conn_event.dev_addr, smp_passkey);
          }else {
            status = rsi_ble_smp_passkey (app_conn_event.dev_addr, RSI_BLE_SMP_PASSKEY);
          }
        }
        break;
      case RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
        }
        break;

      case RSI_BLE_SMP_FAILED_EVENT:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_FAILED_EVENT);
#if (SM_MAS_JW_BV_05_C || SM_MAS_KDU_BI_01_C)
          sleep(7);
          status = rsi_ble_disconnect (app_conn_event.dev_addr);
#endif

        }
        break;

      case RSI_BLE_ENCRYPT_STARTED_EVENT:
        {
          //! start the encrypt event

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
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
  
  //! Call Simple  GATT  Application
  rsi_ble_simple_gatt_test();

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
  rsi_task_create(rsi_ble_simple_gatt_test, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
