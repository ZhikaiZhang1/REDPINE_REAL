/**
 * @file    rsi_ble_heartrate.c
 * @version 0.1
 * @date    09 May 2017
 *
 *  Copyright(C) Redpine Signals 2017
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BLE heart rate profile
 *
 *  @section Description  This application acts as gatt server and helps gatt client
 *
 *  to retrive heart rate profile related information.
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
#include<string.h>
#include<stdio.h>
//! Common include file 
#include<rsi_common_apis.h>

#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT
#endif

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN   10000

//! Memory to initialize driver
uint8_t bt_global_buf[BT_GLOBAL_BUFF_LEN];

//! BLE attribute service types uuid values
#define  RSI_BLE_CHAR_SERV_UUID                      0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID                    0x2902

//! BLE characteristic service uuid
#define  RSI_BLE_HEART_RATE_SERVICE_UUID             0x180D
#define  RSI_BLE_HEART_RATE_MEASUREMENT_UUID         0x2A37
#define  RSI_BLE_SENSOR_LOCATION_UUID                0x2A38
#define  RSI_BLE_HEART_RATE_CONTROL_POINT_UUID       0x2A39 

//! max data length
#define  RSI_BLE_MAX_DATA_LEN                        20

//! local device name
#define RSI_BLE_HEART_RATE_PROFILE                   "BLE_HEART_RATE_PROFILE"

//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_DEV_ADDR                           "00:23:A7:80:70:B9"

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME					   "REDPINE_DEV"

//! attribute properties
#define  RSI_BLE_ATT_PROPERTY_READ                   0x02
#define  RSI_BLE_ATT_PROPERTY_WRITE                  0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY                 0x10

//! application event list
#define  RSI_APP_EVENT_ADV_REPORT                    0x00
#define  RSI_BLE_CONN_EVENT                          0x01
#define  RSI_BLE_DISCONN_EVENT                       0x02
#define  RSI_BLE_GATT_WRITE_EVENT                    0x03
#define  RSI_BLE_GATT_PROFILE_RESP_EVENT             0x04
#define  RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT       0x05
#define  RSI_BLE_GATT_CHAR_DESC_RESP_EVENT           0x06

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE                       1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                         1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS                            1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                     2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE                   3000

#endif

#define SERVER                                       0
#define CLIENT                                       1
#define GATT_ROLE                                    SERVER


typedef struct heart_rate_s
{
  int8_t flags;
  int8_t heart_rate_measure_8;
  int16_t heart_rate_measure_16;
  int8_t energy_expended_status;
  int8_t rr_interval;
}heart_rate_t;

uint8_t heartratefun ( heart_rate_t , uint8_t *); 
void rsi_wireless_driver_task(void);

/******************************************************
 * *                     Heart rate  Macros
 * ******************************************************/

//VALUE_FORMATE_BIT    BIT(0)
#define RSI_BLE_HEART_RATE_VALUE_BYTE_FORMATE   0
#define RSI_BLE_HEART_RATE_VALUE_2BYTE_FORMATE  1

//SENSOR_CONTACT_BITS BIT(1), BIT(2)
#define RSI_BLE_HEART_RATE_SENSOR_CONTACT_NOT_SUPPORTED   0
#define RSI_SENSOR_CONTACT_SUPPORTED_CONNECT_NOT_DETECTED 2
#define RSI_SENSOR_CONTACT_SUPPORTED_CONNECT_DETECTED     3 

//ENERGY_EXPENDED_FEILD_BIT    BIT(3)
#define RSI_ENERGY_EXPENDED_FEILD_NOT_PRESENT 0
#define RSI_ENERGY_EXPENDED_FEILD_PRESENT     1

//RR_INTEVAL_FEILD_BIT  BIT(4)
#define RSI_RR_INTERVAL_FEILD_NOT_PRESENT 0
#define RSI_RR_INTERVAL_FEILD_PRESENT     1

