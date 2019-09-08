/**
 * @file    rsi_ble_dual_role_DEMO_20.c
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

/*=======================================================================*/
//   ! INCLUDES
/*=======================================================================*/
#include <rsi_common_app.h>
#if BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX
//! BT include file to refer BT APIs
#include <rsi_ble_apis.h>
#include <rsi_ble_config.h>
#include <rsi_common_apis.h>
#include <rsi_bt_common.h>
#include <stdio.h>
#include <string.h>
#include "rsi_driver.h"

/*=======================================================================*/
//   ! MACROS
/*=======================================================================*/

/*=======================================================================*/
//   ! GLOBAL VARIABLES
/*=======================================================================*/
uint8_t          conn_dev_addr[18] = { 0 }, ix, conn_done, conn_master_done, conn_update_done, conn_update_master_done;
uint16_t         mtu_size;
uint8_t          str_conn_device[18] = { 0 };
uint16_t         num_of_conn_slaves = 0, count;
uint8_t          num_of_conn_masters = 0;
uint16_t         conn_req_pending = 0;
static uint8_t   device_found = 0;
static uint8_t   remote_name[31];
static uint8_t   remote_addr_type = 0;
static uint8_t   remote_dev_addr[18] = { 0 };
static uint8_t   remote_dev_addr_conn_update[18] = { 0 };
static uint8_t   remote_dev_addr_conn[18] = { 0 };
static uint8_t   remote_dev_bd_addr[6] = { 0 };
#if(CONNECT_OPTION == CONN_BY_NAME)
static uint8_t   remote_dev_addr_conn1[18] = { 0 };
static uint8_t   remote_dev_addr_conn2[18] = { 0 };
static uint8_t   remote_dev_addr_conn3[18] = { 0 };
static uint8_t   remote_dev_addr_conn4[18] = { 0 };
static uint8_t   remote_dev_addr_conn5[18] = { 0 };
static uint8_t   remote_dev_addr_conn6[18] = { 0 };
static uint8_t   remote_name_conn1[31];
static uint8_t   remote_name_conn2[31];
static uint8_t   remote_name_conn3[31];
static uint8_t   remote_name_conn4[31];
static uint8_t   remote_name_conn5[31];
static uint8_t   remote_name_conn6[31];
#endif

//! Contains master device address
static uint8_t   remote_dev_addr_conn7[18] = { 0 };
static uint8_t   remote_dev_addr_conn8[18] = { 0 };

static uint16_t  rsi_app_no_of_adv_reports_rcvd = 0;
static uint16_t  rsi_scan_in_progress;
static uint32_t  ble_app_event_map;
static uint32_t  ble_app_event_mask;
static uint8_t   remote_name[31];
static uint16_t  rsi_ble_att1_val_hndl, rsi_ble_read_att1_val_hndl;

//! Structure variables
rsi_ble_t                                att_list;
rsi_ble_event_conn_update_t              conn_update_resp = {0};
static rsi_ble_event_adv_report_t        rsi_app_adv_reports_to_app[NO_OF_ADV_REPORTS];
static rsi_ble_event_remote_features_t 	 remote_dev_feature;
static rsi_ble_event_conn_status_t       conn_event_to_app[8];
static rsi_ble_event_write_t             app_ble_write_event;
static rsi_ble_read_req_t                app_ble_read_event;
static rsi_ble_event_mtu_t               app_ble_mtu_event;
static profile_descriptors_t             rsi_ble_service;
static rsi_ble_resp_char_services_t      char_servs;

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
int32_t rsi_conn_update_request();


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
		LOG_PRINT ("no data memory for att rec values");
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
	new_att.config_bitmap = auth_read;
	memcpy(&new_att.att_uuid, &att_type_uuid, sizeof(uuid_t));
	new_att.property = val_prop;

	//! preparing the attribute value
	new_att.data_len = RSI_MIN(sizeof(new_att.data), data_len);
	if (data != NULL)
		memcpy(new_att.data, data, new_att.data_len);

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
 * @fn         rsi_ble_add_simple_chat_serv
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
	uint8_t data[100] = { 1, 0 };

	//! adding new service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_NEW_SERVICE_UUID;
	rsi_ble_add_service(new_uuid, &new_serv_resp);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
	rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 1,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY
					| RSI_BLE_ATT_PROPERTY_WRITE,
			new_serv_resp.start_handle + 2, new_uuid);
	//! adding characteristic value attribute to the service

	rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
	rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
			new_serv_resp.start_handle + 2, new_uuid,
			RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY, data,
			sizeof(data), 0);

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
 * @fn         rsi_ble_simple_peripheral_on_remote_features_event
 * @brief      invoked when LE remote features event is received.
 * @param[in] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_peripheral_on_remote_features_event(
		rsi_ble_event_remote_features_t *rsi_ble_event_remote_features) {
	LOG_PRINT("Feature received is %d",
			rsi_ble_event_remote_features->remote_features);
	memcpy(&remote_dev_feature, rsi_ble_event_remote_features,
			sizeof(rsi_ble_event_remote_features_t));
	rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
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
void rsi_ble_simple_central_on_adv_report_event(
		rsi_ble_event_adv_report_t *adv_report) {
	if (device_found == 1) {
		return;
	}

	memcpy(
			&rsi_app_adv_reports_to_app[(rsi_app_no_of_adv_reports_rcvd)
					% (NO_OF_ADV_REPORTS)], adv_report,
			sizeof(rsi_ble_event_adv_report_t));

	rsi_app_no_of_adv_reports_rcvd++;

	if (rsi_app_no_of_adv_reports_rcvd == NO_OF_ADV_REPORTS) {
		rsi_app_no_of_adv_reports_rcvd = 0;
		rsi_ble_app_set_event(RSI_BLE_SCAN_RESTART_EVENT);
	}

	BT_LE_ADPacketExtract(remote_name, adv_report->adv_data,
			adv_report->adv_data_len);

	remote_addr_type = adv_report->dev_addr_type;
	rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
			(uint8_t *) adv_report->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *) adv_report->dev_addr, 6);
	memcpy(remote_dev_addr_conn, remote_dev_addr, sizeof(remote_dev_addr));

