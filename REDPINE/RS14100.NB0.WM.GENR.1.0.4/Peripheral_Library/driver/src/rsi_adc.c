/**
 * @file      rsi_adc.c
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
 *  This file contains APIs relared to Analog to Digital converter peripheral
 *
 *
 */

 /* Includes */
 
#include "rsi_chip.h"

adc_config_t adcConfig;
/**
 * @fn           error_t RSI_ADC_PingPongMemoryAdrConfig(AUX_ADC_DAC_COMP_Type *pstcADC, 
 *                                                       uint32_t channel ,uint32_t ping_addr,
 *																											 uint32_t pong_addr,uint16_t ping_length,
 *																											 uint16_t pong_length,uint8_t ping_enable,
 *																											 uint8_t pong_enable )
 * @brief        This API is used to configure Ping and pong memory location along with 
 *               length of ping memory and pong memory.
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel     : ADC channel to be configured as 0,1,2 ...15
 * @param[in]    ping_addr   : ping address configuration.
 * @param[in]    pong_addr   : pong address configuration.(e.g pong address = ping address +
 *                                                                             ping length ) 
 * @param[in]    ping_length : ping length 
 * @param[in]    pong_length : pong length 
 * @param[in]    ping_enable : ping enable 
 * @param[in]    ping_length : ping enable  
 * @return       execution status 
 * @Note         If ping address is start from 0x24060000 and ping length is decimal 512 
 *               then pong address send as (0x24060000+(512))
 */
error_t RSI_ADC_PingPongMemoryAdrConfig(AUX_ADC_DAC_COMP_Type *pstcADC,
                                        uint32_t channel ,
																				uint32_t ping_addr,
                                        uint32_t pong_addr,
																				uint16_t ping_length, 
																				uint16_t pong_length,
																				uint8_t ping_enable,
																				uint8_t pong_enable )
{
	if(( channel > MAX_CHNL_NO)|| 
		 ( ping_length > PING_LEN_MAX ) ||
	   ( pong_length > PONG_LEN_MAX ) ||
		 ( ping_addr > PING_ADDR_MAX  ) ||
	   ( pong_addr > PONG_ADDR_MAX) )
	{
	  return INVALID_PARAMETERS;
	}		
	adcConfig.ping_addr[channel] = ping_addr;
	adcConfig.pong_addr[channel] = pong_addr;
	adcConfig.ping_length[channel] = ping_length;
	adcConfig.pong_length[channel] = pong_length;
	
	if(ping_addr == pong_addr)
	{
		return ERROR_PING_PONG_ADDR_MATCH;	
	}
	else
  {	
		if(ping_enable)
		{
			ping_addr = ping_addr + (ping_length*2) ;
			pstcADC->ADC_INT_MEM_1_b.PROG_WR_DATA = ping_addr	;
			adcConfig.ping_mem1[channel] = ping_addr;
			
			pstcADC -> ADC_INT_MEM_2 = (1 << POS15) | ((2 * channel) << POS10) | (ping_length << POS0);
			adcConfig.ping_mem2[channel] = pstcADC->ADC_INT_MEM_2;
			
			pstcADC->ADC_INT_MEM_2 = (0 << POS15) | ((2*channel) << POS10) | (ping_length << POS0);
			adcConfig.ping_mem3[channel] = pstcADC->ADC_INT_MEM_2;	
		}
		if(pong_enable)
		{
	  	pong_addr = pong_addr + ((ping_length + (pong_length*2)) + 1) ;
 		  pstcADC->ADC_INT_MEM_1_b.PROG_WR_DATA = pong_addr	;
			adcConfig.pong_mem1[channel] = pong_addr;
			
			pstcADC->ADC_INT_MEM_2 = (1 << POS15) | ((2* channel + 1) << POS10) | (pong_length << POS0);
			adcConfig.pong_mem2[channel] = pstcADC->ADC_INT_MEM_2;
			
			pstcADC->ADC_INT_MEM_2 = (0 << POS15) | ((2* channel +1) << POS10) | (pong_length << POS0);
			adcConfig.pong_mem3[channel] = pstcADC->ADC_INT_MEM_2;					
		}
  }
	return RSI_OK;
}

/**
 * @fn           uint32_t RSI_ADC_PingPongReInit(AUX_ADC_DAC_COMP_Type *pstcADC,
 *                                                uint8_t ping_enable,uint8_t pong_enable )
 * @brief        This API is used to reconfigure Ping-pong memory location along with the length   
 *                of the samples
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel     : ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_PingPongReInit(AUX_ADC_DAC_COMP_Type *pstcADC, uint8_t channel,
                               uint8_t ping_enable,uint8_t pong_enable )
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}		
	if(ping_enable)
	{
		 pstcADC->ADC_INT_MEM_1_b.PROG_WR_DATA =  adcConfig.ping_mem1[channel];
		 pstcADC->ADC_INT_MEM_2 = adcConfig.ping_mem2[channel];
		 pstcADC->ADC_INT_MEM_2 = adcConfig.ping_mem3[channel];
	}
	if(pong_enable)
	{
		pstcADC->ADC_INT_MEM_1_b.PROG_WR_DATA =  adcConfig.pong_mem1[channel];
		pstcADC->ADC_INT_MEM_2 = adcConfig.pong_mem2[channel] ;
		pstcADC->ADC_INT_MEM_2 = adcConfig.pong_mem3[channel];
	}
	return RSI_OK;
} 

/**
 * @fn           void RSI_ADC_Calibration()
 * @brief        This API used to do ADC calibration.
 * @param[in]    None
 * @return       None 
 *
 */
