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
//! Common include file 
#include<rsi_common_apis.h>
#include<rsi_pts_gap_inc.h>
#if 1//def RSI_DEBUG_PRINTS
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

#if (GAP_BROB_BCST_BV_01_C || GAP_BROB_BCST_BV_02_C || GAP_DISC_NONM_BV_01_C || GAP_DISC_NONM_BV_02_C \
    || GAP_DISC_GENM_BV_01_C || GAP_DISC_GENM_BV_02_C || GAP_BOND_BON_BV_03_C || GAP_BOND_BON_BV_01_C \
    || GAP_CONN_NCON_BV_01_C || GAP_CONN_NCON_BV_02_C || GAP_CONN_NCON_BV_03_C || GAP_CONN_DCON_BV_01_C \
    || GAP_CONN_UCON_BV_01_C || GAP_CONN_UCON_BV_02_C || GAP_CONN_UCON_BV_03_C || GAP_CONN_CPUP_BV_01_C \
    || GAP_CONN_CPUP_BV_02_C || GAP_CONN_CPUP_BV_03_C || GAP_BOND_NBON_BV_03_C || GAP_ADV_BV_01_C  \
    || GAP_ADV_BV_02_C || GAP_ADV_BV_03_C || GAP_ADV_BV_04_C || GAP_ADV_BV_05_C || GAP_ADV_BV_08_C \
    || GAP_ADV_BV_10_C || GAP_GAT_BV_01_C || GAP_SEC_AUT_BV_20_C || GAP_SEC_AUT_BV_22_C || GAP_SEC_AUT_BV_18_C \
    || GAP_DISC_LIMM_BV_01_C || GAP_DISC_LIMM_BV_02_C || GAP_GAT_BV_04_C || GAP_CONN_CPUP_BV_08_C)
#define ROLE      SLAVE
#elif (GAP_BROB_OBSV_BV_01_C || GAP_BROB_OBSV_BV_02_C || GAP_IDLE_NAMP_BV_01_C || GAP_IDLE_NAMP_BV_02_C \
    || GAP_BOND_BON_BV_04_C || GAP_BOND_BON_BV_02_C || GAP_CONN_ACEP_BV_01_C || GAP_CONN_GCEP_BV_01_C \
    || GAP_CONN_GCEP_BV_02_C || GAP_CONN_SCEP_BV_01_C || GAP_CONN_DCEP_BV_01_C || GAP_CONN_DCEP_BV_03_C \
    || GAP_CONN_CPUP_BV_04_C || GAP_CONN_CPUP_BV_05_C || GAP_CONN_CPUP_BV_06_C || GAP_CONN_TERM_BV_01_C \
    || GAP_BOND_NBON_BV_01_C || GAP_BOND_NBON_BV_02_C || GAP_SEC_AUT_BV_17_C || GAP_SEC_AUT_BV_19_C \
    || GAP_SEC_AUT_BV_21_C || GAP_DISC_LIMP_BV_01_C || GAP_DISC_LIMP_BV_02_C || GAP_DISC_LIMP_BV_03_C \
    || GAP_DISC_LIMP_BV_04_C || GAP_DISC_LIMP_BV_05_C || GAP_DISC_GENP_BV_01_C || GAP_DISC_GENP_BV_02_C \
    || GAP_DISC_GENP_BV_03_C || GAP_DISC_GENP_BV_04_C || GAP_DISC_GENP_BV_05_C)
#define ROLE      MASTER
#else
#define ROLE      MASTER //SLAVE
#endif
//! maximum advertise list
#define  NO_OF_ADV_REPORTS  5
#define RSI_BLE_DEV_ADDR_TYPE                           LE_PUBLIC_ADDRESS 
#define RSI_BLE_DEV_ADDR                          "00:1B:DC:F2:1C:54"
//#define RSI_BLE_DEV_ADDR                          "00:1B:DC:07:2C:F0"

//! local device IO capability
#define  RSI_BLE_SMP_IO_CAPABILITY      0x00
#define  RSI_BLE_SMP_PASSKEY            0
#if (GAP_BOND_NBON_BV_01_C || GAP_BOND_NBON_BV_02_C || GAP_BOND_NBON_BV_03_C || GAP_SEC_AUT_BV_22_C || GAP_SEC_AUT_BV_18_C || GAP_SEC_AUT_BV_17_C)
#define  MITM_REQ                       0
#else
#define  MITM_REQ                       1
#endif

