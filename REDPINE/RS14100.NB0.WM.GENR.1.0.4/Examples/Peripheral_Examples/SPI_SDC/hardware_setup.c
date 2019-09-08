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

#define NVIC_RTC                MCU_CAL_RTC_IRQn    /*<! RTC NVIC enable   */
#define NVIC_RTC_ALARM          MCU_CAL_ALARM_IRQn 

#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_ps_ram_func.h"
#include "rsi_ds_timer.h"

#define PMU_GOOD_TIME        31    /*Duration in us*/
#define XTAL_GOOD_TIME       31    /*Duration in us*/

#define DST_INTR_IDX			   27

/*Configure the RTC */
void RTC_Start( void )
{
  /*Start RTC */
	RSI_RTC_Start(RTC);
	NVIC_EnableIRQ(NVIC_RTC);
	RSI_RTC_IntrUnMask(RTC_SEC_INTR);
	
}

/*Msec and Sec IRQ handler */
void IRQ029_Handler()
{
 /*Clears the wakeup status */
	RSI_RTC_IntrClear (RTC_SEC_INTR);
}

void hardware_setup()
{

  SystemCoreClockUpdate();
  RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL);
	/*Change the TASS reference clock to 32MHz RC clock 
  NOTE: This should not be used in WiSeMCU mode , should be used in MCU only mode
  */
  RSI_ChangeTassRefClock();

	RSI_CLK_PeripheralClkDisable3(M4CLK , M4_SOC_CLK_FOR_OTHER_ENABLE); /* Disable OTHER_CLK which is enabled at Start-up */
  RSI_ULPSS_TimerClkDisable( ULPCLK ); /* Disable Timer clock which is enabled in Bootloader */

	RSI_ULPSS_DisableRefClks( MCU_ULP_40MHZ_CLK_EN ); 			/* Disabling 40MHz Clocks */
	RSI_ULPSS_DisableRefClks( MCU_ULP_32KHZ_RO_CLK_EN );		/* Disabling LF_RC Clocks */

	RSI_PS_BodPwrGateButtonCalibDisable();	/* Power-Down Button Calibration */
	RSI_IPMU_ProgramConfigData(ana_perif_ptat_common_config2);	/* Disable PTAT for Analog Peripherals */
	RSI_IPMU_ProgramConfigData(ipmu_bod_clks_common_config2);		/* Disable PTAT for Brown-Out Detection Clocks */

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
	);

	RSI_PS_SocPllSpiDisable();			/* Power-Down High-Frequency PLL Domain */
	RSI_PS_QspiDllDomainDisable();	/* Power-Down QSPI-DLL Domain */
	RSI_PS_XtalDisable();						/* Power-Down XTAL-OSC Domain */

	RSI_PS_WirelessShutdown(); 				/* Shutdown Wireless since Wireless Processor is not present */

	/* Change 32MHz-RC to 20MHz-RC to be used as Processor Clock in PS2 state */
	RSI_IPMU_M20rcOsc_TrimEfuse();
	RSI_PS_FsmHfFreqConfig(20);
	system_clocks.rc_32mhz_clock 	  = 20000000;
	
  /* Time Period Programming */
  RSI_TIMEPERIOD_TimerClkSel(TIME_PERIOD , 0x003E7FFF);
	
	/* Configuring Retention-LDO O/P Voltage to 0.75V */
 	RSI_IPMU_ProgramConfigData(retn_ldo_0p75);
	/* Move to HP mode */
	RSI_IPMU_ProgramConfigData(retn_ldo_hpmode);
	
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
	
  /* Power-Up SPI, UDMA Domains required for the Application */
	RSI_PS_UlpssPeriPowerUp(
				ULPSS_PWRGATE_ULP_SSI
			|	ULPSS_PWRGATE_ULP_UDMA
	);

  /* Power-Up 4KB of ULPSS SRAM required for the Application */
	RSI_PS_UlpssRamBanksPowerUp(
				ULPSS_2K_BANK_0
      |	ULPSS_2K_BANK_1
	);

	RSI_PS_UlpssRamBanksPeriPowerUp(
				ULPSS_2K_BANK_0
      |	ULPSS_2K_BANK_1
	);

	RSI_PS_EnableFirstBootUp(1);	/* Enable first boot up */
	RSI_PS_SkipXtalWaitTime(1);		/* XTAL wait time is skipped since RC_32MHZ Clock is used for Processor on wakeup */

//  RSI_PS_SetWkpSources(SEC_BASED_WAKEUP);
//	RTC_Start();
	RSI_PS_SetWkpSources(DST_BASED_WAKEUP); /* Enable Deep-Sleep Timer as Wakeup Source */
	RSI_DST_DurationSet(75000);		 /* Configure Sleep Duration of 75ms */
	RSI_DST_IntrUnMask();					 /* Unmask Deep-Sleep Timer Interrupt */
	NVIC_EnableIRQ(DST_INTR_IDX);  /* Enable IRQ for Deep-Sleep Timer */

	RSI_PS_SetRamRetention(	M4ULP_RAM16K_RETENTION_MODE_EN | M4ULP_RAM_RETENTION_MODE_EN | ULPSS_RAM_RETENTION_MODE_EN ); /* Enable SRAM Retention during Sleep */

	
}
