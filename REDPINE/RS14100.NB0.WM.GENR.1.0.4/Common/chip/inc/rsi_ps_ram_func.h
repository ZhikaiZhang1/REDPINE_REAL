/**
 * @file       rsi_ps_ram_func.c
 * @version    0.9
 * @date       20 Dec 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contains power save functions which are exepected to be RAM.
 *
 * @section Description
 * 
 *
 */


/**
 * Includes
 */

/*This API will be called by the boot loader when it is from non retention wake up
when flash execution is enabled */
void RSI_PS_Restore(void) ;
void Init_QspiStc(void)   ;
