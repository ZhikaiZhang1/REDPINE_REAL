/**
 * @file       rsi_wwdt.h
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
 * @brief This files contains functions prototypes related to WDT
 *
 * @section Description
 * This file contains the list of function prototypes for the WDT and low level function definitions
 * Following are list of API's which need to be defined in this file.
 *
 */


/**
 * Includes
 */
#ifndef __RSI_WDT_H__
#define __RSI_WDT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rsi_chip.h"
#include "RS1xxxx.h"

/**
 * @fn        	void RSI_WWDT_ConfigIntrTimer(MCU_WDT_Type *pstcWDT , uint16_t u16IntrTimerVal)
 * @brief     	This API is used to configure the interrupt timer of the watch dog timer
 * @param[in]  	  pstcWDT 	 			 : pointer to the WDT register instance
 * @param[in]  	  u16IntrTimerVal  : interrupt timer value
 * @return			None
 */
STATIC INLINE void RSI_WWDT_ConfigIntrTimer(MCU_WDT_Type *pstcWDT , uint16_t u16IntrTimerVal)
{
	pstcWDT->MCU_WWD_INTERRUPT_TIMER_b.WWD_INTERRUPT_TIMER = u16IntrTimerVal;
}

/**
 * @fn					void RSI_WWDT_ConfigSysRstTimer(MCU_WDT_Type *pstcWDT , uint16_t u16SysRstVal)	
 * @brief   	 	This API is used to configure the system reset timer of the watch dog timer
 * @param[in]	  pstcWDT 			  : pointer to the WDT register instance
 * @param[in] 	  u16SysRstVal 		: reset value
 * @return			NONE
 */
STATIC INLINE void RSI_WWDT_ConfigSysRstTimer(MCU_WDT_Type *pstcWDT , uint16_t u16SysRstVal)
{
	pstcWDT->MCU_WWD_SYSTEM_RESET_TIMER_b.WWD_SYSTEM_RESET_TIMER = u16SysRstVal;
}

/**
 * @fn			 		void RSI_WWDT_Disable(MCU_WDT_Type *pstcWDT)
 * @brief   	 	This API is used to Disable the Watch dog timer
 * @param[in] 	 	pstcWDT  	:pointer to the WDT register instance
 * @return		 	None
 */
STATIC INLINE void RSI_WWDT_Disable(MCU_WDT_Type *pstcWDT)
{
	/*0xF0 to Disable the watch dog */
	pstcWDT->MCU_WWD_MODE_AND_RSTART_b.WWD_MODE_EN_STATUS = 0xF0;
}

/**
 * @fn		 			 void RSI_WWDT_ReStart(MCU_WDT_Type *pstcWDT)
 * @brief   		 This API is used to restart the Watch dog timer
 * @param[in]		 pstcWDT  :pointer to the WDT register instance
 * @return 			 None
 */
STATIC INLINE void RSI_WWDT_ReStart(MCU_WDT_Type *pstcWDT)
{
	pstcWDT->MCU_WWD_MODE_AND_RSTART_b.WWD_MODE_RSTART =1U;
}

/**
 * @fn					 void RSI_WWDT_IntrUnMask(void)
 * @brief   		 This API is used to unmask the Watch dog timer
 * @return		   None
 */
STATIC INLINE void RSI_WWDT_IntrUnMask(void)
{
	NPSS_INTR_MASK_CLR_REG = NPSS_TO_MCU_WDT_INTR;
}

/**
 * @fn					 void RSI_WWDT_IntrMask(void)
 * @brief   		 This API is used to mask the Watch dog timer
 * @return		   None
 */
STATIC INLINE void RSI_WWDT_IntrMask(void)
{
	NPSS_INTR_MASK_SET_REG = NPSS_TO_MCU_WDT_INTR;
}


// Function prototypes 
void RSI_WWDT_IntrClear(void);

uint8_t RSI_WWDT_GetIntrStatus(void);

void RSI_WWDT_DeInit(MCU_WDT_Type *pstcWDT);

void RSI_WWDT_Start(MCU_WDT_Type *pstcWDT);

void RSI_WWDT_Init(MCU_WDT_Type *pstcWDT);

#ifdef __cplusplus
}
#endif


/*End of file not truncated*/
#endif /*__RSI_WDT_H__*/
