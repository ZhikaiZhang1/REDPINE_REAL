/**
 * @file    rsi_ble_simple_chat.c
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
 *  @brief : This file contains example application for BLE simple chat
 *
 *  @section Description  This application connects as a peripheral and loops back the data
 *  received from the GATT client
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
#include<string.h>
#include<rsi_pts_gap_inc.h>

//! application defines
#define RSI_DEBUG_PRINTS

#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#define LOG_SCAN                      scanf
#else 
#define LOG_PRINT
#define LOG_SCAN
#endif

//! Devide role
#define MASTER    0
#define SLAVE     1


#if (GAP_SEC_AUT_BV_12_C || GAP_SEC_AUT_BV_13_C || GAP_SEC_AUT_BV_24_C) 
#define ROLE      MASTER
#elif (GAP_SEC_AUT_BV_11_C || GAP_SEC_AUT_BV_14_C || GAP_SEC_AUT_BV_23_C)
#define ROLE      SLAVE
#else
#define ROLE      SLAVE
#endif

//! maximum advertise list
#define  NO_OF_ADV_REPORTS  5
#define RSI_BLE_DEV_ADDR_TYPE                           LE_PUBLIC_ADDRESS 
#define RSI_BLE_DEV_ADDR                               "00:1B:DC:F2:1C:54"



//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN   10000
#define BLE_DATA             0

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! BLE attribute service types uuid values
#define  RSI_BLE_CHAR_SERV_UUID         0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID       0x2902

//! BLE characteristic service uuid
#define  RSI_BLE_NEW_SERVICE_UUID       0xAABB
#define  RSI_BLE_ATTRIBUTE_1_UUID       0x1AA1
#define  RSI_BLE_ATTRIBUTE_2_UUID       0x1BB1
#define  RSI_BLE_ATTRIBUTE_3_UUID       0x1CC1
#define  RSI_BLE_ATTRIBUTE_4_UUID       0x1DD1


//! max data length
#define  RSI_BLE_MAX_DATA_LEN           20

//! local device name
#define RSI_BLE_APP_SIMPLE_CHAT        "BLE_SIMPLE_CHAT"

//! attribute properties
#define  RSI_BLE_ATT_PROPERTY_READ      0x02
#define  RSI_BLE_ATT_PROP_WR_NO_RESP    0x04
#define  RSI_BLE_ATT_PROPERTY_WRITE     0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY    0x10
#define  RSI_BLE_ATT_PROPERTY_INDICATE  0x20


//! application event list
#define  RSI_BLE_CONN_EVENT                  0x01
#define  RSI_BLE_DISCONN_EVENT               0x02
#define  RSI_BLE_SMP_REQ_EVENT               0x03
#define  RSI_BLE_SMP_RESP_EVENT              0x04
#define  RSI_BLE_SMP_PASSKEY_EVENT           0x05
#define  RSI_BLE_SMP_FAILED_EVENT            0x06
#define  RSI_BLE_ENCRYPT_STARTED_EVENT       0x07
#define  RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT   0x08
#define  RSI_BLE_SC_PASSKEY_EVENT            0x09
#define  RSI_BLE_LTK_REQ_EVENT               0x0A

#define  RSI_BLE_GATT_WRITE_EVENT       0x0B
#define  RSI_BLE_ADV_REPORT_EVENT       0x0C
#define  RSI_BLE_REMOTE_MTU             0x0D
#define  RSI_BLE_DISCONNECT             0x0F

#define  RSI_BLE_GATT_LONG_WRITE        0x10
#define  RSI_BLE_GATT_PROFILE           0x11
#define  RSI_BLE_GATT_PROFILES          0x12
#define  RSI_BLE_GATT_CHAR_SERVICES     0x13
#define  RSI_BLE_GATT_DESC_SERVICES     0x14
#define  RSI_BLE_GATT_READ              0x15
#define  RSI_BLE_EVENT_GATT_RD          0x16


//! local device IO capability
#define  RSI_BLE_SMP_IO_CAPABILITY      0x01
#define  RSI_BLE_SMP_PASSKEY            0

#if (1)
#define  MITM_REQ                       1
#else 
#define  MITM_REQ                       0
#endif


//! SMP parameters
#if (GAP_DM_NBON_BV_01_C) 
#define AUTH_REQ                                0x08 /*default bonding and SC enable*/ // 0x01 for only bonding
#else
#define AUTH_REQ                                0x09 /*default bonding and SC enable*/ // 0x01 for only bonding
#endif
#define OOB_DATA_FLAG                           0x00
#define ENC_KEY_SIZE                            0x10
#define INI_KEY_DIST                            0x07 /*default EncKey, IdKey and SignKey */ // BIT(0)-> EncKey // BIT(1) -> IdKey // BIT(2) -> SignKey // BTI(3) -> LinkKey
#define RSP_KEY_DIST                            0x07 /*default EncKey, IdKey and SignKey */ // BIT(0)-> EncKey // BIT(1) -> IdKey // BIT(2) -> SignKey // BTI(3) -> LinkKey

