/**
 * @file    rsi_ble_app.c
 * @version 0.1
 * @date    17 Nov 2015
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
 *  @section Description  This file contains example application for BLE simple chat
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
#include<stdio.h>
#include<string.h>
#include<rsi_pts_inc.h>
//#include<rsi_pts_gatt_inc.h>
//! Common include file 
#include<rsi_common_apis.h>

//! application defines 
#if 1//def RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#define LOG_SCAN                      scanf
#else 
#define LOG_PRINT
#define LOG_SCAN                      
#endif

//! Devide role
#define MASTER    0
#define SLAVE     1
#if (GAP_DM_BON_BV_01_C || GAP_DM_GIN_BV_01_C || GAP_DM_LIN_BV_01_C || GAP_DM_NAD_BV_02_C || GAP_DM_LEP_BV_04_C \
    || GAP_DM_LEP_BV_06_C || GAP_DM_LEP_BV_11_C || GAP_DM_LEP_BV_09_C || GATT_CL_GAC_BV_01_C || GATT_CL_GAD_BV_01_C \
    || GATT_CL_GAD_BV_02_C || GATT_CL_GAD_BV_03_C || GATT_CL_GAD_BV_04_C || GATT_CL_GAT_BV_01_C || GATT_CL_GAR_BI_13_C \
    || GATT_CL_GAR_BI_18_C || GATT_CL_GAR_BI_19_C || GATT_CL_GAW_BI_09_C || GATT_CL_GAW_BI_32_C || GATT_CL_GAW_BV_06_C \
    || GATT_CL_GAR_BI_05_C || GATT_CL_GAD_BV_05_C || GATT_CL_GAD_BV_06_C || GATT_CL_GAR_BV_01_C || GATT_CL_GAR_BI_01_C \
    || GATT_CL_GAR_BI_02_C || GATT_CL_GAR_BI_04_C || GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_07_C \
    || GATT_CL_GAR_BI_10_C || GATT_CL_GAR_BI_11_C || GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BI_12_C || GATT_CL_GAR_BI_14_C \
    || GATT_CL_GAR_BI_17_C || GATT_CL_GAR_BV_05_C || GATT_CL_GAR_BI_21_C || GATT_CL_GAR_BI_22_C || GATT_CL_GAR_BV_06_C \
    || GATT_CL_GAR_BV_07_C || GATT_CL_GAR_BI_35_C || GATT_CL_GAW_BV_01_C || GATT_CL_GAW_BV_03_C || GATT_CL_GAW_BI_02_C \
    || GATT_CL_GAW_BI_03_C || GATT_CL_GAW_BI_05_C || GATT_CL_GAW_BI_06_C || GATT_CL_GAW_BV_05_C || GATT_CL_GAW_BI_07_C \
    || GATT_CL_GAW_BI_08_C || GATT_CL_GAW_BI_12_C || GATT_CL_GAW_BI_13_C || GATT_CL_GAW_BV_08_C || GATT_CL_GAW_BV_09_C \
    || GATT_CL_GAW_BI_33_C || GATT_CL_GAW_BI_34_C || GATT_CL_GAN_BV_01_C || GATT_CL_GAI_BV_01_C || GATT_CL_GAS_BV_01_C \
    || GATT_CL_GAT_BV_02_C)
#define ROLE      MASTER 
#else
#define ROLE      SLAVE
#endif

//! maximum advertise list
#define  NO_OF_ADV_REPORTS  5
#define RSI_BLE_DEV_ADDR_TYPE                      LE_PUBLIC_ADDRESS 
#define RSI_BLE_DEV_ADDR                          "00:1B:DC:F2:1C:54"
//#define RSI_BLE_OWN_DEV_RAND_ADDR                 "00:11:22:33:44:55"

//! Memory length for driver
//#define BT_GLOBAL_BUFF_LEN   10000
#define BLE_DATA             0

//! Memory to initialize driver
//uint8_t bt_global_buf[BT_GLOBAL_BUFF_LEN];

//! BLE attribute service types uuid values
#define  RSI_BLE_CHAR_SERV_UUID         0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID       0x2902

//! BLE characteristic service uuid
#define  RSI_BLE_NEW_SERVICE_UUID       0xAABB
#define  RSI_BLE_ATTRIBUTE_1_UUID       0x1AA1
#define  RSI_BLE_ATTRIBUTE_2_UUID       0x1BB1
#define  RSI_BLE_ATTRIBUTE_3_UUID       0x1CC1
#define  RSI_BLE_ATTRIBUTE_4_UUID       0x1DD1
#define  RSI_BLE_ATTRIBUTE_5_UUID       0x1EE1
#define  RSI_BLE_ATTRIBUTE_6_UUID       0x1FF1

//! max data length
#define  RSI_BLE_MAX_DATA_LEN           20

//! local device name
#define RSI_BLE_APP_DEVICE_NAME        "BLE_DUAL_MODE_SIMPLE_CHAT"

//! attribute properties
#define  RSI_BLE_ATT_PROPERTY_READ      0x02
#define  RSI_BLE_ATT_PROP_WR_NO_RESP    0x04
#define  RSI_BLE_ATT_PROPERTY_WRITE     0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY    0x10
#define  RSI_BLE_ATT_PROPERTY_INDICATE  0x20

//! service security defines
//! Config_bitmap values
#define  ATT_REC_MAINTAINDE_IN_HOST     BIT(0)
#define  SEC_MODE_1_LEVEL_1             BIT(1)
#define  SEC_MODE_1_LEVEL_2             BIT(2)
#define  SEC_MODE_1_LEVEL_3             BIT(3)
#define  SEC_MODE_1_LEVEL_4             BIT(4)
#define  ON_BR_EDR_LINK_ONLY            BIT(5)
#define  ON_LE_LINK_ONLY                BIT(6)

//! application event list
#define  RSI_BLE_CONN_EVENT             0x01
#define  RSI_BLE_DISCONN_EVENT          0x02
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
#define  RSI_BLE_GATT_INC_SERVICES      0x17
#define  RSI_BLE_GATT_WRITE             0x18
#define  RSI_BLE_GATT_NOTIFY_EVENT      0x19
#define  RSI_BLE_GATT_INDICATE_EVENT    0x20




//! local device IO capability
#define  RSI_BLE_SMP_IO_CAPABILITY      0x01
#define  RSI_BLE_SMP_PASSKEY            0

#define  MITM_REQ                       1


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
#if (GATT_CL_GAD_BV_04_C )
uint16_t start_handle = 0;
uint16_t end_handle = 0;
static uint8_t dbg_flag;
#endif

#if (GATT_CL_GAD_BV_06_C || GATT_CL_GPA_BV_05_C || GATT_CL_GPA_BV_06_C || GATT_CL_GPA_BV_07_C || GATT_CL_GPA_BV_08_C) 
static uint16_t s_handle;
static uint16_t e_handle;
#endif


//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static rsi_ble_event_conn_status_t app_conn_event;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t app_ble_write_event;
static uint8_t rsi_ble_app_data[RSI_BLE_MAX_DATA_LEN];
static uint16_t rsi_ble_app_data_len;
static uint16_t rsi_ble_att1_val_hndl;
static uint16_t rsi_ble_att2_val_hndl;
static uint16_t rsi_ble_att3_val_hndl;
static uint16_t rsi_ble_att4_val_hndl;
static uint16_t rsi_ble_att5_val_hndl;
static uint16_t rsi_ble_att6_val_hndl;
static rsi_ble_event_mtu_t ble_mtu;

extern bt_done;
extern void intimate_bt();
extern void rsi_ble_app_send_to_bt (uint8_t  data_type, uint8_t *data, uint16_t data_len);

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
  new_att.config_bitmap = (flag & 0x7F);

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
  uint8_t   data1[20] = {0x01,0x00,0x00};


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
      0x44);

#if 1
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
      data1,
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
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_4_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 10,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ,
      data,
      240, 
      0);


//! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_5_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 11,
      RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ,
      new_serv_resp.start_handle + 12,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_att5_val_hndl = new_serv_resp.start_handle + 12;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_5_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 12,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ,
      data,
      10, 
      ON_BR_EDR_LINK_ONLY);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_6_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 13,
      RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ,
      new_serv_resp.start_handle + 14,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_att6_val_hndl = new_serv_resp.start_handle + 14;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_5_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 14,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ,
      data,
      10, 
      ON_LE_LINK_ONLY);
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
 * @brief      sets the specific event.
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
 * This Callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  LOG_PRINT ("rsi_ble_on_disconnect_event \r\n");
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_mtu_event
 * @brief      invoked when remote mtu event is received
 * @param[out] mtu, remote mtu size.
 * @return     none.
 * @section description
 * This Callback function indicates the remote mtu size
 * */

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
#if (GATT_CL_GAN_BV_01_C || GATT_CL_GAI_BV_01_C || GATT_CL_GAS_BV_01_C || GATT_CL_GAW_BV_05_C)
  rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
  return;
