/**
 * @file       PS4_Retention.c
 * @version    0.1
 * @date       05 Jan 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example code for PS4-Sleep with SRAM Retention.
 *
 * @section Description
 * This files contains example code for Sleep state with SRAM Retention from PS4 state. 16KB of SRAM is retained in this mode. UULP_GPIO_BASED_WAKEUP or TIMER_BASED_WAKEUP is possible in this state
 *
 *
 */
/*Includes*/

#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_ps_ram_func.h"
#include "rsi_ds_timer.h"

#define PMU_GOOD_TIME        31    /*Duration in us*/
#define XTAL_GOOD_TIME       31    /*Duration in us*/

int main()
{
  /*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	
	/*Change the TASS reference clock to 32MHz RC clock 
   NOTE: This should not be used in WiSeMCU mode , should be used in MCU only mode
  */
  RSI_ChangeTassRefClock();

 
	RSI_CLK_PeripheralClkDisable3(M4CLK , M4_SOC_CLK_FOR_OTHER_ENABLE); /* Disable OTHER_CLK which is enabled at Start-up */
  ulpss_time_clk_disable( ULPCLK ); /* Disable Timer clock which is enabled in Bootloader */

	RSI_ULPSS_DisableRefClks( MCU_ULP_40MHZ_CLK_EN ); 			/* Disabling 40MHz Clocks */
	RSI_ULPSS_DisableRefClks( MCU_ULP_32KHZ_RC_CLK_EN );		/* Disabling RC_32KHZ Clocks */

	RSI_PS_BodPwrGateButtonCalibDisable();	/* Power-Down Button Calibration */
	RSI_PS_AnalogPeriPtatDisable();					/* Disable PTAT for Analog Peripherals */
	RSI_PS_BodClksPtatDisable();						/* Disable PTAT for Brown-Out Detection Clocks */

	/* Power-Down Analog Peripherals */
	RSI_IPMU_PowerGateClr(
				AUXDAC_PG_ENB
			|	AUXADC_PG_ENB
			|	WURX_CORR_PG_ENB
			|	WURX_PG_ENB
			| ULP_ANG_CLKS_PG_ENB
			|	CMP_NPSS_PG_ENB
	);
	
	/* Power-Down Domains in NPSS */
	RSI_PS_NpssPeriPowerDown(
				SLPSS_PWRGATE_ULP_MCUWDT
			|	SLPSS_PWRGATE_ULP_MCUPS
			|	SLPSS_PWRGATE_ULP_MCUTS
			|	SLPSS_PWRGATE_ULP_MCUSTORE2
			|	SLPSS_PWRGATE_ULP_MCUSTORE3
	);

	RSI_PS_PowerSupplyDisable(
			  POWER_ENABLE_TIMESTAPING
			| POWER_ENABLE_DEEPSLEEP_TIMER
	);

	RSI_PS_SocPllSpiDisable();			/* Power-Down High-Frequency PLL Domain */
	RSI_PS_QspiDllDomainDisable();	/* Power-Down QSPI-DLL Domain */
	RSI_PS_XtalDisable();						/* Power-Down XTAL-OSC Domain */
	
	ps_wireless_shutdown(); 				/* Shutdown Wireless since Wireless Processor is not present */

	RSI_PS_PmuGoodTimeDurationConfig(PMU_GOOD_TIME);		/* Configure PMU Start-up Time to be used on Wakeup */
	RSI_PS_XtalGoodTimeDurationConfig(XTAL_GOOD_TIME);	/* Configure XTAL Start-up Time to be used on Wakeup */
	
	RSI_PS_EnableFirstBootUp(1);	/*Enable first boot up */
	RSI_PS_SkipXtalWaitTime(1);		/* XTAL wait time is skipped since RC_32MHZ Clock is used for Processor on wakeup */

	RSI_PS_SetRamRetention(	M4ULP_RAM16K_RETENTION_MODE_EN ); /* Enable SRAM Retention of 16KB during Sleep */
  /*Configure the wake up paramters for boot loader */
 	ROMAPI_PWR_API->RSI_GotoSleepWithRetention(0 , (uint32_t)RSI_PS_RestoreCpuContext , 0 , 4);
	/*Goto Sleep with retention */
	RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,DISABLE_LF_MODE);

}