/***********************************************************************************************************************************************/
//! BLE characteristic service and attribute uuid
/***********************************************************************************************************************************************/
//! BLE characteristic service uuid
#define  RSI_BLE_GAP_SERVICE_UUID       0x1800
#define  RSI_BLE_GATT_SERVICE_UUID      0x1801

#define  RSI_BLE_DEVICE_NAME_UUID       0x2A00
#define  RSI_BLE_APPEARANCE_UUID        0x2A01
#define  RSI_BLE_PPC_UUID               0x2A04

#define  RSI_BLE_SERVICE_CHANGED_UUID   0x2A05

/***********************************************************************************************************************************************/
//! attribute properties
/***********************************************************************************************************************************************/
#define  RSI_BLE_ATT_PROPERTY_READ                      0x02
#define  RSI_BLE_ATT_PROPERTY_WRITE                     0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY                    0x10
/***********************************************************************************************************************************************/

/***********************************************************************************************************************************************/
//! BLE attribute service types uuid values
/***********************************************************************************************************************************************/
#define  RSI_BLE_CHAR_SERV_UUID                         0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID                       0x2902





/*=========================================================================*/
//! SMP parameters
/*=========================================================================*/

#if (GAP_BOND_NBON_BV_01_C || GAP_BOND_NBON_BV_02_C || GAP_BOND_NBON_BV_03_C)
#define AUTH_REQ                                0x08 /*default bonding and SC enable*/ // 0x01 for only bonding
#else
#define AUTH_REQ                                0x09 /*default bonding and SC enable*/ // 0x01 for only bonding
#endif
#define OOB_DATA_FLAG                           0x00//RSI_DISABLE
#define ENC_KEY_SIZE                            0x10
#define INI_KEY_DIST                            0x07 /*default EncKey, IdKey and SignKey */ // BIT(0)-> EncKey // BIT(1) -> IdKey // BIT(2) -> SignKey // BTI(3) -> LinkKey
#define RSP_KEY_DIST                            0x07 /*default EncKey, IdKey and SignKey */ // BIT(0)-> EncKey // BIT(1) -> IdKey // BIT(2) -> SignKey // BTI(3) -> LinkKey




//! local device name
#define RSI_BLE_APP_GATT_TEST  "RSI_BLE_GATT_TEST"

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
#if 0
//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_DEV_ADDR                          "00:23:A7:80:70:B9"

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME					   "REDPINE_DEV"
#endif

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
uint16_t last_handle = 1;
//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t app_ble_write_event;
static uint16_t rsi_ble_battery_val_hndl;
static uint8_t notify_enabled = 0;
static rsi_ble_resp_profiles_list_t  ble_profiles;
static rsi_ble_resp_att_descs_t  get_desc_services;
rsi_bt_event_encryption_enabled_t glbl_enc_enabled;
rsi_bt_event_le_ltk_request_t temp_le_ltk_req;
static rsi_bt_event_le_security_keys_t app_ble_sec_keys;
static uint32_t  numeric_value;
uint8_t already_paired = 0;
uint8_t enc_start = 0;
static uint32_t smp_passkey=0;
rsi_ble_set_smp_pairing_capabilty_data_t  smp_pairing_cap_data;
uint8_t limited_device_found = 0;
uint8_t general_device_found = 0;

#if ( GAP_IDLE_NAMP_BV_01_C)
static uint8_t uuid_size;
static uint16_t uuid_value;
static uint16_t dummy;
static uint16_t f_handle;
static uint16_t lst_handle;
static uint8_t dbg_flag;
uuid_t uuid_val;
#endif


static uint16_t  rsi_ble_att1_val_hndl;
static uint16_t  rsi_ble_att2_val_hndl;
static uint16_t  rsi_ble_att3_val_hndl;


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
 * @fn         BT_LE_ADPacketExtractApp
 * @brief      This function used to extract device name in advertise report data.
 * @param[in]  pbuf advertise data packet buffer pointer, buf_len buffer length
 * @return     none.
 * @section description
 * This function is used to extract the advertiseed packet.
 */
void BT_LE_ADPacketExtractApp (uint8_t *remote_name,uint8_t *pbuf,uint8_t buf_len)
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
        flags = pbuf[ix+1]; 
        LOG_PRINT (" \n ********** \n AD Data Flags =0X%x \n\n ********** \n",flags);
