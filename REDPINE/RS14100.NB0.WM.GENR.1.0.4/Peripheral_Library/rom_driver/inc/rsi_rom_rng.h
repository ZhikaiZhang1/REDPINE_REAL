/**
 * @file       rsi_rom_rng.h
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
 * @brief This files contains functions prototypes releated to RNG.
 * 
 * @section Description
 * This file contains the function definitions for the RNG in different and configuring parameters
 *
 */
#ifndef  __RSI_ROM_RNG_H__
#define  __RSI_ROM_RNG_H__

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_RNG_DRIVERS RSI:RS1xxxx RNG 
 *  @{
 *
 */
#include "rsi_rng.h"
#include "rsi_packing.h"
#include "rsi_rom_table_RS1xxxx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn           STATIC INLINE uint32_t  RSI_RNG_Start(HWRNG_Type *pRNG, uint8_t rngMode)
 * @brief        This API is used to start the Random Number Generation
 * @param[in]    pRNG		 : Random Number Generator handler
 * @param[in]    rngMode : mode of the Random Number Generator
 *               			- \ref RNG_TRUE_RANDOM   - For True RNG
 *               			- \ref RNG_PSEUDO_RANDOM - For Psudo RNG
 * @return       returns 0 \ref RSI_OK on success ,non zero on failure
 *
 */
STATIC INLINE uint32_t  RSI_RNG_Start(HWRNG_Type *pRNG, uint8_t rngMode)
{
#if defined(ROMDRIVER_PRESENT)
	return ROMAPI_RNG_API ->rng_start(pRNG, rngMode);
#else
	return rng_start(pRNG, rngMode);
#endif
}

/**
 * @fn           STATIC INLINE void  RSI_RNG_Stop(HWRNG_Type *pRNG)
 * @brief        This API is used to stop the Random Number Generation
 * @param[in]    pRNG  : Random Number Generator handler
 * @return       none
 */
STATIC INLINE void  RSI_RNG_Stop(HWRNG_Type *pRNG)
{
#if defined(ROMDRIVER_PRESENT)
	ROMAPI_RNG_API ->rng_stop(pRNG);
#else
	rng_stop(pRNG);
#endif
}

/**
 * @fn           STATIC INLINE void  RSI_RNG_GetBytes(HWRNG_Type *pRNG, uint32_t *randomBytes, uint32_t numberOfBytes)
 * @brief        This API is used to get the random number bytes
 * @param[in]    pRNG	         : Random Number Generator handler
 * @param[in]    numberOfBytes : Number of bytes to generate
 * @param[out]   randomBytes	 : variable or array to store generated random bytes
 * @return       none
 */
STATIC INLINE void  RSI_RNG_GetBytes(HWRNG_Type *pRNG, uint32_t *randomBytes, uint32_t numberOfBytes)
{
#if defined(ROMDRIVER_PRESENT)
	ROMAPI_RNG_API ->rng_get_bytes(pRNG, randomBytes, numberOfBytes);
#else
	rng_get_bytes(pRNG, randomBytes, numberOfBytes);
#endif
}

#ifdef __cplusplus
}
#endif


#endif

/* @} end of RSI_RNG_DRIVERS */