//! service security defines
//! Config_bitmap values
#define  ATT_REC_MAINTAINED_IN_HOST     BIT(0)
#define  SEC_MODE_1_LEVEL_1             BIT(1)
#define  SEC_MODE_1_LEVEL_2             BIT(2)
#define  SEC_MODE_1_LEVEL_3             BIT(3)
#define  SEC_MODE_1_LEVEL_4             BIT(4)
#define  ON_BR_EDR_LINK_ONLY            BIT(5)
#define  ON_LE_LINK_ONLY                BIT(6)

//! Enumeration for commands used in application
typedef enum rsi_app_cmd_e
{
  RSI_DATA = 0
}rsi_app_cmd_t;

uint16_t first_handle = 0;
uint16_t length =0;
uint16_t last_handle = 1;
static uint8_t uuid_size;
static uint16_t uuid_value;
static uint8_t dbg_flag;
uuid_t uuid_val;
static uint16_t f_handle;
static uint16_t lst_handle;
static uint16_t handle;
static uint8_t handles_cnt;
static uint16_t handles[5];


//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t app_ble_write_event;
static uint8_t rsi_ble_app_data[RSI_BLE_MAX_DATA_LEN];
static uint16_t rsi_ble_app_data_len;
static uint16_t rsi_ble_att1_val_hndl;
static uint16_t rsi_ble_att2_val_hndl;
static uint16_t rsi_ble_att3_val_hndl;
static uint16_t rsi_ble_att4_val_hndl;
static rsi_ble_event_mtu_t ble_mtu;

static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;
static app_num_scan_list;

static rsi_ble_event_adv_report_t app_scan_list[NO_OF_ADV_REPORTS];
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t str_remote_address[18];
static uint32_t  numeric_value;
static rsi_bt_event_encryption_enabled_t  encrypt_keys;
static rsi_bt_event_le_ltk_request_t ble_ltk_req;
static rsi_bt_event_le_security_keys_t app_ble_sec_keys;
rsi_ble_set_smp_pairing_capabilty_data_t  smp_pairing_cap_data;

#ifdef RSI_BLE_PING
uint16_t timeout_resp;
#endif
static uint32_t smp_passkey=0;
static uint8_t already_paired=0;
static limited_device_found=0;
static rsi_ble_resp_att_descs_t  ble_desc_services;
static rsi_ble_resp_att_value_t  ble_read_val;
static rsi_ble_resp_profiles_list_t  ble_profiles;
static rsi_ble_resp_char_services_t ble_char_services;
static rsi_ble_resp_query_profile_descriptor_t  ble_profile;
static rsi_ble_resp_inc_services_t  ble_inc_serv;
static rsi_ble_read_req_t app_ble_read_event;