#if(CONNECT_OPTION == CONN_BY_NAME)
	if ((device_found == 0)
			&& ((strcmp((const char *) remote_name, RSI_REMOTE_DEVICE_NAME1))
					== 0)) {
		memcpy(remote_name_conn1, remote_name, 31);
		memcpy(remote_dev_addr_conn1, remote_dev_addr, 18);
//		if (conn_done == 0) 
		{
			device_found = 1;
			conn_done = 1;
			rsi_ble_app_set_event(RSI_APP_EVENT_ADV_REPORT);
			return;
		}
	}

	if ((device_found == 0)
			&& ((strcmp((const char *) remote_name, RSI_REMOTE_DEVICE_NAME2))
					== 0)) {
		memcpy(remote_name_conn2, remote_name, 31);
		memcpy(remote_dev_addr_conn2, remote_dev_addr, 18);
//		if (conn_done == 1) 
		{
			device_found = 1;
			conn_done = 2;
			rsi_ble_app_set_event(RSI_APP_EVENT_ADV_REPORT);
			return;
		}
	}

	if ((device_found == 0)
			&& ((strcmp((const char *) remote_name, RSI_REMOTE_DEVICE_NAME3))
					== 0)) {
		memcpy(remote_name_conn3, remote_name, 31);
		memcpy(remote_dev_addr_conn3, remote_dev_addr, 18);
//		if (conn_done == 2) 
		{
			device_found = 1;
			conn_done = 3;
			rsi_ble_app_set_event(RSI_APP_EVENT_ADV_REPORT);
			return;
		}
	}

	if ((device_found == 0)
			&& ((strcmp((const char *) remote_name, RSI_REMOTE_DEVICE_NAME4))
					== 0)) {
		memcpy(remote_name_conn4, remote_name, 31);
		memcpy(remote_dev_addr_conn4, remote_dev_addr, 18);
//		if (conn_done == 3) 
		{
			device_found = 1;
			conn_done = 4;
			rsi_ble_app_set_event(RSI_APP_EVENT_ADV_REPORT);
			return;
		}
	}

	if ((device_found == 0)
			&& ((strcmp((const char *) remote_name, RSI_REMOTE_DEVICE_NAME5))
					== 0)) {
		memcpy(remote_name_conn5, remote_name, 31);
		memcpy(remote_dev_addr_conn5, remote_dev_addr, 18);
//		if (conn_done == 4) 
		{
			device_found = 1;
			conn_done = 5;
			rsi_ble_app_set_event(RSI_APP_EVENT_ADV_REPORT);
			return;
		}
	}

	if ((device_found == 0)
			&& ((strcmp((const char *) remote_name, RSI_REMOTE_DEVICE_NAME6))
					== 0)) {
		memcpy(remote_name_conn6, remote_name, 31);
		memcpy(remote_dev_addr_conn6, remote_dev_addr, 18);
//		if (conn_done == 5) 
		{
			device_found = 1;
			conn_done = 6;
			rsi_ble_app_set_event(RSI_APP_EVENT_ADV_REPORT);
			return;
		}
	}
#else
	if (!strcmp (RSI_BLE_DEV_1_ADDR, (char *)remote_dev_addr))
		conn_done = 1;
	else if (!strcmp (RSI_BLE_DEV_2_ADDR, (char *)remote_dev_addr))
		conn_done = 2;
	else if (!strcmp (RSI_BLE_DEV_3_ADDR, (char *)remote_dev_addr))
		conn_done = 3;
	else if (!strcmp (RSI_BLE_DEV_4_ADDR, (char *)remote_dev_addr))
		conn_done = 4;
	else if (!strcmp (RSI_BLE_DEV_5_ADDR, (char *)remote_dev_addr))
		conn_done = 5;
	else if (!strcmp (RSI_BLE_DEV_6_ADDR, (char *)remote_dev_addr))
		conn_done = 6;
	else
		return;

	device_found=1;
	//memcpy (conn_dev_addr, remote_dev_addr, sizeof (remote_dev_addr));
	memcpy(remote_dev_addr_conn,remote_dev_addr,18);
	rsi_ble_app_set_event (RSI_APP_EVENT_ADV_REPORT);

#endif
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
	conn_req_pending = 0;
	if (resp_conn->status != 0)
	{
		LOG_PRINT("\n On connect event status report : %d", resp_conn->status);
		//! Restarting scan
		rsi_ble_app_set_event(RSI_BLE_SCAN_RESTART_EVENT);
		return;
	}

	rsi_6byte_dev_address_to_ascii((int8_t *) str_conn_device,
			(uint8_t *) resp_conn->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *) resp_conn->dev_addr, 6);
	LOG_PRINT("connected device address : %s",str_conn_device);
