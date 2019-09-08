 /**
 * @file       ulp_pin_interrupt.c
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
 * This files contains example code to demonstrate the ULPSS GPIO PIN INTERRUPT functionality.
 *
 *
 */
/* Includes -------------------------------------------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"

/* Private typedef ------------------------------------------------------------------------------------------------*/

/* Private define -------------------------------------------------------------------------------------------------*/

/* Private macro --------------------------------------------------------------------------------------------------*/
#define ULP_PININT_IRQ_HANDLER     IRQ018_Handler                   /* GPIO interrupt IRQ function name            */
#define ULP_PININT_NVIC_NAME       ULP_EGPIO_PIN_IRQn               /* GPIO interrupt NVIC interrupt name          */

#define PORT                       0                                /* GPIO port number                            */
#define GPIO_PIN                   6                                /* GPIO pin number  (0 to 15)                           */
#define ULP_PIN_INT                0                                /* ULP pin interrupt                           */

/* Private variables ----------------------------------------------------------------------------------------------*/

/* Private function prototypes ------------------------------------------------------------------------------------*/

/* Private functions ----------------------------------------------------------------------------------------------*/




/**
 * @brief	Interrupt handler
 * @return	Nothing
 */
void ULP_PININT_IRQ_HANDLER(void)
{
	uint32_t gintStatus;

	/*get interrupt status*/
	gintStatus=RSI_EGPIO_GetIntStat(EGPIO1,ULP_PIN_INT);

	if((gintStatus &EGPIO_PIN_INT_CLR_RISING ) || (gintStatus &EGPIO_PIN_INT_CLR_FALLING ))
	{
		/*clear interrupt*/
		RSI_EGPIO_IntClr(EGPIO1, ULP_PIN_INT ,EGPIO_PIN_INT_CLR_RISING);
	}
	else
	{
		RSI_EGPIO_IntMask(EGPIO1 , ULP_PIN_INT);
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
	RSI_ULPSS_PeripheralEnable(ULPCLK,ULP_EGPIO_CLK,ENABLE_STATIC_CLK);

	/*Ren enable for input pin*/
	RSI_EGPIO_UlpPadReceiverEnable(GPIO_PIN);

	/*Configure default GPIO mode(0) */
	RSI_EGPIO_SetPinMux(EGPIO1,PORT ,GPIO_PIN,EGPIO_PIN_MUX_MODE0);

	/*Selects the pin interrupt for the GPIO*/
	RSI_EGPIO_PinIntSel(EGPIO1, ULP_PIN_INT , PORT, GPIO_PIN);

	/*Configures the edge /level interrupt*/
	RSI_EGPIO_SetIntLowLevelEnable(EGPIO1,ULP_PIN_INT);

	/*unmask the  interrupt*/
	RSI_EGPIO_IntUnMask(EGPIO1 , ULP_PIN_INT);

	/*NVIC enable */
	NVIC_EnableIRQ(ULP_EGPIO_PIN_IRQn);

	while (forever)
	{
		/*unmask the  interrupt*/
		RSI_EGPIO_IntUnMask(EGPIO1 , ULP_PIN_INT);
	}
	/*Statement will never reach here , just to satisfy the standard main*/
	return 0;
}
