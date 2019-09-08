/**
 * @file       rsi_comparator.c
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
#include "rsi_comparator.h"
#include "rsi_chip.h"

/**
 * @fn           error_t RSI_COMP_Config(COMP_Type* comp, uint8_t comp_number,uint8_t sel_p_mux,
                                          uint8_t sel_n_mux,uint8_t hyst_en,uint8_t filter_en)
 * @brief        This API is used to configure the comparator parameters.
 * @param[in]    comp        : Pointer to the comparator register instance.
 * @param[in]    comp_number : comparator number to be enabled or disabled
 *                             - 1:for comparator 1 
 *                             - 2:for comparator 2    
 * @param[in]    sel_p_mux   : select positive input for comparator 
 *                             - 0: compx_p0 x is 1 or 2 i.e comp1 and comp2
 *                             - 1: compx_p1 
 *                             - 2: DAC	 
 *                             - 3: reference buffer out 
 *                             - 4: reference scalar out 
 *                             - 5: resistor bank out 
 *                             - 6: opamp1_out 
 *                             - 7: opamp2_out 
 *                             - 8: opamp3_out 
 * @param[in]    sel_n_mux   : select negative input for comparator 
 *                             - 0: compx_n0 x is 1 or 2 i.e comp1 and comp2
 *                             - 1: compx_n1 
 *                             - 2: DAC	 
 *                             - 3: reference buffer out 
 *                             - 4: reference scalar out 
 *                             - 5: resistor bank out 
 *                             - 6: opamp1_out 
 *                             - 7: opamp2_out 
 *                             - 8: opamp3_out                             
 * @param[in]    hyst_en     :  control the hysteresis of the comparator(0 to 4)   
 * @param[in]    filter_en   :  To enable filter for comparator 
 *                             - 1:enable
 *                             - 0:disable 
 * @return       - \ref RSI_OK If valid comp
 *                   else if fail return Invalid parameters.
 * @par            Example     - RSI_COMP_Config(COMPARATOR_1,COMP1,COMP1_POSITIVE_INPUT,COMP1_NEGATIVE_INPUT,HYST_ENABLE,FILTER_ENABLE); 
 *
 */
error_t RSI_COMP_Config(AUX_ADC_DAC_COMP_Type* comp      , uint8_t comp_number , uint8_t sel_p_mux, 
                        uint8_t    sel_n_mux , uint8_t hyst_en     , uint8_t filter_en)
{
	// Parameter validation
  if((comp_number > MAX_NUM_OF_COMPARATORS)||(comp_number < MIN_NUM_OF_COMPARATORS) || (sel_p_mux > MAX_P_SEL_MUX)
		   ||(sel_n_mux > MAX_N_SEL_MUX)||(hyst_en > MAX_NO_OF_HYST))
	{
    return INVALID_PARAMETERS;
	}
	if(comp_number==COMP1)
	{
		comp->COMPARATOR1_b.CMP1_MUX_SEL_P  = sel_p_mux ;
		comp->COMPARATOR1_b.CMP1_MUX_SEL_N  = sel_n_mux ;
		comp->COMPARATOR1_b.CMP1_HYST       = hyst_en;
		comp->COMPARATOR1_b.CMP1_EN_FILTER  = filter_en;
	}
	else{
		comp->COMPARATOR1_b.CMP2_MUX_SEL_P  = sel_p_mux ;
		comp->COMPARATOR1_b.CMP2_MUX_SEL_N  = sel_n_mux ;
		comp->COMPARATOR1_b.CMP2_HYST       = hyst_en;
		comp->COMPARATOR1_b.CMP2_EN_FILTER  = filter_en;
		
	}
	return RSI_OK;
}

/**
 * @fn           error_t RSI_COMP_Enable(COMP_Type* comp,uint8_t comp_number , uint8_t enable)
 * @brief        This API is used to enable and disable the comparator
 * @param[in]    comp        : Pointer to the comparator register instance.
 * @param[in]    comp_number : comparator no to be enabled or disabled
 *                             - 1:for comparator 1 
 *                             - 2:for comparator 2                           
 * @param[in] 	 enable      :enable and disable comp
 *                             - 1:enable
 *                             - 0:disable
 * @return       - \ref RSI_OK If valid comp
 *                   else if fail return Invalid parameters.
 * @par            Example      -	RSI_COMP_Enable(COMPARATOR_1,COMP1,Enable); 
 */