#if (CONNECT_OPTION == CONN_BY_ADDR)

	if(!strcmp((char *)str_conn_device, RSI_BLE_DEV_1_ADDR))
	{
		conn_event_to_app[0].dev_addr_type = resp_conn->dev_addr_type;
		memcpy (conn_event_to_app[0].dev_addr, resp_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[0].status = resp_conn->status;
		conn_done = 1;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_2_ADDR))
	{
		conn_event_to_app[1].dev_addr_type = resp_conn->dev_addr_type;
		memcpy (conn_event_to_app[1].dev_addr, resp_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[1].status = resp_conn->status;
		conn_done = 2;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_3_ADDR))
	{
		conn_event_to_app[2].dev_addr_type = resp_conn->dev_addr_type;
		memcpy (conn_event_to_app[2].dev_addr, resp_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[2].status = resp_conn->status;
		conn_done = 3;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_4_ADDR))
	{
		conn_event_to_app[3].dev_addr_type = resp_conn->dev_addr_type;
		memcpy (conn_event_to_app[3].dev_addr, resp_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[3].status = resp_conn->status;
		conn_done = 4;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_5_ADDR))
	{
		conn_event_to_app[4].dev_addr_type = resp_conn->dev_addr_type;
		memcpy (conn_event_to_app[4].dev_addr, resp_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[4].status = resp_conn->status;
		conn_done = 5;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_6_ADDR))
	{
		conn_event_to_app[5].dev_addr_type = resp_conn->dev_addr_type;
		memcpy (conn_event_to_app[5].dev_addr, resp_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[5].status = resp_conn->status;
		conn_done = 6;
		num_of_conn_slaves++;
	}
#else
	if (strcmp((const char *) remote_dev_addr_conn1,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[0].dev_addr_type = resp_conn->dev_addr_type;
		memcpy(conn_event_to_app[0].dev_addr, resp_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[0].status = resp_conn->status;
		num_of_conn_slaves++;
	} else if (strcmp((const char *) remote_dev_addr_conn2,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[1].dev_addr_type = resp_conn->dev_addr_type;
		memcpy(conn_event_to_app[1].dev_addr, resp_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[1].status = resp_conn->status;
		num_of_conn_slaves++;
	} else if (strcmp((const char *) remote_dev_addr_conn3,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[2].dev_addr_type = resp_conn->dev_addr_type;
		memcpy(conn_event_to_app[2].dev_addr, resp_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[2].status = resp_conn->status;
		num_of_conn_slaves++;
	} else if (strcmp((const char *) remote_dev_addr_conn4,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[3].dev_addr_type = resp_conn->dev_addr_type;
		memcpy(conn_event_to_app[3].dev_addr, resp_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[3].status = resp_conn->status;
		num_of_conn_slaves++;
	} else if (strcmp((const char *) remote_dev_addr_conn5,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[4].dev_addr_type = resp_conn->dev_addr_type;
		memcpy(conn_event_to_app[4].dev_addr, resp_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[4].status = resp_conn->status;
		num_of_conn_slaves++;
	} else if ((strcmp((const char *) remote_dev_addr_conn6,
			(const char *) str_conn_device)) == 0) {
		conn_event_to_app[5].dev_addr_type = resp_conn->dev_addr_type;
		memcpy(conn_event_to_app[5].dev_addr, resp_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[5].status = resp_conn->status;
		num_of_conn_slaves++;
	}
#endif

	else if (!(conn_master_done & BIT(0))) {
		conn_event_to_app[6].dev_addr_type = resp_conn->dev_addr_type;
		memcpy(conn_event_to_app[6].dev_addr, resp_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[6].status = resp_conn->status;
		memcpy(remote_dev_addr_conn7, str_conn_device, 18);
		conn_master_done |= BIT(0);
		num_of_conn_masters++;
	} else if (!(conn_master_done & BIT(1))) {
		conn_event_to_app[7].dev_addr_type = resp_conn->dev_addr_type;
		memcpy(conn_event_to_app[7].dev_addr, resp_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[7].status = resp_conn->status;
		memcpy(remote_dev_addr_conn7, str_conn_device, 18);
		conn_master_done |= BIT(1);
		num_of_conn_masters++;
	}
	LOG_PRINT("\n Device connected \n ");

	rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);

}

/*==============================================*/
/**
 * @fn         ble_on_conn_update_complete_event
 * @brief      invoked when connection update complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
static void ble_on_conn_update_complete_event(
		rsi_ble_event_conn_update_t *resp_conn_update, uint16_t resp_status) {
	rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr_conn_update,
			(uint8_t *) resp_conn_update->dev_addr);

	if (resp_status != 0) {
		LOG_PRINT("\n RSI_BLE_CONN_UPDATE_EVENT FAILED : %d \n", resp_status);
		//! Again request for connection update params
		return;
	} else {


		memcpy(conn_update_resp.dev_addr, resp_conn_update->dev_addr, 6);
		memcpy((uint8_t *)&conn_update_resp.conn_interval, (uint8_t *)&resp_conn_update->conn_interval, 2);
		memcpy((uint8_t *)&conn_update_resp.conn_latency, (uint8_t *)&resp_conn_update->conn_latency, 2);
		memcpy((uint8_t *)&conn_update_resp.timeout, (uint8_t *)&resp_conn_update->timeout, 2);
		LOG_PRINT("\nconn updated device address : %s\nconn_interval:%d\ntimeout:%d",str_conn_device,resp_conn_update->conn_interval, resp_conn_update->timeout);
	}

	rsi_ble_app_set_event(RSI_BLE_CONN_UPDATE_COMPLETE_EVENT);
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

	rsi_6byte_dev_address_to_ascii((int8_t *) str_conn_device,
				(uint8_t *) resp_disconnect->dev_addr);

#if(CONNECT_OPTION == CONN_BY_ADDR)

	if(!strcmp((char *)str_conn_device, RSI_BLE_DEV_1_ADDR))
	{
		memset(&conn_event_to_app[0], 0, sizeof(rsi_ble_event_conn_status_t));
		num_of_conn_slaves--;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_2_ADDR))
	{
		memset(&conn_event_to_app[1], 0, sizeof(rsi_ble_event_conn_status_t));
		num_of_conn_slaves--;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_3_ADDR))
	{
		memset(&conn_event_to_app[2], 0, sizeof(rsi_ble_event_conn_status_t));
		num_of_conn_slaves--;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_4_ADDR))
	{
		memset(&conn_event_to_app[3], 0, sizeof(rsi_ble_event_conn_status_t));
		num_of_conn_slaves--;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_5_ADDR))
	{
		memset(&conn_event_to_app[4], 0, sizeof(rsi_ble_event_conn_status_t));
		num_of_conn_slaves--;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_6_ADDR))
	{
		memset(&conn_event_to_app[5], 0, sizeof(rsi_ble_event_conn_status_t));
		num_of_conn_slaves--;
	}

#else
	if ((strcmp((const char *) remote_dev_addr_conn1,
							(const char *) str_conn_device)) == 0) {
		memset(&conn_event_to_app[0], 0, sizeof(rsi_ble_event_conn_status_t));
		memset(remote_dev_addr_conn1, 0, 18);
		num_of_conn_slaves--;
	} else if ((strcmp((const char *) remote_dev_addr_conn2,
							(const char *) str_conn_device)) == 0) {
		memset(&conn_event_to_app[1], 0, sizeof(rsi_ble_event_conn_status_t));
		memset(remote_dev_addr_conn2, 0, 18);
		num_of_conn_slaves--;
	} else if ((strcmp((const char *) remote_dev_addr_conn3,
							(const char *) str_conn_device)) == 0) {
		memset(&conn_event_to_app[2], 0, sizeof(rsi_ble_event_conn_status_t));
		memset(remote_dev_addr_conn3, 0, 18);
		num_of_conn_slaves--;
	} else if ((strcmp((const char *) remote_dev_addr_conn4,
							(const char *) str_conn_device)) == 0) {
		memset(&conn_event_to_app[3], 0, sizeof(rsi_ble_event_conn_status_t));
		memset(remote_dev_addr_conn4, 0, 18);
		num_of_conn_slaves--;
	} else if ((strcmp((const char *) remote_dev_addr_conn5,
							(const char *) str_conn_device)) == 0) {
		memset(&conn_event_to_app[4], 0, sizeof(rsi_ble_event_conn_status_t));
		memset(remote_dev_addr_conn5, 0, 18);
		num_of_conn_slaves--;
	} else if ((strcmp((const char *) remote_dev_addr_conn6,
							(const char *) str_conn_device)) == 0) {
		memset(&conn_event_to_app[5], 0, sizeof(rsi_ble_event_conn_status_t));
		memset(remote_dev_addr_conn6, 0, 18);
		num_of_conn_slaves--;
	}
#endif
 else if ((strcmp((const char *) remote_dev_addr_conn7,
						(const char *) str_conn_device)) == 0) {
		memset(&conn_event_to_app[6], 0, sizeof(rsi_ble_event_conn_status_t));
		memset(remote_dev_addr_conn7, 0, 18);
		num_of_conn_masters--;
		conn_master_done &= ~BIT(0);
	} else if ((strcmp((const char *) remote_dev_addr_conn8,
						(const char *) str_conn_device)) == 0) {
		memset(&conn_event_to_app[7], 0, sizeof(rsi_ble_event_conn_status_t));
		memset(remote_dev_addr_conn8, 0, 18);
		num_of_conn_masters--;
		conn_master_done &= ~BIT(1);
	}

	rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
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
	conn_req_pending = 0;
	if (resp_enh_conn->status != 0)
	{
		LOG_PRINT("\n On enhanced connect event status report : %d", resp_enh_conn->status);
		rsi_ble_app_set_event(RSI_BLE_SCAN_RESTART_EVENT);
		return;
	}
	rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_device, (uint8_t *)resp_enh_conn->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *) resp_enh_conn->dev_addr, 6);
#if(CONNECT_OPTION == CONN_BY_ADDR)
	if(!strcmp((char *)str_conn_device, RSI_BLE_DEV_1_ADDR))
	{
		conn_event_to_app[0].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy (conn_event_to_app[0].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[0].status = resp_enh_conn->status;
		conn_done = 1;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_2_ADDR))
	{
		conn_event_to_app[1].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy (conn_event_to_app[1].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[1].status = resp_enh_conn->status;
		conn_done = 2;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_3_ADDR))
	{
		conn_event_to_app[2].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy (conn_event_to_app[2].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[2].status = resp_enh_conn->status;
		conn_done = 3;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_4_ADDR))
	{
		conn_event_to_app[3].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy (conn_event_to_app[3].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[3].status = resp_enh_conn->status;
		conn_done = 4;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_5_ADDR))
	{
		conn_event_to_app[4].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy (conn_event_to_app[4].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[4].status = resp_enh_conn->status;
		conn_done = 5;
		num_of_conn_slaves++;
	}
	else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_6_ADDR))
	{
		conn_event_to_app[5].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy (conn_event_to_app[5].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
		conn_event_to_app[5].status = resp_enh_conn->status;
		conn_done = 6;
		num_of_conn_slaves++;
	}
#else
	if (strcmp((const char *) remote_dev_addr_conn1,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[0].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy(conn_event_to_app[0].dev_addr, resp_enh_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[0].status = resp_enh_conn->status;
		conn_done = 1;
		num_of_conn_slaves++;
	} else if (strcmp((const char *) remote_dev_addr_conn2,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[1].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy(conn_event_to_app[1].dev_addr, resp_enh_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[1].status = resp_enh_conn->status;
		conn_done = 2;
		num_of_conn_slaves++;
	} else if (strcmp((const char *) remote_dev_addr_conn3,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[2].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy(conn_event_to_app[2].dev_addr, resp_enh_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_done = 3;
		conn_event_to_app[2].status = resp_enh_conn->status;
		num_of_conn_slaves++;
	} else if (strcmp((const char *) remote_dev_addr_conn4,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[3].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy(conn_event_to_app[3].dev_addr, resp_enh_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[3].status = resp_enh_conn->status;
		conn_done = 4;
		num_of_conn_slaves++;
	} else if (strcmp((const char *) remote_dev_addr_conn5,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[4].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy(conn_event_to_app[4].dev_addr, resp_enh_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[4].status = resp_enh_conn->status;
		conn_done = 5;
		num_of_conn_slaves++;
	} else if (strcmp((const char *) remote_dev_addr_conn6,
			(const char *) str_conn_device) == 0) {
		conn_event_to_app[5].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy(conn_event_to_app[5].dev_addr, resp_enh_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		conn_event_to_app[5].status = resp_enh_conn->status;
		conn_done = 6;
		num_of_conn_slaves++;
	}
#endif
	else if (!(conn_master_done & BIT(0))) {
		conn_event_to_app[6].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy(conn_event_to_app[6].dev_addr, resp_enh_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		memcpy(remote_dev_addr_conn7, str_conn_device, 18);
		conn_event_to_app[6].status = resp_enh_conn->status;
		conn_master_done |= 1 << 0;
		num_of_conn_masters++;
		}
		else if (!(conn_master_done & BIT(1))) {
		conn_event_to_app[7].dev_addr_type = resp_enh_conn->dev_addr_type;
		memcpy(conn_event_to_app[7].dev_addr, resp_enh_conn->dev_addr,
		RSI_DEV_ADDR_LEN);
		memcpy(remote_dev_addr_conn8, str_conn_device, 18);
		conn_event_to_app[7].status = resp_enh_conn->status;
		conn_master_done |= 1 << 1;
		num_of_conn_masters++;
	}

	rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
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
	memcpy(&char_servs, rsi_ble_resp_char_services,
			sizeof(rsi_ble_resp_char_services_t));
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
	rsi_ble_app_set_event(RSI_BLE_READ_REQ_EVENT);
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
	rsi_ble_app_set_event(RSI_BLE_MTU_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_dual_role
 * @brief      this is the application of ble GATT client api's test cases.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used at application.
 */
int32_t rsi_ble_dual_role(void) {
	int32_t status = 0;
	uint8_t adv[31] = { 2, 1, 6 };

	rsi_ble_add_simple_chat_serv();

	//! registering the GAP call back functions
	rsi_ble_gap_register_callbacks(rsi_ble_simple_central_on_adv_report_event,
			rsi_ble_on_connect_event, rsi_ble_on_disconnect_event,
			NULL,
			NULL,
			NULL, rsi_ble_on_enhance_conn_status_event,
			NULL, ble_on_conn_update_complete_event);

	//! registering the GATT call back functions
	rsi_ble_gatt_register_callbacks(NULL, rsi_ble_profile,
			rsi_ble_char_services,
			NULL,
			NULL,
			NULL,
			NULL,
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
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
            );

	rsi_ble_gap_extended_register_callbacks(
				rsi_ble_simple_peripheral_on_remote_features_event,
        NULL);

	//!  initializing the application events map
	rsi_ble_app_init_events();

	//! Set local name
	rsi_bt_set_local_name(RSI_BLE_APP_GATT_TEST);

    if (RSI_BLE_MAX_NBR_MASTERS > 0)
	{
	//! prepare advertise data //local/device name
	adv[3] = strlen(RSI_BLE_APP_GATT_TEST) + 1;
	adv[4] = 9;
	strcpy((char *) &adv[5], RSI_BLE_APP_GATT_TEST);

	//! set advertise data
	rsi_ble_set_advertise_data(adv, strlen(RSI_BLE_APP_GATT_TEST) + 5);

	//! set device in advertising mode.
	rsi_ble_start_advertising();
	}
	
    if (RSI_BLE_MAX_NBR_SLAVES > 0)
	{
	//! start scanning
	status = rsi_ble_start_scanning();
	if (status != RSI_SUCCESS) {
		return status;
	}
	}

	return 0;
}

void rsi_ble_app_task(void) {
	int32_t status = 0;
	int32_t event_id;
	int8_t data[20] = { 0 };
	int8_t client_data1[2] = { 1, 0 };
	uuid_t service_uuid = { 0 };

	uint8_t read_data[100] = { 2 };
#ifndef RSI_WITH_OS
	//! Application main loop
	rsi_wireless_driver_task();
#else
	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! Send Feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	// Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! BLE dual role Initialization
	status = rsi_ble_dual_role();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("ble dual role init failed\n");
		return;
	}
while(1)
#endif
{
	//! checking for events list
	event_id = rsi_ble_app_get_event();
	if (event_id == -1) {
		return;
	}

	switch (event_id) {
	case RSI_APP_EVENT_ADV_REPORT: {
		LOG_PRINT("\n Advertise report received \n");
		//! clear the advertise report event.
				rsi_ble_app_clear_event(RSI_APP_EVENT_ADV_REPORT);
		if (conn_req_pending == 0) {
			if (num_of_conn_slaves < RSI_BLE_MAX_NBR_SLAVES) {
#if(CONNECT_OPTION == CONN_BY_NAME)
				if (((strcmp((const char *) remote_name,
						(void *) RSI_REMOTE_DEVICE_NAME1)) == 0)
						|| ((strcmp((const char *) remote_name,
								(void *) RSI_REMOTE_DEVICE_NAME2)) == 0)
						|| ((strcmp((const char *) remote_name,
								(void *) RSI_REMOTE_DEVICE_NAME3)) == 0)
						|| ((strcmp((const char *) remote_name,
								(void *) RSI_REMOTE_DEVICE_NAME4)) == 0)
						|| ((strcmp((const char *) remote_name,
								(void *) RSI_REMOTE_DEVICE_NAME5)) == 0)
						|| ((strcmp((const char *) remote_name,
								(void *) RSI_REMOTE_DEVICE_NAME6)) == 0))
#else
						if ((!strcmp(RSI_BLE_DEV_1_ADDR, (char *) remote_dev_addr))
								|| (!strcmp(RSI_BLE_DEV_2_ADDR,
												(char *) remote_dev_addr))
								|| (!strcmp(RSI_BLE_DEV_3_ADDR,
												(char *) remote_dev_addr))
								|| (!strcmp(RSI_BLE_DEV_4_ADDR,
												(char *) remote_dev_addr))
								|| (!strcmp(RSI_BLE_DEV_5_ADDR,
												(char *) remote_dev_addr))
								|| (!strcmp(RSI_BLE_DEV_6_ADDR,
												(char *) remote_dev_addr)))

#endif
						{
					LOG_PRINT("\n Device found. Stop scanning \n");
					status = rsi_ble_stop_scanning();
					rsi_scan_in_progress = 0;
					if (status != RSI_SUCCESS) {
						LOG_PRINT("Scan stopping failed with status : %d \n",
								status);
						//return status;
					}
					rsi_app_no_of_adv_reports_rcvd = 0;
					LOG_PRINT("\n Connect command \n");
					memset(remote_name, 0, sizeof(remote_name));
					if ((conn_done == 1) || (conn_done == 2)) {
						status = rsi_ble_connect_with_params(remote_addr_type,
								(int8_t *) remote_dev_bd_addr,
								LE_SCAN_INTERVAL_CONN,
								LE_SCAN_WINDOW_CONN,
								M2S12_CONNECTION_INTERVAL_MAX,
								M2S12_CONNECTION_INTERVAL_MIN,
								M2S12_CONNECTION_LATENCY,
								M2S12_SUPERVISION_TIMEOUT);
						LOG_PRINT("Conneting to device : \ndev_addr : %s\n conn_int_max: %d \nconn_int_min: %d \ntimeout: %d\n",(int8_t *) remote_dev_addr_conn, M2S12_CONNECTION_INTERVAL_MAX, M2S12_CONNECTION_INTERVAL_MIN, M2S12_SUPERVISION_TIMEOUT);
					} else if ((conn_done == 3) || (conn_done == 4)) {
						status = rsi_ble_connect_with_params(remote_addr_type,
								(int8_t *) remote_dev_bd_addr,
								LE_SCAN_INTERVAL_CONN,
								LE_SCAN_WINDOW_CONN,
								M2S34_CONNECTION_INTERVAL_MAX,
								M2S34_CONNECTION_INTERVAL_MIN,
								M2S34_CONNECTION_LATENCY,
								M2S34_SUPERVISION_TIMEOUT);
						LOG_PRINT("Conneting to device : \ndev_addr : %s\n conn_int_max: %d \nconn_int_min: %d \ntimeout: %d\n",(int8_t *) remote_dev_addr_conn, M2S34_CONNECTION_INTERVAL_MAX, M2S34_CONNECTION_INTERVAL_MIN, M2S34_SUPERVISION_TIMEOUT);
					} else if ((conn_done == 5) || (conn_done == 6)) {
						status = rsi_ble_connect_with_params(remote_addr_type,
								(int8_t *) remote_dev_bd_addr,
								LE_SCAN_INTERVAL_CONN,
								LE_SCAN_WINDOW_CONN,
								M2S56_CONNECTION_INTERVAL_MAX,
								M2S56_CONNECTION_INTERVAL_MIN,
								M2S56_CONNECTION_LATENCY,
								M2S56_SUPERVISION_TIMEOUT);
						LOG_PRINT("Conneting to device : \ndev_addr : %s\n conn_int_max: %d \nconn_int_min: %d \ntimeout: %d\n",(int8_t *) remote_dev_addr_conn, M2S56_CONNECTION_INTERVAL_MAX, M2S56_CONNECTION_INTERVAL_MIN, M2S56_SUPERVISION_TIMEOUT);
					}
					if (status != RSI_SUCCESS) {
						LOG_PRINT("Connecting failed with status : %d \n", status);
						//return status;
						rsi_ble_app_set_event(RSI_BLE_SCAN_RESTART_EVENT);
						break;
					}
					conn_req_pending = conn_done;
				}
			}
		}
	}
		break;

	case RSI_BLE_CONN_EVENT: {
		//! event invokes when connection was completed

		//! clear the served event

		rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);

		LOG_PRINT("\n Number of slave devices connected:%d \n", num_of_conn_slaves);
		LOG_PRINT("\n Number of master devices connected:%d \n", num_of_conn_masters);

		conn_req_pending = 0;

		device_found = 0;

		rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);

		if (num_of_conn_masters < RSI_BLE_MAX_NBR_MASTERS) {
			status = rsi_ble_start_advertising();
			if (status != RSI_SUCCESS) {
				//return status;
			}
		}
		service_uuid.size = 2;
		service_uuid.val.val16 = RSI_BLE_NEW_CLIENT_SERVICE_UUID;
		status = rsi_ble_get_profile(remote_dev_bd_addr, service_uuid,
				&rsi_ble_service);

		if (num_of_conn_slaves < RSI_BLE_MAX_NBR_SLAVES) {
			LOG_PRINT("\n Start scanning \n");
			rsi_app_no_of_adv_reports_rcvd = 0;
			device_found = 0;
			status = rsi_ble_start_scanning();
			rsi_scan_in_progress = 1;
			if (status != RSI_SUCCESS) {
				//return status;
			}
		}

	}
		break;

	case RSI_BLE_RECEIVE_REMOTE_FEATURES: {

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
#if 0
		uint8_t received_rem_devce_addr[18];
		rsi_6byte_dev_address_to_ascii((int8_t *) received_rem_devce_addr,
								(uint8_t *) remote_dev_feature.dev_addr);
		if(( (strcmp((const char *) remote_dev_addr_conn7, (const char *) received_rem_devce_addr)) == 0) &&
			 ((conn_master_done & BIT(0))) && (!(conn_update_master_done & BIT(0))) ) {
	#if UPDATE_CONN_PARAMETERS
			    LOG_PRINT("\n Connection update initiated from RSI \n");
				//! Check for connection parameters and update the connection parameters
				status = rsi_ble_conn_params_update((int8_t *)remote_dev_feature.dev_addr,
						(uint16_t)S2M_CONNECTION_INTERVAL_MIN,
						(uint16_t) S2M_CONNECTION_INTERVAL_MAX,
						(uint16_t) S2M_CONNECTION_LATENCY,
						(uint16_t) S2M_SUPERVISION_TIMEOUT);
				if(status != 0)
				{
					LOG_PRINT("\n Connection update initiation from RSI as slave failed with status : %d\n", status);
				}
				conn_update_master_done |= BIT(0);
	#endif
			}
		else if(( (strcmp((const char *) remote_dev_addr_conn8, (const char *) received_rem_devce_addr)) == 0) &&
					 ((conn_master_done & BIT(1))) && (!(conn_update_master_done & BIT(1))) ) {
	#if UPDATE_CONN_PARAMETERS
				//! Check for connection parameters and update the connection parameters
				status = rsi_ble_conn_params_update((int8_t *)remote_dev_feature.dev_addr,
						S2M_CONNECTION_INTERVAL_MIN,
						(uint16_t) S2M_CONNECTION_INTERVAL_MAX,
						(uint16_t) S2M_CONNECTION_LATENCY,
						(uint16_t) S2M_SUPERVISION_TIMEOUT);
				if(status != 0)
				{
					LOG_PRINT("\n Connection update initiation from RSI as slave failed with status : %d\n", status);
				}

				conn_update_master_done |= BIT(1);
				LOG_PRINT("\n Connection update initiated from RSI \n");
	#endif
			}
#endif
		break;
	}

	case RSI_BLE_CONN_UPDATE_COMPLETE_EVENT: {

		rsi_ble_app_clear_event(RSI_BLE_CONN_UPDATE_COMPLETE_EVENT);
#if UPDATE_CONN_PARAMETERS
		status = rsi_conn_update_request();
#endif
	}
		break;

	case RSI_BLE_DISCONN_EVENT: {

		LOG_PRINT("\n Device disconnected\n ");
		LOG_PRINT("\n Number of connected slave devices:%d\n", num_of_conn_slaves);
		LOG_PRINT("\n Number of connected master devices:%d\n", num_of_conn_masters);
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);

		if (num_of_conn_masters < RSI_BLE_MAX_NBR_MASTERS) {
			status = rsi_ble_start_advertising();
			if (status != RSI_SUCCESS) {
				//return status;
			}
		}

		if ((!rsi_scan_in_progress) && (num_of_conn_slaves < RSI_BLE_MAX_NBR_SLAVES)) {
			rsi_app_no_of_adv_reports_rcvd = 0;
			status = rsi_ble_start_scanning();
			rsi_scan_in_progress = 1;
			if ((status != RSI_SUCCESS)) {
				//return status;
			}
		}

	}
		break;
	case RSI_BLE_GATT_WRITE_EVENT: {
		//! event invokes when write/notification events received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_WRITE_EVENT);

		//! prepare the data to set as local attribute value.
		memset(data, 0, sizeof(data));
		memcpy(data, app_ble_write_event.att_value, app_ble_write_event.length);

		//! set the local attribute value.
		rsi_ble_set_local_att_value(0x000C, RSI_BLE_MAX_DATA_LEN,
				(uint8_t *) data);

	}
		break;
	case RSI_BLE_READ_REQ_EVENT: {
		//! event invokes when write/notification events received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_READ_REQ_EVENT);
#if 0
		if (conn_done == 1)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[0].dev_addr);
		else if (conn_done == 2)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[1].dev_addr);
		else if (conn_done == 3)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[2].dev_addr);
		else if (conn_done == 4)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[3].dev_addr);
		else if (conn_done == 5)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[4].dev_addr);
		else if (conn_done == 6)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[5].dev_addr);
#endif

		rsi_ble_gatt_read_response((uint8_t *) conn_event_to_app[conn_done - 1].dev_addr,
        0, app_ble_read_event.handle, 0,
				RSI_MIN(mtu_size - 2, sizeof(read_data)), read_data);
	}
		break;

	case RSI_BLE_GATT_PROFILE_RESP_EVENT: {
		//! event invokes when get profile response received

		//! clear the served event
		rsi_ble_app_clear_event(
		RSI_BLE_GATT_PROFILE_RESP_EVENT);
#if 0
		if (conn_done == 1)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[0].dev_addr);
		else if (conn_done == 2)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[1].dev_addr);
		else if (conn_done == 3)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[2].dev_addr);
		else if (conn_done == 4)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[3].dev_addr);
		else if (conn_done == 5)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[4].dev_addr);
		else if (conn_done == 6)
			rsi_6byte_dev_address_to_ascii((int8_t *) remote_dev_addr,
					(uint8_t *) conn_event_to_app[5].dev_addr);
#endif

		//! get characteristics of the immediate alert servcie
		rsi_ble_get_char_services((uint8_t *)conn_event_to_app[conn_done - 1].dev_addr,
				*(uint16_t *) rsi_ble_service.start_handle,
				*(uint16_t *) rsi_ble_service.end_handle, &char_servs);

	}
		break;

	case RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT: {
		//! event invokes when get characteristics of the service response received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);

		//! verifying the immediate alert characteristic
		for (ix = 0; ix < char_servs.num_of_services; ix++) {
			if (char_servs.char_services[ix].char_data.char_uuid.val.val16
					== RSI_BLE_CLIENT_ATTRIBUTE_1_UUID) {
				rsi_ble_read_att1_val_hndl =
						char_servs.char_services[ix].char_data.char_handle;

				rsi_ble_set_att_cmd(remote_dev_bd_addr,
						rsi_ble_read_att1_val_hndl + 1, 2,
						(uint8_t *) &client_data1);

				break;
			}
		}

	}
		break;

	case RSI_BLE_MTU_EVENT: {
		//! event invokes when write/notification events received

		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_MTU_EVENT);

	}
		break;
	case RSI_BLE_SCAN_RESTART_EVENT:
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SCAN_RESTART_EVENT);

		device_found = 0;

		status = rsi_ble_stop_scanning();
		rsi_scan_in_progress = 0;
		if (status != RSI_SUCCESS) {
			//return status;
		}

		LOG_PRINT("\n Restarting scanning \n");
		rsi_app_no_of_adv_reports_rcvd = 0;
		status = rsi_ble_start_scanning();
		rsi_scan_in_progress = 1;
		if (status != RSI_SUCCESS) {
			//return status;
		}
		break;
	}
}
}
#if UPDATE_CONN_PARAMETERS
int32_t rsi_conn_update_request( )
{
#if(CONNECT_OPTION == CONN_BY_NAME)
	if ( (strcmp((const char *) remote_dev_addr_conn1,
								(const char *) remote_dev_addr_conn_update)) == 0)
#else 
if (!strcmp((char *)remote_dev_addr_conn_update, RSI_BLE_DEV_1_ADDR)) 
#endif
	{
		if( (conn_update_resp.conn_interval < M2S12_CONNECTION_INTERVAL_MAX ) || (conn_update_resp.conn_latency < M2S12_CONNECTION_LATENCY ) || (conn_update_resp.timeout < M2S12_SUPERVISION_TIMEOUT ) )
		{
			conn_update_done |= BIT(0);
			//! Check for connection parameters and update the connection parameters
#if(CONNECT_OPTION == CONN_BY_NAME)
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S12_CONNECTION_INTERVAL_MIN, (uint16_t)M2S12_CONNECTION_INTERVAL_MAX, (uint16_t)M2S12_CONNECTION_LATENCY, (uint16_t)M2S12_SUPERVISION_TIMEOUT);
#else
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S12_CONNECTION_INTERVAL_MIN, (uint16_t)M2S12_CONNECTION_INTERVAL_MAX, (uint16_t)M2S12_CONNECTION_LATENCY, (uint16_t)M2S12_SUPERVISION_TIMEOUT);
#endif
			LOG_PRINT("\n Connection update initiated from RSI \n");
			return 0;
		}
		else
			conn_update_done &= ~BIT(0);
	}
