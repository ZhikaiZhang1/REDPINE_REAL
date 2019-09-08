/**
 * @file       rsi_ir_example.c
 * @version    1.0
 * @date       18 Augest 2018
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contain the example code IR data decoding,and giving the pattern as output.
 *
 * @section Description
 * This example explain the IR functionality.
 */

/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_ir.h"
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define NVIC_IR                    IR_DECODER_IRQ 
/* Private define ------------------------------------------------------------*/ 
#define IR_OFF_DURATION            0x100
#define IR_ON_DURATION             0x10
#define IR_FRAME_DONE_THRESHOLD    0x2F
#define IR_DET_THRESHOLD           0x5
#define SOC_OPER_FREQUENCY         32000000
#define RSI_BLINK_RATE             1000  
#define SYSTICK_PIN                6
#define TEST_PIN_IR_INPUT          10U
#define TEST_PIN_IR_INPUT_MODE     4U
#define TEST_PIN_IR_OUTPUT         5U
#define TEST_PIN_IR_OUTPUT_MODE    1U

/* Private variables ---------------------------------------------------------*/  
uint32_t Memlocation_num;
uint16_t ir_data[100];
int status=0,a,b;
volatile uint8_t flag=0;
volatile uint8_t ir_intr=0;

/* Private function prototype---------------------------------------------------------*/  
void Gpio_Pin_Mux(void);

/**
 * @brief  IR decoder interrupt handler, its clear the pending interrupt.
 * @param  None
 * @retval None
 */
void IRQ015_Handler(void)
{
  /*Clear IRQ*/
  NVIC_ClearPendingIRQ(NVIC_IR);  
	ir_intr=1;
	flag = 0;
  return ;
}

/**
 * @brief  Systic handler to emulate the IR pattern.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
	static uint8_t toggle=0;
	/*Emulates IR patterns*/
	if(flag < 10)
	{
		toggle = !toggle;
    RSI_EGPIO_SetPin(EGPIO, 0, SYSTICK_PIN, toggle);
	}	
	flag++;
	return;
}

/**
 * @brief  Enable the systic pin for input to IR.
 * @param  None
 * @retval None
 */
void Gpio_Pin_Mux(void)
{
	RSI_EGPIO_PadSelectionEnable(1);
	RSI_EGPIO_SetPinMux(EGPIO , 0, SYSTICK_PIN  , EGPIO_PIN_MUX_MODE0);
	RSI_EGPIO_SetDir(EGPIO , 0, SYSTICK_PIN  ,EGPIO_CONFIG_DIR_OUTPUT);
}	

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	uint16_t i=0;

	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
 
	/*Clock enable */
  RSI_IPMU_ClockMuxSel(1);
	
	RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL);
	
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);

  RSI_CLK_PeripheralClkEnable1(M4CLK , ULPSS_CLK_ENABLE);
  
	/* Configure the ULP-IR clock */
	RSI_ULPSS_UlpPeriClkEnable(ULPCLK,IR_PCLK_EN);

	/* Set the pin mux for systic pattern generation pin */
	Gpio_Pin_Mux();
	
  /*ULP pad receive enable*/
	RSI_EGPIO_PadSelectionEnable(18);
	
  RSI_EGPIO_UlpPadReceiverEnable(TEST_PIN_IR_INPUT);
  
	RSI_EGPIO_UlpPadReceiverEnable(TEST_PIN_IR_OUTPUT); 
	/*GPIO Muxing enable */  
  RSI_EGPIO_SetPinMux(EGPIO1 , 0, TEST_PIN_IR_INPUT  , TEST_PIN_IR_INPUT_MODE);
  
	RSI_EGPIO_SetPinMux(EGPIO1 , 0, TEST_PIN_IR_OUTPUT , TEST_PIN_IR_OUTPUT_MODE);

  /*configure the IR off time */	
  RSI_IR_OffDuration(IR , IR_OFF_DURATION);     
  /*configure the IR on time */	
  RSI_IR_OnDuration(IR  , IR_ON_DURATION);       
	
  /* define the frame done threshold value */
	RSI_IR_Framedonethreshold(IR  , IR_FRAME_DONE_THRESHOLD); 
 
  /* define the detection  threshold value */ 
	RSI_IR_Detectionthreshold(IR  , IR_DET_THRESHOLD);         

  /* set the bit of enable clock ir core */
	RSI_IR_SetConfiguration(IR , CONFIG_EN_CLK_IR_CORE); 
	
 /* set the bit for continuous ir detection mode */ 
	RSI_IR_SetConfiguration(IR , CONFIG_EN_CONT_IR_DET);        
  /*enable the IRQ of IR */
	NVIC_EnableIRQ(NVIC_IR);    

  /* set the bit for ir detection mode */
  RSI_IR_SetConfiguration(IR , CONFIG_EN_IR_DET);
  
  /* Set the systic timer to generate the pattern */ 	
	SysTick_Config(SOC_OPER_FREQUENCY/ RSI_BLINK_RATE);
	
  while(1)
	{
		while(!ir_intr);
    ir_intr = 0;
		/* Wait for some time to read the data */ 
		i = 1000;
    while(i--);                          
  
    Memlocation_num=RSI_IR_GetMemoryDepth(IR);
  
    for(i= 0; i < Memlocation_num ; i++)
    {
      ir_data[i] = RSI_IR_ReadData(IR,i);
    }	
		/* Restart the IR module */
		RSI_IR_SoftwareRestart(IR);
  }
   /*Application code never reach here just to satisfy the standard main*/  
}