#endif

  if ((rsi_ble_att3_val_hndl + 1) == *((uint16_t *)rsi_ble_write->handle)) {
    LOG_PRINT ("\n matched \n ");
    if (rsi_ble_write->att_value[0]) {
#if (GATT_SR_GPM_BV_01_C)     
      if (rsi_ble_write->att_value[0] == 0x01) {
        rsi_ble_app_set_event (RSI_BLE_GATT_NOTIFY_EVENT);
      } else if(rsi_ble_write->att_value[0] == 0x02)  {
        rsi_ble_app_set_event (RSI_BLE_GATT_INDICATE_EVENT);
      }
#else
#if (ROLE == SLAVE)
      rsi_ble_app_set_event (RSI_BLE_GATT_WRITE_EVENT);
#endif
#endif
    }

  }

//  rsi_ble_app_send_to_bt (BLE_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
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
#if (GATT_CL_GAD_BV_01_C || GATT_CL_GPA_BV_01_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
      rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
    } else if( resp_status == 0x4a05) {
      rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
    }
  }else {
    memcpy(&ble_profiles, rsi_ble_resp_profiles, sizeof(rsi_ble_resp_profiles_list_t));
    LOG_PRINT("no of profiles = %d \n",ble_profiles.number_of_profiles);
    //LOG_PRINT("reserved value = %d \n",*(uint32_t *)ble_profiles.reserved);

    for(ix=0;ix < ble_profiles.number_of_profiles; ix++)
    {
      LOG_PRINT ("profile uuid size = 0X%x \n",ble_profiles.profile_desc[ix].profile_uuid.size);
      LOG_PRINT ("profile uuid = 0X%x \n",ble_profiles.profile_desc[ix].profile_uuid.val);
      LOG_PRINT ("profile start handle = 0X%x \n",*(uint16_t *)ble_profiles.profile_desc[ix].start_handle);
      LOG_PRINT ("profile end handle = 0X%x \n\n",*(uint16_t *)ble_profiles.profile_desc[ix].end_handle);
    }
    last_handle = *(uint16_t *)ble_profiles.profile_desc[ble_profiles.number_of_profiles - 1].end_handle; 

#if (GATT_CL_GAD_BV_01_C)
    if(last_handle == 0xFFFF) {
      LOG_PRINT ("last handle = 0X%X \n",last_handle);
      sleep(5);
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
    }
#endif

    rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);

  }
  return;
}
/*==============================================*/
/**
 * @fn         rsi_bt_app_send_to_ble
 * @brief      data transfer from BT module to BLE module.
 * @param[in]  data_type, data type, now we are using sample data type '0'.
 * @param[in]  data, raw data received from connected BT device..
 * @param[in]  data_len, raw data length
 * @return     none.
 * @section description
 * This function is used to initialize the BLE module
 */

void rsi_bt_app_send_to_ble (uint8_t  data_type, uint8_t *data, uint16_t data_len) 
{
  uint16_t rx_len;
  if ((data == NULL) || 
      (data_len == 0)) {
    return;
  }
  //app_ble_write_event.length = RSI_MIN (RSI_BLE_MAX_DATA_LEN, data_len);
  //memset (app_ble_write_event.att_value, 0, sizeof (app_ble_write_event.att_value));
	//memcpy (&app_ble_write_event.att_value, data, app_ble_write_event.length);
  rx_len = RSI_MIN (RSI_BLE_MAX_DATA_LEN, data_len);
  memset (rsi_ble_app_data, 0, RSI_BLE_MAX_DATA_LEN);
  memcpy (rsi_ble_app_data, data, rx_len);
  rsi_ble_app_set_event (RSI_BLE_GATT_WRITE_EVENT);
}

void intimate_gatt_ble ( uint8_t * rem_add)
    
