/**
 * @file       rsi_time_period.h
 * @version    0.1
 * @date       20 jan 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains
 *
 * @section Description
 *
 */


/**
 * Includes
 */

#ifndef  __RSI_TIME_PERIOD_H__
#define  __RSI_TIME_PERIOD_H__
#include "RS1xxxx.h"

#ifdef __cplusplus
extern "C" {
#endif

error_t RSI_TIMEPERIOD_TimerClkSel
(
		TimePeriodCalib_t *pstcTimePeriod,
		uint32_t  u32TimePeriod
);

#ifdef __cplusplus
}
#endif

/*End of file not truncated*/
#endif /*__RSI_TIME_PERIOD_H__*/
