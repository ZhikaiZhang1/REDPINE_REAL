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
#include "rsi_common_app.h"
#if BLE_LONG_READ
//! BLE include files to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_common_apis.h>

#include <rsi_bootup_config.h>

#include <stdio.h>
#include <string.h>

uint16_t mtu_size;
int8_t offset_in_read, req_type, offset_in_write;
uint8_t wr_buf[256];
uint8_t read_data[200] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
		34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
		70, 71, 72, 72, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
		25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
		61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 72, 74, 75, 76, 77, 78,
		79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
		97, 98, 99, 100, };

int8_t data[200] = { 0 };


//! user defined structure
/***********************************************************************************************************************************************/
typedef struct rsi_ble_att_list_s{
  uuid_t  char_uuid;
  uint16_t  handle;
  uint16_t  len;
  void     *value;
}rsi_ble_att_list_t;

typedef struct rsi_ble_s{
  uint8_t   DATA[BLE_ATT_REC_SIZE];
  uint16_t  DATA_ix;
  uint16_t  att_rec_list_count;
  rsi_ble_att_list_t  att_rec_list[NO_OF_VAL_ATT];
}rsi_ble_t;

rsi_ble_t att_list;

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t app_ble_write_event;
static uint16_t rsi_ble_att1_val_hndl;
static rsi_ble_read_req_t app_ble_read_event;
static rsi_ble_event_mtu_t app_ble_mtu_event;
static profile_descriptors_t rsi_ble_service;
static rsi_ble_resp_char_services_t char_servs;
rsi_ble_event_prepare_write_t rsi_app_ble_write_buf;
static rsi_ble_event_remote_features_t 		remote_dev_feature;