{
//  strcpy (remote_dev_addr , rem_add);
  LOG_PRINT ("\n intimation to BLE - remote device address : %s \n\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, rem_add));
  
  memcpy(remote_dev_bd_addr, rem_add, 6);
  memcpy(app_conn_event.dev_addr,rem_add, 6);
#if (GATT_CL_GAC_BV_01_C || GATT_CL_GAW_BV_05_C || GATT_CL_GAW_BI_07_C || GATT_CL_GAW_BI_08_C || GATT_CL_GAW_BI_09_C \
    || GATT_CL_GAW_BI_12_C || GATT_CL_GAW_BV_09_C || GATT_CL_GAW_BI_34_C || GATT_CL_GAW_BI_32_C || GATT_CL_GAW_BV_06_C)
  rsi_ble_app_set_event (RSI_BLE_GATT_LONG_WRITE);
#elif (GATT_CL_GAD_BV_01_C || GATT_CL_GPA_BV_01_C)
  sleep(2);
  LOG_PRINT ("\nlast handle = 0x%x",last_handle);
  rsi_ble_get_profiles (app_conn_event.dev_addr, last_handle, 0xffff, &ble_profiles);
#elif (GATT_CL_GAD_BV_02_C )
  rsi_ble_app_set_event (RSI_BLE_GATT_PROFILES);
#elif (GATT_CL_GAD_BV_03_C)
  rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
#elif (GATT_CL_GAD_BV_04_C )
  rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
#elif (GATT_CL_GAR_BV_01_C || GATT_CL_GAR_BI_01_C || GATT_CL_GAR_BI_02_C || GATT_CL_GAR_BV_06_C || GATT_CL_GAR_BV_07_C \
    || GATT_CL_GAR_BI_34_C || GATT_CL_GAR_BI_35_C || GATT_CL_GAT_BV_01_C)
  rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
#elif (GATT_CL_GAD_BV_05_C || GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_07_C || GATT_CL_GAR_BI_10_C \
    || GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BI_12_C || GATT_CL_GAR_BI_13_C || GATT_CL_GAR_BI_14_C || GATT_CL_GAR_BV_05_C \
    || GATT_CL_GAR_BI_18_C || GATT_CL_GAR_BI_19_C || GATT_CL_GAR_BI_21_C || GATT_CL_GPA_BV_02_C || GATT_CL_GPA_BV_03_C \
    || GATT_CL_GPA_BV_04_C)
  rsi_ble_app_set_event (RSI_BLE_GATT_READ);
#elif (GATT_CL_GAD_BV_06_C || GATT_CL_GPA_BV_05_C || GATT_CL_GPA_BV_06_C || GATT_CL_GPA_BV_07_C || GATT_CL_GPA_BV_08_C)
  rsi_ble_app_set_event (RSI_BLE_GATT_INC_SERVICES);
#elif (GATT_CL_GAW_BV_01_C || GATT_CL_GAW_BV_03_C || GATT_CL_GAW_BI_02_C || GATT_CL_GAW_BI_03_C || GATT_CL_GAW_BI_05_C \
    || GATT_CL_GAW_BV_08_C || GATT_CL_GAW_BI_33_C || GATT_CL_GAN_BV_01_C || GATT_CL_GAI_BV_01_C || GATT_CL_GAT_BV_02_C)
  rsi_ble_app_set_event (RSI_BLE_GATT_WRITE);
#endif

}
void intimate_ble ()
{
#if (GATT_CL_GAT_BV_01_C)
  dbg_flag = 0;
#endif
  rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
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
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("\n\n smp_failed_status: 0x%x,  remote_dev_addr : %s\r\n", status,
      rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, remote_dev_address->dev_addr));
  
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
          if(flags & BIT(0)) {
            LOG_PRINT ("\n LE limited discoverble mode \n");
            limited_device_found = 1;
          } else if (flags & BIT(1)) {
            LOG_PRINT ("\n LE general discoverble mode \n");

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
  }
  rsi_ble_app_set_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
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

  BT_LE_ADPacketExtractLocal (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);

  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, RSI_DEV_ADDR_LEN);

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
    rsi_ble_app_set_event (RSI_BLE_ADV_REPORT_EVENT);
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
#if 0
static void rsi_ble_on_read (uint16_t resp_status, uint16_t resp_id, rsi_ble_resp_att_value_t *rsi_ble_resp_att_val)
{
  memcpy(&ble_read_val, rsi_ble_resp_att_val, sizeof(rsi_ble_resp_att_value_t));
  LOG_PRINT("\nble_on_read resp_status %x",resp_status);
  LOG_PRINT ("\n ble_read_val data : %s",&ble_read_val.att_value[3]);

}
#endif
#if 0
static rsi_ble_on_remote_mtu_event (rsi_ble_event_mtu_t *remote_mtu)
{
  LOG_PRINT ("\m remote mtu size : %d", remote_mtu->mtu_size);
  rsi_ble_app_set_event (RSI_BLE_REMOTE_MTU);
}
#endif

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
  LOG_PRINT ("\n ********** \n");
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
#if 0
  rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
