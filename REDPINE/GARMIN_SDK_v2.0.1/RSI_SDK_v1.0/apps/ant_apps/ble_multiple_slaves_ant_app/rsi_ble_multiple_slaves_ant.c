/**
 * @file    rsi_ble_multiple_slaves_ant.c
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
 *  @brief : This file contains example application for BLE Central + ANT coex mode.
 *
 *  @section Description  This application connects as a Central/Master.
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
#include<rsi_ant.h>
#include<rsi_bt_common.h>

#include <string.h>
//! Common include file 
#include<rsi_common_apis.h>

//! application defines 
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT
#endif

//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE          LE_PUBLIC_ADDRESS

#ifdef SINGLE_SLAVE
//! Address of the device to connect
#define RSI_BLE_DEV_ADDR               "00:1B:DC:07:2C:F0"
#else
//! Address of the devices to connect
#define RSI_BLE_DEV_1_ADDR      "00:1E:7C:25:E9:4D"
#define RSI_BLE_DEV_2_ADDR      "00:1B:DC:07:31:E5"
#define RSI_BLE_DEV_3_ADDR      "00:1A:7D:DA:71:0A"
#define RSI_BLE_DEV_4_ADDR      "C0:FF:EE:C0:FF:EE"
#define RSI_BLE_DEV_5_ADDR      "00:1A:7D:DA:71:15"
#define RSI_BLE_DEV_6_ADDR      "00:1A:7D:DA:72:13"
#define RSI_BLE_DEV_7_ADDR      "00:1A:7D:DB:71:21"
#define RSI_BLE_DEV_8_ADDR      "00:1A:7D:DA:71:0C"

//! Maximum number of multiple slaves supported.
#define MAX_NUM_OF_SLAVES       2

//! Maximum number of advertise reports to hold
#define NO_OF_ADV_REPORTS              8

//! application event list
#define  RSI_BLE_ADV_REPORT_EVENT       0x00
#define  RSI_BLE_CONN_EVENT             0x01
#define  RSI_BLE_DISCONN_EVENT          0x02
#define  RSI_BLE_SCAN_RESTART_EVENT     0x03

//! Enumeration for commands used in application
typedef enum rsi_app_cmd_e
{
  RSI_SLAVE1_DATA               = 0,
  RSI_SLAVE2_DATA               = 1,
  RSI_SLAVE3_DATA               = 2,
  RSI_SLAVE4_DATA               = 3,
  RSI_SLAVE5_DATA               = 4,
  RSI_SLAVE6_DATA               = 5,
  RSI_SLAVE7_DATA               = 6,
  RSI_SLAVE8_DATA               = 7,
}rsi_app_cmd_t;
#endif

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME         "REDPINE_DEV"

//! Memory length for the driver
#define BT_GLOBAL_BUFF_LEN             10000

//! Memory to initialize the driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! Application supported events list
#define RSI_APP_EVENT_ADV_REPORT       0
#define RSI_APP_EVENT_CONNECTED        1
#define RSI_APP_EVENT_DISCONNECTED     2

#define DISABLE                       0
#define ENABLE                        1

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE  1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY   1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  3000

void rsi_wireless_driver_task(void);

#endif
//! Application global parameters.
static uint16_t rsi_app_no_of_adv_reports_rcvd = 0;
static uint8_t rsi_app_async_event_map = 0;
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
uint8_t conn_dev_addr[18] = {0};
static uint8_t device_found=0;

uint16_t num_of_conn_slaves = 0;
uint16_t conn_req_pending = 0;
static uint16_t rsi_scan_in_progress;

static rsi_ble_event_conn_status_t     rsi_app_connected_device;
static rsi_ble_event_disconnect_t      rsi_app_disconnected_device;
static rsi_ant_atm_mode_t atm_mode;
static rsi_ant_get_ver_t  ant_get_version;
static rsi_ant_get_stats_t ant_get_stats;                  
static uint8_t rsi_ant_get_ver_resp[6] = {0};
static uint8_t rsi_ant_get_stats[16] = {0};


static rsi_ble_event_adv_report_t   rsi_app_adv_reports_to_app[NO_OF_ADV_REPORTS];
static rsi_ble_event_conn_status_t  conn_event_to_app[3];
static rsi_ble_event_disconnect_t   disconn_event_to_app;
static rsi_ble_event_write_t        app_ble_write_event;

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

#ifdef SINGLE_SLAVE  
  if (device_found == 1) {
    return;
  }

  BT_LE_ADPacketExtract (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
  if((device_found==0) && ((strcmp(remote_name, RSI_REMOTE_DEVICE_NAME))==0) ||
      ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)&&((strcmp(remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  { 
    device_found=1;
    rsi_ble_app_set_event (RSI_APP_EVENT_ADV_REPORT);
  }
#else
  uint8_t  str_adv_addr[18] = {0};

  memcpy(&rsi_app_adv_reports_to_app[(rsi_app_no_of_adv_reports_rcvd)% (NO_OF_ADV_REPORTS)], adv_report, sizeof(rsi_ble_event_adv_report_t));
  rsi_app_no_of_adv_reports_rcvd++;
  if(rsi_app_no_of_adv_reports_rcvd == NO_OF_ADV_REPORTS)
  {
    rsi_app_no_of_adv_reports_rcvd = 0;
    LOG_PRINT ("restart scan\r\n");
    rsi_ble_app_set_event (RSI_BLE_SCAN_RESTART_EVENT);
  }
  rsi_6byte_dev_address_to_ascii ((int8_t *)str_adv_addr, (uint8_t *)adv_report->dev_addr);	
  LOG_PRINT ("\n DEV_ADDR: %s\r\n", str_adv_addr);
  if ((!strcmp (RSI_BLE_DEV_1_ADDR, (char *)str_adv_addr)) || 
      (!strcmp (RSI_BLE_DEV_2_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_3_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_4_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_5_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_6_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_7_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_8_ADDR, (char *)str_adv_addr))) {
    memcpy (conn_dev_addr, str_adv_addr, sizeof (str_adv_addr));
    memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);	
    rsi_ble_app_set_event (RSI_BLE_ADV_REPORT_EVENT);
  }

#endif  
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_central_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_central_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
#ifdef SINGLE_SLAVE  
	rsi_app_connected_device.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (rsi_app_connected_device.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  rsi_app_connected_device.status = resp_enh_conn->status;
	rsi_ble_app_set_event (RSI_APP_EVENT_CONNECTED);
#else
  uint8_t str_conn_device[18] = {0};

  rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_device, (uint8_t *)resp_enh_conn->dev_addr);	
  if(!strcmp((char *)str_conn_device, RSI_BLE_DEV_1_ADDR)) 
  {
    conn_event_to_app[0].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[0].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[0].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_2_ADDR)) 
  {
    conn_event_to_app[1].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[1].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[1].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_3_ADDR))
  {
    conn_event_to_app[2].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[2].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[2].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_4_ADDR))
  {
    conn_event_to_app[3].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[3].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[3].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_5_ADDR))
  {
    conn_event_to_app[4].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[4].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[4].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_6_ADDR))
  {
    conn_event_to_app[5].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[5].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[5].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_7_ADDR))
  {
    conn_event_to_app[6].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[6].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[6].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_8_ADDR))
  {
    conn_event_to_app[7].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[7].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[7].status = resp_enh_conn->status;
  }
  rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
#endif    
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
#ifdef SINGLE_SLAVE  
	memcpy(&rsi_app_connected_device, resp_conn, sizeof(rsi_ble_event_conn_status_t));

	rsi_ble_app_set_event (RSI_APP_EVENT_CONNECTED);
#else
  uint8_t str_conn_device[18] = {0};

  rsi_6byte_dev_address_to_ascii ((int8_t *)str_conn_device, (uint8_t *)resp_conn->dev_addr);	
  if(!strcmp((char *)str_conn_device, RSI_BLE_DEV_1_ADDR)) 
  {
    memcpy(&conn_event_to_app[0], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_2_ADDR)) 
  {
    memcpy(&conn_event_to_app[1], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_3_ADDR))
  {
    memcpy(&conn_event_to_app[2], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_4_ADDR))
  {
    memcpy(&conn_event_to_app[3], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_5_ADDR))
  {
    memcpy(&conn_event_to_app[4], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_6_ADDR))
  {
    memcpy(&conn_event_to_app[5], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_7_ADDR))
  {
    memcpy(&conn_event_to_app[6], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_8_ADDR))
  {
    memcpy(&conn_event_to_app[7], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
#endif    
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
#ifdef SINGLE_SLAVE  
	memcpy(&rsi_app_disconnected_device, resp_disconnect, sizeof(rsi_ble_event_disconnect_t ));

	rsi_ble_app_set_event (RSI_APP_EVENT_DISCONNECTED);
#else
  uint8_t str_disconn_device[18] = {0};

  rsi_6byte_dev_address_to_ascii ((int8_t *)str_disconn_device, (uint8_t *)resp_disconnect->dev_addr);	
  if(!strcmp((char *)str_disconn_device, RSI_BLE_DEV_1_ADDR)) 
  {
    memset(&conn_event_to_app[0], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_2_ADDR)) 
  {
    memset(&conn_event_to_app[1], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_3_ADDR))
  {
    memset(&conn_event_to_app[2], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_4_ADDR))
  {
    memset(&conn_event_to_app[3], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_5_ADDR))
  {
    memset(&conn_event_to_app[4], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_6_ADDR))
  {
    memset(&conn_event_to_app[5], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_7_ADDR))
  {
    memset(&conn_event_to_app[6], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_8_ADDR))
  {
    memset(&conn_event_to_app[7], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
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

int32_t rsi_ble_app_task (void)
{
  int32_t status = 0;
  int32_t  event_id;
  int8_t   state, data[20] = {0};
  uuid_t   search_serv;
  rsi_ble_resp_local_att_value_t  local_att_val;
  static uint8_t loop = 0;
  uint16_t  slave_val;

  //! checking for events list
  event_id = rsi_ble_app_get_event ();
  if (event_id == -1) {
    return -1;
  }

  switch (event_id) {
    case RSI_BLE_ADV_REPORT_EVENT:
      {
        LOG_PRINT("\n Advertise report received \n");
        if(conn_req_pending == 0)
        {
          if(num_of_conn_slaves < MAX_NUM_OF_SLAVES)
          {
            if((!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_1_ADDR)) || 
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_2_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_3_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_4_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_5_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_6_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_7_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_8_ADDR)))
            {
              LOG_PRINT("\n Device found. Stop scanning \n");
              status = rsi_ble_stop_scanning();
              rsi_scan_in_progress = 0;
              if (status != RSI_SUCCESS)
              {
                return status;
              }
              LOG_PRINT("\n Connect command \n");
              status = rsi_ble_connect(RSI_BLE_DEV_ADDR_TYPE, (int8_t *)remote_dev_bd_addr);
              if(status != RSI_SUCCESS)
              {
                return status;
              }
              conn_req_pending = 1;
            }
          }
        }
        //! clear the advertise report event.
        rsi_ble_app_clear_event (RSI_BLE_ADV_REPORT_EVENT);
      }
      break;
    case RSI_BLE_CONN_EVENT:
      {
        num_of_conn_slaves++;

        LOG_PRINT("\n Device connected \n ");
        LOG_PRINT("\n Number of devices connected:%d \n", num_of_conn_slaves);

        conn_req_pending = 0;

        rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);

        if(num_of_conn_slaves < MAX_NUM_OF_SLAVES)
        {
          LOG_PRINT("\n Start scanning \n");
          rsi_app_no_of_adv_reports_rcvd = 0;
          status = rsi_ble_start_scanning();
          rsi_scan_in_progress = 1;
          if(status != RSI_SUCCESS)
          {
            return status;
          }
        }

#if BLE_PS_ENABLE
        else if(num_of_conn_slaves == MAX_NUM_OF_SLAVES)
        {
          LOG_PRINT("\n %d slaves connected \n", MAX_NUM_OF_SLAVES);
          LOG_PRINT("\n Power save Enable command \n");

          status = rsi_bt_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
        }
#endif        
      }
      break;

    case RSI_BLE_DISCONN_EVENT:
      {
        num_of_conn_slaves--;

        LOG_PRINT("\n Device disconnected\n ");
        LOG_PRINT("\n Number of connected devices:%d\n", num_of_conn_slaves);

#if BLE_PS_ENABLE
        LOG_PRINT("\n Power save Enable command \n");

        status = rsi_bt_power_save_profile(RSI_ACTIVE, PSP_TYPE);
        if(status != RSI_SUCCESS)
        {
          return status;
        }
#endif
        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
        
scan: 
        if(!rsi_scan_in_progress) 
        {
          rsi_app_no_of_adv_reports_rcvd = 0;
          status = rsi_ble_start_scanning();
          rsi_scan_in_progress = 1;
          if((status != RSI_SUCCESS) )
          {
            goto scan;
          }
        }
      }
      break;

    case RSI_BLE_SCAN_RESTART_EVENT:
      //! clear the served event
      rsi_ble_app_clear_event (RSI_BLE_SCAN_RESTART_EVENT);

      LOG_PRINT("\n Device found. Stop scanning \n");
      status = rsi_ble_stop_scanning();
      rsi_scan_in_progress = 0;
      if (status != RSI_SUCCESS)
      {
        return status;
      }
      
      LOG_PRINT("\n Start scanning \n");
      rsi_app_no_of_adv_reports_rcvd = 0;
      status = rsi_ble_start_scanning();
      rsi_scan_in_progress = 1;
      if(status != RSI_SUCCESS)
      {
        return status;
      }
      break;

  }

  return 0;
}


/*==============================================*/
/**
 * @fn         rsi_ble_central
 * @brief      Tests the BLE GAP central role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the BLE central role and simple GAP API's.
 */
