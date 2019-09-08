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
 * @brief This files contains example code for Hardware Setup for Active Operations in PS4 State.
 *
 * @section Description
 * This files contains example code for setting up the Hardware for Active Operations in PS4 State.
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

/*Pre-fetch and regestring */
#define ICACHE2_ADDR_TRANSLATE_1_REG  *(volatile uint32_t *)(0x20280000 + 0x24)
#define MISC_CFG_SRAM_REDUNDANCY_CTRL *(volatile uint32_t *)(0x46008000 + 0x18)
#define MISC_CONFIG_MISC_CTRL1        *(volatile uint32_t *)(0x46008000 + 0x44)
#define MISC_QUASI_SYNC_MODE          *(volatile uint32_t *)(0x46008000 + 0x84)

#define SOC_PLL_REF_FREQUENCY           32000000  /*<! PLL input REFERENCE clock 40MHZ */

//Note: Change this macro to required PLL frequency in hertz
#define PS4_SOC_FREQ                    200000000 /*<! PLL out clock 160MHz            */
#define SWITCH_QSPI_TO_SOC_PLL
#define ICACHE_DISABLE
#define DEBUG_DISABLE

/**
 @brief Application entry point 
 **/
int soc_pll_config(void)
{	
  RSI_CLK_SocPllLockConfig(1,1,7);
	// Configuration to select 32MHz RC to SOC-PLL.
  RSI_CLK_SocPllRefClkConfig(2);  
	/*Switch M4 SOC clock to Reference clock*/
	/*Default keep M4 in reference clock*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_ULPREFCLK,0);
	/*Enable fre-fetch and register if SOC-PLL frequency is more than or equal to 120M*/
#if (PS4_SOC_FREQ >= 120000000)
	/*Configure the prefetch and registering when SOC clock is more than 120Mhz*/
	/*Configure the SOC PLL to 220MHz*/
	ICACHE2_ADDR_TRANSLATE_1_REG  = BIT(21);  //Icache registering when clk freq more than 120
	/*When set, enables registering in M4-TA AHB2AHB. This will have performance penalty. This has to be set above 100MHz*/
	MISC_CFG_SRAM_REDUNDANCY_CTRL = BIT(4);
	MISC_CONFIG_MISC_CTRL1 |= BIT(4); //Enable Register ROM as clock frequency is 200 Mhz
	/*Enable Intersubstsem memory Registering as m4_soc_clk clock is going to tass. above 120Mhz we have to enable this.Also enabling prefetch as when registering by default prefetch is expected to be enabled to save the cycles which are lost in registering*/
#if !(defined WISE_AOC_4)
	MISC_QUASI_SYNC_MODE |= BIT(6);
	/*Enable Intersubstsem memory Registering as m4_soc_clk clock is going to tass. above 120Mhz we have to enable this.Also enabling prefetch as when registering by default prefetch is expected to be enabled to save the cycles which are lost in registering*/
	MISC_QUASI_SYNC_MODE |= (BIT(6) | BIT(7));
#endif //(defined WISE_AOC_4)
#endif //(PS4_SOC_FREQ > 120000000)
	/*Configure the PLL frequency*/
	RSI_CLK_SetSocPllFreq(M4CLK , PS4_SOC_FREQ , SOC_PLL_REF_FREQUENCY);
	/*Switch M4 clock to PLL clock for speed operations*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_SOCPLLCLK,0);
#ifdef SWITCH_QSPI_TO_SOC_PLL
	RSI_CLK_QspiClkConfig(M4CLK,QSPI_INTFPLLCLK,0,0,0);
#endif //SWITCH_QSPI_TO_SOC_PLL
	SystemCoreClock = PS4_SOC_FREQ;
}

int hardware_setup_part1()
{	
  /*Update the system clock and power configurations */
  SystemCoreClockUpdate();
 
  /*Change the TASS reference clock to 32MHz RC clock 
   NOTE: This should not be used in WiSeMCU mode , should be used in MCU only mode
  */
  RSI_ChangeTassRefClock();
	
	
	RSI_PS_BypassLdoRfEnable();
  
  /*Configure the SOC-PLL*/
  soc_pll_config();
  
	RSI_IPMU_ClockMuxSel(1);						/* Configuring RO-32KHz Clock for BG_PMU */
	RSI_PS_FsmLfClkSel(KHZ_RO_CLK_SEL);	/* Configuring RO-32KHz Clock for LF-FSM */
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);		/* Configuring RC-32MHz Clock for HF-FSM */
 
	RSI_CLK_PeripheralClkDisable3(M4CLK , M4_SOC_CLK_FOR_OTHER_ENABLE); /* Disable OTHER_CLK which is enabled at Start-up */
  RSI_ULPSS_TimerClkDisable( ULPCLK ); /* Disable Timer clock which is enabled in Bootloader */
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
	);

	RSI_PS_PowerSupplyDisable(
			  POWER_ENABLE_TIMESTAPING
			| POWER_ENABLE_DEEPSLEEP_TIMER
	);

	RSI_PS_QspiDllDomainDisable();	/* Power-Down QSPI-DLL Domain */
	RSI_PS_XtalDisable();						/* Power-Down XTAL-OSC Domain */
	RSI_PS_WirelessShutdown(); 				/* Shutdown Wireless since Wireless Processor is not present */
  RSI_PS_FlashLdoDisable();
	RSI_CLK_I2sPllTurnOff();
	RSI_CLK_IntfPLLTurnOff();
  
 /* Power-Down Unused M4SS Domains */
	RSI_PS_M4ssPeriPowerDown(
      M4SS_PWRGATE_ULP_QSPI
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
	RSI_PS_PmuGoodTimeDurationConfig(PMU_GOOD_TIME);
	RSI_PS_XtalGoodTimeDurationConfig(XTAL_GOOD_TIME);
	RSI_PS_EnableFirstBootUp(1);	/* Enable first boot up */
	RSI_PS_SkipXtalWaitTime(1);		/* XTAL wait time is skipped since RC_32MHZ Clock is used for Processor on wakeup */

	return 0;
}
