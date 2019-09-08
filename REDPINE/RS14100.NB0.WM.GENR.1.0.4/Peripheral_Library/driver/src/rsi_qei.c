/**
 * @file       rsi_qei.c
 * @version    1.0
 * @date       11 AUG 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions related to QEI
 * 
 * @section Description
 * This file contains the list of functions for configuring the QEI.
 * Following are list of API's which need to be defined in this file.
 *
 */



/* Includes */
#include "rsi_chip.h"

/**
 * @fn          void RSI_QEI_Enable(volatile QEI_Type  *pstcQei)        
 * @brief	Enables the clock to the QEI module
 * @param[in]   pstcQei : Pointer to the QEI register instance
 * @return 	none
 */
void RSI_QEI_Enable(volatile QEI_Type  *pstcQei)
{
	/* QEI clock enable routine here */
	clk_peripheral_clk_enable2(M4CLK,QE_PCLK_ENABLE);
	return ;
}

/**
 * @fn          void RSI_QEI_Disable(volatile QEI_Type  *pstcQei)
 * @brief	Disables the clock to the QEI module
 * @param[in]   pstcQei : Pointer to the QEI register instance
 * @return 	none
 */
void RSI_QEI_Disable(volatile QEI_Type  *pstcQei)
{
	/* clock disable to QEI module */
	clk_peripheral_clk_disable2(M4CLK,QE_PCLK_ENABLE);
	return ;
}


/*End of file */