//sensor location 
#define RSI_BLE_BODY_SENSOR_LOCATION_OTHER BIT(0)
#define RSI_BLE_BODY_SENSOR_LOCATION_CHEST BIT(1)
#define RSI_BLE_BODY_SENSOR_LOCATION_WRIST BIT(2)
#define RSI_BLE_BODY_SENSOR_LOCATION_FINGER BIT(3)
#define RSI_BLE_BODY_SENSOR_LOCATION_HAND BIT(4)
#define RSI_BLE_BODY_SENSOR_LOCATION_EAR_LOBE BIT(5)
#define RSI_BLE_BODY_SENSOR_LOCATION_FOOT BIT(6)

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t device_found=0;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t app_ble_write_event;
static uint16_t rsi_ble_measurement_hndl;
static uint16_t rsi_ble_sensor_loc_hndl;
static uint16_t rsi_ble_control_point_hndl;
static uint16_t rsi_ble_heartrate_hndl;
static profile_descriptors_t  rsi_ble_service;
static rsi_ble_resp_char_services_t char_servs;
static rsi_ble_resp_att_descs_t  att_desc;
int8_t notify_start =0;
heart_rate_t  rate = {0x00,75,73,70,0};

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

/*==============================================*/
/**
 * @fn         rsi_ble_heart_rate_add_new_serv
 * @brief      this function is used to add new servcie i.e.., heart rate service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */

