/**
 * @file    main.c
 * @version 0.1
 * @date    15 Jun 2018
 *
 *  Copyright(C) Redpine Signals 2018
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application of coex power save application.
 *
 *  @section Description  This file contains example application for coex power save application in wlan+bt combination. 
 *
 *
 */

#include "rsi_driver.h"

//! Memory length for driver
#define GLOBAL_BUFF_LEN    10000

//! Wlan client mode 
#define RSI_WLAN_CLIENT_MODE    0

//! BT Coex mode 
#define RSI_BT_MODE           5

//! Parameter to run forever loop
#define RSI_FOREVER             1 

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//int32_t     status = RSI_SUCCESS;

//! Function prototypes
extern void rsi_wlan_app_task(void);
extern void rsi_bt_app_task (void);
extern void rsi_bt_app_init (void);

//int32_t     status = RSI_SUCCESS;


 int32_t     status = RSI_SUCCESS; 

int32_t rsi_wlan_bt_app(void)
{
  

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

  //! WiSeConnect initialization
  status = rsi_wireless_init(RSI_WLAN_CLIENT_MODE, RSI_BT_MODE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! BLE initialization
  rsi_bt_app_init();

  while(RSI_FOREVER)
  {
    
		//! BLE application tasks
    rsi_bt_app_task();
		
		//! WLAN application tasks
    rsi_wlan_app_task();

    //! wireless driver tasks
    rsi_wireless_driver_task();

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
#ifdef RSI_M4_INTERFACE
	/*Configures the system default clock and power configurations*/
	SystemCoreClockUpdate();

	/* MCU Hardware Configuration for Low-Power Applications */
	RSI_WISEMCU_HardwareSetup();
#endif


#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

  //! Call WLAN BLE application
  status = rsi_wlan_bt_app();

  //! Application main loop
  main_loop();

  return status;

}