#if (GAP_DISC_LIMP_BV_01_C || GAP_DISC_LIMP_BV_02_C || GAP_DISC_LIMP_BV_03_C || GAP_DISC_LIMP_BV_04_C || GAP_DISC_LIMP_BV_05_C \
    || GAP_DISC_GENP_BV_01_C || GAP_DISC_GENP_BV_02_C || GAP_DISC_GENP_BV_03_C || GAP_DISC_GENP_BV_04_C || GAP_DISC_GENP_BV_05_C)
        if(flags & 0x01) {
          LOG_PRINT ("\n Limited Discoverable device \n");
          limited_device_found = 1;
        }
        else if(flags & 0x02) {
          LOG_PRINT ("\n General Discoverable device \n");
          general_device_found = 1;
        }
#endif
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

  BT_LE_ADPacketExtractApp (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);

  LOG_PRINT("\nreport_type = %d",adv_report->report_type);

  if((device_found==0) && ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)&&((strcmp(remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  {
#if (GAP_BROB_OBSV_BV_01_C || GAP_BROB_OBSV_BV_02_C ) 
    LOG_PRINT ("\ndevice found \n");
    while(1);
#elif (GAP_DISC_LIMP_BV_01_C || GAP_DISC_GENP_BV_01_C || GAP_DISC_GENP_BV_02_C)
    if(limited_device_found) {
      LOG_PRINT ("\n Limited Discoverable device found\n");
      while(1);
    }
    else if (general_device_found) {
      LOG_PRINT ("\n General Discoverable device found\n");
      while(1);
    }
#elif (GAP_DISC_LIMP_BV_02_C || GAP_DISC_LIMP_BV_03_C || GAP_DISC_LIMP_BV_04_C \
    || GAP_DISC_LIMP_BV_05_C || GAP_DISC_GENP_BV_03_C || GAP_DISC_GENP_BV_04_C || GAP_DISC_GENP_BV_05_C)
    return;
#endif
    device_found=1;
    memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
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
  memcpy (remote_dev_bd_addr, (uint8_t *)remote_dev_address->dev_addr, 6);
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
  memcpy (remote_dev_bd_addr, (uint8_t *)remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_resp - remote_dev_addr : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t*)remote_dev_addr, remote_dev_address->dev_addr));
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
  memcpy (remote_dev_bd_addr, (uint8_t *)remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_passkey - remote_dev_addr : %s\r\n",
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

	LOG_PRINT ("passkey: %.6s \r\n", smp_passkey_display->passkey );
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
  memcpy (remote_dev_bd_addr, (uint8_t *)remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_failed status: 0x%x, remote_dev_addr: %s\r\n", status, 
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
  enc_start++;
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
  memcpy (&app_ble_sec_keys, rsi_ble_event_le_security_keys, sizeof (rsi_bt_event_le_security_keys_t));
  LOG_PRINT ("remote_ediv: 0x%x\r\n", app_ble_sec_keys.remote_ediv);
#if (GAP_BOND_BON_BV_04_C )
  rsi_ble_app_set_event (RSI_BLE_DISCONN);
#endif
  return;
}

void rsi_ble_on_sc_passkey (rsi_bt_event_sc_passkey_t *sc_passkey)
{
  memcpy (remote_dev_bd_addr, sc_passkey->dev_addr, 6);
  LOG_PRINT ("remote addr: %s, passkey: %.6s \r\n", 
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, sc_passkey->dev_addr),
      sc_passkey->passkey );
  numeric_value = sc_passkey->passkey;
  rsi_ble_app_set_event (RSI_BLE_SC_PASSKEY_EVENT);
  return;
}

void rsi_ble_on_conn_update_complete_event ( rsi_ble_event_conn_update_t *conn_update_complete_event, uint16_t resp_status)
{
  LOG_PRINT ("\n Connection Parameter update event resp status = %x \n", resp_status);
#if (GAP_CONN_CPUP_BV_04_C || GAP_CONN_CPUP_BV_06_C)
  rsi_ble_app_set_event (RSI_BLE_DISCONN);
#endif
  return;
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
  if (reason == 0x4E3D)
  {
    LOG_PRINT ("\nSMP key or Pin missing \n");
  }
	memcpy(&app_disconn_event, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
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
    if(resp_status == 0x0103){
      sleep(5);
      LOG_PRINT ("\nlast handle = 0x%x",last_handle);
      rsi_ble_get_profiles (app_conn_event.dev_addr, last_handle, 0xffff, &ble_profiles);
      return;
    }
    LOG_PRINT("ble_profile_list resp_status %x \n\n",resp_status);
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
    LOG_PRINT("ble_profile resp_status %x \n\n",resp_status);
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
    LOG_PRINT(" \n ble_char services resp_status %x",resp_status);
  }else {
    memcpy(&ble_char_services, rsi_ble_resp_char_services, sizeof(rsi_ble_resp_char_services_t));
    LOG_PRINT("\n ble_char number of services : 0X%x",ble_char_services.num_of_services);
    for(ix=0; ix<ble_char_services.num_of_services;ix++) {
      LOG_PRINT ("\n char serv handle : 0X%x",(uint16_t )ble_char_services.char_services[ix].handle);
      LOG_PRINT ("\n char handle : 0X%x", (uint16_t )ble_char_services.char_services[ix].char_data.char_handle);
      LOG_PRINT ("\n char properties : 0x%x",(uint8_t )ble_char_services.char_services[ix].char_data.char_property);
#if (GAP_SEC_AUT_BV_19_C || GAP_SEC_AUT_BV_20_C || GAP_SEC_AUT_BV_18_C || GAP_SEC_AUT_BV_17_C)
      if(ble_char_services.char_services[ix].char_data.char_property & (1 << 1)) // checking read property
      {
        LOG_PRINT("\n\n ************* \n\n found read property \n\n ***************** \n\n");
        rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
        return;
      }
#endif
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
    LOG_PRINT ("\n ble inc service resp =%x",resp_status);
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
  uint8_t ix;

  LOG_PRINT("\n ble_on_att_desc resp_status %x",resp_status);
  if(resp_status != RSI_SUCCESS) {
    LOG_PRINT("\n ble_on_att_desc resp_status %x",resp_status);
  }else {
    LOG_PRINT ("\n no of attributes : %d", rsi_ble_resp_att_desc->num_of_att);
    for(ix=0;ix< rsi_ble_resp_att_desc->num_of_att ; ix++)
    {
      LOG_PRINT ("\n att_desc handle : %x", *(uint16_t *)rsi_ble_resp_att_desc->att_desc->handle);
      LOG_PRINT ("\n att_desc uuid : %x", (uint16_t *)rsi_ble_resp_att_desc->att_desc->att_type_uuid.val.val16);
    }
    rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
    return;
  }
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
   LOG_PRINT("\nble_on_read resp_status %x",resp_status);
   if((resp_status == 0x0103) || (resp_status == 0x4a05)) {
     LOG_PRINT ("\n authentication required\n initiate smp request \n");
     rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
     return;
   }
 }else { 

   memcpy(&ble_read_val, rsi_ble_resp_att_val, sizeof(rsi_ble_resp_att_value_t));
#if (GAP_IDLE_NAMP_BV_01_C)
   LOG_PRINT ("\n ble_read_val data : %s \n",&ble_read_val.att_value[3]);
   rsi_ble_app_set_event (RSI_BLE_DISCONN);
#else
   LOG_PRINT ("\n ble_read_val data : %s \n",&ble_read_val.att_value[1]);
#endif
#if (GAP_SEC_AUT_BV_19_C || GAP_DM_NAD_BV_02_C || GAP_SEC_AUT_BV_17_C)
   rsi_ble_app_set_event (RSI_BLE_DISCONN);
#endif
   //	rsi_ble_app_set_event (RSI_BLE_GATT_READ);
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

#if (GAP_IDLE_NAMP_BV_01_C)
void fun ()
{

  uint16_t d2= 0;
  uint16_t d3= 0;
  uint8_t d4[8]= {0x23, 0x01, 0x67, 0x45, 0xef, 0xcd, 0xab, 0x89};

  LOG_PRINT ("\n enter 128-bit uuid value: ");
  LOG_PRINT ("\n enter 4 bytes");
  LOG_SCAN ("%x",(&uuid_val.val.val128.data1));

  /* manually filling the remaining uuid data */
  memcpy ((&uuid_val.val.val128.data2), &d2 , 2);
  memcpy ((&uuid_val.val.val128.data3), &d3 , 2);
  memcpy ((&uuid_val.val.val128.data4), d4 , 8);
  return;
}
#endif
/*==============================================*/
/**
 * @fn         rsi_ble_add_char_serv_att
 * @brief      this function is used to add characteristic service attribute..
 * @param[in]  serv_handler, service handler.
 * @param[in]  handle, characteristic service attribute handle.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  att_val_handle, characteristic value handle
 * @param[in]  att_val_uuid, characteristic value uuid
 * @return     none.
 * @section description
 * This function is used at application to add characteristic attribute
 */
static void rsi_ble_add_char_serv_att (void *serv_handler,
		uint16_t handle,
		uint8_t val_prop,
		uint16_t att_val_handle,
		uuid_t att_val_uuid)
{
	rsi_ble_req_add_att_t  new_att = {0};


	//! preparing the attribute service structure
	new_att.serv_handler = serv_handler;
	new_att.handle = handle;
	new_att.att_uuid.size = 2;
	new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
	new_att.property = RSI_BLE_ATT_PROPERTY_READ;


	//! preparing the characteristic attribute value
	new_att.data_len = 6;
	new_att.data[0] = val_prop;
	rsi_uint16_to_2bytes (&new_att.data[2], att_val_handle);
	rsi_uint16_to_2bytes (&new_att.data[4], att_val_uuid.val.val16);

	//! Add attribute to the service
	rsi_ble_add_attribute (&new_att);

	return;
}

static void rsi_ble_add_char_val_att (void *serv_handler,
		uint16_t handle,
		uuid_t   att_type_uuid,
		uint8_t  val_prop,
		uint8_t *data,
		uint8_t  data_len)
{
	rsi_ble_req_add_att_t  new_att = {0};

	//! preparing the attributes
	new_att.serv_handler = serv_handler;
	new_att.handle = handle;
	memcpy (&new_att.att_uuid, &att_type_uuid, sizeof (uuid_t));
	new_att.property = val_prop;

	//! preparing the attribute value
	new_att.data_len = RSI_MIN(sizeof (new_att.data), data_len);
	memcpy (new_att.data, data, new_att.data_len);

	//! add attribute to the service
	rsi_ble_add_attribute (&new_att);

	//! check the attribute property with notification
	if (val_prop & RSI_BLE_ATT_PROPERTY_NOTIFY)
	{
		//! if notification property supports then we need to add client characteristic service.

		//! preparing the client characteristic attribute & values
		memset (&new_att, 0, sizeof(rsi_ble_req_add_att_t));
		new_att.serv_handler = serv_handler;
		new_att.handle = handle + 1;
		new_att.att_uuid.size = 2;
		new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_UUID;
		new_att.property = RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE;
		new_att.data_len = 2;

		//! add attribute to the service
		rsi_ble_add_attribute (&new_att);
	}

	return;
}

static uint32_t rsi_ble_add_simple_chat_serv2(void) {

	uuid_t  new_uuid = {0};
	rsi_ble_resp_add_serv_t  new_serv_resp = {0};
	uint8_t   data[20] = {"redpine_sampletest_1"};
  uint16_t data1 = 128;
  uint16_t data2[8] = { 0x0020, 0x0030, 0x0000, 3200  };
	//! adding GAP service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_GAP_SERVICE_UUID;
	rsi_ble_add_service (new_uuid, &new_serv_resp);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_DEVICE_NAME_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 1,
			RSI_BLE_ATT_PROPERTY_READ,
			new_serv_resp.start_handle + 2,
			new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_DEVICE_NAME_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 2,
			new_uuid,
			RSI_BLE_ATT_PROPERTY_READ,
			data,
			sizeof (data));


	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_APPEARANCE_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 3,
			RSI_BLE_ATT_PROPERTY_READ,
			new_serv_resp.start_handle + 4,
			new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_att2_val_hndl = new_serv_resp.start_handle + 4;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_APPEARANCE_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 4,
			new_uuid,
			RSI_BLE_ATT_PROPERTY_READ,
			&data1,
			sizeof (data1));



//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_PPC_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 5,
			RSI_BLE_ATT_PROPERTY_READ,
			new_serv_resp.start_handle + 6,
			new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_att2_val_hndl = new_serv_resp.start_handle + 6;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_PPC_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 6,
			new_uuid,
			RSI_BLE_ATT_PROPERTY_READ,
			&data2,
			0x08 /*sizeof (data2)*/);





	//! adding GATT service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_GATT_SERVICE_UUID;
	rsi_ble_add_service (new_uuid, &new_serv_resp);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_SERVICE_CHANGED_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 1,
			RSI_BLE_ATT_PROPERTY_NOTIFY,
			new_serv_resp.start_handle + 2,
			new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_att3_val_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_SERVICE_CHANGED_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 2,
			new_uuid,
			RSI_BLE_ATT_PROPERTY_NOTIFY,
			data,
			sizeof (data));

	return 0;
}



