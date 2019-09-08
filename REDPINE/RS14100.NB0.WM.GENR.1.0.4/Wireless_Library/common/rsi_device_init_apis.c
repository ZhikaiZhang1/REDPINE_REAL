/**
 * @file
 * @version
 * @date
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief :
 *
 *  @section Description  This file contains 
 *
 *
 */

/**
 * Includes
 */
#include "rsi_driver.h"
#ifdef SIM_9118
#include "rsi_board.h"
#endif
#ifdef RSI_M4_INTERFACE
#ifdef ROM_WIRELESS
#include "rsi_chip.h"
#include "rsi_board.h"
#endif
#include "core_cm4.h"
#endif
#ifdef LINUX_PLATFORM
#include<stdint.h>
#include <sys/ioctl.h>
#endif
/*==============================================*/
/**
 * @fn      int32_t rsi_bl_module_power_on(void)
 * @brief   This API is used to turn on the module power
 * @param[in]  None  
 * @param[out] None
 * @return    
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This  API is used to turn on the module power
 *
 *
 */
int32_t rsi_bl_module_power_on(void)
{
  int32_t status = RSI_SUCCESS;

#ifndef WISECONNECT_PLUS
#ifndef RSI_M4_INTERFACE
  //! configure power pin in GPIO mode
  rsi_hal_config_gpio(RSI_HAL_MODULE_POWER_CONTROL,RSI_HAL_GPIO_OUTPUT_MODE,RSI_HAL_GPIO_LOW);
  
  //! reset power pin in GPIO mode
  rsi_hal_clear_gpio(RSI_HAL_MODULE_POWER_CONTROL);

  //! delay for 200 milli seconds
  rsi_delay_ms(200);
#endif
#ifdef RSI_SPI_INTERFACE
  //! power cycle the module
  rsi_bl_module_power_cycle();
#endif
#endif

  return status;

}

/*==============================================*/
/**
 * @fn      int32_t rsi_bl_module_power_off(void)
 * @brief   This API is used to turn off the module power
 * @param[in]  None 
 * @param[out] None
 * @return    
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This  API is used to turn off the module power
 */
int32_t rsi_bl_module_power_off(void)
{
  int32_t status = RSI_SUCCESS;

#ifndef WISECONNECT_PLUS
#ifndef RSI_M4_INTERFACE  
  //! configure power pin in GPIO mode
  rsi_hal_config_gpio(RSI_HAL_MODULE_POWER_CONTROL,RSI_HAL_GPIO_OUTPUT_MODE,RSI_HAL_GPIO_LOW);
  
  //! reset/drive low value on the GPIO
  rsi_hal_set_gpio(RSI_HAL_MODULE_POWER_CONTROL);
#endif  

#endif
  
  return status;
}


/*==============================================*/
/**
 * @fn          int32_t rsi_device_init(uint8_t select_option)
 * @brief       This API is used to set the bootup options of the device
 * @param[in]   select_option, 
 * @param[out]  None
 * @return    
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This API is used to set the bootup options of the device 
 */

int32_t rsi_device_init(uint8_t select_option)
{
  int32_t status = RSI_SUCCESS;
  uint32_t timer_instance;
  uint8_t skip_bootload_sequence = 0;
  int32_t retval=0;
#ifdef RSI_M4_INTERFACE

  SysTick_Config(SystemCoreClock / 1000);
  if(!(P2P_STATUS_REG & TA_is_active))
  {
#ifdef DEBUG_UART
    DEBUGOUT("\n wakeup TA \n");
#endif
    P2P_STATUS_REG |= M4_wakeup_TA;
    skip_bootload_sequence = 1;
  }
#ifdef DEBUG_UART
  DEBUGOUT("\n Waiting for TA wakeup \n");
#endif
  while(!(P2P_STATUS_REG & TA_is_active));
#ifdef DEBUG_UART
  DEBUGOUT("\n TA wokeup \n");
#endif

  if(!skip_bootload_sequence)
  {
    do
    {
      retval = rsi_waitfor_boardready();
      if(retval == -7)
      {
#ifdef DEBUG_UART
      DEBUGOUT("\n Waiting for TA wakeup \n");
#endif
      }
      if((retval < 0) && (retval != -3) && (retval != -7))
      {
        return -1;
      }
    }while((retval == -3 ) && (retval == -7));
    retval = rsi_select_option(select_option);
    if(retval < 0)
    {
      return -1;
    }
}
    //! Updating the TX & RX DMA descriptor address
    rsi_update_tx_dma_desc(skip_bootload_sequence);
    rsi_update_rx_dma_desc();
#endif
  
  
#ifdef RSI_USB_INTERFACE
  if(!skip_bootload_sequence)
  {
  do
  {
    retval = rsi_waitfor_boardready();
   if((retval < 0) && (retval != -3) && (retval != -7))
    {
      return -1;
    }
  }while(retval == -3 );
  retval = rsi_select_option(select_option);
  if(retval < 0)
  {
    return -1;
  }
  }
#endif


#ifdef RSI_SPI_INTERFACE
#ifndef WISECONNECT_PLUS
  //! power cycle the module
  rsi_bl_module_power_cycle();

#ifdef RSI_SPI_INTERFACE
  //! SPI interface initialization
  status = rsi_spi_iface_init();

  if(status != RSI_SUCCESS)
  {
    return -1;
  }
#endif
  rsi_init_timer(&timer_instance, 300);
  do {
    status = rsi_bl_waitfor_boardready();

    if((status < 0) && (status != -3))
    {
      return -1;
    }

    if(rsi_timer_expired(&timer_instance))
    {
      return -1;
    }

  }while(status == -3);
  if(select_option == RSI_LOAD_IMAGE_I_FW )
   {
     rsi_set_intr_type(RSI_ACTIVE_HIGH_INTR);
   }
   else if(select_option == RSI_LOAD_IMAGE_I_ACTIVE_LOW_FW)
   {
     rsi_set_intr_type(RSI_ACTIVE_LOW_INTR);
   }
  status = rsi_bl_select_option(select_option);

  if(status < 0)
  {

    return -1;
  }


#else
  //! Configure SLAVE pin GPIO 
  rsi_hal_config_gpio(RSI_HAL_INTERFACE_READY_PIN ,0, 0);
#endif
#endif
#ifdef RSI_M4_INTERFACE
   rsi_m4_ta_interrupt_init();
	if(!(M4SS_P2P_INTR_SET_REG & RX_BUFFER_VALID))
	{	
   		rsi_submit_rx_pkt(global_cb_p);
	}	
#else

  //! Configure interrupt
  rsi_hal_intr_config(rsi_interrupt_handler);

  //! Unmask interrupts
  rsi_hal_intr_unmask();
#endif

  return status;
}

