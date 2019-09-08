/**
 * @file    rsi_ble_gatt_test_async_DEMO_41.c
 * @version 0.1
 * @date    06 March 2019
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
#include "rsi_common_app.h"
#if BLE_GATT_TEST_ASYNC

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

//! application global variables list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t remote_addr_type = 0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = { 0 };
static uint8_t remote_dev_bd_addr[6] = { 0 };
static uint8_t local_device_address[18];
static uint8_t device_found = 0;
static uint8_t notify_enabled;
static uint16_t rsi_ble_battery_val_hndl;
static rsi_ble_event_adv_report_t app_scan_list[NO_OF_ADV_REPORTS];
static uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = { 0 };
static rsi_ble_event_conn_status_t app_conn_event;
static rsi_ble_event_disconnect_t app_disconn_event;
static rsi_ble_event_write_t app_ble_write_event;

static rsi_ble_event_profiles_list_t get_allprofiles;
static rsi_ble_event_profile_by_uuid_t get_profile;
static rsi_ble_event_read_by_type1_t get_char_services;
static rsi_ble_event_gatt_desc_t get_desc_services;
static rsi_ble_event_error_resp_t gatt_err_reason;
static rsi_ble_event_att_value_t  gatt_read;
static rsi_ble_event_read_by_type3_t  read_val_by_uuid;
/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events(void) {
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
static void rsi_ble_app_set_event(uint32_t event_num) {
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
static void rsi_ble_app_clear_event(uint32_t event_num) {
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
static int32_t rsi_ble_app_get_event() {
	uint32_t ix;

	for (ix = 0; ix < 32; ix++) {
		if (ble_app_event_map & (1 << ix)) {
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
static void rsi_ble_on_adv_reports(rsi_ble_event_adv_report_t *adv_report) {
	if (device_found == 1) {
		return;
	}

	BT_LE_ADPacketExtract(remote_name, adv_report->adv_data,
			adv_report->adv_data_len);

	remote_addr_type = adv_report->dev_addr_type;
  memcpy (remote_dev_bd_addr, (uint8_t *) adv_report->dev_addr, 6);
	rsi_6byte_dev_address_to_ascii(remote_dev_addr, (uint8_t *) adv_report->dev_addr);
	if ((device_found == 0)
			&& ((strcmp(remote_name, RSI_REMOTE_DEVICE_NAME)) == 0)
			|| ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)
					&& ((strcmp(remote_dev_addr, RSI_BLE_DEV_ADDR) == 0)))) {
		device_found = 1;
		rsi_ble_app_set_event(RSI_BLE_ADV_REPORT_EVENT);
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
void rsi_ble_on_enhance_conn_status_event(
		rsi_ble_event_enhance_conn_status_t *resp_enh_conn) {
	app_conn_event.dev_addr_type = resp_enh_conn->dev_addr_type;
	memcpy(app_conn_event.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
	app_conn_event.status = resp_enh_conn->status;
	rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
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
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn) {
	memcpy(&app_conn_event, resp_conn, sizeof(rsi_ble_event_conn_status_t));
	rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
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
static void rsi_ble_on_disconnect_event(
		rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason) {
	memcpy(&app_disconn_event, resp_disconnect,
			sizeof(rsi_ble_event_conn_status_t));
	rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
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
static void rsi_ble_on_gatt_write_event(uint16_t event_id,
		rsi_ble_event_write_t *rsi_ble_write) {
	memcpy(&app_ble_write_event, rsi_ble_write, sizeof(rsi_ble_event_write_t));
	if (rsi_ble_battery_val_hndl + 1
			== *((uint16_t *) app_ble_write_event.handle)) {
		if (app_ble_write_event.att_value[0] == NOTIFY_ENABLE) {
			notify_enabled = NOTIFY_ENABLE;
             LOG_PRINT("\n NOTIFY ENABLED\n");
		} else if (app_ble_write_event.att_value[0] == NOTIFY_DISABLE) {
              LOG_PRINT("\n NOTIFY DISABLED\n");
			notify_enabled = NOTIFY_DISABLE;
		}
	}
	rsi_ble_app_set_event(RSI_BLE_GATT_WRITE_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_profiles_list_event
 * @brief      invoked when response is received for get list of services.
 * @param[out] p_ble_resp_profiles, profiles list details
 * @return     none
 * @section description
 */
