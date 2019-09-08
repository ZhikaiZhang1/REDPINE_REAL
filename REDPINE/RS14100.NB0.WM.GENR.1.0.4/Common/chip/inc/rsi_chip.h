/**
 * @file       rsi_chip.h
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
 * @brief This files contains includes for all the peripherals
 *
 *
 * @section Description
 * This file contains the common include files.
 *
 */

#ifndef __RSI_CHIP_H__
#define __RSI_CHIP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "base_types.h"
#include "RS1xxxx.h"
#include "core_cm4.h"
#include "rsi_error.h"
#ifdef __ICCARM__  
#include "cmsis_iar.h"
#endif
  

/**/
#include "rsi_error.h"
#include "rsi_timers.h"
#include "rsi_ulpss_clk.h"	
#include "rsi_pll.h"
#include "rsi_power_save.h"
#include "rsi_egpio.h"
#include "rsi_fim.h"
#include "rsi_cci.h"
#include "rsi_crc.h"
#include "rsi_efuse.h"
#include "rsi_ethernet.h"
#include "rsi_ulpss_clk.h"
#include "rsi_pwm.h"
#include "rsi_qei.h"
#include "rsi_qspi_proto.h"
#include "rsi_qspi.h"
#include "rsi_gspi.h"
#include "rsi_rng.h"
#include "rsi_gpdma.h"
#include "rsi_ct.h"
#include "rsi_sio.h"
#include "rsi_timers.h"
#include "rsi_udma.h"
#include "rsi_rtc.h"
#include "rsi_reg_spi.h"
#include "rsi_processor_sensor.h"
#include "rsi_retention.h"
#include "rsi_temp_sensor.h"
#include "rsi_time_period.h"
#include "rsi_wwdt.h"
#include "rsi_efuse.h"
#include "rsi_ipmu.h"
#include "rsi_adc.h"
#include "rsi_dac.h"
#include "rsi_comparator.h"
#include "rsi_wurx.h"
#include "rsi_vad.h"
#include "rsi_sdioh.h"

#include "rsi_rom_egpio.h"
#include "rsi_rom_power_save.h"
#include "rsi_rom_timer.h"
#include "rsi_rom_udma.h"	
#include "rsi_rom_ct.h"
#include "rsi_rom_gpdma.h"	
#include "rsi_rom_clks.h"
#include "rsi_rom_ulpss_clk.h"
#include "rsi_rom_qspi.h"
#include "rsi_rom_fim.h"	
#include "rsi_rom_efuse.h"
#include "rsi_rom_crc.h"
#include "rsi_rom_rng.h"
#include "rsi_rom_pwm.h"
#include "rsi_system_config.h"



#ifdef __cplusplus
}
#endif
#endif /* __RSI_CHIP_H__ */

