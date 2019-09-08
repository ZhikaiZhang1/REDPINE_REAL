/**
 * @file    rsi_wlan_tcp_loopback_DEMO_10.c
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
 *  @brief : This file contains example application for TCP loopback
 *
 *  @section Description  This file contains example application for TCP Loopback
 *
 *
 */
/*=======================================================================*/
//  ! INCLUDES
/*=======================================================================*/
#include <rsi_common_app.h>

#if WIFI_TCP_BI_DIR
#include "stdlib.h"
#include "rsi_driver.h"
#include "fsl_lptmr.h"

/*=======================================================================*/
//   ! DEFINES
/*=======================================================================*/
#define BUF_SIZE   1460
#define TCP_LOOPBACK								1    //Set 1 to check TCP Loopback

/*=======================================================================*/
//   ! VARIABLES
/*=======================================================================*/
rsi_wlan_app_cb_t rsi_wlan_app_cb;                 //! application control block
int32_t client_socket;                             //! Client socket id
struct rsi_sockaddr_in server_addr, client_addr;   //! Server and Client IP addresses
static int32_t new_socket;
static int32_t server_socket;

volatile int8_t rxBuff[BUF_SIZE] = {0};
volatile uint32_t rxBytes_len = 0;

volatile uint8_t data_sent  = 0;
volatile uint8_t data_recvd = 0;
volatile uint64_t num_bytes = 0;

/*=======================================================================*/
//   ! EXTERN VARIABLES
/*=======================================================================*/
extern uint8_t loopback_done;


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
 * @brief      Function to create async socket
 * @param[in]  uint32_t sock_no, uint8_t *buffer, uint32_t length
 * @return     void
 * @section description
 * Callback for Socket Async Receive
 *=====================================================*/
void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
	rsi_wlan_app_cb.state = RSI_WLAN_SOCKET_CONNECTED_STATE;
	num_bytes += length;
	rxBytes_len = length;
	memcpy(rxBuff, buffer, rxBytes_len);
	LOG_PRINT("Received %d bytes_1\n", rxBytes_len);
	data_sent = 0;
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
	int32_t     addr_size;

#if SOCKET_ASYNC_FEATURE
#if SSL
		server_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 1, socket_async_recive);
#else
		server_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 0, socket_async_recive);
#endif
#else
		server_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
#endif

		memset(&server_addr, 0, sizeof(server_addr));   //! Set server structure
		server_addr.sin_family= AF_INET;               //! Set family type
		server_addr.sin_port = htons(DEVICE_PORT);     //! Set local port number
		status = rsi_bind(server_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr)); //! Bind socket
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("bind failed\n");
			status = rsi_wlan_get_status();
			rsi_shutdown(server_socket, 0);
			return status;
		}

		status = rsi_listen(server_socket, 1);  //! Socket listen
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("listen failed\n");
			status = rsi_wlan_get_status();
			rsi_shutdown(server_socket, 0);
			return status;
		}

		addr_size = sizeof(server_socket);
		LOG_PRINT("TCP Server started...waiting for connection\n");
		new_socket = rsi_accept(server_socket, (struct rsi_sockaddr *)&client_addr, &addr_size);  //! Socket accept
		if(new_socket < 0)
		{
			LOG_PRINT("accept failed\n");
			status = rsi_wlan_get_status();
			rsi_shutdown(server_socket, 0);
			return status;
		}

		return status;
}

/*====================================================*/
/**
 * @fn         void  rsi_app_task_wifi_tcp_bi_dir(void)
 * @brief      Function to work with application task
 * @param[in]  void
 * @return     void
 *=====================================================*/
void  rsi_app_task_wifi_tcp_bi_dir(void)
{
	int32_t		status = RSI_SUCCESS;
	uint8_t     ip[20] = {0};
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
		rsi_wlan_app_callbacks_init();                         //! register call backs
		rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;    //! update WLAN application state
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
			rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;        //! update WLAN application state
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
			LOG_PRINT("TCP Server closed\n");
			LOG_PRINT("TCP Loopback test end!!!\n");
			data_recvd = 0;          //! Clear data receive flag
			loopback_done = 1;
			break;
		}
			#if TCP_LOOPBACK
			//! Connect to TCP/SSL Server
			#if HIGH_PERFORMANCE_ENABLE
					status = rsi_socket_config();
					if(status < 0)
					{
						status = rsi_wlan_get_status();
						break;
					}
			#endif
			status = rsi_app_wlan_socket_create();    //! Create socket
			if(status != RSI_SUCCESS)
			{
				return;
			}
			else
			{
				LOG_PRINT("WLAN socket connected state\n");
				LOG_PRINT("TCP Client connected\n");
				LOG_PRINT("TCP Loopback test started...\n");
				rsi_wlan_app_cb.state = RSI_WLAN_SOCKET_CONNECTED_STATE;
			}
			#endif
	}
	case RSI_WLAN_SOCKET_CONNECTED_STATE:
	{
		if(!data_sent)
		{
#if TCP_LOOPBACK
      data_sent = 1;        	//! set flag
      LOG_PRINT("Received %d bytes\n", rxBytes_len);
      status = rsi_send(new_socket, (const int8_t *)rxBuff, rxBytes_len, 0);
      if(status < 0)
      {
        status = rsi_wlan_get_status();
        data_sent = 0;        	//! clear flag
        return;
      }
      LOG_PRINT("Sent %d bytes\n", rxBytes_len);
#endif
		}
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
