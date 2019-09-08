/**
 * @file    rsi_fw_update_via_bootloader_DEMO_43.c
 * @version 0.1
 * @date    13-Nov-2018
 *
 *  Copyright(C) Redpine Signals 2018
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 */
/**
 * Include files
 * */
#include <rsi_common_app.h>
#if FIRMWARE_UPGRADE_VIA_BOOTLOADER
//! socket include file to firmware upgrade APIs
#include "rsi_firmware_upgradation.h"
#include "stdlib.h"
#include "rsi_driver.h"
#include "ff.h"
#include "diskio.h"
#include "rsi_data_types.h"
//! application control block
rsi_wlan_app_cb_t rsi_wlan_app_cb;

#define FW_FILE_NAME "/RS9116.rps"
#define RSI_CHUNK_SIZE 4096
#define RSI_IN_BETWEEN_FILE 0
#define RSI_START_OF_FILE   1
#define RSI_END_OF_FILE     2
//! SD Card parameters
UINT bytesRead;
FIL g_fileObject_n;   /* File object */

//! Firmware up request structure 
typedef struct fwupeq_s{
  uint16_t control_flags;
  uint16_t sha_type;
  uint32_t magic_no;
  uint32_t image_size;
  uint32_t fw_version;
  uint32_t flash_loc;
  uint32_t crc;
}fwreq_t;

//! This function gets the size of the firmware
uint32_t get_fw_size(char *buffer)
{
  fwreq_t *fw = (fwreq_t *)buffer;
  return fw->image_size;
}

//! Create a new file on SD card
int8_t openFile_sd()
{
  FRESULT error;

  //! Create a file on SD card
  error = f_open(&g_fileObject_n, _T(FW_FILE_NAME), (FA_WRITE | FA_READ | FA_OPEN_EXISTING));
  if (error)
  {
    PRINTF("Open file failed.\r\n");
    return -1;
  }

  return 0;
}

uint32_t    chunk_cnt, chunk_check, offset, fw_image_size = 0;
int32_t	  	status = RSI_SUCCESS;
uint8_t     recv_buffer[RSI_CHUNK_SIZE] = {0}, fw_version[10]={0};

FRESULT	    error;

int32_t rsi_app_task_fw_update_via_bootloader(void)
{
  {
    switch(rsi_wlan_app_cb.state)
    {
      case RSI_WLAN_INITIAL_STATE:
        {
          error = openFile_sd();
          if (error)
          {
            PRINTF("Open file failed.\r\n");
            return -1;
          }
          LOG_PRINT("DEMO STARTED\n");
          //! update wlan application state
          rsi_wlan_app_cb.state = RSI_WLAN_FW_UPGRADE;

        }
      case RSI_WLAN_FW_UPGRADE:
        {

          error = f_read(&g_fileObject_n, recv_buffer, RSI_CHUNK_SIZE, &bytesRead);

          //! Send the first chunk to extract header
          fw_image_size = get_fw_size(recv_buffer);

          //!caluculate the total number of chunks
          chunk_check = (fw_image_size/RSI_CHUNK_SIZE);
          if(fw_image_size%RSI_CHUNK_SIZE)
            chunk_check += 1;

          //!Loop until all the chunks are burnt 
          while(offset <= fw_image_size)
          {
            if(chunk_cnt == chunk_check)
            {
              LOG_PRINT("fw_upgrade Sucess\n");
              break;
            }
            if(chunk_cnt != 0)
            {
              error = f_read(&g_fileObject_n, recv_buffer, RSI_CHUNK_SIZE, &bytesRead);

            }
            if(chunk_cnt == 0)
            {

              status = rsi_bl_upgrade_firmware((uint8_t *)recv_buffer,RSI_CHUNK_SIZE,RSI_START_OF_FILE);           
              if(status != RSI_SUCCESS)
              {

                break;
              }
              LOG_PRINT("fw_upgrade in progress..\n");
            }
            else if(chunk_cnt == (chunk_check - 1))
            {
              status = rsi_bl_upgrade_firmware((uint8_t *)recv_buffer,RSI_CHUNK_SIZE,RSI_END_OF_FILE);
              if(status != RSI_SUCCESS)
              {

                break;
              }
              LOG_PRINT("fw_upgrade Success\n");
              rsi_wlan_app_cb.state = RSI_WLAN_FW_UPGRADE_DONE;
            }
            else
            {
              status = rsi_bl_upgrade_firmware((uint8_t *)recv_buffer,RSI_CHUNK_SIZE,RSI_IN_BETWEEN_FILE);
              if(status != RSI_SUCCESS)
              {
                break;
              }
              LOG_PRINT("fw_upgrade in progress..\n");
            }
            offset += RSI_CHUNK_SIZE;
            memset(recv_buffer, 0, sizeof(recv_buffer));
            chunk_cnt++;
          }
        }
        break;
      case RSI_WLAN_FW_UPGRADE_DONE :
        {
          status = rsi_wireless_deinit();
          if(status != RSI_SUCCESS)
          {
            return status;
          }

          //! WiSeConnect initialization
          status = rsi_wireless_init(0, 0);
          if(status != RSI_SUCCESS)
          {
            return status;
          }

          memset(fw_version,0,sizeof(fw_version));
          status = rsi_wlan_get(RSI_FW_VERSION,fw_version,sizeof(fw_version));
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT("reading fw version failed\n");
            break;
          }
          LOG_PRINT("fw version after upgrade is: %s\n",fw_version);
          LOG_PRINT("DEMO DONE");
          while(1);
        }

      default:
        break;
    }
  }
}

#endif