#endif
  LOG_PRINT("ble_profile resp_status %x \n\n",resp_status);
  if(resp_status != RSI_SUCCESS) {
    if(resp_status == 0x4a0a) {
#if (GATT_CL_GAD_BV_02_C )
      sleep(5);
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }
  }else {
    LOG_PRINT ("profile uuid = 0X%x \n",rsi_ble_resp_profile->profile_uuid.val);
    LOG_PRINT ("profile start handle = 0X%x \n",*(uint16_t *)rsi_ble_resp_profile->start_handle);
    LOG_PRINT ("profile end handle = 0X%x \n\n",*(uint16_t *)rsi_ble_resp_profile->end_handle);
#if (GATT_CL_GAD_BV_02_C )
    sleep(5);
    rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
    return;
#endif
    //    rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
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
#if 0
  memcpy(&ble_char_services , rsi_ble_resp_char_services, sizeof(rsi_ble_resp_char_services_t));
  rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
#endif

  uint8_t ix;
  LOG_PRINT(" \n ble_char services resp_status %x \n\n",resp_status);
  if(resp_status != RSI_SUCCESS) {
#if (GATT_CL_GAD_BV_04_C )
    if((resp_status == 0x0103) || (resp_status == 0x4a0a)) {
      sleep(3);
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
    }
#endif
    rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
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
#if (GATT_CL_GAD_BV_04_C )
    sleep(3);
    rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
#endif
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
#if (GATT_CL_GAD_BV_03_C)
      sleep(5);
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
#endif
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

#if (GATT_CL_GAD_BV_03_C)
    if(last_handle == 0xFFFF) {
      LOG_PRINT ("last handle = 0X%X \n",last_handle);
      sleep(5);
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
    }
#endif
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
#if (GATT_CL_GAD_BV_06_C || GATT_CL_GPA_BV_05_C || GATT_CL_GPA_BV_06_C || GATT_CL_GPA_BV_07_C || GATT_CL_GPA_BV_08_C)
    LOG_PRINT ("\n no of attributes : %d", rsi_ble_resp_att_desc->num_of_att);
    for(ix=0;ix< rsi_ble_resp_att_desc->num_of_att ; ix++)
    {
      LOG_PRINT ("\n att_desc handle : %x", *(uint16_t *)rsi_ble_resp_att_desc->att_desc[ix].handle);
      LOG_PRINT ("\n att_desc uuid : %x", (uint16_t *)rsi_ble_resp_att_desc->att_desc[ix].att_type_uuid.val.val16);
      LOG_PRINT ("\n");
    }
#if (GATT_CL_GPA_BV_05_C || GATT_CL_GPA_BV_06_C || GATT_CL_GPA_BV_07_C || GATT_CL_GPA_BV_08_C)
    rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
    return;
#endif
    sleep(5);
    rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
    return;
#endif
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
#if (GATT_CL_GAW_BI_05_C || GATT_CL_GAW_BI_12_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
      //rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
      //rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
    }
    else if (resp_status == 0x4a01) {
      LOG_PRINT ("\n Att error Response - Invalid handle error \n");
#if (GATT_CL_GAW_BI_02_C || GATT_CL_GAW_BI_07_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }
    else if (resp_status == 0x4a03) {
      LOG_PRINT ("\n Att error Response - Write not Permitted \n");
#if (GATT_CL_GAW_BI_03_C || GATT_CL_GAW_BI_08_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }
    else if (resp_status == 0x4a0c) {
      LOG_PRINT ("\n Att error Response - Insufficient Encryption Key Size \n");
#if (GATT_CL_GAW_BI_06_C || GATT_CL_GAW_BI_13_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }
    else if (resp_status == 0x4a0a) {
      LOG_PRINT ("\n Att error Response - Attribute Not Found \n");
#if (GATT_CL_GAW_BI_09_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }
    else if (resp_status == 0x4a0d) {
      LOG_PRINT ("\n Att error Response - Attribute Length Invalid \n");
#if (GATT_CL_GAW_BI_33_C || GATT_CL_GAW_BI_34_C || GATT_CL_GAW_BI_09_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
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
#if (GATT_CL_GAC_BV_01_C  ||  GATT_CL_GAW_BV_01_C || GATT_CL_GAW_BV_03_C || GATT_CL_GAW_BV_08_C || GATT_CL_GAW_BI_32_C \
    || GATT_CL_GAW_BV_06_C || GATT_CL_GAW_BV_05_C || GATT_CL_GAW_BV_09_C)
    rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
    return;
#endif
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
#if (GATT_CL_GAR_BI_10_C || GATT_CL_GAR_BI_21_C || GATT_CL_GAR_BI_04_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
#else
      LOG_PRINT(" \nAuthentication required");
      rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
#endif
      //rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
    }
    else if (resp_status == 0x4a01) {
      LOG_PRINT ("\n Att error Response - Invalid handle error \n");
#if (GATT_CL_GAR_BI_01_C || GATT_CL_GAR_BI_14_C || GATT_CL_GAR_BI_19_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }
    else if (resp_status == 0x4a02) {
      LOG_PRINT ("\n Att error Response - Read not Permitted \n");
#if (GATT_CL_GAR_BI_02_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_12_C || GATT_CL_GAR_BI_18_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }
    else if (resp_status == 0x4a0c) {
      LOG_PRINT ("\n Att error Response - Insufficient Encryption Key Size \n");
#if (GATT_CL_GAR_BI_05_C || GATT_CL_GAR_BI_11_C || GATT_CL_GAR_BI_17_C || GATT_CL_GAR_BI_22_C )
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }
    else if (resp_status == 0x4a0a) {
      LOG_PRINT ("\n Att error Response - Attribute Not Found \n");
#if (GATT_CL_GAR_BI_07_C || GATT_CL_GAD_BV_05_C || GATT_CL_GPA_BV_02_C || GATT_CL_GPA_BV_03_C || GATT_CL_GPA_BV_04_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }
    else if (resp_status == 0x4a07) {
      LOG_PRINT ("\n Att error Response - Attribute Invalid Offset \n");
#if (GATT_CL_GAR_BV_07_C)
      dbg_flag =0;
      length=0;
      rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
      return;
#endif
#if (GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BI_13_C ) 
#if (GATT_CL_GAR_BI_13_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
#else
      dbg_flag = 0;
      rsi_ble_app_set_event(RSI_BLE_GATT_READ);
#endif
      return;
#endif
    } else if(resp_status >= 0x4a80) {
      LOG_PRINT ("\n Att error Response - Appilication error \n");
#if (GATT_CL_GAR_BI_35_C || GATT_CL_GAR_BI_34_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
      return;
#endif
    }

  }else { 

    memcpy(&ble_read_val, rsi_ble_resp_att_val, sizeof(rsi_ble_resp_att_value_t));
#if (GATT_CL_GAR_BV_01_C || GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BV_05_C || GATT_CL_GAR_BV_06_C || GATT_CL_GAR_BV_07_C || GATT_CL_GPA_BV_02_C || GATT_CL_GPA_BV_03_C || GATT_CL_GPA_BV_04_C || GATT_CL_GPA_BV_05_C || GATT_CL_GPA_BV_06_C || GATT_CL_GPA_BV_07_C || GATT_CL_GPA_BV_08_C)

    LOG_PRINT ("\n ble_read_val length : %d",ble_read_val.len);
    LOG_PRINT ("\n ble_read_val data : %s \n",&ble_read_val.att_value[1]);

#if (GATT_CL_GAR_BV_07_C)
    rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
    return;
#endif
#if (GATT_CL_GAR_BV_04_C )
    rsi_ble_app_set_event (RSI_BLE_GATT_READ);
    return;
#endif
#if (GATT_CL_GPA_BV_02_C || GATT_CL_GPA_BV_03_C || GATT_CL_GPA_BV_04_C || GATT_CL_GPA_BV_05_C || GATT_CL_GPA_BV_06_C || GATT_CL_GPA_BV_07_C || GATT_CL_GPA_BV_08_C)
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
#endif
#if (GATT_CL_GAR_BV_03_C)
    if(uuid_size == 2)
      rsi_ble_app_set_event (RSI_BLE_GATT_READ);
    else
#endif
      rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
    return;
#endif
    //	rsi_ble_app_set_event (RSI_BLE_GATT_READ);
  }
  return;
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

#if (GATT_CL_GAD_BV_05_C || GATT_CL_GAD_BV_02_C || GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_07_C \
    || GATT_CL_GAR_BI_11_C || GATT_CL_GAR_BI_10_C || GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BI_12_C || GATT_CL_GAR_BI_13_C \
    || GATT_CL_GAR_BI_14_C || GATT_CL_GAR_BI_17_C || GATT_CL_GPA_BV_02_C || GATT_CL_GPA_BV_03_C || GATT_CL_GPA_BV_04_C)
void fun ()
{

  uint16_t d2= 0;
  uint16_t d3= 0;
  uint8_t d4[8]= {0x23, 0x01, 0x67, 0x45, 0xef, 0xcd, 0xab, 0x89};

  LOG_PRINT ("\n enter 128-bit uuid value: ");
  LOG_PRINT ("\n enter 4 bytes as xxxxxxxx :");
  LOG_SCAN ("%x",(&uuid_val.val.val128.data1));

  /* manually filling the remaining uuid data */
  memcpy ((&uuid_val.val.val128.data2), &d2 , 2);
  memcpy ((&uuid_val.val.val128.data3), &d3 , 2);
  memcpy ((&uuid_val.val.val128.data4), d4 , 8);

  return;
}
#endif

void rsi_ble_on_read_req_event (uint16_t resp_status, rsi_ble_read_req_t *read_req)
{
{
  LOG_PRINT ("gatt rd \n");
  memcpy (&app_ble_read_event, read_req, sizeof (rsi_ble_read_req_t));
  LOG_PRINT ("\nread offset= %x", app_ble_read_event.offset);
//  rsi_ble_app_set_event(RSI_BLE_EVENT_GATT_RD);
}

}

/*==============================================*/
/**
 * @fn         rsi_ble_app_init
 * @brief      initialize the BLE module.
 * @param[in]  none
 * @return     none.
 * @section description
 * This function is used to initialize the BLE module
 */
void rsi_ble_app_init (void)
{
	uint8_t adv[31] = {2,1,2};
  
  //!  initializing the application events map
  rsi_ble_app_init_events ();

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
      rsi_ble_char_services,
      rsi_ble_inc_services,
      rsi_ble_on_att_desc,
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

  //! Set local name
  rsi_bt_set_local_name (RSI_BLE_APP_DEVICE_NAME);

	//! prepare advertise data //local/device name
  adv[3] = strlen (RSI_BLE_APP_DEVICE_NAME) + 1;
  adv[4] = 9;
  strcpy ((char *)&adv[5], RSI_BLE_APP_DEVICE_NAME);

	//! set advertise data
	rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_APP_DEVICE_NAME) + 5);
#if 1
  smp_pairing_cap_data.io_capability = RSI_BLE_SMP_IO_CAPABILITY;
  smp_pairing_cap_data.oob_data_flag = OOB_DATA_FLAG;
  smp_pairing_cap_data.auth_req      = AUTH_REQ | (MITM_REQ << 2) ;
  smp_pairing_cap_data.enc_key_size  = ENC_KEY_SIZE;
  smp_pairing_cap_data.ini_key_distribution = INI_KEY_DIST;
  smp_pairing_cap_data.rsp_key_distribution = RSP_KEY_DIST;


  //! set smp pairing capability data
  rsi_ble_set_smp_pairing_cap_data(&smp_pairing_cap_data);
#endif

  rsi_ascii_dev_address_to_6bytes_rev (remote_dev_bd_addr, RSI_BLE_DEV_ADDR);
  rsi_ble_addto_whitelist(remote_dev_bd_addr, RSI_BLE_DEV_ADDR_TYPE);
#if (ROLE == SLAVE)
  
//  rsi_ble_set_random_address_with_value (RSI_BLE_OWN_DEV_RAND_ADDR);
  //! set device in advertising mode.
  rsi_ble_start_advertising ();
#else
#if (GATT_CL_GAR_BI_05_C || GAP_DM_NAD_BV_02_C || GAP_DM_LEP_BV_04_C || GAP_DM_LEP_BV_06_C || GAP_DM_LEP_BV_09_C \
    || GAP_DM_LEP_BV_11_C || GATT_CL_GAR_BI_04_C || GATT_CL_GAR_BI_10_C || GATT_CL_GAR_BI_11_C || GATT_CL_GAR_BI_17_C \
    || GATT_CL_GAR_BI_21_C || GATT_CL_GAR_BI_22_C || GATT_CL_GAR_BI_35_C || GATT_CL_GAW_BI_05_C || GATT_CL_GAW_BI_06_C \
    || GATT_CL_GAW_BI_12_C || GATT_CL_GAW_BI_13_C )
  rsi_ble_start_scanning();
#endif
#endif


}

/*==============================================*/
/**
 * @fn         rsi_ble_app_task
 * @brief      this function will execute when BLE events are raised.
 * @param[in]  none.
 * @return     none.
 * @section description
 */
void rsi_ble_app_task (void)
{
  int32_t status = 0;
  int32_t  event_id;
  int8_t   data[20] = {0};
  rsi_ble_resp_local_att_value_t  local_att_val;
  static uint8_t loop = 0;
  uuid_t   search_serv = {0};

  //! checking for events list
  event_id = rsi_ble_app_get_event ();
  if (event_id == -1) {
    return;
  }

  switch (event_id) {
    case RSI_BLE_CONN_EVENT:
      {
        //! event invokes when connection was completed

        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);
#if (GAP_DM_BON_BV_01_C)
        if(!already_paired) {
          status = rsi_ble_smp_pair_request (app_conn_event.dev_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
        }else {
          rsi_ble_start_encryption(app_conn_event.dev_addr,app_ble_sec_keys.remote_ediv, app_ble_sec_keys.remote_rand, app_ble_sec_keys.remote_ltk);
        }
#endif
#if (GAP_DM_LEP_BV_11_C)
        sleep(20);
        rsi_ble_app_set_event (RSI_BLE_DISCONNECT);
#endif
#if (GAP_DM_LEP_BV_09_C)
        intimate_bt();
#endif
#if (GATT_CL_GAC_BV_01_C || GATT_CL_GAW_BV_05_C || GATT_CL_GAW_BI_07_C || GATT_CL_GAW_BI_08_C || GATT_CL_GAW_BI_09_C \
    || GATT_CL_GAW_BI_12_C || GATT_CL_GAW_BV_09_C || GATT_CL_GAW_BI_34_C || GATT_CL_GAW_BI_32_C )
        rsi_ble_app_set_event (RSI_BLE_GATT_LONG_WRITE);
        break;
#elif (GATT_CL_GAD_BV_01_C || GATT_CL_GPA_BV_01_C)
        sleep(2);
        LOG_PRINT ("\nlast handle = 0x%x",last_handle);
        rsi_ble_get_profiles (app_conn_event.dev_addr, last_handle, 0xffff, &ble_profiles);
        break;
#elif (GATT_CL_GAD_BV_02_C )
        rsi_ble_app_set_event (RSI_BLE_GATT_PROFILES);
        break;
#elif (GATT_CL_GAD_BV_03_C)
        rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
        break;
#elif (GATT_CL_GAD_BV_04_C )
        rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
        break;
#elif (GATT_CL_GAR_BV_01_C || GATT_CL_GAR_BI_01_C || GATT_CL_GAR_BI_02_C || GATT_CL_GAR_BV_06_C || GATT_CL_GAR_BV_07_C \
    || GATT_CL_GAR_BI_35_C || GATT_CL_GAT_BV_01_C)
        rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
        break;
#elif (GATT_CL_GAD_BV_05_C || GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_07_C || GATT_CL_GAR_BI_10_C \
    || GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BI_12_C || GATT_CL_GAR_BI_13_C || GATT_CL_GAR_BI_14_C || GATT_CL_GAR_BV_05_C \
    || GATT_CL_GAR_BI_18_C || GATT_CL_GAR_BI_19_C || GATT_CL_GAR_BI_21_C || GATT_CL_GPA_BV_02_C || GATT_CL_GPA_BV_03_C \
    || GATT_CL_GPA_BV_04_C)
        rsi_ble_app_set_event (RSI_BLE_GATT_READ);
        break;
#elif (GATT_CL_GAD_BV_06_C || GATT_CL_GPA_BV_05_C || GATT_CL_GPA_BV_06_C || GATT_CL_GPA_BV_07_C || GATT_CL_GPA_BV_08_C)
        rsi_ble_app_set_event (RSI_BLE_GATT_INC_SERVICES);        
        break;
#elif (GATT_CL_GAR_BI_04_C)
        rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
        break;
#elif (GATT_CL_GAW_BV_01_C || GATT_CL_GAW_BV_03_C || GATT_CL_GAW_BI_02_C || GATT_CL_GAW_BI_03_C || GATT_CL_GAW_BI_05_C \
    || GATT_CL_GAW_BV_08_C || GATT_CL_GAW_BI_33_C || GATT_CL_GAN_BV_01_C || GATT_CL_GAI_BV_01_C || GATT_CL_GAT_BV_02_C)
        rsi_ble_app_set_event (RSI_BLE_GATT_WRITE);
        break;
#endif


      }
      break;

    case RSI_BLE_DISCONN_EVENT:
      {
        //! event invokes when disconnection was completed

        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
        device_found = 0;
#if (GATT_CL_GAD_BV_01_C || GATT_CL_GAD_BV_03_C)
        last_handle = 1;
#endif

#if (GATT_CL_GAD_BV_04_C )
          start_handle = 0;
          end_handle = 0;
          dbg_flag = 0;
#endif

#if (GATT_CL_GAD_BV_05_C || GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_07_C || GATT_CL_GAR_BI_10_C || GATT_CL_GPA_BV_02_C || GATT_CL_GPA_BV_03_C || GATT_CL_GPA_BV_04_C)
          uuid_size =0;
          uuid_value =0;
          f_handle =0;
          lst_handle =0;
          dbg_flag =0;
          memset(&uuid_val,0,sizeof(uuid_t));
#endif

#if (GATT_CL_GAD_BV_06_C || GATT_CL_GPA_BV_05_C || GATT_CL_GPA_BV_06_C || GATT_CL_GPA_BV_07_C || GATT_CL_GPA_BV_08_C)
          s_handle=0;
          e_handle=0;
#endif

#if (GATT_CL_GAR_BV_01_C || GATT_CL_GAR_BI_01_C || GATT_CL_GAR_BI_02_C || GATT_CL_GAR_BI_05_C || GATT_CL_GAR_BV_06_C || GATT_CL_GAR_BI_35_C)
          handle = 0;
#endif

#if (GATT_CL_GAR_BI_13_C)         
          first_handle = 0;
          length = 0;
#endif
#if (ROLE == SLAVE)
        //! set device in advertising mode.
//adv:
        status = rsi_ble_start_advertising ();
        if (status != RSI_SUCCESS) {
//          goto adv;
        }
#else
        if(bt_done) {
          rsi_ble_start_scanning();
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
          status = rsi_ble_ltk_req_reply(app_conn_event.dev_addr,1,encrypt_keys.localltk);
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

#if (ROLE == MASTER)
        //          if(already_paired)
        //! If once paired
        //! initiating the SMP pairing process
        status = rsi_ble_smp_pair_request (app_conn_event.dev_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
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
        //          status = rsi_ble_disconnect (app_conn_event.dev_addr);

      }
      break;
    case RSI_BLE_ENCRYPT_STARTED_EVENT:
      {
        //! start the encrypt event

        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
        already_paired = 1;
#if (GATT_CL_GAR_BI_05_C )
        rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
#elif (GATT_CL_GAR_BI_11_C || GATT_CL_GAR_BI_17_C || GATT_CL_GAR_BI_22_C)
        rsi_ble_app_set_event (RSI_BLE_GATT_READ);
#elif (GATT_CL_GAW_BI_06_C)
        rsi_ble_app_set_event (RSI_BLE_GATT_WRITE);
#elif (GATT_CL_GAW_BI_13_C || GATT_CL_GAW_BV_06_C)
        rsi_ble_app_set_event (RSI_BLE_GATT_LONG_WRITE);
#endif

//        rsi_ble_app_set_event (RSI_BLE_GATT_LONG_WRITE);
      }
      break;

    case RSI_BLE_ADV_REPORT_EVENT:
      {
        //! advertise report event.
        //! clear the advertise report event.
        rsi_ble_app_clear_event (RSI_BLE_ADV_REPORT_EVENT);

#if (GAP_DM_LEP_BV_04_C || GAP_DM_GIN_BV_01_C || GAP_DM_LIN_BV_01_C)
        if(device_found) {
          rsi_ble_stop_scanning();
        }
#else
#if (GAP_DM_LEP_BV_11_C)
        rsi_ble_stop_scanning();
        sleep(30);
#endif
        //status = rsi_ble_connect(remote_addr_type, (int8_t *)app_conn_event.dev_addr);
        status = rsi_ble_connect(0, remote_dev_bd_addr);
        if(status != RSI_SUCCESS)
        {
          LOG_PRINT ("\n connect status: 0x%X", status);
        }     
#endif
#if (GAP_DM_LIN_BV_01_C || GAP_DM_LEP_BV_04_C || GAP_DM_GIN_BV_01_C)
        intimate_bt();
#endif

      }
      break;
    case RSI_BLE_DISCONNECT:
      {
        rsi_ble_app_clear_event (RSI_BLE_DISCONNECT);

        dbg_flag = 0;
        sleep(5);
        if (bt_done) {
          rsi_ble_disconnect(app_conn_event.dev_addr);
        } else {
          rsi_bt_disconnect (app_conn_event.dev_addr);
        }

      }
      break;
    case RSI_BLE_REMOTE_MTU:
      {
        rsi_ble_app_clear_event (RSI_BLE_REMOTE_MTU);
#if (GAP_DM_NAD_BV_02_C)
        bt_done = 1;
        sleep(2);
        search_serv.size = 0x02;
        search_serv.val.val16 = 0x2A00;
        rsi_ble_get_char_value_by_uuid_async (app_conn_event.dev_addr, 0x1, 0xffff, search_serv, &ble_read_val);
#endif

      }
      break;
    case RSI_BLE_GATT_WRITE:
      {
        rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE);
#if (GATT_CL_GAW_BV_01_C || GATT_CL_GAW_BV_03_C || GATT_CL_GAW_BI_02_C || GATT_CL_GAW_BI_03_C || GATT_CL_GAW_BI_05_C \
    || GATT_CL_GAW_BI_06_C || GATT_CL_GAW_BV_08_C || GATT_CL_GAW_BI_33_C || GATT_CL_GAN_BV_01_C || GATT_CL_GAI_BV_01_C \
    || GATT_CL_GAW_BV_02_C || GATT_CL_GAT_BV_02_C)

        LOG_PRINT("Enter Hndl val: ");
        LOG_SCAN("%x", &first_handle);

        LOG_PRINT("Enter offset in decimal: ");
        LOG_SCAN("%d", &length);

#endif
#if (GATT_CL_GAW_BV_01_C )
        rsi_ble_set_att_cmd (app_conn_event.dev_addr, first_handle, length, "REDPINE_GATT_WRITE");
#elif (GATT_CL_GAW_BV_03_C || GATT_CL_GAW_BI_02_C || GATT_CL_GAW_BI_03_C || GATT_CL_GAW_BI_05_C || GATT_CL_GAW_BI_06_C \
    || GATT_CL_GAW_BV_08_C || GATT_CL_GAW_BI_33_C || GATT_CL_GAT_BV_02_C)
        rsi_ble_set_att_value (app_conn_event.dev_addr, first_handle, length, "REDPINE_GATT_WRITE");