void RSI_ADC_Calibration(void)
{
	volatile uint32_t auxadcCalibValueLoad=0 , auxadcCalibValue=0;
  volatile uint32_t calib_done=0,cal,impuDummyRead=0;

	AUX_ADC_DAC_COMP->AUXADC_CTRL_1_b.ADC_ENABLE |= 1U;
	AUX_ADC_DAC_COMP->AUXADC_CONFIG_2 |= BIT(10);

	impuDummyRead = ULP_SPI_MEM_MAP(0);
	if(calib_done==0)
	{	
	  ULP_SPI_MEM_MAP(AUXADCREG0) |= BIT(13) | BIT(12) | BIT(11) | BIT(8) | BIT(7) | BIT(6) ;
 	  while(GSPI_CTRL_REG1 & SPI_ACTIVE);	
	  /*Wait for 1*/
    while(!(ULP_SPI_MEM_MAP(SPAREREG2) & BIT(0)));
    /*wait for 0*/
    while((ULP_SPI_MEM_MAP(SPAREREG2) & BIT(0)));	
	  /*150 clocks of 1 Mhz wait*/
	  auxadcCalibValue = ULP_SPI_MEM_MAP(AUXADCREG2);
	  auxadcCalibValueLoad |= BIT(0) | BIT(7);
	  auxadcCalibValueLoad |= (auxadcCalibValue & 0x1F) <<2;
	  auxadcCalibValueLoad |= (((auxadcCalibValue >> POS6) & 0x1F) << POS8);
	  auxadcCalibValueLoad |= (((auxadcCalibValue >> POS11) & 0x1F) << POS13);
    calib_done = 1;
	}
  else
	{}
 
  ULP_SPI_MEM_MAP(AUXADCREG1) = (auxadcCalibValueLoad);
  while(GSPI_CTRL_REG1 & SPI_ACTIVE);
	AUX_ADC_DAC_COMP->AUXADC_CTRL_1_b.ADC_ENABLE = 0;
}

/**
 * @fn           error_t RSI_ADC_PingpongEnable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
 * @brief        This API is used to enable ping pong for corresponding ADC channels 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  : ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_PingpongEnable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}			
	pstcADC->ADC_SEQ_CTRL_b.ADC_SEQ_CTRL_PING_PONG |= 1 << channel;	
	return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_PingpongDisable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
 * @brief        This API is used to disable the ping pong for corresponding ADC channels 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  :ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_PingpongDisable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}			
	pstcADC->ADC_SEQ_CTRL_b.ADC_SEQ_CTRL_PING_PONG &= ~(1 << channel);	
	return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_InternalPerChnlDmaEnable(AUX_ADC_DAC_COMP_Type *pstcADC, * uint32_t channel)
 * @brief        This API is used to enable internal DMA for corresponding ADC channels 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  :ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_InternalPerChnlDmaEnable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}	
	pstcADC->ADC_SEQ_CTRL_b.ADC_SEQ_CTRL_DMA_MODE |= 1 << channel;	
	return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_InternalPerChnlDmaDisable(AUX_ADC_DAC_COMP_Type *pstcADC,
 *                                                         uint32_t channel)
 * @brief        This API is used to disable the internal dma channel for corresponding ADC channels 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  :ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_InternalPerChnlDmaDisable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}	
	pstcADC->ADC_SEQ_CTRL_b.ADC_SEQ_CTRL_DMA_MODE &= ~(1 << channel);	
	return RSI_OK;
}

/**
 * @fn           void RSI_ADC_Config(AUX_ADC_DAC_COMP_Type *pstcADC,uint8_t multi_channel_en,
 *                                   uint8_t static_fifo_mode, uint8_t fifo_threshold )
 * @brief        This API is used to configure the ADC channels 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    multi_channel_en : Multi channel enable parameter.
 * @param[in]    static_fifo_mode : Static mode  enable parameter,
 *                                  static_fifo_mode =1 static mode
 *                                  static_fifo_mode =0 fifo mode
 * @param[in]    fifo_threshold   : fifo threshold value for ADC operation.
 * @param[in]    internal_dma_en  : Internal DMA enable parameter.
 * @return       execution status.
 *
 */
error_t RSI_ADC_Config(AUX_ADC_DAC_COMP_Type *pstcADC,uint8_t multi_channel_en,
                       uint8_t static_fifo_mode, uint8_t fifo_threshold, 
                       uint8_t internal_dma_en )
{
	if(fifo_threshold > MAX_THRS_VAL)
	{
    return INVALID_PARAMETERS;	
	}	
	pstcADC->AUXADC_CTRL_1_b.ADC_NUM_PHASE= 1;
	pstcADC->AUXADC_CTRL_1_b.ADC_MULTIPLE_CHAN_ACTIVE = multi_channel_en ; 
	pstcADC->AUXADC_CTRL_1_b.ADC_STATIC_MODE = static_fifo_mode  ;
			
	pstcADC->AUXADC_CTRL_1_b.ADC_FIFO_THRESHOLD = fifo_threshold;
	pstcADC->AUXADC_CTRL_1_b.ADC_FIFO_FLUSH = 1;
	pstcADC->INTERNAL_DMA_CH_ENABLE_b.INTERNAL_DMA_ENABLE = internal_dma_en;	
	return RSI_OK;
}


