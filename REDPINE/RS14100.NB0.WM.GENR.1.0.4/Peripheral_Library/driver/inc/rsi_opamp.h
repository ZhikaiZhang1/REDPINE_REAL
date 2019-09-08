/**
 * @file      rsi_opamp.h
 * @version   1.0
 * @date      1 Aug 2017
 *
 *  Copyright(C) Redpine Signals 2017
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief This file contains APIs related to AUX-ADC
 *
 *  @section Description
 *  This file contains APIs related to Analog to Digital converter peripheral
 *
 *
 */
 #include "rsi_chip.h"
 
#ifndef __RSI_OPAMP_H__
#define __RSI_OPAMP_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \cond HIDDEN_SYMBOLS 
 */
 
#define BG_SCDC_PROG_REG_1       0x127
#define LDO_ON            (*(volatile uint32_t *)(0x24043A10) )

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

typedef struct opamp1
{
	uint8_t opamp1_dyn_en;
	uint8_t opamp1_sel_p_mux;
	uint8_t opamp1_sel_n_mux;
	uint8_t opamp1_lp_mode;
	uint8_t opamp1_r1_sel;
	uint8_t opamp1_r2_sel;
	uint8_t opamp1_en_res_bank;
	uint8_t opamp1_res_mux_sel;
	uint8_t opamp1_res_to_out_vdd;
	uint8_t opamp1_out_mux_en;
	uint8_t opamp1_out_mux_sel;
	uint8_t opamp1_enable;
}opamp1_config;

typedef struct opamp2
{
	uint8_t opamp2_dyn_en;
	uint8_t opamp2_sel_p_mux;
	uint8_t opamp2_sel_n_mux;
	uint8_t opamp2_lp_mode;
	uint8_t opamp2_r1_sel;
	uint8_t opamp2_r2_sel;
	uint8_t opamp2_en_res_bank;
	uint8_t opamp2_res_mux_sel;
	uint8_t opamp2_res_to_out_vdd;
	uint8_t opamp2_out_mux_en;
	uint8_t opamp2_enable;
	
}opamp2_config;

typedef struct opamp3
{
	uint8_t opamp3_dyn_en;
	uint8_t opamp3_sel_p_mux;
	uint8_t opamp3_sel_n_mux;
	uint8_t opamp3_lp_mode;
	uint8_t opamp3_r1_sel;
	uint8_t opamp3_r2_sel;
	uint8_t opamp3_en_res_bank;
	uint8_t opamp3_res_mux_sel;
	uint8_t opamp3_res_to_out_vdd;
	uint8_t opamp3_out_mux_en;
	uint8_t opamp3_enable;
}opamp3_config;

typedef struct opamp_config_t
{
	opamp1_config opamp1_t;
	opamp2_config opamp2_t;
	opamp3_config opamp3_t;
}opamp_config;

/**
 * \endcond
 */

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_OPAMP RSI:RS1xxxx OPAMP 
 *  @{
 *
 */


void RSI_Opamp1_Config(OPAMP_Type *opamp,uint32_t channel,opamp_config *config);

void RSI_Opamp2_Config(OPAMP_Type *pstcOpamp,uint32_t channel,opamp_config *config);

void RSI_Opamp3_Config(OPAMP_Type *pstcOpamp, uint32_t channel,opamp_config *config);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif // __RSI_OPAMP_H__
