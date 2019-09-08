/**
 * @file       rsi_hal_mcu_timer.c
 * @version    0.1
 * @date       15 Aug 2015
 *
 * Copyright(C) Redpine Signals 2015
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief HAL TIMER: Functions related to HAL timers
 *
 * @section Description
 * This file contains the list of functions for configuring the microcontroller timers.
 * Following are list of API's which need to be defined in this file.
 *
 */


/**
 * Includes
 */


#include "rsi_driver.h"
#if (defined(RSI_WITH_OS) && defined(FRDM_K28F))
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "stack_macros.h"

extern volatile TickType_t xTickCount;
#endif

#ifdef RSI_M4_INTERFACE
static volatile uint32_t 		_dwTickCount  ;		//systick cout variable
#endif


#ifndef RSI_WITH_OS
#ifdef FRDM_K28F
/*! @brief Time variable as sys ticks. */
volatile uint32_t g_systicks;

void SysTick_Handler(void)
{
	g_systicks++;
}
#endif
#endif
/*===================================================*/
/**
 * @fn           int32_t rsi_timer_start(uint8_t timer_no, uint8_t mode,uint8_t type,uint32_t duration,void (* rsi_timer_expiry_handler)())
 * @brief        Starts and configures timer
 * @param[in]    timer_node, timer node to be configured.
 * @param[in]    mode , mode of the timer
 *               0 - Micro seconds mode
 *               1 - Milli seconds mode
 * @param[in]    type, type of  the timer
 *               0 - single shot type
 *               1 - periodic type
 * @param[in]    duration, timer duration
 * @param[in]    rsi_timer_expiry_handler() ,call back function to handle timer interrupt
 * @param[out]   none
 * @return       0 - success
 *               !0 - Failure
 * @description  This HAL API should contain the code to initialize the timer and start the timer
 *
 */

int32_t rsi_timer_start(uint8_t timer_node, uint8_t mode, uint8_t type, uint32_t duration, void (* rsi_timer_expiry_handler)(void))
{

  //! Initialise the timer


  //! register the call back


  //! Start timer

  return 0;
}


/*===================================================*/
/**
 * @fn           int32_t rsi_timer_stop(uint8_t timer_no)
 * @brief        Stops timer
 * @param[in]    timer_node, timer node to stop
 * @param[out]   none
 * @return       0 - success
 *               !0 - Failure
 * @description  This HAL API should contain the code to stop the timer
 *
 */

int32_t rsi_timer_stop(uint8_t timer_node)
{

	//! Stop the timer

	return 0;


}




/*===================================================*/
/**
 * @fn           uint32_t rsi_timer_read(uint8_t timer_node)
 * @brief        read timer
 * @param[in]    timer_node, timer node to read
 * @param[out]   none
 * @return       timer value
 * @description  This HAL API should contain API to  read the timer
 *
 */

uint32_t rsi_timer_read(uint8_t timer_node)
{

	volatile uint32_t timer_val = 0;

	//! read the timer and return timer value

	return timer_val;


}






/*===================================================*/
/**
 * @fn           void rsi_delay_us(uint32_t delay)
 * @brief        create delay in micro seconds
 * @param[in]    delay_us, timer delay in micro seconds
 * @param[out]   none
 * @return       none
 * @description  This HAL API should contain the code to create delay in micro seconds
 *
 */
void rsi_delay_us(uint32_t delay_us)
{
    //! call the API for delay in micro seconds
#ifdef FRDM_K28F
	uint32_t start;

	if (delay_us == 0)
		return;
#if TICK_100_USEC
	delay_us = delay_us/100;
#endif
#ifdef RSI_WITH_OS
	start = xTickCount;
#else	
	start = g_systicks;
#endif	
	do {
#ifdef RSI_WITH_OS
	} while ((xTickCount - start) < delay_us);
#else
	} while ((g_systicks - start) < delay_us);
#endif
	return;
#endif
}


#ifdef RSI_M4_INTERFACE

 extern void SysTick_Handler(void);

void SysTick_Handler(void)
{
	_dwTickCount++;
}
uint32_t GetTickCount( void )
{
	return _dwTickCount ;						// gets the tick count from systic ISR
}
#endif
/*===================================================*/
/**
 * @fn           void rsi_delay_ms(float delay)
 * @brief        create delay in micro seconds
 * @param[in]    delay, timer delay in micro seconds
 * @param[out]   none
 * @return       none
 * @description  This HAL API should contain the code to create delay in micro seconds
 *
 */
void rsi_delay_ms(uint32_t delay_ms)
{
#ifdef RSI_M4_INTERFACE
	uint32_t start;

	if (delay_ms == 0)
		return;
	 start = GetTickCount();
	do {
	} while (GetTickCount() - start < delay_ms);
	return;
#endif	
#ifdef FRDM_K28F
#if TICK_1_MSEC
	uint32_t start;

	if (delay_ms == 0)
		return;

#ifdef RSI_WITH_OS
	start = xTickCount;
#else	
	start = g_systicks;
#endif	
	do {
#ifdef RSI_WITH_OS
	} while ((xTickCount - start) < delay_ms);
#else
	} while ((g_systicks - start) < delay_ms);
#endif

#elif TICK_100_USEC
	rsi_delay_us((uint32_t)(delay_ms * 1000U));
#endif
	return;
#endif	
}

#if (defined(RSI_WITH_OS) && defined(FRDM_K28F))
void rsi_os_delay_ms(uint32_t delay_ms)
{
volatile unsigned long  i = 0;
volatile unsigned long  j =  4000 * delay_ms;
for(i = 0; i < j; i++)
{
	__asm("NOP");
}
}
#endif
/*===================================================*/
/**
 * @fn           uint32_t rsi_hal_gettickcount()
 * @brief        provides a tick value in milliseconds
 * @return       tick value
 * @description  This HAL API should contain the code to read the timer tick count value in milliseconds 
 *
 */

uint32_t rsi_hal_gettickcount(void)
{
  //! Define your API to get the tick count delay in milli seconds from systic ISR and return the resultant value
#ifdef RSI_WITH_OS
	return xTickCount;
#elif defined(FRDM_K28F)
	return g_systicks;
#else
    return 0;

#endif
}
