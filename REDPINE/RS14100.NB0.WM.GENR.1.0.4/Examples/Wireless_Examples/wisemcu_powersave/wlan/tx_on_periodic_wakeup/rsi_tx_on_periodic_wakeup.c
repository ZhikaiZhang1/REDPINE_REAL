/**
 * @file    rsi_tx_on_periodic_wakeup.c
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
 *  @section Description  This file contains example application for power save,
 *
 *  NWP will be in connected powersave mode .
 *  M4 wakeup source will be alarm.M4 wakes up at periodic interval,
 *  transmits a wireless packet by waking up NWP and goes back to sleep
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

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE             1 

//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP         0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF 

#endif

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x640AA8C0

//! Server port number
#define SERVER_PORT       5001

//! Device port number
#define DEVICE_PORT       5001

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

#ifdef GPIO_TOGGLE
#define GPIO_LOW                   0U
#define GPIO_HIGH                  1U
#endif

#ifdef ALARM_CONFIG
void InitM4AlarmConfig(void);
static RTC_TIME_CONFIG_T rtcConfig, readTime , alarmConfig , readAlarmConfig; 
#define RTC_ALARM_IRQHandler 	      IRQ028_Handler
#define NVIC_RCT_ALARM              MCU_CAL_ALARM_IRQn

#define ALARAM_PERIODIC_TIME        5    /*<! periodic alarm configuration in SEC */
#endif

#define IVT_OFFSET_ADDR            0x8012000  /*<!Application IVT location VTOR offset        */
#define WKP_RAM_USAGE_LOCATION     0x24061000 /*<!Bootloader RAM usage location upon wake up  */

#define WIRELESS_WAKEUP_IRQHandler  NPSS_TO_MCU_WIRELESS_INTR_IRQn

void M4_sleep_wakeup()
{
	/* Configure Wakeup-Source */
	RSI_PS_SetWkpSources(WIRELESS_BASED_WAKEUP);

#ifdef DEBUG_UART
	RSI_PS_SetWkpSources(ULPSS_BASED_WAKEUP);
#endif	

	/* Enable NVIC */
	NVIC_EnableIRQ(WIRELESS_WAKEUP_IRQHandler);

#ifdef ALARM_CONFIG
	RSI_RTC_GetDateTime(RTC , &readTime);
	DEBUGOUT("\n TA permitted M4 to sleep\n");
	DEBUGOUT("TIME: %d:%d:%d:%d\n",readTime.Hour,readTime.Minute,readTime.Second,readTime.MilliSeconds);
	DEBUGOUT("DATE: %d/%d/%d\n\n",readTime.Day,readTime.Month,(readTime.Year + BASE_YEAR));  
#endif	

#ifdef GPIO_TOGGLE
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_LOW);
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_HIGH);
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_LOW);
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_HIGH);	
#endif			

#ifndef FLASH_BASED_EXECUTION_ENABLE
	
	/* LDOSOC Default Mode needs to be disabled */
	RSI_PS_LdoSocDefaultModeDisable();

	/* bypass_ldorf_ctrl needs to be enabled */
	RSI_PS_BypassLdoRfEnable();

	/* Disable FlashLDO */
	RSI_PS_FlashLdoDisable();		

	/* Configure RAM Usage and Retention Size */
	RSI_WISEMCU_ConfigRamRetention(WISEMCU_128KB_RAM_IN_USE, WISEMCU_RETAIN_DEFAULT_RAM_DURING_SLEEP);

	/* Trigger M4 Sleep */
	RSI_WISEMCU_TriggerSleep(SLEEP_WITH_RETENTION, DISABLE_LF_MODE, 0, 
			(uint32_t)RSI_PS_RestoreCpuContext, 0, RSI_WAKEUP_WITH_RETENTION_WO_ULPSS_RAM);	

#else
	/* Configure RAM Usage and Retention Size */
	RSI_WISEMCU_ConfigRamRetention(WISEMCU_192KB_RAM_IN_USE, WISEMCU_RETAIN_DEFAULT_RAM_DURING_SLEEP);

	/* Trigger M4 Sleep*/
	RSI_WISEMCU_TriggerSleep(SLEEP_WITH_RETENTION, DISABLE_LF_MODE, WKP_RAM_USAGE_LOCATION, 
			(uint32_t)RSI_PS_RestoreCpuContext, IVT_OFFSET_ADDR, RSI_WAKEUP_FROM_FLASH_MODE);
												
