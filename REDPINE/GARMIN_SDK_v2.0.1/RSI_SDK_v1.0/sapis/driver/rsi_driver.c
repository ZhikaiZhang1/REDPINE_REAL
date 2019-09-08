/**
 * @file     rsi_driver.c
 * @version  0.1
 * @date     15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contians core driver functionality
 *
 *  @section Description  This file contains core driver functionality
 *
 *
 */
/**
 * Include files
 * */
#include "rsi_driver.h"

extern void rsi_set_event_from_isr(uint32_t event_num);
/*===================================================*/
/**
 * @fn			void rsi_interrupt_handler(void)
 * @brief		interrupt handler to handle interrupt coming from module
 * @param[in]	none
 * @param[out]  none
 * @return	    none
 * @description This API is the call back for handling packet pedning interrupt from module
 */

void  rsi_interrupt_handler(void)
{
#ifndef RSI_M4_INTERFACE
  //! Disable the interrupt 
  rsi_hal_intr_mask();
#ifdef RSI_WITH_OS
  //! Set event RX pending from device
  rsi_set_event_from_isr(RSI_RX_EVENT);
#else
  //! Set event RX pending from device
  rsi_set_event(RSI_RX_EVENT);
#endif
#endif
  return;
}