static uint32_t rsi_ble_heart_rate_add_new_serv (void)
{
	uuid_t  new_uuid = {0};
	rsi_ble_resp_add_serv_t  new_serv_resp = {0};
    uint8_t   sensor_data = RSI_BLE_BODY_SENSOR_LOCATION_WRIST;
    uint8_t   control_data = 0;


	//! adding new service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_HEART_RATE_SERVICE_UUID;
	rsi_ble_add_service (new_uuid, 3, 60, &new_serv_resp);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_HEART_RATE_MEASUREMENT_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
			                       new_serv_resp.start_handle + 1,
			                       RSI_BLE_ATT_PROPERTY_NOTIFY,
			                       new_serv_resp.start_handle + 2,
			                       new_uuid);                                       

  //! adding characteristic value attribute to the service
	rsi_ble_measurement_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_HEART_RATE_MEASUREMENT_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 2,
                            new_uuid,
                            RSI_BLE_ATT_PROPERTY_NOTIFY,
                            (uint8_t *)&rate,
                            sizeof (rate));

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_SENSOR_LOCATION_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
			                       new_serv_resp.start_handle + 4,
			                       RSI_BLE_ATT_PROPERTY_READ,
			                       new_serv_resp.start_handle + 5,
			                       new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_sensor_loc_hndl = new_serv_resp.start_handle + 5;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_SENSOR_LOCATION_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
			                      new_serv_resp.start_handle + 5,
			                      new_uuid,
			                      RSI_BLE_ATT_PROPERTY_READ,
			                      &sensor_data,
			                      sizeof (sensor_data));


	//! adding characteristic service attribute to the service
  new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_HEART_RATE_CONTROL_POINT_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
			                       new_serv_resp.start_handle + 6,
			                       RSI_BLE_ATT_PROPERTY_WRITE,
			                       new_serv_resp.start_handle + 7,
			                       new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_control_point_hndl = new_serv_resp.start_handle + 7;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_HEART_RATE_CONTROL_POINT_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 7,
                            new_uuid,
                            RSI_BLE_ATT_PROPERTY_WRITE,
                            &control_data,
                            sizeof (control_data));



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
      ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)&&((strcmp(remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  { 
    device_found=1;
    rsi_ble_app_set_event (RSI_APP_EVENT_ADV_REPORT);
  }
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
  conn_event_to_app.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (conn_event_to_app.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  conn_event_to_app.status = resp_enh_conn->status;
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
	memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
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
	memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
	rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_gatt_write_event (uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{

  memcpy (&app_ble_write_event, rsi_ble_write, sizeof (rsi_ble_event_write_t));
  if ((rsi_ble_measurement_hndl + 1) == *((uint16_t *)app_ble_write_event.handle)) {
    if(app_ble_write_event.att_value[0] == 1)
    {
      notify_start = 1;
    }
    else if(app_ble_write_event.att_value[0] == 0)
    {
      notify_start = 0;
    }
  }
  rsi_ble_app_set_event (RSI_BLE_GATT_WRITE_EVENT);
  
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_profiles_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] p_ble_resp_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_on_profiles_event (uint16_t resp_status, profile_descriptors_t *rsi_ble_resp_profile)
{
    rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE_RESP_EVENT);
    return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_char_services_event
 * @brief      invoked when response is received for characteristic services details
 * @param[out] p_ble_resp_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_on_char_services_event (uint16_t resp_status, rsi_ble_resp_char_services_t *rsi_ble_resp_char_services)
{
	rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);
	return;
}

static void ble_on_att_desc_event (uint16_t resp_status, rsi_ble_resp_att_descs_t *rsi_ble_resp_att_desc)
{
	rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_DESC_RESP_EVENT);
	return;
}
/*=============================================*/
/**
 * @fn         heartratefun 
 * @brief      Creates the  heart rate megurement data.
 * @param[in]  rate, it indicates heart rate mesurement strcture.
 * @param[out] p_data, heart rate mesurement strcture in buffer pointer.
 * @return     length of heart rate data buffer.
 * @section description
 */

uint8_t heartratefun(heart_rate_t rate, uint8_t *p_data)
{
  int len = 0;

  p_data[len++] = rate.flags;

  if (!(rate.flags & BIT(0))) {
	if(rate.heart_rate_measure_8 < 85){
	  rate.heart_rate_measure_8 = rate.heart_rate_measure_8 + 2 ;
	}else{
      rate.heart_rate_measure_8 = 75 ;
	}
    p_data[len++] = rate.heart_rate_measure_8;
  } else {
    *((uint16_t *)(p_data + len)) = rate.heart_rate_measure_16;
    len += 2;
  }

  if (rate.flags & BIT(3)) {
    *((uint16_t *)(p_data + len)) = rate.energy_expended_status;
    len += 2;
  }

  if (rate.flags & BIT(4)) {
    *((uint16_t *)(p_data + len)) = rate.rr_interval;
    len += 2;
  }

  return len;
}

/*==============================================*/
/**
 * @fn         rsi_ble_heart_rate_gatt_server
 * @brief      this is the application of heart rate profile.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This function is used to test the simple chat application.
 */
int32_t rsi_ble_heart_rate_gatt_server (void)
{
	int32_t status = 0;
	int32_t  event_id;
	int8_t   data[8] = {0};
  uint8_t  len;
  uuid_t   search_serv;
  rsi_ble_resp_local_att_value_t  local_att_val;
  uint8_t adv[31] = {2,1,6};
  static uint32_t loop_cnt = 1;
  uuid_t  service_uuid;
  uint8_t  connected = 0;
  uint8_t  ix;
  
  
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

#if (GATT_ROLE == SERVER)
	//! adding simple BLE chat service
	rsi_ble_heart_rate_add_new_serv ();
#endif

	//! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(rsi_ble_simple_central_on_adv_report_event,
                                 rsi_ble_on_connect_event,
                                 rsi_ble_on_disconnect_event,
                                 NULL,
                                 NULL,
                                 NULL,
                                 rsi_ble_on_enhance_conn_status_event,
                                 NULL,
                                 NULL);

	//! registering the GATT call back functions
	rsi_ble_gatt_register_callbacks (NULL,
			                             rsi_ble_on_profiles_event,
			                             rsi_ble_on_char_services_event,
			                             NULL,
			                             ble_on_att_desc_event,
			                             NULL,
			                             NULL,
			                             rsi_ble_on_gatt_write_event,
                                   NULL,
                                   NULL,
                                   NULL,
   	                               NULL);

	//!  initializing the application events map
	rsi_ble_app_init_events ();

	//! Set local name
	rsi_bt_set_local_name(RSI_BLE_HEART_RATE_PROFILE);

#if(GATT_ROLE == SERVER)
	//! prepare advertise data //local/device name
  adv[3] = strlen (RSI_BLE_HEART_RATE_PROFILE) + 1;
  adv[4] = 9;
  strcpy (&adv[5], RSI_BLE_HEART_RATE_PROFILE);

	//! set advertise data
	rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_HEART_RATE_PROFILE) + 5);

	//! set device in advertising mode.
	rsi_ble_start_advertising ();
#endif

#if(GATT_ROLE == CLIENT)
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
    if ((event_id == -1))
    {
#if(GATT_ROLE == SERVER)
      if((connected == 1))
      {
        if(notify_start == 1) 
        {
#ifdef __linux__
          usleep(3000000);
#endif					
#ifdef RSI_M4_INTERFACE 
          rsi_delay_ms (5000);
#endif
          len =heartratefun(rate, data);
          rsi_ble_set_local_att_value (rsi_ble_measurement_hndl, len, data);

        }
      }
#endif
      continue;
    }

    switch (event_id) {
#if(GATT_ROLE == CLIENT)
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

#endif
      case RSI_BLE_CONN_EVENT:
        {
          //! event invokes when connection was completed

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);

        connected = 1;

#if(GATT_ROLE == CLIENT)
        rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, (uint8_t *)conn_event_to_app.dev_addr);
        service_uuid.size = 2;
          service_uuid.val.val16 = RSI_BLE_HEART_RATE_SERVICE_UUID;
retry:          
          status = rsi_ble_get_profile_async (remote_dev_addr, service_uuid, &rsi_ble_service);
          if(status != 0)
          {
             goto retry;
          }
#endif
			}
			break;

			case RSI_BLE_DISCONN_EVENT:
			{
				//! event invokes when disconnection was completed

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
        connected = 0;

				//! set device in advertising mode.
#if(GATT_ROLE == SERVER)

adv:
				status = rsi_ble_start_advertising ();
				if (status != RSI_SUCCESS) {
					goto adv;
				}
#endif
#if(GATT_ROLE == CLIENT)
                //! start scanning
                device_found=0;
                status = rsi_ble_start_scanning();
                if(status != RSI_SUCCESS)
                {
                  LOG_PRINT ("start_scanning status: 0x%X\r\n", status);
                }
#endif
            }
			break;

			case RSI_BLE_GATT_WRITE_EVENT:
			{
				//! event invokes when write/notification events received

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE_EVENT);
				LOG_PRINT ("\r\nhandle: 0x%x, len: %d, data: \r\n", 
            *((uint16_t *)app_ble_write_event.handle),
            app_ble_write_event.length);
