/**
 * @file       rsi_processor_sensor.h
 * @version    0.1
 * @date       20 jan 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes related to power save
 *
 * @section Description
 * This file contains the list of function prototypes for the Power save and low level function definitions
 * Following are list of API's which need to be defined in this file.
 *
 */


/**
 * Includes
 */

#ifndef  __RSI_PROCESSOR_SENSOR_H__
#define  __RSI_PROCESSOR_SENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_PROCESSOR_SENSOR RSI:RS1xxxx PROCESSOR_SENSOR 
 *  @{
 *
 */
#include "RS1xxxx.h"

error_t RSI_ProSense_Enable(MCU_ProcessSensor_t *pstcProcessSensor , boolean_t bEN);
uint32_t RSI_ProSense_Read(MCU_ProcessSensor_t *pstcProcessSensor);
uint32_t RSI_ProSense_GetNumCycles(MCU_ProcessSensor_t *pstcProcessSensor);
error_t RSI_ProSense_ClkEnable(MCU_ProcessSensor_t *pstcProcessSensor , boolean_t bEN);
error_t RSI_ProSense_RingClkStart(MCU_ProcessSensor_t *pstcProcessSensor , boolean_t bEN);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /*__RSI_PROCESSOR_SENSOR_H__*/