/**
 * @fn           error_t RSI_ADC_ChannelConfig(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel , uint8_t an_perif_adc_ip_sel,uint8_t an_perif_adc_in_sel, uint8_t an_perif_adc_diffmode )
 * @brief        This API is used to configure the ADC channels 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  :ADC channel to be configured as 0,1,2 ...15
 * @param[in]    an_perif_adc_ip_sel     : ADC positive input select in static mode.
 * @param[in]    an_perif_adc_in_sel     : ADC negative input select in static mode.
 * @param[in]    an_perif_adc_diffmode   : ADC differential mode selection in static mode.
 * @return       channel configuration 
 *
 */
error_t RSI_ADC_ChannelConfig( AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel , 
                               uint8_t an_perif_adc_ip_sel,uint8_t an_perif_adc_in_sel,
                               uint8_t an_perif_adc_diffmode )
{
	if((channel > MAX_CHNL_NO) || (an_perif_adc_ip_sel > MAX_POS_IN_SEL) 
		 ||(an_perif_adc_in_sel > MAX_NEG_IN_SEL) ||
		 (an_perif_adc_diffmode > MAX_DIFF_MODE))
	{
	 return INVALID_PARAMETERS;
	}	
  if(pstcADC->AUXADC_CTRL_1_b.ADC_MULTIPLE_CHAN_ACTIVE)
  {		
	 if(an_perif_adc_diffmode)
	 {	
	   pstcADC->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_0_b.CHANNEL_BITMAP |=((an_perif_adc_diffmode << POS26)|
	                                                                                    (an_perif_adc_in_sel << POS22));
   }		
	 pstcADC->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_0_b.CHANNEL_BITMAP |= an_perif_adc_ip_sel << POS17  ;
  } 
	else
	{
	 if(an_perif_adc_diffmode)
	 {
		 pstcADC->AUXADC_CONFIG_1_b.AUXADC_DIFF_MODE = an_perif_adc_diffmode;
		 pstcADC->AUXADC_CONFIG_1_b.AUXADC_INN_SEL  = an_perif_adc_in_sel ;
	 }
	 pstcADC->AUXADC_CONFIG_1_b.AUXADC_INP_SEL = an_perif_adc_ip_sel ;
	}	
  return RSI_OK;
}	
/**
 * @fn           error_t RSI_ADC_ChannelSamplingRate(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel )
 * @brief        This API is used to configure the ADC sampling rate for ADC channels. 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  :ADC channel to be configured as 0,1,2 ...15
 * @param[in]    adc_ch_offset     : channel offset for each channel 
 * @param[in]    adc_ch_freq_val   : channel frequency for each channel to set sampling rate
 * @return       execution status 
 *
 */
error_t RSI_ADC_ChannelSamplingRate(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel , uint16_t adc_ch_offset, uint16_t adc_ch_freq_val)
{
	if(adc_ch_freq_val==1)
	{
		return ERROR_FREQ_VAL ;
	}
	else
	{
	  pstcADC->ADC_CH_OFFSET_b[channel].CH_OFFSET = adc_ch_offset ;
	  pstcADC->ADC_CH_FREQ_b[channel].CH_FREQ_VALUE= adc_ch_freq_val ; //fifo for channel 0 freq val is 1 offset is 0
	}
	return RSI_OK;
}



/**
 * @fn           error_t RSI_ADC_StaticMode(AUX_ADC_DAC_COMP_Type *pstcADC, adcConfig *config)
 * @brief        This API is used to configure the ADC in Static Mode 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    an_perif_adc_ip_sel   : ADC positive input select in static mode.
 * @param[in]    an_perif_adc_in_sel   : ADC negative input select in static mode.
 * @param[in]    an_perif_adc_diffmode : ADC differential mode selection in static mode.
 * @return       execution status 
 */