#if(GATT_ROLE == CLIENT)
        if (rsi_ble_heartrate_hndl != 0)
        {
          if((app_ble_write_event.att_value[0] & 1) == 0)
          {
            LOG_PRINT("\nbpm: 0x%02x\r\n",app_ble_write_event.att_value[1]);
          }

          if((app_ble_write_event.att_value[0] & 1 )== 1)
          {
            LOG_PRINT("\nbpm: 0x%04x \r\n",
                (app_ble_write_event.att_value[1] | (app_ble_write_event.att_value[2]<< 8)));
          }
        }
#endif
      }
			break;
#if(GATT_ROLE == CLIENT)
      case RSI_BLE_GATT_PROFILE_RESP_EVENT:
			{
				//! event invokes when get profile response received

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILE_RESP_EVENT);

				//! get characteristics of the immediate alert servcie
				rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, (uint8_t *)conn_event_to_app.dev_addr);
				rsi_ble_get_char_services_async (remote_dev_addr, *(uint16_t *)rsi_ble_service.start_handle, *(uint16_t *)rsi_ble_service.end_handle, &char_servs);
			}
			break;
      
      case RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT:
      {
        //! event invokes when get characteristics of the service response received

        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);

        //! verifying the immediate alert characteristic
        for (ix = 0; ix < char_servs.num_of_services; ix++) {
          LOG_PRINT ("handle: 0x%04x - property: 0x%02x, handle: 0x%04x, uuid: 0x%04x\r\n",
              char_servs.char_services[ix].handle,
              char_servs.char_services[ix].char_data.char_property,
              char_servs.char_services[ix].char_data.char_handle,
              char_servs.char_services[ix].char_data.char_uuid.val.val16);
			  
			  if (char_servs.char_services[ix].char_data.char_uuid.val.val16 == RSI_BLE_HEART_RATE_MEASUREMENT_UUID) 
			  {
			      rsi_ble_heartrate_hndl = char_servs.char_services[ix].char_data.char_handle;
				  rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, (uint8_t *)conn_event_to_app.dev_addr);
				  rsi_ble_get_att_descriptors_async (remote_dev_addr, 
                                                     char_servs.char_services[ix].handle,
                                                    (char_servs.char_services[ix + 1].handle - 1),
                                                    &att_desc);
              }
        }
      }
      break;

      case RSI_BLE_GATT_CHAR_DESC_RESP_EVENT:
        rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_DESC_RESP_EVENT);

        for (ix = 0; ix < att_desc.num_of_att; ix++) {
          LOG_PRINT ("hancle: 0x%04x - 0x%04x\r\n",
              *((uint16_t *)att_desc.att_desc[ix].handle),
              att_desc.att_desc[ix].att_type_uuid.val.val16);
          if (att_desc.att_desc[ix].att_type_uuid.val.val16 == 0x2902) {
            rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, (uint8_t *)conn_event_to_app.dev_addr);
            data[0] = 0x01;
            data[1] = 0x00;
            rsi_ble_set_att_cmd (remote_dev_addr,
                *((uint16_t *)att_desc.att_desc[ix].handle),
                  2,
                  data);
          }
        }

      break;
#endif

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
  
  //! Call Simple chat GATT Server Application
  rsi_ble_heart_rate_gatt_server();

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
  rsi_task_create(rsi_ble_heart_rate_gatt_server, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
