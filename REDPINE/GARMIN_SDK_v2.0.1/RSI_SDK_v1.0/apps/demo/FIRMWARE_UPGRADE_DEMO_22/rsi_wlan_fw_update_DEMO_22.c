/**
 * @file    rsi_fw_update_DEMO_22.c
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
#if FIRMWARE_UPGRADE
//! socket include file to firmware upgrade APIs
#include "rsi_firmware_upgradation.h"
#include "stdlib.h"
#include "rsi_driver.h"
#include "ff.h"
#include "diskio.h"

//! application control block
rsi_wlan_app_cb_t rsi_wlan_app_cb;

#define FW_FILE_NAME "/RS9116.rps"
uint8_t header=0;
uint16_t  fwup_chunk_length;
//! SD Card parameters
UINT bytesRead;
FIL g_fileObject_n;   /* File object */

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

void  rsi_app_task_fw_update(void)
{
  int32_t		status = RSI_SUCCESS;
  uint8_t     recv_buffer[1500] = {0}, fw[10]={0};
  FRESULT	    error;
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
          LOG_PRINT("DEMO STARTED\n");
          //! update wlan application state
          rsi_wlan_app_cb.state = RSI_WLAN_RADIO_INIT_STATE;

        }
      case RSI_WLAN_RADIO_INIT_STATE:
        {
          status = rsi_wlan_get(RSI_FW_VERSION,fw,sizeof(fw));
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT("reading fw version failed\n");
            break;
          }
          LOG_PRINT("fw version before upgrade is: %s\n",fw);
          //! WLAN Radio Init
          status = rsi_wlan_radio_init();
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT("wlan radio init failed\n");
            break;
          }
          LOG_PRINT("WLAN RADIO Init success\n");
          error = openFile_sd();
          if (error)
          {
            PRINTF("Open file failed.\r\n");
            return -1;
          }
          rsi_wlan_app_cb.state = RSI_WLAN_FW_UPGRADE;
        }
      case RSI_WLAN_FW_UPGRADE:
        {
          if(!header)
          {
            error = f_read(&g_fileObject_n, recv_buffer, 64, &bytesRead);
            //! Send RPS header which is received as first chunk
            status = rsi_fwup_start(recv_buffer);
            if(status != RSI_SUCCESS)
            {
              LOG_PRINT("fw_upgrade header failure\n");

              return status;
            }
            header = 1;
          }
          else
          {
            error = f_read(&g_fileObject_n, recv_buffer, 1024, &bytesRead);
            //! Send RPS content
            status = rsi_fwup_load(recv_buffer, bytesRead);
            if(status != RSI_SUCCESS)
            {
              if(status == 3)
              {
                LOG_PRINT("fw_upgrade success\n");
                rsi_wlan_app_cb.state = RSI_WLAN_FW_UPGRADE_DONE;
#ifndef RSI_WITH_OS
                return 0;
#else
                continue;
#endif
              }
              else
              {
                LOG_PRINT("fw_upgrade content failure\n");
                return status;
              }
            }
            LOG_PRINT("fw_upgrade in progress..\n");

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

          memset(fw,0,sizeof(fw));
          status = rsi_wlan_get(RSI_FW_VERSION,fw,sizeof(fw));
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT("reading fw version failed\n");
            break;
          }
          LOG_PRINT("fw version after upgrade is: %s\n",fw);
          LOG_PRINT("DEMO DONE");
          while(1);
        }

      default:
        break;
    }
  }
}

#endif