error_t RSI_ADC_StaticMode(AUX_ADC_DAC_COMP_Type *pstcADC,uint16_t an_perif_adc_ip_sel, uint16_t an_perif_adc_in_sel, uint8_t an_perif_adc_diffmode)
{
	pstcADC->AUXADC_CTRL_1_b.ADC_NUM_PHASE = 1;
 
	pstcADC->AUXADC_CTRL_1_b.ADC_STATIC_MODE = 1 ;

	pstcADC->AUXADC_CONFIG_2 |= BIT(10);
	
	if(an_perif_adc_diffmode)
	{
		pstcADC->AUXADC_CONFIG_1_b.AUXADC_DIFF_MODE = an_perif_adc_diffmode;
		pstcADC->AUXADC_CONFIG_1_b.AUXADC_INN_SEL  = an_perif_adc_in_sel ;
	}
	pstcADC->AUXADC_CONFIG_1_b.AUXADC_INP_SEL = an_perif_adc_ip_sel ;
	
	return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_FifoMode(AUX_ADC_DAC_COMP_Type *pstcADC,uint16_t an_perif_adc_ip_sel, uint16_t an_perif_adc_in_sel, uint8_t an_perif_adc_diffmode)
 * @brief        This API is used to configure the ADC in FIFO Mode 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    an_perif_adc_ip_sel   : ADC positive input select in Fifo mode.
 * @param[in]    an_perif_adc_in_sel   : ADC negative input select in Fifo mode.
 * @param[in]    an_perif_adc_diffmode : ADC differential mode selection in Fifo mode.
 * @return       execution status 
 */
error_t RSI_ADC_FifoMode(AUX_ADC_DAC_COMP_Type *pstcADC,uint16_t channel_no,uint16_t an_perif_adc_ip_sel, uint16_t an_perif_adc_in_sel, uint8_t an_perif_adc_diffmode)
{
	pstcADC->AUXADC_CTRL_1_b.ADC_NUM_PHASE = 1;
	
	pstcADC->INTERNAL_DMA_CH_ENABLE_b.PER_CHANNEL_ENABLE |= 1 << channel_no;	
	
	if(an_perif_adc_diffmode)
	{
		pstcADC->AUXADC_CONFIG_1_b.AUXADC_DIFF_MODE = an_perif_adc_diffmode;
		pstcADC->AUXADC_CONFIG_1_b.AUXADC_INN_SEL = an_perif_adc_in_sel ;
	}
	pstcADC->AUXADC_CONFIG_1_b.AUXADC_INP_SEL = an_perif_adc_ip_sel ;
	
	return RSI_OK;
}



/**
 * @fn           error_t RSI_ADC_Start( AUX_ADC_DAC_COMP_Type *pstcADC )
 * @brief        This API is used to start the ADC
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  : ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_Start( AUX_ADC_DAC_COMP_Type *pstcADC )
{
	pstcADC->AUXADC_CONFIG_2_b.AUXADC_CONFIG_ENABLE = 1U;
	pstcADC->AUXADC_CTRL_1_b.ADC_ENABLE = 1U;
	return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_Stop( AUX_ADC_DAC_COMP_Type *pstcADC )
 * @brief        This API is used to stop the ADC
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  :ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_Stop( AUX_ADC_DAC_COMP_Type *pstcADC )
{
	pstcADC->AUXADC_CONFIG_2_b.AUXADC_CONFIG_ENABLE = 0;
	pstcADC->AUXADC_CTRL_1_b.ADC_ENABLE = 0;
  return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_ChnlEnable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
 * @brief        This API is used to Enable the ADC
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  :ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_ChnlEnable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}		
	if((AUX_ADC_DAC_COMP->AUXADC_CTRL_1_b.ADC_MULTIPLE_CHAN_ACTIVE) || 
		(AUX_ADC_DAC_COMP->INTERNAL_DMA_CH_ENABLE_b.INTERNAL_DMA_ENABLE ))
	{
	  pstcADC->INTERNAL_DMA_CH_ENABLE_b.PER_CHANNEL_ENABLE  |= 1 << channel;
	}
	else
  {
	  return ERROR_NO_MULTI_CHNL_ENABLE;
	}
  return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_ChnlDisable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
 * @brief        This API is used to Disable the ADC
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  :ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_ChnlDisable(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}		
  pstcADC->INTERNAL_DMA_CH_ENABLE_b.PER_CHANNEL_ENABLE  &= ~( 1 << channel);
  return RSI_OK;
}

/**
 * @fn           uint32_t RSI_ADC_ReadDataStatic(AUX_ADC_DAC_COMP_Type *pstcADC)
 * @brief        This API is used to Read the ADC samples when static mode is enabled.
 * @param[in]    pstcADC             : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[out]   data_process_en     : This parameter define for the output ADC samples 
 *                                     gain and calculation enable or disable.
 *                                     data_process_en -  1 enable the gain 
 *                                                        and calculation on output samples
 *                                     data_process_en -  0 disable the gain 
 *                                                        and calculation on output samples
 * @param[in]    diff_en             : ADC mode of operation single ended or differential ended
 *                                     diff_en   - 1   differential ended mode enable.
 *                                     diff_en   - 0   single ended mode enable. 
 * @return       execution status 
 *
 */
int16_t RSI_ADC_ReadDataStatic(AUX_ADC_DAC_COMP_Type *pstcADC,uint8_t data_process_en,uint8_t diff_en)
{
   int16_t adc_temp=0;
   if(data_process_en)
   {
	   adc_temp = pstcADC->AUXADC_DATA_b.AUXADC_DATA;
	   adc_temp = RSI_ADC_GainOffsetCal(adc_temp,diff_en);
	   return adc_temp;
   }   
   else
   {   
    return pstcADC->AUXADC_DATA_b.AUXADC_DATA;
   } 
}
/**
 * @fn           error_t RSI_ADC_ReadData(AUX_ADC_DAC_COMP_Type *pstcADC, int16_t *data,
 *                        uint8_t ping_pong,uint16_t channel,uint8_t data_process_en,uint8_t *   diff_en)
 * @brief        This API is used to Read the ADC samples when ulp memories are used.
 * @param[in]    pstcADC         : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[out]   data            :Pointer to read buffer 
 * @param[in]    Channel         :ADC channel number 
 * @param[in]    ping_pong       : 1 - ping memory must be read
 *                                 0 - pong memory must be read
 * @param[in]    data_process_en : This parameter define process of sample or without samples. 
 * @param[in]    diff_en         : This parameter define differential mode enable or disable.
 * @return       execution status 
 *
 */
error_t RSI_ADC_ReadData(AUX_ADC_DAC_COMP_Type *pstcADC, int16_t *data,
                         uint8_t ping_pong,uint16_t channel,uint8_t data_process_en,uint8_t diff_en)
{
	uint16_t i,ping_len_read=0;
	
	if(ping_pong)
	{			
		for(i=0;i < adcConfig.ping_length[channel]; i++)
		{
			data[i] = *(volatile uint16_t *)(adcConfig.ping_addr[channel] + i*2 );
		  if(data_process_en) 
			 {
			   data[i] = RSI_ADC_GainOffsetCal(data[i],diff_en);
			 } 				
		}
	}
	else		
	{
		ping_len_read = adcConfig.ping_length[channel];	
		for(i=0;i < adcConfig.pong_length[channel]; i++)
		{
			data[i] = *(volatile uint16_t *)((adcConfig.pong_addr[channel] + ping_len_read) + i*2 );
		  if(data_process_en) 
			 {
			   data[i] = RSI_ADC_GainOffsetCal(data[i],diff_en);
			 } 	 
	  }	
	}	
	return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_ClkConfig(AUX_ADC_DAC_COMP_Type *pstcADC,
 *                                         uint16_t adc_on_time , uint16_t adc_total_duration
 *                                         )
 * @brief        This API is used to set clock with configurable on time 
 * @param[in]    pstcADC          : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    adcOnTime        : ON duration of the clock 
 * @param[in]    adcTotalDuration : Total ON and OFF duration of the clock
 * @return       execution status 
 * @note         mininum ON time is 15us if opamp comes into picture.
 *               adc should maintain minimum off time of 200ns.
 * When 1mhz is adc clk to obtain 15us on time adc_on_time must be programmed to 15 i.e. 15 clocks.
 * adc_total_duration = total clock duration i.e adc_on_time + adc_off time dution. (15 + 1) clocks
 */

error_t RSI_ADC_ClkDivfactor(AUX_ADC_DAC_COMP_Type *pstcADC, uint16_t adc_on_time ,
                             uint16_t adc_total_duration)
{
	if((adc_on_time > MAX_ON_DUR_TIME) || (adc_total_duration > MAX_TOL_DUR_TIME))
	{
	 return INVALID_PARAMETERS;		
	}
	pstcADC->AUXADC_CTRL_1_b.EN_ADC_CLK = 0; 
	pstcADC->AUXADC_CLK_DIV_FAC_b.ADC_CLK_ON_DUR = adc_on_time;
	pstcADC->AUXADC_CLK_DIV_FAC_b.ADC_CLK_DIV_FAC = adc_total_duration;
	pstcADC->AUXADC_CTRL_1_b.EN_ADC_CLK = 1;
	
  return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_ChnlIntrMask(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
 * @brief        This API is used to Unmask the ADC channel
 * @param[in]    pstcADC  : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[in]    channel  : ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 */
error_t RSI_ADC_ChnlIntrUnMask(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}		
	if((AUX_ADC_DAC_COMP->INTERNAL_DMA_CH_ENABLE_b.INTERNAL_DMA_ENABLE ))
	{
	  pstcADC->INTR_MASK_REG_b.FIRST_MEM_SWITCH_INTR_MASK &=~(1 << channel);
	}
	else
  {
		pstcADC->INTR_MASK_REG_b.THRESHOLD_DETECTION_INTR_EN = 0;		
		pstcADC->INTR_MASK_REG_b.ADC_FIFO_AFULL_INTR_MASK = 0;
		pstcADC->INTR_MASK_REG_b.ADC_FIFO_FULL_INTR_MASK = 0;
	}
	return RSI_OK;	
}

/**
 * @fn           error_t RSI_ADC_ChnlIntrMask(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
 * @brief        This API is used to Mask the ADC channel
 * @param[in]    pstcADC  : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[in]    channel  : ADC channel to be configured as 0,1,2 ...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_ChnlIntrMask(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}		
	pstcADC->INTR_MASK_REG_b.FIRST_MEM_SWITCH_INTR_MASK |=(1 << channel);
	return RSI_OK;		
}

/**
 * @fn           error_t RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
 * @brief        This API is used to clr the ADC channel
 * @param[in]    pstcADC  : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    channel  :ADC channel to be configured as 2^n where n = channel number = 0,1,2,3...15
 * @return       execution status 
 *
 */
error_t RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
{
	if(channel > MAX_CHNL_NO)
	{
	 return INVALID_PARAMETERS;
	}		
	 pstcADC->INTR_CLEAR_REG_b.INTR_CLEAR_REG |= (1 << (channel));
	 pstcADC->INTR_CLEAR_REG_b.INTR_CLEAR_REG &= ~(1 << (channel));
   return RSI_OK;
}

/**
 * @fn           uint32_t RSI_ADC_ChnlIntrStatus(AUX_ADC_DAC_COMP_Type *pstcADC, uint32_t channel)
 * @brief        This API is used to status the ADC channel
 * @param[in]    pstcADC  : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @return       execution status 
 *
 */
uint32_t RSI_ADC_ChnlIntrStatus(AUX_ADC_DAC_COMP_Type *pstcADC)
{
	return pstcADC->INTR_STATUS_REG_b.FIRST_MEM_SWITCH_INTR;
}
/**
 * @fn           void RSI_ADC_PowerControl(POWER_STATE state)
 * @brief        This API is used to Power On and off for ADC
 * @param[in]    state : ADC_POWER_ON - To powerup adc powergates
                         ADC_POWER_OFF - To powerdown adc powergates
 * @return       execution status 
 *
 */
void RSI_ADC_PowerControl(POWER_STATE state)
{
  switch(state)
	{
		case ADC_POWER_ON  :
		RSI_IPMU_PowerGateSet(AUXADC_PG_ENB);
		RSI_PS_UlpssPeriPowerUp(ULPSS_PWRGATE_ULP_AUX);
		break;
		case ADC_POWER_OFF :
		RSI_IPMU_PowerGateClr( AUXADC_PG_ENB  );
		break;
	}
}

/**
 * @fn           error_t RSI_ADC_NoiseAvgMode(AUX_ADC_DAC_COMP_Type *pstcADC, bool en_disable)
 * @brief        This API is used to Enable or Disable Noise averaging mode
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[in]    en_disable  : 1 - To enable noise averaging mode
 *                             0 - To disable noise averaging mode           
 * @return       execution status 
 * @NOTE:        When noise averaging mode is enabled adc gives more accurate values as output.
 * For example : When voltage of 1.37V is given the output is 0x21D. 
 *               When this mode is enabled accurate value of 0x223 is obtained.
 *
 */
error_t RSI_ADC_NoiseAvgMode(AUX_ADC_DAC_COMP_Type *pstcADC, bool en_disable)
{
	pstcADC->AUXADC_CTRL_1_b.BYPASS_NOISE_AVG = en_disable;
	ULP_SPI_MEM_MAP(0x110) |= en_disable << POS17 ;
	return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_TempSensorEnable(AUX_ADC_DAC_COMP_Type *pstcADC)
 * @brief        This API is used to Enable temp-Sensor 
 * @param[in]    pstcADC             : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @return       execution status 
 * @NOTE         ADC must be in static mode to check temperature measurement
 *
 */
error_t RSI_ADC_TempSensorEnable(AUX_ADC_DAC_COMP_Type *pstcADC)
{
	pstcADC->TS_PTAT_ENABLE_b.TS_PTAT_EN = 1U;
	return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_ThresholdConfig(AUX_ADC_DAC_COMP_Type *pstcADC , uint32_t threshold1,uint32_t threshold2, uint8_t range)
 * @brief        This API is used to compare threshold value with adc data 
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    threshold1  : threshold value to be programmed
 * @param[in]    threshold2  : threshold value to be programmed when range is 1 
 * @param[in]    range       : When adc data compared lies in certain range of threshold values set this bit
 * @return       execution status 
 * NOTE          threshold1 and  threshold2 are programmed to set upper and lower limits of the adc output. 
 *               When the output is within this limits an interrupt is rised.
 *
 */
error_t RSI_ADC_ThresholdConfig(AUX_ADC_DAC_COMP_Type *pstcADC ,
                                uint32_t threshold1,uint32_t threshold2, uint8_t range)
{
	int16_t temp_1=0;
	temp_1 = pstcADC->AUXADC_DATA_b.AUXADC_DATA >> 2;
	pstcADC->ADC_DET_THR_CTRL_0_b.RANGE_COMPARISON_ENABLE = 1U;	
	pstcADC->ADC_DET_THR_CTRL_0_b.ADC_INPUT_DETECTION_THRESHOLD_0 = threshold1;
	pstcADC->ADC_DET_THR_CTRL_0_b.ADC_INPUT_DETECTION_THRESHOLD_1 = (threshold1 << 8) ; 
	
	if((pstcADC->ADC_DET_THR_CTRL_0_b.ADC_INPUT_DETECTION_THRESHOLD_0  |
 		(pstcADC->ADC_DET_THR_CTRL_0_b.ADC_INPUT_DETECTION_THRESHOLD_1>>8)) <= temp_1 )
	{
		if((pstcADC->ADC_DET_THR_CTRL_0_b.ADC_INPUT_DETECTION_THRESHOLD_0 |
			(pstcADC->ADC_DET_THR_CTRL_0_b.ADC_INPUT_DETECTION_THRESHOLD_1>>8)) == temp_1 )
		{
			pstcADC->ADC_DET_THR_CTRL_0_b.COMP_EQ_EN = 1;
		}
		else
	  {
			pstcADC->ADC_DET_THR_CTRL_0_b.COMP_GRTR_THAN_EN = 1;	
		}	
	}
	else
	{	
		pstcADC->ADC_DET_THR_CTRL_0_b.COMP_LESS_THAN_EN = 1;
	}
	
	if(range)
	{
		pstcADC->ADC_DET_THR_CTRL_1_b.ADC_INPUT_DETECTION_THRESHOLD_2 = threshold2;
		pstcADC->ADC_DET_THR_CTRL_1_b.ADC_DETECTION_THRESHOLD_2_UPPER_BITS = ( threshold2 << 8 );
		
	  if((pstcADC->ADC_DET_THR_CTRL_1_b.ADC_INPUT_DETECTION_THRESHOLD_2  | 
			 (pstcADC->ADC_DET_THR_CTRL_0_b.ADC_INPUT_DETECTION_THRESHOLD_1>>8)) <= temp_1 )
	  {
		  if((pstcADC->ADC_DET_THR_CTRL_1_b.ADC_INPUT_DETECTION_THRESHOLD_2 |
				(pstcADC->ADC_DET_THR_CTRL_0_b.ADC_INPUT_DETECTION_THRESHOLD_1>>8)) == temp_1 )
		  {
			 pstcADC->ADC_DET_THR_CTRL_1_b.COMP_EQ_EN = 1;
		  }
			else
		  {
			 pstcADC->ADC_DET_THR_CTRL_1_b.COMP_GRTR_THAN_EN = 1;	
		  }	
	  }
		else
		{	
		  pstcADC->ADC_DET_THR_CTRL_1_b.COMP_LESS_THAN_EN = 1;
	  }	
	}
	
	return RSI_OK;
}

/**
 * @fn           error_t RSI_ADC_Bbp(AUX_ADC_DAC_COMP_Type *pstcADC ,uint8_t adc_bbp_en, uint8_t bbp_en, uint8_t bbp_id)
 * @brief        This API is used for baseband
 * @param[in]    pstcADC    : Pointer to the AUX_ADC_DAC_COMP_Type structure. 
 * @param[in]    adc_bbp_en : To enable adc to bbp path
 * @param[in]    bbp_en     : Adc samples are given to bbp
 * @param[in]    bbp_id     : Channel id for bbp samples
 * @return       execution status 
 *
 */

error_t RSI_ADC_Bbp(AUX_ADC_DAC_COMP_Type *pstcADC ,uint8_t adc_bbp_en, uint8_t bbp_en, uint8_t bbp_id)
{
	pstcADC->VAD_BBP_ID_b.AUX_ADC_BPP_EN = adc_bbp_en;
	pstcADC->VAD_BBP_ID_b.BPP_EN         = bbp_en ;
	pstcADC->VAD_BBP_ID_b.BPP_ID         = bbp_id ;
	
	return RSI_OK;
}

/**
 * @fn          error_t RSI_ADC_InterruptHandler( AUX_ADC_DAC_COMP_Type *pstcADC,RSI_ADC_CALLBACK_T *pADCCallBack)
 * @brief		    Handles all interrupt of ADC
 * @param[in]	  pstcADC   :  Pointer to the MCPWM instance register area
 * @param[in]	  pADCCallBack Pointer to the structure of type RSI_CALLBACK
*/
error_t RSI_ADC_InterruptHandler( AUX_ADC_DAC_COMP_Type *pstcADC,RSI_ADC_CALLBACK_T *pADCCallBack)
{
	uint32_t intr_status =0; 
	
	if((pADCCallBack->adccallbacFunc) == NULL)
	{
		return ERROR_ADC_INVALID_ARG;
	}
	if((AUX_ADC_DAC_COMP->AUXADC_CTRL_1_b.ADC_MULTIPLE_CHAN_ACTIVE) || 
		(AUX_ADC_DAC_COMP->INTERNAL_DMA_CH_ENABLE_b.INTERNAL_DMA_ENABLE ))
	{
		intr_status = RSI_ADC_ChnlIntrStatus(AUX_ADC_DAC_COMP);
		if(intr_status!=NULL)
		{	
		  if(intr_status & BIT(0))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL0_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL0_INTR , MULTI_CHANNEL_EVENT);
		  }	
		  else if(intr_status & BIT(1))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL1_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL1_INTR , MULTI_CHANNEL_EVENT);
		  } 
		  else if(intr_status & BIT(2))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL2_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL2_INTR , MULTI_CHANNEL_EVENT);
		  }
		  else if(intr_status & BIT(3))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL3_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL3_INTR , MULTI_CHANNEL_EVENT);
		  }		
		  else if(intr_status & BIT(4))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL4_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL4_INTR , MULTI_CHANNEL_EVENT);
		  } 		
		  else if(intr_status & BIT(5))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL5_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL5_INTR , MULTI_CHANNEL_EVENT);
		  }			
		  else if(intr_status & BIT(6))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL6_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL6_INTR , MULTI_CHANNEL_EVENT);
		  }	
		  else if(intr_status & BIT(7))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL7_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL7_INTR , MULTI_CHANNEL_EVENT);
		  }		
		  else if(intr_status & BIT(8))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL8_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL8_INTR , MULTI_CHANNEL_EVENT);
		  }	
		  else if(intr_status & BIT(9))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL9_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL9_INTR , MULTI_CHANNEL_EVENT);
		  }		
		  else if(intr_status & BIT(10))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL10_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL10_INTR , MULTI_CHANNEL_EVENT);
		  }	
		  else if(intr_status & BIT(11))
		  {  	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL11_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL11_INTR , MULTI_CHANNEL_EVENT);
		  }	
		  else if(intr_status & BIT(12))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL12_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL12_INTR , MULTI_CHANNEL_EVENT);
		  }			
		  else if(intr_status & BIT(13))
		  {	
	      RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL13_INTR);
		    pADCCallBack ->adccallbacFunc(ADC_CHNL13_INTR , MULTI_CHANNEL_EVENT);
	    }	
		 else if(intr_status & BIT(14))
		 {	
	     RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL14_INTR);
		   pADCCallBack ->adccallbacFunc(ADC_CHNL14_INTR , MULTI_CHANNEL_EVENT);
	   }	
     else
		 {	
	     RSI_ADC_ChnlIntrClr(AUX_ADC_DAC_COMP,ADC_CHNL15_INTR);
		   pADCCallBack ->adccallbacFunc(ADC_CHNL15_INTR , MULTI_CHANNEL_EVENT);
		 }
	 }
   else
   {} 		
	}
	if((!(AUX_ADC_DAC_COMP->AUXADC_CTRL_1_b.ADC_STATIC_MODE)) &&
		(!(AUX_ADC_DAC_COMP->INTERNAL_DMA_CH_ENABLE_b.INTERNAL_DMA_ENABLE)))
	{
	  AUX_ADC_DAC_COMP->AUXADC_CTRL_1_b.ADC_FIFO_FLUSH = 1;
		pADCCallBack ->adccallbacFunc(intr_status , FIFO_MODE_EVENT);
	}	
	return RSI_OK;
}	


