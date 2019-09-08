/**
 * @file       ct_pwm.c
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
 * @brief This files contains example describing usage of Configurable Timer APIs.
 *
 * @section Description
 * CT ROM driver LED fade in fade out example. 
 */
 

/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define    CT_RATE       10000
#define    TICKS         1000		/* 1 ms Tick rate */
/* Private define ------------------------------------------------------------*/
#define    CT_IRQHandler         IRQ034_Handler		
/* Private variables ---------------------------------------------------------*/
uint8_t isr_check = 0;															
OCU_PARAMS_T vsOCUparams;
RSI_CT_CALLBACK_T vsCB;
/* Private function prototypes -----------------------------------------------*/
 
/* Private functions ---------------------------------------------------------*/
void SysTick_Handler(void)
{
	                 
}	
/**
 * @brief	 CT pin muxing 
 * @return	Nothing
 */
 
 
void RSI_CT_PinMux()
{
  RSI_EGPIO_PadReceiverEnable(50);
	RSI_EGPIO_PadReceiverEnable(7);
  RSI_EGPIO_PadSelectionEnable(12);	
  /* CT_OUT0 GPIO */	
	RSI_EGPIO_SetPinMux(EGPIO ,3 , GPIO2 ,EGPIO_PIN_MUX_MODE6);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , GPIO7 ,EGPIO_PIN_MUX_MODE6);
}

/*	 CT OCU pulse width con-fig */
uint32_t CT_SetPeriod(uint32_t freq)
{
  uint32_t rate = 0;
  
  rate = SystemCoreClock/freq;
	/* set the Peak Match value for CT0 coutner0 & counter 1 */
	RSI_CT_SetMatchCount(CT0, rate,0,0);
  RSI_CT_SetMatchCount(CT0, rate,0,1);    
	
	return rate;
}


/* Converts duty cycle percentage to system ticks */
uint32_t CT_PercentageToTicks(uint8_t percent,uint32_t freq)
{
	uint16_t ticks = 0;
  uint32_t rate = 0;
  
  rate = SystemCoreClock/freq;
  ticks = (rate * percent) / 100;
	return ticks;
}


/* Main program. */
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
  volatile uint32_t time_period = 0; 
	uint32_t ticks =0;

	OCU_PARAMS_T vsOCUparams;
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
  /*	Enable the DEBUG UART port for debug prints and  Set up and initialize all required 
        blocks and functions  related to the board hardware.  */
  RSI_Board_Init();
	
  /* ULPGPIO_0 and ULPGPIO_2 are using*/
	/*  Sets the state of a board '2'number LED to ON. */ 
  RSI_Board_LED_Set(2, 1);
  
	/* enables pad selection bits,enables receive_enable for the CT pins and 
   configures the pins for CT.(enable gpio muxing )   */
	RSI_CT_PinMux();																																						
	
	/*	Initialization and Configure the CT clocks */
	RSI_CLK_CtClkConfig(M4CLK, M4_SOCCLKFOROTHERCLKSCT ,1, ENABLE_STATIC_CLK );

	  /* set the 32bit coutner*/
	RSI_CT_Config(CT0,0);																										
  	
  /* Sets control for counter0 and counter 1 */
  RSI_CT_SetControl( CT0, PERIODIC_EN_COUNTER_0_FRM_REG| 
                          PERIODIC_EN_COUNTER_1_FRM_REG );
    /* CT OCU pulse width config */
  ticks = CT_SetPeriod(CT_RATE);
  
  /* Set Duty cycle value for channel 0 and channel 1*/	
  RSI_MCPWM_SetDutyCycle(MCPWM, 0 ,PWM_CHNL_0);       /* LED */
  RSI_MCPWM_SetDutyCycle(MCPWM, ticks, PWM_CHNL_1);   /* OUT */

  vsOCUparams.CompareVal1_0 = 0x0; 
  vsOCUparams.CompareVal1_1 = ticks/2; 

  /* OCU configuration on Counter0 & 1 of CT0 instance */
  RSI_CT_OCUConfigSet(CT0,OUTPUT_OCU_0 | OUTPUT_OCU_1 | 
                          MAKE_OUTPUT_0_HIGH_SEL_0 |
                          MAKE_OUTPUT_0_LOW_SEL_0	 | 
                          MAKE_OUTPUT_1_HIGH_SEL_1 | 
                          MAKE_OUTPUT_1_LOW_SEL_1 |
                          OCU_SYNC_WITH_0 |OCU_SYNC_WITH_1);	

  /* OCU counter 0 compare values and compare next values */
  RSI_CT_OCUControl(CT0,COUNTER_0,0, &vsOCUparams, &vsCB);

  /* OCU counter 1 compare values and compare next values */
  RSI_CT_OCUControl(CT0,COUNTER_1,0, &vsOCUparams, &vsCB);
   
	 /*  Programs General control register bit fields */
  RSI_CT_SetControl(CT0,COUNTER_0_UP | COUNTER_1_UP|
                        COUNTER0_SYNC_TRIG |COUNTER1_SYNC_TRIG|
                        COUNTER0_TRIG|COUNTER1_TRIG);
	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / TICKS);

  while (loop) 
  {
		delay++;
		if(delay >= 20) 
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
       /* Converts duty cycle percentage to system ticks */  
       vsOCUparams.CompareVal1_0 = CT_PercentageToTicks(pwm_out_0, CT_RATE); 
       vsOCUparams.CompareVal1_1 = CT_PercentageToTicks(pwm_out_1, CT_RATE);     
			 
			/* Set duty cycle */
			 RSI_CT_OCUControl(CT0,COUNTER_0,0, &vsOCUparams, &vsCB);  /* LED */
			 RSI_CT_OCUControl(CT0,COUNTER_1,0, &vsOCUparams, &vsCB);  /* LED */				
			 delay = 0;
		}
		__WFI();
  } 
}

