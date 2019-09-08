/**
 * @file       rsi_adc_example.c
 * @version    1.0
 * @date       17 July 2018
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contains example describing usage of ADC APIs.
 *
 * @section Description
 * Conversion of Analog signal to Digital samples with 100ksps sampling rate using ADC
 */

/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_bod.h"
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define  ADC_IRQ_Handler   IRQ011_Handler
/* Private define ------------------------------------------------------------*/ 
#define  PING_ADDRESS              0x24060000      
#define  PONG_ADDRESS              0x240603FF  

#define  PING_LENGTH               1023
#define  PONG_LENGHT               1023

#define  PING_ENABLE               1
#define  PONG_ENABLE               1
#define  CHANNEL_NO                1
#define  FIFO_THR                  3
           
#define  ANALOG_MODE               7  
#define  POSITIVE_INP_SEL          2 
#define  NEGATIVE_INP_SEL          2
#define  INPUT_MODE                1           /* If INPUT_MODE is 0,single ended mode will be configured  
                                                  If INPUT_MODE is 1,differential mode will be configured */
#define  CLOCK_DIV_FAC             16  
#define  SAMPLING_RATE_FAC         10

#define  INPUT_POS_PIN             4           /* Here ULP_GPIO_4 is using as pos input to ADC,Can program ULP_GPIO pins from 0 to 15 */          
#define  INPUT_NEG_PIN             5           /* Here ULP_GPIO_5 is using as neg input to ADC,Can program ULP_GPIO pins from 0 to 15 */

#define  BUFFER_SIZE               1023

#define  GAIN_OFFSET_CAL_ENABLE    1           /* If this macro is 1 then gain and offset calculation will be done on ADC samples */   

float  max_ip_volt_scdc = 2.4;

/* Private variables ---------------------------------------------------------*/  

int16_t ping_buf[BUFFER_SIZE];
int16_t pong_buf[BUFFER_SIZE];

volatile uint16_t a,i;
RSI_ADC_CALLBACK_T vsADCCallBack;
volatile uint16_t dout;
volatile float vout,vref_v; 
float battery_status;
/* Private functions ---------------------------------------------------------*/


/**
 * @brief  This function print the input voltage.
 * @param  data    :  ADC output samples.  
 * @param  mode    :  Type of mode (Differential or single mode) 
 * @retval None
 */
void PrintADCInputVoltage(int16_t *data,uint8_t mode)
{
   vref_v = RSI_ADC_VrefCal();			

	 for(i=0;i<BUFFER_SIZE;i++)
	 {
			dout = ping_buf[i];
			if(dout & BIT(11))
			{
				  dout = (dout & (ADC_MASK_VALUE));
			}	
	   else
			{
				 dout = dout | BIT(11);	
			}	
	    vout = (( (float) dout / (float) 4096 ) * vref_v );  
	    if(mode)
			{
				 vout =vout-(vref_v/2);
			   DEBUGOUT("Differential ended input  :%f\n",vout);					
			}
      else
      {				
				DEBUGOUT("Single ended input :%f\n",vout);	
			}
		}
}	

/**
 * @brief  This function define pin mux.
 * @param  mode    :  Type of mode (Differential or single mode)  
 * @retval None
 */
void adc_input_pin_mux(uint8_t mode)
{
  if(mode)
  {
    /* Differential mode pin config */
    /* both pos and neg input pins are ocnfigured for Differential mode */
    RSI_EGPIO_SetPinMux(EGPIO1,0,INPUT_POS_PIN,ANALOG_MODE);
    RSI_EGPIO_SetPinMux(EGPIO1,0,INPUT_NEG_PIN,ANALOG_MODE);     
  }
  else
  {
    /* Either pos or neg input can be selected for Single ended mode */
    RSI_EGPIO_SetPinMux(EGPIO1,0,INPUT_POS_PIN,ANALOG_MODE);    
  }
}

/**
 * @brief  This function is ADC IRQ to clear the ADC interrupt.
 * @param  None
 * @retval None
 */
void ADC_IRQ_Handler(void)
{
	RSI_ADC_InterruptHandler(AUX_ADC_DAC_COMP,&vsADCCallBack);
}

/**
 * @brief  This function is callback function to reconfigure ping and pong address.
 * @param  channel_no  :  Channel number
 * @param  event       :  Event type 
 * @retval None
 */