int32_t rsi_ble_simple_gatt_test (void)
{
  int32_t status = 0;
  int32_t  event_id;
  int8_t   data[30] = {0};
  uint8_t  remote_dev_addr[18] = {0};
  int8_t local_dev_addr[18] = {0};
  uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = {0};
  uuid_t   search_serv = {0};
  rsi_ble_resp_local_att_value_t  local_att_val;
  uint8_t loop;
  uint8_t scn_data[31] = {0};
#if(GAP_DISC_GENM_BV_01_C || GAP_DISC_GENM_BV_02_C || GAP_CONN_NCON_BV_02_C)
  uint8_t adv_data[31] = {2,1,2};
#elif (GAP_CONN_NCON_BV_03_C || GAP_CONN_UCON_BV_03_C || GAP_DISC_LIMM_BV_01_C || GAP_DISC_LIMM_BV_02_C)
  uint8_t adv_data[31] = {2,1,1};
#elif (GAP_ADV_BV_03_C)
  uint8_t  adv_data[31] = {0x05, 0x03, 0x00, 0x18, 0x01, 0x18, 0x0D, 0x09, 0x50, 0x54, 0x53, 0x2D, 0x47, 0x41, 0x50, 0x2D, 0x30, 0x36, 0x42, 0x38, 0x03, 0x19, 0x00, 0x00, 0x02, 0x01, 0x03};
#elif (GAP_ADV_BV_04_C)
  uint8_t adv_data[31] = {0x03,0xff,0x0a,0x00}; // for Manufacturee Specific data dummy 
#elif (GAP_ADV_BV_05_C)
  uint8_t adv_data[31] = {0x02,0x0a,0x0a}; // for Tx Power level
#elif (GAP_ADV_BV_08_C)
  uint8_t adv_data[31] = {0x5,0x12,0x06,0x00,0xc8,0x00}; // for Slave conn interval
#elif (GAP_ADV_BV_10_C)
  uint8_t adv_data[31] = {0x05,0x16,0xbb,0xaa,0x05,0x01}; // for service data 
#else
  uint8_t  adv_data[31] = {0x05, 0x03, 0x00, 0x18, 0x01, 0x18, 0x0D, 0x09, 0x50, 0x54, 0x53, 0x2D, 0x47, 0x41, 0x50, 0x2D, 0x30, 0x36, 0x42, 0x38, 0x03, 0x19, 0x00, 0x00};
#endif
  //  fun();

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


#if (GAP_GAT_BV_04_C)
	rsi_ble_add_simple_chat_serv2();
#endif

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
      rsi_ble_on_conn_update_complete_event);
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

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  rsi_6byte_dev_address_to_ascii (local_dev_addr,rsi_app_resp_get_dev_addr);
  LOG_PRINT("local device addr: %s \n",local_dev_addr);


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

