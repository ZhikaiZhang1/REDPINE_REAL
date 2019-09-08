/**
 * @file       rsi_comparator.h
 * @version    1.0
 * @date       1 Aug 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes releated to COMPARATOR.
 * 
 * @section Description
 * This file contains the list of function prototypes for the COMPARATOR peripheral in different and configuring parameters
 * Following are list of API's which need to be defined in this file.
 *
 */
 
#include "rsi_chip.h"

/** 
 *\cond HIDDEN_SYMBOLS
 */
#define MAX_NUM_OF_COMPARATORS 2 
#define MIN_NUM_OF_COMPARATORS 1
#define MAX_P_SEL_MUX          15
#define MAX_N_SEL_MUX          15
#define MAX_NO_OF_HYST         3
#define MAX_THRSHOLD_VALUE     31
#define MAX_SCALAR_FACTOR      15
#define COMP1                  1
#define COMP2                  2   

#define AGPIO_PIN0    0
#define AGPIO_PIN1    1
#define AGPIO_PIN2    2
#define AGPIO_PIN3    3
#define AGPIO_PIN4    4
#define AGPIO_PIN5    5
#define AGPIO_PIN6    6
#define AGPIO_PIN7    7
#define AGPIO_PIN8    8
#define AGPIO_PIN9    9
#define AGPIO_PIN10   10
#define AGPIO_PIN11   11
#define AGPIO_PIN12   12
#define AGPIO_PIN13   13
#define AGPIO_PIN14   14
#define AGPIO_PIN15   15

#define TGPIO_PIN0   25
#define TGPIO_PIN1   26
#define TGPIO_PIN2   27
#define TGPIO_PIN3   28
#define TGPIO_PIN4   29
#define TGPIO_PIN5   30
#define TGPIO_PIN6   23
#define TGPIO_PIN7   24

#define AGPIO_MODE  7
#define TGPIO_MODE 14

#define AGPIO_PORT  7
#define BG_SCDC_PROG_REG1  0x127

#define COMP1                 1  
#define COMP2                 2
/**
 * \endcond
 */


 /**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 */

/**
 * \defgroup RSI_COMP RSI:RS1xxxx COMPARATOR 
 *  @{
 *
 */
error_t RSI_COMP_Enable(AUX_ADC_DAC_COMP_Type* comp,uint8_t comp_number , uint8_t enable);

error_t RSI_COMP_Config(AUX_ADC_DAC_COMP_Type* comp,uint8_t comp_number , uint8_t sel_p_mux, 
                     uint8_t sel_n_mux   , uint8_t hyst_en  , uint8_t filter_en);
					 
void RSI_COMP_ResistorBank_Enable(AUX_ADC_DAC_COMP_Type* comp,uint8_t enable);
					 
error_t RSI_COMP_ResistorBank_Thrsh(AUX_ADC_DAC_COMP_Type* comp,uint16_t value_thrsh);
					 
error_t RSI_COMP_ReferenceScalarOut(AUX_ADC_DAC_COMP_Type* comp,uint16_t scalar_factor_value);

void RSI_COMP_RefenceBufferOut_Enable(AUX_ADC_DAC_COMP_Type* comp,uint8_t enable);
										 
/**
 * @} end of RSI_COMP 
 */
