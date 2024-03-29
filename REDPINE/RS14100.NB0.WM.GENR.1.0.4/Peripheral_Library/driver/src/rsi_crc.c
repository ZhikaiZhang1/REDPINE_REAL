
/**
 * @file       rsi_crc.c
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
 * @brief This files contains functions prototypes releated to CRC.
 * 
 * @section Description
 * This file contains the list of function prototypes for the CRC in different and configuring parameters
 * Following are list of API's which need to be defined in this file.
 *
 */


/* Includes */

#include "rsi_chip.h"

/* This API is used to clears the FIFO and settles all the state machines to their IDLE. */
void crc_set_gen_control( CRC_Type *pCRC)
{
	pCRC->CRC_GEN_CTRL_SET_REG = BIT(0);
}

/* This API is used to get the general status of the crc signals */
uint32_t crc_get_gen_status( CRC_Type *pCRC)
{
	return pCRC->CRC_GEN_STS ;
}

/* This API is used to load the polynomial value into the polynomial reg.*/
void crc_polynomial(CRC_Type *pCRC, RSI_CRC_PARAMS_T *pCRCParams )
{
	pCRC->CRC_POLYNOMIAL = pCRCParams->polynomial;
}

/* This API is used to load the width of the polynomail .Number of bits/width of the polynomial */
uint32_t crc_polynomial_width( CRC_Type *pCRC, RSI_CRC_PARAMS_T *pCRCParams )
{
	if(pCRCParams->polyWidth <= 32)
	{
		pCRC->CRC_POLYNOMIAL_CTRL_RESET = 0x1F;
		pCRC->CRC_POLYNOMIAL_CTRL_SET = pCRCParams->polyWidth & 0x1F;
	}
	else
	{
	  return ERROR_CRC_INVALID_ARG;
	}
	return RSI_OK;
}

/* This API is used to initialize the  LFSR Value. 
   When ever the LFSR need to be Initilized this has to be updated with the Init value.*/
void crc_lfsr_init( CRC_Type *pCRC, RSI_CRC_PARAMS_T *pCRCParams )
{
	pCRC ->CRC_LFSR_INIT_CTRL_SET = BIT(0) ;

	pCRC ->CRC_LFSR_INIT_VAL = pCRCParams ->lfsrVal ;

	pCRC ->CRC_LFSR_INIT_CTRL_SET = BIT(1) ;
}

/* This API is used for swapped init value. If this is set bit swapped */
uint32_t crc_use_swapped_init( CRC_Type *pCRC ,RSI_CRC_PARAMS_T *pCRCParams)
{
  if(pCRCParams->swapLfsr == 0 || pCRCParams->swapLfsr ==1)
  {	
    if (pCRCParams->swapLfsr)
    {
      pCRC->CRC_LFSR_INIT_CTRL_RESET = BIT(2);
      pCRC->CRC_LFSR_INIT_CTRL_SET = BIT(2);
    }
    else
    {
      pCRC->CRC_LFSR_INIT_CTRL_RESET = BIT(2);
    }
  }
  else
  {
    return ERROR_CRC_INVALID_ARG;
  }
  return RSI_OK;
}

/* This API is used to Set and control the data width types.*/
uint32_t crc_set_data_width_type( CRC_Type *pCRC, RSI_CRC_PARAMS_T *pCRCParams )
{
	if(pCRCParams->widthType <= 2)
	{
		switch(pCRCParams->widthType)
	  {
			case 0 : //Width From ULI
		  /* Set DIN_CTRL_RESET [din_width_from_reg] and DIN_CTR5L_RESET [din_width_from_cnt]
     			to get the engine into this ULI width mode*/

							pCRC->CRC_DIN_CTRL_RESET_REG =BIT(5);
							pCRC ->CRC_DIN_CTRL_RESET_REG =BIT(6);
			break;
			case 1: //Width from REG				
		  /* Set DIN_CTRL_SET [din_width_from_reg] and DIN_CTRL_RESET [din_width_from_cnt]
			     to get the engine into this Width from reg mode*/

						pCRC->CRC_DIN_CTRL_SET = BIT(5);
						pCRC->CRC_DIN_CTRL_RESET_REG = BIT(6);
		  /* In this mode data length also programmed in din_width reg.*/
						pCRC->CRC_DIN_CTRL_RESET_REG = 0x1F;
	        	pCRC->CRC_DIN_CTRL_SET =(pCRCParams->dinWidth  & 0x1F);
			break;
			case 2: //Width from CNT
		  /* Set DIN_CTRL_SET [din_width_from_cnt] and DIN_CTRL_RESET [din_width_from_reg]
			    to get the engine into this Width from cnt mode*/
						pCRC->CRC_DIN_CTRL_SET = BIT(6) ;
						pCRC->CRC_DIN_CTRL_RESET_REG = BIT(5) ;
		  /* Write the number of bytes that the packet/data has in total into DIN_NUM_BYTES register.*/
						pCRC->CRC_DIN_NUM_BYTES = pCRCParams->numBytes;
			break;
			default:
						pCRC ->CRC_DIN_CTRL_RESET_REG =BIT(6);
						pCRC->CRC_DIN_CTRL_RESET_REG =BIT(5);
			break;
		}
	}
	else
	{
		return ERROR_CRC_INVALID_ARG;
	}
	return RSI_OK;
}	

