/**
 * @file       rsi_rtc_example.c
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
 * This files contains example RTC alarm example
 *
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define NPSS_ALARM_IRQHandler   IRQ028_Handler     /*<! Alarm IRQ Handler     */
#define NPSS_RTC_IRQ_Handler    IRQ029_Handler     /*<! RTC IRQ Handler       */
#define NVIC_RTC_ALARM          MCU_CAL_ALARM_IRQn /*<! RTC NVIC enable  */
#define NVIC_RTC                MCU_CAL_RTC_IRQn   /*<! RTC GPIO NVIC enable  */

/* Private macro -------------------------------------------------------------*/
#define ALARAM_PERIODIC_TIME    5    /*<! periodic alarm configuration in SEC */

/* Private variables ---------------------------------------------------------*/
/*<!RTC configuration structures     */
static RTC_TIME_CONFIG_T rtcConfig, readTime , alarmConfig; 

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  This is  Sec and msec IRQ handler of RTC
 * @param  None
 * @return None
 */
void NPSS_RTC_IRQ_Handler(void)
{
	volatile uint32_t statusRead = 0;

	statusRead = RSI_RTC_GetIntrStatus();
	/*Clear the milli seconds interrupt */
	if(statusRead & NPSS_TO_MCU_MSEC_INTR){
		RSI_RTC_IntrClear (NPSS_TO_MCU_MSEC_INTR);
	}

	if(statusRead & NPSS_TO_MCU_SEC_INTR){
		RSI_RTC_IntrClear (NPSS_TO_MCU_SEC_INTR);
	}
	return ;
}

/**
 * @brief  This is Alarm IRQ Handler
 * @param  None
 * @return None
 */
void NPSS_ALARM_IRQHandler(void)
{
	volatile uint32_t statusRead = 0;

	/*Get the interrupt status */
	statusRead = RSI_RTC_GetIntrStatus();
	if(statusRead & NPSS_TO_MCU_ALARM_INTR)
	{
		/*Clear wake up interrupt */
		RSI_RTC_IntrClear(RTC_ALARM_INTR);
		/*Update seconds for next boundary alarm */
		alarmConfig.Second   = (alarmConfig.Second + ALARAM_PERIODIC_TIME);

		/*Update minutes for next boundary alarm */
		if(alarmConfig.Second  >= 59)
		{
			alarmConfig.Second = 0;
			alarmConfig.Minute = (alarmConfig.Minute + 1);
		}
		/*Update minutes for next boundary alarm */
		if(alarmConfig.Minute >= 59)
		{
			alarmConfig.Minute =0;
			alarmConfig.Hour = (alarmConfig.Hour + 1);
		}
		/*Update date for next boundary alarm */
		if(alarmConfig.Hour  >= 23)
		{
			alarmConfig.Hour  =0;
			alarmConfig.Day = (alarmConfig.Day + 1);
		}
		/*Reconfigure the Alarm for next alarm boundary*/
		RSI_RTC_SetAlarmDateTime(RTC , &alarmConfig);
	}
  RSI_RTC_GetDateTime(RTC , &readTime);
#ifdef DEBUG_UART
  DEBUGOUT("TIME: %d:%d:%d:%d\n",readTime.Hour,readTime.Minute,readTime.Second,readTime.MilliSeconds);
  DEBUGOUT("DATE: %d/%d/%d\n\n",readTime.Day,readTime.Month,(readTime.Year + 2000));  
#endif // DEBUG_UART 
	return ;
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();	
#ifdef DEBUG_UART
	/*Init debug UART*/
	DEBUGINIT();
#endif
	/*Enable clock sources */
	RSI_IPMU_ClockMuxSel(1);
	RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL);
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);

//	/*Init RTC*/
//	RSI_RTC_Init(RTC);
  
	/*RTC configuration with some default time */
	rtcConfig.DayOfWeek        = Saturday;
	rtcConfig.MilliSeconds     = 0;
	rtcConfig.Century          = 0;
	rtcConfig.Day              = 27;
	rtcConfig.Hour             = 6;
	rtcConfig.Minute           = 30;
	rtcConfig.Month            = Febuary;
	rtcConfig.Second           = 2 ;
	rtcConfig.Year             = 17;

	/*RTC alarm configuration */
	alarmConfig.DayOfWeek      = Saturday;
	alarmConfig.Second         = (2 + ALARAM_PERIODIC_TIME);
	alarmConfig.MilliSeconds   = 0;
	alarmConfig.Century        = 0;
	alarmConfig.Day            = 27;
	alarmConfig.Hour           = 6;
	alarmConfig.Minute         = 30;
	alarmConfig.Month          = Febuary;
	alarmConfig.Year           = 17;



	/*start RTC */
	RSI_RTC_Start(RTC);
	/*Set the RTC configuration*/
	RSI_RTC_SetDateTime(RTC ,&rtcConfig);
	/*Set Alarm configuration */
	RSI_RTC_SetAlarmDateTime(RTC , &alarmConfig);
	/*Enable Alarm feature*/
	RSI_RTC_AlamEnable(RTC ,ENABLE);
	/*Enable RTC ALARM ,sec and milli second interrupts*/
	RSI_RTC_IntrUnMask(RTC_ALARM_INTR | RTC_SEC_INTR | RTC_MSEC_INTR);
	/*Initilization RTC CALIBRATION*/
	RSI_RTC_CalibInitilization();
	/* To calibrate rc and ro */
	RSI_RTC_ROCLK_Calib(TIME_PERIOD,1,1,3 ,1,1,0);
	/*Start RTC */
	RSI_RTC_Start(RTC);
	/*Enable NVIC for RTC */
	NVIC_EnableIRQ(NVIC_RTC_ALARM);
  /*Enable NVIC for Sec and milli second interrupts */  
	NVIC_EnableIRQ(NVIC_RTC);

	while(1){
    /*Wait for interrupt */
		__WFI();
	}
  /*Code will never reach here,Just to satisfy the standard main*/
}
