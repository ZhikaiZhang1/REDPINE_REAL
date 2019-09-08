
/**
 * @file    rsi_wlan_sta_tcp_rx_WIFI_4_1.c
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
*
 */
/*=======================================================================*/
//  ! INCLUDES
/*=======================================================================*/
#include <rsi_common_app.h>
#if WIFI_TCP_RX_POWERSAVE
#include "stdlib.h"
#include "rsi_driver.h"
#include "rsi_socket.h"
#include "rsi_wlan_apis.h"
#include "fsl_debug_console.h"
#include "math.h"
#include "fsl_lptmr.h"
#include "fsl_common.h"

/*=======================================================================*/
//   ! DEFINES
/*=======================================================================*/
#define BUF_SIZE   1460

/*=======================================================================*/
//   ! VARIABLES
/*=======================================================================*/
rsi_wlan_app_cb_t rsi_wlan_app_cb;              //! application control block
int32_t client_socket,addr_size,server_socket;  //! client socket id
int8_t buff[1460];
struct rsi_sockaddr_in server_addr, client_addr; //! server and client IP addresses
static int32_t new_socket;

extern uint8_t tx_rx_Completed;

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

volatile int32_t  tot_time,start_time=0,end_time=0;
volatile int32_t  packet_count = 0;
volatile uint8_t  data_recvd = 0;
volatile uint64_t num_bytes = 0;

#if (!(DHCP_MODE))
	uint32_t    ip_addr      = DEVICE_IP;
	uint32_t    network_mask = NETMASK;
	uint32_t    gateway      = GATEWAY;
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
 * @fn         void Throughput(void)
 * @brief      Function to calculate throughput
 * @param[in]  void
 * @return     void
 * @section description
 *====================================================*/
void compute_throughput(void)
{
	num_bits = num_bytes * 8;                   //! number of bits
	xfer_time = t_end - t_start; 	            //! data transfer time
	throughput = ((float)(num_bits)/xfer_time); //!Throughput calculation

	LOG_PRINT("Time taken in msec: %d\n", xfer_time);
	LOG_PRINT("Throughput: %d kbps \r\n", (uint32_t) (num_bits / xfer_time));
#ifdef RSI_WITH_OS
  while(1);
#endif
}

int32_t rsi_app_wlan_socket_create()
{
	int32_t	status = RSI_SUCCESS;

	#if SOCKET_ASYNC_FEATURE
			server_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 0, socket_async_recive);
	#else
			server_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
	#endif
		if(server_socket < 0)
		{
			status = rsi_wlan_get_status();
			return status;
		}

		memset(&server_addr, 0, sizeof(server_addr));    //! Set server structure
		server_addr.sin_family= AF_INET;                 //! Set family type
		server_addr.sin_port = htons(DEVICE_PORT);       //! Set local port number

		status = rsi_bind(server_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));    //! Bind socket
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("bind failed\n");
			status = rsi_wlan_get_status();
			rsi_shutdown(server_socket, 0);
			return status;
		}

		status = rsi_listen(server_socket, 1);       //! Socket listen
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("listen failed\n");
			status = rsi_wlan_get_status();
			rsi_shutdown(server_socket, 0);
			return status ;
		}

		addr_size = sizeof(server_socket);
		LOG_PRINT("TCP Server started...waiting for connection\n");
		new_socket = rsi_accept(server_socket, (struct rsi_sockaddr *)&client_addr, &addr_size);   //! Socket accept
		if(new_socket < 0)
		{
			LOG_PRINT("accept failed\n");
			status = rsi_wlan_get_status();
			rsi_shutdown(server_socket, 0);
			return status;
		}

		t_start = rsi_hal_gettickcount();
		//! Apply power save profile with deep sleep
    status = rsi_wlan_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
    if(status != RSI_SUCCESS)
    {
      return status;
    }
}
/*====================================================*/
/**
 * @fn         void  rsi_app_task_wifi_tcp_rx_ps(void)
 * @brief      Function to work with application task
 * @param[in]  void
 * @return     void
 *=====================================================*/
void  rsi_app_task_wifi_tcp_rx_ps(void)
{
	int32_t	status = RSI_SUCCESS;
	uint8_t ip[20] = {0};
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
		rsi_wlan_app_callbacks_init();             	         //! register call backs
		rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;  //! update wlan application state
		status = rsi_wlan_power_save_profile(RSI_ACTIVE, PSP_TYPE);
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("WLAN, setting to active failed\n");
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
		status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP,0, 0, 0, ip, sizeof(ip),0);
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
			LOG_PRINT("RSI_STA IP ADDR: %d.%d.%d.%d\n",ip[6],ip[7],ip[8],ip[9]);
		}
	}
	case RSI_WLAN_IPCONFIG_DONE_STATE:
	{
		if(data_recvd)
		{
			rsi_shutdown(server_socket, 0);
			LOG_PRINT("TCP client closed\n");
			LOG_PRINT("TCP RX test end\n");
			data_recvd = 0;    //! Clear data receive flag

			#if THROUGHPUT_EN
				compute_throughput();
				tx_rx_Completed = 1;

			#endif

			break;
		}
		#if HIGH_PERFORMANCE_ENABLE
				status = rsi_socket_config();
				if(status < 0)
				{
					status = rsi_wlan_get_status();
					return ;
				}
		#endif
			status = rsi_app_wlan_socket_create();    //! Create socket
			if(status != RSI_SUCCESS)
			{
				return;
			}

       	LOG_PRINT("wlan socket connected state\n");
		LOG_PRINT("TCP Client connected\n");
		LOG_PRINT("Powersave Mode enabled\n");
		LOG_PRINT("TCP RX test with powersave started...\n");
		//! update wlan application state
		rsi_wlan_app_cb.state = RSI_WLAN_SOCKET_CONNECTED_STATE;
	}

	case RSI_WLAN_SOCKET_CONNECTED_STATE:
		{
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
  rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;      //! update WLAN application state
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
  //! update WLAN application state
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
	data_recvd = 1;
#if THROUGHPUT_EN
	//! capture timestamp after data transfer is completed
	t_end = rsi_hal_gettickcount();
#endif
	//! update WLAN application state
	rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
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
  //! update WLAN application state
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
