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
#define  MCPWM_RATE    10000
#define  TICKS         1000		/* 1 ms Tick rate */
/* Private define ------------------------------------------------------------*/
#define    MCPWM_IRQHandler     IRQ048_Handler
         
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
 
/* Private functions ---------------------------------------------------------*/

void SysTick_Handler(void)
{
	                 
}	
/**
 * @brief	 PWM pin muxing 
 * @return	Nothing
 */
void RSI_MCPWM_PinMux()
{
  RSI_EGPIO_PadReceiverEnable(19);
	RSI_EGPIO_PadSelectionEnable(6);
	/* 1L */
	RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , 19 ,EGPIO_PIN_MUX_MODE9);
	/* 1H */
  RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , GPIO9 ,EGPIO_PIN_MUX_MODE8);
	
	/* 2H */
  RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT2 , GPIO13 ,EGPIO_PIN_MUX_MODE8);
  
  RSI_EGPIO_PadReceiverEnable(11);
	RSI_EGPIO_PadSelectionEnable(2);	
	/* 2L */
  RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , GPIO11 ,EGPIO_PIN_MUX_MODE8);
}

/**
 * @brief	 PWM Channel time period config 
 * @param  freq  PWM clock frequency
 * @return none
 */
void MCPWM_SetChannelPeriod(uint32_t freq)
{
  uint32_t rate = 0;
  
  rate = SystemCoreClock/freq;
	
	/* Set time period */
  RSI_MCPWM_SetTimePeriod( MCPWM,PWM_CHNL_0 ,rate ,COUNTER_INIT_VAL); 
	RSI_MCPWM_SetTimePeriod( MCPWM,PWM_CHNL_1 ,rate ,COUNTER_INIT_VAL); 
}

/**
 * @brief	 Converts duty cycle percentage to system ticks
 * @param  percent  Duty cycle in percentage
 * @param  chnl_num PWM channel number (0...3)
 * @return Returns number of ticks
 */
uint32_t MCPWM_PercentageToTicks(uint8_t percent,uint8_t chnl_num)
{
	uint16_t time_period = 0;
	uint32_t ticks =0;
	
	/* Get time period */
	RSI_MCPWM_GetTimePeriod(MCPWM,chnl_num,&time_period);
  ticks = (time_period* percent) / 100;
	return ticks;
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
  int loop = 1;	
	uint32_t delay = 0;
	int pwm_out_0, pwm_out_1, duty_p = 0, incr = 1, Led = 0;
  uint16_t time_period = 0; 
	uint32_t ticks =0;
  
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
  /* Sets pin mux */
	RSI_MCPWM_PinMux();
  
  /* Board LED config */
  RSI_Board_Init();
	
  /* Using these GPIOs for LED */
  /* Blue and Green LEDs are using*/
	RSI_Board_LED_Set(1, 1);
  
	/* MCPWM clock enable */
	RSI_CLK_PeripheralClkEnable( M4CLK, PWM_CLK, ENABLE_STATIC_CLK );
  
	/* select number of base timers as four base timers for four channels MCPWM*/
	RSI_MCPWM_BaseTimerSelect(MCPWM, ONE_TIMER_FOR_EACH_CHNL);
	
	/* Set output polarity for MCPWM */
	RSI_MCPWM_SetOutputPolarity(MCPWM,1 ,1);
 
	/* Set PWM output mode */
  RSI_MCPWM_SetBaseTimerMode(MCPWM, TMR_FREE_RUN_MODE, PWM_CHNL_0);
	RSI_MCPWM_SetBaseTimerMode(MCPWM, TMR_FREE_RUN_MODE, PWM_CHNL_1);

 /* configure PWM Channel time period  */
  MCPWM_SetChannelPeriod(MCPWM_RATE);
  
	/* Get time period for required channel. */
	RSI_MCPWM_GetTimePeriod(MCPWM,PWM_CHNL_1,&time_period);
	ticks = time_period/2;
	
  /* Set Duty cycle value for channel 0 and channel 1*/	
  RSI_MCPWM_SetDutyCycle(MCPWM, 0 ,PWM_CHNL_0);       /* LED */
 	RSI_MCPWM_SetDutyCycle(MCPWM, ticks, PWM_CHNL_1);   /* OUT */
	
	/* Start PWM */
  RSI_MCPWM_Start( MCPWM , PWM_CHNL_0);
	RSI_MCPWM_Start( MCPWM , PWM_CHNL_1);
  
	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / TICKS);

  while (loop) 
  {
		delay++;
		if (delay >= 20) 
    {
			duty_p = duty_p + incr;
			if (duty_p < 0) 
      {
				duty_p = 0;
				incr = 1;
			}
			if (duty_p > 100) 
      {
				duty_p = 100;
				incr = -1;
				Led = 1 - Led;
			}
			if (Led) 
      {
				pwm_out_0 = duty_p;
				pwm_out_1 = 100;
			}
			else 
      {
				pwm_out_0 = 100;
				pwm_out_1 = duty_p;
			}

			if(pwm_out_0 == 100)
			{
				/* Enable the output override operation of MCPWM */
				RSI_MCPWM_OutputOverrideEnable(MCPWM, PWM_OUTPUT_L0);
				
				/* Set override value for the required output of MCPWM. */
				RSI_MCPWM_OverrideValueSet(MCPWM, PWM_OUTPUT_L0, 1);     
      }
			else
			{
				/* Disable the output override operation of MCPWM */
				RSI_MCPWM_OutputOverrideDisable(MCPWM, PWM_OUTPUT_L0);
				/* Set duty cycle */
			  RSI_MCPWM_SetDutyCycle(MCPWM, MCPWM_PercentageToTicks(pwm_out_0,PWM_CHNL_0),PWM_CHNL_0);  /* LED */
			}
			
			if(pwm_out_1 == 100)
      {
				/* Enable the output override operation of MCPWM */
				RSI_MCPWM_OutputOverrideEnable(MCPWM, PWM_OUTPUT_L1);
				
				/* Set override value for the required output of MCPWM. */
				RSI_MCPWM_OverrideValueSet(MCPWM, PWM_OUTPUT_L1, 1);           
      }	
      else
      {
				/* Disable the output override operation of MCPWM */
        RSI_MCPWM_OutputOverrideDisable(MCPWM, PWM_OUTPUT_L1);
				/* Set duty cycle */
			  RSI_MCPWM_SetDutyCycle(MCPWM, MCPWM_PercentageToTicks(pwm_out_1,PWM_CHNL_1),PWM_CHNL_1);  /* LED */
      }				
			delay = 0;
		}
		__WFI();
  } 
}


