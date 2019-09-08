/**
 * @file      rsi_rng.c
 * @version   1.0
 * @date      1 Aug 2017
 *
 *  Copyright(C) Redpine Signals 2016
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief This file contains APIs related to Random Number Generation
 *
 *  @section Description
 *  This file contains APIs related to Random Number Generation
 *
 *
 */

 /* Includes */
 
#include "rsi_chip.h"


/*
 This API is used to start the Random Number Generation
*/
uint32_t rng_start(HWRNG_Type *pRNG, uint8_t rngMode)
{
	if(rngMode == 0 || rngMode ==1)
	{
		if(rngMode == RSI_RNG_TRUE_RANDOM)
		{
			pRNG->HWRNG_CTRL_REG_b.HWRNG_RNG_ST = 1;
		}
		else
		{
			pRNG->HWRNG_CTRL_REG_b.HWRNG_PRBS_ST = 1;
		}
	}
	else
	{
		return ERROR_RNG_INVALID_ARG;
	}	
return RSI_OK;
}	


/*This API is used to stop the Random Number Generation
*/
void rng_stop(HWRNG_Type *pRNG)
{
  //Disable clock
  pRNG->HWRNG_CTRL_REG = 0;

}

/*This API is used to get the random number bytes
*/
void rng_get_bytes(HWRNG_Type *pRNG, uint32_t *randomBytes, uint32_t numberOfBytes)
{
  uint32_t i;

  for(i = 0; i < numberOfBytes; i++)
  {
	  randomBytes[i] = pRNG->HWRNG_RAND_NUM_REG;
  }
}
