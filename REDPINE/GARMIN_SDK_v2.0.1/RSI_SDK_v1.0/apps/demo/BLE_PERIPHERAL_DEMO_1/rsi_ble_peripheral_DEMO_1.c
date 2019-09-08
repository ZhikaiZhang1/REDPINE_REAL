/**
 * @file    rsi_ble_peripheral_DEMO_1.c
 * @version 0.1
 * @date    xx Oct 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BT Classic SPP Slave Role
 *
 *  @section Description  This application serves as a BT SPP Slave.
 *
 */

/*=======================================================================*/
//   ! INCLUDES
/*=======================================================================*/

#include <rsi_common_app.h>
#if BLE_PERIPHERAL
//! BT include file to refer BT APIs
#include <rsi_driver.h>
#include <rsi_bt_common_apis.h>
#include <rsi_ble_apis.h>
#include <rsi_bt.h>
#include <stdio.h>
#include <rsi_ble.h>

/*=======================================================================*/
//   ! MACROS
/*=======================================================================*/

/*=======================================================================*/
//   ! GLOBAL VARIABLES
/*=======================================================================*/

uint32_t         start_handle;
uint32_t         end_handle;
uint32_t         number_of_profiles;
uint8_t          profiles_list_done;
uint8_t          conn_dev_addr[18] = { 0 };
uint16_t         num_of_conn_slaves = 0;
uint8_t          num_of_conn_masters = 0;
uint16_t         conn_req_pending = 0;
uint8_t          rsi_conn_update = 0;
uint8_t          req_type, offset;
uint16_t         handle;
#define   MITM_ENABLE 0x1

static uint8_t   adv_128bit_uuid[16] = { 0x6e, 0x40, 0x00, 0x01, 0xb5, 0xa3, 0xf3, 0x93, 0xe0, 0xa9, 0xe5, 0x0e, 0x24, 0xdc, 0xca, 0x9e };     //! 128bit UUID
static uint8_t   manufacturing_data[2] = { 0x00, 0x87 };                                                                                       //! Manufacturing data
static uint8_t   first_connect = 0;
static uint32_t  passkey;
static uint8_t   str_remote_address[18];
static uint8_t   remote_dev_address[6];
static uint8_t   rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = { 0 };
static uint32_t  rsi_app_async_event_map = 0;
static uint32_t  rsi_ble_async_event_mask = 0;
static uint8_t   rsi_app_resp_rssi = 0;
static uint16_t  rsi_ble_att1_val_hndl;
static uint16_t  rsi_ble_att2_val_hndl;
static uint16_t  rsi_ble_att3_val_hndl;
static uint16_t  rsi_ble_att4_val_hndl;
static uint16_t  rsi_ble_write_att1_val_hndl;
static uint16_t  rsi_ble_write_att2_val_hndl, notification_handle;

//! Structure Variables
rsi_ble_t                                   att_list;
static rsi_ble_event_phy_update_t   		rsi_app_phy_update_complete;
static profile_descriptors_t        		rsi_ble_service;
static rsi_ble_resp_char_services_t 		char_servs;
static rsi_ble_read_req_t           		app_ble_read_event;
static rsi_ble_event_remote_features_t 		remote_dev_feature;
static rsi_bt_event_le_ltk_request_t 		temp_le_ltk_req;
static rsi_bt_event_encryption_enabled_t 	glbl_enc_enabled;
static rsi_bt_event_le_security_keys_t 		temp_le_sec_keys;
static rsi_ble_event_write_t 				app_ble_write_event;
static rsi_bt_resp_get_local_name_t         rsi_app_resp_get_local_name = { 0 };
static rsi_ble_resp_profiles_list_t         prof_list;
static profile_descriptors_t                list_of_profs[100];

/*=======================================================================*/
//   ! EXTERN VARIABLES
/*=======================================================================*/

/*=======================================================================*/
//   ! EXTERN FUNCTIONS
/*=======================================================================*/

/*========================================================================*/
//!  CALLBACK FUNCTIONS
/*=======================================================================*/

/*=======================================================================*/
//   ! PROCEDURES
/*=======================================================================*/

/**
 * @fn         rsi_ble_fill_128bit_uuid
 * @brief      this function is used to form 128bit uuid of apple device from 32 bit uuid.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */
