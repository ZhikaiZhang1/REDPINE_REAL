/**
 * @file       rsi_tempe_sensor.h
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
 * @brief This files contains Temprature sensors function prototypes 
 *
 * 
 *
 */


/**
 * Includes
 */
#ifndef  __RSI_TEMP_SENSOR_H__
#define  __RSI_TEMP_SENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_TEMP_SENSORS RSI:RS1xxxx TS
 *  @brief   
 *  @{
 *
 */
#include "RS1xxxx.h"

void RSI_TS_SetCntFreez(NPSS_TEMPSENSOR_Type  *pstcTempSens , uint32_t u32CountFreez);
void RSI_TS_RefClkSel(NPSS_TEMPSENSOR_Type  *pstcTempSens , boolean_t bRefClk);
void RSI_TS_Enable(NPSS_TEMPSENSOR_Type  *pstcTempSens , boolean_t bEn);

/**
 * @}
 */
void RSI_TS_Config(NPSS_TEMPSENSOR_Type  *pstcTempSens,uint32_t u32Nomial);
/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_TEMP_SENSORS RSI:RS1xxxx TEMPERARTURE SENSOR
 *  @brief   
 *  @{
 *
 */
uint32_t RSI_TS_ReadTemp(NPSS_TEMPSENSOR_Type  *pstcTempSens);
uint32_t RSI_TS_GetRefClkCnt(NPSS_TEMPSENSOR_Type  *pstcTempSens);
uint32_t RSI_TS_GetPtatClkCnt(NPSS_TEMPSENSOR_Type  *pstcTempSens);
void RSI_TS_LoadBjt(NPSS_TEMPSENSOR_Type *pstcTempSens , uint8_t temp);
void RSI_TS_RoBjtEnable(NPSS_TEMPSENSOR_Type  *pstcTempSens , boolean_t  enable);
void RSI_Periodic_TempUpdate(TimePeriodCalib_t *temp,uint8_t enable,uint8_t trigger_time);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif


/*End of file not truncated */
#endif /**__RSI_TEMP_SENSOR_H__*/


/* @}end of group RSI_TEMP_SENSORS */
