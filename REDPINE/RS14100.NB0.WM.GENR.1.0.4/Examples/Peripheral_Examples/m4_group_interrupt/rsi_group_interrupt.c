/**
 * @file       rsi_group_interrupt.c
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
 * This files contains example code to demonstrate the GPIO GROUP INTERRUPTS functionality.
 *
 *
 */
/* Includes -------------------------------------------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"

/* Private typedef ------------------------------------------------------------------------------------------------*/

/* Private define -------------------------------------------------------------------------------------------------*/

/* Private macro --------------------------------------------------------------------------------------------------*/

#define GROUP_INTERRUPT            1                               /*group interrupt number(1 or 2) ,1 is for Group
                                                                    interrupt one and 2 is for group interrupt two */
#if   (GROUP_INTERRUPT == 1)
#define GROUPINT_1_IRQ_HANDLER     IRQ050_Handler                  /* GPIO group interrupt IRQ function name       */
#define GROUPINT_NVIC_NAME         EGPIO_GROUP_0_IRQn              /* GPIO group interrupt NVIC interrupt name     */
#elif (GROUP_INTERRUPT == 2)
#define GROUPINT_2_IRQ_HANDLER     IRQ051_Handler                  /* GPIO group interrupt IRQ function name       */
#define GROUPINT_NVIC_NAME         EGPIO_GROUP_1_IRQn              /* GPIO group interrupt NVIC interrupt name     */
#endif

#define GROUPINT_1                 0                               /* Group interrupt one                          */
#define GROUPINT_2                 1                               /* Group interrupt two                          */

#define POL_HIGH                   1                               /* polarity high                                */
#define POL_LOW                    0                               /* polarity low                                 */

#define M4_GPIO_PORT               0                               /* GPIO port number                             */
#define M4_PIN1                    6                               /* GPIO pin number                              */
#define M4_PIN2                    7                               /* GPIO pin number                              */
/* Private variables ----------------------------------------------------------------------------------------------*/


/* Private function prototypes ------------------------------------------------------------------------------------*/

/* Private functions ----------------------------------------------------------------------------------------------*/

/**
 * @brief	Interrupt handler
 * @return	Nothing
 */
#if   (GROUP_INTERRUPT == 1)
void GROUPINT_1_IRQ_HANDLER(void)
{
	uint32_t gintStatus;
	/*read group interrupt status*/
	gintStatus=RSI_EGPIO_GroupIntStat(EGPIO, GROUPINT_1);
	if(gintStatus &INTERRUPT_STATUS_CLR ){
		/*clears interrupt*/
		RSI_EGPIO_GroupIntClr(EGPIO, GROUPINT_1 ,INTERRUPT_STATUS_CLR);
	}
	return ;
}

#elif (GROUP_INTERRUPT == 2)
void GROUPINT_2_IRQ_HANDLER(void)
{
	uint32_t gintStatus;
	/*read group interrupt status*/
	gintStatus=RSI_EGPIO_GroupIntStat(EGPIO, GROUPINT_1);
	if(gintStatus & INTERRUPT_STATUS_CLR){
		/*clears interrupt*/
		RSI_EGPIO_GroupIntClr(EGPIO, GROUPINT_2 ,INTERRUPT_STATUS_CLR  );
	}
	return ;
}
#endif

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
	RSI_CLK_PeripheralClkEnable(M4CLK,EGPIO_CLK,ENABLE_STATIC_CLK);

  /*PAD selection enable for gpio 6 and 7*/
  RSI_EGPIO_PadSelectionEnable(1);
  
  /*Receiver enables for ipnut pins*/
  RSI_EGPIO_PadReceiverEnable(M4_PIN1);
  RSI_EGPIO_PadReceiverEnable(M4_PIN2);
  
	/*configures default GPIO mode(0) */
	RSI_EGPIO_SetPinMux(EGPIO,M4_GPIO_PORT ,M4_PIN1,EGPIO_PIN_MUX_MODE0);
	RSI_EGPIO_SetPinMux(EGPIO,M4_GPIO_PORT ,M4_PIN2,EGPIO_PIN_MUX_MODE0);
  
  /*Configure input direction for both the pins*/
  RSI_EGPIO_SetDir(EGPIO,M4_GPIO_PORT ,M4_PIN1,EGPIO_CONFIG_DIR_INPUT);  
  RSI_EGPIO_SetDir(EGPIO,M4_GPIO_PORT ,M4_PIN2,EGPIO_CONFIG_DIR_INPUT);
  
#if (GROUP_INTERRUPT == 1)

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

	/*unmask group interrupt*/
	RSI_EGPIO_GroupIntUnMask(EGPIO ,GROUPINT_1);

	/*NVIC enable*/
	NVIC_EnableIRQ(GROUPINT_NVIC_NAME);
#else
	/*Enables group interrupt one*/
	RSI_EGPIO_GroupIntEnable(EGPIO ,GROUPINT_2);

	/*enables group interrupt for both the pins*/
	RSI_EGPIO_GroupIntTwoEnable(EGPIO,M4_GPIO_PORT ,M4_PIN1);
	RSI_EGPIO_GroupIntTwoEnable(EGPIO,M4_GPIO_PORT ,M4_PIN2);

	/*Configures group interrupt for edge with or event*/
	RSI_EGPIO_GroupIntEdge(EGPIO ,GROUPINT_2);
	RSI_EGPIO_GroupIntOr(EGPIO ,GROUPINT_2);

	/*configures group interrupt polarity high for both the pins*/
	RSI_EGPIO_SetGroupIntTwoPol(EGPIO,M4_GPIO_PORT,M4_PIN1,POL_HIGH);
	RSI_EGPIO_SetGroupIntTwoPol(EGPIO,M4_GPIO_PORT,M4_PIN2,POL_HIGH);

	/*unmask group interrupt*/
	RSI_EGPIO_GroupIntUnMask(EGPIO ,GROUPINT_2);

	/*NVIC enable*/
	NVIC_EnableIRQ(GROUPINT_NVIC_NAME);
#endif
	while (forever) 
  {
		
	}
	/*Statement will never reach here , just to satisfy the standard main*/
	return 0;
}
