/**
 * @file       timer_led.c
 * @version    1.0
 * @date       27 jun 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example describing usage of TIMER APIs.
 *
 * @section Description
 * TIMERS ROM driver LED blinking example example. 
 */
 

/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define   TIMER0_IRQHandler	 IRQ002_Handler
#define   TIMER1_IRQHandler	 IRQ003_Handler
#define   TIMER2_IRQHandler	 IRQ004_Handler
#define   TIMER3_IRQHandler	 IRQ005_Handler
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
 void RSI_Board_LED_Toggle(int x);
 void Timer0Config(void);
 void Timer1Config(void);
 void RSI_Board_Init(void);
 void RSI_Board_LED_Set(int x, int y);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  TIMER0 Interrupt Handler
 * @param  None
 * @return None
 */
void TIMER0_IRQHandler()
{ 
	/* Clears interrupt */
	RSI_TIMERS_InterruptClear(TIMERS, TIMER_0);
  RSI_Board_LED_Toggle(1);
}

/**
 * @brief  TIMER1 Interrupt Handler
 * @param  None
 * @return None
 */
void TIMER1_IRQHandler()
{
	/* Clears interrupt */
	RSI_TIMERS_InterruptClear(TIMERS, TIMER_1);
  RSI_Board_LED_Toggle(2); 
}

/**
 * @brief  TIMER0 Configurations
 * @param  None
 * @return None
 */
void Timer0Config()
{
  /* Sets periodic mode */ 
  RSI_TIMERS_SetTimerMode(TIMERS, PERIODIC_TIMER, TIMER_0);
  
  /* Sets timer in 1 Micro second mode */
  RSI_TIMERS_SetTimerType(TIMERS, MICRO_SEC_MODE, TIMER_0);
  
  /* 1 Micro second timer configuration */ 
	RSI_TIMERS_MicroSecTimerConfig(TIMERS, TIMER_0, 32, 0 ,MICRO_SEC_MODE);
  
  RSI_TIMERS_SetMatch(TIMERS, TIMER_0,1000000);
  
  /* Enables timer interrupt */
	RSI_TIMERS_InterruptEnable(TIMERS , TIMER_0);
}

/**
 * @brief  TIMER1 Configurations
 * @param  None
 * @return None
 */
void Timer1Config()
{
  /* Sets periodic mode */ 
  RSI_TIMERS_SetTimerMode(TIMERS, PERIODIC_TIMER, TIMER_1);
  
  /* Sets timer in 1 Micro second mode */
  RSI_TIMERS_SetTimerType(TIMERS, MICRO_SEC_MODE, TIMER_1);
  
  /* 1 Micro second timer configuration */ 
	RSI_TIMERS_MicroSecTimerConfig(TIMERS, TIMER_1, 32, 0 ,MICRO_SEC_MODE);
  
  RSI_TIMERS_SetMatch(TIMERS, TIMER_1,5000000);
  
  /* Enables timer interrupt */
	RSI_TIMERS_InterruptEnable(TIMERS , TIMER_1);
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main()
{
  /* At this stage the MICROCONTROLLER clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs9116.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs9116.c file
	 */
  	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
  /* Timer clock config */
  RSI_ULPSS_TimerClkConfig( ULPCLK ,ENABLE_STATIC_CLK,0,ULP_TIMER_32MHZ_RC_CLK,0);
  
  /* Timer 0 Configuration */
  Timer0Config();
  /* Timer 1 Configuration */
  Timer1Config();
  
  /* GPIO Configuration */
  RSI_Board_Init();
  
	/*  Sets the state of a board '0'number LED to ON. */ 
	RSI_Board_LED_Set(0, 1);
 
  /* Interrupt map to ARM */
  NVIC_EnableIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);
  
	/* Timers start */
	RSI_TIMERS_TimerStart(TIMERS, TIMER_0);
	RSI_TIMERS_TimerStart(TIMERS, TIMER_1);
  
	/* Code never reaches here. Only used to satisfy standard main() */
  while(1);
}