/*  This API is used to Set the FIFO Threshold Levels. */
uint32_t crc_set_fifo_thresholds( CRC_Type *pCRC, RSI_CRC_PARAMS_T *pCRCParams)
{
	if(pCRCParams->aempty <= 16)
	{
    /* Before Set the FIFO Thresholds Reset the Threshold levels.*/
    pCRC->CRC_DIN_CTRL_RESET_REG = 0xFF000000 ; 
    
    /* Write the Almost empty threshold value*/
    pCRC->CRC_DIN_CTRL_SET = ((uint32_t )(pCRCParams->aempty & 0xF)) <<  24;
    
    /* Write the Almost full Threshold level value*/
    pCRC->CRC_DIN_CTRL_SET = ((uint32_t )(pCRCParams->afull & 0xF)) << 28;
	}
  else
	{
		return ERROR_CRC_INVALID_ARG;
	}
	return RSI_OK;
}

/* This API is used to Set the Input data Information for CRC Calculation. */
uint32_t crc_write_data( CRC_Type *pCRC, RSI_CRC_PARAMS_T *pCRCParams, uint32_t data)
{	
  if(pCRCParams->swapDin ==0 || pCRCParams->swapDin ==1)	
  {
    if(pCRCParams->swapDin)
    {
      pCRC->CRC_DIN_CTRL_SET =BIT(7);
    }
    else
    {
      pCRC->CRC_DIN_CTRL_RESET_REG = BIT(7);
    }
    /* Wait for the FIFO occupancy is less than AFULL Threshold*/
    while((pCRC->CRC_DIN_STS & BIT(2)) == BIT(2));

    if(pCRCParams->dinWidth == DIN_WIDTH_32)
    {
      pCRC->CRC_DIN_FIFO = (uint32_t)data; 
    }
    if(pCRCParams->dinWidth == DIN_WIDTH_16)
    {      
      pCRC->CRC_DIN_FIFO = (uint16_t)data;      
    }
    if(pCRCParams->dinWidth == DIN_WIDTH_8)
    {
      pCRC->CRC_DIN_FIFO = (uint8_t)data; 
    }
  }
  else
  {
    return	ERROR_CRC_INVALID_ARG;
  }
  return RSI_OK;
}

/* This API is used to monitor the CRC Caluclation status and the returns the CRC Value. */
uint32_t monitor_crc_calc( CRC_Type *pCRC, RSI_CRC_PARAMS_T *pCRCParams)
{
	uint32_t crc =0;

	if(pCRCParams->widthType == 2)
  {
		/* Wait for the no.of bytes done.*/
		while((pCRC->CRC_GEN_STS & 0x02)!= 0x02);
	}
	/* Wait for the CRC Cal Done*/
	while((pCRC->CRC_GEN_STS & 0x01)!= 0x01);
  
	/* Read LFSR_STATE for final CRC */
	crc = pCRC->CRC_LFSR_STATE;

	/* Soft reset*/
	pCRC->CRC_GEN_CTRL_SET_REG = BIT(0);

	return crc;
}
/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_CRC_DRIVER  RSI:RS1xxxx CRC 
 *  @{
 *
 */

/**
* @fn           void crc_lfsr_dynamic_write(CRC_Type *pCRC, RSI_CRC_PARAMS_T *pCRCParams )
* @brief        This API can be used for writing the LFSR state directly.
* @param[in]    pCRC - Pointer to the CRC Register set instance area.
* @param[in]    pCRCParams - Pointer to the CRC Parameters structure.
                The possible variable is lfsrState.
* @return       None
*
 */
void crc_lfsr_dynamic_write(CRC_Type *pCRC, RSI_CRC_PARAMS_T *pCRCParams )
{
	pCRC->CRC_LFSR_STATE =pCRCParams->lfsrState;
}
/**
 * @}
 */
/*  This API is used to Reset fifo pointer. This clears the FIFO.When this is set */
void crc_reset_fifo(CRC_Type *pCRC)
{
	pCRC->CRC_DIN_CTRL_SET= BIT(8);
}

/* This API is used to get the fifo status of the crc occupancy */
uint32_t crc_get_fifo_status( CRC_Type *pCRC)
{
	return pCRC->CRC_DIN_STS ;
}
