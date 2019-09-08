/**
 * @file     rsi_sdio_iface_init.c
 * @version  0.1
 * @date     15 Aug 2015
 *
 * Copyright(C) Redpine Signals 2015
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief SPI INIT: Functions which initiaize the SPI hardware interface in the module
 *
 * @section Description
 * This file contains the SPI Initialization function.
 * This function enables the SPI interface on the Wi-Fi chip.
 * This function is only run once during startup, after power-on, or reset
 *
 *
 */


/**
 * Includes
 */
#ifdef RSI_SDIO_INTERFACE
#include "rsi_driver.h"
#include "rsi_board.h"
#include "rsi_chip.h"
#include "rsi_events.h"
#include "rsi_sdio.h"


/**
 * Global Variables
 */



	
	
/*=============================================*/
/**
 * @fn                  int16_t rsi_sdio_iface_init(void)
 * @brief               Start the SDIO interface
 * @param[in]           none
 * @param[out]          none
 * @return              errCode
 *                      -1 = SDIO busy / Timeout
 *                      -2 = SDIO Failure
 *                      0  = SUCCESS
 * @section description         
 * This API initializes the Wi-Fi module’s Slave SDIO interface.
 */


int16_t rsi_sdio_iface_init(void)
{ 
  error_t Stat = RSI_OK;
  uint8_t data;	

  //! Do enumeration
  Stat = rsi_mcu_sdio_init();

  if(Stat != RSI_OK)
  {
    return Stat;
  }

  //! SDIO_READ_START_LVL
  data = 0x24;
  Stat = rsi_reg_wr(0xfc, &data);
  if(Stat != RSI_OK)
  {
    return Stat;
  }

  //! SDIO_READ_FIFO_CTL
  data = 0x60;
  Stat = rsi_reg_wr(0xfd, &data);
  if(Stat != RSI_OK)
  {
    return Stat;
  }

  //! SDIO_WRITE_FIFO_CTL
  data = 0x20;
  Stat = rsi_reg_wr(0xfe, &data);
  if(Stat != RSI_OK)
  {
    return Stat;
  }
}
#endif
