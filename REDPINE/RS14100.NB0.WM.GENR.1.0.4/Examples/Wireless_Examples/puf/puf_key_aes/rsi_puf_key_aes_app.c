/**
 * @file    rsi_puf_aes_app.c
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
 *  @section Description  This file contains example application for Starting PUF in module and usage of AES engine
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

//! PUF include file for PUF Apis
#include "rsi_puf.h"

#ifdef RSI_ENABLE_DEBUG_PRINT
#include <stdio.h>
#endif

//! Receive data length
#define RECV_BUFFER_SIZE   1027

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

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//!Global buffers to provide inputs
//!Size of data should be multiples of key_size(in bytes)
#define RSI_AES_PLAIN_TXT " Redpine module is based on Redpine Signals ultra-low-power, single spatial stream, dual-band 802.11n + Bluetooth (BT4.0) + ZigBee (ZB) Convergence SoC. The Redpine module is a complete IEEE 802.11 a/b/g/n ZB and BT wireless device ser"

//! AES Key to be used to encrypt and decrypt
#define RSI_AES_KEY "123456abcdefghijklmnopqrstuvwxyz"

//! AES CBC mode IV
#define RSI_AES_CBC_IV "abcdefghijklmnop"

//! Function to use PUF to encrypt and decrypt data using key encrypted by PUF
int32_t rsi_puf_aes_encry_decry_app()
{
  int32_t     status       = RSI_SUCCESS;
  //Buffers to store responses
  uint8_t aes_encry_data[256];
  uint8_t aes_decry_data[256];
  uint8_t get_key[MAX_KEY_SIZE_BYTES], keycode[KEY_CODE_SIZE_BYTES];

  //! WC initialization
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //!Start PUF
  status = rsi_puf_start_req();
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n PUF Start Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n PUF Start Success \n");
#endif

  //!Set Key(16Bytes) operation with PUF
  status = rsi_puf_set_key_req(1, PUF_KEY_SIZE_128 , RSI_AES_KEY, keycode, KEY_CODE_SIZE_BYTES );
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Set Key Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n PUF set key Success 16Bytes Size \n");
#endif

  //!Get Key(16Bytes) operation with PUF
  status = rsi_puf_get_key_req(keycode, get_key, KEY_CODE_SIZE_BYTES );
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Get Key Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n PUF Get key Success 16Bytes Size \n");
#endif

  //!Load Key(16Bytes) operation with PUF
  status = rsi_puf_load_key_req(keycode);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Load Key to PUF/AES Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n PUF Load key Success 16Bytes Size \n");
#endif

  //!Encrypt using AES ECB mode
  memset(aes_encry_data, 0, 256);
  status = rsi_puf_aes_encrypt_req(AES_ECB_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, 256, RSI_AES_PLAIN_TXT, NULL, NULL, aes_encry_data, 256);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Encryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Encryption Done with AES ECB 16bytes key Data Size 256\n");
#endif

  //! Decryption of data AES ECB mode
  memset(aes_decry_data, 0, 256);
  status = rsi_puf_aes_decrypt_req(AES_ECB_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, 256, aes_encry_data, NULL, NULL, aes_decry_data, 256);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Decryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Decryption Done with AES ECB 16bytes key Size\n");
#endif

  //!Encrypt using AES CBC mode 128bit Key
  memset(aes_encry_data, 0, (256));
  status = rsi_puf_aes_encrypt_req(AES_CBC_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, (256), RSI_AES_PLAIN_TXT, PUF_IV_SIZE_128, RSI_AES_CBC_IV, aes_encry_data, (256));
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Encryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Encryption Done with AES CBC 16bytes key Data Size 256 \n");
#endif

  //!Decrypt using AES CBC mode 128bit Key
  memset(aes_decry_data, 0, (256));
  status = rsi_puf_aes_decrypt_req(AES_CBC_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, (256), aes_encry_data, 0, RSI_AES_CBC_IV, aes_decry_data, (256) );
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Decryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Decryption Done with AES CBC 16bytes key Size 256 ");
#endif

  /**************************************************************************/
  //!Set Key(32Bytes) operation with PUF
  memset(keycode, 0, KEY_CODE_SIZE_BYTES);
  status = rsi_puf_set_key_req(1, PUF_KEY_SIZE_256 , RSI_AES_KEY, keycode, KEY_CODE_SIZE_BYTES );
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Set Key Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n PUF set key Success 32Bytes Size \n");
#endif

  //!Get Key(32Bytes) operation with PUF
  memset(get_key, 0, MAX_KEY_SIZE_BYTES);
  status = rsi_puf_get_key_req(keycode, get_key, MAX_KEY_SIZE_BYTES);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Get Key Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n PUF Get key Success 32Bytes Size \n");
