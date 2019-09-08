/**
 * @file      rsi_adc.h
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
 *  @brief This file contains functions prototypes related to AUX-ADC peripheral
 *
 *  @section Description 
 *  this file contains list of functions prototypes for the AUX-ADC peripheral
 *
 *
 */

#ifndef __RSI_AUX_ADC_H__
#define __RSI_AUX_ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "RS1xxxx.h"
#include "base_types.h"

/******************************************************
 * *                      Macros
******************************************************/ 

  /**** List of Mask Bits ****/              
	#define POS0       0
	#define POS1       1
	#define POS2       2
	#define POS3       3
	#define POS4       4
	#define POS5       5
	#define POS6       6
	#define POS7       7
	#define POS8       8
	#define POS9       9
	#define POS10      10
	#define POS11      11
	#define POS12      12
 	#define POS13      13
 	#define POS14      14
 	#define POS15      15
 	#define POS16      16
 	#define POS17      17
 	#define POS18      18
 	#define POS19      19
 	#define POS20      20
 	#define POS21      21
 	#define POS22      22
 	#define POS23      23
 	#define POS24      24
 	#define POS25      25
 	#define POS26      26
 	#define POS27      27
 	#define POS28      28
 	#define POS29      29
 	#define POS30      30
 	#define POS31      31


#define  ADC_CHNL0_INTR   0
#define  ADC_CHNL1_INTR   1
#define  ADC_CHNL2_INTR   2
#define  ADC_CHNL3_INTR   3
#define  ADC_CHNL4_INTR   4
#define  ADC_CHNL5_INTR   5
#define  ADC_CHNL6_INTR   6
#define  ADC_CHNL7_INTR   7
#define  ADC_CHNL8_INTR   8
#define  ADC_CHNL9_INTR   9
#define  ADC_CHNL10_INTR  10
#define  ADC_CHNL11_INTR  11
#define  ADC_CHNL12_INTR  12
#define  ADC_CHNL13_INTR  13
#define  ADC_CHNL14_INTR  14
#define  ADC_CHNL15_INTR  15

#define AUXADCREG0    0x110 
#define AUXADCREG1    0x111 	
#define AUXADCREG2    0x112
#define SPAREREG2     0x1C1
#define MULTI_CHANNEL_EVENT          1
#define FIFO_MODE_EVENT              0
#define MAX_CHNL_NO                  15
#define PING_LEN_MAX     1023
#define PONG_LEN_MAX     1023
#define PING_ADDR_MAX    4294967295
#define PONG_ADDR_MAX    4294967295
#define MAX_THRS_VAL     7
#define MAX_POS_IN_SEL   31
#define MAX_NEG_IN_SEL   15
#define MAX_DIFF_MODE    1
#define MAX_TOL_DUR_TIME 1023
#define MAX_ON_DUR_TIME  511 

#define LDO_REG          0x24043A10
#define LDO_BYPASS_VAL   0x7b
#define LDO_EN           0x5b
#define ADC_MASK_VALUE       0xF7FF
 /******************************************************
 * *                    Constants
 * ******************************************************/
/******************************************************
 * *                   Enumerations
******************************************************/

typedef enum
{
	ADC_POWER_ON =0,
	ADC_POWER_OFF
}POWER_STATE;

/******************************************************
 * *                 Type Definitions
 * ******************************************************/
/******************************************************
 * *                    Structures
 **/

/*
\brief ADC channel Configuration structure
*/

volatile typedef struct 
{
	 uint32_t ping_addr[16];
	 uint32_t pong_addr[16];
	 uint16_t ping_length[16];
	 uint16_t pong_length[16];
	 uint32_t ping_mem1[16];
	 uint32_t ping_mem2[16];
	 uint32_t ping_mem3[16];
	 uint32_t pong_mem1[16];
	 uint32_t pong_mem2[16];
	 uint32_t pong_mem3[16];	 
}adc_config_t;
 
typedef struct
{
	void (*adccallbacFunc)( uint16_t channel,uint8_t event);   			  /*!< Call back function pointer */
		
} RSI_ADC_CALLBACK_T;

/******************************************************
 * *                 Global Variables
 * ******************************************************/
/******************************************************
 * *               Function Declarations
 * ******************************************************/
 