/*==============================================*/
/**
 * @fn         rsi_ble_add_char_serv_att
 * @brief      this function is used to add characteristic service attribute
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

  //! add attribute to the service
  rsi_ble_add_attribute (&new_att);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_char_val_att
 * @brief      this function is used to add characteristic value attribute.
 * @param[in]  serv_handler, new service handler.
 * @param[in]  handle, characteristic value attribute handle.
 * @param[in]  att_type_uuid, attribute uuid value.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  data, characteristic value data pointer.
 * @param[in]  data_len, characteristic value length.
 * @return     none.
 * @section description
 * This function is used at application to create new service.
 */

static void rsi_ble_add_char_val_att (void *serv_handler,
    uint16_t handle,
    uuid_t   att_type_uuid,
    uint8_t  val_prop,
    uint8_t *data,
    uint8_t  data_len,
    uint8_t  flag)
{
  rsi_ble_req_add_att_t  new_att = {0};

  //! preparing the attributes
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  memcpy (&new_att.att_uuid, &att_type_uuid, sizeof (uuid_t));
  new_att.property = val_prop;
  new_att.config_bitmap = flag & 0x7F;

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
    if (flag & 0x80) {
      new_att.data_len = 240;
    } else {
      new_att.data_len = 2;
    }

    //! add attribute to the service
    rsi_ble_add_attribute (&new_att);
  }

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_chat_add_new_serv
 * @brief      this function is used to add new service i.e., simple chat service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */
static uint32_t rsi_ble_add_simple_chat_serv (void)
{
  uuid_t  new_uuid = {0};
  rsi_ble_resp_add_serv_t  new_serv_resp = {0};
  uint8_t   data[20] = {"redpine_sampletest_1"};

  //! adding new service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_NEW_SERVICE_UUID;
  rsi_ble_add_service (new_uuid, &new_serv_resp);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 1,
      RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ,
      new_serv_resp.start_handle + 2,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 2,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ,
      data,
      sizeof (data),
      SEC_MODE_1_LEVEL_3);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_2_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 3,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROP_WR_NO_RESP,
      new_serv_resp.start_handle + 4,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_att2_val_hndl = new_serv_resp.start_handle + 4;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_2_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 4,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROP_WR_NO_RESP,
      data,
      240 /*sizeof (data)*/,
      0);

#if 1
  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_3_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 6,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_INDICATE,
      new_serv_resp.start_handle + 7,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_att3_val_hndl = new_serv_resp.start_handle + 7;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_3_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 7,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_INDICATE,
      data,
      240 /*sizeof (data)*/,
      0x80);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_4_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 9,
      RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ,
      new_serv_resp.start_handle + 10,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_att4_val_hndl = new_serv_resp.start_handle + 10;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 10,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ,
      data,
      40, 
      0);
#endif


}

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
 * @fn         rsi_ble_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  memcpy (remote_dev_bd_addr, resp_enh_conn->dev_addr, 6);
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
  memcpy (remote_dev_bd_addr, resp_conn->dev_addr, 6);
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
 * This Callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  memcpy (remote_dev_bd_addr, resp_disconnect->dev_addr, 6);
  LOG_PRINT ("rsi_ble_on_disconnect_event \r\n");
  rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
}



