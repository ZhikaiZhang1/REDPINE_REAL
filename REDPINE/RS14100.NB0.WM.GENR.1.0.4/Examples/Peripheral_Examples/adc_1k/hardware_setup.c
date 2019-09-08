/**
 * @file       hardware_setup.c
 * @version    1.0
 * @date       30 July 2018
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example code for hardware setup for ADC block.
 *
 * @section Description
 * This files contains example code for Sleep state with SRAM Retention from PS2 state. 16KB of SRAM is retained in this mode. UULP_GPIO_BASED_WAKEUP is possible in this state
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
#define DST_INTR_IDX			   27

void hardware_setup()
{
  RSI_ChangeTassRefClock();

	RSI_IPMU_ClockMuxSel(1);						/* Configuring RO-32KHz Clock for BG_PMU */
	RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL);	/* Configuring RC-32KHz Clock for LF-FSM */
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);		/* Configuring RC-32MHz Clock for HF-FSM */
 
	RSI_CLK_PeripheralClkDisable3(M4CLK , M4_SOC_CLK_FOR_OTHER_ENABLE); /* Disable OTHER_CLK which is enabled at Start-up */
  RSI_ULPSS_PeripheralDisable(ULPCLK,ULP_VAD_CLK);
	RSI_ULPSS_TimerClkDisable( ULPCLK ); /* Disable Timer clock which is enabled in Bootloader */

	RSI_ULPSS_DisableRefClks( MCU_ULP_40MHZ_CLK_EN ); 			/* Disabling 40MHz Clocks */

	RSI_PS_BodPwrGateButtonCalibDisable();	/* Power-Down Button Calibration */ //Nagaraj
	RSI_PS_AnalogPeriPtatDisable();					/* Disable PTAT for Analog Peripherals */
	RSI_PS_BodClksPtatDisable();						/* Disable PTAT for Brown-Out Detection Clocks */
	ULP_SPI_MEM_MAP(0x141) &= ~(BIT(11));   //ULP_PDO
	
	/* Power-Down Analog Peripherals */
	RSI_IPMU_PowerGateClr(
				AUXDAC_PG_ENB
			|	WURX_CORR_PG_ENB
			|	WURX_PG_ENB
			|	ULP_ANG_PWRSUPPLY_PG_ENB
			| ULP_ANG_CLKS_PG_ENB
			|	CMP_NPSS_PG_ENB
	);
	
	RSI_PS_SocPllSpiDisable();			/* Power-Down High-Frequency PLL Domain */
	RSI_PS_QspiDllDomainDisable();	/* Power-Down QSPI-DLL Domain */
	RSI_PS_XtalDisable();						/* Power-Down XTAL-OSC Domain */

	ps_wireless_shutdown(); 				/* Shutdown Wireless since Wireless Processor is not present */

	/* Change ULPSS-REF and M4SS-REF and NPSS-HF_FSM clocks to 32MHz RC to be used in PS2 state */
	RSI_ULPSS_EnableRefClks( MCU_ULP_32MHZ_RC_CLK_EN, ULP_PERIPHERAL_CLK, 0 ); 			/* Enable HF-RC Clock */
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);		/* Configuring HF-RC Clock for HF-FSM */
	RSI_PS_FsmHfFreqConfig(20);					/* Configuring HF-FSM Clock Frequency as 20MHz */
	RSI_ULPSS_RefClkConfig( ULPSS_ULP_32MHZ_RC_CLK ); /* Configuring ULPSS Reference Clock to HF-RC */
	RSI_CLK_M4ssRefClkConfig( M4CLK, ULP_32MHZ_RC_CLK ); /* Configuring ULPSS Reference Clock to HF-RC */
	
	*(volatile uint32_t *)(PMU_SPI_BASE_ADDR) &= ~(BIT(3) | BIT(2) | BIT(1) | BIT(0)); /* clear the AHB clock  and SPI clock Ratio bit */ 
	
  /* Time Period Programming */
  RSI_TIMEPERIOD_TimerClkSel(TIME_PERIOD , 0x003E7FFF);
	RSI_TIMEPERIOD_TimerClkSel(TIME_PERIOD , 0x003E7FFF);
	RSI_TIMEPERIOD_TimerClkSel(TIME_PERIOD , 0x003E7FFF);
	RSI_TIMEPERIOD_TimerClkSel(TIME_PERIOD , 0x003E7FFF);

	/* Power-Down Domains in NPSS */
	RSI_PS_NpssPeriPowerDown(
				SLPSS_PWRGATE_ULP_MCURTC
			| SLPSS_PWRGATE_ULP_TIMEPERIOD
			|	SLPSS_PWRGATE_ULP_MCUWDT	
			|	SLPSS_PWRGATE_ULP_MCUPS
			|	SLPSS_PWRGATE_ULP_MCUTS
			|	SLPSS_PWRGATE_ULP_MCUSTORE2
			|	SLPSS_PWRGATE_ULP_MCUSTORE3
	);

	RSI_PS_PowerSupplyDisable(
			  POWER_ENABLE_TIMESTAPING
			| POWER_ENABLE_DEEPSLEEP_TIMER
	);

	/*Before going to power save shift to the Reference clock*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_ULPREFCLK,0);

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
	
  /* Power-Down Unused M4SS Domains */
	RSI_PS_M4ssPeriPowerDown(
			/*	M4SS_PWRGATE_ULP_M4_DEBUG
      |	*/M4SS_PWRGATE_ULP_QSPI
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
		  	ULPSS_2K_BANK_2
			|	ULPSS_2K_BANK_3
			|	ULPSS_2K_BANK_4
			|	ULPSS_2K_BANK_5
			|	ULPSS_2K_BANK_6
			|	ULPSS_2K_BANK_7
	);

	RSI_PS_UlpssRamBanksPeriPowerDown(
		  	ULPSS_2K_BANK_2
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
			|	RAM_BANK_7
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
			|	RAM_BANK_7
			|	RAM_BANK_8
			|	RAM_BANK_9
			|	RAM_BANK_10
			|	RAM_BANK_11
			|	RAM_BANK_12
			|	RAM_BANK_13
	);

	RSI_PS_PmuGoodTimeDurationConfig(PMU_GOOD_TIME);
	RSI_PS_XtalGoodTimeDurationConfig(XTAL_GOOD_TIME);

	RSI_PS_EnableFirstBootUp(1);	/* Enable first boot up */
	RSI_PS_SkipXtalWaitTime(1);		/* XTAL wait time is skipped since RC_32MHZ Clock is used for Processor on wakeup */

	RSI_ConfigBuckBoost(0,1); /* BuckBoost is configured to ON state bypassing the H/W Control */

	RSI_PS_SetRamRetention(	M4ULP_RAM16K_RETENTION_MODE_EN | M4ULP_RAM_RETENTION_MODE_EN | ULPSS_RAM_RETENTION_MODE_EN ); /* Enable SRAM Retention during Sleep */	
}