error_t RSI_ADC_PingPongMemoryAdrConfig(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel ,
                                        uint32_t ping_addr, uint32_t pong_addr,uint16_t ping_length,
                                        uint16_t pong_length,uint8_t ping_enable,uint8_t pong_enable );

void RSI_ADC_Calibration(void);

error_t RSI_ADC_PingpongEnable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel);

error_t RSI_ADC_InternalPerChnlDmaEnable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel);

error_t RSI_ADC_Config(AUX_ADC_DAC_COMP_Type *pstcADC,uint8_t multi_channel_en, uint8_t static_fifo_mode,
                       uint8_t fifo_threshold, uint8_t internal_dma_en );

error_t RSI_ADC_ChannelConfig(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel , 
                              uint8_t an_perif_adc_ip_sel,uint8_t an_perif_adc_in_sel,
 															uint8_t an_perif_adc_diffmode );

error_t RSI_ADC_ChannelSamplingRate(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel , 
                                    uint16_t adc_ch_offset, uint16_t adc_ch_freq_val);

error_t RSI_ADC_StaticMode(AUX_ADC_DAC_COMP_Type *pstcADC,uint16_t an_perif_adc_ip_sel,
                          uint16_t an_perif_adc_in_sel, uint8_t an_perif_adc_diffmode);

error_t RSI_ADC_Start( AUX_ADC_DAC_COMP_Type *pstcADC );

error_t RSI_ADC_Stop( AUX_ADC_DAC_COMP_Type *pstcADC );

error_t RSI_ADC_ChnlEnable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel);

error_t RSI_ADC_ChnlDisable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel);

error_t RSI_ADC_ClkDivfactor(AUX_ADC_DAC_COMP_Type *pstcADC, uint16_t adc_on_time , uint16_t adc_total_duration);

error_t RSI_ADC_ChnlIntrUnMask(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel);

error_t RSI_ADC_ChnlIntrMask(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel);

error_t RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel);

uint32_t RSI_ADC_ChnlIntrStatus(AUX_ADC_DAC_COMP_Type *pstcADC);

void RSI_ADC_PowerControl(POWER_STATE state);

error_t RSI_ADC_NoiseAvgMode(AUX_ADC_DAC_COMP_Type *pstcADC, bool en_disable);

error_t RSI_ADC_TempSensorEnable(AUX_ADC_DAC_COMP_Type *pstcADC);

error_t RSI_ADC_ThresholdConfig(AUX_ADC_DAC_COMP_Type *pstcADC , uint32_t threshold1,
                                uint32_t threshold2, uint8_t range);
error_t RSI_ADC_Bbp(AUX_ADC_DAC_COMP_Type *pstcADC ,uint8_t adc_bbp_en, uint8_t bbp_en, uint8_t bbp_id);

error_t RSI_ADC_InterruptHandler( AUX_ADC_DAC_COMP_Type *pstcADC,RSI_ADC_CALLBACK_T *pADCCallBack);

void RSI_ADC_ThreshInterruptClr(AUX_ADC_DAC_COMP_Type *pstcADC);

error_t RSI_ADC_PingPongReInit(AUX_ADC_DAC_COMP_Type *pstcADC, uint8_t channel,uint8_t ping_enable,uint8_t pong_enable );

error_t RSI_ADC_InternalPerChnlDmaDisable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel);

error_t RSI_ADC_PingpongDisable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel);

error_t RSI_ADC_FifoMode(AUX_ADC_DAC_COMP_Type *pstcADC,uint16_t channel_no,
                         uint16_t an_perif_adc_ip_sel, uint16_t an_perif_adc_in_sel,
                          uint8_t an_perif_adc_diffmode);													

error_t RSI_ADC_ReadData(AUX_ADC_DAC_COMP_Type *pstcADC, int16_t *data,
                         uint8_t ping_pong,uint16_t channel,uint8_t data_process_en,uint8_t diff_en);

int16_t RSI_ADC_ReadDataStatic(AUX_ADC_DAC_COMP_Type *pstcADC,uint8_t data_process_en,uint8_t diff_en);

int16_t  RSI_ADC_GainOffsetCal(int16_t data,uint8_t diff_en);

void RSI_ADC_AUXLdoConfig(AUX_ADC_DAC_COMP_Type *pstcADC,uint16_t bypass_en,uint16_t value);

float RSI_ADC_VrefCal(void);

#ifdef __cplusplus
}
#endif

#endif // __RSI_AUX_ADC_H__
