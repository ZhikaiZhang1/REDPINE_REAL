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

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN            10000


//! BT Coex mode 
#define RSI_BT_BLE_MODE           9

//! Parameter to run forever loop
#define RSI_FOREVER             1 

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];


//! Global_variables
uint8_t bt_done=0;
uint8_t ble_done=0;
//! Function prototypes
extern void rsi_bt_app_init  (void);
extern void rsi_ble_app_init (void);
extern void rsi_bt_app_task  (void);
extern void rsi_ble_app_task (void);

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

  //! WiSeConnect initialization
  status = rsi_wireless_init(0, RSI_BT_BLE_MODE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! BT initialization
  rsi_bt_app_init();

#if (!(GAP_MOD_NDIS_BV_01_C || GAP_MOD_LDIS_BV_01_C || GAP_MOD_LDIS_BV_02_C || GAP_MOD_LDIS_BV_03_C || GAP_MOD_GDIS_BV_01_C || GAP_MOD_GDIS_BV_02_C || GAP_MOD_NCON_BV_01_C || GAP_MOD_CON_BV_01_C))  
  //! BLE initialization
  rsi_ble_app_init();
#endif

  while(RSI_FOREVER)
  {
    //! BLE application tasks
    rsi_bt_app_task();

#if (!(GAP_MOD_NDIS_BV_01_C || GAP_MOD_LDIS_BV_01_C || GAP_MOD_LDIS_BV_02_C || GAP_MOD_LDIS_BV_03_C || GAP_MOD_GDIS_BV_01_C || GAP_MOD_GDIS_BV_02_C || GAP_MOD_NCON_BV_01_C || GAP_MOD_CON_BV_01_C))  
    //! BLE application tasks
    rsi_ble_app_task();
#endif
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



  //! Call BT BLE application
  status = rsi_bt_ble_app();

  //! Application main loop
  main_loop();

  return status;

}
