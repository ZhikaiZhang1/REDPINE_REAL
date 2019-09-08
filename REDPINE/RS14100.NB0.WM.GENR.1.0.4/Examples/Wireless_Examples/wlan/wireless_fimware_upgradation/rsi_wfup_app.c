/**
 * @file    rsi_wfup_app.c
 * @version 0.7
 * @date    31 March 2017
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for wireless firmware upgrade functionality 
 *
 *  @section Description  This file contains example application for wireless firmware upgrade functionality 
 *
 *
 */

/**
 * Include files
 * */
//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_nwk.h"
#include "rsi_wlan.h"


//! configurattion Parameters

//! Access point SSID to be created
#define SSID              "REDPINE_AP"

//!Channel number 
#define CHANNEL_NO             2

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Encryption type
#define ENCRYPTION_TYPE   RSI_NONE

//! Password
#define PSK               NULL

//! Beacon interval
#define  BEACON_INTERVAL        100

//! DTIM interval 
#define DTIM_INTERVAL           4

//! Flags
#define FLAGS     WEB_PAGE_ASSOCIATED_TO_JSON 

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP              0x010AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY                0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK                0x00FFFFFF 

#define   RSI_WLAN_RSP_WIRELESS_FWUP_OK     0x59
#define   RSI_WLAN_RSP_WIRELESS_FWUP_DONE   0x5A

#define   RSI_FWUP_RSP_OK              1
#define   RSI_FWUP_RSP_DONE            2    
#define   RSI_FWUP_RSP_FAILED          3

//! Memory length for driver
#define GLOBAL_BUFF_LEN        10000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500


//! standard defines
uint8_t  rsp_received;

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Function prototypes
void main_loop(void);

//! Scan submit loop
int32_t wfup_loop(void);

//! wireless firmware upgrade handler 
void rsi_wireless_fw_upgrade_handler(uint8_t type, uint32_t status);


//! To register HTTP server related callbacks
void rsi_http_server_cbs_init(void)
{
  //! Register wirless firmware upgrade callback
  rsi_wlan_nwk_register_wireless_fw_upgrade_cb(RSI_WLAN_NWK_FW_UPGRADE_CB, rsi_wireless_fw_upgrade_handler);
}


//! wfup Application 
int32_t rsi_wfup_app()
{
  int32_t     status       = RSI_SUCCESS;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#endif
  
  //! WC initialization
  status = rsi_wireless_init(6, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  
  //! register HTTP server call backs
  rsi_http_server_cbs_init();

 
  //! Configure IP 
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

 
  
  //! Start Access point
  status =  rsi_wlan_ap_start((int8_t *)SSID, CHANNEL_NO, SECURITY_TYPE, ENCRYPTION_TYPE, PSK, BEACON_INTERVAL, DTIM_INTERVAL); 
  if(status != RSI_SUCCESS)
  {

    return status;
  }

  //! wait for the success response 
  status = wfup_loop();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

	return status;
  /////////////////////////////////////////////////////////////////////////////////////
  /////////// Note: After completion of the successful firmware upgradation ///////////
  //////////////// Module has to be rebooted //////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////
}



int32_t wfup_loop(void)
{
  int32_t     status       = RSI_SUCCESS;

  while(1)
  {
#ifndef RSI_WITH_OS
    //! event loop 
    rsi_wireless_driver_task();
#endif

    //! Check for firmware upgrade request ok
    if(rsp_received == RSI_FWUP_RSP_OK)
    {
      //! Send wireless firmware upgrade request
      status = rsi_req_wireless_fwup();
      if(status != RSI_SUCCESS)
      {
        return status;
      }

      rsp_received = 0;
    }

    //! Check for firmware upgrade success
    if(rsp_received == RSI_FWUP_RSP_DONE)
    {
      rsp_received = 0;

      return status;
    }

    //! Check for error response
    if(rsp_received == RSI_FWUP_RSP_FAILED)
    {
      rsp_received = 0;

      //! Get error status
      status = rsi_wlan_get_status();

      return status;
    }
  }
}


void main_loop(void)
{
  while(1)
  {
    ////////////////////////
    //! Application code ///
    ////////////////////////

    //! event loop 
    rsi_wireless_driver_task();

    if(rsp_received)
    {
      rsp_received = 0;
      break;
    }

  }
}

int main()
{
  int32_t status;
 #ifdef RSI_WITH_OS

   rsi_task_handle_t wlan_task_handle = NULL;

   rsi_task_handle_t driver_task_handle = NULL;
 #endif

   #ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

  //! Driver initialization
   status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
   if((status < 0) || (status > GLOBAL_BUFF_LEN))
   {
     return status;
   }

   //! Redpine module intialisation
   status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
   if(status != RSI_SUCCESS)
   {
     return status;
   }

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_wfup_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call  WFUP application
  status = rsi_wfup_app();

  //! Application main loop
  main_loop();
#endif
  return status;

}


//! wireless firmware upgrade handler 
void rsi_wireless_fw_upgrade_handler(uint8_t type, uint32_t status)
{
  if(status == RSI_SUCCESS)
  {
    switch(type)
    {

      case RSI_WLAN_RSP_WIRELESS_FWUP_OK:
        {
          rsp_received = RSI_FWUP_RSP_OK;
#ifdef RSI_ENABLE_DEBUG_PRINT
          printf("\n__________________ FWUPOK _______________\n");
#endif
        }
        break;
      case RSI_WLAN_RSP_WIRELESS_FWUP_DONE:
        {
          rsp_received = RSI_FWUP_RSP_DONE;
#ifdef RSI_ENABLE_DEBUG_PRINT
          printf("\n__________________ FWUP SUCCESS _____________\n");
#endif
        }
        break;
    }
  }
  else
  {
    rsp_received = RSI_FWUP_RSP_FAILED;

    //! Set error status
    rsi_wlan_set_status(status);

#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n__________________ FWUP FAILED _______________\n");
#endif
  }

  return;
}