#if (ROLE == SLAVE)

#if (GAP_DISC_GENM_BV_01_C || GAP_DISC_GENM_BV_02_C)  
  //! prepare advertise data //local/device name
  adv_data[3] = strlen (RSI_BLE_APP_GATT_TEST) + 1;
  adv_data[4] = 9;
  strcpy ((char *)&adv_data[5], RSI_BLE_APP_GATT_TEST);
#endif
  //! set advertising data
  rsi_ble_set_advertise_data (adv_data, 24);

#if (GAP_BROB_BCST_BV_02_C)  
  //! set scan response data
  rsi_ble_set_scan_response_data (adv_data, 27);
#else
  rsi_ble_set_scan_response_data (adv_data, 24);
#endif
  //! set device in advertising mode.
  status = rsi_ble_start_advertising ();
  if(status != RSI_SUCCESS)
  {
    return;
  }
#else
  //! add device to white-list
  rsi_ascii_dev_address_to_6bytes_rev (remote_dev_bd_addr, RSI_BLE_DEV_ADDR);
  status = rsi_ble_addto_whitelist(remote_dev_bd_addr,RSI_BLE_DEV_ADDR_TYPE);
  if(status != RSI_SUCCESS){
    return status;
  }

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

          //status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_bd_addr);
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

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);
          device_found = 0;
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
#if (GAP_CONN_CPUP_BV_01_C || GAP_CONN_CPUP_BV_02_C || GAP_CONN_CPUP_BV_03_C || GAP_CONN_CPUP_BV_06_C)

          sleep(5);
          rsi_ble_conn_params_update (app_conn_event.dev_addr, 100, 200, 0, 3200);
          break;
