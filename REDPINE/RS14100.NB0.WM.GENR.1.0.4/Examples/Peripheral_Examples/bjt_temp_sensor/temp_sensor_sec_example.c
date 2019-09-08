#include "rsi_chip.h"
#include "rsi_adc.h"
#include "rsi_board.h"
#include "rsi_opamp.h"

#include "UDMA.h"
#include "rsi_ipmu.h"

extern RSI_DRIVER_UDMA Driver_UDMA1;
static RSI_DRIVER_UDMA *UDMAdrv1 = &Driver_UDMA1;
extern RSI_UDMA_HANDLE_T udmaHandle1;

#define ADC_CALIB   1
volatile uint32_t done,ret;

#define  BUFFER_SIZE    3
#define  NPSS_GPIO_PIN   3
uint16_t rx_buffer[BUFFER_SIZE];

#define  UDMA1_IRQHandler  IRQ010_Handler
#define  ADC_IRQ_Handler   IRQ011_Handler

#define NPSS_RTC_IRQ_Handler    IRQ029_Handler
#define NVIC_RTC MCU_CAL_RTC_IRQn

RTC_TIME_CONFIG_T rtcConfig;

uint8_t rtc_done=0;

void NPSS_RTC_IRQ_Handler(void)
{
	volatile uint32_t statusRead = 0;
	
	statusRead = RSI_RTC_GetIntrStatus();
	
	if(statusRead & NPSS_TO_MCU_SEC_INTR){
		RSI_RTC_IntrClear (NPSS_TO_MCU_SEC_INTR);
	}
	
  RSI_RTC_SetDateTime(RTC ,&rtcConfig);
   
  rtc_done=1;
	
	return;
}