static void rsi_ble_profiles_list_event(uint16_t resp_status,
		rsi_ble_event_profiles_list_t *rsi_ble_event_profiles) {
  int8_t i=0;    
  LOG_PRINT("\n in rsi_ble_profiles_event\n");
  memcpy(&get_allprofiles, rsi_ble_event_profiles, sizeof(rsi_ble_event_profiles_list_t));
  memcpy (remote_dev_bd_addr, get_allprofiles.dev_addr, 6);
  LOG_PRINT ("REMOTE_DEVICE_ADDRESS : %s\r\n",
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, get_allprofiles.dev_addr));
  for(i=0;i<get_allprofiles.number_of_profiles;i++)
  { 
    LOG_PRINT("\n\nPROFILES LIST:\nStart handle :0x%x \nEnd handle:0x%x \nUUID:0x%x \n\n",
        *((uint16_t *)(get_allprofiles.profile_desc[i].start_handle)),
        *((uint16_t *)(get_allprofiles.profile_desc[i].end_handle)),
        get_allprofiles.profile_desc[i].profile_uuid.val.val16);
  }
	rsi_ble_app_set_event(RSI_BLE_GATT_PROFILES);
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
static void rsi_ble_profile_event(uint16_t resp_status,
		rsi_ble_event_profile_by_uuid_t *rsi_ble_event_profile) {
	LOG_PRINT("\n in rsi_ble_profile_event\n");
	memcpy(&get_profile, rsi_ble_event_profile,
			sizeof(rsi_ble_event_profile_by_uuid_t));
	rsi_ble_app_set_event(RSI_BLE_GATT_PROFILE);
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
static void rsi_ble_char_services_event(uint16_t resp_status,
		rsi_ble_event_read_by_type1_t *rsi_ble_event_char_services) {
	memcpy(&get_char_services, rsi_ble_event_char_services,
			sizeof(rsi_ble_event_read_by_type1_t));
  memcpy (remote_dev_bd_addr, get_char_services.dev_addr, 6);
  LOG_PRINT ("\nREMOTE_DEVICE_ADDRESS : %s\r\n",
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, get_char_services.dev_addr));
	rsi_ble_app_set_event(RSI_BLE_GATT_CHAR_SERVICES);
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
static void rsi_ble_on_att_desc_event(uint16_t resp_status,
		rsi_ble_event_read_by_type3_t *rsi_ble_event_att_desc) {
    LOG_PRINT("\nIn GATT READ ATT VALUE BY UUID\n");
    memcpy(&read_val_by_uuid, rsi_ble_event_att_desc, sizeof(rsi_ble_event_read_by_type3_t));
    memcpy (remote_dev_bd_addr, read_val_by_uuid.dev_addr, 6);
    LOG_PRINT ("REMOTE_DEVICE_ADDRESS : %s\r\n",
        rsi_6byte_dev_address_to_ascii (remote_dev_addr, read_val_by_uuid.dev_addr));
    LOG_PRINT("handle :0x%x \n length:0x%x \nAttribute Value:0x%x\n",
      *((uint16_t *)(read_val_by_uuid.handle)),
      (uint16_t *)(read_val_by_uuid.length),
      read_val_by_uuid.att_value);
	rsi_ble_app_set_event(RSI_BLE_GATT_DESC_SERVICES);
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
static void rsi_ble_on_read_event(uint16_t resp_status, uint16_t resp_id,
		rsi_ble_event_att_value_t *rsi_ble_event_att_val) {
    LOG_PRINT("\nIn gatt read event\n");
    memcpy(&gatt_read, rsi_ble_event_att_val, sizeof(rsi_ble_event_att_value_t));
    LOG_PRINT("\nAttribute Length:0x%x\n, Attribute Value:0x%x\n",(uint16_t *)gatt_read.length, gatt_read.att_value);
	rsi_ble_app_set_event(RSI_BLE_GATT_READ);
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
static void rsi_ble_write_event(uint16_t resp_status,
		rsi_ble_set_att_resp_t *rsi_ble_event_set_att_rsp) {
    LOG_PRINT("\nIn gatt write event\n");
	rsi_ble_app_set_event(RSI_BLE_GATT_WRITE);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_event_indication_confirmation
 * @brief      this function will invoke when received indication confirmation event
 * @param[out] resp_id, response id
 * @param[out] status, status of the response
 * @return     none
 * @section description
 */
static void rsi_ble_on_event_indication_confirmation(uint16_t resp_status,
		rsi_ble_set_att_resp_t *rsi_ble_event_set_att_rsp) {
    LOG_PRINT("\nIn rsi_ble_on_event_indication_confirmation event\n");
	rsi_ble_app_set_event(RSI_BLE_GATT_INDICATION_CONFIRMATION);
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
static void rsi_ble_gatt_error_event(uint16_t resp_status,
		rsi_ble_event_error_resp_t *rsi_ble_gatt_error) {
    LOG_PRINT("\nIn gatt error event\n");
    memcpy(&gatt_err_reason, rsi_ble_gatt_error, sizeof(rsi_ble_event_error_resp_t));
    LOG_PRINT("\nERROR REASON:0x%x \n",*((uint16_t *)gatt_err_reason.error));
	rsi_ble_app_set_event(RSI_BLE_GATT_ERROR);
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
static void rsi_ble_gatt_desc_val_event(uint16_t resp_status,
		rsi_ble_event_gatt_desc_t *rsi_ble_gatt_desc_val) {
	memcpy(&get_desc_services, rsi_ble_gatt_desc_val,
			sizeof(rsi_ble_event_gatt_desc_t));
	rsi_ble_app_set_event(RSI_BLE_GATT_DESC_VAL);
}
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
static void rsi_ble_add_char_serv_att(void *serv_handler, uint16_t handle,
		uint8_t val_prop, uint16_t att_val_handle, uuid_t att_val_uuid) {
	rsi_ble_req_add_att_t new_att = { 0 };

	//! preparing the attribute service structure
	new_att.serv_handler = serv_handler;
	new_att.handle = handle;
	new_att.att_uuid.size = 2;
	new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
	new_att.property = RSI_BLE_ATT_PROPERTY_READ;

	//! preparing the characteristic attribute value
	new_att.data_len = 6;
	new_att.data[0] = val_prop;
	rsi_uint16_to_2bytes(&new_att.data[2], att_val_handle);
	rsi_uint16_to_2bytes(&new_att.data[4], att_val_uuid.val.val16);

	//! Add attribute to the service
	rsi_ble_add_attribute(&new_att);

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

static void rsi_ble_add_char_val_att(void *serv_handler, uint16_t handle,
		uuid_t att_type_uuid, uint8_t val_prop, uint8_t *data, uint8_t data_len) {
	rsi_ble_req_add_att_t new_att = { 0 };

	//! preparing the attributes
	new_att.serv_handler = serv_handler;
	new_att.handle = handle;
	memcpy(&new_att.att_uuid, &att_type_uuid, sizeof(uuid_t));
	new_att.property = val_prop;

	//! preparing the attribute value
	new_att.data_len = RSI_MIN(sizeof(new_att.data), data_len);
	memcpy(new_att.data, data, new_att.data_len);

	//! add attribute to the service
	rsi_ble_add_attribute(&new_att);

	//! check the attribute property with notification
	if (val_prop & RSI_BLE_ATT_PROPERTY_NOTIFY) {
		//! if notification property supports then we need to add client characteristic service.

		//! preparing the client characteristic attribute & values
		memset(&new_att, 0, sizeof(rsi_ble_req_add_att_t));
		new_att.serv_handler = serv_handler;
		new_att.handle = handle + 1;
		new_att.att_uuid.size = 2;
		new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_UUID;
		new_att.property = RSI_BLE_ATT_PROPERTY_READ
				| RSI_BLE_ATT_PROPERTY_WRITE;
		new_att.data_len = 2;
		//! add attribute to the service
		rsi_ble_add_attribute(&new_att);
	}
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_battery_service_serv
 * @brief      this function is used to add new servcie i.e.., battery service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */

static uint32_t rsi_ble_add_battery_service_serv(void) {
	uuid_t new_uuid = { 0 };
	rsi_ble_resp_add_serv_t new_serv_resp = { 0 };
	uint8_t data[1] = { 90 };
	rsi_ble_req_add_att_t new_att = { 0 };
	rsi_ble_pesentation_format_t presentation_format;
	uint8_t format_data[7];

	//!adding descriptor fileds
	format_data[0] = presentation_format.format = RSI_BLE_UINT8_FORMAT;
	format_data[1] = presentation_format.exponent = RSI_BLE_EXPONENT;
	presentation_format.unit = RSI_BLE_PERCENTAGE_UNITS_UUID;
	memcpy(&format_data[2], &presentation_format.unit,
			sizeof(presentation_format.unit));
	format_data[4] = presentation_format.name_space = RSI_BLE_NAME_SPACE;
	presentation_format.description = RSI_BLE_DESCRIPTION;
	memcpy(&format_data[5], &presentation_format.description,
			sizeof(presentation_format.description));

	//! adding new service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_BATTERY_SERVICE_UUID;
	rsi_ble_add_service(new_uuid, &new_serv_resp);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_BATTERY_LEVEL_UUID;
	rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 1,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
			new_serv_resp.start_handle + 2, new_uuid);
	//! adding characteristic value attribute to the service
	rsi_ble_battery_val_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_BATTERY_LEVEL_UUID;
	rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 2, new_uuid,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY, data,
			sizeof(data));

	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_CHAR_PRESENTATION_FORMATE_UUID;
	rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 4, new_uuid,
			RSI_BLE_ATT_PROPERTY_READ, format_data, sizeof(format_data));

	return 0;
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_gatt_test_app_task
 * @brief      this is the application of ble GATT client api's test cases.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used at application.
 */
int32_t rsi_ble_simple_gatt_test_app_init(void) {
	int32_t status = 0;

	//! registering the GAP call back functions
	rsi_ble_gap_register_callbacks(rsi_ble_on_adv_reports,
			rsi_ble_on_connect_event,
            rsi_ble_on_disconnect_event,
			NULL,
			NULL,
			NULL,
			rsi_ble_on_enhance_conn_status_event,
			NULL,
			NULL);

	//! registering the GATT call back functions
	rsi_ble_gatt_register_callbacks(NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL, rsi_ble_on_gatt_write_event,
	NULL,
	NULL,
	NULL,
	NULL,
	rsi_ble_gatt_error_event,
	rsi_ble_gatt_desc_val_event,
	rsi_ble_profiles_list_event,
	rsi_ble_profile_event,
	rsi_ble_char_services_event,
	NULL,
	rsi_ble_on_att_desc_event,
	rsi_ble_on_read_event,
	rsi_ble_write_event,
  rsi_ble_on_event_indication_confirmation,
	NULL);

	//!  initializing the application events map
	rsi_ble_app_init_events();

	//! Set local name
	rsi_bt_set_local_name(RSI_BLE_APP_GATT_TEST);
	//! get the local device address(MAC address).
	status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
	if (status != RSI_SUCCESS) {
		return status;
	}
   rsi_6byte_dev_address_to_ascii (local_device_address, rsi_app_resp_get_dev_addr);
   LOG_PRINT ("LOCAL_DEVICE_ADDRESS : %s\r\n", local_device_address);

  //! adding BLE Battery Service service
   rsi_ble_add_battery_service_serv ();

	//! set device in advertising mode.
	status = rsi_ble_start_advertising();
	if (status != RSI_SUCCESS) {
		return;
	}
}

int32_t rsi_ble_simple_gatt_test_app_task(void) {
	int32_t status = 0;
	int32_t event_id;
	int8_t data[20] = { 0 }, ix;
	uint8_t remote_dev_addr[18] = { 0 };
	uuid_t search_serv = { { 0 } };
	uint8_t loop = 0;

	//! Application main loop
#ifndef RSI_WITH_OS
	rsi_wireless_driver_task();
#endif
	//! checking for events list
	event_id = rsi_ble_app_get_event();
	if (event_id == -1) {
//			continue;
	}

	switch (event_id) {
	case RSI_BLE_ADV_REPORT_EVENT: {
		//! advertise report event.
		//! clear the advertise report event.
		rsi_ble_app_clear_event(RSI_BLE_ADV_REPORT_EVENT);
		status = rsi_ble_connect(remote_addr_type, (int8_t *) remote_dev_bd_addr);
		if (status != RSI_SUCCESS) {
			LOG_PRINT("connect status: 0x%X\r\n", status);
		}

	}
		break;

	case RSI_BLE_CONN_EVENT: {
		//! event invokes when connection was completed

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);
		//rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr, app_conn_event.dev_addr);
		rsi_ble_get_profiles_async(app_conn_event.dev_addr, 1, 0xffff, NULL);
	}
		break;

	case RSI_BLE_DISCONN_EVENT: {
		//! event invokes when disconnection was completed

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);

		//! set device in advertising mode.
		adv: status = rsi_ble_start_advertising();
		if (status != RSI_SUCCESS) {
			goto adv;
		}

	}
		break;

	case RSI_BLE_GATT_WRITE_EVENT: {
		//! event invokes when write/notification events received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_WRITE_EVENT);
	}
		break;

	case RSI_BLE_GATT_PROFILES: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_PROFILES);
		LOG_PRINT("\nin gatt test:rsi_ble_gatt_profiles\n");
		//rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr, app_conn_event.dev_addr);
		search_serv.size = 0x02;
		search_serv.val.val16 = 0x1800;
		rsi_ble_get_profile_async(app_conn_event.dev_addr, search_serv, NULL);
	}
		break;

	case RSI_BLE_GATT_PROFILE: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_PROFILE);

		//rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr, app_conn_event.dev_addr);
		LOG_PRINT(
				"\n\nGatt profile:\nStart handle: 0x%x  \nEnd handle:0x%x \n\n\n",
				*((uint16_t *) get_profile.start_handle),
				*((uint16_t *) get_profile.end_handle));
		//! Get characteristic services within battery service profile

		rsi_ble_get_char_services_async(app_conn_event.dev_addr,
				*((uint16_t *) get_profile.start_handle),
				*((uint16_t *) get_profile.end_handle), NULL);
	}
		break;

	case RSI_BLE_GATT_CHAR_SERVICES: {
		rsi_ble_app_clear_event(RSI_BLE_GATT_CHAR_SERVICES);
		//rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr, app_conn_event.dev_addr);
for(ix=0;ix<get_char_services.num_of_services;ix++){ 

                LOG_PRINT("\n\nCharecteristic services:\nChar_serv_handle:0x%x \nProperty:0x%x \nChar_value_handle:0x%x \nUuid:0x%x \n\n",
                    (get_char_services.char_services[ix].handle),
                    (get_char_services.char_services[ix].char_data.char_property),
                    (get_char_services.char_services[ix].char_data.char_handle),
                    get_char_services.char_services[ix].char_data.char_uuid.val.val16);

                if(ix<(get_char_services.num_of_services - 1)){
                  //!Get descriptors from next handle of characteristic definition to previous handle of the next characteristic service 
                  rsi_ble_get_att_descriptors_async(app_conn_event.dev_addr,
                      (get_char_services.char_services[ix].char_data.char_handle + 1), 
                      get_char_services.char_services[ix+1].handle - 1,   
                      &get_desc_services);      //f7
                }
                else{ 
                  //! Get descriptors from next handle of characteristic definition to profile end handle because it has only one service
                  rsi_ble_get_att_descriptors_async(app_conn_event.dev_addr,
                      (get_char_services.char_services[ix].char_data.char_handle + 1), 
                      *((uint16_t *)get_profile.end_handle),  
                      NULL);      //f7

                }
              }
           }
		break;
	case RSI_BLE_GATT_DESC_SERVICES: {
	   rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_SERVICES);
	   //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
	}
		break;
	case RSI_BLE_GATT_READ: {
		rsi_ble_app_clear_event(RSI_BLE_GATT_READ);
		//rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr, app_conn_event.dev_addr);
            search_serv.size = 2;
			search_serv.val.val16 = RSI_BLE_BATTERY_LEVEL_UUID;
			rsi_ble_get_char_value_by_uuid_async(app_conn_event.dev_addr, 0x01, 0xff,
					search_serv, NULL);
	}
		break;
	case RSI_BLE_GATT_WRITE: {
		rsi_ble_app_clear_event(RSI_BLE_GATT_WRITE);
		rsi_ble_set_att_cmd(remote_dev_bd_addr, 3, 7, "redpine");
	}
		break;
	case RSI_BLE_GATT_ERROR: {
		rsi_ble_app_clear_event(RSI_BLE_GATT_ERROR);
	}
		break;

	case RSI_BLE_GATT_DESC_VAL: {
		rsi_ble_app_clear_event(RSI_BLE_GATT_DESC_VAL);
		LOG_PRINT("\n get att value \n");
		LOG_PRINT("\n\nDescriptors:");
		LOG_PRINT("\nNo.of attributes: %d \n", get_desc_services.num_of_att);

		for (ix = 0; ix < get_desc_services.num_of_att; ix++) {
			LOG_PRINT("\nHandle:0x%x  Uuid:0x%x \n",
					*((uint16_t *) get_desc_services.att_desc[ix].handle),
					get_desc_services.att_desc[ix].att_type_uuid.val.val16);

			if (get_desc_services.att_desc[ix].att_type_uuid.val.val16
					== RSI_BLE_CLIENT_CHAR_UUID) {
				data[1] = 0x0;
				if (get_char_services.char_services[0].char_data.char_property
						& RSI_BLE_ATT_PROPERTY_NOTIFY) {
					data[0] = RSI_BLE_NOTIFY_VALUE;
				} else {
					data[0] = RSI_BLE_INDICATE_VALUE;
				}
				rsi_ble_set_att_value_async(remote_dev_bd_addr,
						*((uint16_t *) get_desc_services.att_desc[ix].handle),
						2, data);
			}
		}
		rsi_ble_get_att_value_async(remote_dev_bd_addr, 3, NULL);
	}
    break;
  case RSI_BLE_GATT_INDICATION_CONFIRMATION: {
    rsi_ble_app_clear_event(RSI_BLE_GATT_INDICATION_CONFIRMATION);
  }
		break;
	}
	return 0;
}

#endif
