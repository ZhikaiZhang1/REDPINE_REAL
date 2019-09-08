/**************************************************************************//**
 * @file     system_ARMCM4.h
 * @brief    CMSIS Device System Header File for
 *           ARMCM4 Device Series
 * @version  V 0.9
 * @date     20. Dec 2016
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2011 - 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
 *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include<stdint.h>

#ifndef __SYSTEM_ARMCM4_H__
#define __SYSTEM_ARMCM4_H__

/**
 * 
 */
#include "rsi_error.h"
#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */
extern uint32_t npssIntrState;
extern uint32_t __sp;


void RSI_PS_ConfigureTaMemories(void);
/*WiSeAOC specific VTOR values*/
#if defined (WISE_AOC_MODE)
#define VECTOR_OFF_SET_TA_ROM         (0x100000 + 0x70100 ) /*<! M4 execution from TA ROM                  */
#define VECTOR_OFF_SET_M4_ROM         (0x300000 + 0xB100  ) /*<! M4 execution from M4 ROM                  */
#define VECTOR_OFF_SET_TA_ROM_DIRECT  (0x80000  + 0x70100 ) /*<! M4 execution from TA ROM in directed mode */
#endif //WISE_AOC_MODE

typedef enum SLEEP_TYPE{
	SLEEP_WITH_RETENTION,    /*Saves the CPU context while entering into sleep */
	SLEEP_WITHOUT_RETENTION, /*do not Save the CPU context while entering into sleep */
}SLEEP_TYPE_T;

#define DISABLE_LF_MODE      0x00
#define LF_32_KHZ_RC         BIT(0)
#define LF_32_KHZ_XTAL       BIT(1)
#define EXTERNAL_CAP_MODE    BIT(2)
#define HF_MHZ_RO            3

/*System default clocks*/
#define DEFAULT_SOC_PLL_CLOCK     80000000
#define DEFAULT_INTF_PLL_CLOCK    80000000
#define DEFAULT_MODEM_PLL_CLOCK   80000000
#define DEFAULT_32MHZ_RC_CLOCK    32000000
#define DEFAULT_20MHZ_RO_CLOCK    20000000
#define DEFAULT_DOUBLER_CLOCK     (2 * DEFAULT_20MHZ_RO_CLOCK)
#define DEFAULT_32KHZ_RC_CLOCK    32000
#define DEFAULT_32KHZ_RO_CLOCK    32000
#define DEFAULT_32KHZ_XTAL_CLOCK  32000
#define DEFAULT_RF_REF_CLOCK      40000000
#define DEFAULT_MEMS_REF_CLOCK    40000000
#define DEFAULT_BYP_RC_CLOCK      32000000
#define DEFAULT_I2S_PLL_CLOCK    6144000

/* system clock source look up table*/
typedef struct SYSTEM_CLOCK_SOURCE_FREQUENCIES {
  uint32_t 					m4ss_ref_clk;
  uint32_t 					ulpss_ref_clk;
  uint32_t          soc_pll_clock;
  uint32_t          soc_clock;
	uint32_t					modem_pll_clock;
	uint32_t					intf_pll_clock;
  uint32_t          i2s_pll_clock;
	uint32_t					modem_pll_clock2;
	uint32_t					ro_32khz_clock;
	uint32_t					rc_32khz_clock;
	uint32_t					xtal_32khz_clock;
	uint32_t					rc_32mhz_clock;
	uint32_t					ro_20mhz_clock;
	uint32_t					doubler_clock;
  uint32_t					sleep_clock;
  uint32_t					rf_ref_clock;
  uint32_t					mems_ref_clock;
  uint32_t					byp_rc_ref_clock;
} SYSTEM_CLOCK_SOURCE_FREQUENCIES_T;

extern SYSTEM_CLOCK_SOURCE_FREQUENCIES_T system_clocks;

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 */

/**
 * \defgroup RSI_SYSTEM_RS1XXXX RSI:SYSTEM RS1XXXX  
 *  @{
 *
 */

extern void SystemInit (void);

extern void SystemCoreClockUpdate (void);

// this function is used to shift soc clock to pll clock

// extern void shift_soc_clk_to_160(void);

error_t RSI_PS_EnterDeepSleep(SLEEP_TYPE_T sleepType , uint8_t lf_clk_mode);

void RSI_PS_SaveCpuContext(void);
void RSI_PS_RestoreCpuContext(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_ARMCM4_H__ */