/**
 * @fn           void RSI_ADC_ThreshInterruptClr(AUX_ADC_DAC_COMP_Type *pstcADC)
 * @brief        This API is used clear the threshold interrupt.
 * @param[in]    pstcADC     : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @return       None
 *
 */
void RSI_ADC_ThreshInterruptClr(AUX_ADC_DAC_COMP_Type *pstcADC)
{
	pstcADC->INTR_CLEAR_REG_b.CLR_INTR=1;
}	

/**
 * @fn           void RSI_ADC_AUXLdoConfig(AUX_ADC_DAC_COMP_Type *pstcADC,uint16_t bypass_en,uint16_t value)
 * @brief        This API is used enable or disable the LDO for application.
 * @param[in]    pstcADC   : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    bypass_en : This parameter define LDO bypass enable or disable.
 *                           bypass_en : 1 bypass the LDO    
 *                           bypass_en : 0 On LDO mode 
 * @param[in]    value     : Configuration value for LDO when bypass_en =1.
 * @return       None
 *
 */
void RSI_ADC_AUXLdoConfig(AUX_ADC_DAC_COMP_Type *pstcADC,uint16_t bypass_en,uint16_t value)
{
	pstcADC->AUX_LDO_b.ENABLE_LDO=1;
	pstcADC->AUX_LDO_b.LDO_DEFAULT_MODE=1;
	
	if(bypass_en)
	{	
    pstcADC->AUX_LDO_b.BYPASS_LDO = 1;
    pstcADC->AUX_LDO_b.LDO_CTRL = value;		
  }
	else
	{
    pstcADC->AUX_LDO_b.LDO_CTRL=value;
	}	
}