#if(CONNECT_OPTION == CONN_BY_NAME)
	else if ( (strcmp((const char *) remote_dev_addr_conn2,
									(const char *) remote_dev_addr_conn_update)) == 0)  
#else
    else if(!strcmp((char *)remote_dev_addr_conn_update, RSI_BLE_DEV_2_ADDR))
#endif
	{
		if( (conn_update_resp.conn_interval < M2S12_CONNECTION_INTERVAL_MAX ) || (conn_update_resp.conn_latency < M2S12_CONNECTION_LATENCY ) || (conn_update_resp.timeout < M2S12_SUPERVISION_TIMEOUT ) )
		{
			conn_update_done |= BIT(1);
			//! Check for connection parameters and update the connection parameters
#if(CONNECT_OPTION == CONN_BY_NAME)
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S12_CONNECTION_INTERVAL_MIN, (uint16_t)M2S12_CONNECTION_INTERVAL_MAX, (uint16_t)M2S12_CONNECTION_LATENCY, (uint16_t)M2S12_SUPERVISION_TIMEOUT);
#else
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S12_CONNECTION_INTERVAL_MIN, (uint16_t)M2S12_CONNECTION_INTERVAL_MAX, (uint16_t)M2S12_CONNECTION_LATENCY, (uint16_t)M2S12_SUPERVISION_TIMEOUT);
#endif
			LOG_PRINT("\n Connection update initiated from RSI \n");
			return 0;
		}
		else
			conn_update_done &= ~BIT(1);
	}