static void rsi_ble_fill_128bit_uuid(uint32_t uuid_32bit, uuid_t *serv_uuid) {
	uint8_t ix;
	serv_uuid->size = 16;
	rsi_uint32_to_4bytes((uint8_t *)&serv_uuid->val.val128.data1, uuid_32bit);
	rsi_uint16_to_2bytes((uint8_t *)&serv_uuid->val.val128.data2, 0x00);
	rsi_uint16_to_2bytes((uint8_t *)&serv_uuid->val.val128.data3, 0x1000);
	rsi_uint16_to_2bytes(&serv_uuid->val.val128.data4[0], 0x8000);
	for (ix = 0; ix < 6; ix++) {
		serv_uuid->val.val128.data4[2] = 0x26;
		serv_uuid->val.val128.data4[3] = 0x00;
		serv_uuid->val.val128.data4[4] = 0x91;
		serv_uuid->val.val128.data4[5] = 0x52;
		serv_uuid->val.val128.data4[6] = 0x76;
		serv_uuid->val.val128.data4[7] = 0xBB;
	}

	return;
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
 * @param[in]  p_val, pointer to homekit structure
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
		LOG_PRINT("\n no data memory for att rec values \n");
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

static void rsi_ble_add_char_val_att(void *serv_handler, uint16_t handle,
		uuid_t att_type_uuid, uint8_t val_prop, uint8_t *data, uint8_t data_len,
		uint8_t auth_read) {
	rsi_ble_req_add_att_t new_att = { 0 };

	//! preparing the attributes
	new_att.serv_handler = serv_handler;
	new_att.handle = handle;
	new_att.config_bitmap = auth_read;
	memcpy(&new_att.att_uuid, &att_type_uuid, sizeof(uuid_t));
	new_att.property = val_prop;

	if (data != NULL)
		memcpy(new_att.data, data, RSI_MIN(sizeof(new_att.data), data_len));

	//! preparing the attribute value
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
static uint32_t rsi_ble_add_simple_chat_serv1(void) {
	uuid_t new_uuid = { 0 };
	rsi_ble_resp_add_serv_t new_serv_resp = { 0 };
	uint8_t data[20] = { "redpine_sampletest" };

	//! adding new service
	new_uuid.size = 16;
	new_uuid.val.val32 = RSI_BLE_NEW_SERVICE_UUID;
	rsi_ble_fill_128bit_uuid(RSI_BLE_NEW_SERVICE_UUID, &new_uuid);

	rsi_ble_add_service(new_uuid, &new_serv_resp);

	//! adding characteristic service attribute to the service
	new_uuid.size = 16;
	new_uuid.val.val32 = RSI_BLE_ATTRIBUTE_1_UUID;
	rsi_ble_fill_128bit_uuid(RSI_BLE_ATTRIBUTE_1_UUID, &new_uuid);
	rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 1,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE,
			new_serv_resp.start_handle + 2, new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size = 16;
	new_uuid.val.val32 = RSI_BLE_ATTRIBUTE_1_UUID;
	rsi_ble_fill_128bit_uuid(RSI_BLE_ATTRIBUTE_1_UUID, &new_uuid);
	rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 2, new_uuid,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE, data,
			sizeof(data), 0);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_2_UUID;
	rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 3,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
			new_serv_resp.start_handle + 4, new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_att2_val_hndl = new_serv_resp.start_handle + 4;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_2_UUID;
	rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 4, new_uuid,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY, data,
			sizeof(data), 0);

	return 0;
}

static uint32_t rsi_ble_add_simple_chat_serv2(void) {
	uuid_t new_uuid = { 0 };
	rsi_ble_resp_add_serv_t new_serv_resp = { 0 };

	uint8_t data[20] = { "12345678901234567890" };
	uint8_t data1[40] = { "redpine_sampletestredpine_sampletest" };

	//! adding new service
	new_uuid.size = 16;
	new_uuid.val.val32 = RSI_BLE_NEW_SERVICE_UUID2;
	rsi_ble_fill_128bit_uuid(RSI_BLE_NEW_SERVICE_UUID2, &new_uuid);

	rsi_ble_add_service(new_uuid, &new_serv_resp);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val32 = RSI_BLE_ATTRIBUTE_3_UUID;
	rsi_ble_fill_128bit_uuid(RSI_BLE_ATTRIBUTE_3_UUID, &new_uuid);
	rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 1,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE,
			new_serv_resp.start_handle + 2, new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_att3_val_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size = 16;
	new_uuid.val.val32 = RSI_BLE_ATTRIBUTE_3_UUID;
	rsi_ble_fill_128bit_uuid(RSI_BLE_ATTRIBUTE_3_UUID, &new_uuid);
	rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 2, new_uuid,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE, data,
			sizeof(data), 0);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_4_UUID;
	rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 3,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE,
			new_serv_resp.start_handle + 4, new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_att4_val_hndl = new_serv_resp.start_handle + 4;
	new_uuid.size = 16;
	new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_4_UUID;
	rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 4, new_uuid,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE, data1,
			sizeof(data1), ATT_REC_MAINTAINED_IN_HOST);

	return 0;
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
static void rsi_ble_app_init_events() {
	rsi_app_async_event_map = 0;
	rsi_ble_async_event_mask = 0xFFFFFFFF;
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
static void rsi_ble_app_set_event(uint32_t event_num) {
	rsi_app_async_event_map |= BIT(event_num);
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
	rsi_app_async_event_map &= ~BIT(event_num);
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
		if (rsi_app_async_event_map & (1 << ix)) {
			return ix;
		}
	}

	return (RSI_FAILURE);
}