#endif
#ifdef DEBUG_UART
     fpuInit();
		 DEBUGINIT();
#endif
#ifdef ALARM_CONFIG	
	RSI_RTC_GetDateTime(RTC , &readTime);  
	DEBUGOUT("TIME: %d:%d:%d:%d\n",readTime.Hour,readTime.Minute,readTime.Second,readTime.MilliSeconds);
	DEBUGOUT("DATE: %d/%d/%d\n\n",readTime.Day,readTime.Month,(readTime.Year + BASE_YEAR));  
#endif		
#ifdef GPIO_TOGGLE
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_LOW);
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_HIGH);
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_LOW);
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_HIGH);
  RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_LOW);
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_HIGH);
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_LOW);
	RSI_NPSSGPIO_SetPin(NPSS_GPIO_3,GPIO_HIGH);	
#endif															
}

void	 data_recv(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
	int i;
	DEBUGOUT("\n ####RX PACKET RECEIVED ####### \n");
	DEBUGOUT("\n RX: LEN %d\n", length);
	DEBUGOUT("\n Data:");

	for(i = 0; i < length; i++)
	{
		DEBUGOUT("%c", buffer[i]);
	}

	DEBUGOUT("\n");
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
	int32_t     client_socket;
	struct      rsi_sockaddr_in server_addr;
	int32_t     status       = RSI_SUCCESS;
	int32_t     packet_count = 0;
#if !(DHCP_MODE)
	uint32_t    ip_addr      = DEVICE_IP;
	uint32_t    network_mask = NETMASK;
	uint32_t    gateway      = GATEWAY;
#endif

	DEBUGOUT("wireless init  \n");
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

	//! Scan for Access Point
	do
	{
		DEBUGOUT("\n SCAN  \n"); 
		status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)SCAN_CHANNEL, NULL, 0);  
		if(status != RSI_SUCCESS)
		{
			DEBUGOUT("\n Scan  Failed 0x%x \n", status);
		}
	}while(status != 0);

	DEBUGOUT("\n SCAN success \n"); 

	//! Connect to an Acces point
	status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
	if(status != RSI_SUCCESS)
	{
		DEBUGOUT("\n Join  Failed 0x%x \n", status);
		return status;
	}
	DEBUGOUT("\n JOIN success \n"); 

	//! Configure IP
#if DHCP_MODE
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0, NULL, 0, 0);
#else
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0, 0);
#endif 
	if(status != RSI_SUCCESS)
	{ 
		DEBUGOUT("\n IPCONFIG  Failed 0x%x \n", status);
		return status;
	}

	DEBUGOUT("\n IP success \n");  					
	DEBUGOUT("\n Socket \n");  	
	//! Create socket
	client_socket = rsi_socket(AF_INET, SOCK_DGRAM, 0);
	if(client_socket < 0)
	{
		DEBUGOUT("\n Socket  Failed 0x%x \n", status);
		status = rsi_wlan_get_status();
		return status;
	}

	//! Set server structure
	memset(&server_addr, 0, sizeof(server_addr));  

	//! Set family type
	server_addr.sin_family= AF_INET;

	//! Set server port number, using htons function to use proper byte order
	server_addr.sin_port = htons(SERVER_PORT);
	//! Set IP address to localhost
	server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

	DEBUGOUT("\n powersave cmd \n");
	
	//! Apply power save profile with deep sleep
	status = rsi_wlan_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
	if(status != RSI_SUCCESS)
	{
		DEBUGOUT("\n powersave Failed 0x%x \n", status);
		return status;
	}
#ifdef ALARM_CONFIG	
	InitM4AlarmConfig();
#endif	
	do
	{
		//! Keep M4 sleep Here,Wake On wirless Here
		M4_sleep_wakeup();
		//! Send data on socket
		status = rsi_sendto(client_socket, (int8_t *)"Hello from UDP client!!!", (sizeof("Hello from UDP client!!!") - 1), 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
		if(status < 0)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(client_socket, 0);
			return status;
		}
		DEBUGOUT("\n packet sent 0x%x \n", status);
		//! Increase packet count
		packet_count++;
		DEBUGOUT("\n packet count 0x%x \n", packet_count);
	}while(1);
	//! Wakeup
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
	int32_t status;

	/*Configures the system default clock and power configurations*/
	SystemCoreClockUpdate();

	/* MCU Hardware Configuration for Low-Power Applications */
	RSI_WISEMCU_HardwareSetup();

	/*Enable the debug prints int */
	DEBUGINIT();
	DEBUGOUT("In main M4 PS\n");	