int32_t rsi_ble_central(void)
{
	int32_t status = 0;
	int32_t temp_event_map = 0;

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

#ifdef RSI_UART_FLOW_CTRL_ENABLE
	rsi_cmd_uart_flow_ctrl (ENABLE); 
#endif
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

	//! initialize the event map
	rsi_ble_app_init_events ();

  ant_get_version.cmd_ix = ANT_GET_VERSION;

  //! get the ANT Stack Local Version.
  status = rsi_ant_send_cmd(&ant_get_version, rsi_ant_get_ver_resp);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  atm_mode.cmd_ix = ANT_ATM_CMD;
  atm_mode.flags = 0x01;
  status = rsi_ant_send_cmd (&atm_mode,rsi_ant_get_stats);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! start scanning
  status = rsi_ble_start_scanning();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
  ant_get_stats.cmd_ix = ANT_GET_STATS;
  //! get the ANT Stack Local Version.
  status = rsi_ant_send_cmd(&ant_get_stats, rsi_ant_get_stats);
  if(status != RSI_SUCCESS)
  {
    return status;
  }


  while(1)
  {
    //! Application main loop
#ifndef RSI_WITH_OS
		//! run the non os scheduler
		rsi_wireless_driver_task();
#endif
#ifndef SINGLE_SLAVE        
        rsi_ble_app_task();
#else        
		//! checking for received events
		temp_event_map = rsi_ble_app_get_event ();
		if (temp_event_map == RSI_FAILURE) {
			//! if events are not received, loop will be continued
			continue;
		}

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
    }
#endif        
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
  int32_t status;
#ifdef RSI_WITH_OS
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
  
  //! Call BLE Central application
  status = rsi_ble_central();

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
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_ble_central, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
