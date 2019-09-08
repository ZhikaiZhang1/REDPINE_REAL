/**
 * @file       PS2_Retention.c
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
 * @brief This files contains example code for PS2-Sleep with SRAM Retention.
 *
 * @section Description
 * This files contains example code for Sleep state with SRAM Retention from PS2 state. 16KB of SRAM is retained in this mode. UULP_GPIO_BASED_WAKEUP or DST_BASED_WAKEUP is possible in this state
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
  RSI_ULPSS_TimerClkDisable( ULPCLK ); /* Disable Timer clock which is enabled in Bootloader */
	RSI_ULPSS_DisableRefClks( MCU_ULP_40MHZ_CLK_EN ); 			/* Disabling 40MHz Clocks */
	RSI_ULPSS_DisableRefClks( MCU_ULP_32KHZ_RC_CLK_EN );		/* Disabling LF_RC Clocks */
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

	RSI_PS_WirelessShutdown(); 				/* Shutdown Wireless since Wireless Processor is not present */

	/* Change ULPSS-REF and M4SS-REF and NPSS-HF_FSM clocks to 20MHz RO to be used in PS2 state */
	RSI_ULPSS_EnableRefClks( MCU_ULP_20MHZ_RING_OSC_CLK_EN, ULP_PERIPHERAL_CLK, 0 ); 			/* Enable HF-RO Clock */
	RSI_PS_FsmHfClkSel(FSM_20MHZ_RO);		/* Configuring HF-RO Clock for HF-FSM */
	RSI_PS_FsmHfFreqConfig(20);					/* Configuring HF-FSM Clock Frequency as 20MHz */
	RSI_ULPSS_RefClkConfig( ULPSS_ULP_20MHZ_RINGOSC_CLK ); /* Configuring ULPSS Reference Clock to HF-RO */
	RSI_CLK_M4ssRefClkConfig( M4CLK, ULP_20MHZ_RINGOSC_CLK ); /* Configuring ULPSS Reference Clock to HF-RO */
	/* Switching from PS4 to PS2 state */
	RSI_PS_PowerStateChangePs4toPs2(ULP_MCU_MODE ,
			PWR_MUX_SEL_ULPSSRAM_SCDC_0_9        ,
			PWR_MUX_SEL_M4_ULP_RAM_SCDC_0_9      ,
			PWR_MUX_SEL_M4_ULP_RAM16K_SCDC_0_9   ,
			PWR_MUX_SEL_M4ULP_SCDC_0_6           ,
			PWR_MUX_SEL_ULPSS_SCDC_0_9           ,
			DISABLE_BG_SAMPLE_ENABLE             ,
			DISABLE_DC_DC_ENABLE                 ,
			DISABLE_SOCLDO_ENABLE                ,
			DISABLE_STANDBYDC
	);
	
	RSI_PS_EnableFirstBootUp(1);	/* Enable first boot up */
	RSI_PS_SkipXtalWaitTime(1);		/* XTAL wait time is skipped since RC_32MHZ Clock is used for Processor on wakeup */
	RSI_PS_SetRamRetention(	M4ULP_RAM16K_RETENTION_MODE_EN ); /* Enable SRAM Retention of 16KB during Sleep */
	/*Configure the wake up paramters for boot loader */
 	ROMAPI_PWR_API->RSI_GotoSleepWithRetention(0 , (uint32_t)RSI_PS_RestoreCpuContext , 0 , 4);
  /*Goto Sleep with retention */
	RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,DISABLE_LF_MODE);

}