#elif (GATT_CL_GAN_BV_01_C || GATT_CL_GAI_BV_01_C)

        LOG_PRINT("\nenter 1.notify 2.indicate \n");
        LOG_SCAN ("%x",&data[0]);
        //          data[0] = 0x01;
        rsi_ble_set_att_value (app_conn_event.dev_addr, first_handle, 0x02, data );
#elif (GATT_CL_GAW_BV_02_C)
        rsi_ble_set_att_cmd (app_conn_event.dev_addr, first_handle, length, "REDPINE_GATT_WRITE");
#endif
      }
      break;

    case RSI_BLE_GATT_LONG_WRITE:
      {
        rsi_ble_app_clear_event (RSI_BLE_GATT_LONG_WRITE);
#if (GATT_CL_GAC_BV_01_C || GATT_CL_GAW_BV_05_C || GATT_CL_GAW_BI_07_C || GATT_CL_GAW_BI_08_C || GATT_CL_GAW_BI_09_C \
    || GATT_CL_GAW_BI_12_C || GATT_CL_GAW_BI_13_C || GATT_CL_GAW_BV_09_C || GATT_CL_GAW_BI_34_C || GATT_CL_GAW_BI_32_C \
    || GATT_CL_GAW_BV_06_C)
        LOG_PRINT("Enter Hndl val: ");
        LOG_SCAN("%x", &first_handle);

        LOG_PRINT("Enter Len: ");
        LOG_SCAN("%d", &length);

        rsi_ble_set_long_att_value (app_conn_event.dev_addr, first_handle, 0,length, "abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789");