/**
 * @fn           int32_t  RSI_ADC_GainOffsetCal(int32_t data)
 * @brief        This API is used to calculate gain and offset value.
 * @param[in]    data : data sample. 
 * @param[in]    diff_en : This parameter define differential mode is enable or disable.
 * @return       None
 *
 */
int16_t  RSI_ADC_GainOffsetCal(int16_t data,uint8_t diff_en)
{  
    volatile	uint32_t adc_gain=0,integer=0;
    volatile float frac=0,temp=0,vref_v1,adc_offset=0,dataf=0,vef_voltage=3.2;	
    vref_v1=RSI_ADC_VrefCal();
  	if(data & BIT(11))
	  {
	    data = data & (ADC_MASK_VALUE);
	  }				
		else 
	  {
			data = data | BIT(11);	
		}	
		if(diff_en)
		{	
		 adc_offset = RSI_IPMU_Auxadcoff_DiffEfuse(); 	
		 adc_gain   = RSI_IPMU_Auxadcgain_DiffEfuse();
		}
		else
		{
		 adc_offset = RSI_IPMU_Auxadcoff_SeEfuse();			
     adc_gain   = RSI_IPMU_Auxadcgain_SeEfuse();	
		}	
		adc_offset = (adc_offset * (vef_voltage/vref_v1));		
		frac   = (adc_gain & (0x3FFF));
		frac  /= (1000);
		integer = (adc_gain>>14);		
		temp = frac + integer;	
		dataf = (((float)data- adc_offset)*temp); 		
		
		data = (dataf);
		if(data > 4095)
		{
		  data = 4095;
		}			
		if(dataf <= 0 )
		{
		  data = 0;
		}	
		if(data >= 2048)
		{
		 data=data-2048;	
		}	
		else
		{
		 data=data+2048;
		}	
    return data;
}

