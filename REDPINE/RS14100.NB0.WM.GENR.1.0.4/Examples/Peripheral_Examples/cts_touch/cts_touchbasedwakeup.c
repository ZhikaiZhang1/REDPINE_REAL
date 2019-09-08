#include "rsi_chip.h" 
#include "rsi_board.h"
#include "rsi_cts.h"
#include "rsi_opamp.h"
#include "rsi_ds_timer.h"


#define NVIC_DST                  NPSS_MCU_INTR_IRQn
#define PMU_GOOD_TIME        31    /*Duration in us*/
#define XTAL_GOOD_TIME       31 
#define NPSS_GPIO_PIN   3

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
   /*read data*/
	read_data_fifo=RSI_CTS_ReadFifo(CTS);
  /*Clear interrupt */ 
	RSI_CTS_IntrClear(CTS);
	return ;
}

/*CTS application function*/
void RSI_CTS()
{
	/*enable the clock sources*/
	RSI_ULPSS_UlpPeriClkEnable(ULPCLK,TOUCH_SENSOR_PCLK_ENABLE);
	
  RSI_ULPSS_TouchClkConfig(ULPCLK,ULP_TOUCH_32MHZ_RC_CLK,1);
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
	/*if average is enabled write 1 to wakeup register*/
	RSI_CTS_ConfigWakeUp(CTS,1,WAKEUP_THRESHOLD);
	/*config ref voltage*/
	RSI_CTS_ConfigRefVoltage(CTS,4,ENABLE);
	/*if wake interrupt enabled enable the fifo afull interrupt*/
	RSI_CTS_FifoInterruptEnable(CTS,ENABLE);
	/*enable the cts*/	
	RSI_CTS_Enable(CTS,ENABLE);
	
	read_data_fifo=RSI_CTS_ReadFifo(CTS);
	/*enable NVIC for cap sensor*/
	NVIC_EnableIRQ(CAP_SENSOR_IRQn);

}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main()
{
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/*configure the npss gpio*/
	RSI_NPSSGPIO_InputBufferEn(NPSS_GPIO_PIN , 1U);
	RSI_NPSSGPIO_SetPinMux(NPSS_GPIO_PIN , 0);
	RSI_NPSSGPIO_SetDir(NPSS_GPIO_PIN , NPSS_GPIO_DIR_OUTPUT);
	/*call the hardware setup function*/
	hardware_setup();
	
	/*set the duration for dst*/
	RSI_DST_IntrUnMask();
	/*enable the nvic for dst timer*/
	NVIC_EnableIRQ(NVIC_DST);
	/*Make GPIO-Low */

  while(1)
	{			/*Make GPIO-Low */
	   RSI_NPSSGPIO_SetPin(NPSS_GPIO_PIN,0U);
		/*set the dst based wakeup source*/
     RSI_PS_SetWkpSources(ULPSS_BASED_WAKEUP);
		/*calls the cap sensor application function*/
		 RSI_CTS();	
     /*set the duration for dst*/
	   RSI_DST_DurationSet(CTS_ON_TIME);
		/*set the dst based wakeup source*/
	   RSI_PS_SetWkpSources(DST_BASED_WAKEUP);
		/*Configure the wake up paramters for boot loader */
		 RSI_PS_RetentionSleepConfig(0 , (uint32_t)RSI_PS_RestoreCpuContext, 0 , 4);  
		 /*Goto Sleep with retention */
     RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,DISABLE_LF_MODE);		
	
		 RSI_NPSSGPIO_SetPin(NPSS_GPIO_PIN,1U);
		 /*set the duration for dst*/
		 RSI_DST_DurationSet(CTS_OFF_TIME);
		 
		 RSI_NPSSGPIO_SetPin(NPSS_GPIO_PIN,0U);
		 /*clear the ulpss based Interrupt*/
	   RSI_PS_ClrWkpSources(ULPSS_BASED_WAKEUP);
     /*set the dst based wakeup*/
     RSI_PS_SetWkpSources(DST_BASED_WAKEUP);

     RSI_PS_RetentionSleepConfig(0 , (uint32_t)RSI_PS_RestoreCpuContext, 0 , 4);   
	  /*Goto Sleep with retention */
		 RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,DISABLE_LF_MODE);
		/*Upon wake up execution resumes from here*/
		
		/*Make GPIO-High */
		 RSI_NPSSGPIO_SetPin(NPSS_GPIO_PIN,1U);		
	
   }
}