#endif
      }
      break;

    case RSI_BLE_GATT_PROFILE:
      {
        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILE);
        sleep(2);

        last_handle =(uint16_t )ble_char_services.char_services[ble_char_services.num_of_services - 1].char_data.char_handle + 1; 

        LOG_PRINT ("\nhandle = 0X%x ",last_handle);
        //          rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
#if (GATT_CL_GAD_BV_04_C )
        if(!dbg_flag) {
          dbg_flag = 1;
          LOG_PRINT("\nEnter Hndl start val: ");
          LOG_SCAN("%x", &start_handle);
          LOG_PRINT("\nEnter Hndl end val: ");
          LOG_SCAN("%x", &end_handle);
        }
        else {
          start_handle =(uint16_t )ble_char_services.char_services[ble_char_services.num_of_services - 1].char_data.char_handle + 1; 
        }

        rsi_ble_get_char_services (app_conn_event.dev_addr, start_handle, end_handle, &ble_char_services);
#else
        rsi_ble_get_char_services (app_conn_event.dev_addr, last_handle, 0xFFFF, &ble_char_services);
#endif
      }
      break;
    case RSI_BLE_GATT_PROFILES:
      {
        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILES);
        sleep(5);
//        rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, remote_dev_addr);
#if (GATT_CL_GAD_BV_02_C )
        LOG_PRINT ("\n enter uuid size value in Hex Format:");
        LOG_SCAN ("%x", &uuid_size);

        search_serv.size = uuid_size;

        if(uuid_size == 0x02) {
          LOG_PRINT ("\n enter 16-bit uuid value:");
          LOG_SCAN ("%x", &uuid_value);

          search_serv.val.val16 = uuid_value;
        } else if(uuid_size == 0x10) {
          fun(); // get 128-bit uuid value
          memcpy(&search_serv.val ,&uuid_val.val, 16);
        } else {
          LOG_PRINT ("\n wrong uuid size entered");
        }

        rsi_ble_get_profile (app_conn_event.dev_addr, search_serv, &ble_profile);
        //rsi_ble_get_char_services_async(app_conn_event.dev_addr, 1, 0x10, NULL);
