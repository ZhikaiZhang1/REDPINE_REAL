/**
 * @file     rsi_ftp_client.c
 * @version  0.4
 * @date     19 May 2016
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for FTP client 
 *
 *  @section Description  This file contains example application for FTP client.
 *  This application reads the file(original file) content present in FTP server and creates a 
 *  new file(duplicate file) with same content read from original file by overwriting the 
 *  some part of file with the string "\n REDPINE FTP CLIENT DEMO \n"
 *
 */

/**
 * Include files
 * */
#include "rsi_wlan_apis.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_nwk.h"

#include "string.h"

//! Access point SSID to connect
#define SSID              "REDPINE_AP"

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               ""


//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE         1 

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

//! FTP Server port number
#define FTP_SERVER_PORT       21

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x640AA8C0

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

//! Expected data length from file read
#define FILE_CONTENT_LENGTH   20000

//! Expected list length from directory 
#define DIRECTORY_CONTENT_LENGTH   100


//! FTP Server login username 
#define FTP_SERVER_LOGIN_USERNAME   "username"


//! FTP Server login Password 
#define FTP_SERVER_LOGIN_PASSWORD   "password"

//! File to read which is on FTP server
#define FTP_FILE_TO_READ            "read.txt"

//! File to write on FTP server
#define FTP_FILE_TO_WRITE           "write.txt"

//! FTP client mode 1- Passive mode 0- Active mode
#define FTP_ENABLE_PASSIVE         1
#define FTP_ENABLE_ACTIVE          0
#define FTP_CLIENT_MODE            FTP_ENABLE_ACTIVE 


//! File to rename on FTP server
#define FTP_FILE_TO_RENAME           "read.txt"

//!Set the directory on FTP server 
#define FTP_DIRECTORY_SET             "/work/FTP"

//!Set the directory on FTP server 
#define FTP_DIRECTORY_CREATE             "FTP"

//!list the directories on FTP server
#define FTP_DIRECTORY_LIST            "/work/FTP"
//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];



//! Data read from the file will be stored here
int8_t file_data_read[FILE_CONTENT_LENGTH];

//! Write data to the file 
int8_t file_data_write[FILE_CONTENT_LENGTH] = "THIS IS FTP APPLICATION DEMO FROM REDPINE";


int8_t directory_list_read[FILE_CONTENT_LENGTH];

volatile uint8_t wait_for_complete = 0;

volatile uint8_t wait_for_complete_dir_list = 0;

uint32_t file_data_offset = 0;

uint32_t file_data_length = 200;

uint32_t dir_list_offset = 0;

uint32_t dir_list_length = 0;


//! This call back is called when file read opertion is in progress
void rsi_file_read_cb(uint16_t status,uint8_t *data, uint16_t length, uint8_t end_of_file)
{
  int8_t *file_ptr;
  uint16_t i; 
  file_ptr = file_data_read;
  if(status != RSI_SUCCESS)
  {
    //! Error in file read operation
  }
  //! Indicates end of file
  if(end_of_file == 1)
  {
    wait_for_complete = 1;
  }
  //! Copy data from file to global array
  if((data != NULL) && length && (file_data_offset < FILE_CONTENT_LENGTH))
  {
    memcpy((file_ptr + file_data_offset), data,length);
   
    file_data_offset += length;

    file_data_length += length;
  }
}

//! This call back is called when directory list opertion is in progress
void rsi_directory_list_cb(uint16_t status,int8_t *data, uint16_t length, uint8_t end_of_list)
{
  int8_t *file_ptr;
  uint16_t i; 
  file_ptr = directory_list_read;
  if(status != RSI_SUCCESS)
  {
    //! Error in file read operation
  }
  //! Indicates end of file
  if(end_of_list == 1)
  {
    wait_for_complete_dir_list = 1;
  }
  //! Copy data from file to global array
  if((data != NULL) && length && (dir_list_offset < DIRECTORY_CONTENT_LENGTH))
  {
    memcpy((file_ptr + dir_list_offset), data,length);
   
    dir_list_offset += length;

    dir_list_length += length;
  }
}