void call_register( uint16_t channel_no,uint8_t event)
{
	if(event & MULTI_CHANNEL_EVENT)
  {
		if(a)
		{
		  /* Reconfigure the ping and pong address */			
	    RSI_ADC_PingPongReInit(AUX_ADC_DAC_COMP,channel_no,0,PONG_ENABLE);
			
      /* Read the pong address ADC data */			
		  RSI_ADC_ReadData(AUX_ADC_DAC_COMP,pong_buf,0,channel_no,GAIN_OFFSET_CAL_ENABLE,INPUT_MODE);
		  
		  /* print the equivalent ADC input voltage from obtain ADC samples */			
 			PrintADCInputVoltage(pong_buf,INPUT_MODE);    	
		  
			a=0;	
		}
		else
		{
		  /* Reconfigure the ping and pong address */	
	    RSI_ADC_PingPongReInit(AUX_ADC_DAC_COMP,channel_no,PING_ENABLE,0);	
			
		  /* Read the ping address ADC data */
		  RSI_ADC_ReadData(AUX_ADC_DAC_COMP,ping_buf,1,channel_no,GAIN_OFFSET_CAL_ENABLE,INPUT_MODE);	
		  
		  /* print the equivalent ADC input voltage from obtain ADC samples */
      PrintADCInputVoltage(ping_buf,INPUT_MODE);
 		  
			a=1;	
		}
  }
}
		
/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main()
{  
	/* At this stage the MICROCONTROLLER clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs1xxxx.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs1xxxx.c file
	 */
	SystemCoreClockUpdate();
		
  vsADCCallBack.adccallbacFunc = call_register;	
	
	/* Read the input suuply for chip */
	battery_status = RSI_BOD_SoftTriggerGetBatteryStatus();
	
	if(battery_status < max_ip_volt_scdc)
	{
	  RSI_IPMU_ProgramConfigData(hp_ldo_voltsel);		
	}		
	
	RSI_Board_Init();

	/* Powerup of ADC block */
	RSI_ADC_PowerControl(ADC_POWER_ON);
	
	/* Clock configuration of ADC */
	RSI_ULPSS_AuxClkConfig(ULPCLK, ENABLE_STATIC_CLK ,ULP_AUX_32MHZ_RC_CLK);
	
	/* Bypass ldo disable */
	RSI_ADC_AUXLdoConfig(AUX_ADC_DAC_COMP,0,0xb);
	
	/* Clock division factor , here divide clock to 1mhz*/
	RSI_ADC_ClkDivfactor(AUX_ADC_DAC_COMP,0,CLOCK_DIV_FAC);
  
  /* Calibrate the ADC */ 	
	RSI_ADC_Calibration();
	
	/* configures ULP GPIO 4 in analog mode */ 
	adc_input_pin_mux(INPUT_MODE);	
	
	/* Configure the ADC in multichannel mode with internal DMA */ 
	RSI_ADC_Config(AUX_ADC_DAC_COMP,1,0,FIFO_THR,1);
	
	/* Configure the required channel of ADC for conversion */
	RSI_ADC_ChannelConfig( AUX_ADC_DAC_COMP, CHANNEL_NO, POSITIVE_INP_SEL, NEGATIVE_INP_SEL, INPUT_MODE );
 
  /* Configure the ping and pong memory address */
	RSI_ADC_PingPongMemoryAdrConfig( AUX_ADC_DAC_COMP,CHANNEL_NO,PING_ADDRESS,PONG_ADDRESS,PING_LENGTH,
	                                 PONG_LENGHT, PING_ENABLE, PONG_ENABLE);	
  
  /* 	Configure the sampling rate of ADC */
  RSI_ADC_ChannelSamplingRate(AUX_ADC_DAC_COMP,CHANNEL_NO,0,SAMPLING_RATE_FAC);

  /* Enable the ADC interrupt */
	RSI_ADC_ChnlIntrUnMask(AUX_ADC_DAC_COMP,CHANNEL_NO);
  
	NVIC_EnableIRQ(ADC_IRQn);
	
	/* Ping pong enable */
	RSI_ADC_PingpongEnable(AUX_ADC_DAC_COMP,CHANNEL_NO);
	
	/* DMA enable */
	RSI_ADC_InternalPerChnlDmaEnable(AUX_ADC_DAC_COMP,CHANNEL_NO);
	
	/* ADC channel enable */
	RSI_ADC_ChnlEnable(AUX_ADC_DAC_COMP,CHANNEL_NO);		
	
	/* Starts ADC conversion */
	RSI_ADC_Start(AUX_ADC_DAC_COMP);
	
	while(1);
}	