#if(CONNECT_OPTION == CONN_BY_NAME)
	else if ( (strcmp((const char *) remote_dev_addr_conn3,
										(const char *) remote_dev_addr_conn_update)) == 0) 
#else
	else if(!strcmp((char *)remote_dev_addr_conn_update, RSI_BLE_DEV_3_ADDR))
#endif
	{
		if( (conn_update_resp.conn_interval < M2S34_CONNECTION_INTERVAL_MAX ) || (conn_update_resp.conn_latency < M2S34_CONNECTION_LATENCY ) || (conn_update_resp.timeout < M2S34_SUPERVISION_TIMEOUT ) )
		{
			conn_update_done |= BIT(2);
			//! Check for connection parameters and update the connection parameters
#if(CONNECT_OPTION == CONN_BY_NAME)
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S34_CONNECTION_INTERVAL_MIN, (uint16_t)M2S34_CONNECTION_INTERVAL_MAX, (uint16_t)M2S34_CONNECTION_LATENCY, (uint16_t)M2S34_SUPERVISION_TIMEOUT);
#else
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S34_CONNECTION_INTERVAL_MIN, (uint16_t)M2S34_CONNECTION_INTERVAL_MAX, (uint16_t)M2S34_CONNECTION_LATENCY, (uint16_t)M2S34_SUPERVISION_TIMEOUT);
#endif
			LOG_PRINT("\n Connection update initiated from RSI \n");
			return 0;
		}
		else
			conn_update_done &= ~BIT(2);
	}