void rsi_ble_on_mtu_event(rsi_ble_event_mtu_t *mtu) 
{
  memcpy (&ble_mtu, mtu, sizeof (rsi_ble_event_mtu_t));
  rsi_ble_app_set_event (RSI_BLE_REMOTE_MTU);
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
  LOG_PRINT ("\n Received Gatt write event on Handle = 0X%x \n", *((uint16_t *)rsi_ble_write->handle));
  if ((rsi_ble_att3_val_hndl + 1) == *((uint16_t *)rsi_ble_write->handle)) {
    LOG_PRINT ("\n matched \n ");
		rsi_ble_app_set_event (RSI_BLE_GATT_WRITE_EVENT);
  }
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
  if(resp_status == 0x0103) {
    rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
    return;
  }
#if 0
  rsi_ble_app_set_event (RSI_BLE_GATT_PROFILES);
#endif
  LOG_PRINT("ble_profile_list resp_status %x \n\n",resp_status);
  if(resp_status != RSI_SUCCESS) {
    if(resp_status == 0x4a0a) {
      rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
    } else if( resp_status == 0x4a05) {
      rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
    }
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
    last_handle = *(uint16_t *)ble_profiles.profile_desc[ble_profiles.number_of_profiles - 1].end_handle; 

    if(last_handle == 0xFFFF) {
      LOG_PRINT ("last handle = 0X%X \n",last_handle);
      sleep(5);
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
    }
    rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);

  }
  return;
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
	LOG_PRINT ("remote addr: %s, passkey: %.6s \r\n", 
      rsi_6byte_dev_address_to_ascii (str_remote_address, smp_passkey_display->dev_addr),
      smp_passkey_display->passkey );
    rsi_ble_app_set_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);

}
  
