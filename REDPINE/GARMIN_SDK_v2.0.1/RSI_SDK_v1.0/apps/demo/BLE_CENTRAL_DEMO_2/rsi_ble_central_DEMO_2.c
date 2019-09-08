/**
 * @file    rsi_ble_central_DEMO_2.c
 * @version 0.1
 * @date    05 Sep 2018
 *
 *  Copyright(C) Redpine Signals 2018
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BLE dual-role
 *
 *  @section Description  This application serves as a BLE Peripheral/Slave and Central/Master.
 *
 */

/*=======================================================================*/
//   ! INCLUDES
/*=======================================================================*/

#include <rsi_common_app.h>
#if BLE_CENTRAL
#include "stdint.h"
//! BLE include file to refer BLE APIs
#include <rsi_ble_apis.h>
#include <rsi_ble_config.h>
#include <rsi_bt_common_apis.h>
#include <rsi_bt_common.h>
//! Common include file
#include <rsi_common_apis.h>
#include <rsi_ble_config.h>
#include <string.h>

/*=======================================================================*/
//   ! MACROS
/*=======================================================================*/

/*=======================================================================*/
//   ! GLOBAL VARIABLES
/*=======================================================================*/

uint8_t          conn_dev_addr[18] = {0},ix,conn_done;
uint16_t         num_of_conn_slaves = 0,count;
uint8_t          num_of_conn_masters = 0;
uint16_t         conn_req_pending = 0;
uint8_t          remote_dev_addr_conn[18] = {0};
uint8_t          remote_dev_bd_addr[6] = {0};
uint8_t          rsi_conn_params_update;
uint32_t         start_handle;
uint32_t         end_handle;
uint32_t         number_of_profiles;
uint8_t          profiles_list_done;

#if(DATA_TRANSFER_ENABLE == 1)
static uint16_t  rsi_ble_att1_val_hndl;
static uint16_t  rsi_ble_att2_val_hndl;
static rsi_ble_notified_device_t notify_device_list[6];
#endif

static uint8_t   rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint16_t  rsi_scan_in_progress;
static uint16_t  rsi_app_no_of_adv_reports_rcvd = 0;
static uint32_t  rsi_app_async_event_map = 0;
static uint32_t  rsi_ble_async_event_mask = 0;
static uint8_t   device_found=0;
static uint8_t   remote_name[31];
static uint8_t   remote_name_conn[31];
static uint8_t   remote_addr_type=0;
static uint8_t   remote_dev_addr[18] = {0};
static uint16_t  rsi_ble_att1_val_hndl, rsi_ble_read_att1_val_hndl;

//! Structure variables
static rsi_bt_resp_get_local_name_t  rsi_app_resp_get_local_name = {0};
static rsi_ble_event_adv_report_t    rsi_app_adv_reports_to_app[NO_OF_ADV_REPORTS];
static rsi_ble_event_conn_status_t   conn_event_to_app[10];
static rsi_ble_event_disconnect_t    disconn_event_to_app;
static rsi_ble_event_write_t         app_ble_write_event;
static rsi_ble_resp_char_services_t  char_servs;
static rsi_ble_resp_profiles_list_t  prof_list;
static profile_descriptors_t         list_of_profs[100];
static profile_descriptors_t         rsi_ble_service;


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

