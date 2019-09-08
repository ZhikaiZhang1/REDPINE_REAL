/**
 * @file       rsi_wakeup_interrupt.c
 * @version    0.9
 * @date       28 Jun 2017
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
 * This files contains example code to demonstrate the GPIO interrupt functionality.
 *
 *
 */
/* Includes ------------------------------------------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"

/* Private typedef -----------------------------------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------------------------------------------*/
#define WAKEUP_NVIC_NAME           EGPIO_WAKEUP_IRQn                /* GPIO wake up NVIC interrupt name           */
#define GROUPINT_IRQ_WAKEUPHANDLER IRQ041_Handler                   /* GPIO wake up IRQ function name             */

#define POL_HIGH                   1                                /* polarity high                              */
#define POL_LOW                    0                                /* polarity low                               */

#define GROUPINT_1                 0                                /* group interrupt one                        */

#define M4_GPIO_PORT               0                                /* GPIO port number                           */
#define M4_PIN1                    6                                /* GPIO pin number                            */
#define M4_PIN2                    7                                /* GPIO PIN NUMBER                            */


/* Private variables ---------------------------------------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------------------------------------------*/

/* Private functions ---------------------------------------------------------------------------------------------*/




/**
 * @brief	Interrupt handler
 * @return	Nothing
 */
void GROUPINT_IRQ_WAKEUPHANDLER(void)
{
	uint32_t gintStatus;

	/*read group interrupt status*/
	gintStatus=RSI_EGPIO_GroupIntStat(EGPIO, GROUPINT_1);
	if(gintStatus & WAKEUP_INTERRUPT ){
		/*Disables the wakeup interrupt*/
		RSI_EGPIO_GroupIntWkeUpDisable(EGPIO , GROUPINT_1);

	}

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

	SystemCoreClockUpdate();

	/*enables clock for EGPIO module*/
	//RSI_CLK_PeripheralClkEnable(M4CLK,EGPIO_CLK,ENABLE_STATIC_CLK);

	/*configures default GPIO mode(0) */
	RSI_EGPIO_SetPinMux(EGPIO,M4_GPIO_PORT ,M4_PIN1,0);
	RSI_EGPIO_SetPinMux(EGPIO,M4_GPIO_PORT ,M4_PIN2,0);

	/*Enables group interrupt one*/
	RSI_EGPIO_GroupIntEnable(EGPIO ,GROUPINT_1);

	/*enables group interrupt for both the pins*/
	RSI_EGPIO_GroupIntOneEnable(EGPIO,M4_GPIO_PORT ,M4_PIN1);
	RSI_EGPIO_GroupIntOneEnable(EGPIO,M4_GPIO_PORT ,M4_PIN2);

	/*Configures group interrupt for edge with or event*/
	RSI_EGPIO_GroupIntEdge(EGPIO ,GROUPINT_1);
	RSI_EGPIO_GroupIntOr(EGPIO ,GROUPINT_1);

	/*configures group interrupt polarity high for both the pins*/
	RSI_EGPIO_SetGroupIntOnePol(EGPIO,M4_GPIO_PORT,M4_PIN1,POL_HIGH);
	RSI_EGPIO_SetGroupIntOnePol(EGPIO,M4_GPIO_PORT,M4_PIN2,POL_HIGH);

	/*enables wake up interrupt for group interrupt one*/
	RSI_EGPIO_GroupIntWkeUpEnable(EGPIO , GROUPINT_1);

	/* unmask the group interrupt */
	RSI_EGPIO_GroupIntUnMask(EGPIO ,GROUPINT_1);

	/* Enable interrupt in the NVIC */
	NVIC_EnableIRQ(WAKEUP_NVIC_NAME);

	while (forever) {
		/*Sleep till interrupt occurs*/
		DEBUGOUT("Core is Sleeping...\n");
		__WFI();
	}
	/*Statement will never reach here , just to satisfy the standard main*/
	return 0;
}
