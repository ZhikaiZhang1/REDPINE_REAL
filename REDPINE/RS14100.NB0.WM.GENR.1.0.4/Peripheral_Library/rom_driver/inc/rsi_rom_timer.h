/**
 * @file       rsi_rom_timer.h
 * @version    1.0
 * @date       1 AUG 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes releated to TIMERS peripheral
 * 
 * @section Description
 * This file contains the list of function prototypes for the TIMERS and low level function definations 
 * Following are list of API's which need to be defined in this file.
 *
 */

#ifndef  __RSI_ROM_TIMER_H__
#define  __RSI_ROM_TIMER_H__

#include "rsi_timers.h"
#include "rsi_packing.h"
#include "rsi_rom_table_RS1xxxx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_TIMER RSI:RS1xxxx TIMER 
 *  @{
 *
 */	


/**
 * @fn          STATIC INLINE error_t  RSI_TIMERS_MicroSecTimerConfig(RSI_TIMERS_T *pTIMER, uint8_t timerNum,
 *																				uint16_t integer,uint8_t fractional,uint8_t mode)
 * @brief		    This API is used to configure timer mode as a 1 micro second or 256 micro second mode
 * @param[in]		pTIMER			: Pointer to the TIMERS instance register area
 * @param[in]   timerNum    : Timer number(0 to 3)
 * @param[in]   integer     : Integer part of number of clock cycles
 *														\n per microsecond of the system clock being used
 * @param[in]   fractional  : Fractional part of clock cycles per microsecond of the system clock being used
 * @param[in]   mode        : timer mode
 *                               - \ref MICRO_SEC_MODE
 *                               - \ref _256_MICRO_SEC_MODE
 *                               - \ref COUNTER_DOWN_MODE
 * @return 		  - \ref TIMERS_INVAL_TIMER_MODE       : If timer mode is invalid
 *              - \ref TIMERS_INVAL_TIMER_NUM_ERROR  : If timer number is invalid
 *              - \ref RSI_OK                        : If process is done
 */
STATIC INLINE error_t RSI_TIMERS_MicroSecTimerConfig(RSI_TIMERS_T *pTIMER, uint8_t timerNum,
		uint16_t integer,uint8_t fractional,
		uint8_t mode)
{
#if defined(TIMER_ROMDRIVER_PRESENT)
	return ROMAPI_TIMER_API ->timers_microsec_timer_config(pTIMER,timerNum,integer,fractional, mode);
#else
	return timers_microsec_timer_config(pTIMER,timerNum,integer,fractional, mode);
#endif

}

/**
 * @fn          STATIC INLINE uint32_t  RSI_TIMERS_ReadTimer(RSI_TIMERS_T *pTIMER, uint8_t timerNum,boolean_t countDir)
 * @brief		    This API is used to get the count of the required timer
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerNum   : Timer number(0 to 3)
 * @param[in]   countDir   : For reading/tracking counter in up counting this bit has to be set.
 * @return 		  Timer count value
 */
STATIC INLINE uint32_t  RSI_TIMERS_ReadTimer(RSI_TIMERS_T *pTIMER, uint8_t timerNum,boolean_t countDir)
{
#if defined(TIMER_ROMDRIVER_PRESENT)
	return ROMAPI_TIMER_API ->timers_read_timer(pTIMER,timerNum,countDir);
#else
	return timers_read_timer(pTIMER,timerNum,countDir);
#endif

}

		//prototypes
		error_t RSI_TIMERS_MicroSecTimerConfig(RSI_TIMERS_T *pTIMER, uint8_t timerNum,
				uint16_t integer,uint8_t fractional,
				uint8_t mode);


		uint32_t RSI_TIMERS_ReadTimer(RSI_TIMERS_T *pTIMER, uint8_t timerNum,boolean_t countDir);


#ifdef __cplusplus
}
#endif


#endif /* __RSI_ROM_TIMER_H__ */

/* @} end of RSI_TIMER  */
