/**
 * @file      rsi_opamp.c
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
 *  This file contains APIs related to Analog to Digital converter peripheral
 *
 *
 */

 /* Includes */
 
 #include "rsi_chip.h"
 #include "rsi_opamp.h"
 
 /**   
 * @fn          void RSI_Opamp1_Config(AUX_ADC_DAC_COMP_Type *Opamp,uint32_t channel,opamp_config *config)
 * @brief       This API is used to configuration of an opamp1 
 * @param[in]   Opamp   : pointer to opamp
 * @param[in]   channel : channel no to set in dynamic mode           
 * @param[in]   config  : pointer to opamp configuration                       
 * @return      none
 */
void RSI_Opamp1_Config(OPAMP_Type *Opamp,uint32_t channel,opamp_config *config)
{
		
	if(config->opamp1_t.opamp1_dyn_en   == 1)
	{	
	  Opamp->OPAMP_1_b.OPAMP1_DYN_EN  = 1;
	  AUX_ADC_DAC_COMP->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_1_b.CHANNEL_BITMAP = ( config->opamp1_t.opamp1_out_mux_sel	     << 29 |  
	                                                                   config->opamp1_t.opamp1_sel_p_mux      		 << 25 |	 
	                                                                   config->opamp1_t.opamp1_sel_n_mux 			   << 22 |   
	                                                                   config->opamp1_t.opamp1_out_mux_en	     << 21 |   
	                                                                   config->opamp1_t.opamp1_res_to_out_vdd	 << 20 |  
	                                                                   config->opamp1_t.opamp1_res_mux_sel			 << 17 |   
	                                                                   config->opamp1_t.opamp1_en_res_bank	     << 16 |   
	                                                                   config->opamp1_t.opamp1_r2_sel			     << 13 |  
	                                                                   config->opamp1_t.opamp1_r1_sel		       << 11 |   
	                                                                   config->opamp1_t.opamp1_lp_mode	         << 10 |   
	                                                                   config->opamp1_t.opamp1_enable	         << 9   );

		
	}else{
	  Opamp->OPAMP_1_b.OPAMP1_EN_RES_BANK = config->opamp1_t.opamp1_en_res_bank;
	  Opamp->OPAMP_1_b.OPAMP1_INN_SEL = config->opamp1_t.opamp1_sel_n_mux;
	  Opamp->OPAMP_1_b.OPAMP1_INP_SEL = config->opamp1_t.opamp1_sel_p_mux   ;
	  Opamp->OPAMP_1_b.OPAMP1_LP_MODE = config->opamp1_t.opamp1_lp_mode	;
	  Opamp->OPAMP_1_b.OPAMP1_R1_SEL = config->opamp1_t.opamp1_r1_sel;
	  Opamp->OPAMP_1_b.OPAMP1_R2_SEL = config->opamp1_t.opamp1_r2_sel;
	  Opamp->OPAMP_1_b.OPAMP1_RES_MUX_SEL = config->opamp1_t.opamp1_res_mux_sel;
	  Opamp->OPAMP_1_b.OPAMP1_RES_TO_OUT_VDD = config->opamp1_t.opamp1_res_to_out_vdd;
	  Opamp->OPAMP_1_b.OPAMP1_OUT_MUX_EN=config->opamp1_t.opamp1_out_mux_en;
    Opamp->OPAMP_1_b.OPAMP1_OUT_MUX_SEL=config->opamp1_t.opamp1_out_mux_sel;
	  Opamp->OPAMP_1_b.OPAMP1_ENABLE=config->opamp1_t.opamp1_enable	;
	}
			
}

/**   
 * @fn          void RSI_Opamp2_Config(AUX_ADC_DAC_COMP_Type *Opamp,uint32_t channel,opamp_config *config)
 * @brief       This API is used to configuration of an opamp2 
 * @param[in]   Opamp   : pointer to opamp
 * @param[in]   channel : channel no to set in dynamic mode           
 * @param[in]   config  : pointer to opamp configuration                        
 * @return      none
 */
