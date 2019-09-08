/**
 * @file    main.c
 * @version 0.1
 * @date    15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains application to explain bridging between wlab and ble tasks
 *
 *  @section Description  This file contains initialization , wlan and ble tasks execution in loop
 *
 *
 */

#include "rsi_driver.h"

//! Memory length for driver
#define GLOBAL_BUFF_LEN    10000

//! Wlan access point mode 
#define RSI_WLAN_AP_MODE    6

//! BTLE Coex mode 
#define RSI_WLAN_BT_CLASSIC_LE_MODE           9

//! Parameter to run forever loop
#define RSI_FOREVER             1 

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Function prototypes
extern void rsi_wlan_ap_app_task(void);
extern void rsi_ble_app_init (void);
extern void rsi_ble_app_task (void);

int32_t rsi_wlan_ap_bt_ble_app(void)
{
  int32_t     status = RSI_SUCCESS;

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
  status = rsi_wireless_init(RSI_WLAN_AP_MODE, RSI_WLAN_BT_CLASSIC_LE_MODE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! BLE initialization
  rsi_ble_app_init();

  //! BLE initialization
  rsi_bt_app_init();

  while(RSI_FOREVER)
  {
    //! WLAN application tasks
    rsi_wlan_ap_app_task();

    //! BLE application tasks
    rsi_bt_app_task();

    //! BLE application tasks
    rsi_ble_app_task();

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

#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

  //! Call WLAN BLE application
  status = rsi_wlan_ap_bt_ble_app();

  //! Application main loop
  main_loop();

  return status;

}
