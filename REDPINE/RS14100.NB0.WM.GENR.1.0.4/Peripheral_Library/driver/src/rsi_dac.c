/**
 * @file      rsi_dac.c
 * @version   1.0
 * @date      4 Aug 2018
 *
 *  Copyright(C) Redpine Signals 2017
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief This file contains APIs related to AUX-DAC
 *
 *  @section Description
 *  This file contains APIs relared to Digital to Analog converter peripheral
 *
 *
 */

 /* Includes */
 
#include "rsi_chip.h"

/**
 * @fn           error_t RSI_DAC_Config(AUX_ADC_DAC_COMP_Type *pstcDAC,uint8_t static_fifo_mode,
 *                                      uint16_t aux_dac_out_mux_en,uint16_t aux_dac_out_mux_sel)
 * @brief        This API is used to configure DAC in static or FIFO mode.
 * @param[in]    pstcADC              : Pointer to the AUX_ADC_DAC_COMP_Type structure.  
 * @param[in]    static_fifo_mode     : This parameter define DAC in FIFO mode or static mode
 *                                      static_fifo_mode = 0  , for Fifo mode
 *                                      static_fifo_mode = 1  , for Static mode 
 * @param[in]    aux_dac_out_mux_en   : This parameter define DAC output play on AGPIO or not. 
 *                                      aux_dac_out_mux_en = 0  , DAC output not play on any AGPIO.
 *                                      aux_dac_out_mux_en = 1  , DAC output on GPIO. 
 * @param[in]    aux_dac_out_mux_sel  : This parameter define DAC output play on which AGPIO 
 *                                      aux_dac_out_mux_sel = 0 , DAC output play on ULP_GPIO4
 *                                      aux_dac_out_mux_sel = 1 , DAC output play on ULP_GPIO15
 * @return       execution status 
 *
 */
error_t RSI_DAC_Config(AUX_ADC_DAC_COMP_Type *pstcDAC,uint8_t static_fifo_mode,uint16_t aux_dac_out_mux_en,
                        uint16_t aux_dac_out_mux_sel)
{
	pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_OUT_MUX_EN  = aux_dac_out_mux_en;
	pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_OUT_MUX_SEL = aux_dac_out_mux_sel;	
  pstcDAC->AUXDAC_CTRL_1_b.DAC_STATIC_MODE     = static_fifo_mode; 
	if(static_fifo_mode == 0)
	{
	  pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_DYN_EN = 1U;
	}	
	return RSI_OK;
}

/**
 * @fn           error_t RSI_DAC_DynamicModeConfig(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t channel_no,
 *                                                uint16_t aux_dac_out_mux_en,uint16_t aux_dac_out_mux_sel)
 * @brief        This API is used to configure DAC in dynamic mode.
 * @param[in]    pstcADC              : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[in]    channel_no           : DAC channel to be configured as 0,1,2 ...15 when ADC multichannel enable is present
 * @param[in]    aux_dac_out_mux_en   : This parameter define DAC output play on AGPIO or not. 
 *                                      aux_dac_out_mux_en = 0  , DAC output not play on any AGPIO.
 *                                      aux_dac_out_mux_en = 1  , DAC output on GPIO. 
 * @param[in]    aux_dac_out_mux_sel  : This parameter define DAC output play on which AGPIO 
 *                                      aux_dac_out_mux_sel = 0 , DAC output play on ULP_GPIO4
 *                                      aux_dac_out_mux_sel = 1 , DAC output play on ULP_GPIO15
 * @return       execution status 
 *
 */
error_t RSI_DAC_DynamicModeConfig(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t channel_no,
                                  uint16_t aux_dac_out_mux_en,uint16_t aux_dac_out_mux_sel)
{
	pstcDAC->INTERNAL_DMA_CH_ENABLE_b.PER_CHANNEL_ENABLE |= 1 << channel_no;	
	pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_DYN_EN = 1U;
  pstcDAC->AUXDAC_CTRL_1_b.DAC_TO_CTRL_ADC = 1U;				
	pstcDAC->ADC_CH_BIT_MAP_CONFIG[channel_no].ADC_CH_BIT_MAP_CONFIG_0_b.CHANNEL_BITMAP |=( aux_dac_out_mux_en << 29 
		                                                                                   | aux_dac_out_mux_sel<< 30 );
	return RSI_OK;
}

