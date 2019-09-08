/**
 * @file    rsi_wake_on_wireless.c
 * @version 1.0
 * @date    12 Mar 2018
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for power save.
 *  @section Description  This file contains example application for power save
 *
 *
 */
/*================================================================================
	=================================================================================*/
/**
 * Include files
 * */

#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! Hardware and powersave related include files
#include "rsi_board.h"
#include "rsi_chip.h"
#include "rsi_ps_ram_func.h"
#include "rsi_wisemcu_hardware_setup.h"
#include "rsi_m4.h"

//! Access point SSID to connect
#define SSID                  "REDPINE_AP"

#define SCAN_CHANNEL          0

//! Security type
#define SECURITY_TYPE         RSI_OPEN

//! Password
#define PSK                   ""

//! Power Save Profile Mode
#define PSP_TYPE              RSI_MAX_PSP

//! Runs scheduler for some delay and do not send any command
#define RSI_DELAY                   6000000

//! Memory length for driver
#define GLOBAL_BUFF_LEN             15000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY      1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY    2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE    500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

#define IVT_OFFSET_ADDR            0x8012000  /*<!Application IVT location VTOR offset        */
#define WKP_RAM_USAGE_LOCATION     0x24061000 /*<!Bootloader RAM usage location upon wake up  */

#define WIRELESS_WAKEUP_IRQHandler  NPSS_TO_MCU_WIRELESS_INTR_IRQn

void M4_sleep_wakeup()
{

	/* LDOSOC Default Mode needs to be disabled */
	RSI_PS_LdoSocDefaultModeDisable();

	/* bypass_ldorf_ctrl needs to be enabled */
	RSI_PS_BypassLdoRfEnable();

	RSI_PS_FlashLdoDisable();

	/* Configure RAM Usage and Retention Size */
	RSI_WISEMCU_ConfigRamRetention(WISEMCU_48KB_RAM_IN_USE, WISEMCU_RETAIN_DEFAULT_RAM_DURING_SLEEP);

	/* Trigger M4 Sleep */
	RSI_WISEMCU_TriggerSleep(SLEEP_WITH_RETENTION, DISABLE_LF_MODE, 0, 
			(uint32_t)RSI_PS_RestoreCpuContext, 0, RSI_WAKEUP_WITH_RETENTION_WO_ULPSS_RAM);	
}

void HardFault_Handler()
{
	while(1)
	{
		DEBUGOUT("HardFault_Handler\n");
	}
}

int32_t rsi_powersave_profile_app()
{
	int32_t     status       = RSI_SUCCESS;

	DEBUGOUT("\n Wireless init \n");

	//! WC initialization
	status = rsi_wireless_init(0, 0);
	if(status != RSI_SUCCESS)
	{
		DEBUGOUT("\n opermode Failed 0x%x \n", status);
		return status;
	}
	DEBUGOUT("\n Opermode success \n");

	//! Send feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		DEBUGOUT("\n Feature frame sent failed 0x%x \n", status);
		return status;
	}
	DEBUGOUT("\n Featutre frame sent success \n");


	//! Scan for the Access Point
	do
	{
		DEBUGOUT("\n SCAN  \n"); 
		status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)SCAN_CHANNEL, NULL, 0);  
		if(status != RSI_SUCCESS)
		{
			DEBUGOUT("\n Scan  Failed 0x%x \n", status);
		}
		else
		{
			DEBUGOUT("\n SCAN success \n");
		}
	}while(status != 0);


	//! Connect to an Acces point
	status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
	if(status != RSI_SUCCESS)
	{
		DEBUGOUT("\n Join  Failed 0x%x \n", status);
		return status;
	}

	DEBUGOUT("\n JOIN success \n"); 

	DEBUGOUT("\n powersave cmd \n");

	//! Apply power save profile with deep sleep
	status = rsi_wlan_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
	if(status != RSI_SUCCESS)
	{
		DEBUGOUT("\n powersave Failed 0x%x \n", status);
		return status;
	}	

	//! Keep M4 in sleep
	M4_sleep_wakeup();

	return status;
}

void app_task()
{
	////////////////////////
	//! Application code ///
	////////////////////////
	return;
}

void main_loop(void)
{
	while(1)
	{
		//! Application task
		app_task();

		//! event loop 
		rsi_wireless_driver_task();
		if(!(P2P_STATUS_REG & TA_wakeup_M4))
		{
			M4_sleep_wakeup();
		}
	}
}

int main()
{
	int32_t status , cntr;

	/*Configures the system default clock and power configurations*/
	SystemCoreClockUpdate();

	/* MCU Hardware Configuration for Low-Power Applications */
	RSI_WISEMCU_HardwareSetup();

	/*Enable the debug prints int */
	DEBUGINIT();
	DEBUGOUT("In main M4 PS\n");	

#ifdef RSI_WITH_OS

	rsi_task_handle_t wlan_task_handle = NULL;

	rsi_task_handle_t driver_task_handle = NULL;
#endif

#ifndef RSI_SAMPLE_HAL
	//! Board Initialization
	Board_init();
#endif
	DEBUGOUT("In rsi_driver_init \n");

	//! Driver initialization
	status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
	if((status < 0) || (status > GLOBAL_BUFF_LEN))
	{
		DEBUGOUT("In rsi_driver_init failed\n");
		return status;
	}

	DEBUGOUT("\n In rsi_device_init \n");

	//! Redpine module intialisation
	status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
	if(status != RSI_SUCCESS)
	{
		DEBUGOUT("In rsi_device_init failed\n");
		return status;
	}

#ifdef RSI_WITH_OS
	//! OS case
	//! Task created for WLAN task
	rsi_task_create(rsi_powersave_profile_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! CallPower save application
	status = rsi_powersave_profile_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}

