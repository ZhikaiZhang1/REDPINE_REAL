/**
 * @file       rsi_cts.c
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
 * This file contains CTS low level function definations
 *
 */
 
#include "rsi_cts.h"


/**   
 * @fn          void RSI_CTS_ClkSelection(CTS_Type *cts,uint8_t clk_sel_1,uint8_t clk_sel_2,uint8_t clk_divider_1,uint8_t clk_divider_2)
 * @brief       This API is used for clk selection 
 * @param[in]   cts           :pointer to cts config structure
 * @param[in]   clk_Sel_1     :mux select value for clk select 1
 * @param[in]   clk_Sel_2     :mux select value for clk select 2              
 * @param[in]   clk_divider_1 :division factor for clk_1
 * @param[in]   clk_divider_2 :division factor for clk_2
 *               
 * @return      none 
 *              
 */
void RSI_CTS_ClkSelection(CTS_Type *cts,uint8_t clk_sel_1,uint8_t clk_sel_2,uint8_t clk_divider_1,uint8_t clk_divider_2)
{
  cts->CTS_CONFIG_REG_0_0_b.CLK_SEL1=clk_sel_1;
	cts->CTS_CONFIG_REG_0_0_b.CLK_SEL2=clk_sel_2;           //select the musx for clk_Sel1 and clk sel2
  cts->CTS_CONFIG_REG_0_0_b.PRE_SCALAR_1=clk_divider_1;   //select the clk divider for both clk
	cts->CTS_CONFIG_REG_0_0_b.PRE_SCALAR_2=clk_divider_2;
}

/**   
 * @fn          uint32_t RSI_CTS_GetStatus(CTS_Type *cts)
 * @brief       This API is used to get the status of cts 
 * @param[in]   cts           :pointer to cts config structure
 *               
 * @return      status of an cts 
 *              
 */
uint32_t RSI_CTS_GetStatus(CTS_Type *cts)
{
	uint32_t status=0;
  status=cts->CTS_CONFIG_REG_0_0; //GET THE STATUS
  status|=cts->CTS_CONFIG_REG_1_1;
	return (status&10004000);
}

/**   
 * @fn          void RSI_CTS_ConfigPolynomial(CTS_Type *cts,boolean_t enable,uint8_t poly_length,uint32_t seed,uint32_t polynomial)
 * @brief       This API is used config polynomial parameters
 * @param[in]   cts        :pointer to cts config structure
 * @param[in]   enable     : 1 -enable seed value feed
 *                           0 -disable seed value feed
 * @param[in]   poly_length: polynomial length 
 * @param[in]    seed       : seed value to be loaded
 * @param[in]   polynomial  : polynomial for PRS 
 * @return      none 
 *              
 */
void RSI_CTS_ConfigPolynomial(CTS_Type *cts,boolean_t enable,uint8_t poly_length,uint32_t seed,uint32_t polynomial)
{ 
  cts->CTS_CONFIG_REG_1_1_b.SEED_LOAD=enable; //load the seed value
  cts->CTS_CONFIG_REG_1_1_b.POLYNOMIAL_LEN=poly_length;
  cts->CTS_CONFIG_REG_1_3_b.PRS_SEED=seed;          //LOAD SEED VALUE
  cts->CTS_CONFIG_REG_1_4_b.PRS_POLY=polynomial;   //load the polynomial for PRS
}

/**   
 * @fn          void RSI_CTS_ConfigOnOffTime(CTS_Type *cts,uint16_t on_time ,uint16_t off_time)
 * @brief       This API is used set on and off time
 * @param[in]   cts        :pointer to cts config structure
 * @param[in]   on_time    : required on time 
 * @param[in]   off_time   : required off time 
 * @return      none 
 *              
 */
void RSI_CTS_ConfigOnOffTime(CTS_Type *cts,uint16_t on_time ,uint16_t off_time)
{ 
   cts->CTS_CONFIG_REG_1_2_b.PWM_ON_PERIOD=on_time; //sets on and off time for sensor
   cts->CTS_CONFIG_REG_1_2_b.PWM_OFF_PERIOD=off_time;
}

/**   
 * @fn          void RSI_CTS_ConfigSampling(CTS_Type *cts,uint16_t delay,uint16_t repetitions)
 * @brief       This API is used for configue the sampling delay
 * @param[in]   cts        :pointer to cts config structure
 * @param[in]   delay      : inter sensor delay time
 * @param[in]   repetitions: no of times scan 
 * @return      none 
 *              
 */
