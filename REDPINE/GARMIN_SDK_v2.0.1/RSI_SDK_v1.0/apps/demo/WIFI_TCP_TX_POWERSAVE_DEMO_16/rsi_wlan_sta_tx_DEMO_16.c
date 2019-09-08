
/**
 * @file    rsi_wlan_sta_tx_DEMO_16.c
 * @version 0.1
 * @date    13 Nov 2018
 *
 *  Copyright(C) Redpine Signals 2018
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 */
/*=======================================================================*/
//  ! INCLUDES
/*=======================================================================*/
#include <rsi_common_app.h>
#if WIFI_TCP_TX_POWERSAVE
#include "stdlib.h"
#include "rsi_driver.h"
#include "math.h"
#include "fsl_lptmr.h"
#include "fsl_common.h"

/*=======================================================================*/
//   ! VARIABLES
/*=======================================================================*/
rsi_wlan_app_cb_t rsi_wlan_app_cb;                      //! application control block
int32_t client_socket,addr_size,server_socket;         //! client socket id
int8_t buff[1460];
volatile uint8_t data_recvd = 0;
volatile uint64_t num_bytes = 0;
volatile int32_t     packet_count = 0;
volatile int32_t	tot_time,start_time=0,end_time=0;
struct rsi_sockaddr_in server_addr, client_addr;      //! server and client IP addresses

#define BUF_SIZE   1460

//! Throughput parameters
#if THROUGHPUT_EN
uint32_t pkts = 0;
uint64_t num_bits = 0;
uint64_t total_bits = 0;
uint32_t xfer_time;
uint32_t total_time = 0;
uint64_t xfer_time_usec;
uint32_t t_start = 0;
uint32_t t_end;
float throughput;
float throughput_mbps;
extern lptmr_config_t lptmrConfig;
#endif


/*************************************************************************/
//!  CALLBACK FUNCTIONS
/*************************************************************************/
/*====================================================*/
/**
 * @fn          void rsi_wlan_app_callbacks_init(void)
 * @brief       To initialize WLAN application callback
 * @param[in]   void
 * @return      void
 * @section description
 * This callback is used to initialize WLAN
 * ==================================================*/
void rsi_wlan_app_callbacks_init(void)
{
  rsi_wlan_register_callbacks(RSI_JOIN_FAIL_CB, rsi_join_fail_handler);                                   //! Initialize join fail call back
  rsi_wlan_register_callbacks(RSI_IP_FAIL_CB, rsi_ip_renewal_fail_handler);                               //! Initialize IP renewal fail call back
  rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler);       //! Initialize remote terminate call back
  rsi_wlan_register_callbacks(RSI_IP_CHANGE_NOTIFY_CB, rsi_ip_change_notify_handler);                     //! Initialize IP change notify call back
  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, rsi_stations_connect_notify_handler);       //! Initialize IP change notify call back
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, rsi_stations_disconnect_notify_handler); //! Initialize IP change notify call back
}


/*====================================================*/
/**
 * @fn         void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
 * @brief      Function to create Async socket
 * @param[in]  uint32_t sock_no, uint8_t *buffer, uint32_t length
 * @return     void
 * @section description
 * Callback for Socket Async Receive
 * ====================================================*/
void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
	num_bytes += length;
}

/*====================================================*/
/**
 * @fn         int32_t rsi_app_wlan_socket_create()
 * @brief      Function to create socket
 * @param[in]  void
 * @return     int32_t
  *====================================================*/
int32_t rsi_app_wlan_socket_create()
{
	int32_t	status = RSI_SUCCESS;
	#if SOCKET_ASYNC_FEATURE
		client_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 0, socket_async_recive);
	#else
		client_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
	#endif
		if(client_socket < 0)
		{
			LOG_PRINT("socket open failed\n");
			status = rsi_wlan_get_status();
			return status;
		}

		LOG_PRINT("socket create\n");
		memset(&client_addr, 0, sizeof(client_addr));        //! Memset client structruersi_wlan_app_task
		client_addr.sin_family= AF_INET;                    //! Set family type
		client_addr.sin_port = htons(DEVICE_PORT);          //! Set local port number

		LOG_PRINT("socket bind\n");

		status = rsi_bind(client_socket, (struct rsi_sockaddr *) &client_addr, sizeof(client_addr));//! Bind socket
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("bind failed\n");
			status = rsi_shutdown(client_socket, 0);
			return status;
		}
		memset(&server_addr, 0, sizeof(server_addr));     //! Reset server structure
		server_addr.sin_family = AF_INET;                //! Set server address family
		server_addr.sin_port = htons(SERVER_PORT);
		server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

		LOG_PRINT("socket connect\n");

		status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr)); 	//! Connect to server socket
		if(status != RSI_SUCCESS)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(client_socket, 0);
			LOG_PRINT("socket connect failed\n");
			return status;
		}

		return status;
}
/*====================================================*/
/**
 * @fn         void  rsi_app_task_wifi_tcp_tx_ps(void)
 * @brief      Function to work with application task
 * @param[in]  void
 * @return     void
 *=====================================================*/
