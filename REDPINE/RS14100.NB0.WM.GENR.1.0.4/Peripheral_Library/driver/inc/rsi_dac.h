/**
 * @file      rsi_dac.h
 * @version   1.0
 * @date      4 Aug 2018
 *
 *  Copyright(C) Redpine Signals 2017
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief This file contains functions prototypes related to AUX-ADC peripheral
 *
 *  @section Description 
 *  this file contains list of functions prototypes for the AUX-ADC peripheral
 *
 *
 */

#ifndef __RSI_AUX_DAC_H__
#define __RSI_AUX_DAC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rsi_chip.h"
//#include "base_types.h"

/******************************************************
 * *                      Macros
******************************************************/ 
#define	 MAX_DIV_FAC_DAC    1023
#define  MAX_DAC_FIFO_THR   7 
	/******************************************************
 * *                    Structures
 **/

/*
\brief DAC channel Configuration structure
*/
	
typedef enum
{
	DAC_POWER_ON =0,
	DAC_POWER_OFF
}POWER_STATE_DAC;


/******************************************************
 * *                 Global Variables
 * ******************************************************/
/******************************************************
 * *               Function Declarations
 * ******************************************************/
 
error_t RSI_DAC_ClkDivFactor(AUX_ADC_DAC_COMP_Type *pstcDAC, uint16_t div_factor);
void RSI_DAC_PowerControl(POWER_STATE_DAC state);
error_t RSI_DAC_Config(AUX_ADC_DAC_COMP_Type *pstcDAC,uint8_t static_fifo_mode,
                       uint16_t aux_dac_out_mux_en,uint16_t aux_dac_out_mux_sel);
error_t RSI_DAC_WriteData(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t *data,
                                uint8_t static_fifo_mode,uint16_t len);
error_t RSI_DAC_DynamicModeWriteData(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t channel,
                                     uint16_t *data,uint32_t len);
uint16_t RSI_DAC_DyanamicModeReadData(AUX_ADC_DAC_COMP_Type *pstcDAC, uint32_t channel,uint16_t data);
error_t RSI_DAC_Stop( AUX_ADC_DAC_COMP_Type *pstcDAC);
error_t RSI_DAC_Start( AUX_ADC_DAC_COMP_Type *pstcDAC ,uint16_t aux_dac_en);
error_t RSI_DAC_DynamicModeConfig(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t channel_no,uint16_t aux_dac_out_mux_en,
                        uint16_t aux_dac_out_mux_sel);
error_t RSI_DAC_DynamicModeStart( AUX_ADC_DAC_COMP_Type *pstcDAC ,uint32_t channel,
                                   uint16_t aux_dac_en);
error_t RSI_DAC_SetFifoThreshold(AUX_ADC_DAC_COMP_Type *pstcDAC,uint32_t fifo_threshold);	
uint16_t RSI_DAC_ReadData(AUX_ADC_DAC_COMP_Type *pstcDAC);
#ifdef __cplusplus
}
#endif

#endif // __RSI_AUX_DAC_H__
