/**
 * @file    rsi_ap_start.c
 * @version 0.1
 * @date    15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for TCP server socket in AP mode
 *
 *  @section Description  This file contains example application for TCP server socket in AP mode
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

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_bootup_config.h" 

#include "socket.h"


//#include "socket.h"
//! Access point SSID to be created
#define SSID              "REDPINE_AP"

//! Channel number
#define CHANNEL_NO              36		//11

//! Security type
#define SECURITY_TYPE     RSI_WPA2

//! Encryption type
#define ENCRYPTION_TYPE   RSI_CCMP

//! Password
#define PSK               "12345678"

//! Beacon interval
#define  BEACON_INTERVAL        100

//! DTIM interval 
#define DTIM_INTERVAL           4


//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP         0x010AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF 

//! Device port number
#define DEVICE_PORT       5001

//! Number of packets to receive
#define NUMBER_OF_PACKETS  1000

//! Receive data length
#define RECV_BUFFER_SIZE   1000 

//! Memory length for driver
#define GLOBAL_BUFF_LEN   10000


//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500
#define SEND_LENGTH 48
#define HOSTNAME "channel" // changed from channel
#define SERVER_PORT       3490
#define SSIDD              "Logan_conqueror"
#define CHANNEL_NON              0			
#define PSK2               "89706090"

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

uint8_t station_mac[6];
volatile uint8_t pack_received = 0;
volatile int pack_length;
uint8_t SPI_buff[1024];
typedef struct {
	int bot_id;
	uint8_t send_buffer[SEND_LENGTH];
}array_sending;
int32_t     client_socket;
struct      rsi_sockaddr_in server_addr;

//! This callback function is called when Station is connected.
//! Buffer has the MAC address of the station connected
void   stations_connect_notify_handler(uint16_t status,const uint8_t *buffer, const uint16_t length)
{

  memcpy(station_mac, buffer,6);
}

//! This callback function is called when Station is disconnected.
//! Buffer has the MAC address of the station disconnected
void   stations_disconnect_notify_handler(uint16_t status,const uint8_t *buffer, const uint16_t length)
{

}

static void receive_callback(uint32_t sock_no, uint8_t *buffer, uint32_t length){
//	pack_length = length;
	pack_received = 1;
	memcpy(SPI_buff, buffer, length);
}

/*static void receive_callback(uint32_t sock_no, uint8_t *buffer, uint32_t length){
	packet_received = 1;
	
//array_receiving *SPI_buff;
//	memcpy(SPI_buff, (array_receiving *) buffer, length);
}*/

int32_t rsi_ap_start()
{
  int32_t     server_socket, new_socket;
  struct      rsi_sockaddr_in server_addr, client_addr;
  int32_t     status       = RSI_SUCCESS;
  int32_t     packet_count = 0, recv_size  = 0, addr_size;
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
  //! buffer to receive data over TCP client socket
  int8_t recv_buffer[RECV_BUFFER_SIZE];

  //! Register callbacks for Station conencted and disconnected events
  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, stations_connect_notify_handler);
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, stations_disconnect_notify_handler);


  //! WC initialization
  status = rsi_wireless_init(9, 0); 
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Configure IP 
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,1);
  if(status != RSI_SUCCESS)//used to be RSI_DHCP -> last parameter: vap_id is 1 if acess point (otherwise 0, whcih was the intitial state)
  {
    return status;
  }
  
  //! Start Access point
  status =  rsi_wlan_ap_start((int8_t *)SSID, CHANNEL_NO, SECURITY_TYPE, ENCRYPTION_TYPE, (uint8_t *)PSK, BEACON_INTERVAL, DTIM_INTERVAL);
    if(status != RSI_SUCCESS)
    {
      return status;
    }


  server_socket = rsi_socket(AF_INET, SOCK_STREAM, 0); //! Create socket
  if(server_socket < 0)
  {
    status = rsi_wlan_get_status();
    return status;
  }

  //! Set server structure
  memset(&server_addr, 0, sizeof(server_addr));  

  //! Set family type
  server_addr.sin_family= AF_INET;

  //! Set local port number
  server_addr.sin_port = htons(DEVICE_PORT);



  //! Bind socket
  status = rsi_bind(server_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(server_socket, 0);
    return status;
  }

  //! Socket listen
  status = rsi_listen(server_socket, 1);
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(server_socket, 0);
    return status;
  }

  addr_size = sizeof(server_socket);

 /* ! Socket accept
  new_socket = rsi_accept(server_socket, (struct rsi_sockaddr *)&client_addr, &addr_size);
  if(new_socket < 0)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(server_socket, 0);
    return status;
  }

  while(packet_count < NUMBER_OF_PACKETS)
  {
    recv_size = RECV_BUFFER_SIZE;

    do
    {
      //! Receive data on socket
      status = rsi_recvfrom(new_socket, recv_buffer, recv_size, 0, (struct rsi_sockaddr *)&client_addr, &addr_size);
      if(status < 0)
      {
        status = rsi_wlan_get_status();
        if(status == RSI_RX_BUFFER_CHECK)
        {
          continue;
        }

        rsi_shutdown(server_socket, 0);
        return status;
      }

      //! subtract received bytes 
      recv_size -= status;

    } while(recv_size > 0);

    packet_count++;
  }
  
  //! Disconnect the station which is connected
  status = rsi_wlan_disconnect_stations(station_mac);
  if(status != RSI_SUCCESS)
  {
    return status;
  }*/
  return 0;
}