#if(CONNECT_OPTION == CONN_BY_NAME)
	else if ( (strcmp((const char *) remote_dev_addr_conn4,
										(const char *) remote_dev_addr_conn_update)) == 0)  
#else
    else if(!strcmp((char *)remote_dev_addr_conn_update, RSI_BLE_DEV_4_ADDR))
#endif
	{
		if( (conn_update_resp.conn_interval < M2S34_CONNECTION_INTERVAL_MAX ) || (conn_update_resp.conn_latency < M2S34_CONNECTION_LATENCY ) || (conn_update_resp.timeout < M2S34_SUPERVISION_TIMEOUT ) )
		{
			conn_update_done |= BIT(3);
			//! Check for connection parameters and update the connection parameters
#if(CONNECT_OPTION == CONN_BY_NAME)
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S34_CONNECTION_INTERVAL_MIN, (uint16_t)M2S34_CONNECTION_INTERVAL_MAX, (uint16_t)M2S34_CONNECTION_LATENCY, (uint16_t)M2S34_SUPERVISION_TIMEOUT);
#else
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S34_CONNECTION_INTERVAL_MIN, (uint16_t)M2S34_CONNECTION_INTERVAL_MAX, (uint16_t)M2S34_CONNECTION_LATENCY, (uint16_t)M2S34_SUPERVISION_TIMEOUT);
#endif
			return 0;
		}
		else
			conn_update_done &= ~BIT(3);
	}