/*==============================================*/
/**
 * @fn         rsi_ble_on_adv_report_event
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
void rsi_ble_on_adv_report_event(rsi_ble_event_adv_report_t *adv_report) {

	rsi_ble_app_set_event(RSI_BLE_ADV_REPORT_EVENT);
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
	memcpy ((int8_t *)remote_dev_address, resp_enh_conn->dev_addr, 6);
	LOG_PRINT("\nconnect - str_remote_address : %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, resp_enh_conn->dev_addr));
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
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn)
{
	memcpy ((int8_t *)remote_dev_address, resp_conn->dev_addr, 6);
	LOG_PRINT("\nconnect - str_remote_address : %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, resp_conn->dev_addr));
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
 * This Callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
	//uint8_t str_disconn_device[18] = { 0 };

	//memcpy ((int8_t *) str_disconn_device, (uint8_t *) resp_disconnect->dev_addr);
	rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
}

static void rsi_ble_on_read_req_event(uint16_t event_id,
		rsi_ble_read_req_t *rsi_ble_read_req) {
	offset = 0;
	req_type = 0;
	memcpy(&app_ble_read_event, rsi_ble_read_req, sizeof(rsi_ble_read_req_t));
	if (rsi_ble_read_req->type == 1)
		req_type = 1;
	offset = rsi_ble_read_req->offset;
	handle = rsi_ble_read_req->handle;
	rsi_6byte_dev_address_to_ascii((int8_t *) str_remote_address,
			rsi_ble_read_req->dev_addr);
  memcpy (remote_dev_address, rsi_ble_read_req->dev_addr, 6);
	LOG_PRINT("\nOffset value is %d\n", offset);
	rsi_ble_app_set_event(RSI_BLE_READ_REQ_EVENT);
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
	LOG_PRINT("\nFeature received is %d\n",
			rsi_ble_event_remote_features->remote_features);
	memcpy(&remote_dev_feature, rsi_ble_event_remote_features,
			sizeof(rsi_ble_event_remote_features_t));
	rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
}

/*
static void rsi_ble_on_mtu_event(rsi_ble_event_mtu_t *rsi_ble_mtu) {
	rsi_ble_app_set_event(RSI_BLE_MTU_EVENT);
}
*/

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
 * @fn         rsi_ble_phy_update_complete_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This Callback function indicates disconnected device information and status
 */
