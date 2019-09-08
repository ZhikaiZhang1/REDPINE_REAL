/**
 * @file       rsi_matrix_example.c
 * @version    1.0
 * @date       1 Aug 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains FIM example code which demonstrates simple matrix multiplication
 *
 * @section Description
 *   Example code demonstrating matrix multiplication.
 *
 *
 */
/*Includes*/

#include <stdio.h>
#include <string.h>
#include "rsi_chip.h"



/* Private define ------------------------------------------------------------*/
#define NUM_ROW1                      4
#define NUM_COL1                      1
#define NUM_ROW2                      1
#define NUM_COL2                      4

/* Private variables ---------------------------------------------------------*/
q31_t a2[NUM_ROW1 * NUM_COL1] = {0X7FFFFFFF, 0X7FFFFFFF,  0X7FFFFFFF, 0X7FFFFFFF};
q31_t b2[NUM_ROW2 * NUM_COL2] = {0X7FFFFFFF, 0X7FFFFFFF, 0X7FFFFFFF, 0X7FFFFFFF};


q31_t c2[NUM_ROW1*NUM_COL2];
																		
/* Private typedef -----------------------------------------------------------*/
arm_matrix_instance_q31_opt x31 = { NUM_ROW1, NUM_COL1, a2};          /* Matrix Input1 Instance */
arm_matrix_instance_q31_opt y31 = { NUM_ROW2, NUM_COL2, b2};          /* Matrix Input2 Instance */
arm_matrix_instance_q31_opt z31 = { NUM_ROW1, NUM_COL2, c2};          /* Matrix Output Instance */

/* Private macro -------------------------------------------------------------*/



																		
																		
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
volatile uint8_t stat=0,ret;
void IRQ017_Handler()
{	
	fim_interrupt_handler(FIM);	
	stat = 1;
}
/**
 * @brief	Application entry function for FIM matrix multiplication example
 * @return	Nothing
 */
 
 int main(void)
 {
	      uint32_t matrix_len;
	 	    /*Configures the system default clock and power configurations*/
 	       SystemCoreClockUpdate();
	      /* Enables a FIM interrupt in the NVIC interrupt controller.*/
	      NVIC_EnableIRQ(FIM_IRQn);
	      
	      /*Fim clock enable */
	      RSI_ULPSS_PeripheralEnable(ULPCLK,ULP_FIM_CLK,ENABLE_STATIC_CLK);
	      
	      /* Set the FIM Matrix Multiplication for real numbers */
	      ret = arm_mat_mult_q31_opt(&x31,&y31,&z31);
	      if(ret == RSI_OK){
 	      /*No error in API*/
	      /*Wait for the multiplication done*/
	      while(stat != 1);
				 stat = 0;
	      /*Read the destination */
		    matrix_len = (NUM_ROW1*NUM_COL2);
					/* Set the FIM to read the output */
	      fim_read_data(BANK2,matrix_len,c2,FORMAT_Q31,ULP_FIM_COP_DATA_REAL_REAL);
        }else{
		    /*check the input parameters*/
		    while(1);	 
        }
				
	 while(1);
				
}
 

 