#endif
      }
      break;
    
    case RSI_BLE_GATT_CHAR_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_SERVICES);
          sleep(2);
//          rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
          rsi_ble_get_inc_services (app_conn_event.dev_addr, last_handle, 0xFFFF, &ble_inc_serv);

        }
        break;
    
    case RSI_BLE_GATT_DESC_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_SERVICES);
//          rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
#if (GATT_CL_GAR_BV_01_C || GATT_CL_GAR_BI_01_C || GATT_CL_GAR_BI_02_C || GATT_CL_GAR_BI_05_C || GATT_CL_GAR_BV_06_C \
    || GATT_CL_GAR_BV_07_C || GATT_CL_GAR_BI_35_C || GATT_CL_GAT_BV_01_C || GATT_CL_GAR_BI_04_C || GATT_CL_GAR_BI_34_C)
          sleep(3);
          if(!dbg_flag) {
            LOG_PRINT ("\n enter handle:");
            LOG_SCAN ("%x",&handle);
            dbg_flag = 1;
            rsi_ble_get_att_value (app_conn_event.dev_addr, handle, &ble_read_val);
          } else {
            length += ble_mtu.mtu_size ;
            rsi_ble_get_long_att_value (app_conn_event.dev_addr, handle, length, NULL);
          }
#else
          rsi_ble_get_att_value (app_conn_event.dev_addr,*(uint16_t *)ble_desc_services.att_desc[0].handle, &ble_read_val);