#endif

  //!Load Key(32Bytes) operation with PUF
  status = rsi_puf_load_key_req(keycode);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Load Key to PUF/AES Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n PUF Load key Success 32Bytes Size \n");
#endif

  //!Encrypt using AES ECB mode
  memset(aes_encry_data, 0, 256);
  status = rsi_puf_aes_encrypt_req(AES_ECB_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_256, NULL, 32, RSI_AES_PLAIN_TXT, NULL, NULL, aes_encry_data, 32);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Encryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Encryption Done with AES ECB 32bytes key\n");
#endif

  //! Decryption with AES ECB 32bytes key
  memset(aes_decry_data, 0, 256);
  status = rsi_puf_aes_decrypt_req(AES_ECB_MODE, PUF_AS_KEY_SOURCE, 1, NULL, 32, aes_encry_data, NULL, 0, aes_decry_data, 32 );
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Decryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Decryption Done with AES ECB 32bytes key Size \n");
#endif


  //!Encrypt using AES CBC mode 32Bytes key, data size 256Bytes
  memset(aes_encry_data, 0, (256));
  status = rsi_puf_aes_encrypt_req(AES_CBC_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_256, NULL, 256, RSI_AES_PLAIN_TXT, PUF_IV_SIZE_128, RSI_AES_CBC_IV, aes_encry_data, 256);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES CBC 256bit key Encryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n AES CBC 256bit key Encryption Success \n");
#endif

  //!Decrypt using AES CBC mode 32Bytes key, data size 256Bytes
  memset(aes_decry_data, 0, (256));
  status = rsi_puf_aes_decrypt_req(AES_CBC_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_256, NULL, 256, aes_encry_data, PUF_IV_SIZE_128, RSI_AES_CBC_IV, aes_decry_data, 256 );
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES CBC 256bit key Decryption Fail Status %d \n", status);
#endif
    return status;
  }


#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Decryption Done with AES CBC 256bit Key\n");
#endif
  status = rsi_puf_intr_key_req(1, PUF_KEY_SIZE_128 , keycode, KEY_CODE_SIZE_BYTES );
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Intr Key Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n PUF Intr key Success 16Bytes Size \n");
#endif


  //!Load Key(16Bytes) operation with PUF
  status = rsi_puf_load_key_req(keycode);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n Load Key to PUF/AES Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n PUF Load key Success 16Bytes Size \n");
#endif




  //!Encrypt using AES ECB mode
  memset(aes_encry_data, 0, 256);
  status = rsi_puf_aes_encrypt_req(AES_ECB_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, 256, RSI_AES_PLAIN_TXT, NULL, NULL, aes_encry_data, 256);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Encryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Encryption Done with AES ECB 16bytes key Data Size 256\n");
#endif

  //! Decryption of data AES ECB mode
  memset(aes_decry_data, 0, 256);
  status = rsi_puf_aes_decrypt_req(AES_ECB_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, 256, aes_encry_data, NULL, NULL, aes_decry_data, 256);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Decryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Decryption Done with AES ECB 16bytes key Size\n");
#endif

  //!Encrypt using AES CBC mode 128bit Key
  memset(aes_encry_data, 0, (256));
  status = rsi_puf_aes_encrypt_req(AES_CBC_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, (256), RSI_AES_PLAIN_TXT, PUF_IV_SIZE_128, RSI_AES_CBC_IV, aes_encry_data, (256));
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Encryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Encryption Done with AES CBC 16bytes key Data Size 256 \n");
#endif

  //!Decrypt using AES CBC mode 128bit Key
  memset(aes_decry_data, 0, (256));
  status = rsi_puf_aes_decrypt_req(AES_CBC_MODE, PUF_AS_KEY_SOURCE, PUF_KEY_SIZE_128, NULL, (256), aes_encry_data, 0, RSI_AES_CBC_IV, aes_decry_data, (256) );
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES Decryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Decryption Done with AES CBC 16bytes key Size 256 ");
#endif
#ifdef RSI_ENABLE_DEBUG_PRINT

  printf("\n\n\nDone\n\n\n");
#endif

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

  //! Application main loop
  main_loop();
#endif
  return status;

}
