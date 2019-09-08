/**
 * @file       rsi_bod_example.c
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
 *
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_bod.h"
#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_ps_ram_func.h"
#include "rsi_ds_timer.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define MANUAL_MODE          0 /* To enable Manual mode */
#define AUTOMATIC_MODE       1 /* To enable Manual mode  */
#define ENABLE               1 
#define DISABLE              0
#define NVIC_BUTTON          NPSS_TO_MCU_BUTTON_INTR_IRQn
#define NPSS_TO_MCU_BUTTON_IRQHandler   IRQ024_Handler 
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 * @brief	  Handle interrupt from Button
 * @return	Nothing
 */
void NPSS_TO_MCU_BUTTON_IRQHandler(void)
{ 
	/*Clear the Button interrupt */
	RSI_BOD_ButtonIntrClr();
}
/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	volatile static uint32_t button_number=0 ;
    /*Configures the system default clock and power configurations*/
  	SystemCoreClockUpdate();
    /* clock enable */
    RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL);
    /*set the BOD to Manual  mode*/
    RSI_BOD_SetMode(MANUAL_MODE) ;
    /* enable Button wake up */
    RSI_BOD_ButtonWakeUpEnable(ENABLE);
    /* enable button interrupt */
    RSI_BOD_ButtonIntrEnable(ENABLE);
    /* BOD NVIC enable */
    NVIC_EnableIRQ(NVIC_BUTTON);
	while(1)
	{
		/*Wait for interrupt */
    __WFI();
	}
  /*Statement will never reach here , just to satisfy the standard main*/
  return 0;	
}  
