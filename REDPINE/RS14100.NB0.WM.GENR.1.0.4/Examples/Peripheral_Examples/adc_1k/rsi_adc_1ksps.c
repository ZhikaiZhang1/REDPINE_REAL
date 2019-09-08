/**
 * @file       rsi_adc_1ksps.c
 * @version    1.0
 * @date       19 July 2018
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example code of ADC functionality for 1ksps 
 *        as sampling rate with sleep wakeup.
 * @section Description
 * This files contains example code of ADC 1ksps sampling rate. 
 *
 *
 */
/*Includes*/

#include "rsi_chip.h"
#include "rsi_bod.h" 
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define ADC_IRQ_Handler       IRQ011_Handler
/* Private define ------------------------------------------------------------*/ 
#define  CHANNEL_NO            0
#define  PING_ADDRESS          0x24060000  
#define  PONG_ADDRESS          0x240603FF
#define  PING_LEN              1023
#define  PONG_LEN              1023
#define  FIFO_THR              3
#define  POSITIVE_INP_SEL      2
#define  NEGATIVE_INP_SEL      2
#define  INPUT_POS_PIN         4
#define  INPUT_NEG_PIN         5
float    max_input_voltage_scdc = 2.4;
/* Private Function prototype ------------------------------------------------------------*/ 
void hardware_setup(void);
/* Private variables ---------------------------------------------------------*/
volatile uint32_t i=0,ret,a;
float battery_status;
RSI_ADC_CALLBACK_T vsADCCallBack;


/* Private MACRO use for changing clock source */
#define   XTAL_32KHZ_CLOCK
//#define   ENOB               /* Enable this MACRO and to collect first 2k sample of ADC */ 

/* change input mode (single ended or differential mode) */
//#define SINGLE_ENDED
#define DIFFERENTIAL_ENDED


/* 32khz XTAL clock configuration */
#ifdef XTAL_32KHZ_CLOCK
#define SOC_OPER_FREQUENCY    32000000
#define RSI_BLINK_RATE        1000
volatile uint32_t 		_dwTickCount=0 ;		//systick cout variable

void SysTick_Handler(void)
{
	_dwTickCount++;

}
uint32_t GetTickCount( void )
{
	return _dwTickCount ;						// gets the tick count from systic ISR
}


void rsi_delay(uint32_t val)
{
	uint32_t start;
	ret=val;
	val=val/1000;
	if (val == 0)
		return;
	 start = GetTickCount();
	do {
	} while (GetTickCount() - start < val);
	return ;
}
#endif

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  ADC IRQ handler to clear the ADC interrupt re configure the ping and pong address of ADC.
 * @param  None  
 * @retval None
 */
void ADC_IRQ_Handler(void)
{	
	RSI_ADC_InterruptHandler(AUX_ADC_DAC_COMP,&vsADCCallBack);
}

/**
 * @brief  Call back function , This function interrupt handler  
 * @param  channel_no  : Channel number for clearing   
 * @retval None
 */
void call_register( uint16_t channel_no,uint8_t event)
{
	
#ifndef ENOB
	if(i)
	{
		/* Reconfigure the pong address */			
		RSI_ADC_PingPongReInit(AUX_ADC_DAC_COMP, CHANNEL_NO, 0,1);
		i=0;
	}
	else
	{	
		/* Reconfigure the ping address */			
		RSI_ADC_PingPongReInit(AUX_ADC_DAC_COMP,CHANNEL_NO, 1,0);
		i=1;
	}
#endif
	a++;
}

/**
 * @brief  This function configure ADC sampling rate as 1ksps.
 * @param  None
 * @retval None
 */
void adc_clk_config()
{
#ifdef XTAL_32KHZ_CLOCK
	 SysTick_Config(SOC_OPER_FREQUENCY/ RSI_BLINK_RATE);
	 RSI_ULPSS_EnableRefClks(MCU_ULP_32KHZ_XTAL_CLK_EN,ULP_PERIPHERAL_CLK,rsi_delay);
 	 RSI_ULPSS_AuxClkConfig(ULPCLK, ENABLE_STATIC_CLK ,ULP_AUX_32KHZ_XTAL_CLK);	
#else	
   RSI_ULPSS_AuxClkConfig(ULPCLK, ENABLE_STATIC_CLK ,ULP_AUX_32KHZ_RC_CLK);
#endif
	 RSI_ADC_ClkDivfactor(AUX_ADC_DAC_COMP,0,1); // ADC clk division factor 
	 RSI_ADC_ChannelSamplingRate(AUX_ADC_DAC_COMP,CHANNEL_NO,1,16); // channel , ch offset , ch freq val // 1KSPS
}

/**
 * @brief  This function use for configure the pin for ADC operation.
 * @param  input mode e.g mode : 0 single ended and mode : 1 differential
 * @retval None
 */