#endif
#if (GAP_CONN_ACEP_BV_01_C || GAP_CONN_GCEP_BV_01_C || GAP_CONN_GCEP_BV_02_C || GAP_CONN_SCEP_BV_01_C || GAP_CONN_DCEP_BV_01_C || GAP_CONN_DCEP_BV_03_C || GAP_CONN_CPUP_BV_05_C || GAP_CONN_TERM_BV_01_C)
          sleep(5);
          rsi_ble_app_set_event (RSI_BLE_DISCONN);
          break;
#endif
#if (GAP_BOND_NBON_BV_02_C || GAP_SEC_AUT_BV_22_C || GAP_BOND_BON_BV_01_C )
          if(1)
#elif (GAP_BOND_BON_BV_02_C || GAP_SEC_AUT_BV_19_C || GAP_SEC_AUT_BV_21_C)
            if(!already_paired)
#else
              if(0)
#endif
              {
                LOG_PRINT ("\n Initiating Pairing Request \n");
                //! If not paired even once
                //! initiating the SMP pairing process
                status = rsi_ble_smp_pair_request (app_conn_event.dev_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
              }
#if (GAP_BOND_BON_BV_04_C || GAP_SEC_AUT_BV_21_C )
              else if(already_paired) {
                rsi_ble_start_encryption(app_conn_event.dev_addr, app_ble_sec_keys.remote_ediv, app_ble_sec_keys.remote_rand, app_ble_sec_keys.remote_ltk);
                break;
              }
#endif
#if (GAP_SEC_AUT_BV_19_C || GAP_SEC_AUT_BV_20_C )
          if(already_paired)
#elif (GAP_SEC_AUT_BV_18_C || GAP_SEC_AUT_BV_17_C)
            if(!already_paired)
#else
              if(0)
#endif
              {
                sleep(5);
                LOG_PRINT ("\nlast handle = 0x%x",last_handle);
                rsi_ble_get_profiles (app_conn_event.dev_addr, last_handle, 0xffff, &ble_profiles);
              }

#if (GAP_IDLE_NAMP_BV_01_C ) 
          rsi_ble_app_set_event (RSI_BLE_GATT_READ);
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
            if (status == 0x4E0C) {
              break;
            }
            goto adv;
          }
