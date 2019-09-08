/**
 * @file    rsi_dhcp_dns_fqdn_app.c
 * @version 0.4
 * @date    7 Oct 2016
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for dns update functionality in station mode
 *
 *  @section Description  This file contains example application for dns update functionality 
 *
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

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_nwk.h"

#include "rsi_utils.h"

//! configurattion Parameters

//! Access point SSID to connect
#define SSID              "REDPINE_AP"

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO              0

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               NULL

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE         1 

//! DHCP HOSTNAME feature 1- Enable 0- Disable
#define DHCP_HOST_NAME    0

//! DHCP OPTION-81 1- Enable 0- Disable
#define DHCP_OPTION_81    1

//! DHCP UNICAST OFFER 1- Enable 0- Disable
#define DHCP_UNICAST_OFFER    1 

//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP              0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY                0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK                0x00FFFFFF 

#endif

//! Number of packet to send
#define NUMBER_OF_PACKETS        20

//! Memory length for driver
#define GLOBAL_BUFF_LEN        10000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! IP address of the DNS server 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define RSI_DNS_SERVER_IP          0x6500A8C0

//! DNS hostname time to live value
#define RSI_DNS_TTL                53

//! Zone name of the domain
#define RSI_DNS_ZONE_NAME          "rps"

//! Hostname of the domain
#define RSI_DNS_HOST_NAME          "Logan"

#define RSI_DNS_URL_NAME           "www.redpinesignals.com"


#define PING_SIZE              100


//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

void main_loop();
void rsi_dns_response_handler(uint16_t status);
volatile uint8_t dns_rsp_received;
uint8_t ping_rsp_received;

rsi_rsp_dns_query_t dns_query_resp;
void rsi_ping_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length);

  int32_t     packet_count      = 0;

//! DHCP-DNS FQDN Application 
int32_t rsi_dhcp_dns_fqdn_app()
{
  int32_t     status            = RSI_SUCCESS;
  uint32_t    server_address    = RSI_DNS_SERVER_IP;
  uint16_t     size             = PING_SIZE;
#if !(DHCP_MODE) 
  uint32_t    ip_addr             = DEVICE_IP;
  uint32_t    network_mask        = NETMASK;
  uint32_t    gateway             = GATEWAY;
#endif
  uint8_t     dhcp_mode         = 0;
  uint32_t    remote_ip_addr    = 0;
  

#if DHCP_MODE
   dhcp_mode |= RSI_DHCP;
#endif

#if DHCP_OPTION_81
   dhcp_mode |= RSI_DHCP_OPTION81; 
#endif

#if (DHCP_HOST_NAME && (!DHCP_OPTION_81))
   dhcp_mode |= RSI_DHCP_HOSTNAME;
#endif

#if (DHCP_MODE && (DHCP_UNICAST_OFFER))
   dhcp_mode |= RSI_DHCP_UNICAST_OFFER;
#endif

  //! WC initialization
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Scan for Access points
  status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Connect to an Access point
  status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Configure IP 
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0, 0);
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0, 0);
#endif
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_dns_update(RSI_IP_VERSION_4, (uint8_t *)RSI_DNS_ZONE_NAME, (uint8_t *)RSI_DNS_HOST_NAME, 
                    (uint8_t *)&server_address, (uint16_t)RSI_DNS_TTL, rsi_dns_response_handler);

  //! wait for the success response 
  do
  {
#ifndef RSI_WITH_OS
    //! event loop 
    rsi_wireless_driver_task();
#endif

  }while(!dns_rsp_received);

  if(dns_rsp_received == 2)
  {
    return  rsi_wlan_get_status();
  }

  dns_rsp_received = 0;

  //! DNS QUERY
  status = rsi_dns_req(RSI_IP_VERSION_4,(uint8_t *)RSI_DNS_URL_NAME,(uint8_t *)&server_address,&dns_query_resp,sizeof(rsi_rsp_dns_query_t));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  remote_ip_addr = rsi_bytes4R_to_uint32(dns_query_resp.ip_address[0].ipv4_address);

  //! PING Request
  while(packet_count < NUMBER_OF_PACKETS)
  {

	  status = rsi_wlan_ping_async(0, (uint8_t *)&remote_ip_addr, size, rsi_ping_response_handler);


	  ping_rsp_received = 0;
	  packet_count++;

#ifndef RSI_WITH_OS
    //! wait for Driver task to complete
		 main_loop();

#else
	  while(!ping_rsp_received);
#endif

  }


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

    if(ping_rsp_received == 1)
    {
      ping_rsp_received = 0;
      break;
    }
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
  rsi_task_create(rsi_dhcp_dns_fqdn_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call dns application
  status = rsi_dhcp_dns_fqdn_app();

  //! Application main loop
  main_loop();
#endif
  return status;

}


void rsi_dns_response_handler(uint16_t status)
{
  if(status == RSI_SUCCESS)
  {
    dns_rsp_received = 1;
  }
  else
  {
    dns_rsp_received = 2;
  }
}

//! ping response notify call back handler 
void rsi_ping_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length)
{
  ping_rsp_received = 1;
}

