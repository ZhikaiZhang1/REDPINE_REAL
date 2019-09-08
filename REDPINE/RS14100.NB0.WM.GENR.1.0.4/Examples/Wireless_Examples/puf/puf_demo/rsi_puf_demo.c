/**
 * @file    rsi_puf_demo.c
 * @version 1.0
 * @date    03 APR 2018
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for PUF and AES engine usage
 *
 *  @section Description  This file contains example application for Starting PUF in module and authorizing the user.
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

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! Socket include file for socket APIs
#include "rsi_socket.h"

//! PUF include file for PUF Apis
#include "rsi_puf.h"

#ifdef RSI_ENABLE_DEBUG_PRINT
#include <stdio.h>
#endif

//For the first time to enroll PUF 
#define PUF_ENROLL      0

//! Access point SSID to connect
#define SSID               "REDPINE"

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO              0

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               NULL

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE          1

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

//! Receive data length
#define RECV_BUFFER_SIZE   500

//! send data length
#define SEND_BUFFER_SIZE   500
//! Memory length for driver
#define GLOBAL_BUFF_LEN   10000

//! Wlan task priority
#define RSI_PUF_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_PUF_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500
//! Device port number
#define DEVICE_PORT       5001

//! Server port number
#define SERVER_PORT       5003

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x640AA8C0

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//!Size of data should be multiples of key_size(in bytes)
#define RSI_AES_PLAIN_TXT "Redpine Signals Redpine Signals "

//! AES Key to be used to encrypt and decrypt
#define RSI_AES_KEY "123456abcdefghij"

uint8_t	  rsp[18];
uint8_t error = 0;

//! Function to use PUF to encrypt and decrypt data using key encrypted by PUF
int32_t rsi_puf_aes_encry_decry_app()
{
  int32_t     status       = RSI_SUCCESS , len = 0;
  int32_t     client_socket;
  int32_t     addr_size;
  struct      rsi_sockaddr_in server_addr, client_addr;
  uint8_t     i = 0, j = 0, k = 0;
  //Buffers to store responses
  uint8_t aes_encry_data[sizeof(RSI_AES_PLAIN_TXT)-1];
  uint8_t aes_decry_data[RECV_BUFFER_SIZE] = {0}, signature[32] = {0}, msg[100] = {0};
  uint8_t keycodeI[KEY_CODE_SIZE_BYTES]={0}, keycodeK[KEY_CODE_SIZE_BYTES]={0}, kck[KEY_CODE_SIZE_BYTES]={0}, kci[KEY_CODE_SIZE_BYTES]={0};
  uint8_t recv_buffer[RECV_BUFFER_SIZE] = {0};
  uint8_t recv[RECV_BUFFER_SIZE] = {0};
  uint8_t send_buffer[SEND_BUFFER_SIZE] = {0};
  uint8_t key[MAX_KEY_SIZE_BYTES] = {RSI_AES_KEY};
  uint8_t decry_len = 0;
  uint8_t sign_len = sizeof(RSI_AES_PLAIN_TXT) - 1;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#endif


  //! WC initialization
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#if PUF_ENROLL
  //! Enroll PUF for the first Time
  status = rsi_puf_enroll_req();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  while(1);
#endif
  //!Start PUF
  status = rsi_puf_start_req();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //!Set Key(16Bytes) operation with PUF
  status = rsi_puf_set_key_req(1, PUF_KEY_SIZE_128 , key, keycodeK, KEY_CODE_SIZE_BYTES );
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //!Set Intrinsic Key(16Bytes) operation with PUF
  status = rsi_puf_intr_key_req(2, PUF_KEY_SIZE_128 , keycodeI, KEY_CODE_SIZE_BYTES );
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  //!Load Key(16Bytes) operation with PUF
  status = rsi_puf_load_key_req(keycodeI);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Encryption of secret messsage in AES ECB mode to get signature data
  memset(aes_encry_data, 0,strlen(RSI_AES_PLAIN_TXT) );
  status = rsi_puf_aes_encrypt_req(AES_ECB_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, sign_len, RSI_AES_PLAIN_TXT, NULL, NULL, aes_encry_data, sign_len);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Forming packet (Intrinsic keycode, Keycode of key, Signatur data)
  memcpy(&send_buffer[i],keycodeI,KEY_CODE_SIZE_BYTES);
  i = i+KEY_CODE_SIZE_BYTES;
  memcpy(&send_buffer[i],keycodeK,KEY_CODE_SIZE_BYTES);
  i = i+KEY_CODE_SIZE_BYTES;
  memcpy(&send_buffer[i], aes_encry_data, sign_len);
  i = i + strlen(RSI_AES_PLAIN_TXT);

  len = i;

  //! Scan for Access points
  status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Unable to scan \n");
#endif
    return status;
  }

  //! Connect to an Access point
  status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Unable to connect\n");
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Connecting to network ..........\n");
#endif

  //! Configure IP
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP,0, 0, 0, rsp, 18,0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif
  ///////////////////////////////////

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
  while(1)
  {
    status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
    if(status != RSI_SUCCESS)
    {
      continue;
    }
    else
    {
#ifdef RSI_ENABLE_DEBUG_PRINT
      printf("\nConnected to Network\n");
#endif
      break;
    }
  }

  //! Sending the packet to the user
  status = rsi_send(client_socket, (const int8_t *)send_buffer, len, 0);
  if(status < 0)
  {
    return status;
  }


  //! Receive data on socket
receive_again:
  memset(recv_buffer, 0, RECV_BUFFER_SIZE);
  status = rsi_recvfrom(client_socket, (int8_t *)recv_buffer, RECV_BUFFER_SIZE, 0, (struct rsi_sockaddr *)&client_addr, &addr_size);
  if(status < 0)
  {
    status = rsi_wlan_get_status();
    if(status == RSI_RX_BUFFER_CHECK)
    {
      continue;
    }
    rsi_shutdown(client_socket, 0);
    return status;
  }

  //! Parsing the received packet 
  memcpy(kck,recv_buffer,KEY_CODE_SIZE_BYTES);
  j = j + KEY_CODE_SIZE_BYTES;
  memcpy(recv,&recv_buffer[j],(status - j));
  decry_len = status - j;

  //!Load Key(16Bytes) operation with PUF
  status = rsi_puf_load_key_req(kck);
  if(status != RSI_SUCCESS)
  {
    error = 1;
    return status;
  }

  //! Decrypt the received data using PUF in AES ECB mode
  memset(aes_decry_data, 0, decry_len);
  status = rsi_puf_aes_decrypt_req(AES_ECB_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, decry_len, recv, NULL, NULL, aes_decry_data, decry_len);
  if(status != RSI_SUCCESS)
  {
    error = 1;
    return status;
  }

  //! After Decryption parse the Intrinsic keycode, signature data, Message
  memcpy(kci,aes_decry_data,KEY_CODE_SIZE_BYTES);
  k = k + KEY_CODE_SIZE_BYTES;
  memcpy(signature,&aes_decry_data[k],sign_len);
  k = k + sign_len;
  memset(msg, 0, 100);
  memcpy(msg,&aes_decry_data[k],(decry_len - k));

  //!Load Key(16Bytes) operation with PUF
  status = rsi_puf_load_key_req(kci);
  if(status != RSI_SUCCESS)
  {
    error = 1;
    return status;
  }

  //! Decrypt the signature data to get the secret message in AES ECB mode
  memset(aes_decry_data, 0, sign_len);
  status = rsi_puf_aes_decrypt_req(AES_ECB_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, sign_len, signature, NULL, NULL, aes_decry_data, sign_len);
  if(status != RSI_SUCCESS)
  {
    error = 1;
    return status;
  }

  //! Compare the decrypted secret message with the actual secret message
  if(!(strncmp((char *)aes_decry_data,RSI_AES_PLAIN_TXT,strlen(RSI_AES_PLAIN_TXT))))
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n\n**************************        %s****************************************\n\n",msg);
#endif
  }
  else
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\nUnauthorized to communicate \n");
#endif
  }

  j = 0;
  k = 0;

  goto receive_again;
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

  rsi_task_handle_t puf_task_handle = NULL;

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
  //! Task created for PUF task
  rsi_task_create( rsi_aes_encry_decry_app, "puf_aes_task", RSI_PUF_TASK_STACK_SIZE, NULL, RSI_PUF_TASK_PRIORITY, &puf_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call PUF based aes encryption and decryption application
  status = rsi_puf_aes_encry_decry_app();
  if(status != RSI_SUCCESS)
  {
    if(error == 1)
    {
#ifdef RSI_ENABLE_DEBUG_PRINT
      printf("\n Unauthorized to communicate\n");
#endif
    }
  }

  //! Application main loop
  main_loop();
#endif
  return status;

}
