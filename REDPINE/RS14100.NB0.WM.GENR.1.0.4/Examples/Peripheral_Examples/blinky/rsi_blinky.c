/**
 * @file       rsi_blinky.c
 * @version    0.9
 * @date       23 Jun 2017
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
 * This files contains example code to demonstrate the GPIO/LED toggle functionality.
 *
 *
 */
/*Includes*/

#include "rsi_chip.h"
#include "rsi_board.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define RSI_BLINK_RATE (10)	                       /* 10 ticks per second     */

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
  /*  Toggles the current state of a board '0' number LED. */
	RSI_Board_LED_Toggle(0);
}

/* Main application entry point */
int main(void)
{
	int forever = 1;
   /*Configures the system default clock and power configurations*/
	SystemCoreClockUpdate();

   /*	Enable the DEBUG UART port for debug prints and  Set up and initialize all required 
        blocks and functions  related to the board hardware.  */
	RSI_Board_Init();
 
  /*  Sets the state of a board '0'number LED to off(false). */ 
	RSI_Board_LED_Set(0, false);

	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / RSI_BLINK_RATE);

   /* LED toggling and debug prints for ever */
	while (forever) 
	{
		/*Sleep till interrupt occurs*/
#ifdef DEBUG_UART
     /* Prints on hyper-terminal as "Core is Sleeping...\n" */
		DEBUGOUT("Core is Sleeping...\n");
#endif
		/*Sleep untill next interrupt occurres*/
		__WFI();
	}
	/*Statement will never reach here , just to satisfy the standard main*/
	return 0;
}
