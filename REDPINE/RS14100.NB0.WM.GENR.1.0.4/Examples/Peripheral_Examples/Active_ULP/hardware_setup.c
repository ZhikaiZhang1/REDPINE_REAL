/**
 * @file       hardware_setup.c
 * @version    0.1
 * @date       16 Jan 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example code for Hardware Setup to be used for Active Mode in PS2 State.
 *
 * @section Description
 * This files contains example code for setting up the Hardware for Active operations in PS2 state.
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

int hardware_setup_part1()
{
  /*Update the system clock and power configurations */
  SystemCoreClockUpdate();
  
	// Increasing current in RETN_LDO
  ULP_SPI_MEM_MAP(0x140) |= BIT(19);
	ULP_SPI_MEM_MAP(0x140) |= BIT(18);

	/*Change the TASS reference clock to 32MHz RC clock 
  NOTE: This should not be used in WiSeMCU mode , should be used in MCU only mode
  */
  RSI_ChangeTassRefClock();
	
	RSI_IPMU_ClockMuxSel(1);						/* Configuring RO-32KHz Clock for BG_PMU */
	RSI_PS_FsmLfClkSel(KHZ_RO_CLK_SEL);	/* Configuring RO-32KHz Clock for LF-FSM */
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);		/* Configuring RC-32MHz Clock for HF-FSM */
 
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
			|	ULP_ANG_PWRSUPPLY_PG_ENB
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
			| SLPSS_PWRGATE_ULP_TIMEPERIOD
			| SLPSS_PWRGATE_ULP_MCURTC
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
	RSI_ULPSS_DisableRefClks( MCU_ULP_32MHZ_RC_CLK_EN );		/* Disabling HF_RC Clock */

	/* Switching from PS4 to PS2 state */
	RSI_PS_PowerStateChangePs4toPs2(UULP_MCU_MODE ,
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
	
  /* Power-Down Unused M4SS Domains */
	RSI_PS_M4ssPeriPowerDown(
				M4SS_PWRGATE_ULP_M4_DEBUG
      |	M4SS_PWRGATE_ULP_QSPI
			|	M4SS_PWRGATE_ULP_M4_FPU
      |	M4SS_PWRGATE_ULP_ICACHE
			|	M4SS_PWRGATE_ULP_ETHERNET
      |	M4SS_PWRGATE_ULP_EFUSE
			|	M4SS_PWRGATE_ULP_SDIO_SPI
			|	M4SS_PWRGATE_ULP_USB
			|	M4SS_PWRGATE_ULP_RPDMA
			|	M4SS_PWRGATE_ULP_PERI1
			|	M4SS_PWRGATE_ULP_PERI2
			|	M4SS_PWRGATE_ULP_PERI3
			|	M4SS_PWRGATE_ULP_CCI
			|	M4SS_PWRGATE_ULP_SD_MEM
	);			
  /* Power-Down Unused ULPSS Domains */
	RSI_PS_UlpssPeriPowerDown(
				ULPSS_PWRGATE_ULP_MISC
			|	ULPSS_PWRGATE_ULP_AUX
			|	ULPSS_PWRGATE_ULP_CAP
			|	ULPSS_PWRGATE_ULP_VAD
			|	ULPSS_PWRGATE_ULP_UART
			|	ULPSS_PWRGATE_ULP_SSI
			|	ULPSS_PWRGATE_ULP_I2S
			|	ULPSS_PWRGATE_ULP_I2C
			|	ULPSS_PWRGATE_ULP_IR
			|	ULPSS_PWRGATE_ULP_UDMA
			|	ULPSS_PWRGATE_ULP_FIM
	);
	/*Turn off ULPSS SRAM Core/Periphery domains*/
	RSI_PS_UlpssRamBanksPowerDown(
				ULPSS_2K_BANK_0
      |	ULPSS_2K_BANK_1
      |	ULPSS_2K_BANK_2
			|	ULPSS_2K_BANK_3
			|	ULPSS_2K_BANK_4
			|	ULPSS_2K_BANK_5
			|	ULPSS_2K_BANK_6
			|	ULPSS_2K_BANK_7
	);
	RSI_PS_UlpssRamBanksPeriPowerDown(
				ULPSS_2K_BANK_0
      |	ULPSS_2K_BANK_1
      |	ULPSS_2K_BANK_2
			|	ULPSS_2K_BANK_3
			|	ULPSS_2K_BANK_4
			|	ULPSS_2K_BANK_5
			|	ULPSS_2K_BANK_6
			|	ULPSS_2K_BANK_7
	);	
	/*Turn off M4SS Unused SRAM Core/Periphery domains*/
	RSI_PS_M4ssRamBanksPowerDown(
				RAM_BANK_0
			|	RAM_BANK_1
			|	RAM_BANK_2
			|	RAM_BANK_8
			|	RAM_BANK_9
			|	RAM_BANK_10
			|	RAM_BANK_11
			|	RAM_BANK_12
			|	RAM_BANK_13
	);

	RSI_PS_M4ssRamBanksPeriPowerDown(
				RAM_BANK_0
			|	RAM_BANK_1
			|	RAM_BANK_2
			|	RAM_BANK_8
			|	RAM_BANK_9
			|	RAM_BANK_10
			|	RAM_BANK_11
			|	RAM_BANK_12
			|	RAM_BANK_13
	);
	
  /* switching slpss clk */
  RSI_ULPSS_SlpSensorClkConfig(ULPCLK, 0 , 0xA);		
	RSI_PS_PmuGoodTimeDurationConfig(PMU_GOOD_TIME);
	RSI_PS_XtalGoodTimeDurationConfig(XTAL_GOOD_TIME);
	RSI_PS_EnableFirstBootUp(1);	/* Enable first boot up */
	RSI_PS_SkipXtalWaitTime(1);		/* XTAL wait time is skipped since RC_32MHZ Clock is used for Processor on wakeup */
	return 0;
}