void rsi_ble_on_sc_passkey (rsi_bt_event_sc_passkey_t *sc_passkey)
{
  memcpy (remote_dev_bd_addr, sc_passkey->dev_addr, 6);
	LOG_PRINT ("remote addr: %s, passkey: %06d \r\n", 
      rsi_6byte_dev_address_to_ascii (str_remote_address, sc_passkey->dev_addr),
      sc_passkey->passkey );
    numeric_value = sc_passkey->passkey;
    rsi_ble_app_set_event (RSI_BLE_SC_PASSKEY_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_le_ltk_req_event
 * @brief      invoked when ltk request event is received
 * @param[in]  le_ltk_req, ltk request parameters.
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_le_ltk_req_event(rsi_bt_event_le_ltk_request_t *le_ltk_req)
{
  LOG_PRINT (" \r\n rsi_ble_on_le_ltk_req_event \r\n");
  memcpy(&ble_ltk_req, le_ltk_req, sizeof(rsi_bt_event_le_ltk_request_t));
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
            limited_device_found = 1;
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
  if (enc_enabled->localediv != 0) {
    memcpy (&encrypt_keys, enc_enabled, sizeof (rsi_bt_event_encryption_enabled_t));
    LOG_PRINT ("\n\n enabled value = %x \n\n", enc_enabled->enabled);
  }
  rsi_ble_app_set_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
}

static void rsi_ble_on_adv_reports (rsi_ble_event_adv_report_t *adv_report)
{
  if (device_found == 1) {
    return;
  }

  BT_LE_ADPacketExtractLocal (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);

  LOG_PRINT("\nreport_type = %d",adv_report->report_type);
  
  if((device_found==0) && ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)&&((strcmp(remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  {
#if (GAP_DM_LEP_BV_04_C)
    if(adv_report->report_type == 0x00)
    {
      device_found = 1;
    }
#elif (GAP_DM_LIN_BV_01_C)
    if(limited_device_found) {
      device_found = 1;
    }
#else
    device_found=1;
#endif
    memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr , 6);
    rsi_ble_app_set_event (RSI_BLE_ADV_REPORT_EVENT);
  }
}

static rsi_ble_on_read_att_value_event (uint16_t resp_status, rsi_ble_event_read_by_type3_t *read_att_value)
{
  uint8_t ix;
  LOG_PRINT("\nble_on_read resp_status %x",resp_status);

  LOG_PRINT ("\n ble_read_val length : %d",read_att_value->length);
  
  LOG_PRINT ("\n ble_read_val data : ");
  for(ix=0; ix<= read_att_value->length; ix++)
  {
  LOG_PRINT ("%C",read_att_value->att_value[ix]);
  }
  rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
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
    if(resp_status == 0x4a0a) {
    }
  }else {
    LOG_PRINT ("profile uuid = 0X%x \n",rsi_ble_resp_profile->profile_uuid.val);
    LOG_PRINT ("profile start handle = 0X%x \n",*(uint16_t *)rsi_ble_resp_profile->start_handle);
    LOG_PRINT ("profile end handle = 0X%x \n\n",*(uint16_t *)rsi_ble_resp_profile->end_handle);
    //    rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
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
    if(resp_status == 0x4a0a || resp_status == 0x0103) {
      LOG_PRINT ("\nATT error response / NO Attributes found");
    }
  }else {
    LOG_PRINT ("\n no.of include services = %d",rsi_ble_resp_inc_services->num_of_services );
    for(ix=0;ix<rsi_ble_resp_inc_services->num_of_services; ix++)
    {
      LOG_PRINT ("\n inc service handle = %x",rsi_ble_resp_inc_services->services[ix].handle);
      LOG_PRINT ("\n inc service handle uuid  = %x",rsi_ble_resp_inc_services->services[ix].inc_serv.uuid.val);
      LOG_PRINT ("\n inc service start handle = %x",rsi_ble_resp_inc_services->services[ix].inc_serv.start_handle);
      LOG_PRINT ("\n inc service end handle   = %x",rsi_ble_resp_inc_services->services[ix].inc_serv.end_handle);
    }

    last_handle = rsi_ble_resp_inc_services->services[ix-1].handle + 1;

    rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);

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
    // rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
  }else {
    memcpy(&ble_desc_services, rsi_ble_resp_att_desc, sizeof (rsi_ble_resp_att_descs_t));
    rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
    return;
  }
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
  if(resp_status != RSI_SUCCESS) {
    if(resp_status == 0x0103)
    {
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
    }
    else if(resp_status == 0x4a05)
    {
      LOG_PRINT(" \nAtt error Response - Insufficient Authentication error \n");
      //rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
      //rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
    }
    else if (resp_status == 0x4a01) {
      LOG_PRINT ("\n Att error Response - Invalid handle error \n");
    }
    else if (resp_status == 0x4a03) {
      LOG_PRINT ("\n Att error Response - Write not Permitted \n");
    }
    else if (resp_status == 0x4a0c) {
      LOG_PRINT ("\n Att error Response - Insufficient Encryption Key Size \n");
    }
    else if (resp_status == 0x4a0a) {
      LOG_PRINT ("\n Att error Response - Attribute Not Found \n");
    }
    else if (resp_status == 0x4a0d) {
      LOG_PRINT ("\n Att error Response - Attribute Length Invalid \n");
    }
    else if (resp_status == 0x4a07) {
      LOG_PRINT ("\n Att error Response - Attribute Invalid Offset \n");
//      rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
      return;
    } else if(resp_status >= 0x4a80) {
      LOG_PRINT ("\n Att error Response - Appilication error \n");
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
    }

  }else { 
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
    if(resp_status == 0x4a05)
    {
      LOG_PRINT(" \nAtt error Response - Insufficient Authentication error \n");
      LOG_PRINT(" \nAuthentication required");
      rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
      //rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
    }
    else if (resp_status == 0x4a01) {
      LOG_PRINT ("\n Att error Response - Invalid handle error \n");
    }
    else if (resp_status == 0x4a02) {
      LOG_PRINT ("\n Att error Response - Read not Permitted \n");
    }
    else if (resp_status == 0x4a0c) {
      LOG_PRINT ("\n Att error Response - Insufficient Encryption Key Size \n");
    }
    else if (resp_status == 0x4a0a) {
      LOG_PRINT ("\n Att error Response - Attribute Not Found \n");
    }
    else if (resp_status == 0x4a07) {
      LOG_PRINT ("\n Att error Response - Attribute Invalid Offset \n");
    }
    else if(resp_status >= 0x4a80) {
      LOG_PRINT ("\n Att error Response - Appilication error \n");
    }

  }else { 

    memcpy(&ble_read_val, rsi_ble_resp_att_val, sizeof(rsi_ble_resp_att_value_t));
    //	rsi_ble_app_set_event (RSI_BLE_GATT_READ);
  }
  return;
}



void rsi_ble_on_read_req_event (uint16_t resp_status, rsi_ble_read_req_t *read_req)
{
  LOG_PRINT ("gatt rd \n");
  memcpy (&app_ble_read_event, read_req, sizeof (rsi_ble_read_req_t));
  LOG_PRINT ("\nread offset= %x", app_ble_read_event.offset);
  rsi_ble_app_set_event(RSI_BLE_EVENT_GATT_RD);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_chat_gatt_server
 * @brief      this is the application of simple BLE chat application.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This function is used to test the simple chat application.
 */
int32_t rsi_ble_simple_chat_gatt_server (void)
{
  int32_t status = 0;
  int32_t  event_id;
  int8_t   data[20] = {0};
  //	uint8_t  remote_dev_addr[18] = {0};
  uuid_t   search_serv={0};
  rsi_ble_resp_local_att_value_t  local_att_val;
  uint8_t adv[31] = {2,1,6};
  static uint8_t loop = 0;

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

  //! adding simple BLE chat service
  rsi_ble_add_simple_chat_serv ();

  //! registering the GAP callback functions
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

  //! registering the GATT callback functions
  rsi_ble_gatt_register_callbacks (rsi_ble_profiles_list,
      rsi_ble_profile,
      NULL,
      rsi_ble_inc_services,
      NULL,
      rsi_ble_on_read,
      rsi_ble_write,
      rsi_ble_on_gatt_write_event,
      NULL,
      NULL,
      rsi_ble_on_read_req_event,
      rsi_ble_on_mtu_event,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      rsi_ble_on_read_att_value_event,
      NULL,
      NULL,
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

  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! Set local name
  rsi_bt_set_local_name(RSI_BLE_APP_SIMPLE_CHAT);

  //! prepare advertise data //local/device name
  adv[3] = strlen (RSI_BLE_APP_SIMPLE_CHAT) + 1;
  adv[4] = 9;
  strcpy ((char *)&adv[5], RSI_BLE_APP_SIMPLE_CHAT);

  //! set advertise data
  rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_APP_SIMPLE_CHAT) + 5);

  //! add revote deivce to white-list
  status = rsi_ble_addto_whitelist(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_bd_addr,RSI_BLE_DEV_ADDR), RSI_BLE_DEV_ADDR_TYPE);
  if(status != RSI_SUCCESS){
    return status;
  }

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
    return status;
  }

#if (ROLE == SLAVE)
  //! set device in advertising mode.
  rsi_ble_start_advertising ();
#else
  rsi_ble_start_scanning ();
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
      case RSI_BLE_CONN_EVENT:
        {
          //! event invokes when connection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);
        }
        break;

      case RSI_BLE_DISCONN_EVENT:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
          device_found = 0;
          sleep(3);

#if (ROLE == SLAVE)
          //! set device in advertising mode.
adv:
          status = rsi_ble_start_advertising ();
          if (status != RSI_SUCCESS) {
            sleep(5);
            goto adv;
          }
#else 
          status = rsi_ble_start_scanning();
scan:
          if (status != RSI_SUCCESS) {
            sleep(5);
            goto scan;
          }

#endif
        }
        break;

      case RSI_BLE_GATT_WRITE_EVENT:
        {
          //! event invokes when write/notification events received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE_EVENT);

          LOG_PRINT ("\n set local att value\n");
          //! set the local attribute value.
          rsi_ble_app_data[0]= 0x01;
          rsi_ble_set_local_att_value (rsi_ble_att3_val_hndl, RSI_BLE_MAX_DATA_LEN, rsi_ble_app_data);
        }
        break;
      case RSI_BLE_LTK_REQ_EVENT:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_LTK_REQ_EVENT);
          //rsi_6byte_dev_address_to_ascii (remote_dev_addr,ble_ltk_req.dev_addr);

