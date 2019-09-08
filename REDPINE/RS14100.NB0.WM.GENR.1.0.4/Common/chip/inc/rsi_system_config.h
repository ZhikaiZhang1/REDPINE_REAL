/**
 * @file       rsi_ps_ipmu_config.h
 * @version    1.0
 * @date       08 Jan 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contains the programming values for IPMU
 *
 * @section Description
 * 
 *
 */
// Revision History
// Date           |  version number  |  Modification
// 08/01/2017        1.0                Initial version

/**
 * Includes
 */

#ifndef __RSI_SYSTEM_CONFIG_H__
#define __RSI_SYSTEM_CONFIG_H__

#include "rsi_chip.h"

#ifdef __cplusplus
extern "C" {
#endif
  
extern uint32_t scdc_volt_sel[];
extern uint32_t lp_scdc_extcapmode[];  
extern uint32_t m32rc_osc_trim_efuse[] ;
extern uint32_t m20rc_osc_trim_efuse[] ;
extern uint32_t dblr_32m_trim_efuse[]  ;
extern uint32_t m20ro_osc_trim_efuse[] ;
extern uint32_t ro_32khz_trim_efuse[] ;
extern uint32_t rc_16khz_trim_efuse[] ;
extern uint32_t rc_64khz_trim_efuse[] ;
extern uint32_t rc_32khz_trim_efuse[] ;
extern uint32_t vbatt_status_trim_efuse[] ;
extern uint32_t ro_ts_efuse[] ;
extern uint32_t ro_tempsense_config[];
extern uint32_t vbg_tsbjt_efuse[] ;
extern uint32_t auxadc_off_diff_efuse[] ;
extern uint32_t auxadc_gain_diff_efuse[] ;
extern uint32_t auxadc_off_se_efuse[] ;
extern uint32_t auxadc_gain_se_efuse[] ;
extern uint32_t bg_trim_efuse[] ;
extern uint32_t blackout_trim_efuse[] ;
extern uint32_t poc_bias_efuse[] ;
extern uint32_t  buck_trim_efuse[] ;
extern uint32_t ldosoc_trim_efuse[] ;
extern uint32_t dpwm_freq_trim_efuse[] ;
extern uint32_t delvbe_tsbjt_efuse[] ;
extern uint32_t xtal1_bias_efuse[] ;
extern uint32_t xtal2_bias_efuse[] ;
extern uint32_t bod_cmp_hyst[] ;
extern uint32_t ipmu_bod_clks_common_config2[] ;
extern uint32_t ipmu_bod_clks_common_config1[] ;
extern uint32_t  pmu_common_config[];
extern uint32_t ipmu_common_config[];
extern uint32_t xtal1_khz_fast_start_en[];
extern uint32_t xtal1_khz_fast_start_disable[];
extern uint32_t  hp_ldo_voltsel[];
  
	
extern uint32_t  ana_perif_ptat_common_config2[];
extern uint32_t  ana_perif_ptat_common_config1[];	  
extern uint32_t  retn_ldo_0p75[] ;
extern uint32_t  retn_ldo_lpmode[] ;
extern uint32_t  retn_ldo_hpmode[]  ;
extern uint32_t  retn_ldo_voltsel[] ;

#define POWER_TARN_DISABLE         0  
#define POWER_TARN_ALWAYS_USE      1  
#define POWER_TARN_CONDITIONAL_USE 2

  
#define  XTAL_CAP_MODE   POWER_TARN_CONDITIONAL_USE

#ifdef __cplusplus
}
#endif


#endif //__RSI_SYSTEM_CONFIG_H__