int32_t rsi_ftp_client()
{
  int32_t     status       = RSI_SUCCESS;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#endif

  uint32_t    server_ip    = SERVER_IP_ADDRESS;
  int32_t     retval=0;

  //! WC initialization
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Scan for Access points
  status = rsi_wlan_scan((int8_t *)SSID, 0, NULL, 0);
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
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0, NULL, 0,0);
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Connect to  FTP Server
  retval = rsi_ftp_connect(RSI_IP_VERSION_4,(uint8_t *)&server_ip,FTP_SERVER_LOGIN_USERNAME,FTP_SERVER_LOGIN_PASSWORD,FTP_SERVER_PORT);
  if(retval != RSI_SUCCESS)
  {
    return retval;
  }

  ////////////////////////////////////////////////////////////////
  ////// Note: By default FTP client will be in ACTIVE mode///////
  ////////////////////////////////////////////////////////////////
 
  if(FTP_CLIENT_MODE)
  {
    //! Set FTP client PASSIVE mode 
    retval = rsi_ftp_mode_set(FTP_CLIENT_MODE);
    if(retval != RSI_SUCCESS)
    {
      return retval;
    }
  }
  
  //! Read the file in FTP server
  retval =  rsi_ftp_directory_list_async(FTP_DIRECTORY_LIST,rsi_directory_list_cb);
  while(!(wait_for_complete_dir_list))
  {
#ifndef RSI_WITH_OS
    //! Wait for the read to complete
    rsi_wireless_driver_task();
#endif
  }

  //! Directory Create
  retval = rsi_ftp_directory_create(FTP_DIRECTORY_CREATE);

  if(retval != RSI_SUCCESS)
  {
    //! Error in creating directory or 
    //! Directory already exists  
    return retval;
  }

  //! Directory set
  retval = rsi_ftp_directory_set(FTP_DIRECTORY_SET);

  if(retval != RSI_SUCCESS)
  {
    return retval;
  }

  //! File Write
  retval = rsi_ftp_file_write(FTP_FILE_TO_WRITE);

  if(retval != RSI_SUCCESS)
  {
    return retval;
  }
  
  //! File Write Content
  retval = rsi_ftp_file_write_content(0, file_data_write,file_data_length,1);

  if(retval != RSI_SUCCESS)
  {
    return retval;
  }

  //! File Rename
  retval = rsi_ftp_file_rename(FTP_FILE_TO_WRITE,FTP_FILE_TO_RENAME);

  if(retval != RSI_SUCCESS)
  {
    return retval;
  }

  //! Read the file in FTP server
  retval =  rsi_ftp_file_read_aysnc(FTP_FILE_TO_READ,rsi_file_read_cb);
  while(!(wait_for_complete))
  {
#ifndef RSI_WITH_OS
    //! Wait for the read to complete
    rsi_wireless_driver_task();
#endif
  }
  wait_for_complete = 0;
 
 //! File Delete
  retval = rsi_ftp_file_delete(FTP_FILE_TO_READ);

  if(retval != RSI_SUCCESS)
  {
    return retval;
  }
 
  //! File Directory delete
  retval = rsi_ftp_directory_delete(FTP_DIRECTORY_SET);

  if(retval != RSI_SUCCESS)
  {
    return retval;
  }


  //! Disconnect from FTP server 
  retval = rsi_ftp_disconnect();
  if(retval != RSI_SUCCESS)
  {
    return retval;
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
  rsi_task_create(rsi_ftp_client, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task 
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler 
  rsi_start_os_scheduler();

#else
  //! Call FTP client application
  status = rsi_ftp_client();

  //! Application main loop
  main_loop();
#endif
  return status;

}

