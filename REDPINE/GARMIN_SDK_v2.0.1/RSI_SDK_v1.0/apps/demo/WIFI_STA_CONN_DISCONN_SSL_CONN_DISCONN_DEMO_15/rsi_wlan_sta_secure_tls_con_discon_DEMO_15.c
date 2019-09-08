
/**
 * @file    rsi_wlan_sta_secure_tls_con_discon_DEMO_15.c
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
 *  @brief : This file contains example application for TCP+SLL client socket
 *
 *  @section Description  This file contains example application for TCP+SLL client socket
 *
 *
 */
/*=======================================================================*/
//  ! INCLUDES
/*=======================================================================*/
#include "rsi_common_app.h"
#if WIFI_STA_CONN_DISCONN_SSL_CONN_DISCONN
#include "stdlib.h"
#include "rsi_driver.h"

/*=======================================================================*/
//  ! VARIABLES
/*=======================================================================*/
rsi_wlan_app_cb_t rsi_wlan_app_cb;              //! application control block
int32_t client_socket,addr_size,server_socket;  //! client socket id
struct rsi_sockaddr_in server_addr, client_addr; //! server and client IP addresses
volatile uint64_t num_bytes = 0;

#if SSL
//! Include SSL CA certificate
//#include "servercert.pem"
#include "cacert.pem"
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
 * @fn         int32_t rsi_app_load_ssl_cert()
 * @brief      Function to load SSL certificate
 * @param[in]  void
 * @return     void
 *====================================================*/
int32_t rsi_app_load_ssl_cert()
{
	int32_t	status = RSI_SUCCESS;
	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, NULL ,0);      //! erase existing certificate
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, cacert, (sizeof(cacert)-1));    //! Load SSL CA certificate
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	return status;
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

			memset(&client_addr, 0, sizeof(client_addr));       //! Memset client structruersi_wlan_app_task
			client_addr.sin_family= AF_INET;                    //! Set family type
			client_addr.sin_port = htons(DEVICE_PORT);          //! Set local port number

			LOG_PRINT("socket bind\n");

			status = rsi_bind(client_socket, (struct rsi_sockaddr *) &client_addr, sizeof(client_addr)); //! Bind socket
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("bind failed\n");
				status = rsi_shutdown(client_socket, 0);
				return status;
			}
			memset(&server_addr, 0, sizeof(server_addr));      //! Reset server structure
			server_addr.sin_family = AF_INET;                  //! Set server address family
			server_addr.sin_port = htons(SERVER_PORT);
			server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

			LOG_PRINT("socket connect\n");

			status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));    //! Connect to server socket
            return status;
}
/*====================================================*/
/**
 * @fn         void  rsi_app_task_wifi_sta_ssl_conn_disconn(void)
 * @brief      Function to work with application task
 * @param[in]  void
 * @return     void
 *=====================================================*/
void  rsi_app_task_wifi_sta_ssl_conn_disconn(void)
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
		rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;         //! update wlan application state
	}
	case RSI_WLAN_UNCONNECTED_STATE:
	{
		#if SECURE_CON_DISCON
				while(1)
				{
					do
					{
						status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);       //! Connect to an Access point
					}
					while(status !=RSI_SUCCESS);

					if(status != RSI_SUCCESS)
					{
						LOG_PRINT("WLAN connection failed\n");
						break;
					}
					else
					{
						LOG_PRINT("WLAN connected successfully\n");
					}

					status= rsi_wlan_disconnect();
					if(status!=RSI_SUCCESS)
					{
						LOG_PRINT("WLAN disconnection failed\n");
						break;
					}
					LOG_PRINT("WLAN disconnected successfully\n");
				}
		#endif

		#if TCP_TLS_CON_DISCON
				status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
				if(status != RSI_SUCCESS)
				{
					LOG_PRINT("WLAN connection failed\n");
					break;
				}
				else
				{
					rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;
					LOG_PRINT("WLAN connected state\n");
				}
		#endif
	}
	case RSI_WLAN_CONNECTED_STATE:
	{
		#if DHCP_MODE
				status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP,0, 0, 0, NULL, 0,0);    //! Configure IP
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
			rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;      //! update wlan application state
			LOG_PRINT("wlan ipconfig done state\n");
		}
	}
	case RSI_WLAN_IPCONFIG_DONE_STATE:
	{
		#if (SSL && LOAD_CERTIFICATE)
		rsi_app_load_ssl_cert();
		#endif
		while(1)
		{
			rsi_app_wlan_socket_create();
			if(status != RSI_SUCCESS)
			{
				status = rsi_wlan_get_status();
				rsi_shutdown(client_socket, 0);
				LOG_PRINT("SSL Socket connection failed\n");
				break;
			}
			else
			{
				LOG_PRINT("SSL Socket connected\n");
				rsi_shutdown(client_socket, 0);
				LOG_PRINT("SSL Socket disconnected\n");
			}
		}
	}

	break;

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
	//! update wlan appication state
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

