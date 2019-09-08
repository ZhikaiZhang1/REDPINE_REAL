/**
 * @file    rsi_eap_connectivity.c
 * @version 0.4
 * @date    19 May 2016
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for enterprise connectivity.
 *  @section Description  This file contains example application for enterprise connectivity.
 *
 *
 */
/**
 * Include files
 * */

//! Driver include file to refer driver APIs
#include "rsi_driver.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
//! include the certificate 
#include "wifiuser.pem"

//! Access point SSID to connect
#define SSID                  "REDPINE_AP"

//! Security type
#define SECURITY_TYPE         RSI_WPA2_EAP

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE             1

//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP             0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY               0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK               0x00FFFFFF 

#endif

//! Load certificate to device flash :
//! Certificate could be loaded once and need not be loaded for every boot up
#define LOAD_CERTIFICATE  1

//! User ID which is configured in the user configuration file of the radius sever
#define USER_IDENTITY         "\"user1\""            

//! Password which is configured in the user configuration file of the Radius Server for that User Identity.
#define PASSWORD              "\"test123\""          

//! Device port number
#define DEVICE_PORT       5001

//! Server port number
#define SERVER_PORT       5001

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x6B01A8C0

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Memory length for driver
#define GLOBAL_BUFF_LEN 10000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500


//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

int32_t rsi_eap_connectivity()
{
  int32_t     client_socket;
  struct      rsi_sockaddr_in server_addr, client_addr;
  int32_t     status       = RSI_SUCCESS;
  int32_t     packet_count = 0;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#endif
  rsi_eap_credentials_t credentials;
  
  //! configuring user name
  rsi_strcpy(credentials.username, USER_IDENTITY);
  
  //! configuring password
  rsi_strcpy(credentials.password, PASSWORD);

  //! WC initialization
  status = rsi_wireless_init(2, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#if LOAD_CERTIFICATE
  //! Load certificates
  status  = rsi_wlan_set_certificate(RSI_EAP_CLIENT, wifiuser, (sizeof(wifiuser)-1));
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! Connect to an Acces point
  status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, (uint8_t *)&credentials);
  if(status != RSI_SUCCESS)
  {
	  status = rsi_wlan_get_status();
    return status;
  }


  //! Configure IP 
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0, NULL, 0,0);
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif 
 if(status != RSI_SUCCESS)
  {
	  status = rsi_wlan_get_status();
    return status;
  }

  //! Create socket
  client_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
  if(client_socket < 0)
  {
    status = rsi_wlan_get_status();
    return status;
  }

  //! Memset client structrue
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
  server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

  //! Connect to server socket
  status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(client_socket, 0);
    return status;
  }

  while(packet_count < NUMBER_OF_PACKETS)
  {
    //! Send data on socket
    status = rsi_send(client_socket,(int8_t *)"Hello from TCP client!!!", (sizeof("Hello from TCP client!!!") - 1), 0);
    if(status < 0)
    {
      status = rsi_wlan_get_status();
      rsi_shutdown(client_socket, 0);
      return status;
    }

    packet_count++;
  }
  rsi_shutdown(client_socket, 0);
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

  rsi_task_handle_t wlan_task_handle = NULL;

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
  //! Task created for WLAN task
  rsi_task_create(rsi_eap_connectivity, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call EAP application
  status = rsi_eap_connectivity();

  //! Application main loop
  main_loop();
#endif
    return status;
}