opamp_config UnityGB =
{
	{
	 /*opamp1_dyn_en;*/         0, 
	 /*opamp1_sel_p_mux;*/      5, 
		/*opamp1_sel_n_mux;*/     4,
	 /*opamp1_lp_mode;*/        0,
	 /*opamp1_r1_sel;*/         1,
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

RSI_ADC_CALLBACK_T vsADCCallBack;
uint32_t adc_done=0;


void udmaTransferComplete(uint32_t event, uint8_t ch)
{
  if(event == UDMA_EVENT_XFER_DONE)
  {
    if(ch == 11)
    {
      done = 1;  
    }
  }
} 

void UDMA_Write()
{
	RSI_UDMA_CHA_CONFIG_DATA_T control;
  RSI_UDMA_CHA_CFG_T config;
	
	
	memset(&control, 0, sizeof(RSI_UDMA_CHA_CONFIG_DATA_T));
	memset(&config, 0, sizeof(RSI_UDMA_CHA_CFG_T));
	
	config.altStruct = 0;
  config.burstReq  = 1;
  config.channelPrioHigh = 0;
  config.periAck = 0;
  config.periphReq = 0;
  config.reqMask = 0;
  config.dmaCh     = 11;
	
	/* Setup source to desination copy for trigger for memory */
	/* Descriptor configurations */	 
	control.transferType       = UDMA_MODE_AUTO;
	control.nextBurst					 = 0;
	control.totalNumOfDMATrans = BUFFER_SIZE -1;
	control.rPower             = ARBSIZE_2;
	control.srcProtCtrl 			 = 0x000;
	control.dstProtCtrl 			 = 0x000;
	control.srcSize            = SRC_SIZE_16;
	control.srcInc             = SRC_INC_NONE;
	control.dstSize            = DST_SIZE_16;
	control.dstInc             = DST_INC_16;
		
   /* Initialise dma */ 
   UDMAdrv1->Initialize();
 
	
	 /* Configure dma channel */ 
   UDMAdrv1->ChannelConfigure( 11,(uint32_t )0x24043814, (uint32_t )rx_buffer, BUFFER_SIZE, 
                               control, &config, udmaTransferComplete );
   /* Enable dma channel */
   UDMAdrv1->ChannelEnable(11); 
}


void ADC_IRQ_Handler(void)
{
	RSI_ADC_InterruptHandler(AUX_ADC_DAC_COMP,&vsADCCallBack);
  adc_done=1;
}

uint32_t i=0,adc_off=0,Vbg=0;
int32_t Voffset=0;
float Temp;
uint16_t adc2_out[1]={0},adc_out[]={0};

uint16_t read_data;

void bjt_temp_sensor()
{
    RSI_ADC_ClkDivfactor(AUX_ADC_DAC_COMP,48,64); 
	
    RSI_Opamp1_Config(OPAMP,0,&UnityGB);	
	 
    RSI_ADC_Config(AUX_ADC_DAC_COMP,0,1,3,0);

    RSI_ADC_StaticMode(AUX_ADC_DAC_COMP,20,0,0);
	 		
    RSI_ADC_ChannelSamplingRate(AUX_ADC_DAC_COMP,0,0,2);
	 	
    RSI_ADC_Start(AUX_ADC_DAC_COMP);
	 
    RSI_ADC_FifoMode(AUX_ADC_DAC_COMP,0,20,0,0);

		while(adc_done);
			
    UDMA_Write();
	 
    UDMAdrv1->DMAEnable();
	 
    RSI_UDMA_ChannelSoftwareTrigger(udmaHandle1,11); 
	 
    while(!done); 

    done=0;
	 
    adc_out[i]=rx_buffer[i];
	 	  
    if(adc_out[i] & BIT(11) )
    {
      adc_out[i] = ((adc_out[i] ) & 0xF7FF);
    }else{						
      adc_out[i] = ( adc_out[i] | BIT(11) );
    }
			
    RSI_ADC_Stop(AUX_ADC_DAC_COMP);
		
    RSI_ADC_Config(AUX_ADC_DAC_COMP,0,1,3,0);
		 	 
    RSI_ADC_TempSensorEnable(AUX_ADC_DAC_COMP);

    RSI_ADC_StaticMode(AUX_ADC_DAC_COMP,23,0,0);		
	 
    RSI_ADC_Start(AUX_ADC_DAC_COMP);
		
    RSI_ADC_FifoMode(AUX_ADC_DAC_COMP,0,23,0,0);
	 
    while(adc_done);
		
    UDMA_Write();
		
    UDMAdrv1->DMAEnable();
	
    RSI_UDMA_ChannelSoftwareTrigger(udmaHandle1,11);
	 
    while(!done); 

    done=0;

    adc2_out[i]=rx_buffer[i];
    	 
    if(adc2_out[i] & BIT(11) )
    {
      adc2_out[i] = ((adc2_out[i] ) & 0xF7FF);
    }else{						
      adc2_out[i] = ( adc2_out[i] | BIT(11) );
    }
		
    RSI_ADC_Stop(AUX_ADC_DAC_COMP);
		
		adc_off=RSI_IPMU_Auxadcoff_SeEfuse();
		
		Vbg=RSI_IPMU_Vbg_Tsbjt_Efuse();
		
		Voffset=(961-RSI_IPMU_Delvbe_Tsbjt_Efuse());
  	 
 	  Temp=-273+(310*((((adc2_out[i]-adc_off)/(float)(adc_out[i]-adc_off))*(Vbg/(float)1000))+((Voffset)/(float)1000)));
#ifdef DEBUG_UART    
		DEBUGOUT("Temp=%f",Temp);
#endif
}

/*set bg sampling voltage*/
void RSI_BG_VlgProg()
{
	*(volatile uint32_t *)(0x2405A50C)   = 0x50E80010;

	*(volatile uint32_t *)(0x2405A4A4) |= BIT(4);

}

int main(void)
{	 
	/*Configures the system default clock and power configurations*/
	SystemCoreClockUpdate();

  RSI_BG_VlgProg();

#ifdef DEBUG_UART		
	DEBUGINIT();
#endif
	RSI_ADC_PowerControl(ADC_POWER_ON);
	
  RSI_ULPSS_AuxClkConfig(ULPCLK, ENABLE_STATIC_CLK ,ULP_AUX_32MHZ_RC_CLK);
	
  ULP_SPI_MEM_MAP(BG_SCDC_PROG_REG_1)|=BIT(3);
	
  *(volatile uint32_t *)(0x24043A10)   = 0x5b;
		
  RSI_ADC_ClkDivfactor(AUX_ADC_DAC_COMP,0,1);

	#if ADC_CALIB
  RSI_ADC_Calibration();
	#endif
	/*Init RTC*/
	RSI_RTC_Init(RTC);
	
	/*RTC configuration with some default time */
	rtcConfig.DayOfWeek        = Saturday;
	rtcConfig.MilliSeconds     = 999;
	rtcConfig.Century          = 0;
	rtcConfig.Day              = 18;
	rtcConfig.Hour             = 2;
	rtcConfig.Minute           = 20;
	rtcConfig.Month            = January;
	rtcConfig.Second           = 0 ;
	rtcConfig.Year             = 16;
	
	/*start RTC */
	RSI_RTC_Start(RTC);
	/*Set the RTC configuration*/
	RSI_RTC_SetDateTime(RTC ,&rtcConfig);
	/*Enable RTC second based interrupt*/
	RSI_RTC_IntrUnMask( RTC_SEC_INTR );
  /*Start RTC */
	RSI_RTC_Start(RTC);
	
	NVIC_EnableIRQ(ADC_IRQn);

	NVIC_EnableIRQ(UDMA1_IRQn);
	
	/*Enable NVIC for second based interrupts */  
	NVIC_EnableIRQ(NVIC_RTC);
  
  while(1)
	{		
	  bjt_temp_sensor();
	
    while(!rtc_done);	
		
		rtc_done=0;
	}	
}	