void RSI_CTS_ConfigSampling(CTS_Type *cts,uint16_t delay,uint16_t repetitions)
{ 
   cts->CTS_CONFIG_REG_1_5_b.INTER_SENSOR_DELAY=delay;  //sets the inter senser dealy
   cts->CTS_CONFIG_REG_1_5_b.N_SAMPLE_COUNT=repetitions; //sets the no time scan the sensor
}

/**   
 * @fn          void RSI_CTS_ConfigSamplingPattern(CTS_Type *cts,uint32_t pattern,uint32_t valid_sensor)
 * @brief       This API is used for configue the scanning pattern and valid sensor
 * @param[in]   cts         :pointer to cts config structure
 * @param[in]   pattern     :sensor pattern to scan(0x12345678)
 * @param[in]   valid_sensor: no of valid sensors(1 to 8)
 * @return      none 
 *              
 */
void RSI_CTS_ConfigSamplingPattern(CTS_Type *cts,uint32_t pattern,uint32_t valid_sensor)
{ 
   cts->CTS_CONFIG_REG_1_6_b.SENSOR_CFG=pattern;   //set the pattern to sense sensor
	 cts->CTS_CONFIG_REG_1_7_b.VALID_SENSORS=valid_sensor; //sets the valid sensors to sense  
}

/**   
 * @fn          void RSI_CTS_RefVoltageConfig(CTS_Type *cts,uint16_t ref_voltage,boolean_t enable)
 * @brief       This API is used to configure reference voltage
 * @param[in]   cts     :pointer to cts config structure
 * @param[in]   enable  : enable reference voltage select  
 * @param[in]   ref_voltage : reference voltage to be set
 *  								0  -0.5V
 *  								1  -0.6V
 *  								2  -0.7V
 *  								3  -0.8V
 *  								4  -0.9V
 *  								5  -1.0V
 *  								6  -1.1V
 *  								7  -1.2V
 * @return      none 
 *              
 */
void RSI_CTS_ConfigRefVoltage(CTS_Type *cts,uint16_t ref_voltage,boolean_t enable)
{ 
   cts->CTS_CONFIG_REG_1_7_b.REF_VOLT_CONFIG=ref_voltage; //set the reference voltage
   cts->CTS_CONFIG_REG_1_7_b.VREF_SEL=enable;             //enables ref vlg select
}

/**   
 * @fn          void RSI_CTS_ConfigWakeUp(CTS_Type *cts,uint8_t mode,uint16_t threshold)
 * @brief       This API is used to configure the wakeup mode
 * @param[in]   cts      :pointer to cts config structure
 * @param[in]   mode     : 1 -wakeup if count is greater than threshold  
 *                         0 -wakeup if count is less than threshold 
 * @param[in]   threshold : wakeup threshold
 * @return      none 
 *              
 */
void RSI_CTS_ConfigWakeUp(CTS_Type *cts,uint8_t mode,uint16_t threshold)
{ 
  cts->CTS_CONFIG_REG_1_7_b.WAKEUP_MODE=mode;            //configure the wakeup mode
  cts->CTS_CONFIG_REG_1_7_b.WAKE_UP_THRESHOLD=threshold; //write threshold value
}

/**   
 * @fn          uint32_t RSI_CTS_ReadRandomData(CTS_Type *cts)
 * @brief       This API is used read the random data
 * @param[in]   cts      :pointer to cts config structure
 *                        
 * @return      none 
 *              
 */
uint32_t RSI_CTS_ReadRandomData(CTS_Type *cts)
{ 
  uint32_t random_data;
  random_data=cts->CTS_CONFIG_REG_1_8_b.PRS_STATE;//read the random data
  return random_data;
}

/**   
 * @fn          uint32_t RSI_CTS_ReadFifo(CTS_Type *cts)
 * @brief       This API is used read the data from fifo
 * @param[in]   cts      :pointer to cts config structure
 *                        
 * @return      none 
 *              
 */
uint32_t RSI_CTS_ReadFifo(CTS_Type *cts)
{ 
	 uint32_t read_data;
   read_data=cts->CTS_FIFO_ADDRESS_b.FIFO;
	 return read_data;
}

/**   
 * @fn          void RSI_CTS_IntrClear(CTS_Type *cts)
 * @brief       This API is used clear the interrupt for cts
 * @param[in]   cts      :pointer to cts config structure
 *                        
 * @return      none 
 *              
 */
void RSI_CTS_IntrClear(CTS_Type *cts)
{ 
	CTS->CTS_CONFIG_REG_1_1_b.WAKE_UP_ACK =1;
	CTS->CTS_CONFIG_REG_1_1_b.WAKE_UP_ACK =0;
}
