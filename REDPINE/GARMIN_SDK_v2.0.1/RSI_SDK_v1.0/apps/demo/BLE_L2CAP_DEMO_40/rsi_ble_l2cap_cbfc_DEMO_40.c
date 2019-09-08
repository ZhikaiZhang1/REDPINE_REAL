/**
 * @file    rsi_ble_l2cap_cbfc_DEMO_40.c
 * @version 0.1
 * @date    05 Sep 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BLE Central mode and l2cap connection.
 *
 *  @section Description  This application connects as a Central/Master with l3cap connection.
 *
 */

/*=======================================================================*/
//   ! INCLUDES
/*=======================================================================*/

#include <rsi_common_app.h>
#if BLE_L2CAP_CBFC
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

static uint16_t rsi_app_no_of_adv_reports_rcvd = 0;
static uint8_t rsi_app_async_event_map = 0;
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;
static rsi_ble_event_conn_status_t     rsi_app_connected_device;
static rsi_ble_event_disconnect_t      rsi_app_disconnected_device;
//static rsi_ble_event_cbfc_conn_req_t   rsi_app_cbfc_conn_req;
static rsi_ble_event_cbfc_conn_complete_t rsi_app_cbfc_conn_cmpl;

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
	rsi_app_async_event_map |= BIT(event_num);
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
  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
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
 * @fn         rsi_ble_simple_peripheral_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_central_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  rsi_app_connected_device.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (rsi_app_connected_device.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  rsi_app_connected_device.status = resp_enh_conn->status;
  rsi_ble_app_set_event (RSI_APP_EVENT_CONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_central_on_conn_status_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_central_on_conn_status_event(rsi_ble_event_conn_status_t *resp_conn)
{
	memcpy(&rsi_app_connected_device, resp_conn, sizeof(rsi_ble_event_conn_status_t));

	rsi_ble_app_set_event (RSI_APP_EVENT_CONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_central_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This Callback function indicates disconnected device information and status
 */
void rsi_ble_simple_central_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
	memcpy(&rsi_app_disconnected_device, resp_disconnect, sizeof(rsi_ble_event_disconnect_t ));

	rsi_ble_app_set_event (RSI_APP_EVENT_DISCONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_cbfc_conn_req_event
 * @brief      invoked when BLE L2CAP CBFC connect request event is received
 * @param[in]  rsi_ble_cbfc_conn_req, connection requested remote device information
 * @return     none.
 * @section description
 * This Callback function indicates BLE L2CAP CBFC connect request from remote device.
 */
void rsi_ble_on_cbfc_conn_req_event (rsi_ble_event_cbfc_conn_req_t *rsi_ble_cbfc_conn_req)
{
  uint8_t remote_dev_addr[18] = {0};

  //memcpy (remote_dev_addr, rsi_ble_cbfc_conn_req->dev_addr, 6);
  LOG_PRINT ( "remote_dev_addr: %s, lcid: 0x%x, %d\r\n" ,
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_ble_cbfc_conn_req->dev_addr),
      rsi_ble_cbfc_conn_req->psm, rsi_ble_cbfc_conn_req->lcid, rsi_ble_cbfc_conn_req->lcid);

  //memcpy (&rsi_app_cbfc_conn_req, rsi_ble_cbfc_conn_req, sizeof (rsi_ble_event_cbfc_conn_req_t));
	rsi_ble_app_set_event (RSI_APP_EVENT_CBFC_CONN_REQ);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_cbfc_conn_complete_event
 * @brief      invoked when BLE L2CAP CBFC connect complete status event is received
 * @param[in]  rsi_ble_cbfc_conn_complete, connection completed information with remote device.
 * @param[in]  status, connection completed status with remote device.
 * @return     none.
 * @section description
 * This Callback function indicates BLE L2CAP CBFC connect complete status with remote devivce.
 */
void rsi_ble_on_cbfc_conn_complete_event (rsi_ble_event_cbfc_conn_complete_t *rsi_ble_cbfc_conn_complete, uint16_t status)
{
  uint8_t remote_dev_addr[18] = {0};

  //memcpy (remote_dev_addr, rsi_ble_cbfc_conn_complete->dev_addr, 6);
  LOG_PRINT ("remote_dev_addr: %s, psm: 0x%x, lcid: 0x%x, %d\r\n",
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_ble_cbfc_conn_complete->dev_addr)
      , rsi_ble_cbfc_conn_complete->psm, rsi_ble_cbfc_conn_complete->lcid, rsi_ble_cbfc_conn_complete->lcid);

  memcpy (&rsi_app_cbfc_conn_cmpl, rsi_ble_cbfc_conn_complete, sizeof (rsi_ble_event_cbfc_conn_complete_t));
	rsi_ble_app_set_event (RSI_APP_EVENT_CBFC_CONN_CMPL);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_cbfc_rx_data_event
 * @brief      invoked when BLE L2CAP CBFC data receive event is received
 * @param[in]  rsi_ble_cbfc_rx_data, CBFC receive data info from remote device.
 * @return     none.
 * @section description
 * This Callback function indicates BLE L2CAP CBFC data received from remote devivce.
 */
void rsi_ble_on_cbfc_rx_data_event (rsi_ble_event_cbfc_rx_data_t *rsi_ble_cbfc_rx_data)
{
  uint8_t ix;
  uint8_t remote_dev_addr[18] = {0};

  //memcpy  (remote_dev_addr, rsi_ble_cbfc_rx_data->dev_addr, 6);
  LOG_PRINT ("remote_dev_addr: %s, lcid: 0x%x, %d, data_len: %d, data:\r\n",
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_ble_cbfc_rx_data->dev_addr),
      rsi_ble_cbfc_rx_data->lcid, rsi_ble_cbfc_rx_data->lcid, rsi_ble_cbfc_rx_data->len);

  for (ix= 0; ix < rsi_ble_cbfc_rx_data->len; ix++) {
    LOG_PRINT ("0x%x, ",rsi_ble_cbfc_rx_data->data[ix]);
  }
  LOG_PRINT ("\r\n");

	rsi_ble_app_set_event (RSI_APP_EVENT_CBFC_RX_DATA);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_cbfc_disconn_event
 * @brief      invoked when BLE L2CAP CBFC disconnect event is received
 * @param[in]  rsi_ble_cbfc_disconn, CBFC disconnected remote device info.
 * @return     none.
 * @section description
 * This Callback function indicates BLE L2CAP CBFC disconnect with remote devivce.
 */
void rsi_ble_on_cbfc_disconn_event (rsi_ble_event_cbfc_disconn_t *rsi_ble_cbfc_disconn)
{
  uint8_t remote_dev_addr[18] = {0};

  //memcpy  (remote_dev_addr, rsi_ble_cbfc_disconn->dev_addr);
  LOG_PRINT ("remote_dev_addr: %s, lcid: 0x%x, %d\r\n" ,
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_ble_cbfc_disconn->dev_addr),
      rsi_ble_cbfc_disconn->lcid,
      rsi_ble_cbfc_disconn->lcid);

	rsi_ble_app_set_event (RSI_APP_EVENT_CBFC_DISCONN);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_central_l2cap_cbfc
 * @brief      Tests the BLE L2cap central role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the BLE central role and simple GAP API's.
 */
int32_t rsi_ble_central_l2cap_cbfc(void)
{
	int32_t status = 0;

	//! BLE register GAP callbacks
	rsi_ble_gap_register_callbacks(
      rsi_ble_simple_central_on_adv_report_event,
      rsi_ble_simple_central_on_conn_status_event,
      rsi_ble_simple_central_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_simple_central_on_enhance_conn_status_event,
      NULL,
      NULL);

  rsi_ble_l2cap_cbsc_register_callbacks (
      rsi_ble_on_cbfc_conn_req_event, 
      rsi_ble_on_cbfc_conn_complete_event,
      rsi_ble_on_cbfc_rx_data_event,
      rsi_ble_on_cbfc_disconn_event);

	//! initialize the event map
	rsi_ble_app_init_events ();

  //! start scanning
  status = rsi_ble_start_scanning();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  else
  {
	  LOG_PRINT("\nScanning remote device\n");
  }
  return 0;
}

int32_t rsi_ble_app_task() {
    int32_t status = 0;
    int32_t temp_event_map = 0;
	uint8_t ix;
	uint8_t cbfc_data[10] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,0x39, 0x30};


	//! checking for received events
	temp_event_map = rsi_ble_app_get_event();
	if (temp_event_map == RSI_FAILURE) {
		//! if events are not received loop will be continued.
		return 0;
	}

	//! if any event is received, it will be served.
		switch(temp_event_map)
		{
			case RSI_APP_EVENT_ADV_REPORT:
			{
				//! advertise report event.

				//! clear the advertise report event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_ADV_REPORT);
                status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_bd_addr);
                if(status != RSI_SUCCESS)
                {
                    LOG_PRINT ("connect status: 0x%X\r\n", status);
                }
            }
			break;

			case RSI_APP_EVENT_CONNECTED:
			{
				//! remote device connected event

				//! clear the connected event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CONNECTED);

        //!
        //rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_app_connected_device.dev_addr);
        
        //! initiated the CBFC connection request.
        rsi_ble_cbfc_connreq (rsi_app_connected_device.dev_addr, RSI_BLE_PSM_VALUE);
			}
			break;

			case RSI_APP_EVENT_DISCONNECTED:
			{
				//! remote device disconnected event

				//! clear the disconnected event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_DISCONNECTED);
                device_found=0;
                //! start scanning
                status = rsi_ble_start_scanning();
                if(status != RSI_SUCCESS)
                {
                    LOG_PRINT ("start_scanning status: 0x%X\r\n", status);
                }

            }
			break;

      case RSI_APP_EVENT_CBFC_CONN_REQ:
      {
				//! clear the CBFC connection request event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CBFC_CONN_REQ);
      }
      break;

      case RSI_APP_EVENT_CBFC_RX_DATA:
      {
				//! clear the CBFC data receive event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CBFC_RX_DATA);
      }
      case RSI_APP_EVENT_CBFC_CONN_CMPL:
      {
				//! clear the CBFC connection completed status event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CBFC_CONN_CMPL);

        //! convert the 6 byte remote BD-address to ascii BD-address
        //rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_app_cbfc_conn_cmpl.dev_addr);

        for (ix = 0; ix < sizeof (cbfc_data); ix++)
        {
          cbfc_data[ix]++;
          if (cbfc_data[ix] >= 0x3a) {
            cbfc_data[ix] = 0x30;
          }
        }
        //! send the CBFC data TX with length of 10 bytes.
        rsi_ble_cbfc_data_tx (rsi_app_cbfc_conn_cmpl.dev_addr, rsi_app_cbfc_conn_cmpl.lcid, sizeof (cbfc_data), cbfc_data);
      }
      break;

      case RSI_APP_EVENT_CBFC_DISCONN:
      {
				//! clear the CBFC disconnected event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CBFC_DISCONN);
      }
      break;

		}
	return 0;
}
#endif