/**
 * @fn           uin16_t RSI_ADC_VrefCal(AUX_ADC_DAC_COMP_Type *pstcADC)
 * @brief        This API is used to calculate vref value for application.
 * @param[in]    pstcADC   : Pointer to the AUX_ADC_DAC_COMP_Type structure.
 * @param[in]    None. 
 * @return       None
 *
 */
float RSI_ADC_VrefCal(void)
{
	  volatile	uint32_t ldo_drv =0;
    volatile float vref_v,adc_offset=0;	
		ldo_drv = AUX_ADC_DAC_COMP->AUX_LDO_b.LDO_CTRL;
		if(AUX_ADC_DAC_COMP->AUX_LDO_b.BYPASS_LDO==1)
		{
		  vref_v=3.2;		
		}	
		else
		{
		 switch(ldo_drv)
     {
		   case 0 : vref_v = 1.6 ;
                break;			 
		   case 1 : vref_v = 1.68;
                break;						     
		   case 2 : vref_v = 1.76;
                break;		
		   case 3 : vref_v = 1.88;
                break;		
		   case 4 : vref_v = 1.92 ;
                break;		
		   case 5 : vref_v = 2;
                break;		
		   case 6 : vref_v = 2.08;
                break;		
		   case 7 : vref_v = 2.16;
                break;		
		   case 8 : vref_v = 2.24;
                break;		
		   case 9 : vref_v = 2.32;
                break;		
		   case 10 : vref_v= 2.4;
                break;		
		   case 11 : vref_v = 2.527 ;
                break;		
		   case 12 : vref_v = 2.56;
                break;		
		   case 13 : vref_v = 2.64;
                break;			
		   case 14 : vref_v = 2.72;
                break;
		   case 15 : vref_v = 2.8;
                break;	  
       default:
                return INVALID_PARAMETERS;											
		 }			 
		}	
		return vref_v;
}






