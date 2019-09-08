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
#include "rsi_bod.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define NPSS_ALARM_IRQHandler   IRQ028_Handler     /*<! Alarm IRQ Handler     */
#define NPSS_RTC_IRQ_Handler    IRQ029_Handler     /*<! RTC IRQ Handler       */
#define NVIC_RTC_ALARM          MCU_CAL_ALARM_IRQn /*<! RTC NVIC enable  */
#define NVIC_RTC                MCU_CAL_RTC_IRQn   /*<! RTC GPIO NVIC enable  */

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static float vbatt_status ;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


int temp=0;

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	SystemCoreClockUpdate();
#ifdef DEBUG_UART
	/*Init debug UART*/
	DEBUGINIT();
#endif
	/*Enable clock sources */
	RSI_IPMU_ClockMuxSel(1);
	RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL);
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);

	RSI_BOD_BodTestSel(1,3);
	while(1)
	{
		/* Get the battery level status */
    vbatt_status = RSI_BOD_SoftTriggerGetBatteryStatus() ;
		DEBUGOUT("VBATT_STATUS: %f \n",vbatt_status );
	  temp++ ;
	}
    
  /*Code will never reach here,Just to satisfy the standard main*/
}