int32_t rsi_udp_client()
{
  int32_t     status       = RSI_SUCCESS;
  int32_t     packet_count = 0;
	struct hostent * he;
	char* hostname = HOSTNAME;
	//char* psk = PSK;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#endif

  /*! WC initialization
	wireless_again:
  status = rsi_wireless_init(9, 0);
  if(status != RSI_SUCCESS){
		//DEBUGOUT("init wireless failed\n");
		goto wireless_again;
    //return status;
  }*/
  //! Scan for Access points
	
	scanning_again:
  status = rsi_wlan_scan((int8_t *)SSIDD, (uint8_t)CHANNEL_NON, NULL, 0);
  if(status != RSI_SUCCESS)
  {
	//	DEBUGOUT("scanning failed\n");
		goto scanning_again;
    //return status;
  }

  //! Connect to an Access point
	connect_again:
  status = rsi_wlan_connect((int8_t *)SSIDD, SECURITY_TYPE, (void *)PSK2);
  if(status != RSI_SUCCESS)
  {
	//	DEBUGOUT("connecting failed\n");
		goto connect_again;
    //return status;
  }

  //! Configure IP 
	IP_CONFIG:
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0, NULL, 0,0);//RSI_DHCP_HOSTNAME gives hostname feature, not sure what it does, will try out later
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);	//vap_id may need to be 1 again
#endif
	
  if(status != RSI_SUCCESS)
  {
		//DEBUGOUT("config ip address failed\n");
		goto IP_CONFIG;
    //return status;
  }

	if ((he = RSI_bsd_gethostbyname("channel")) == NULL) {		//change hostname as needed
    	goto Find_Host;
        //perror ("gethostbyname");
        //exit (1);
    }
	
  //! Create socket
  client_socket = rsi_socket_async(AF_INET, SOCK_DGRAM, 0, receive_callback);
  if(client_socket < 0)
  {
    status = rsi_wlan_get_status();
    //return status;
  }

  //! Set server structure
  memset(&server_addr, 0, sizeof(server_addr));

  //! Set server address family
  server_addr.sin_family = AF_INET;

  //! Set server port number, using htons function to use proper byte order
  server_addr.sin_port = htons(SERVER_PORT);
	Find_Host:
	
	//he = gethostbyname("Logan");
  //! Set IP address to localhost
  //server_addr.sin_addr.s_addr = * ((struct in_addr *) he-> h_addr);
	server_addr.sin_addr = * ((struct in_addr *) he-> h_addr_list[0]);
/*
  while(packet_count < NUMBER_OF_PACKETS)
  {
    //! Send data on socket
    status = rsi_sendto(client_socket, (int8_t *)"Hello from UDP client!!!", (sizeof("Hello from UDP client!!!") - 1), 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
    if(status < 0)
    {
      status = rsi_wlan_get_status();
      rsi_shutdown(client_socket, 0);
      return status;
    }

    //! Increase packet count
    packet_count++;
  }
	*/
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

int main(){
  int32_t status;
	array_sending sender;
	int i;

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
  rsi_task_create(rsi_ap_start, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call TCP server application in AP mode
  status = rsi_ap_start();
	rsi_udp_client();
 //-------------------------------------------------------------
  //temperory buffer content, will be changed after SPI is implemented
	sender.bot_id = 1;
//	for (i = 1; i < 10 ;i++){
	//	sender.send_buffer[i] = 30;
 // }
  for (i = 0; i < 48 ;i++){
		sender.send_buffer[i] = i+1;
  }
	
 // sender.send_buffer[46] = 100;
  //sender.send_buffer[47] = 100;
	//--------------------------------------------------------------
  while(1){
		
		if (pack_received == 1){
			pack_received = 0;
		}
			status = RSI_bsd_sendto(client_socket, &sender, (sizeof(sender)), 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
	}
  //! Application main loop
//  main_loop();
#endif
  return status;

}

