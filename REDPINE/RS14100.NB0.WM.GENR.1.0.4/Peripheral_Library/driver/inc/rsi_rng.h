/**
 * @file      rsi_rng.h
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
 *  @brief This file contains functions prototypes related to Random Number Generation peripheral
 *
 *  @section Description 
 *  this file contains list of functions prototypes for the RNG
 *
 *
 */

#ifndef __RSI_RNG_H__
#define __RSI_RNG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "RS1xxxx.h"
#include "base_types.h"

/******************************************************
 * *                      Macros
 * ******************************************************/
#define RSI_RNG_TRUE_RANDOM    0
#define RSI_RNG_PSEUDO_RANDOM  1
/******************************************************
 * *                    Constants
 * ******************************************************/
/******************************************************
 * *                   Enumerations
 * ******************************************************/
/******************************************************
 * *                 Type Definitions
 * ******************************************************/
/******************************************************
 * *                    Structures
 * ******************************************************/
/******************************************************
 * *                 Global Variables
 * ******************************************************/
/******************************************************
 * *               Function Declarations
 * ******************************************************/
uint32_t rng_start(HWRNG_Type *pRNG, uint8_t rng_mode);
void rng_stop(HWRNG_Type *pRNG);
void rng_get_bytes(HWRNG_Type *pRNG, uint32_t *random_bytes, uint32_t number_of_bytes);

#define HWRNG_CLK_ENABLE           0x400000

#ifdef __cplusplus
}
#endif

#endif // __RSI_RNG_H__
