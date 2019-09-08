/**
 * @file       rsi_bod_example.c
 * @version    0.9
 * @date       27 Jun 2017
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
 *
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_bod.h"
#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_ps_ram_func.h"
#include "rsi_ds_timer.h"

/* Uncommenent one of the below to select the mode */ 
/* Note:one of that should be in commnet  */
#define  AUTOMATIC
//#define MANUAL 
/* Mode selection */
#define PMU_GOOD_TIME        31    /*Duration in us*/
#define XTAL_GOOD_TIME       31    /*Duration in us*/
#define  MANUAL_MODE         0
#define AUTOMATIC_MODE       1
#define ENABLE               1
#define DISABLE              0
#define BOD_THRESHOLD        3.35
#define SLOT_VALUE           2
#define NVIC_BOD             NPSS_TO_MCU_BOD_INTR_IRQn
#define NVIC_BUTTON          NPSS_TO_MCU_BUTTON_INTR_IRQn
#define  NPSS_TO_MCU_BUTTON_IRQHandler   IRQ024_Handler 
#define  NPSS_TO_MCU_BOD_IRQHandler      IRQ023_Handler 


volatile float vbatt_status =0;

void NPSS_TO_MCU_BOD_IRQHandler(void)
{	
	DEBUGOUT("Your Vbatt status is less than the threshold voltage i.e%fV \n",RSI_BOD_SoftTriggerGetBatteryStatus());
		  RSI_BOD_IntrClr();
}


int main(void)
{
	volatile static uint32_t t_value =0,button_number=0 ;
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	#ifdef DEBUG_UART
	/*Init debug UART*/
	 DEBUGINIT();
#endif
#ifdef AUTOMATIC
	/*BOD enable and set the threshold value */
	RSI_BOD_Enable(ENABLE,BOD_THRESHOLD) ;
	
	/* set the slot value */
	RSI_BOD_ConfigSlotValue(SLOT_VALUE) ;
	
	/*set the BOD to automatic  mode*/
  RSI_BOD_SetMode(AUTOMATIC_MODE) ;
	/*enable bod interrupt */
	RSI_BOD_IntrEnable(ENABLE) ;

	RSI_BOD_BodTestSel(1,3);
	/*Gets the threshold value */
	
	/* BOD NVIC enable  */
	NVIC_EnableIRQ(NVIC_BOD);

  t_value = RSI_BOD_GetThreshold();
	 
#endif 

#ifdef MANUAL 
   RSI_BOD_BodTestSel(1,3);
   /*BOD enable and set the threshold value */
   RSI_BOD_Enable(ENABLE,BOD_THRESHOLD) ;
   /*set the BOD to automatic  mode*/
   RSI_BOD_SetMode(MANUAL_MODE) ;
	 	/*enable bod interrupt */
   RSI_BOD_IntrEnable(ENABLE) ;
	 /* BOD NVIC enable  */
   NVIC_EnableIRQ(NVIC_BOD);

#endif



	while(1){

__WFI();
		 
	}
}  
