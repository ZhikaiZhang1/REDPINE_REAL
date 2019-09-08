/**
 * @file       rsi_comparator_example.c
 * @version    1.0
 * @date       25 Augest 2018
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contains example describing usage of comaparator APIs.
 *
 * @section Description
 * Compare the input voltages and on AGPIO0 and AGPIO1 ,based input voltage comparator
 * is hit.
 */
 /* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"

/* This example demonstrate simple application of comparator , here for comparator1 are used as well as
   take external input pin for comparator */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/ 
#define COMP1_POSITIVE_INPUT  0 /* Select the ULP_GPIO0 for comparator non-inverting input */   
#define COMP1_NEGATIVE_INPUT  0 /* Select the ULP_GPIO0 for comparator inverting input */ 
#define HYST_ENABLE           0  
#define FILTER_ENABLE         0   /* Filter disable */
#define COMP_OUTPUT_PIN       9    /* Select the ULP_GPIO9 for comparator output pin */ 
#define COMP_OUTPUT_PORT      0
#define COMP_OUTPUT_MODE      5
#define Enable                1    
#define Disable               0 

/* Private functions prototype---------------------------------------------------------*/
void comparator_pinmux(void);

/**
 * @brief  This function is comparator IRQ handler , its disable the comparator IRQ.
 * @param  None
 * @retval None
 */
void IRQ008_Handler(void)
{
   NVIC_DisableIRQ(COMP1_IRQn);	
}

/**
 * @brief  This function define pin mux for comparator.
 * @param  mode    :  Type of mode (Differential or single mode)  
 * @retval None
 */
void comparator_pinmux(void)
{
	RSI_EGPIO_PadReceiverDisable(AGPIO_PIN0);
	RSI_EGPIO_PadReceiverDisable(AGPIO_PIN1);
	RSI_EGPIO_PadReceiverDisable(AGPIO_PIN9);	
	
	RSI_EGPIO_SetPinMux(EGPIO1,AGPIO_PORT ,AGPIO_PIN0,AGPIO_MODE);	   /* non-inverting external input pin */ 
	RSI_EGPIO_SetPinMux(EGPIO1,AGPIO_PORT ,AGPIO_PIN1,AGPIO_MODE);    /*  inverting external input pin */
	
	RSI_EGPIO_SetPinMux(EGPIO1,COMP_OUTPUT_PORT,COMP_OUTPUT_PIN,COMP_OUTPUT_MODE); /* comparator1 output pin */
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
	
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
  /* Configure the AUX clock source */
  RSI_ULPSS_AuxClkConfig(ULPCLK, ENABLE_STATIC_CLK ,ULP_AUX_32MHZ_RC_CLK);	
 
	/*  Address of a target register ULP  */
	ULP_SPI_MEM_MAP(BG_SCDC_PROG_REG1)|=BIT(3);
	
	/* Bypass ldo disable */
	RSI_ADC_AUXLdoConfig(AUX_ADC_DAC_COMP,0,0xb);
	
	/* Disable the receiver enable bit(REN) and configures the pin to Comparator */
	comparator_pinmux();
	
	/* Configure the comparator parameters. */
	RSI_COMP_Config(AUX_ADC_DAC_COMP,COMP1,COMP1_POSITIVE_INPUT,COMP1_NEGATIVE_INPUT,
	                HYST_ENABLE,FILTER_ENABLE);	
	
	/* enable  the comparator */
	RSI_COMP_Enable(AUX_ADC_DAC_COMP,COMP1,Enable);
	
  /* enables a device-specific interrupt in the NVIC interrupt controller of comparator . */
	NVIC_EnableIRQ(COMP1_IRQn);	
	
	while(1)	
	{
		/* enables a device-specific interrupt in the NVIC interrupt controller of comparator . */
	   NVIC_EnableIRQ(COMP1_IRQn);
	}
	
}	

