/**
 * @file    rsi_wlan_tcp_ssl_rx_tpt_DEMO_9.c
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
 *  @brief : This file contains example application for TCP+SSL client socket
 *
 *  @section Description  This file contains example application for TCP+SSL client socket
 *
 *
 */
/*=======================================================================*/
//  ! INCLUDES
/*=======================================================================*/
#include <rsi_common_app.h>
#if WIFI_SSL_RX_THROUGHPUT
#include "stdlib.h"
#include "rsi_driver.h"

#if SSL
#include "servercert.pem"                //! Include SSL CA certificate
#endif

/*=======================================================================*/
//   ! VARIABLES
/*=======================================================================*/
rsi_wlan_app_cb_t rsi_wlan_app_cb;                  //! application control block
int32_t client_socket;                              //! client socket id
struct rsi_sockaddr_in server_addr, client_addr;    //! server and client IP addresses

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
#endif

volatile uint8_t data_recvd = 0;
volatile uint64_t num_bytes = 0;

/*=======================================================================*/
//   ! EXTERN VARIABLES
/*=======================================================================*/
extern volatile int do_print_stats;
extern uint8_t tx_rx_Completed;

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


/*====================================================*/
/**
 * @fn         int32_t rsi_app_load_ssl_cert()
 * @brief      Function to load SSL certificate
 * @param[in]  void
 * @return     void
 *====================================================*/
int32_t rsi_app_load_ssl_cert()
{
	int32_t	status = RSI_SUCCESS;

	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, NULL ,0); //! erase existing certificate
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("CA cert erase failed\n");
		return status;
	}

	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, servercert, (sizeof(servercert)-1)); //! Load SSL CA certificate
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("CA cert load failed\n");
		return status;
	}
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
#if SSL
	client_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 1, socket_async_recive);
#else
	client_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 0, socket_async_recive);
#endif
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

	memset(&client_addr, 0, sizeof(client_addr));   //! Memset client structures in wlan_app_task
	client_addr.sin_family= AF_INET;               //! Set family type
	client_addr.sin_port = htons(DEVICE_PORT);    //! Set local port number

	LOG_PRINT("socket bind\n");

	status = rsi_bind(client_socket, (struct rsi_sockaddr *) &client_addr, sizeof(client_addr));   //! Bind socket
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("bind failed\n");
		rsi_shutdown(client_socket, 0);
		return status;
	}

	memset(&server_addr, 0, sizeof(server_addr));       //! Reset server structure
	server_addr.sin_family = AF_INET;                  //! Set server address family
	server_addr.sin_port = htons(SERVER_PORT);         //! Set server port number, using htons function to use proper byte order
	server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

	LOG_PRINT("socket connect\n");

	status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr)); //! Connect to server socket
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
 * @fn         void  rsi_app_task_WIFI_1(void)
 * @brief      Function to work with application task
 * @param[in]  void
 * @return     void
 *=====================================================*/
void  rsi_app_task_wifi_ssl_rx_tpt(void)
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
			rsi_wlan_app_callbacks_init();                           //! register callback to initialize WLAN
			rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;      //! update WLAN application state to unconnected state
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
			if(data_recvd)
			{
				rsi_shutdown(client_socket, 0);
				LOG_PRINT("TCP/SSL client closed\n");
				LOG_PRINT("TCP/SSL RX test end\n");
				data_recvd = 0;                             //! Clear data receive flag
				compute_throughput();
				tx_rx_Completed = 1;
				break;
			}

#if (SSL && LOAD_CERTIFICATE)
			status = rsi_app_load_ssl_cert();        //Function to load certificate
			if(status != RSI_SUCCESS)
			{
				return;
			}
#endif

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
				break;
			}
			t_start = rsi_hal_gettickcount();
			rsi_wlan_app_cb.state = RSI_WLAN_SOCKET_CONNECTED_STATE;      //! update WLAN application state
			LOG_PRINT("WLAN socket connected state\n");
			LOG_PRINT("TCP/SSL RX test started...\n");

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