void  rsi_app_task_wifi_tcp_tx_ps(void)
{
		int32_t	status = RSI_SUCCESS;
	#if !(DHCP_MODE)
		uint32_t    ip_addr      = DEVICE_IP;
		uint32_t    network_mask = NETMASK;
		uint32_t    gateway      = GATEWAY;
	#endif
#ifdef RSI_WITH_OS
		  //! Redpine module initialization
			  status = rsi_device_init(LOAD_NWP_FW);
			  if(status != RSI_SUCCESS)
			  {
			    return status;
			  }

	  //! WiSeConnect initialization
	  status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	  if(status != RSI_SUCCESS)
	  {
	    return status;
	  }

	  //! Send Feature frame
	  status = rsi_send_feature_frame();
	  if(status != RSI_SUCCESS)
	  {
		  return status;
	  }
while(1)	  
#endif
{
	switch(rsi_wlan_app_cb.state)
	{
		case RSI_WLAN_INITIAL_STATE:
		{
			rsi_wlan_app_callbacks_init();                              //! register call backs
			rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;      	//! update wlan application state

			status = rsi_wlan_power_save_profile(RSI_ACTIVE, PSP_TYPE);
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("Wlan, setting to active failed\n");
				break;
			}
		}
		case RSI_WLAN_UNCONNECTED_STATE:
		{
			status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK); //! Connect to an Access point
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("WLAN connection failed\n");
				break;
			}
			else
			{
				rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;       //! update WLAN application state to connected state
				LOG_PRINT("WLAN connected state\n");
			}
		}
		case RSI_WLAN_CONNECTED_STATE:
		{
			//! Configure IP
			#if DHCP_MODE
					status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP,0, 0, 0, NULL, 0,0);
			#else
					status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
			#endif
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("IP Config failed\n");
				break;
			}
			else
			{
				rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
				LOG_PRINT("WLAN ipconfig done state\n");
			}
		}
	case RSI_WLAN_IPCONFIG_DONE_STATE:
	{
		status = rsi_app_wlan_socket_create();
		if (status != RSI_SUCCESS)
		{
			return;
		}
		else
		{
			status = rsi_wlan_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);        //! Apply power save profile with connected sleep
			if(status != RSI_SUCCESS)
			{
				return;
			}
		}
		LOG_PRINT("TCP Client connected\n");
		LOG_PRINT("Powersave Mode enabled\n");
		LOG_PRINT("TCP TX test with powersave started...\n");

		start_time = rsi_hal_gettickcount();
		do
		{
			status = rsi_send(client_socket,(int8_t *)buff, sizeof(buff), 0);
			//! Send data on socket
			if(status < 0)
			{
				continue;
				while(1);
				status = rsi_wlan_get_status();
				rsi_shutdown(client_socket, 0);
			}
			//! Increase packet count
			packet_count++;
			if((packet_count==(int)(REQ_THRUPT_IN_MBPS*90)))
			{
				//status = rsi_sendto(client_socket, (int8_t *)buff, sizeof(buff), 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
				packet_count =0;
				end_time = rsi_hal_gettickcount();
				tot_time = end_time - start_time;
				//if(tot_time < 1000 && ((1000-tot_time) >5))
				if(tot_time < 1000 )
				{
#ifndef RSI_WITH_OS
					rsi_wireless_driver_task();
#endif
					rsi_delay_ms(1000 -tot_time);
				}
				start_time = rsi_hal_gettickcount();
			}

		}while(1);

		break;
	
	}
	default:
		break;
	}
}
}

/*====================================================*/
/**
 * @fn         void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler in station mode at rejoin failure
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;     //! update wlan application state
}


/*====================================================*/
/**
 * @fn         void rsi_ip_renewal_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      IP renewal failure call back handler in station mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_ip_renewal_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	  //! update wlan application state
	  rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;
}


/*====================================================*/
/**
 * @fn         void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to terminate stations remote socket
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
		data_recvd = 1;        //Set data receive flag
	#if (THROUGHPUT_EN)
		t_end = rsi_hal_gettickcount();           //! capture time-stamp after data transfer is completed
	#endif
		rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE; //! update wlan application state
}


/*====================================================*/
/**
 * @fn         void rsi_ip_change_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to notify IP change in Station mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_ip_change_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  //! update wlan application state
  rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
}


/*====================================================*/
/**
 * @fn         void rsi_stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to notify stations connect in AP mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{

}

/*====================================================*/
/**
 * @fn         void rsi_stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to notify stations disconnect in AP mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{

}
#endif