/**
 * @fn           error_t RSI_DAC_DynamicModeWriteData(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t channel,
 *                                                    uint16_t *data,uint32_t len)
 * @brief        This API is used to write input data DAC in dynamic mode  
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[in]    channel_no  : DAC channel to be configured as 0,1,2 ...15 when ADC multichannel enable is present
 * @param[in]    data        : Input buffer pointer
 * @param[in]    len         : length of digital samples which play in DAC .
 * @return       execution status 
 *
 */
error_t RSI_DAC_DynamicModeWriteData(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t channel_no,
                                     uint16_t *data,uint32_t len)
{
	uint32_t i;
	for(i=0;i<len;i++)
	{ 
		/* clear the DAC input register in dynamic mode */
		pstcDAC->ADC_CH_BIT_MAP_CONFIG[channel_no].ADC_CH_BIT_MAP_CONFIG_0_b.CHANNEL_BITMAP &=(0x7FFFFFFF);	
		/* Feed the input sample in dynamic mode */
		pstcDAC->ADC_CH_BIT_MAP_CONFIG[channel_no].ADC_CH_BIT_MAP_CONFIG_0_b.CHANNEL_BITMAP |=(data[i])<<31; 
		/* clear the DAC input register in dynamic mode */
		pstcDAC->ADC_CH_BIT_MAP_CONFIG[channel_no].ADC_CH_BIT_MAP_CONFIG_1_b.CHANNEL_BITMAP &=(0xFFFFFE00);		
		/* Feed the input sample in dynamic mode */		
		pstcDAC->ADC_CH_BIT_MAP_CONFIG[channel_no].ADC_CH_BIT_MAP_CONFIG_1_b.CHANNEL_BITMAP |=(data[i])>>1;
	}	
 return RSI_OK;
}
/**
 * @fn           error_t RSI_DAC_WriteData(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t *data,
 *                                         uint8_t static_fifo_mode,uint16_t len)
 * @brief        This API is used to write input data to DAC in static or in FIFO mode. 
 * @param[in]    pstcADC             : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    data                : This parameter define input data to DAC.
 * @param[in]    static_fifo_mode    : This parameter define write data to DAC in FIFO mode 
 *                                      or in static mode
 *                                      static_fifo_mode = 1  , Write data to DAC in static mode
 *                                      static_fifo_mode = 0  , Write data to DAC in fifo mode  
 * @return       execution status 
 *
 */
error_t RSI_DAC_WriteData(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t *data,
                                uint8_t static_fifo_mode,uint16_t len)
{
	uint32_t i;
	for(i=0;i<len;i++)
	{
		if(static_fifo_mode == 0)
		{	
			pstcDAC->AUXDAC_DATA_REG_b.AUXDAC_DATA = (*data) ;
			data++;
		}
    else
    {
	 	  pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_DATA_S = (*data);
		  data++;		
		} 			
	}
	return RSI_OK;
}


/**
 * @fn           uint16_t RSI_DAC_ReadData(AUX_ADC_DAC_COMP_Type *pstcDAC,uint16_t data)
 * @brief        This API is used to Read output data of DAC in FIFO or static mode. 
 * @param[in]    pstcADC             : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[out]   data                : Output data 
 * @return       Output data of DAC. 
 *
 */
uint16_t RSI_DAC_ReadData(AUX_ADC_DAC_COMP_Type *pstcDAC)
{	
	 uint16_t data;
	 data = pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_DATA_S;	
   return data;
}

/**
 * @fn           uint16_t RSI_DAC_DynamicModeReadData(AUX_ADC_DAC_COMP_Type *pstcDAC, uint32_t channel,uint16_t data)
 * @brief        This API is used to Read output data 
 * @param[in]    pstcADC  : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  : DAC channel to be configured as 0,1,2 ...15 when ADC multichannel enable is present * @param[out]   data     :Output data 
 * @param[in]    data     : Output data in dynamic mode 
 * @return       DAC output data , where DAC in dynamic mode. 
 *
 */
