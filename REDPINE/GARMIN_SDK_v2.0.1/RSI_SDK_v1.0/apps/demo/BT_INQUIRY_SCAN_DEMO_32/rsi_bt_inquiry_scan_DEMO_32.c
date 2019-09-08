/**
 * @file    rsi_bt_inquiry_scan_DEMO_32.c
 * @version 0.1
 * @date    xx Feb 2019
 *
 *  Copyright(C) Redpine Signals 2019
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BT Classic Inquiry Scan
 *
 *  @section Description  This application serves as a BT Master.
 *
 */

/**
 * Include files
 * */

#include <rsi_common_app.h>
#if BT_INQUIRY_SCAN

#include<rsi_driver.h>
//! BT include file to refer BT APIs
#include<rsi_bt_apis.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_bt_config.h>
#include<rsi_bt.h>
#include <stdio.h>

#define RSI_DID_SPEC_ID                0x0200
#define RSI_DID_VENDOR_ID              0x0201
#define RSI_DID_PRODUCT_ID             0x0202
#define RSI_DID_VERSION                0x0203
#define RSI_DID_PRIMARY_RECOED         0x0001
#define RSI_DID_VENDOR_ID_SOURCE       0x0002

//! Application global parameters.
static rsi_bt_resp_get_local_name_t   local_name = {0};
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
uint8_t power_save_given = 0;

extern int32_t rsi_bt_power_save_profile(uint8_t psp_mode, uint8_t psp_type);



/*==============================================*/
/**
 * @fn         rsi_app_task_bt_inquiry_scan
 * @brief      Keeps the BT Device in inquiry scan mode.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the inquiry scan mode functionality.
 */
int32_t rsi_app_task_bt_inquiry_scan (void)
{
  int32_t status = 0;
  uint8_t str_bd_addr[18] = {0};
  uint8_t eir_data[200] = {2,1,0};


  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(local_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\r\nlocal_bd_address: %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_bd_addr, local_dev_addr));

  //! set the local device name
  status = rsi_bt_set_local_name((int8_t *)RSI_BT_LOCAL_NAME);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! get the local device name
  status = rsi_bt_get_local_name(&local_name);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\r\nlocal_name: %s\r\n", local_name.name);

  //! prepare Extended Response Data
  eir_data[3] = strlen (RSI_BT_LOCAL_NAME) + 1;
  eir_data[4] = 9;
  strcpy ((char *)&eir_data[5], RSI_BT_LOCAL_NAME);
  //! set eir data
  rsi_bt_set_eir_data (eir_data, strlen (RSI_BT_LOCAL_NAME) + 5);


	status = rsi_bt_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
	if (status != RSI_SUCCESS)
	{
		return status;
	}

	//! start the discover mode/iscan mode
		status = rsi_bt_start_discoverable();
		if(status != RSI_SUCCESS)
		{
			return status;
		}

while(1)
  {
    //! Application main loop

#ifndef RSI_WITH_OS
    //! run the non OS scheduler
    rsi_wireless_driver_task();
#endif

  }

  return 0;
}
#endif
