/**
 * @file       rsi_hal_mcu_platform_init.c
 * @version    0.1
 * @date       11 OCT 2018
 *
 * Copyright(C) Redpine Signals 2018
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief HAL Board Init: Functions related to platform initialization
 *
 * @section Description
 * This file contains the list of functions for configuring the microcontroller clock.
 * Following are list of API's which need to be defined in this file.
 *
 */


/**
 * Includes
 */
#ifdef RSI_ENABLE_DEMOS
#include "rsi_common_app.h"
#endif
#include "rsi_driver.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_sysmpu.h"
#include "fsl_dspi.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


extern dspi_master_config_t spi_init(void);
extern dspi_master_config_t spi_dma_init(void);
#ifdef RSI_ENABLE_DEMOS
extern int8_t sd_filesystem_init(void);
extern int sdram_init(void);
#endif
uint8_t platform_initialised;
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*==============================================*/
/**
 * @fn           void rsi_hal_board_init()
 * @brief        This function Initializes the platform
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * This function initializes the platform
 *
 */

void rsi_hal_board_init()
{
  if(!platform_initialised)
  {
    //! Initializes the platform
    BOARD_InitPins();
    //BOARD_BootClockRUN();
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();
    SYSMPU_Enable(SYSMPU, false);

#ifdef RSI_ENABLE_DEMOS
#ifndef RSI_WITH_OS
    //Initialize systick
#if TICK_100_USEC
    //! Set systick reload value to generate 100us interrupt
    if(SysTick_Config(SystemCoreClock / 10000U))
#elif TICK_1_MSEC
      //! Set systick reload value to generate 1ms interrupt
      if(SysTick_Config(SystemCoreClock / 1000U))
#endif
      {
        while(1)
        {
        }
      }

#endif
    //! SDRAM Init
    sdram_init();

    //! Initialize SD FILESYSTEM
    sd_filesystem_init();
#else
    //! Set systick reload value to generate 1ms interrupt
    if(SysTick_Config(SystemCoreClock / 1000U))

    {
      while(1)
      {
      }
    }
#endif

    //! Initialize SPI
#ifndef SPI_DMA
    spi_init();
#else
    spi_dma_init();
#endif
    platform_initialised = 1;
  }
}


/*==============================================*/
/**
 * @fn           void rsi_switch_to_high_clk_freq()
 * @brief        This function intializes SPI to high clock
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * This function intializes SPI to high clock
 *
 *
 */

void rsi_switch_to_high_clk_freq()
{
  //! Initializes the high clock
}