/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events() {
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
 * @brief      clears the specific event.
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
static int32_t rsi_ble_app_get_event(void) {
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
 * @fn         rsi_gatt_add_attribute_to_list
 * @brief      This function is used to store characteristic service attribute.
 * @param[in]  p_val, pointer to the structure
 * @param[in]  handle, characteristic service attribute handle.
 * @param[in]  data_len, characteristic value length
 * @param[in]  data, characteristic value pointer
 * @param[in]  uuid, characteristic value uuid
 * @return     none.
 * @section description
 * This function is used to store all attribute records
 */
void rsi_gatt_add_attribute_to_list(rsi_ble_t *p_val, uint16_t handle,
		uint16_t data_len, uint8_t *data, uuid_t uuid) {
	if ((p_val->DATA_ix + data_len) >= BLE_ATT_REC_SIZE) {
		LOG_PRINT("no data memory for att rec values");
		return;
	}

	p_val->att_rec_list[p_val->att_rec_list_count].char_uuid = uuid;
	p_val->att_rec_list[p_val->att_rec_list_count].handle = handle;
	p_val->att_rec_list[p_val->att_rec_list_count].len = data_len;
	memcpy(p_val->DATA + p_val->DATA_ix, data, data_len);
	p_val->att_rec_list[p_val->att_rec_list_count].value = p_val->DATA
			+ p_val->DATA_ix;
	p_val->att_rec_list_count++;
	p_val->DATA_ix += data_len;

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
		uuid_t att_type_uuid, uint8_t val_prop, uint8_t *data, uint8_t data_len,
		uint8_t auth_read) {
	rsi_ble_req_add_att_t new_att = { 0 };

	//! preparing the attributes
	new_att.serv_handler = serv_handler;
	new_att.handle = handle;
	new_att.reserved = auth_read;
	memcpy(&new_att.att_uuid, &att_type_uuid, sizeof(uuid_t));
	new_att.property = val_prop;

	//! preparing the attribute value
	if (data != NULL)
		memcpy(new_att.data, data, RSI_MIN(sizeof(new_att.data), data_len));
	new_att.data_len = data_len;

	//! add attribute to the service
	rsi_ble_add_attribute(&new_att);

	if ((auth_read == ATT_REC_MAINTAINED_IN_HOST) || (data_len > 20)) {
		if (data != NULL)
			rsi_gatt_add_attribute_to_list(&att_list, handle, data_len, data,
					att_type_uuid);
	}

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
 * @fn         rsi_ble_simple_chat_add_new_serv
 * @brief      this function is used to add new servcie i.e.., simple chat service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */

static uint32_t rsi_ble_add_simple_chat_serv(void) {
	uuid_t new_uuid = { 0 };
	rsi_ble_resp_add_serv_t new_serv_resp = { 0 };
	uint8_t data1[200] = { 1, 0 };

	//! adding new service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_NEW_SERVICE_UUID;
	rsi_ble_add_service(new_uuid, &new_serv_resp);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
	rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 1,
			RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ
					| RSI_BLE_ATT_PROPERTY_NOTIFY,
			new_serv_resp.start_handle + 2, new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
	rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 2, new_uuid,
			RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ
					| RSI_BLE_ATT_PROPERTY_NOTIFY, data1, sizeof(data1), ATT_REC_MAINTAINED_IN_HOST);
	return 0;
}

/*==============================================*/
/**
 * @fn         rsi_ble_profile
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] p_ble_resp_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_profile(uint16_t resp_status,
		profile_descriptors_t *rsi_ble_resp_profile) {
	rsi_ble_app_set_event(RSI_BLE_GATT_PROFILE_RESP_EVENT);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_char_services
 * @brief      invoked when response is received for characteristic services details
 * @param[out] p_ble_resp_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_char_services(uint16_t resp_status,
		rsi_ble_resp_char_services_t *rsi_ble_resp_char_services) {
	rsi_ble_app_set_event(RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);
	return;
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
static void rsi_ble_on_gatt_write_event(uint16_t event_id,
		rsi_ble_event_write_t *rsi_ble_write) {
	memcpy(&app_ble_write_event, rsi_ble_write, sizeof(rsi_ble_event_write_t));
	rsi_ble_app_set_event(RSI_BLE_GATT_WRITE_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_read_req_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_read_req_event(uint16_t event_id,
		rsi_ble_read_req_t *rsi_ble_read_req) {
	memcpy(&app_ble_read_event, rsi_ble_read_req, sizeof(rsi_ble_read_req_t));
	 req_type = (rsi_ble_read_req)->type;
	 offset_in_read   = rsi_ble_read_req->offset;
	rsi_ble_app_set_event(RSI_BLE_READ_REQ_EVENT);
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
	uint8_t str_remote_address[18] = { 0 };
	//rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, resp_conn->dev_addr);
	memcpy ((int8_t *)conn_event_to_app.dev_addr, resp_conn->dev_addr, 6);
	LOG_PRINT("\n connected to : %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, resp_conn->dev_addr));
	memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
	rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
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
	uint8_t str_remote_address[18] = { 0 };
	//rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, resp_enh_conn->dev_addr);
	memcpy(&conn_event_to_app, resp_enh_conn, sizeof(rsi_ble_event_enhance_conn_status_t));
	LOG_PRINT("\n connected to : %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, resp_enh_conn->dev_addr));
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
	memcpy(&disconn_event_to_app, resp_disconnect,
			sizeof(rsi_ble_event_conn_status_t));
	rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_mtu_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_mtu_event(rsi_ble_event_mtu_t *rsi_ble_mtu) {
	memcpy(&app_ble_mtu_event, rsi_ble_mtu, sizeof(rsi_ble_event_mtu_t));
	mtu_size = (uint16_t) app_ble_mtu_event.mtu_size;
	LOG_PRINT("\n MTU size is: %d", mtu_size);
	rsi_ble_app_set_event(RSI_BLE_MTU_EVENT);
}

/**
 * @callback   rsi_ble_on_gatt_write_event_t
 * @brief      Callback function to be called if the GATT write/notify/indicate events are received
 * @param[in]  event_id, contains the event id (RSI_BLE_EVENT_GATT_NOTIFICATION,  RSI_BLE_EVENT_GATT_INDICATION, RSI_BLE_EVENT_GATT_WRITE)
 * @param[in]  rsi_ble_write,  contains the GATT event information
 * @return      void
 * @section description
 * This callback function will be called if the GATT write/notify/indicate events are received
 * This callback has to be registered using rsi_ble_gatt_register_callbacks API
 */

void  ble_on_gatt_prepare_write_event(uint16_t event_id, rsi_ble_event_prepare_write_t *rsi_ble_write)
{
	uint16_t offset = rsi_ble_write->offset[0] | (rsi_ble_write->offset[1] << 8);
	memcpy(&rsi_app_ble_write_buf, rsi_ble_write, sizeof(rsi_ble_event_prepare_write_t));
	memcpy(&wr_buf[offset], rsi_ble_write->att_value, rsi_ble_write->length);
}

/**
 * @callback   rsi_ble_on_execute_write_event_t
 * @brief      Callback function to be called if the GATT write/notify/indicate events are received
 * @param[in]  event_id, contains the event id (RSI_BLE_EVENT_GATT_NOTIFICATION,  RSI_BLE_EVENT_GATT_INDICATION, RSI_BLE_EVENT_GATT_WRITE)
 * @param[in]  rsi_ble_write,  contains the GATT event information
 * @return      void
 * @section description
 * This callback function will be called if the GATT write/notify/indicate events are received
 * This callback has to be registered using rsi_ble_gatt_register_callbacks API
 */
void  ble_on_execute_write_event(uint16_t event_id, rsi_ble_execute_write_t *rsi_ble_execute_write)
{
	LOG_PRINT ("\nReceived data from remote device through Prepare Write Event is : %s", wr_buf);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_peripheral_on_remote_features_event
 * @brief      invoked when LE remote features event is received.
 * @param[in] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_peripheral_on_remote_features_event(
		rsi_ble_event_remote_features_t *rsi_ble_event_remote_features) {
	LOG_PRINT("\n Remote device supported feature bitmap is %x",
			rsi_ble_event_remote_features->remote_features);
	memcpy(&remote_dev_feature, rsi_ble_event_remote_features,
			sizeof(rsi_ble_event_remote_features_t));
	rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
}

/*==============================================*/
/**
 * @fn         rsi_ble_data_length_change_event
 * @brief      invoked when data length is set
 * @section description
 * This Callback function indicates data length is set
 */
void rsi_ble_data_length_change_event(
		rsi_ble_event_data_length_update_t *rsi_ble_data_length_update) {
	LOG_PRINT("\nMax_tx_octets: %d \r\n",
			rsi_ble_data_length_update->MaxTxOctets);
	LOG_PRINT("Max_tx_time: %d \r\n", rsi_ble_data_length_update->MaxTxTime);
	LOG_PRINT("Max_rx_octets: %d \r\n",
			rsi_ble_data_length_update->MaxRxOctets);
	LOG_PRINT("Max_rx_time: %d \r\n", rsi_ble_data_length_update->MaxRxTime);
	rsi_ble_app_set_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);
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
int32_t rsi_app_init_ble_simple_gatt_test(void) {
	uint8_t adv[31] = { 2, 1, 6 };

	rsi_ble_add_simple_chat_serv();

	//! registering the GAP call back functions

	rsi_ble_gap_register_callbacks(
	NULL, rsi_ble_on_connect_event, rsi_ble_on_disconnect_event,
	NULL,
	NULL,
	rsi_ble_data_length_change_event,
	rsi_ble_on_enhance_conn_status_event,
	NULL,
	NULL);

	rsi_ble_gap_extended_register_callbacks(
			rsi_ble_simple_peripheral_on_remote_features_event,
      NULL);

	//! registering the GATT call back functions
	rsi_ble_gatt_register_callbacks(
			NULL,
			rsi_ble_profile,
			rsi_ble_char_services,
			NULL,
			NULL,
			NULL,
			NULL,
			rsi_ble_on_gatt_write_event,
			ble_on_gatt_prepare_write_event,
			ble_on_execute_write_event,
			rsi_ble_on_read_req_event,
			rsi_ble_on_mtu_event,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

	//!  initializing the application events map
	rsi_ble_app_init_events();

	//! Set local name
	rsi_bt_set_local_name(RSI_BLE_APP_GATT_TEST);

	//! prepare advertise data //local/device name
	adv[3] = strlen(RSI_BLE_APP_GATT_TEST) + 1;
	adv[4] = 9;
	strcpy((char *) &adv[5], RSI_BLE_APP_GATT_TEST);

	//! set advertise data
	rsi_ble_set_advertise_data(adv, strlen(RSI_BLE_APP_GATT_TEST) + 5);

	//! set device in advertising mode.
	rsi_ble_start_advertising();

}


int32_t rsi_ble_app_task() {
	int32_t status = 0;
	int32_t event_id;
	uint8_t remote_dev_addr[18] = { 0 };
	rsi_ble_resp_local_att_value_t local_att_val;

	//! Application main loop
	//rsi_wireless_driver_task();

	//! checking for events list
	event_id = rsi_ble_app_get_event();

	switch (event_id) {
	case RSI_BLE_CONN_EVENT: {
		//! event invokes when connection was completed

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);
		//rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,(uint8_t *) conn_event_to_app.dev_addr);
//		LOG_PRINT("\n Device Connected");
	}
		break;

	case RSI_BLE_DISCONN_EVENT: {
		//! event invokes when disconnection was completed

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);
		LOG_PRINT("\n Device disconnected");
		adv:
		//! set device in advertising mode.
		status = rsi_ble_start_advertising();
		if (status != RSI_SUCCESS) {
			goto adv;
		}
		LOG_PRINT("\n Advertising after disconnect");

	}
		break;
	case RSI_BLE_GATT_WRITE_EVENT: {
		//! event invokes when write/notification events received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_WRITE_EVENT);

		//! prepare the data to set as local attribute value.
		memset(data, 0, sizeof(data));
		memcpy(data, app_ble_write_event.att_value, app_ble_write_event.length);

		rsi_ble_get_local_att_value(rsi_ble_att1_val_hndl, &local_att_val);
		//! set the local attribute value.
		rsi_ble_set_local_att_value(rsi_ble_att1_val_hndl, RSI_BLE_MAX_DATA_LEN,
				(uint8_t *) data);
		rsi_ble_get_local_att_value(rsi_ble_att1_val_hndl, &local_att_val);

		LOG_PRINT("\n Received data length from remote device through RSI_BLE_GATT_WRITE_EVENT is %d", app_ble_write_event.length);
		LOG_PRINT("\n Received data from remote device through RSI_BLE_GATT_WRITE_EVENT %s", data);
	}
		break;
	case RSI_BLE_READ_REQ_EVENT: {
		//! event invokes when write/notification events received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_READ_REQ_EVENT);

		//rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,(uint8_t *) conn_event_to_app.dev_addr);
		if (req_type == 0) {
			LOG_PRINT("\nRead Request came to the host\n");
			rsi_ble_gatt_read_response(conn_event_to_app.dev_addr, 0,
					app_ble_read_event.handle, 0, (sizeof(read_data)),
					read_data);
		} else {
			LOG_PRINT(
					"\nRead blob Request came to the host with the offset value as %d\n",
					offset_in_read);

			rsi_ble_gatt_read_response(conn_event_to_app.dev_addr, 1,
					app_ble_read_event.handle, offset_in_read,
					(sizeof(read_data) - offset_in_read), &(read_data[offset_in_read]));
		}
	}
		break;

	case RSI_BLE_GATT_PROFILE_RESP_EVENT: {
		//! event invokes when get profile response received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_PROFILE_RESP_EVENT);
	}
		break;

	case RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT: {
		//! event invokes when get characteristics of the service response received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);
	}
		break;

	case RSI_BLE_MTU_EVENT: {
		//! event invokes when write/notification events received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_MTU_EVENT);
	}
		break;
	case RSI_BLE_RECEIVE_REMOTE_FEATURES: {
			//! clear the served event
			rsi_ble_app_clear_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);

			//rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,(uint8_t *) conn_event_to_app.dev_addr);
			if (remote_dev_feature.remote_features[0] & 0x20) {
				status = rsi_ble_set_data_len(conn_event_to_app.dev_addr, TX_LEN, TX_TIME);

				if (status != RSI_SUCCESS) {
					return status;
				}
			}
	}
		break;
	case RSI_APP_EVENT_DATA_LENGTH_CHANGE: {
			//! clear the disconnected event.
			rsi_ble_app_clear_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);
		}
		break;
	}

	return;
}
#endif
