/**
 * @file    rsi_puf_key_app.c
 * @version 1.0
 * @date    03 APR 2018
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for using PUF and Key Usage with PUF
 *
 *  @section Description  This file contains example application for PUF usage and PUF key management
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

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! Include PUF declarations File
#include "rsi_puf.h"

#ifdef RSI_ENABLE_DEBUG_PRINT
#include <stdio.h>
#endif

//! Receive data length
#define RECV_BUFFER_SIZE   1027

//! Memory length for driver
#define GLOBAL_BUFF_LEN   12000

//! Wlan task priority
#define RSI_PUF_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_PUF_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Enable to configure enroll disable
#define RSI_PUF_ENROLL_DIS			DISABLE

//! Enable to configure set_key disable
#define RSI_PUF_SET_KEY_DIS			DISABLE

//! Enable to configure get_key disable
#define RSI_PUF_GET_KEY_DIS			DISABLE

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//!App to use PUF Configuration, like enroll and to disable enroll/set_key/get_key operations
int32_t rsi_puf_config_app()
{
	int32_t     status       = RSI_SUCCESS;
	//Output buffers for app

	//! WC initialization
	status = rsi_wireless_init(0, 0);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! Enroll PUF for the first Time
	status = rsi_puf_enroll_req();
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n PUF Start Enroll Fail/Blocked Status %x \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n PUF Enroll Success \n");
#endif

#if RSI_PUF_ENROLL_DIS
	//! Disable Enroll
	status = rsi_puf_enroll_disable_req();
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n Disable Enroll Fail Status %d \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n PUF Further Enroll Disabled \n");
#endif
#endif

#if RSI_PUF_SET_KEY_DIS
	//! Disable set key
	status = rsi_puf_set_key_disable_req();
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n Disable Set Key Fail Status %d \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n PUF Set Key Disabled \n");
#endif
#endif

#if RSI_PUF_GET_KEY_DIS
	//! Disable Get Key
	status = rsi_puf_get_key_disable_req();
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n Disable Get Key Fail Status %d \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n PUF Get Key Disabled \n");
#endif
#endif

	return 0;
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
	}
}

int main()
{
	int32_t status;

#ifdef RSI_WITH_OS
	rsi_task_handle_t puf_task_handle = NULL;

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
	//! Task created for PUF task
	rsi_task_create( rsi_puf_config_app, "puf_task", RSI_PUF_TASK_STACK_SIZE, NULL, RSI_PUF_TASK_PRIORITY, &puf_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task", RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call PUF configuration application
	status = rsi_puf_config_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}
