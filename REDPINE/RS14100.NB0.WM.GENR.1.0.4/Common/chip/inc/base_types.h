/**
 * @file       base_types.h
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
 * @brief This files contains data types
 *
 * @section Description
 * This files contains data types
 *
 *
 */
#ifndef __BASE_TYPES_H__
#define __BASE_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include <stddef.h>
#include "stdint.h"
#include <stdbool.h>
#include <string.h>  
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
#ifndef TRUE
/** Value is true (boolean_t type) */
#define TRUE        ((boolean_t) 1)
#endif

#ifndef FALSE
/** Value is false (boolean_t type) */
#define FALSE       ((boolean_t) 0)
#endif  

/** Returns the minimum value out of two values */
#define MIN( X, Y )  ((X) < (Y) ? (X) : (Y))

/** Returns the maximum value out of two values */
#define MAX( X, Y )  ((X) > (Y) ? (X) : (Y))

/** Returns the dimension of an array */
#define  DIM( X )  (sizeof(X) / sizeof(X[0]))

#define  BIT(x)    ((uint32_t)1 << (x))

typedef enum en{
	Enable = 1,
	Disable = 0
}en_t;
/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/** logical datatype (only values are TRUE and FALSE) */
typedef uint8_t      boolean_t;

/** single precision floating point number (4 byte) */
typedef float        float32_t;

/** double precision floating point number (8 byte) */
typedef double       float64_t;

/** ASCCI character for string generation (8 bit) */
typedef char         char_t;

/** function pointer type to void/void function */
typedef void         (*func_ptr_t)(void);

/** function pointer type to void/uint8_t function */
typedef void         (*func_ptr_arg1_t)(uint8_t);


#define RSI_DRIVER_VERSION_MAJOR_MINOR(major,minor) (((major) << 8) | (minor))

/**
\brief Driver Version
 */
typedef struct _RSI_DRIVER_VERSION {
	uint16_t api;                         ///< API version
	uint16_t drv;                         ///< Driver version
} RSI_DRIVER_VERSION;

/* General return codes */
#define RSI_DRIVER_OK                 0 ///< Operation succeeded
#define RSI_DRIVER_ERROR             -1 ///< Unspecified error
#define RSI_DRIVER_ERROR_BUSY        -2 ///< Driver is busy
#define RSI_DRIVER_ERROR_TIMEOUT     -3 ///< Timeout occurred
#define RSI_DRIVER_ERROR_UNSUPPORTED -4 ///< Operation not supported
#define RSI_DRIVER_ERROR_PARAMETER   -5 ///< Parameter error
#define RSI_DRIVER_ERROR_SPECIFIC    -6 ///< Start of driver specific errors

#define SET_BIT(n)                     BIT(n)//((uint32_t)1 << n)
#define CLR_BIT(n)                    ~BIT(n)//(~((uint32_t)1 << n))

#define STATIC static

#ifdef __CC_ARM
#define INLINE  __inline
#else
#define INLINE inline
#endif

#define   ENABLE                      1
#define   DISABLE											0

#ifdef __cplusplus
}
#endif

#endif /* __BASE_TYPES_H__ */

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
