/**
 * @file       rsi_rom_ct.h
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
 * @brief This files contains functions prototypes releated to CT peripheral
 * 
 * @section Description
 * This file contains the list of function prototypes for the CT and low level function definations 
 * Following are list of API's which need to be defined in this file.
 *
 */

#ifndef  __RSI_ROM_CT_H__
#define  __RSI_ROM_CT_H__

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_CT_DRIVER  RSI:RS1xxxx CT
 *  @{
 *
 */
#include "rsi_ct.h"
#include "rsi_packing.h"
#include "rsi_rom_table_RS1xxxx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn           STATIC INLINE void  RSI_CT_OCUHighLowToggleSelect(RSI_CT_T *pCT, boolean_t lowHigh,
																		   boolean_t counterNum, uint8_t outputSel)
 * @brief		     This API is used to toggle output state of OCU.
 * @param[in]	   pCT         : Pointer to the CT instance register area
 * @param[in]	   lowHigh     : Output low/high state
 * @param[in]    counterNum  : Counter 0/1
 * @param[in]    outputSel   : OCU possibilities(0 to 7)
 * @return 		   none
 */
STATIC INLINE void  RSI_CT_OCUHighLowToggleSelect( RSI_CT_T *pCT,
		boolean_t lowHigh,
		boolean_t counterNum,
		uint8_t outputSel )
{
#if defined(CT_ROMDRIVER_PRESENT)
	ROMAPI_CT_API ->ct_ocu_high_Low_toggle_select(pCT,lowHigh,counterNum,outputSel);
#else
	ct_ocu_high_Low_toggle_select(pCT, lowHigh,counterNum,outputSel);
#endif
}
/**
 * @fn          STATIC INLINE error_t RSI_CT_WFGControlConfig( RSI_CT_T *pCT, WFG_PARAMS_T ctrlReg )
 * @brief		    This API is used to toggle output state of WFG
 * @param[in]	  pCT         : Pointer to the CT instance register area
 * @param[in]	  ctrlReg     : Variable to a structure of type \ref WFG_PARAMS_T
 * @return 		  \ref ERROR_CT_INVALID_ARG : If WFG_PARAMS_T structure parameters are invalid
                \n \ref RSI_OK            : If process is done successfully
 */
STATIC INLINE error_t RSI_CT_WFGControlConfig(RSI_CT_T *pCT, WFG_PARAMS_T ctrlReg )
{
#if defined(CT_ROMDRIVER_PRESENT)
	return ROMAPI_CT_API ->ct_wfg_control_config(pCT,ctrlReg);
#else
	return ct_wfg_control_config(pCT,ctrlReg);
#endif
}
/**
 * @fn          STATIC INLINE error_t RSI_CT_OCUControl(RSI_CT_T *pCT, boolean_t counterNum,boolean_t dmaEn,
                                          OCU_PARAMS_T *pOCUparams,RSI_CT_CALLBACK_T *pCB)
 * @brief		    This API controls OCU operation
 * @param[in]   pCT        	 : Pointer to the CT instance register area
 * @param[in]   counterNum   : Counter 0/1
 * @param[in]   dmaEn		     : DMA enable flag
                                - \ref 0 : not enable DMA.
                                - \ref 1 : DMA enable.
 * @param[in]   pOCUparams   : Pointer to the OCU parameters structure and its members are as below \ref OCU_PARAMS_T
 *                              - \ref  CompareVal1_0       : Threshold one of Counter 0,possible values  (0x0 to 0xFFFF)
 *                              - \ref  CompareVal2_0 	    : Threshold two of Counter 0,possible values (0x0 to 0xFFFF)
 *                              - \ref  CompareVal1_1 	    : Threshold two of Counter 1,possible values (0x0 to 0xFFFF)
 *                              - \ref  CompareVal2_1 	    : Threshold two of Counter 1,possible values (0x0 to 0xFFFF)
 *                              - \ref  CompareVal1_0 	    : Threshold two of Counter 0,possible values (0x0 to 0xFFFF)
 *                              - \ref  CompareNextVal2_0 	: next compare value of counter 0,possible values (0x0 to 0xFFFF)
 *                              - \ref  CompareNextVal1_1 	: next compare value of counter 1,possible values (0x0 to 0xFFFF)
 *                              - \ref  CompareNextVal2_1 	: next compare value of counter 1,possible values (0x0 to 0xFFFF)
 *                              - \ref  SyncValue         	: Holds the starting point of counters for synchronization purpose
 *                                                          This is logical OR of sync values for counter 0& 1,
 *                                                          possible values for two counters (0x0 to 0xFFFF).
 * @param[in]   pCB          : Pointer to CT callback structure \ref RSI_CT_CALLBACK_T
 * @return 	    \ref ERROR_CT_INVALID_ARG : If passed parameters are invalid
                \n \ref RSI_OK            : If process is done successfully
 */
STATIC INLINE error_t RSI_CT_OCUControl( RSI_CT_T *pCT,
		boolean_t counterNum,
		boolean_t dmaEn,
		OCU_PARAMS_T *pOCUparams,
		RSI_CT_CALLBACK_T *pCB)
{
#if defined(CT_ROMDRIVER_PRESENT)
	return ROMAPI_CT_API ->ct_ocu_control(pCT,counterNum,dmaEn,pOCUparams, pCB);
#else
	return ct_ocu_control(pCT,counterNum,dmaEn,pOCUparams, pCB);
#endif
}
/**
 * @fn           STATIC INLINE error_t RSI_CT_WFGComapreValueSet( RSI_CT_T *pCT,
                                                    boolean_t counterNum,
																	                  OCU_PARAMS_T *pOCUparams)
 * @brief		     This function controls WFG operation
 * @param[in]	   pCT          : Pointer to the CT instance register area
 * @param[in]    counterNum   : Counter 0/1
 * @param[in]    pOCUparams   : Pointer to the OCU parameters structure and its members are as below \ref OCU_PARAMS_T
 *									               - \ref  CompareVal1_0   : Threshold one of Counter 0,possible values  (0x0 to 0xFFFF)
 *									               - \ref  CompareVal2_0   : Threshold two of Counter 0,possible values  (0x0 to 0xFFFF)
 *									               - \ref  CompareVal1_1   : Threshold one of Counter 1,possible values  (0x0 to 0xFFFF)
 *									               - \ref  CompareVal2_1   : Threshold one of Counter 1,possible values  (0x0 to 0xFFFF)
 *                                 - \ref  SyncValue       : Holds the starting point of counters for synchronization purpose
 *			                                                    This is logical OR of sync values for counter 0& 1
 *															                            possible values for two counters (0x0 to 0xFFFF)
 * @return 		  \ref ERROR_CT_INVALID_ARG : If passed parameters are invalid
                \n \ref RSI_OK            : If process is done successfully
 */
STATIC INLINE error_t RSI_CT_WFGComapreValueSet( RSI_CT_T *pCT,
		boolean_t counterNum,
		OCU_PARAMS_T *pOCUparams)
{
#if defined(CT_ROMDRIVER_PRESENT)
	return ROMAPI_CT_API ->ct_wfg_comapre_value_set(pCT,counterNum,pOCUparams);
#else
	return ct_wfg_comapre_value_set(pCT,counterNum,pOCUparams);
#endif
}

#ifdef __cplusplus
}
#endif	

#endif /*__RSI_ROM_CT_H__*/

/* @} end of  RSI_CT_DRIVER */