uint16_t RSI_DAC_DynamicModeReadData(AUX_ADC_DAC_COMP_Type *pstcDAC, uint32_t channel,uint16_t data)
{
	data	= pstcDAC->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_1_b.CHANNEL_BITMAP ;
	data  = data << 1;
	data  = ( data | ( pstcDAC->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_0_b.CHANNEL_BITMAP >> 31 ));
	return data;
}


/**
 * @fn           uint32_t RSI_DAC_Start( AUX_ADC_DAC_COMP_Type *pstcDAC ,uint32_t channel, dac_config *config )
 * @brief        This API is used to start the DAC operation in Static or FIFO mode.
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[in]    aux_dac_en  : This parameter define DAC module start or not start.
 *                             aux_dac_en  : 1  -> Enable DAC in static or in dynamic mode operation.
 *                             aux_dac_en  : 0  -> Disable DAC in static or in dynamic mode operation. 
 * @return       execution status 
 *
 */
error_t RSI_DAC_Start( AUX_ADC_DAC_COMP_Type *pstcDAC ,uint16_t aux_dac_en)
{
	pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_EN_S       = 1U;		
  pstcDAC->AUXDAC_CTRL_1_b.ENDAC_FIFO_CONFIG  = 1U;			
	pstcDAC->AUXDAC_CTRL_1_b.DAC_ENABLE_F       = aux_dac_en ;
	return RSI_OK;
}

/**
 * @fn           uint32_t RSI_DAC_Stop( AUX_ADC_DAC_COMP_Type *pstcDAC )
 * @brief        This API is used to stop the DAC operation in FIFO or Static mode.
 * @param[in]    pstcADC             : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @return       execution status 
 *
 */
error_t RSI_DAC_Stop( AUX_ADC_DAC_COMP_Type *pstcDAC)
{
 	pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_DYN_EN     = 0U;		
  pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_EN_S       = 0U;
	pstcDAC->AUXDAC_CTRL_1_b.DAC_ENABLE_F       = 0U;
	pstcDAC->AUXDAC_CTRL_1_b.ENDAC_FIFO_CONFIG  = 0U;
	return RSI_OK;
}

/**
 * @fn           uint32_t RSI_DAC_Start( AUX_ADC_DAC_COMP_Type *pstcDAC ,uint32_t channel, dac_config *config )
 * @brief        This API is used to start the DAC in dynamic mode.
 * @param[in]    pstcADC      : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel      : DAC channel to be configured as 0,1,2 ...15 when ADC multichannel enable is present
 * @param[in]    aux_dac_en   : This parameter define DAC enable or disable.
 *                              aux_dac_en = 0 , Disable DAC for dynamic mode operation. 
 *                              aux_dac_en = 1 , Enable  DAC for dynamic mode operation. 
 * @return       execution status 
 *
 */
error_t RSI_DAC_DynamicModeStart( AUX_ADC_DAC_COMP_Type *pstcDAC ,uint32_t channel,
                                   uint16_t aux_dac_en)
{
	pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_EN_S = 1U;		
	pstcDAC->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_0_b.CHANNEL_BITMAP  |= aux_dac_en << 28;			
	return RSI_OK;
}

/**
 * @fn           error_t RSI_DAC_DynamicModeStop( AUX_ADC_DAC_COMP_Type *pstcDAC,uint32_t channel)
 * @brief        This API is used to stop the DAC in dynamic mode.
 * @param[in]    pstcADC  : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  : DAC channel to be configured as 0,1,2 ...15 when ADC multichannel enable is present
 * @return       execution status 
 *
 */
error_t RSI_DAC_DynamicModeStop( AUX_ADC_DAC_COMP_Type *pstcDAC,uint32_t channel)
{
	pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_DYN_EN = 0 ;		
  pstcDAC->AUXDAC_CONIG_1_b.AUXDAC_EN_S   = 0 ;
	pstcDAC->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_0_b.CHANNEL_BITMAP =  0 << 28;
  return RSI_OK;
}


/**
 * @fn           uint32_t RSI_DAC_ClkDivFactor(AUX_ADC_DAC_COMP_Type *pstcDAC, uint16_t div_factor)
 * @brief        This API is used to set clock with configurable on time 
 * @param[in]    pstcADC      : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    div_factor   : clock division factor to be programmed
 * @return       execution status 
 *
 */
