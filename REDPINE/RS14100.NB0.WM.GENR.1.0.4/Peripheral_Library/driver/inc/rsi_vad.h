/**
 * @file       rsi_vad.h
 * @version    0.1
 * @date       8 jan 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes releated to VAD.
 * 
 * @section Description
 * This file contains the list of function prototypes for the SDIO slave  in different and configuring parameters
 * Following are list of API's which need to be defined in this file.
 *
 */
 
#ifndef __RSI_VAD_H__
#define __RSI_VAD_H__

#include "RS1xxxx.h"
#include "rsi_error.h"

/* VAD algorithm select enum*/
typedef enum VAD_ALGORITHM_SELECT
{
 ZCR,     /* select the ZCR algorithm for VAD detection */
 ACF,     /* select the ACF algorithm for VAD detection */
 AMDF,    /* select the AMDF algorithm for VAD detection */
 WACF,    /* select the WACF algorithm for VAD detection */
 ZCR_ACF_AMDF_WACF, /* select the ZCR,ACF,AMDF and WACF algorithm for VAD detection */
 ZCR_ACF,           /* select the ZCR and ACF algorithm for VAD detection */ 
 ZCR_AMDF,          /* select the ZCR and ACF algorithm for VAD detection */
 ZCR_WACF 	        /* select the ZCR and WACF algorithm for VAD detection */
}VAD_ALGORITHM_SELECT_T;

/* AMDF threshold configuration structure */ 
typedef struct 
{
  uint32_t null_threshold       : 12;
	uint32_t null_threshold_count : 10;
  uint32_t peak_threshold       : 10;
	uint32_t peak_threshold_count : 10;
}VAD_AMDF_THRESHOLD_T;

typedef VAD_Type RSI_VAD_T;

#define VAD_1SMPLS_PER_ADDR            0x2
#define VAD_2SMPLS_PER_ADDR            0x1
#define VAD_4SMPLS_PER_ADDR            0x0
#define VAD_INTREG_SOURCE              0x0
#define VAD_AUXADC_SOURCE              0x3
#define VAD_METHOD_ZCR                 0x0
#define VAD_METHOD_ACF                 0x1
#define VAD_METHOD_AMDF                0x2
#define VAD_METHOD_WACF                0x3
#define VAD_METHOD_ZCR_ACF_AMDF_WACF   0x4
#define VAD_METHOD_ZCR_ACF             0x5
#define VAD_METHOD_ZCR_AMDF            0x6
#define VAD_METHOD_ZCR_WACF            0x7
#define PING_ADDR1                     1
#define PONG_ADDR1                     0
#define VAD_ACF_START                  0x05
#define VAD_ACF_END                    0x50
#define MAXIMUM_VALUE_1024             1024  
#define MAXIMUM_VALUE_4096             4096 
#define MAXIMUM_VALUE_4                4 
#define MAXIMUM_VALUE_8                8 
#define ULP_MEMORY_BASE                0x24060000
#define TEST_SAMPL_VAL                 32 
#define VAD_MASK_VALUE                 0xFC00

void RSI_VAD_PingPongMemoryAddrConfig(RSI_VAD_T *ptrvad,uint32_t ping_addr,
	                                   uint32_t pong_addr,uint16_t ping_enable);

error_t RSI_VAD_Config( RSI_VAD_T *ptrvad,uint16_t samples_per_frame,
		                    uint16_t samples_per_address,bool fullwidth,
											  uint8_t datasourceselect );
												
void RSI_VAD_Enable(RSI_VAD_T *ptrvad);

void RSI_VAD_InterruptClr(RSI_VAD_T *ptrvad,uint16_t ping_interrupt);

error_t RSI_VAD_SetAlgorithmThreshold( RSI_VAD_T *ptrvad,uint16_t algorithm_type,
                                       uint32_t zcr_threshold,
                                       uint32_t acf_threshold,uint32_t wacf_threshold,
                                       VAD_AMDF_THRESHOLD_T *config );
												
error_t RSI_VAD_Set_Delay(RSI_VAD_T *ptrvad,uint16_t startdelayval,uint16_t enddelayval);

error_t RSI_VAD_Input(RSI_VAD_T *ptrVad, int16_t data);

error_t RSI_VAD_FrameEnergyConfig(RSI_VAD_T *ptrvad,uint32_t threshold_frame_energy,
	                              	uint32_t threshold_smpl_collect,uint32_t prog_smpls_for_energy_check);

void RSI_VAD_Stop(RSI_VAD_T *pVAD);

uint16_t RSI_VAD_ProccessDone(RSI_VAD_T *pVAD);

int32_t RSI_VAD_ProcessData(RSI_VAD_T *pVAD,uint32_t vad_addr,
                         int16_t *data_in,uint16_t dc_est,
                         uint32_t dig_scale,uint32_t sample_len);
												 
void RSI_VAD_FastClkEnable(uint16_t fast_clk_sel,uint16_t clk_div_factor);												 

#endif /* __RSI_VAD_H__ */





























