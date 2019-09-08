/**
 * @file       rsi_processor_sensor.c
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

#include "rsi_chip.h"

/** 
 * @brief   This API is used to Enable / Disable the process sensor
 *@param[in] pstcProcessSensor is pointer to the processor sensor register instance
 *@param[in] bEN is to Enable / Disble of process sensor
 *           0: Disable process sensor
 *           1: Enable process sensor
 * @par      Example    RSI_ProSense_Enable(&PROCESSOR_SENSE , 1);
 *@return   returns 0 on success
 *			 Error code on failure
 * */
error_t RSI_ProSense_Enable(MCU_ProcessSensor_t *pstcProcessSensor , boolean_t bEN)
{
	pstcProcessSensor->PROCESS_SENSOR_EN =  bEN;
	return RSI_OK;
}


/** 
 * @brief    This API is used to read the PS count from the process sensor
 * @param[in] pstcProcessSensor is pointer to the processor sensor register instance
 * @par       Example    RSI_ProSense_Read(&PROCESSOR_SENSE);
 * @return   returns the PS count values
 * */
uint32_t RSI_ProSense_Read(MCU_ProcessSensor_t *pstcProcessSensor)
{
	return pstcProcessSensor->PS_COUNT;
}

/**
 * @brief    This API is used to read the Num cycle count from the process sensor
 * @param[in] pstcProcessSensor is pointer to the processor sensor register instance
 * @usage    RSI_ProSense_GetNumCycles(&PROCESSOR_SENSE);
 * @return   returns the PS count values
 */
uint32_t RSI_ProSense_GetNumCycles(MCU_ProcessSensor_t *pstcProcessSensor)
{
	return pstcProcessSensor->NUM_CYCLES;
}

/**
 * @brief    This API is used to Enable / Disable the process sensor clock
 * @param[in] pstcProcessSensor is pointer to the processor sensor register instance
 * @param[in] bEN is to Enable / Disble of process sensor
 *           0: Disable process sensor clock
 *           1: Enable process sensor clock
 * @usage    RSI_ProSense_ClkEnable(&PROCESSOR_SENSE , 1);
 * @return   returns 0 on success
 *			 Error code on failure
 */
error_t RSI_ProSense_ClkEnable(MCU_ProcessSensor_t *pstcProcessSensor , boolean_t bEN)
{
	if(bEN){
		pstcProcessSensor->PS_CLK_SW_ON =  1;
	}else{
		pstcProcessSensor->PS_CLK_SW_OFF =  1;
	}
	return RSI_OK;
}

/** 
 * @brief    This API is used to start / Stop the Ring-Oscillator clock for estimating process corner.
 * @param[in] pstcProcessSensor is pointer to the processor sensor register instance
 * @param[in] bEN is to Enable / Disble of process sensor
 *           0: STOP  Ring-Oscillator clock for estimating process corner.
 *           1: START Ring-Oscillator clock for estimating process corner.
 * @par      Example    RSI_ProSense_RingClkStart(&PROCESSOR_SENSE , 1);
 * @return   returns 0 on success
 *			 Error code on failure
 */
error_t RSI_ProSense_RingClkStart(MCU_ProcessSensor_t *pstcProcessSensor , boolean_t bEN)
{
	pstcProcessSensor->PS_RING_CLK_START =  bEN;
	return RSI_OK;
}


