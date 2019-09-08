/**
 * @file       rsi_ir.c
 * @version    1.0
 * @date       1 AUG 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief IR peripheral function definition .
 *
 * @section Description
 * This file contains the driver code for IR peripheral function definition.
 *
 */


/**
 * Includes
 */

#include "rsi_chip.h"
#include "rsi_ir.h"


/**
 * @fn           uint16_t RSI_IR_ReadData(IR_Type* pIr,uint16_t memory_address)
 * @brief        This API is used read IR address.
 * @param[in]    pIr  : IR type pointer
 * @param[in]    memory_address      : memory address value (0 .. 128).
 * @return       16-Bit IR data received (BIT[15] in received data will indicate the polarity of pulse)
 *               \n remaining bit will contain the incremented counter value of the pulse.
 * @par          Example
                 ir_data = RSI_IR_ReadData(IR,0); 
 */
uint16_t RSI_IR_ReadData(IR_Type* pIr,uint16_t memory_address)
{
	if(memory_address > MAX_MEMORY_ADDRESS)
	{
		 return INVALID_PARAMETERS ;
	}		
        pIr->IR_MEM_ADDR_ACCESS_b.IR_MEM_ADDR  = memory_address;
        pIr->IR_MEM_ADDR_ACCESS_b.IR_MEM_RD_EN = 1U;
	return pIr->IR_MEM_READ_b.IR_MEM_DATA_OUT;  
}


/**
 * @fn           void RSI_IR_SoftwareRestart(IR_Type* pIr)
 * @brief        This API restart the IR operation. 
 * @param[in]    pIr  : IR type pointer
 */
 void RSI_IR_SoftwareRestart(IR_Type* pIr)
{
	uint8_t i;
	pIr->IR_CONFIG_b.SREST_IR_CORE = 1U;
	for(i=0;i<10;i++)
	{
	  __ASM("nop");
	}	
	pIr->IR_CONFIG_b.SREST_IR_CORE = 0U;
}