#if(CONNECT_OPTION == CONN_BY_NAME)
	else if ( (strcmp((const char *) remote_dev_addr_conn5,
										(const char *) remote_dev_addr_conn_update)) == 0)  
#else
    else if(!strcmp((char *)remote_dev_addr_conn_update, RSI_BLE_DEV_5_ADDR))
#endif
	{
		if( (conn_update_resp.conn_interval < M2S56_CONNECTION_INTERVAL_MAX ) || (conn_update_resp.conn_latency < M2S56_CONNECTION_LATENCY ) || (conn_update_resp.timeout < M2S56_SUPERVISION_TIMEOUT ) )
		{
			conn_update_done |= BIT(4);
			//! Check for connection parameters and update the connection parameters
#if(CONNECT_OPTION == CONN_BY_NAME)
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S56_CONNECTION_INTERVAL_MIN, (uint16_t)M2S56_CONNECTION_INTERVAL_MAX, (uint16_t)M2S56_CONNECTION_LATENCY, (uint16_t)M2S56_SUPERVISION_TIMEOUT);
#else
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S56_CONNECTION_INTERVAL_MIN, (uint16_t)M2S56_CONNECTION_INTERVAL_MAX, (uint16_t)M2S56_CONNECTION_LATENCY, (uint16_t)M2S56_SUPERVISION_TIMEOUT);
#endif
			return 0;
		}
		else
			conn_update_done &= ~BIT(4);
	}