#else

#if (GAP_IDLE_NAMP_BV_01_C)
          uuid_size =0;
          uuid_value =0;
          dummy =0;
          f_handle =0;
          lst_handle =0;
          dbg_flag =0;
          memset(&uuid_val,0,sizeof(uuid_t));
#endif

          //#if (GAP_BOND_BON_BV_04_C || GAP_BOND_BON_BV_02_C)
          //! start scanning
scan:
          status = rsi_ble_start_scanning();
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("\nscan status");
            sleep(1);
            goto scan;
            //return status;
          }
          //#endif
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
          rsi_ble_get_char_services(app_conn_event.dev_addr, 1, 0x10, NULL);
        }
        break;

      case RSI_BLE_GATT_PROFILE:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILE);


          LOG_PRINT ("\nhandle = 0X%x ",last_handle);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
          rsi_ble_get_char_services (app_conn_event.dev_addr, last_handle, 0xFFFF, &ble_char_services);
        }
        break;

      case RSI_BLE_GATT_CHAR_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_SERVICES);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
          rsi_ble_get_inc_services (app_conn_event.dev_addr, 1, 0xFFFF, &ble_inc_serv);
        }
        break;

      case RSI_BLE_GATT_INC_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_INC_SERVICES);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);

          rsi_ble_get_att_descriptors (app_conn_event.dev_addr, 1, 0xFFFF, &ble_desc_services);

        }
        break;

      case RSI_BLE_GATT_DESC_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_SERVICES);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
          rsi_ble_get_att_value (app_conn_event.dev_addr, 3, &ble_read_val);
          loop = 0;
        }
        break;

      case RSI_BLE_GATT_READ:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_READ);
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
#if (GAP_IDLE_NAMP_BV_01_C )

          search_serv.size = 0x02;
          search_serv.val.val16 = 0x2A00;
          sleep(5);
          rsi_ble_get_char_value_by_uuid (app_conn_event.dev_addr, 0x1, 0xffff, search_serv, &ble_read_val);