void RSI_Opamp2_Config(OPAMP_Type *Opamp,uint32_t channel,opamp_config *config)
{			
	
	if( config->opamp2_t.opamp2_dyn_en   == 1 )
	{	
	  Opamp->OPAMP_2_b.OPAMP2_DYN_EN   = 1;
	  AUX_ADC_DAC_COMP->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_1_b.CHANNEL_BITMAP =  (config->opamp2_t.opamp2_lp_mode << 31 |  
                                                                         config->opamp2_t.opamp2_enable  << 30 ) ;
   	AUX_ADC_DAC_COMP->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_2_b.CHANNEL_BITMAP = ( config->opamp2_t.opamp2_sel_p_mux	    << 14 |
				                                         config->opamp2_t.opamp2_sel_n_mux		<< 12 |
									 config->opamp2_t.opamp2_out_mux_en      << 11 |
									 config->opamp2_t.opamp2_res_to_out_vdd	 << 9  |
									 config->opamp2_t.opamp2_res_mux_sel     << 6  |
									 config->opamp2_t.opamp2_en_res_bank	 << 5  |
									 config->opamp2_t.opamp2_r2_sel		      << 2  |
									 config->opamp2_t.opamp2_r1_sel          << 0   );	
	}else{
	  Opamp->OPAMP_2_b.OPAMP2_EN_RES_BANK = config->opamp2_t.opamp2_en_res_bank;
	  Opamp->OPAMP_2_b.OPAMP2_INN_SEL = config->opamp2_t.opamp2_sel_n_mux;
	  Opamp->OPAMP_2_b.OPAMP2_INP_SEL = config->opamp2_t.opamp2_sel_p_mux   ;
	  Opamp->OPAMP_2_b.OPAMP2_LP_MODE = config->opamp2_t.opamp2_lp_mode	;
	  Opamp->OPAMP_2_b.OPAMP2_R1_SEL = config->opamp2_t.opamp2_r1_sel;
	  Opamp->OPAMP_2_b.OPAMP2_R2_SEL = config->opamp2_t.opamp2_r2_sel;
	  Opamp->OPAMP_2_b.OPAMP2_RES_MUX_SEL = config->opamp2_t.opamp2_res_mux_sel;
	  Opamp->OPAMP_2_b.OPAMP2_RES_TO_OUT_VDD = config->opamp2_t.opamp2_res_to_out_vdd;
	  Opamp->OPAMP_2_b.OPAMP2_OUT_MUX_EN=config->opamp2_t.opamp2_out_mux_en;
	  Opamp->OPAMP_2_b.OPAMP2_ENABLE=config->opamp2_t.opamp2_enable;
	}		
}



/**   
 * @fn          void RSI_Opamp3_Config(AUX_ADC_DAC_COMP_Type *Opamp,uint32_t channel,opamp_config *config)
 * @brief       This API is used to configuration of an opamp3 
 * @param[in]   Opamp   : pointer to opamp
 * @param[in]   channel : channel no to set in dynamic mode           
 * @param[in]   config  : pointer to opamp configuration                        
 * @return      none
 */			
void RSI_Opamp3_Config(OPAMP_Type *Opamp, uint32_t channel,opamp_config *config)
{
	if(config->opamp3_t.opamp3_dyn_en   == 1)
	{
	  Opamp->OPAMP_3_b.OPAMP3_DYN_EN   = 1 ;
	  AUX_ADC_DAC_COMP->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_3_b.CHANNEL_BITMAP = config->opamp3_t.opamp3_sel_p_mux;
	  AUX_ADC_DAC_COMP->ADC_CH_BIT_MAP_CONFIG[channel].ADC_CH_BIT_MAP_CONFIG_2_b.CHANNEL_BITMAP = (config->opamp3_t.opamp3_sel_n_mux    << 30 |
									config->opamp3_t.opamp3_out_mux_en	       << 29 |
									config->opamp3_t.opamp3_res_to_out_vdd    << 28 |
									config->opamp3_t.opamp3_res_mux_sel	     << 25 |
									config->opamp3_t.opamp3_en_res_bank	      << 24 |
									config->opamp3_t.opamp3_r2_sel			       << 21 |
									config->opamp3_t.opamp3_r1_sel		         << 19 |
		     					
		config->opamp3_t.opamp3_lp_mode 	         << 18 |
									config->opamp3_t.opamp3_enable	           << 17 );
																																				
	}else{
 	  Opamp->OPAMP_3_b.OPAMP3_EN_RES_BANK = config->opamp3_t.opamp3_en_res_bank;
	  Opamp->OPAMP_3_b.OPAMP3_INN_SEL = config->opamp3_t.opamp3_sel_n_mux;
	  Opamp->OPAMP_3_b.OPAMP3_INP_SEL = config->opamp3_t.opamp3_sel_p_mux   ;
	  Opamp->OPAMP_3_b.OPAMP3_LP_MODE = config->opamp3_t.opamp3_lp_mode	;
	  Opamp->OPAMP_3_b.OPAMP3_R1_SEL = config->opamp3_t.opamp3_r1_sel;
	  Opamp->OPAMP_3_b.OPAMP3_R2_SEL = config->opamp3_t.opamp3_r2_sel;
	  Opamp->OPAMP_3_b.OPAMP3_RES_MUX_SEL = config->opamp3_t.opamp3_res_mux_sel;
	  Opamp->OPAMP_3_b.OPAMP3_RES_TO_OUT_VDD = config->opamp3_t.opamp3_res_to_out_vdd;			
	  Opamp->OPAMP_3_b.OPAMP3_OUT_MUX_EN=config->opamp3_t.opamp3_out_mux_en;
	  Opamp->OPAMP_3_b.OPAMP3_ENABLE=config->opamp3_t.opamp3_enable;
	}

}
