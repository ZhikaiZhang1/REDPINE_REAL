/**
 * @file    rsi_wlan_ssl_server_cert_bypass_DEMO_36.c
 * @version 0.1
 * @date    20 Feb 2019
 *
 *  Copyright(C) Redpine Signals 2019
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for SSL Sever cert bypass
 *
 *  @section Description  This file contains example application for SSL Sever cert bypass
 *
 *
 */
/*=======================================================================*/
//  ! INCLUDES
/*=======================================================================*/
#include <rsi_common_app.h>
#if WIFI_SSL_SERVER_CERT_BYPASS
#include "stdlib.h"
#include "rsi_driver.h"

/*=======================================================================*/
//  ! VARIABLES
/*=======================================================================*/
rsi_wlan_app_cb_t rsi_wlan_app_cb;                  //! application control block

uint8_t cert_buffer[2048];							//! Application buffer for receiving certificate
uint8_t  volatile cert_recv = 0, connect_rsp = 0;
uint16_t sock_id = 0;

/*=======================================================================*/
//  ! EXTERN VARAIBLES
/*=======================================================================*/


/*=======================================================================*/
//  ! EXTERN FUNCTIONS
/*=======================================================================*/
void rsi_wlan_app_callbacks_init(void)
{
  rsi_wlan_register_callbacks(RSI_JOIN_FAIL_CB, rsi_join_fail_handler);                                   //! Initialize join fail call back
  rsi_wlan_register_callbacks(RSI_IP_FAIL_CB, rsi_ip_renewal_fail_handler);                               //! Initialize IP renewal fail call back
  rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler);       //! Initialize remote terminate call back
  rsi_wlan_register_callbacks(RSI_IP_CHANGE_NOTIFY_CB, rsi_ip_change_notify_handler);                     //! Initialize IP change notify call back
  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, rsi_stations_connect_notify_handler);       //! Initialize IP change notify call back
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, rsi_stations_disconnect_notify_handler); //! Initialize IP change notify call back
}

void recv_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	//! Callback to receive data
}

void socket_connect_response_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	if(status == RSI_SUCCESS)
	{
		//! Connection success
		connect_rsp = 1;
	}
}
void certificate_response_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	rsi_cert_recv_t *cert_recev = (rsi_cert_recv_t *)buffer;

	sock_id = rsi_bytes2R_to_uint16(cert_recev->sock_desc);

	//! User needs to copy the certificate into the certificate buffer
	if(status != RSI_SUCCESS)
	{
		while(1);
	}
	else
	{
		if(cert_recev->more_chunks == 0)
		{
			//! Complete certificate received
			cert_recv = 1;
		}
	}

	return ;
}
int32_t rsi_app_task_wifi_ssl_server_cert_bypass()
{
	struct      rsi_sockaddr_in server_addr, client_addr;
	int32_t     status       = RSI_SUCCESS;
	int32_t     packet_count = 0;
#if !(DHCP_MODE)
	uint32_t    ip_addr      = DEVICE_IP;
	uint32_t    network_mask = NETMASK;
	uint32_t    gateway      = GATEWAY;
#endif
	int32_t     client_socket;


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
#endif
	  //! Register socket connetion notify handler
		rsi_wlan_register_callbacks(RSI_WLAN_SOCKET_CONNECT_NOTIFY_CB, socket_connect_response_handler);

	//! Register certificate receive notify handler
	rsi_wlan_register_callbacks(RSI_WLAN_SERVER_CERT_RECEIVE_NOTIFY_CB, certificate_response_handler);


	//! Scan Access points
	status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("WLAN scan failed\n");
		return status;
	}
	LOG_PRINT("WLAN scan success\n");
	//! Connect to an Access point
	status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("WLAN connection failed\n");
		return status;
	}
	LOG_PRINT("WLAN connection success\n");
	//! Configure IP
#if DHCP_MODE
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0, NULL, 0,0);
#else
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("IP Config failed\n");
		return status;
	}

	LOG_PRINT("WLAN ipconfig done state\n");

	//! Create socket
	client_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 1, (void *)recv_handler);
	if(client_socket < 0)
	{
		status = rsi_wlan_get_status();
		return status;
	}

	//! Memset client structure
	memset(&client_addr, 0, sizeof(client_addr));

	//! Set family type
	client_addr.sin_family= AF_INET;

	//! Set local port number
	client_addr.sin_port = htons(DEVICE_PORT);

	//! Bind socket
	status = rsi_bind(client_socket, (struct rsi_sockaddr *) &client_addr, sizeof(client_addr));
	if(status != RSI_SUCCESS)
	{
		status = rsi_wlan_get_status();
		rsi_shutdown(client_socket, 0);
		return status;
	}

	//! Set server structure
	memset(&server_addr, 0, sizeof(server_addr));

	//! Set server address family
	server_addr.sin_family = AF_INET;

	//! Set server port number, using htons function to use proper byte order
	server_addr.sin_port = htons(SERVER_PORT);

	//! Set IP address to localhost
	server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

	//! set the application buffer
	status=rsi_setsockopt(client_socket, SOL_SOCKET, SO_RCVBUF, cert_buffer, sizeof(cert_buffer));
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! Connect to server socket
	status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
	if(status != RSI_SUCCESS)
	{
		status = rsi_wlan_get_status();
		rsi_shutdown(client_socket, 0);
		return status;
	}
	
	while(1)
	{
#ifndef RSI_WITH_OS
		//! event loop
		rsi_wireless_driver_task();
#endif
		if(cert_recv == 1)
		{
			////////////////////////////////////////
			//! User certificate validation code ///
			////////////////////////////////////////
			/* User need to take care of validating the received certificate
				 and then only should send the certificate valid reponse to the module */

			//! Send certificate valid response
			status = rsi_certificate_valid(1, sock_id);
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("SSL certificate valid send failed\n");
				return status;
			}

			//! After sending the certificate valid response,
			//resetting certificate received
			cert_recv = 0;
		}
		if(connect_rsp == 1)
		{
			LOG_PRINT("Connected to SSL Server\n");
			LOG_PRINT("Sending SSL data to SSL Server...\n");
			
			//! Successful connection ready to send data
			while(packet_count < NUMBER_OF_PACKETS)
			{
				//! Send data on socket
				status = rsi_send(client_socket,(int8_t *)"Hello from SSL TCP client!!!", (sizeof("Hello from SSL TCP client!!!") - 1), 0);
				if(status < 0)
				{
					status = rsi_wlan_get_status();
					rsi_shutdown(client_socket, 0);
					return status;
				}

				packet_count++;
			}

			LOG_PRINT("Demo Complete\n");
			while(1);

		}
	}

	return 0;
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