error_t RSI_COMP_Enable(AUX_ADC_DAC_COMP_Type* comp,uint8_t comp_number , uint8_t enable)
{
  if((comp_number>MAX_NUM_OF_COMPARATORS)||(comp_number < MIN_NUM_OF_COMPARATORS))
  {
	  return INVALID_PARAMETERS;
  }
  if(comp_number==COMP1)
  {
       comp->COMPARATOR1_b.CMP1_EN = enable;
  }
  else
  {
      comp->COMPARATOR1_b.CMP2_EN = enable;
  }
  return RSI_OK;
}

/**
 * @fn           void RSI_COMP_ResistorBank_Enable(COMP_Type* comp,uint8_t enable)
 * @brief        This API is used to enable the Register bank output.
 * @param[in]    comp   : Pointer to the comparator register instance                      
 * @param[in] 	 enable : enable or  disable BOD regiser input,
 *                             - 1:enable
 *                             - 0:disable
 * @return       None
 * @par          Example      -	RSI_COMP_ResistorBank_Enable(COMPARATOR_1,Enable);  
 */
void RSI_COMP_ResistorBank_Enable(AUX_ADC_DAC_COMP_Type* comp,uint8_t enable)
{
  comp->BOD_b.BOD_RES_EN = enable;
}

/**
 * @fn           error_t RSI_COMP_ResistorBank_Thrsh(COMP_Type* comp,uint16_t value_thrsh)
 * @brief        This API is used to set register bank threshold value.
 * @param[in]    comp        : Pointer to the comparator register instance                       
 * @param[in] 	 value_thrsh : Value of register threshold value. 
 * @return       - \ref RSI_OK If valid comp
 *                   else if fail return Invalid parameters.
 * @par          Example      -  RSI_COMP_ResistorBank_Thrsh(COMPARATOR_1,value_thrsh);
 */
error_t RSI_COMP_ResistorBank_Thrsh(AUX_ADC_DAC_COMP_Type* comp,uint16_t value_thrsh)
{
  if(value_thrsh>MAX_THRSHOLD_VALUE)
  {
	return INVALID_PARAMETERS;
  }	  
  comp->BOD_b.BOD_THRSH = value_thrsh;
  return RSI_OK;
}

/**
 * @fn           error_t RSI_COMP_ReferenceScalarOut(COMP_Type* comp,uint16_t scalar_factor_value)
 * @brief        This API is used to set the scalar output value.
 * @param[in]    comp         : Pointer to the comparator register instance                      
 * @param[in] 	 scalar_factor_value : Set reference scalar value.
 * @return       - \ref RSI_OK If valid comp
 *                   else if fail return Invalid parameters.
 * @par          Example      - RSI_COMP_ReferenceScalarOut(COMPARATOR_1,scalar_factor_value);    
 */
error_t RSI_COMP_ReferenceScalarOut(AUX_ADC_DAC_COMP_Type* comp,uint16_t scalar_factor_value)
{
  if(scalar_factor_value>MAX_SCALAR_FACTOR)
  {
	return INVALID_PARAMETERS;
  }	   
  comp->BOD_b.REFBUF_VOLT_SEL = scalar_factor_value;
  return RSI_OK;
}

/**
 * @fn          void RSI_COMP_RefenceBufferOut_Enable(COMP_Type* comp,uint8_t enable)
 * @brief        This API is used to enable reference buffer output.
 * @param[in]    comp         : Pointer to the comparator register instance                      
 * @param[in] 	 enable       : Enable reference buffer output.
 * @return       None
 * @par          Example      -RSI_COMP_RefenceBufferOut_Enable(COMPARATOR_1,Enable);
 */
void RSI_COMP_RefenceBufferOut_Enable(AUX_ADC_DAC_COMP_Type* comp,uint8_t enable)
{
	
  comp->BOD_b.REFBUF_EN = enable;
}

