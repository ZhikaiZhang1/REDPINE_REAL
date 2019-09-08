/**
 * @file       cts_continuousmode_example.c
 * @version    0.9
 * @date       27 Jun 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example code of all end to end functionality
 *
 * @section Description
 * This file contains CTS example for continuous mode
 *
 */
 
#include "rsi_cts.h"
#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_opamp.h"

#define SEED_VALUE        0x10241024
#define POLYNOMIAL        0x68110000
#define SAMPLING_PATTERN  0x12345678
#define WAKEUP_THRESHOLD   150
#define ON_TIME            256
#define OFF_TIME           4
#define CONTINUOUS_MODE    1
#define ONEHOT_MODE        0
#define CTS_ON_TIME        4000
#define CTS_OFF_TIME       1000000

uint32_t read_data_fifo=0,status=0;

/*Configure the gpio pins to be used in CTS */
void cts_pinmuxing()
{
	/*Enable the ren for ulpss*/
  RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN0);
	RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN3);
  RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN6);
  RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN7);
  RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN8);
  RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN9);
  RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN10);
  RSI_EGPIO_UlpPadReceiverDisable(AGPIO_PIN11);
	
	/*set the pin multiplexing*/  
  RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN0,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN3,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN6,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN7,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN8,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN9,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN10,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN11,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN4,AGPIO_MODE);
	RSI_EGPIO_SetPinMux(EGPIO1,0,AGPIO_PIN5,AGPIO_MODE);
	
	/*set the gpio direction*/
	RSI_EGPIO_SetDir(EGPIO1,0,AGPIO_PIN0,1); //TOUCH6
	RSI_EGPIO_SetDir(EGPIO1,0,AGPIO_PIN3,1); //TOUCH5
	RSI_EGPIO_SetDir(EGPIO1,0,AGPIO_PIN6,1); //TOUCH4
	RSI_EGPIO_SetDir(EGPIO1,0,AGPIO_PIN7,1); //TOUCH3
	RSI_EGPIO_SetDir(EGPIO1,0,AGPIO_PIN8,1); //TOUCH0
	RSI_EGPIO_SetDir(EGPIO1,0,AGPIO_PIN9,1);  //TOUCH1
	RSI_EGPIO_SetDir(EGPIO1,0,AGPIO_PIN10,1); //TOUCH2
	RSI_EGPIO_SetDir(EGPIO1,0,AGPIO_PIN11,1); //TOUCH7
}

/*CAP SENSOR IRQhandler*/
void ULPSS_CAP_SENSOR_IRQHandler(void)
{
  /*read the cts status*/	
	status=RSI_CTS_GetStatus(CTS);
	
	while((status & FIFO_EMPTY_STATUS));
	/*read the data*/
	read_data_fifo=RSI_CTS_ReadFifo(CTS);
	#ifdef DEBUG_UART
	DEBUGOUT("Data %x\n", read_data_fifo);
	#endif
	/*Clear interrupt */
  RSI_CTS_IntrClear(CTS);
 	return ;
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 * @note
 *    To use this example in one hot mode pass the parameter ONEHOT_MODE in FUNCTION
 *    RSI_CTS_ModeSelect(CTS,ONEHOT_MODE);
 *    And select any one sensor e.g SAMPLING_PATTERN 0x88888888 RSI_CTS_ConfigSamplingPattern(CTS,SAMPLING_PATTERN,1);
 */


int main()
{
  /*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/*enable the clock sources*/
	RSI_ULPSS_UlpPeriClkEnable(ULPCLK,TOUCH_SENSOR_PCLK_ENABLE);
	
  RSI_ULPSS_TouchClkConfig(ULPCLK,ULP_TOUCH_32MHZ_RC_CLK,1);
	/*configures the trim values*/
	RSI_PS_ConfigurTrimValues(5, 88, 5, 38, 4, 2); 
	#ifdef DEBUG_UART
	DEBUGINIT();
  #endif
	/*configure the gpio pins*/
	cts_pinmuxing();   
	/*enable static clk*/
	RSI_CTS_StaticClkEnable(CTS,ENABLE);  
  /*clk selection for cts operation*/
	RSI_CTS_ClkSelection(CTS,1,1,5,0);  
	/*set the fifo aempty threshold*/
  RSI_CTS_ThresholdProgam(CTS,4);	
	/*select cts mode of operation*/
	RSI_CTS_ModeSelect(CTS,CONTINUOUS_MODE);  
	/*enables sampling mode(averaging)*/
	RSI_CTS_ConfigSampleMode(CTS,ENABLE);  
	/*set the buffer delay*/
	RSI_CTS_BufferDelay(CTS,8);  
	/*configure the polynomial length,seed value and polynomial value for generator*/
	RSI_CTS_ConfigPolynomial(CTS,ENABLE,32,SEED_VALUE,POLYNOMIAL); 
  /*bypass the random no generator output*/
  RSI_CTS_BypassPRS(CTS,ENABLE);     
	/*config on and off duration of pwm pulse*/	
	RSI_CTS_ConfigOnOffTime(CTS,ON_TIME,OFF_TIME);   			
	/*configures the inter sensor delay and no of repetitions of sample*/
	RSI_CTS_ConfigSampling(CTS,4,1);      
	/*Configure sampling pattern and valid sensor*/
	RSI_CTS_ConfigSamplingPattern(CTS,SAMPLING_PATTERN,8);
	/*configure the wakeup threshold*/
	RSI_CTS_ConfigWakeUp(CTS,1,WAKEUP_THRESHOLD);     //if average is enabled write 1 to wakeup register 
	/*config ref voltage*/
	RSI_CTS_ConfigRefVoltage(CTS,4,ENABLE);
	/*if wake interrupt enabled enable the fifo afull interrupt*/
	RSI_CTS_FifoInterruptEnable(CTS,ENABLE);
	/*Enable the cts*/	
	RSI_CTS_Enable(CTS,ENABLE); 
	/*enable NVIC for cap sensor*/
	NVIC_EnableIRQ(CAP_SENSOR_IRQn);
	
	while(1){
    /*Wait for interrupt */
		__WFI();
	}
}