#if(DATA_TRANSFER_ENABLE == 1)
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
#endif
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
static void rsi_ble_app_set_event(uint32_t event_num)
{
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
static void rsi_ble_app_clear_event(uint32_t event_num)
{
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
static int32_t rsi_ble_app_get_event(void)
{
	uint32_t  ix;

	for (ix = 0; ix < 32; ix++)
	{
		if (rsi_app_async_event_map & (1 << ix))
		{
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
void rsi_ble_on_adv_report_event(rsi_ble_event_adv_report_t *adv_report)
 {
	//uint8_t status = 0;

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
		LOG_PRINT("\nrestart scan\r\n");
		rsi_ble_app_set_event(RSI_BLE_SCAN_RESTART_EVENT);
	}

	BT_LE_ADPacketExtract(remote_name, adv_report->adv_data,
			adv_report->adv_data_len);

	remote_addr_type = adv_report->dev_addr_type;
	rsi_6byte_dev_address_to_ascii((int8_t *)remote_dev_addr,
			(uint8_t *) adv_report->dev_addr);
  memcpy ((int8_t *)remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);

#if (CONNECT_OPTION_APP2 == CONN_BY_NAME)
	if ((device_found == 0)
			&& ((strcmp((const char *)remote_name, RSI_REMOTE_DEVICE_NAME)) == 0)) {
		memcpy(remote_name_conn, remote_name, 31);
		memcpy(remote_dev_addr_conn, remote_dev_addr, 18);
		if (conn_done == 0) {
			device_found = 1;
			conn_done = 1;
			rsi_ble_app_set_event(RSI_BLE_ADV_REPORT_EVENT);
			return;
		}
	}
#else
	if ((!strcmp (RSI_BLE_DEV_1_ADDR, (char *)remote_dev_addr)))
	{
		memcpy (conn_dev_addr, remote_dev_addr, sizeof (remote_dev_addr));
		memcpy(remote_dev_addr_conn,remote_dev_addr,18);
		device_found = 1;
		rsi_ble_app_set_event (RSI_BLE_ADV_REPORT_EVENT);
	}
#endif

	return;
}

#if(DATA_TRANSFER_ENABLE == 1)
/*==============================================
 *
 * @fn         rsi_ble_notify_to_requested_device
 * @brief      send notifications to specific devices
 * @param[in]  data
 * @return     none.
 ===============================================*/
void rsi_ble_notify_to_requested_device(uint8_t *data)
{
  if(count == 50000){
    for(ix=0;ix<MAX_NUM_OF_SLAVES;ix++) {
      if(notify_device_list[ix].used ==1){
        rsi_ble_notify_value(notify_device_list[ix].dev_addr,rsi_ble_att2_val_hndl,RSI_BLE_MAX_DATA_LEN,data);
      }
    }
    count =0;
  }
  return;
}
/*==============================================
 *
 * @fn         add_device_to_notify_list
 * @brief      add device to the notify list
 * @param[in]  dev_addr
 * @return     none.
 ===============================================*/

void add_device_to_notify_list(uint8_t *dev_addr)
{
  for(ix =0;ix<MAX_NUM_OF_NOTIFIED_DEVICES;ix++)
  {
    if(notify_device_list[ix].used == 0){
      memcpy(notify_device_list[ix].dev_addr,dev_addr,6);
      notify_device_list[ix].used = 1;
      return;
    }
  }
}

/*==============================================
 *
 * @fn         remove_device_from_notify_list
 * @brief      use to remove device from the notify list
 * @param[in]  dev_addr
 * @return     none.
 ===============================================*/
void remove_device_from_notify_list(uint8_t *dev_addr)
{
  for(ix=0;ix<MAX_NUM_OF_NOTIFIED_DEVICES;ix++){
    if(!memcmp(notify_device_list[ix].dev_addr,dev_addr,6)){
      notify_device_list[ix].used = 0;
      return;
    }
  }
}
/*==============================================
 *
 * @fn         rsi_ble_handle_notifications
 * @brief      it will handle notification enabling and disabling according to remote device request
 * @param[in]  none
 * @return     none.
 * ================================================*/
void rsi_ble_handle_notifications()
{

  if((rsi_ble_att2_val_hndl + 1) == *((uint16_t *)app_ble_write_event.handle)){
    if(app_ble_write_event.att_value[0] == 1){
      add_device_to_notify_list(app_ble_write_event.dev_addr);
      rsi_ble_app_set_event (RSI_BLE_GATT_NOTIFY_EVENT)  ;
    }else{
      remove_device_from_notify_list(app_ble_write_event.dev_addr);
      for(ix=0;ix<MAX_NUM_OF_NOTIFIED_DEVICES;ix++)
      {
        if(notify_device_list[ix].used==1){
          return;
        }
      }
      rsi_ble_app_clear_event(RSI_BLE_GATT_NOTIFY_EVENT);
    }
  }
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
void rsi_ble_on_gatt_write_event(uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{

  memcpy(&app_ble_write_event , rsi_ble_write , sizeof(rsi_ble_event_write_t));

  rsi_ble_handle_notifications();

  return;
}
#endif
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
	if(resp_enh_conn->status != 0)
		return;


  uint8_t str_conn_device[18] = {0};
#if(CONNECT_OPTION_APP2 == CONN_BY_ADDR)
  //rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_device, (uint8_t *)resp_enh_conn->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *)resp_enh_conn->dev_addr, 6);

  if(!strcmp((char *)str_conn_device, RSI_BLE_DEV_1_ADDR))
  {
    conn_event_to_app[0].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[0].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[0].status = resp_enh_conn->status;
  }
  if(resp_enh_conn->status == 0)
	  rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
#else
  rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
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
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn)
{
	if(resp_conn->status != 0)
		return;

  uint8_t str_conn_device[18] = {0};

#if(CONNECT_OPTION_APP2 == CONN_BY_ADDR)
  //rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_device, (uint8_t *)resp_conn->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *)resp_conn->dev_addr, 6);
  if(!strcmp((char *)str_conn_device, RSI_BLE_DEV_1_ADDR))
  {
    memcpy(&conn_event_to_app[0], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
#endif
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
  uint8_t str_disconn_device[18] = {0};

#if(CONNECT_OPTION_APP2 == CONN_BY_ADDR)
  rsi_6byte_dev_address_to_ascii ((int8_t *)str_disconn_device, (uint8_t *)resp_disconnect->dev_addr);
  if(!strcmp((char *)str_disconn_device, RSI_BLE_DEV_1_ADDR))
  {
    memset(&conn_event_to_app[0], 0, sizeof(rsi_ble_event_conn_status_t));
  }
#endif
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
}
#if(DATA_TRANSFER_ENABLE == 1)
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
      RSI_BLE_ATT_PROPERTY_WRITE,
      new_serv_resp.start_handle + 2,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 2,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_WRITE,
      data,
      sizeof (data));


  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_2_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 3,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
      new_serv_resp.start_handle + 4,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_att2_val_hndl = new_serv_resp.start_handle + 4;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_2_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 4,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
      data,
      sizeof (data));

  return 0;
}
#endif

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
static void ble_on_profiles_list_resp (uint16_t resp_status, rsi_ble_resp_profiles_list_t  *rsi_ble_resp_profiles)
{
	if(resp_status != 0)
	{
		profiles_list_done = 1;
       	LOG_PRINT("\nresp status 0x%04x", resp_status);
	}

	rsi_ble_app_set_event (RSI_BLE_PROFILE_LIST_EVENT);
  return;
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

uint32_t rsi_ble_on_conn_update_complete_event(
		rsi_ble_event_conn_update_t *rsi_ble_event_conn_update_complete,
		uint16_t resp_status) {
	rsi_ble_app_set_event(RSI_BLE_CONN_UPDATE_EVENT);
	return 0;
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
int32_t rsi_ble_dual_role(void)
{
	int32_t status = 0;
	//uint8_t adv[31] = {2,1,6};
    int8_t  data[20] = {0};

	//! initialize the event map
	rsi_ble_app_init_events ();

	//! registering the GAP callback functions
    rsi_ble_gap_register_callbacks(
      rsi_ble_on_adv_report_event,
      rsi_ble_on_connect_event,
      rsi_ble_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_on_enhance_conn_status_event,
      NULL,
	  (void *)rsi_ble_on_conn_update_complete_event);
#if(DATA_TRANSFER_ENABLE == 1)
  //! adding simple BLE chat service
  rsi_ble_add_simple_chat_serv();

  data[0]=1;  data[1]=2;  data[2]=3;  data[3]=4;  data[4]=5;  data[5]=6;  data[6]=7;  data[7]=8;  data[8]=9;
  //! set the local attribute value.
  rsi_ble_set_local_att_value (rsi_ble_att2_val_hndl, RSI_BLE_MAX_DATA_LEN, (uint8_t *)data);

#endif
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
      NULL,
      NULL);


  //! initialize the event map
  rsi_ble_app_init_events ();

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

	//! set the local device name
	status = rsi_bt_set_local_name(RSI_BLE_LOCAL_NAME);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! get the local device name
	status = rsi_bt_get_local_name(&rsi_app_resp_get_local_name);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

  LOG_PRINT("\n Start scanning \n");
  rsi_app_no_of_adv_reports_rcvd = 0;
  rsi_ble_start_scanning();
  rsi_scan_in_progress = 1;
  if(status != RSI_SUCCESS)
  {
    return status;
  }
	return 0;
}


int32_t rsi_ble_app_task()
 {
	int32_t status = 0;
	int32_t temp_event_map = 0;
	int8_t i = 0;
	int8_t data[20]= {0};
	int8_t client_data1[2] = { 1, 0 };
	uuid_t service_uuid = { 0 };

	//! Application main loop
#ifndef RSI_WITH_OS
	//! run the non OS scheduler
	//rsi_wireless_driver_task();
#endif
	//! checking for received events
	temp_event_map = rsi_ble_app_get_event();
	if (temp_event_map == RSI_FAILURE) {
		//! if events are not received loop will be continued.
		return 0;
	}

	//! if any event is received, it will be served.
	switch (temp_event_map) {
	case RSI_BLE_ADV_REPORT_EVENT: {
		LOG_PRINT("\n Advertise report received \n");
		if (conn_req_pending == 0) {
			if (num_of_conn_slaves < MAX_NUM_OF_SLAVES) {
#if(CONNECT_OPTION_APP2 == CONN_BY_NAME)
				if (((strcmp((const char *)remote_name, (void *) RSI_REMOTE_DEVICE_NAME)) == 0))
#else
				if ((!strcmp((char *) conn_dev_addr, RSI_BLE_DEV_1_ADDR)))
#endif
				{
					LOG_PRINT("\n Device found. Stop scanning \n");
					status = rsi_ble_stop_scanning();
					rsi_scan_in_progress = 0;
					if (status != RSI_SUCCESS) {
						//return status;
					}
					LOG_PRINT("\n Connect command \n");
					status = rsi_ble_connect(remote_addr_type,
							(int8_t *) remote_dev_bd_addr);
					if (status != RSI_SUCCESS) {
						//return status;
					}
					memset(remote_name, 0, sizeof(remote_name));
					conn_req_pending = 1;
				}
			}
		}
		//! clear the advertise report event.
		rsi_ble_app_clear_event(RSI_BLE_ADV_REPORT_EVENT);
	}
		break;

	case RSI_BLE_CONN_EVENT: {
		num_of_conn_slaves++;

		LOG_PRINT("\n Device connected \n ");
		LOG_PRINT("\n Number of devices connected:%d \n", num_of_conn_slaves);
#if ENABLE_POWER_SAVE
		  //! initiateing power save in BLE mode
		  status = rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE);
		  if(status != RSI_SUCCESS)
		  {
			  return status;
		  }
#endif
		conn_req_pending = 0;
		device_found = 0;

		rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);

		memset(&prof_list, 0, sizeof(rsi_ble_resp_profiles_list_t));

		get_prof: status = rsi_ble_get_profiles(remote_dev_bd_addr,
				0x0001, 0xFFFF, &prof_list);
		if (status != 0) {
			goto get_prof;
		}
	}
		break;

	case RSI_BLE_DISCONN_EVENT: {
		num_of_conn_slaves--;

		LOG_PRINT("\n Device disconnected\n ");
		LOG_PRINT("\n Number of connected devices:%d\n", num_of_conn_slaves);
		rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);
		rsi_ble_app_set_event(RSI_BLE_SCAN_RESTART_EVENT);
	}
		break;

	case RSI_BLE_SCAN_RESTART_EVENT:
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_SCAN_RESTART_EVENT);
#if ENABLE_POWER_SAVE
		 //! initiateing power save in BLE mode
		status = rsi_bt_power_save_profile(0,0);
		if(status != RSI_SUCCESS)
		{
		//	return status;
		}
#endif
		status = rsi_ble_stop_scanning();
		rsi_scan_in_progress = 0;
		if (status != RSI_SUCCESS) {
			//return status;
		}

		LOG_PRINT("\n Start scanning \n");
		rsi_app_no_of_adv_reports_rcvd = 0;
		status = rsi_ble_start_scanning();
		rsi_scan_in_progress = 1;
		if (status != RSI_SUCCESS) {
			//return status;
		}
		break;

	case RSI_BLE_GATT_NOTIFY_EVENT: {
#if(DATA_TRANSFER_ENABLE == 1)
		count++;
		rsi_ble_notify_to_requested_device((uint8_t *)data);
#endif
	}
		break;
	case RSI_BLE_PROFILE_LIST_EVENT: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_PROFILE_LIST_EVENT);

		if(!profiles_list_done)
		{
			for (i = number_of_profiles; i < prof_list.number_of_profiles;
					i++) {
				memcpy(&list_of_profs[i], &prof_list.profile_desc[i],
						sizeof(profile_descriptors_t));
			}

			number_of_profiles += prof_list.number_of_profiles;
			end_handle =
					(prof_list.profile_desc[number_of_profiles - 1].end_handle[0])
							| (prof_list.profile_desc[number_of_profiles - 1].end_handle[1]
									<< 8);

			start_handle = end_handle + 1;
			status = rsi_ble_get_profiles(remote_dev_bd_addr,
					start_handle, 0xFFFF, &prof_list);
		}
		else
		{
			status = rsi_ble_conn_params_update((int8_t *)remote_dev_bd_addr, 320, 320, 0,
							400);
							rsi_conn_params_update = 1;
		}
	}
		break;

	case RSI_BLE_CONN_UPDATE_EVENT: {
		//! clear the served event
		rsi_ble_app_clear_event(RSI_BLE_CONN_UPDATE_EVENT);
		if(rsi_conn_params_update)
		{
			rsi_conn_params_update = 0;

			service_uuid.size = 2;
			service_uuid.val.val16 = RSI_BLE_NEW_CLIENT_SERVICE_UUID;
			status = rsi_ble_get_profile(remote_dev_bd_addr, service_uuid, &rsi_ble_service);

		}
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
							rsi_ble_read_att1_val_hndl + 1, 2, (uint8_t *)&client_data1);

					break;
				}
			}

			}
			break;
	case RSI_BLE_GATT_PROFILE_RESP_EVENT: {
			//! event invokes when get profile response received

			//! clear the served event
			rsi_ble_app_clear_event(RSI_BLE_GATT_PROFILE_RESP_EVENT);

			//! get characteristics of the immediate alert servcie
			rsi_ble_get_char_services(remote_dev_bd_addr, *(uint16_t *) rsi_ble_service.start_handle, *(uint16_t *) rsi_ble_service.end_handle, &char_servs);

		}
	break;
	}
	return 0;
}


#endif