#endif
        }
        break;

    case RSI_BLE_GATT_READ:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_READ);
          sleep(3);
//          rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
#if (GATT_CL_GAR_BV_05_C || GATT_CL_GAR_BI_18_C || GATT_CL_GAR_BI_19_C || GATT_CL_GAR_BI_21_C || GATT_CL_GAR_BI_22_C)

          uint8_t ix=0;
          LOG_PRINT ("\n enter no 0f handles to be read:");
          LOG_SCAN ("%d", &handles_cnt);
          for(ix =0; ix < handles_cnt ; ix++ ) {
            LOG_PRINT ("\nenter ix handle :");
            LOG_SCAN ("%x",&handles[ix]);
          }

          rsi_ble_get_multiple_att_values(app_conn_event.dev_addr, handles_cnt, handles, NULL);
          break;

#endif
#if (GATT_CL_GAR_BV_04_C || GATT_CL_GAR_BI_12_C || GATT_CL_GAR_BI_13_C || GATT_CL_GAR_BI_14_C || GATT_CL_GAR_BI_17_C)
          if (!dbg_flag) {
            LOG_PRINT("Enter Hndl val: ");
            LOG_SCAN("%x", &first_handle);

            LOG_PRINT("Enter offset in decimal: ");
            LOG_SCAN("%d", &length);

            dbg_flag = 1;
            rsi_ble_get_long_att_value (app_conn_event.dev_addr, first_handle, length, NULL);

          } else {
            length += ble_mtu.mtu_size ;
            rsi_ble_get_long_att_value (app_conn_event.dev_addr, first_handle, length, NULL);

          }

          break;
#endif

#if (GATT_CL_GAD_BV_05_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_10_C || GATT_CL_GAR_BI_11_C || GATT_CL_GPA_BV_02_C || GATT_CL_GPA_BV_03_C || GATT_CL_GPA_BV_04_C)
          {
            LOG_PRINT("\nEnter start hndl: ");
            LOG_SCAN("%x", &f_handle);

            LOG_PRINT("\nEnter end hndl: ");
            LOG_SCAN("%x", &lst_handle);
          }
#endif

#if (GATT_CL_GAD_BV_05_C || GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BI_06_C || GATT_CL_GAR_BI_07_C || GATT_CL_GAR_BI_11_C || GATT_CL_GAR_BI_10_C || GATT_CL_GPA_BV_02_C || GATT_CL_GPA_BV_03_C || GATT_CL_GPA_BV_04_C)
          LOG_PRINT ("\nenter uuid size value in Hex foramt:");
          LOG_SCAN ("%x", &uuid_size);
          search_serv.size = uuid_size;

          if(uuid_size == 2) {
            LOG_PRINT ("\nenter 16-bit uuid value:");
            LOG_SCAN ("%x", &uuid_value);

            search_serv.val.val16 = uuid_value;
          } else if(uuid_size == 16) {
            fun(); //get 128-bit uuid
            memcpy(&search_serv.val ,&uuid_val.val, 16);
          }
#endif
#if (GATT_CL_GAR_BV_03_C || GATT_CL_GAR_BI_07_C )
          rsi_ble_get_char_value_by_uuid (app_conn_event.dev_addr, 0x1, 0xffff, search_serv, &ble_read_val);
#else
          rsi_ble_get_char_value_by_uuid (app_conn_event.dev_addr, f_handle, lst_handle, search_serv, &ble_read_val);
#endif

        }
        break;

    case RSI_BLE_GATT_INC_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_INC_SERVICES);

          sleep(3);
#if (GATT_CL_GAD_BV_06_C || GATT_CL_GPA_BV_05_C || GATT_CL_GPA_BV_06_C || GATT_CL_GPA_BV_07_C || GATT_CL_GPA_BV_08_C)
          LOG_PRINT ("\n enter start handle:");
          LOG_SCAN ("%x",&s_handle);

          LOG_PRINT ("\n enter end handle:");
          LOG_SCAN ("%x",&e_handle);

          rsi_ble_get_att_descriptors (app_conn_event.dev_addr, s_handle, e_handle, &ble_desc_services);
#else
          rsi_ble_get_att_descriptors_async (app_conn_event.dev_addr, 1, 0xFFFF, &ble_desc_services);
#endif

        }
        break;

    case RSI_BLE_EVENT_GATT_RD : 
        {
          rsi_ble_app_clear_event (RSI_BLE_EVENT_GATT_RD);
          uint8_t error_code;
//          if (app_ble_read_event.offset > sizeof(data2))
          {
            LOG_PRINT("att err resp \n");
            error_code = 0x0C; // BT_LE_INVALID_OFFSET 
            rsi_ble_att_error_response (app_conn_event.dev_addr,
                app_ble_read_event.handle,
                app_ble_read_event.type,
                error_code);

          }
        }
        break;

    case RSI_BLE_GATT_NOTIFY_EVENT:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_NOTIFY_EVENT);
        
          rsi_ble_app_data[0]= 0x01;

          rsi_ble_notify_value(app_conn_event.dev_addr,rsi_ble_att3_val_hndl, 0x02, rsi_ble_app_data);
        }
        break;
    case RSI_BLE_GATT_INDICATE_EVENT:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_INDICATE_EVENT);
        
          rsi_ble_app_data[0]= 0x02;

          rsi_ble_indicate_value (app_conn_event.dev_addr,rsi_ble_att3_val_hndl, 0x02, rsi_ble_app_data);
        }
        break;


  }

  return;
}