void rsi_ble_phy_update_complete_event(
		rsi_ble_event_phy_update_t *rsi_ble_event_phy_update_complete) {
	memcpy(&rsi_app_phy_update_complete, rsi_ble_event_phy_update_complete,
			sizeof(rsi_ble_event_phy_update_t));

	rsi_ble_app_set_event(RSI_APP_EVENT_PHY_UPDATE_COMPLETE);
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
void rsi_ble_on_smp_request(rsi_bt_event_smp_req_t *remote_dev_address) {
	memcpy ((int8_t *)remote_dev_address, remote_dev_address->dev_addr, 6);
	LOG_PRINT("\nsmp_req - str_remote_address : %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, remote_dev_address->dev_addr));
	rsi_ble_app_set_event(RSI_BLE_SMP_REQ_EVENT);
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
void rsi_ble_on_smp_response(rsi_bt_event_smp_resp_t *remote_dev_address) {
	memcpy ((int8_t *)remote_dev_address, remote_dev_address->dev_addr, 6);
	LOG_PRINT("\nsmp_resp - str_remote_address : %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, remote_dev_address->dev_addr));
	rsi_ble_app_set_event(RSI_BLE_SMP_RESP_EVENT);
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
void rsi_ble_on_smp_passkey(rsi_bt_event_smp_passkey_t *remote_dev_address) {
	memcpy ((int8_t *)remote_dev_address, remote_dev_address->dev_addr, 6);
	LOG_PRINT("\nsmp_passkey - str_remote_address : %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, remote_dev_address->dev_addr));
	rsi_ble_app_set_event(RSI_BLE_SMP_PASSKEY_EVENT);
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
void rsi_ble_on_smp_passkey_display(
		rsi_bt_event_smp_passkey_display_t *smp_passkey_display) {
	memcpy ((int8_t *)remote_dev_address,	smp_passkey_display->dev_addr, 6);
	LOG_PRINT("\nremote addr: %s, passkey: %s \r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address,	smp_passkey_display->dev_addr),
			smp_passkey_display->passkey);
	//(RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);

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
void rsi_ble_on_sc_passkey(rsi_bt_event_sc_passkey_t *sc_passkey) {
	memcpy ((int8_t *)remote_dev_address, sc_passkey->dev_addr, 6);
	LOG_PRINT("\nremote addr: %s, passkey: %06d \r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address, sc_passkey->dev_addr),
			sc_passkey->passkey);
	passkey = sc_passkey->passkey;
	rsi_ble_app_set_event(RSI_BLE_SC_PASSKEY_EVENT);
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
void rsi_ble_on_smp_failed(uint16_t status,
		rsi_bt_event_smp_failed_t *remote_dev_address) {
	memcpy ((int8_t *)remote_dev_address,	remote_dev_address->dev_addr, 6);
	LOG_PRINT("\nsmp_failed status: 0x%x, str_remote_address: %s\r\n", status,
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address,	remote_dev_address->dev_addr));
	rsi_ble_app_set_event(RSI_BLE_SMP_FAILED_EVENT);
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
void rsi_ble_on_encrypt_started(uint16_t status,
		rsi_bt_event_encryption_enabled_t *enc_enabled) {
	LOG_PRINT("\nstart encrypt status: %d \r\n", status);
	memcpy(&glbl_enc_enabled, enc_enabled,
			sizeof(rsi_bt_event_encryption_enabled_t));
	rsi_ble_app_set_event(RSI_BLE_ENCRYPT_STARTED_EVENT);
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
static void rsi_ble_on_le_ltk_req_event(
		rsi_bt_event_le_ltk_request_t *le_ltk_req) {
	LOG_PRINT(" \r\n rsi_ble_on_le_ltk_req_event \r\n");
	memcpy(&temp_le_ltk_req, le_ltk_req, sizeof(rsi_bt_event_le_ltk_request_t));
	rsi_ble_app_set_event(RSI_BLE_LTK_REQ_EVENT);
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
static void rsi_ble_on_le_security_keys_event(
		rsi_bt_event_le_security_keys_t *le_sec_keys) {
	LOG_PRINT(" \r\n rsi_ble_on_le_security_keys_event \r\n");
	memcpy(&temp_le_sec_keys, le_sec_keys,
			sizeof(rsi_bt_event_le_security_keys_t));
	rsi_ble_app_set_event(RSI_BLE_SECURITY_KEYS_EVENT);
}
#if 0
/*==============================================*/
/**
 * @fn         add_device_to_resolvlist
 * @brief      this function will add device to resolvlistwith updated irks
 * @param[out] ix, index of resolvlist
 * @return     none.
 * @section description
 * add device to resolvlistwith updated irks
 * */
uint32_t add_device_to_resolvlist(rsi_ble_resolvlist_group_t *resolvlist_p,
		rsi_ble_resolve_key_t *resolve_key_p) {
	uint8_t status;
	uint8_t ix;

	for (ix = 0; ix < RSI_BLE_RESOLVING_LIST_SIZE; ix++) {
		if (resolvlist_p[ix].used == 0) {
			resolvlist_p[ix].used = 1;
			resolvlist_p[ix].Identity_addr_type =
					resolve_key_p->Identity_addr_type;
			memcpy(resolvlist_p[ix].Identity_addr, resolve_key_p->Identity_addr,
					sizeof(resolve_key_p->Identity_addr));
			memcpy(resolvlist_p[ix].peer_irk, resolve_key_p->peer_irk,
					sizeof(resolve_key_p->peer_irk));
			memcpy(resolvlist_p[ix].local_irk, resolve_key_p->local_irk,
					sizeof(resolve_key_p->local_irk));
			break;
		}
	}
	if (ix >= RSI_BLE_RESOLVING_LIST_SIZE) {
		return -1;
	}

	//add device to resolvlist or remove from resolvlist or clear the resolvlist
	status = rsi_ble_resolvlist(RSI_BLE_ADD_TO_RESOLVE_LIST,
			resolve_key_p->Identity_addr_type, resolve_key_p->Identity_addr,
			resolve_key_p->peer_irk, resolve_key_p->local_irk);
	if (status != RSI_SUCCESS) {
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
void update_resolvlist(rsi_ble_resolvlist_group_t *resolvlist_p,
		rsi_ble_resolve_key_t *resolve_key_p) {
	uint8_t ix = 0;
	while (ix < RSI_BLE_RESOLVING_LIST_SIZE) {
		if (!strcmp(resolvlist_p[ix].Identity_addr,
				resolve_key_p->Identity_addr)) {
			if (memcmp(resolvlist_p[ix].peer_irk, resolve_key_p->peer_irk,
					sizeof(resolve_key_p->peer_irk))) {
				add_device_to_resolvlist(resolvlist_p, resolve_key_p);
				break;
			}
		} else {
			strcpy(resolvlist_p[ix].Identity_addr,
					resolve_key_p->Identity_addr);
			add_device_to_resolvlist(resolvlist_p, resolve_key_p);
			break;
		}
		ix++;
	}
}
#endif

void rsi_ble_on_conn_update_complete_event(
		rsi_ble_event_conn_update_t *rsi_ble_event_conn_update_complete,
		uint16_t resp_status) {
	memcpy (remote_dev_address, rsi_ble_event_conn_update_complete->dev_addr, 6);
	rsi_ble_app_set_event(RSI_BLE_CONN_UPDATE_EVENT);
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
	memcpy(&rsi_ble_service, rsi_ble_resp_profile,
			sizeof(profile_descriptors_t));
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
 * @fn         rsi_ble_on_profiles_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] p_ble_resp_profile, specific profile details
 * @return     none
 * @section description
 * this is a callback function
 */
static void ble_on_profiles_list_resp(uint16_t resp_status,
		rsi_ble_resp_profiles_list_t *rsi_ble_resp_profiles) {
	if (resp_status != 0) {
		profiles_list_done = 1;
		LOG_PRINT("\nresp status 0x%04x", resp_status);
	}

	rsi_ble_app_set_event(RSI_BLE_PROFILE_LIST_EVENT);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_dual_role
 * @brief      Tests the BLE GAP peripheral role.
 * @param[in]  none
 * @return    none.
 * @section description
 * This function is used to test the BLE peripheral role and simple GAP API's.
 */
int32_t rsi_ble_dual_role(void) {
	int32_t status = 0;
	uint8_t adv_data[31] = { 0 };
	uint8_t scan_data[31] = { 0 };
	int8_t adv_data_length = 0;
	int8_t scan_data_length = 0;

	//! initialize the event map
	rsi_ble_app_init_events();

	rsi_ble_add_simple_chat_serv1();
	rsi_ble_add_simple_chat_serv2();

	//! registering the GAP callback functions
	rsi_ble_gap_register_callbacks(
			rsi_ble_on_adv_report_event,
			rsi_ble_on_connect_event,
			rsi_ble_on_disconnect_event,
			NULL,
			rsi_ble_phy_update_complete_event,
			rsi_ble_data_length_change_event,
			rsi_ble_on_enhance_conn_status_event,
			NULL,
			rsi_ble_on_conn_update_complete_event);

	rsi_ble_gap_extended_register_callbacks(
			rsi_ble_simple_peripheral_on_remote_features_event,
			NULL);

	rsi_ble_gatt_register_callbacks(ble_on_profiles_list_resp, 
            rsi_ble_profile,
			rsi_ble_char_services,
			NULL,
			NULL,
			NULL,
			NULL,
			rsi_ble_on_gatt_write_event,
			NULL,
			NULL,
			rsi_ble_on_read_req_event,
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
			NULL,
			NULL
            );

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
			rsi_ble_on_le_security_keys_event);

	//! get the local device address(MAC address).
	status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
	if (status != RSI_SUCCESS) {
		return status;
	}

	//! set the local device name
	status = rsi_bt_set_local_name(RSI_BLE_LOCAL_NAME);
	if (status != RSI_SUCCESS) {
		return status;
	}

	//! get the local device name
	status = rsi_bt_get_local_name(&rsi_app_resp_get_local_name);
	if (status != RSI_SUCCESS) {
		return status;
	}

	//flags
	scan_data[scan_data_length] = 2;
	scan_data_length += 1;

	scan_data[scan_data_length] = 1;
	scan_data_length += 1;

	scan_data[scan_data_length] = 6;
	scan_data_length += 1;

	//Name
	scan_data[scan_data_length] = strlen(RSI_ADV_DATA_NAME) + 1;
	scan_data_length += 1;

	scan_data[scan_data_length] = 9;
	scan_data_length += 1;

	strcpy((char *) &scan_data[scan_data_length], RSI_ADV_DATA_NAME);
	scan_data_length += strlen(RSI_ADV_DATA_NAME);

	//Manufacturing data
	scan_data[scan_data_length] = sizeof(manufacturing_data) + 1;
	scan_data_length += 1;

	scan_data[scan_data_length] = 0xFF;
	scan_data_length += 1;

	memcpy((char *) &scan_data[scan_data_length], manufacturing_data,
			sizeof(manufacturing_data));
	scan_data_length += sizeof(manufacturing_data);

	//! 128bit UUID in advertise data
	adv_data[adv_data_length] = 16 + 1; //strlen (RSI_SCAN_RESP_128BIT_UUID) + 1;
	adv_data_length += 1;

	adv_data[adv_data_length] = 0x07;
	adv_data_length += 1;

	memcpy((char *) &adv_data[adv_data_length], &adv_128bit_uuid, 16);

	adv_data_length = sizeof(adv_data);

	//!set scan response data
	rsi_ble_set_scan_response_data(scan_data, scan_data_length);

	//! set advertise data
	rsi_ble_set_advertise_data(adv_data, adv_data_length);


	//! start the advertising
	status = rsi_ble_start_advertising();
	if (status != RSI_SUCCESS) {
		return status;
	}
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

uint8_t read_data1[500] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
			14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
			31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
			48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
			65, 66, 67, 68, 69, 70, 71, 72, 72, 74, 75, 76, 77, 78, 79, 80, 81,
			82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
			99, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
			18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
			35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
			69, 70, 71, 72, 72, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
			86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 0, 1, 2, 3,
			4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
			22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
			39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
			56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
			72, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
			90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 0, 1, 2, 3, 4, 5, 6, 7, 8,
			9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
			26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
			43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
			60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 72, 74, 75, 76,
			77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93,
			94, 95, 96, 97, 98, 99, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
			47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
			64, 65, 66, 67, 68, 69, 70, 71, 72, 72, 74, 75, 76, 77, 78, 79, 80,
			81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
			98, 99 };
	uint8_t data[20] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 11, 22, 33, 44, 55, 66,
			77, 88, 99, 00 };
	uint8_t data1[2] = { 1, 0 };

int32_t rsi_ble_app_task() {
	int32_t status = 0;
	int32_t temp_event_map = 0;
	//uuid_t service_uuid = { 0 };
	uint8_t ix = 0, i;


	//! checking for received events
	temp_event_map = rsi_ble_app_get_event();
	if (temp_event_map == RSI_FAILURE) {
		//! if events are not received loop will be continued.
		return 0;
	}

	//! if any event is received, it will be served.
	switch (temp_event_map) {
	case RSI_BLE_ADV_REPORT_EVENT: {
		//! clear the advertise report event.
		rsi_ble_app_clear_event(RSI_BLE_ADV_REPORT_EVENT);
	}
	break;

	case RSI_BLE_CONN_EVENT: {
		LOG_PRINT("\n Device connected \n ");
		LOG_PRINT("\n Number of devices connected:%d \n", num_of_conn_slaves);
		conn_req_pending = 0;
		rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);
	}
	break;

	case RSI_BLE_READ_REQ_EVENT: {
		//! event invokes when write/notification events received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_READ_REQ_EVENT);

		if (req_type == 1) {
			rsi_ble_gatt_read_response((uint8_t *) remote_dev_address, 1,
					app_ble_read_event.handle, offset,
					(sizeof(read_data1) - offset), &(read_data1[offset]));
			offset = 0;
		} else {

			rsi_ble_gatt_read_response((uint8_t *) remote_dev_address, 0,
					app_ble_read_event.handle, 0, (sizeof(read_data1)),
					read_data1);
		}
	}
	break;
	case RSI_BLE_DISCONN_EVENT: {
		LOG_PRINT("\n Device disconnected\n ");
#if ENABLE_POWER_SAVE
		//! initiating power save in BLE mode
		status = rsi_bt_power_save_profile(0,0);
		if(status != RSI_SUCCESS)
		{
			return status;
		}
#endif				
		status = rsi_ble_start_advertising();
#if ENABLE_POWER_SAVE
		//! initiating power save in BLE mode
		status = rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE);
		if(status != RSI_SUCCESS)
		{
			return status;
		}
#endif				
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);
	}
	break;

	case RSI_BLE_SCAN_RESTART_EVENT: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SCAN_RESTART_EVENT);
	}
	break;

	case RSI_BLE_RECEIVE_REMOTE_FEATURES: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);

		if (remote_dev_feature.remote_features[0] & 0x20) {
			status = rsi_ble_set_data_len(remote_dev_feature.dev_addr, TX_LEN, TX_TIME);

			if (status != RSI_SUCCESS) {
				return status;
			}

		} else if (remote_dev_feature.remote_features[1] & 0x01) {
			status = rsi_ble_setphy((int8_t *)remote_dev_feature.dev_addr, TX_PHY_RATE,
					RX_PHY_RATE, CODDED_PHY_RATE);
			if (status != RSI_SUCCESS) {
				return status;
			}
		} else {
			//! initiating the SMP pairing process
			status = rsi_ble_smp_pair_request(remote_dev_address,
					RSI_BLE_SMP_IO_CAPABILITY, MITM_ENABLE);
		}
	}
	break;

	case RSI_APP_EVENT_DATA_LENGTH_CHANGE: {
		//! clear the disconnected event.
		rsi_ble_app_clear_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);

		if (remote_dev_feature.remote_features[1] & 0x01) {
			status = rsi_ble_setphy((int8_t *)remote_dev_feature.dev_addr, TX_PHY_RATE,
					RX_PHY_RATE, CODDED_PHY_RATE);
			if (status != RSI_SUCCESS) {
				return status;
			}
		} else {
			//! initiating the SMP pairing process
			status = rsi_ble_smp_pair_request(remote_dev_feature.dev_addr,
					RSI_BLE_SMP_IO_CAPABILITY, MITM_ENABLE);
		}
	}
	break;

	case RSI_APP_EVENT_PHY_UPDATE_COMPLETE: {
		//! phy update complete event

		//! clear the phy updare complete event.
		rsi_ble_app_clear_event(RSI_APP_EVENT_PHY_UPDATE_COMPLETE);

		//! initiating the SMP pairing process
		status = rsi_ble_smp_pair_request(rsi_app_phy_update_complete.dev_addr,
				RSI_BLE_SMP_IO_CAPABILITY, MITM_ENABLE);
	}
	break;
	case RSI_BLE_SMP_REQ_EVENT: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SMP_REQ_EVENT);

		if (first_connect == 0) {
			//! initiating the SMP pairing process
			status = rsi_ble_smp_pair_request(remote_dev_address,
					RSI_BLE_SMP_IO_CAPABILITY, MITM_ENABLE);
		} else {
			status = rsi_ble_start_encryption(remote_dev_address,
					glbl_enc_enabled.localediv, glbl_enc_enabled.localrand,
					glbl_enc_enabled.localltk);
		}
	}
	break;

	case RSI_BLE_SMP_RESP_EVENT: {

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SMP_RESP_EVENT);

		//! initiating the SMP pairing process
		status = rsi_ble_smp_pair_response(remote_dev_address,
				RSI_BLE_SMP_IO_CAPABILITY, MITM_ENABLE);
	}
	break;

	case RSI_BLE_SMP_PASSKEY_EVENT: {

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SMP_PASSKEY_EVENT);

		//! initiating the SMP pairing process
		status = rsi_ble_smp_passkey(remote_dev_address, RSI_BLE_APP_SMP_PASSKEY);
	}
	break;
	case RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
	}
	break;
	case RSI_BLE_LTK_REQ_EVENT: {
		//! event invokes when disconnection was completed

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_LTK_REQ_EVENT);

		rsi_6byte_dev_address_to_ascii((int8_t *)str_remote_address,
				temp_le_ltk_req.dev_addr);

		if ((temp_le_ltk_req.localediv == glbl_enc_enabled.localediv)
				&& !((memcmp(temp_le_ltk_req.localrand,
						glbl_enc_enabled.localrand, 8)))) {
			LOG_PRINT("\n positive reply\n");
			//! give le ltk req reply cmd with positive reply
			status = rsi_ble_ltk_req_reply(temp_le_ltk_req.dev_addr, 1,
					glbl_enc_enabled.localltk);
		} else {
			LOG_PRINT("\n negative reply\n");
			//! give le ltk req reply cmd with negative reply
			status = rsi_ble_ltk_req_reply(temp_le_ltk_req.dev_addr, 0, NULL);
		}
	}
	break;

	case RSI_BLE_SC_PASSKEY_EVENT: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SC_PASSKEY_EVENT);
		rsi_ble_smp_passkey(remote_dev_address, passkey);
	}
	break;

	case RSI_BLE_SECURITY_KEYS_EVENT: {
		//! event invokes when security keys are received
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SECURITY_KEYS_EVENT);
#if 0
		rsi_6byte_dev_address_to_ascii(str_remote_address,
				temp_le_sec_keys.dev_addr);
		resolve_key.remote_dev_addr_type = dev_address_type;

		resolve_key.remote_ediv = temp_le_sec_keys.remote_ediv;
		memcpy(resolve_key.remote_rand, temp_le_sec_keys.remote_rand,
				sizeof(temp_le_sec_keys.remote_rand));
		memcpy(resolve_key.remote_ltk, temp_le_sec_keys.remote_ltk,
				sizeof(temp_le_sec_keys.remote_ltk));

		rsi_6byte_dev_address_to_ascii(resolve_key.remote_dev_addr,
				temp_le_sec_keys.dev_addr);
		memcpy(resolve_key.peer_irk, temp_le_sec_keys.remote_irk,
				sizeof(temp_le_sec_keys.remote_irk));
		memcpy(resolve_key.local_irk, temp_le_sec_keys.local_irk,
				sizeof(temp_le_sec_keys.local_irk));

		resolve_key.Identity_addr_type = temp_le_sec_keys.Identity_addr_type;
		rsi_6byte_dev_address_to_ascii(resolve_key.Identity_addr,
				temp_le_sec_keys.Identity_addr);

		update_resolvlist((rsi_ble_resolvlist_group_t *) &resolvlist,
				&resolve_key);

		//get resolvlist size
		status = rsi_ble_get_resolving_list_size(&rsi_app_resp_resolvlist_size);
		if (status != RSI_SUCCESS) {
			return status;
		}

		//set address resolution enable and resolvable private address timeout
		status = rsi_ble_set_addr_resolution_enable(
				RSI_BLE_DEV_ADDR_RESOLUTION_ENABLE,
				RSI_BLE_SET_RESOLVABLE_PRIV_ADDR_TOUT);
		if (status != RSI_SUCCESS) {
			return status;
		}

		//set privacy mode
		status = rsi_ble_set_privacy_mode(resolve_key.Identity_addr_type,
				temp_le_sec_keys.Identity_addr, RSI_BLE_PRIVACY_MODE);
		if (status != RSI_SUCCESS) {
			return status;
		}
		rsi_ble_update_directed_address(temp_le_sec_keys.Identity_addr);
