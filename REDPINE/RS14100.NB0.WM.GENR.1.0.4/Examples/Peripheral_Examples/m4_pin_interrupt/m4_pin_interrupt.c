 /**
 * @file       m4_pin_interrupt.c
 * @version    0.9
 * @date       20th Dec 2017
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
 * This file contains example code to demonstrate the GPIO PIN INTERRUPT functionality.
 *
 *
 */
/* Includes -------------------------------------------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"

/* Private typedef ------------------------------------------------------------------------------------------------*/

/* Private define -------------------------------------------------------------------------------------------------*/

/* Private macro --------------------------------------------------------------------------------------------------*/
#define PININT_IRQ_HANDLER         IRQ059_Handler                   /* GPIO interrupt IRQ function name            */
#define PININT_NVIC_NAME           EGPIO_PIN_7_IRQn                 /* GPIO interrupt NVIC interrupt name          */
#define M4_GPIO_PORT               0                                /* GPIO port number                            */
#define M4_GPIO_PIN                15                                /* GPIO pin number                             */
#define PIN_INT                    7                                /* Pin  interrupt number(0 to 7)               */

/* Private variables ----------------------------------------------------------------------------------------------*/

/* Private function prototypes ------------------------------------------------------------------------------------*/

/* Private functions ----------------------------------------------------------------------------------------------*/

/**
 * @brief	Interrupt handler
 * @return	Nothing
 */
void PININT_IRQ_HANDLER(void)
{
	uint32_t gintStatus;

	/*get interrupt status*/
	gintStatus=RSI_EGPIO_GetIntStat(EGPIO,PIN_INT);

	if((gintStatus &EGPIO_PIN_INT_CLR_RISING ) || (gintStatus &EGPIO_PIN_INT_CLR_FALLING ))
	{
		/*clear interrupt*/
		RSI_EGPIO_IntClr(EGPIO, PIN_INT ,INTERRUPT_STATUS_CLR);
	}
	else
	{
		RSI_EGPIO_IntMask(EGPIO , PIN_INT);
	}
	return ;
}



/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	/* At this stage the microcontroller clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs9116.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs9116.c file
	 */
	int forever = 1;
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	
	/*Enable clock for EGPIO module*/
	RSI_CLK_PeripheralClkEnable(M4CLK,EGPIO_CLK,ENABLE_STATIC_CLK);

	/*PAD selection*/
	RSI_EGPIO_PadSelectionEnable(1);

	/*REN enable */
	RSI_EGPIO_PadReceiverEnable(M4_GPIO_PIN);

	/*Configure default GPIO mode(0) */
	RSI_EGPIO_SetPinMux(EGPIO,M4_GPIO_PORT ,M4_GPIO_PIN,EGPIO_PIN_MUX_MODE0);

	/*Selects the pin interrupt for the GPIO*/
	RSI_EGPIO_PinIntSel(EGPIO, PIN_INT , M4_GPIO_PORT, M4_GPIO_PIN);

	/*Configures the edge /level interrupt*/
	RSI_EGPIO_SetIntLowLevelEnable(EGPIO,PIN_INT);

	/*Unmask the  interrupt*/
	RSI_EGPIO_IntUnMask(EGPIO , PIN_INT);

	/*NVIC enable */
	NVIC_EnableIRQ(PININT_NVIC_NAME);

	while (forever) 
	{
		RSI_EGPIO_IntUnMask(EGPIO , PIN_INT);
	}
	/*Statement will never reach here , just to satisfy the standard main*/
	return 0;
}