#if (ROLE == SLAVE)
          if((ble_ltk_req.localediv == encrypt_keys.localediv) && 
              !((memcmp(ble_ltk_req.localrand, encrypt_keys.localrand,8)))
            )
          {
            LOG_PRINT("\n positive reply\n");
            //! give le ltk req reply cmd with positive reply
            status = rsi_ble_ltk_req_reply(ble_ltk_req.dev_addr, (1 | encrypt_keys.enabled), encrypt_keys.localltk);
          }
          else
          {
            LOG_PRINT("\n negative reply\n");
            //! give le ltk req reply cmd with negative reply
            status = rsi_ble_ltk_req_reply(ble_ltk_req.dev_addr, 0, NULL);
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
          status = rsi_ble_smp_pair_request (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
        }
        break;

      case RSI_BLE_SMP_RESP_EVENT:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_SMP_RESP_EVENT);

          //! initiating the SMP pairing process
          status = rsi_ble_smp_pair_response (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
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
            status = rsi_ble_smp_passkey (remote_dev_bd_addr, smp_passkey);
          }else {
            status = rsi_ble_smp_passkey (remote_dev_bd_addr, RSI_BLE_SMP_PASSKEY);
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
          //          status = rsi_ble_disconnect (remote_dev_bd_addr);

        }
        break;
      case RSI_BLE_ENCRYPT_STARTED_EVENT:
        {
          //! start the encrypt event

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
          already_paired = 1;

#if (GAP_SEC_AUT_BV_24_C)
          rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
#endif
        }
        break;

      case RSI_BLE_ADV_REPORT_EVENT:
        {
          //! advertise report event.
          //! clear the advertise report event.
          rsi_ble_app_clear_event (RSI_BLE_ADV_REPORT_EVENT);
          //status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_bd_addr);
          status = rsi_ble_connect(0, rsi_ascii_dev_address_to_6bytes_rev(remote_dev_bd_addr,RSI_BLE_DEV_ADDR));
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("\n connect status: 0x%X", status);
          }     
        }
        break;
      case RSI_BLE_DISCONNECT:
        {
          rsi_ble_app_clear_event (RSI_BLE_DISCONNECT);

          dbg_flag = 0;
          sleep(5);
          rsi_ble_disconnect (remote_dev_bd_addr);
        }
        break;
      case RSI_BLE_REMOTE_MTU:
        {
          rsi_ble_app_clear_event (RSI_BLE_REMOTE_MTU);

        }
        break;
      case RSI_BLE_GATT_LONG_WRITE:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_LONG_WRITE);
        }
        break;

      case RSI_BLE_GATT_PROFILE:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILE);
          sleep(2);

          last_handle =(uint16_t )ble_char_services.char_services[ble_char_services.num_of_services - 1].char_data.char_handle + 1; 

          LOG_PRINT ("\nhandle = 0X%x ",last_handle);
          rsi_ble_get_char_services (remote_dev_bd_addr, last_handle, 0xFFFF, &ble_char_services);
        }
        break;
      case RSI_BLE_GATT_PROFILES:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILES);
          sleep(5);
        }
        break;

      case RSI_BLE_GATT_CHAR_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_SERVICES);
          sleep(2);
          rsi_ble_get_inc_services (remote_dev_bd_addr, last_handle, 0xFFFF, &ble_inc_serv);
        }
        break;

      case RSI_BLE_GATT_DESC_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_SERVICES);
          rsi_ble_get_att_value (remote_dev_bd_addr,*(uint16_t *)ble_desc_services.att_desc[0].handle, &ble_read_val);
        }
        break;

      case RSI_BLE_GATT_READ:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_READ);
          sleep(3);
          rsi_ble_get_char_value_by_uuid (remote_dev_bd_addr, f_handle, lst_handle, search_serv, &ble_read_val);
        }
        break;

      case RSI_BLE_EVENT_GATT_RD : 
        {
          rsi_ble_app_clear_event (RSI_BLE_EVENT_GATT_RD);
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
  
  //! Call Simple chat GATT Server Application
  rsi_ble_simple_chat_gatt_server();

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
  rsi_task_create(rsi_ble_simple_chat_gatt_server, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
