/**
 * @file    main.c
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
 *  @brief : This file contains example application for BT Dual mode
 *
 *  @section Description  This file contains example application for BT Dual mode 
 *
 *
 */

#include "rsi_driver.h"
#include "rsi_ant.h"

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN            10000


//! Parameter to run forever loop
#define RSI_FOREVER             1 

//! application defines 
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                      printf
#else 
#define LOG_PRINT                      
#endif
//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

extern rsi_ant_atm_mode_t atm_mode;
extern rsi_ant_get_ver_t  ant_get_version;
extern rsi_ant_get_stats_t ant_get_stats;                  
extern uint8_t rsi_ant_get_ver_resp[6];
extern uint8_t rsi_ant_get_stats[16];
//! Function prototypes
extern uint32_t rsi_bt_a2dp_source  (void);
#ifdef BLE_CENTRAL
extern uint32_t rsi_ble_central_app_init();
#else
extern uint32_t rsi_ble_app_init (void);
#endif
extern void rsi_bt_a2dp_souce_task  (void);
#ifdef BLE_CENTRAL
extern uint32_t rsi_ble_central_app_task();
#else
extern void rsi_ble_app_task (void);
#endif

uint8_t ble_connected=0;
uint8_t bt_a2dp_connect=0;

int32_t rsi_bt_ble_app(void)
{
  int32_t     status = RSI_SUCCESS;

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
  LOG_PRINT ("\n FIRMWARE IMAGE LOADING .... \n");
  //! WiSeConnect initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  LOG_PRINT ("\n FIRMWARE IMAGE LOADING DONE.\n");
  LOG_PRINT ("\n BT/A2DP INTIALIZATION GONING ON ....\n");
  //! BT initialization
  status = rsi_bt_a2dp_source();
  if(status != RSI_SUCCESS)
  {
  	LOG_PRINT ("\n BT/A2DP INTIALIZATION ERROR/Fails\n");
    return status;
  }
#if 1
  LOG_PRINT ("\n ANT APP INTIALIZATION GONING ON....\n");
  status = rsi_ble_ant_app();
  if(status != RSI_SUCCESS)
  {
  	LOG_PRINT ("\n ANT APP INTIALIZATION ERROR/Fails\n");
    return status;
  }
#endif
#ifdef BLE_ENABLE
  LOG_PRINT ("\n BLE INTIALIZATION GONING ON....\n");
#ifdef BLE_CENTRAL
	status = rsi_ble_central_app_init();
#else
  //! BLE initialization
  status = rsi_ble_app_init();
#endif	
  if(status != RSI_SUCCESS)
  {
  	LOG_PRINT ("\n BLE  INTIALIZATION ERROR/Fails\n");
    return status;
  }
#endif	
#if 0
  LOG_PRINT ("\n ANT APP INTIALIZATION GONING ON....\n");

  status = rsi_ble_ant_app();
  if(status != RSI_SUCCESS)
  {
  	LOG_PRINT ("\n ANT APP INTIALIZATION ERROR/Fails\n");
    return status;
  }
#endif
  LOG_PRINT ("\n BLE INTIALIZATION COMPLETED SUCCESS\n");
  while(RSI_FOREVER)
  {
#ifdef BLE_ENABLE
    //! BLE application tasks
#ifdef BLE_CENTRAL
    //! BLE Central application tasks
		rsi_ble_central_app_task();
#else
    //! BLE Peripheral application tasks
    rsi_ble_app_task();
#endif		
#endif		
    //! BLE application tasks
    rsi_bt_a2dp_source_task();

    //! wireless driver tasks
    rsi_wireless_driver_task();

    //! BLE application tasks
    rsi_ble_ant_app_task();

  }
  return 0;
}

//! Forever in wireless driver task 
void main_loop()
{
  while (RSI_FOREVER)
  {
    rsi_wireless_driver_task ();
  }
}

//! main funtion definition
int main(void)
{
  int32_t status;

#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

  //Start BT-BLE Stack
  intialize_bt_stack(STACK_BT_MODE);

  //! Call BT BLE application
  status = rsi_bt_ble_app();

  //! Application main loop
  main_loop();

  return status;

}