#endif
	}
	break;

	case RSI_BLE_SMP_FAILED_EVENT: {
		//! initiate SMP protocol as a Master

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SMP_FAILED_EVENT);
	}

	break;

	case RSI_BLE_ENCRYPT_STARTED_EVENT: {
		//! start the encrypt event

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_ENCRYPT_STARTED_EVENT);

		status = rsi_ble_conn_params_update((int8_t *)glbl_enc_enabled.dev_addr, 24, 36, 0, 400);
		if (status == RSI_SUCCESS) {
			rsi_conn_update = 1;
		}
	}
	break;

	case RSI_BLE_CONN_UPDATE_EVENT: {

		rsi_ble_app_clear_event(RSI_BLE_CONN_UPDATE_EVENT);

		if (rsi_conn_update == 1) {
#if 0
			service_uuid.size = 2;
			service_uuid.val.val16 = RSI_BLE_NEW_CLIENT_SERVICE_UUID;
			retry1: status = rsi_ble_get_profile(remote_dev_address,
					service_uuid, &rsi_ble_service);
			if (status != 0)
				goto retry1;
#endif

			memset(&prof_list, 0, sizeof(rsi_ble_resp_profiles_list_t));

			get_prof: status = rsi_ble_get_profiles(remote_dev_address,
					0x0001, 0xFFFF, &prof_list);
			if (status != 0) {
				goto get_prof;
			}
			rsi_conn_update = 0;
		}

		if (rsi_conn_update == 2) {
			rsi_ble_disconnect((int8_t *)remote_dev_address);
			rsi_conn_update = 0;

		}
	}
	break;

	case RSI_BLE_GATT_PROFILE_RESP_EVENT: {
		//! event invokes when get profile response received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_PROFILE_RESP_EVENT);

		//! get characteristics of the immediate alert servcie
		//rsi_6byte_dev_address_to_ascii ((int8_t *)str_remote_address, (uint8_t *)conn_event_to_app->dev_addr);
		rsi_ble_get_char_services(remote_dev_address,
				*(uint16_t *) rsi_ble_service.start_handle,
				*(uint16_t *) rsi_ble_service.end_handle, &char_servs);
	}
	break;

	case RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT: {
		//! event invokes when get characteristics of the service response received
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);

		//! verifying the immediate alert characteristic
		for (ix = 0; ix <= char_servs.num_of_services; ix++) {
			if (char_servs.char_services[ix].char_data.char_property
					& RSI_BLE_ATT_PROPERTY_NOTIFY) {
				rsi_ble_write_att1_val_hndl =
						char_servs.char_services[ix].char_data.char_handle;
				rsi_ble_write_att2_val_hndl = rsi_ble_write_att1_val_hndl + 1;
			}
		}

		//!CCCD indication to remote device
		for (i = 0; i < char_servs.num_of_services; i++) {
			LOG_PRINT("Value of property of services are  %x",
					char_servs.char_services[i].char_data.char_property);
			if (char_servs.char_services[i].char_data.char_property == 0x10
					|| char_servs.char_services[i].char_data.char_property
					== 0x20) {
				notification_handle =
						char_servs.char_services[i].char_data.char_handle + 1;
				rsi_ble_set_att_value(remote_dev_address,
						notification_handle, 2, data1);
			}
		}
	}
	break;

	case RSI_BLE_GATT_WRITE_EVENT:
	{

		//! event invokes when write/notification events receive
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_WRITE_EVENT);
		status = rsi_ble_set_att_value(app_ble_write_event.dev_addr,
				rsi_ble_att2_val_hndl, 2, data1);

		rsi_ble_set_local_att_value(0x0009, 2, data1);

		while (i < 100) {
			//! prepare the data to set as local attribute value.
			memset(data, 0, sizeof(data));
			data[0] = i;
			//! set the local attribute value.
			rsi_ble_set_local_att_value(rsi_ble_att2_val_hndl,
					RSI_BLE_MAX_DATA_LEN, (uint8_t *) data);
			i++;
		}

		i = 0;
		while (i < 100) {
			//! get the RSSI value with connected remote device
			status = rsi_bt_get_rssi((int8_t *) app_ble_write_event.dev_addr,
					&rsi_app_resp_rssi);
			i++;
		}

		//Need to add CCCD stuff

		status = rsi_ble_conn_params_update((int8_t *)app_ble_write_event.dev_addr, 852, 852, 0,
				400);
		if (status == RSI_SUCCESS) {
			rsi_conn_update = 2;
		}
	}
	break;
	case RSI_BLE_PROFILE_LIST_EVENT:
	{
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_PROFILE_LIST_EVENT);

		if (!profiles_list_done) {
			for (i = number_of_profiles;
					i < prof_list.number_of_profiles; i++) {
				memcpy(&list_of_profs[i], &prof_list.profile_desc[i],
						sizeof(profile_descriptors_t));
			}

			number_of_profiles += prof_list.number_of_profiles;
			end_handle =
					(prof_list.profile_desc[number_of_profiles - 1].end_handle[0])
					| (prof_list.profile_desc[number_of_profiles
											  - 1].end_handle[1] << 8);

			start_handle = end_handle + 1;
			status = rsi_ble_get_profiles(remote_dev_address,
					start_handle, 0xFFFF, &prof_list);
		} else {

		}
	}
	break;

	}
	return 0;
}
#endif


