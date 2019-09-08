#include "rsi_chip.h"
#include "rsi_opamp.h"


void opamp_pinmuxing()
{
	//OPAMP INPUT
	 /*ULP Pad receive enable*/
	RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN4);
  RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN7);
	RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN15);
	RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN11);
	RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN0);
	RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN9);
	RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN2);
	RSI_EGPIO_UlpPadReceiverDisable(TGPIO_PIN2);
	
	RSI_EGPIO_SetPinMux(EGPIO,0,TGPIO_PIN2,TGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN7,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN11,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN0,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN2,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN4,AGPIO_MODE);
	
	//OUTPUT OF OPAMP1,OPAMP2&OPAMP3
  
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN15,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN9,AGPIO_MODE);

}

//unity gain buffer


opamp_config UnityGB =
{
	{
	 /*opamp1_dyn_en;*/         0, 
	 /*opamp1_sel_p_mux;*/      1, 
		/*opamp1_sel_n_mux;*/     4,
	 /*opamp1_lp_mode;*/        0,
	 /*opamp1_r1_sel;*/         0,
	 /*opamp1_r2_sel;*/         0,
	 /*opamp1_en_res_bank;*/    0,
	 /*opamp1_res_mux_sel;*/    0,
	 /*opamp1_res_to_out_vdd;*/ 0,
	 /*opamp1_out_mux_en;*/     1,
   /*opamp1_out_mux_sel;*/    0,
	 /*opamp1_enable;*/         1
	},
	{
		/*   opamp2_dyn_en;*/      0,
		/* 	 opamp2_sel_p_mux;*/   0,
		/*	 opamp2_sel_n_mux;*/   0,
		/*	 opamp2_lp_mode;*/     0,
		/*	 opamp2_r1_sel;*/      0,
		/*	 opamp2_r2_sel;*/      0,
		/*	 opamp2_en_res_bank;*/ 0,
		/*	 opamp2_res_mux_sel;*/ 0,
		/*	 opamp2_res_to_out_vdd;*/ 0,
		/*	 opamp2_out_mux_en;*/     0,
		/*	 opamp2_enable;*/         0,
	
	},
	{
		/* opamp3_dyn_en;*/          0,
		/* opamp3_sel_p_mux;*/       0,
		/* opamp3_sel_n_mux;*/       0,
		/* opamp3_lp_mode;*/         0,
		/* opamp3_r1_sel;*/          0,
		/* opamp3_r2_sel;*/          0,
		/* opamp3_en_res_bank;*/     0,
		/* opamp3_res_mux_sel;*/     0,
		/* opamp3_res_to_out_vdd;*/  0,
		/* opamp3_out_mux_en;*/      0,
		/* opamp3_enable;*/          0,
	} 
};


int main()
{
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/* Configure an AUX clock source */
  RSI_ULPSS_AuxClkConfig(ULPCLK, ENABLE_STATIC_CLK ,ULP_AUX_32MHZ_RC_CLK);
	ULP_SPI_MEM_MAP(BG_SCDC_PROG_REG_1)|=BIT(3);
	  /* LDO mode enable */  
	RSI_ADC_AUXLdoConfig(AUX_ADC_DAC_COMP,0,0xb);

		/* Enables pad selection bits,enables receive_enable for the opamp pins and 
         configures the pins for opamp.(enable gpio muxing ) */ 
	opamp_pinmuxing();
	
	/*Configure an opamp1  */
	RSI_Opamp1_Config(OPAMP,0,&UnityGB);
	
	/*Configure an opamp2  */
	RSI_Opamp2_Config(OPAMP,0,&UnityGB);
	
	/*Configure  an opamp3  */
	RSI_Opamp3_Config(OPAMP,0,&UnityGB);
	
  while(1);
}
