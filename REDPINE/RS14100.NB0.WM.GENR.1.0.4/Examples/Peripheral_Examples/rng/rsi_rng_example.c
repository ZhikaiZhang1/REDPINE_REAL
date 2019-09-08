
/**
 * @file
 * @version
 * @date
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief :
 *
 *  @section Description  This file contains 
 *
 *
 */


#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_rom_rng.h"

#define RNG_TRUE_RANDOM    0
#define RNG_PSEUDO_RANDOM  1
#define RNG_NUM_OF_WORDS_TO_READ 100

uint32_t random_bytes[RNG_NUM_OF_WORDS_TO_READ];


int main()
{

	uint32_t count=0U;
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
  /*Enable the debug interface*/
  DEBUGINIT();
	
	/* Enable the peripheral clocks for SET1 register */
  RSI_CLK_PeripheralClkEnable1(M4CLK , HWRNG_PCLK_ENABLE);
	
	/* Start the Random Number Generation */
	RSI_RNG_Start(HWRNG, RNG_TRUE_RANDOM);

  /* Get random bytes */
  RSI_RNG_GetBytes(HWRNG, random_bytes, RNG_NUM_OF_WORDS_TO_READ);
	
	/* Stop the Random Number Generation */
  RSI_RNG_Stop(HWRNG);
	for(count = 0 ; count < RNG_NUM_OF_WORDS_TO_READ ; count++)
	{
		/*  Prints on hyper-terminal  */
    DEBUGOUT("0x%x\n",random_bytes[count]);
  }

	while(1);
}