#endif
#if 0
          if (loop == 0)
          {
            search_serv.size = 2;
            search_serv.val.val16 = 0x2A00;
            rsi_ble_get_char_value_by_uuid(app_conn_event.dev_addr, 0x01, 0x05, search_serv, NULL);
          } else if (loop == 1) {
            uint16_t handles[2];

            handles[0] = 0x03;
            handles[1] = 0x05;
            rsi_ble_get_multiple_att_values(remote_dev_bd_addr, 2, handles, NULL);
          } else if (loop == 2) {
            rsi_ble_get_long_att_value(remote_dev_bd_addr, 3, 0, NULL);
          } else {
            rsi_ble_set_att_value (remote_dev_bd_addr, 3, 12, "mallikarjuna");
          }
          loop++;
#endif
        }
        break;

      case RSI_BLE_GATT_WRITE:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE);
          //				rsi_ble_set_att_cmd (app_conn_event.dev_addr, 3, 12, "mallikarjuna");
        }
        break;
      case RSI_BLE_GATT_ERROR:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_ERROR);
          scanf("loop is %d:",&loop);
          //rsi_ble_set_att_cmd (app_conn_event.dev_addr, 3, 12, "mallikarjuna");
          LOG_PRINT ("get att descriptor value");
          rsi_ble_get_att_descriptors(app_conn_event.dev_addr, 1, 0x10, NULL);
        }
        break;

      case RSI_BLE_GATT_DESC_VAL:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_VAL);
          LOG_PRINT ("\n get att value \n");
          rsi_ble_get_att_value(app_conn_event.dev_addr, 3, NULL);
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
            status = rsi_ble_ltk_req_reply(temp_le_ltk_req.dev_addr,1,glbl_enc_enabled.localltk);
          }
          else
          {
            LOG_PRINT("\n negative reply\n");
            //! give le ltk req reply cmd with negative reply
            status = rsi_ble_ltk_req_reply(temp_le_ltk_req.dev_addr, 0, NULL);
          }
#endif
        }
        break;

      case RSI_BLE_SMP_REQ_EVENT:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_REQ_EVENT);

#if (ROLE == MASTER)
#if (GAP_BOND_BON_BV_02_C )
          if(already_paired) {
            rsi_ble_start_encryption(app_conn_event.dev_addr, app_ble_sec_keys.remote_ediv, app_ble_sec_keys.remote_rand, app_ble_sec_keys.remote_ltk);
            break;
          } else 

#endif
          {
            //! initiating the SMP pairing process
            status = rsi_ble_smp_pair_request (app_conn_event.dev_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
          }
#else
                      //! initiate SMP protocol pairing request
            status = rsi_ble_smp_pair_request (app_conn_event.dev_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
#endif
        }
        break;

      case RSI_BLE_SMP_RESP_EVENT:
        {
          //! initiate SMP protocol as a Master
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_RESP_EVENT);

          if(already_paired) {
            LOG_PRINT ("\n **************\n \t BOND LOST \n *************\n");
          } 
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

        }
        break;

      case RSI_BLE_ENCRYPT_STARTED_EVENT:
        {
          //! start the encrypt event

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
#if (GAP_SEC_AUT_BV_18_C || GAP_SEC_AUT_BV_19_C || GAP_SEC_AUT_BV_20_C )
          sleep(5);
          rsi_ble_get_att_value (app_conn_event.dev_addr, 3, &ble_read_val);
#elif (GAP_SEC_AUT_BV_17_C )
          {
                sleep(3);
                LOG_PRINT ("\nlast handle = 0x%x",last_handle);
                last_handle = 0x01;
                rsi_ble_get_profiles (app_conn_event.dev_addr, last_handle, 0xffff, &ble_profiles);
          }
       //   rsi_ble_app_set_event (RSI_BLE_DISCONN);
#elif (GAP_SEC_AUT_BV_21_C)
          if (enc_start > 1) {
     //       rsi_ble_app_set_event (RSI_BLE_DISCONN);
     /*       No need give disconncet, if encryption failes fw itslef gives disconnect */
          }
#endif

          already_paired = 1;
#if (GAP_IDLE_NAMP_BV_01_C || GAP_DM_NAD_BV_02_C)
          rsi_ble_get_profiles (app_conn_event.dev_addr, 1, 0xffff, &ble_profiles);
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
