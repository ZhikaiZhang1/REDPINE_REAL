/**
 * @file       rsi_ulp_group_interrupt.c
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
 * This files contains example code to demonstrate the ULP GPIO GROUP INTERRUPT functionality.
 *
 *
 */
/* Includes --------------------------------------------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"

/* Private typedef -------------------------------------------------------------------------------------------------*/

/* Private define --------------------------------------------------------------------------------------------------*/

/* Private macro ---------------------------------------------------------------------------------------------------*/
#define ULP_GROUPINT_IRQ_HANDLER    IRQ019_Handler                   /* GPIO interrupt IRQ function name            */
#define ULP_GROUPINT_NVIC_NAME      ULP_EGPIO_GROUP_IRQn             /* GPIO interrupt NVIC interrupt name          */

#define GROUPINT                    0                                /* group interrupt one                         */

#define POL_HIGH                    1                                /* polarity high                               */
#define POL_LOW                     0                                /* polarity low                                */

#define PORT                        0                                /* GPIO port number                            */
#define ULP_PIN1                    0                                /* GPIO pin number                             */
#define ULP_PIN2                    1                                /* GPIO pin number                             */

/* Private variables -----------------------------------------------------------------------------------------------*/

/* Private function prototypes -------------------------------------------------------------------------------------*/

/* Private functions -----------------------------------------------------------------------------------------------*/



/**
 * @brief	Interrupt handler
 * @return	Nothing
 */
void ULP_GROUPINT_IRQ_HANDLER(void)
{

	uint32_t gintStatus;

	/*reads interrupt status*/
	gintStatus=RSI_EGPIO_GroupIntStat(EGPIO1, GROUPINT);

	if(gintStatus &INTERRUPT_STATUS_CLR ){
		/*clears group interrupt*/
		RSI_EGPIO_GroupIntClr(EGPIO1, GROUPINT ,INTERRUPT_STATUS_CLR);
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
	RSI_ULPSS_PeripheralEnable(ULPCLK,ULP_EGPIO_CLK,ENABLE_STATIC_CLK);

  /*Receiver enables for ipnut pins*/
  RSI_EGPIO_UlpPadReceiverEnable(ULP_PIN1);
  RSI_EGPIO_UlpPadReceiverEnable(ULP_PIN2);
  
	/*configures default GPIO mode(0) */
	RSI_EGPIO_SetPinMux(EGPIO1,PORT ,ULP_PIN1,EGPIO_PIN_MUX_MODE0);
	RSI_EGPIO_SetPinMux(EGPIO1,PORT ,ULP_PIN2,EGPIO_PIN_MUX_MODE0);

  /*Configure input direction for both the pins*/
	RSI_EGPIO_SetPinMux(EGPIO1,PORT ,ULP_PIN1,EGPIO_CONFIG_DIR_INPUT);
	RSI_EGPIO_SetPinMux(EGPIO1,PORT ,ULP_PIN2,EGPIO_CONFIG_DIR_INPUT);   
  
	/*Enables group interrupt one*/
	RSI_EGPIO_GroupIntEnable(EGPIO1 ,GROUPINT);

	/*enables group interrupt for both the pins*/
	RSI_EGPIO_GroupIntOneEnable(EGPIO1,PORT ,ULP_PIN1);
	RSI_EGPIO_GroupIntOneEnable(EGPIO1,PORT ,ULP_PIN2);

	/*Configures group interrupt for edge with or event*/
	RSI_EGPIO_GroupIntEdge(EGPIO1 ,GROUPINT);
	RSI_EGPIO_GroupIntOr(EGPIO1 ,GROUPINT);

	/*configures group interrupt polarity high for both the pins*/
	RSI_EGPIO_SetGroupIntOnePol(EGPIO1,PORT,ULP_PIN1,POL_HIGH);
	RSI_EGPIO_SetGroupIntOnePol(EGPIO1,PORT,ULP_PIN2,POL_HIGH);

	/*unmask group interrupt*/
	RSI_EGPIO_GroupIntUnMask(EGPIO1 ,GROUPINT);

	/*NVIC enable*/
	NVIC_EnableIRQ(ULP_GROUPINT_NVIC_NAME);

	while (forever) 
  {
		
	}
	/*Statement will never reach here , just to satisfy the standard main*/
	return 0;
}