#ifdef GPIO_TOGGLE
	RSI_NPSSGPIO_InputBufferEn(NPSS_GPIO_3 , 1U);
	RSI_NPSSGPIO_SetPinMux(NPSS_GPIO_3 , 0);
	RSI_NPSSGPIO_SetDir(NPSS_GPIO_3, NPSS_GPIO_DIR_OUTPUT);
#endif

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


#ifdef ALARM_CONFIG
/*RTC Alarm interrupt*/
void RTC_ALARM_IRQHandler(void)
{
	volatile uint32_t statusRead = 0;

	/*Get the interrupt status */
	statusRead = RSI_RTC_GetIntrStatus();
	if(statusRead & NPSS_TO_MCU_ALARM_INTR)
	{
		/*Clear wake up interrupt */
		RSI_RTC_IntrClear(RTC_ALARM_INTR);
		/*Update seconds for next boundary alarm */
		alarmConfig.Second   = (alarmConfig.Second + ALARAM_PERIODIC_TIME);

		/*Update minutes for next boundary alarm */
		if(alarmConfig.Second  >= (NO_OF_SECONDS_IN_A_MINUTE-1))
		{
			alarmConfig.Second = 0;
			alarmConfig.Minute = (alarmConfig.Minute + 1);
		}
		/*Update minutes for next boundary alarm */
		if(alarmConfig.Minute >= (NO_OF_MINUTES_IN_AN_HOUR-1))
		{
			alarmConfig.Minute =0;
			alarmConfig.Hour = (alarmConfig.Hour + 1);
		}
		/*Update date for next boundary alarm */
		if(alarmConfig.Hour  >= (NO_OF_HOURS_IN_A_DAY-1))
		{
			alarmConfig.Hour  =0;
			alarmConfig.Day = (alarmConfig.Day + 1);
		}
		/*Reconfigure the Alarm for next alarm boundary*/
		RSI_RTC_SetAlarmDateTime(RTC , &alarmConfig);
	}
	RSI_RTC_GetDateTime(RTC , &readTime);


	return ;
}

void InitM4AlarmConfig(void)
{
	/*Init calendar*/
	RSI_RTC_Init(RTC);

	/*RTC configuration with some default time */
	rtcConfig.DayOfWeek        = Saturday;
	rtcConfig.MilliSeconds     = 0;
	rtcConfig.Century          = 0;
	rtcConfig.Day              = 27;
	rtcConfig.Hour             = 6;
	rtcConfig.Minute           = 30;
	rtcConfig.Month            = Febuary;
	rtcConfig.Second           = 2 ;
	rtcConfig.Year             = 17;

	/*RTC alarm configuration */
	alarmConfig.DayOfWeek      = Saturday;
	alarmConfig.Second         = (2 + ALARAM_PERIODIC_TIME);
	alarmConfig.MilliSeconds   = 0;
	alarmConfig.Century        = 0;
	alarmConfig.Day            = 27;
	alarmConfig.Hour           = 6;
	alarmConfig.Minute         = 30;
	alarmConfig.Month          = Febuary;
	alarmConfig.Year           = 17;

	/*Set the RTC configuration*/
	RSI_RTC_SetDateTime(RTC ,&rtcConfig);
	/*Set Alarm configuration */
	RSI_RTC_SetAlarmDateTime(RTC , &alarmConfig);
	/*Enable Alarm feature*/
	RSI_RTC_AlamEnable(RTC ,ENABLE);
	/*Enable RTC ALARM interrupts*/
	RSI_RTC_IntrUnMask(RTC_ALARM_INTR);

	RSI_PS_SetWkpSources(ALARM_BASED_WAKEUP);

	RSI_RTC_IntrUnMask(RTC_ALARM_INTR);

	NVIC_EnableIRQ(NVIC_RCT_ALARM);	
}
#endif
void IRQ026_Handler()
{
	volatile uint32_t wakeUpSrc = 0;

	DEBUGOUT("\n Wireless Wakeup received \n ");

	/*Get the wake up source */
	wakeUpSrc = RSI_PS_GetWkpUpStatus();

	/*Clear interrupt */
	RSI_PS_ClrWkpUpStatus(NPSS_TO_MCU_WIRELESS_INTR);

	return ;
}