#if(CONNECT_OPTION == CONN_BY_NAME)
	else if ( (strcmp((const char *) remote_dev_addr_conn6,
										(const char *) remote_dev_addr_conn_update)) == 0) 
#else
    else if(!strcmp((char *)remote_dev_addr_conn_update, RSI_BLE_DEV_6_ADDR))
#endif	
	{
		if( (conn_update_resp.conn_interval < M2S56_CONNECTION_INTERVAL_MAX ) || (conn_update_resp.conn_latency < M2S56_CONNECTION_LATENCY ) || (conn_update_resp.timeout < M2S56_SUPERVISION_TIMEOUT ) )
		{
			conn_update_done |= BIT(5);
			//! Check for connection parameters and update the connection parameters
#if(CONNECT_OPTION == CONN_BY_NAME)
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S56_CONNECTION_INTERVAL_MIN, (uint16_t)M2S56_CONNECTION_INTERVAL_MAX, (uint16_t)M2S56_CONNECTION_LATENCY, (uint16_t)M2S56_SUPERVISION_TIMEOUT);
#else
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, M2S56_CONNECTION_INTERVAL_MIN, (uint16_t)M2S56_CONNECTION_INTERVAL_MAX, (uint16_t)M2S56_CONNECTION_LATENCY, (uint16_t)M2S56_SUPERVISION_TIMEOUT);
#endif
			return 0;
		}
		else
			conn_update_done &= ~BIT(5);
	}
	//! Connection updates for our master
	else if ((strcmp((const char *) remote_dev_addr_conn7,
										(const char *) remote_dev_addr_conn_update)) == 0)
	{
		if( (conn_update_resp.conn_interval < S2M_CONNECTION_INTERVAL_MAX ) || (conn_update_resp.conn_latency < S2M_CONNECTION_LATENCY ) || (conn_update_resp.timeout < S2M_SUPERVISION_TIMEOUT ) )
		{
			conn_update_master_done |= BIT(0);
			//! Check for connection parameters and update the connection parameters
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, S2M_CONNECTION_INTERVAL_MIN, (uint16_t)S2M_CONNECTION_INTERVAL_MAX, (uint16_t)S2M_CONNECTION_LATENCY, (uint16_t)S2M_SUPERVISION_TIMEOUT);
			LOG_PRINT("\n Connection update initiated from RSI \n");
			return 0;
		}
		else
			conn_update_master_done &= ~BIT(0);
	}
	//! Connection updates for our master
	else if ((strcmp((const char *) remote_dev_addr_conn8,
										(const char *) remote_dev_addr_conn_update)) == 0)
	{
		if( (conn_update_resp.conn_interval < S2M_CONNECTION_INTERVAL_MAX ) || (conn_update_resp.conn_latency < S2M_CONNECTION_LATENCY ) || (conn_update_resp.timeout < S2M_SUPERVISION_TIMEOUT ) )
		{
			conn_update_master_done |= BIT(1);
			//! Check for connection parameters and update the connection parameters
			rsi_ble_conn_params_update((int8_t *)conn_update_resp.dev_addr, S2M_CONNECTION_INTERVAL_MIN, (uint16_t)S2M_CONNECTION_INTERVAL_MAX, (uint16_t)S2M_CONNECTION_LATENCY, (uint16_t)S2M_SUPERVISION_TIMEOUT);
			LOG_PRINT("\n Connection update initiated from RSI \n");
			return 0;
		}
		else
			conn_update_master_done &= ~BIT(1);
	}
	return 0;
}
#endif
#endif