void adc_input_pin_mux(uint8_t mode)
{
  if(mode)
  {
    /* Differential mode pin config */
    /* both pos and neg input pins are ocnfigured for Differential mode */
    RSI_EGPIO_SetPinMux(EGPIO1,0,INPUT_POS_PIN,EGPIO_PIN_MUX_MODE7);
    RSI_EGPIO_SetPinMux(EGPIO1,0,INPUT_NEG_PIN,EGPIO_PIN_MUX_MODE7);     
  }
  else
  {
    /* Either pos or neg input can be selected for Single ended mode */
    RSI_EGPIO_SetPinMux(EGPIO1,0,INPUT_POS_PIN,EGPIO_PIN_MUX_MODE7);    
  }
}

/**
 * @brief  This function configure ADC parameter.
 * @param  None
 * @retval None
 */
void ADC_1k(void)
{
	vsADCCallBack.adccallbacFunc = call_register;	
	
	RSI_ADC_PowerControl(ADC_POWER_ON);
	
	/* Select the ADC clock calibration */	
	RSI_ULPSS_AuxClkConfig(ULPCLK, ENABLE_STATIC_CLK ,ULP_AUX_32MHZ_RC_CLK);
	
	 /* Bypass the Aux LDO */	
	RSI_ADC_AUXLdoConfig(AUX_ADC_DAC_COMP,1,0xb);
		
	RSI_ADC_ClkDivfactor(AUX_ADC_DAC_COMP,0,1);
	
	/* Enable ADC calibration */	
	RSI_ADC_Calibration();
	
	NVIC_EnableIRQ(ADC_IRQn);
	
	/* Configure the ADC FIFO threshold and mode of ADC */
	RSI_ADC_Config(AUX_ADC_DAC_COMP,1,0,FIFO_THR,1);
	
	/*Internal DMA enable */	
	RSI_ADC_InternalPerChnlDmaEnable(AUX_ADC_DAC_COMP,CHANNEL_NO);
	
	RSI_ADC_PingpongEnable(AUX_ADC_DAC_COMP,CHANNEL_NO);	

	/* Configure the ping and pong address */
	RSI_ADC_PingPongMemoryAdrConfig(AUX_ADC_DAC_COMP,CHANNEL_NO,
	                                PING_ADDRESS,PONG_ADDRESS,PING_LEN,PONG_LEN,1,1);

	RSI_ADC_ChnlEnable(AUX_ADC_DAC_COMP,CHANNEL_NO);
	
	RSI_ADC_ChnlIntrUnMask(AUX_ADC_DAC_COMP,CHANNEL_NO);
		
	RSI_ADC_NoiseAvgMode(AUX_ADC_DAC_COMP,ENABLE) ;
	
#ifdef SINGLE_ENDED
  /* Configure the ULP_GPIO4 for single ended operation */
  adc_input_pin_mux(0);
	RSI_ADC_ChannelConfig(AUX_ADC_DAC_COMP, CHANNEL_NO,POSITIVE_INP_SEL,0,0); // channel, ip, in, differential mode
#endif
#ifdef DIFFERENTIAL_ENDED
  /* Configure the ULP_GPIO4 and ULP_GPIO5 for differential operation */
  adc_input_pin_mux(1);
	RSI_ADC_ChannelConfig(AUX_ADC_DAC_COMP, CHANNEL_NO,POSITIVE_INP_SEL,NEGATIVE_INP_SEL,1); // channel, ip, in, differential mode
#endif		
	
	/* Configure the ADC sampling rate as 1ksps */ 
	adc_clk_config();
	
	RSI_ADC_Start(AUX_ADC_DAC_COMP);		
		
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	/* At this stage the MICROCONTROLLER clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs1xxxx.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs1xxxx.c file
	 */		
  volatile int forever=0;
	
	SystemCoreClockUpdate();

	/* Read the input suuply for chip */
	battery_status = RSI_BOD_SoftTriggerGetBatteryStatus();
	
	if(battery_status < max_input_voltage_scdc)
	{
	  RSI_IPMU_ProgramConfigData(hp_ldo_voltsel);		
	}	
	
	RSI_IPMU_M20rcOsc_TrimEfuse();
	
	hardware_setup();
	
	/* Set wakeup source as ADC interrupt */
	RSI_PS_SetWkpSources(ULPSS_BASED_WAKEUP);

	/* Configure the ADC parameter */	
	ADC_1k();
	
	forever=1;
	
	while(forever)
	{		
		/* Configure the Sleep type */
		RSI_PS_RetentionSleepConfig(0 , (uint32_t)RSI_PS_RestoreCpuContext, 0 , 4);
		/*Enter Deep sleep with retention*/
		RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,LF_32_KHZ_RC);
#ifdef ENOB
		 if(a==2)
		 {
		   RSI_ADC_Stop(AUX_ADC_DAC_COMP); 
	     RSI_PS_PowerStateChangePs2toPs4(100,100);
       while(1);			 
		 } 
#endif
	}

	/*Application will never reach here , just to satisfy the main*/
	return 0;
}