error_t RSI_DAC_ClkDivFactor(AUX_ADC_DAC_COMP_Type *pstcDAC, uint16_t div_factor)
{
	if(div_factor > MAX_DIV_FAC_DAC)
	{
	 return INVALID_PARAMETERS;
	}
	pstcDAC->AUXDAC_CLK_DIV_FAC_b.DAC_CLK_DIV_FAC = div_factor ;
  return RSI_OK;
}


/**
 * @fn           uint32_t RSI_DAC_PowerControl(POWER_STATE state)
 * @brief        This API is used to Power On and off for DAC
 * @param[in]    state : DAC_POWER_ON - To powerup dac powergates
 *                       DAC_POWER_OFF - To powerdown dac powergates
 * @return       execution status 
 *
 */
void RSI_DAC_PowerControl(POWER_STATE_DAC state)
{  
	switch(state)
	{
		case DAC_POWER_ON  :
		RSI_IPMU_PowerGateSet(AUXDAC_PG_ENB);
		RSI_PS_UlpssPeriPowerUp(ULPSS_PWRGATE_ULP_AUX);
		break;
		case DAC_POWER_OFF :
		RSI_IPMU_PowerGateClr( AUXDAC_PG_ENB );
		break;
	}
}

/**
 * @fn           error_t RSI_DAC_SetFifoThreshold(AUX_ADC_DAC_COMP_Type *pstcDAC,uint32_t fifo_threshold)
 * @brief        This API is used to set fifo threshold for DAC.
 * @param[in]    pstcADC        : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    fifo_threshold : FIFO threshold value.
 * @return       execution status 
 *
 */
error_t RSI_DAC_SetFifoThreshold(AUX_ADC_DAC_COMP_Type *pstcDAC,uint32_t fifo_threshold)
{
	if(fifo_threshold > MAX_DAC_FIFO_THR)
	{
	  return INVALID_PARAMETERS;
	}	
	pstcDAC->AUXDAC_CTRL_1_b.DAC_FIFO_THRESHOLD   = fifo_threshold ;
	pstcDAC->AUXDAC_CTRL_1_b.DAC_FIFO_FLUSH = 1U;
	return RSI_OK;
}

/**
 * @fn           error_t RSI_DAC_InterruptUnMask(AUX_ADC_DAC_COMP_Type *pstcDAC)
 * @brief        This API is used to unmask the DAC interrupt in FIFO mode.
 * @param[in]    pstcADC  : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    none
 * @return       zero on success. 
 *
 */
error_t RSI_DAC_InterruptUnMask(AUX_ADC_DAC_COMP_Type *pstcDAC)
{
	pstcDAC->INTR_MASK_REG_b.DAC_FIFO_EMPTY_INTR_MASK    = 0;
	pstcDAC->INTR_MASK_REG_b.DAC_FIFO_AEMPTY_INTR_MASK   = 0;
	return RSI_OK;
}

/**
 * @fn           error_t RSI_DAC_InterruptMask(AUX_ADC_DAC_COMP_Type *pstcDAC)
 * @brief        This API is used to mask the DAC interrupt in FIFO mode.
 * @param[in]    pstcADC      : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[in]    none
 * @return       zero on success. 
 *
 */
error_t RSI_DAC_InterruptMask(AUX_ADC_DAC_COMP_Type *pstcDAC)
{
	pstcDAC->INTR_MASK_REG_b.DAC_FIFO_EMPTY_INTR_MASK    = 1;
	pstcDAC->INTR_MASK_REG_b.DAC_FIFO_AEMPTY_INTR_MASK   = 1;
	return RSI_OK;
}

/**
 * @fn           error_t RSI_DAC_InterruptMask(AUX_ADC_DAC_COMP_Type *pstcDAC)
 * @brief        This API is used to mask the DAC interrupt in FIFO mode.
 * @param[in]    pstcADC      : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[in]    none
 * @return       zero on success. 
 *
 */
error_t RSI_DAC_InterruptClr(AUX_ADC_DAC_COMP_Type *pstcDAC)
{
  pstcDAC->AUXDAC_CTRL_1_b.DAC_FIFO_FLUSH = 1U;
  return RSI_OK;
}







