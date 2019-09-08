/**
 * @file       blinky.c
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
 * @brief This files contains freeRTOS example code of all end to end functionality
 *
 * @section Description
 * This files contains example code to demonstrate the FreeRTOS toggle functionality.
 *
 *
 */
/*Includes*/

#include "rsi_board.h"
#include "FreeRTOS.h"
#include "task.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief	Configures the hardware related
 * @return	Nothing
 */
static void prvSetupHardware(void)
{
	RSI_Board_Init();
	/* Initial LED0 state is off */
	RSI_Board_LED_Set(0, false);
}

/**
 * @brief	Thread 1 to toggle the LED0
 * @return	Nothing
 */
static void vLEDTask1(void *pvParameters) {
	bool LedState = false;
	while (1) {
		RSI_Board_LED_Set(0, LedState);
		LedState = (bool) !LedState;

		/* About a 3Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ / 6);
	}
}

/**
 * @brief	Thread 2 to toggle the LED1
 * @return	Nothing
 */
static void vLEDTask2(void *pvParameters) {
	bool LedState = false;

	while (1) {
		RSI_Board_LED_Set(1, LedState);
		LedState = (bool) !LedState;

		/* About a 7Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ / 14);
	}
}

/**
 * @brief	Thread 3 to prints the tick counts on UART debug port
 * @return	Nothing
 */
static void vUARTTask(void *pvParameters) {
	int tickCnt = 0;

	while (1) {
		tickCnt++;

		/* About a 1s delay here */
		vTaskDelay(configTICK_RATE_HZ);
	}
}

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */

int main(void)
{
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/* Configure the hardware */
	prvSetupHardware(); 
	
	/* LED1 toggle thread*/
	xTaskCreate(vLEDTask1, "vTaskLed1",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);         
	
	/* LED2 toggle thread */
	xTaskCreate(vLEDTask2, "vTaskLed2",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);        
	
	/* UART output thread, simply counts seconds */
	xTaskCreate(vUARTTask, "vTaskUart",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);  
	
  /* Start the scheduler */
	vTaskStartScheduler(); 

	/* Should never arrive here */
	return 1;
}
