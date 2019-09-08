/**
 * @file       rsi_Ps2_Sleep.c
 * @version    0.9
 * @date       27 Jun 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example code of all end to end functionality
 *
 * @section Description
 * This file contains the list of function prototypes for the CTS  
 *
 *
 */

#include "rsi_chip.h"

#ifndef __RSI_CTS_H__
#define __RSI_CTS_H__


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup  RSI_CTS RSI:RS1xxxx CTS 
 *  @{
 *
 */


#define ULPSS_CAP_SENSOR_IRQHandler           IRQ006_Handler 
#define ACK_FOR_WAKE_UP_INTERRUPT             BIT(8)

#define FIFO_EMPTY_STATUS                    BIT(28)
#define RESET_WR_FIFO_STATUS                 BIT(14)


/**   
 * @fn          STATIC INLINE void RSI_CTS_BypassPRS(CTS_Type *cts,boolean_t enable)
 * @brief       This API is used Bypass the Random number generator output to the Non-overlapping stream generator
 * @param[in]   cts           :pointer to cts config structure
 * @param[in]   enable        : 1:bypass the random no generator
 *                              0:use random no generator output
 * @return      none 
 *              
 */
STATIC INLINE void RSI_CTS_BypassPRS(CTS_Type *cts,boolean_t enable)
{
  CTS->CTS_CONFIG_REG_1_1_b.BYPASS=enable;
}


/**   
 * @fn          STATIC INLINEvoid RSI_CTS_ThresholdProgam(CTS_Type *cts,uint8_t threshold)
 * @brief       This API is used for threshold programming
 * @param[in]   cts           :pointer to cts config structure
 * @param[in]   threshold     :Threshold for fifo aempty
 *               
 * @return      none 
 *              
 */
STATIC INLINE void RSI_CTS_ThresholdProgam(CTS_Type *cts,uint8_t threshold)
{
  cts->CTS_CONFIG_REG_0_0_b.FIFO_AEMPTY_THRLD=threshold; //Threshold for fifo aempty
}


/**   
 * @fn          STATIC INLINE void RSI_CTS_StaticClkEnable(CTS_Type *cts,boolean_t enable)
 * @brief       This API is used for static clk gating 
 * @param[in]   cts      :pointer to cts config structure
 * @param[in]   enable   :write 0 for static clk gate else 1
 *               
 * @return      none 
 *              
 */
STATIC INLINE void RSI_CTS_StaticClkEnable(CTS_Type *cts,boolean_t enable)
{
  cts->CTS_CONFIG_REG_0_0_b.CTS_STATIC_CLK_EN=enable; //clk are not gated
}

/**   
 * @fn          STATIC INLINE void RSI_CTS_ConfigSampleMode(CTS_Type *cts,boolean_t avg_enable)
 * @brief       This API is used for select averaging mode to apply samples
 * @param[in]   cts           :pointer to cts config structure
 * @param[in]   avg_enable    : 1 - for selecting averaging of samples
 *                              0 - for directly apply samples
 * @return      none 
 *              
 */
STATIC INLINE void RSI_CTS_ConfigSampleMode(CTS_Type *cts,boolean_t avg_enable)
{
  cts->CTS_CONFIG_REG_1_1_b.SAMPLE_MODE=avg_enable;  //selects Averaging
}

/**   
 * @fn          STATIC INLINE void RSI_CTS_ModeSelect(CTS_Type *cts,boolean_t mode)
 * @brief       This API is used for scanning mode selection 
 * @param[in]   cts    :pointer to cts config structure
 * @param[in]   mode   : 1 - for continious mode select
 *                       0 - for one hot mode
 * @return      none 
 *              
 */
STATIC INLINE void RSI_CTS_ModeSelect(CTS_Type *cts,boolean_t mode)
{
  cts->CTS_CONFIG_REG_1_1_b.CNT_ONEHOT_MODE=mode;   
}


/**   
 * @fn          STATIC INLINEvoid RSI_CTS_Enable(CTS_Type *cts,boolean_t enable)
 * @brief       This API is used enable/disable the cts
 * @param[in]   cts         :pointer to cts config structure
 * @param[in]   enable      : 1 - enable the CTS
 *                            0 - disable the cts
 * @return      none 
 *              
 */
STATIC INLINE void RSI_CTS_Enable(CTS_Type *cts,boolean_t enable)
{ 
  cts->CTS_CONFIG_REG_1_1_b.ENABLE1=enable;//enable the CTS
}

/**   
 * @fn          STATIC INLINE void RSI_CTS_BufferDelay(CTS_Type *cts,uint8_t delay)
 * @brief       This API is used set the buffer delay
 * @param[in]   cts         :pointer to cts config structure
 * @param[in]   delay     : delay time (max value is 31)                    
 * @return      none 
 *              
 */
STATIC INLINE void RSI_CTS_BufferDelay(CTS_Type *cts,uint8_t delay)
{ 
   cts->CTS_CONFIG_REG_1_1_b.BUFFER_DELAY=delay;//set the buffer delay
}

/**   
 * @fn          STATIC INLINE void RSI_CTS_FifoInterruptEnable(CTS_Type *cts,boolean_t enable)
 * @brief       This API is used to mask the fifo interrupt
 * @param[in]   cts        :pointer to cts config structure
 * @param[in]   enable     : 1 -fifo afull interrupt will mask
 *                           0 -unmask
 * @return      none 
 *              
 */
STATIC INLINE void RSI_CTS_FifoInterruptEnable(CTS_Type *cts,boolean_t enable)
{
  cts->CTS_CONFIG_REG_1_7_b.MASK_FIFO_AFULL_INTR=enable;  // mask/Uunmask the interrupt
}


void RSI_CTS_ClkSelection(CTS_Type *cts,uint8_t clk_sel_1,uint8_t clk_sel_2,uint8_t clk_divider_1,
	                        uint8_t clk_divider_2);

uint32_t RSI_CTS_GetStatus(CTS_Type *cts);

void RSI_CTS_ConfigPolynomial(CTS_Type *cts,boolean_t enable,uint8_t poly_length,uint32_t seed,uint32_t polynomial);

void RSI_CTS_ConfigOnOffTime(CTS_Type *cts,uint16_t on_time ,uint16_t off_time);

void RSI_CTS_ConfigSampling(CTS_Type *cts,uint16_t dealy,uint16_t repetitions);

void RSI_CTS_ConfigSamplingPattern(CTS_Type *cts,uint32_t pattern,uint32_t valid_sensor);

void RSI_CTS_ConfigRefVoltage(CTS_Type *cts,uint16_t ref_voltage,boolean_t enable);

void RSI_CTS_ConfigWakeUp(CTS_Type *cts,uint8_t mode,uint16_t threshold);

uint32_t RSI_CTS_ReadRandomData(CTS_Type *cts);

uint32_t RSI_CTS_ReadFifo(CTS_Type *cts);

void RSI_CTS_IntrClear(CTS_Type *cts);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif // __RSI_CTS_H__

