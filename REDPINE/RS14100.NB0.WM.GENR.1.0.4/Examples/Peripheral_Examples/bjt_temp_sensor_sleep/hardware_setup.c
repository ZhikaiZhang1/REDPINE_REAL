#include "rsi_chip.h"

#define NVIC_RTC                MCU_CAL_RTC_IRQn    /*<! RTC NVIC enable   */
#define NPSS_GPIO_PIN           3
#define SEC_BASED_WKP           /*<! Default sec based wake-up              */ 
#define NVIC_RTC_ALARM          MCU_CAL_ALARM_IRQn 
#define PMU_GOOD_TIME           31    /*Duration in us*/
#define XTAL_GOOD_TIME          31    /*Duration in us*/
#define MHZ_CLOCK               32



/*Configure the RTC */
void RTC_Start( void )
{
	/*Start RTC */
	RSI_RTC_Start(RTC);
	NVIC_EnableIRQ(NVIC_RTC);
#ifdef  SEC_BASED_WKP
	RSI_RTC_IntrUnMask(RTC_SEC_INTR);
#else
	RSI_RTC_IntrUnMask(RTC_MSEC_INTR);
#endif //SEC_BASED_WKP	
}

/*Msec and Sec IRQ handler */
void IRQ029_Handler()
{
#ifdef SEC_BASED_WKP	
	/*Clears the wakeup status */
	RSI_RTC_IntrClear (RTC_SEC_INTR);
#else
	/*Clears the wakeup status */
	RSI_RTC_IntrClear (RTC_MSEC_INTR);
#endif 	
}


void hardware_setup()
{
	/*Configure the NPSS-GPIO for sleep wake up indication*/
	RSI_NPSSGPIO_InputBufferEn(NPSS_GPIO_PIN , 1U);
	RSI_NPSSGPIO_SetPinMux(NPSS_GPIO_PIN , 0);
	RSI_NPSSGPIO_SetDir(NPSS_GPIO_PIN , NPSS_GPIO_DIR_OUTPUT);

	// Increasing current in RETN_LDO
	ULP_SPI_MEM_MAP(0x140) |= BIT(19);
	ULP_SPI_MEM_MAP(0x140) |= BIT(18);

	/*Change the TASS reference clock to 32MHz RC clock 
NOTE: This should not be used in WiSeMCU mode , should be used in MCU only mode
*/
	RSI_ChangeTassRefClock();

	RSI_IPMU_ClockMuxSel(1);						/* Configuring RO-32KHz Clock for BG_PMU */
	RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL);	/* Configuring RO-32KHz Clock for LF-FSM */
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);		/* Configuring RC-32MHz Clock for HF-FSM */

	RSI_CLK_PeripheralClkDisable3(M4CLK , M4_SOC_CLK_FOR_OTHER_ENABLE); /* Disable OTHER_CLK which is enabled at Start-up */
	RSI_ULPSS_TimerClkDisable( ULPCLK ); /* Disable Timer clock which is enabled in Bootloader */

	RSI_ULPSS_DisableRefClks( MCU_ULP_40MHZ_CLK_EN ); 			/* Disabling 40MHz Clocks */
	RSI_ULPSS_DisableRefClks( MCU_ULP_32KHZ_RC_CLK_EN );		/* Disabling LF_RC Clocks */

	RSI_PS_BodPwrGateButtonCalibDisable();	/* Power-Down Button Calibration */

	RSI_PS_BodClksPtatDisable();						/* Disable PTAT for Brown-Out Detection Clocks */

	/* Power-Down Analog Peripherals */
	RSI_IPMU_PowerGateClr(
			AUXDAC_PG_ENB
			| AUXADC_PG_ENB
			|	WURX_CORR_PG_ENB
			|	WURX_PG_ENB
			| ULP_ANG_CLKS_PG_ENB
			|	CMP_NPSS_PG_ENB
			);

	/* Power-Down Domains in NPSS */
	RSI_PS_NpssPeriPowerDown(
			SLPSS_PWRGATE_ULP_MCUWDT
			|	SLPSS_PWRGATE_ULP_MCUPS
			|	SLPSS_PWRGATE_ULP_MCUSTORE2
			|	SLPSS_PWRGATE_ULP_MCUSTORE3
			);

	RSI_PS_PowerSupplyDisable(
			POWER_ENABLE_TIMESTAPING
			| POWER_ENABLE_DEEPSLEEP_TIMER
			);

	RSI_PS_SocPllSpiDisable();			/* Power-Down High-Frequency PLL Domain */
	RSI_PS_QspiDllDomainDisable();	/* Power-Down QSPI-DLL Domain */

	RSI_PS_WirelessShutdown(); 			/* Shutdown Wireless since Wireless Processor is not present */

	/* Change ULPSS-REF and M4SS-REF and NPSS-HF_FSM clocks to 20MHz RO to be used in PS2 state */
	RSI_ULPSS_EnableRefClks( MCU_ULP_32MHZ_RC_CLK_EN, ULP_PERIPHERAL_CLK, 0 ); 			/* Enable HF-RO Clock */
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);		/* Configuring HF-RO Clock for HF-FSM */
	RSI_PS_FsmHfFreqConfig(MHZ_CLOCK);					/* Configuring HF-FSM Clock Frequency as 20MHz */
	RSI_ULPSS_RefClkConfig( ULPSS_ULP_32MHZ_RC_CLK ); /* Configuring ULPSS Reference Clock to HF-RO */
	RSI_CLK_M4ssRefClkConfig( M4CLK, ULP_32MHZ_RC_CLK ); /* Configuring ULPSS Reference Clock to HF-RO */

	RSI_ULPSS_DisableRefClks(	MCU_ULP_32MHZ_RC_CLK_EN );		/* Disabling HF_RC Clock */

	RSI_PS_EnableFirstBootUp(1);	/* Enable first boot up */
	
	RSI_PS_SkipXtalWaitTime(1);		/* XTAL wait time is skipped since RC_32MHZ Clock is used for Processor on wakeup */

	RSI_PS_SetRamRetention(	M4ULP_RAM16K_RETENTION_MODE_EN |M4ULP_RAM_RETENTION_MODE_EN); /* Enable SRAM Retention of 16KB during Sleep */

	/* Configure NPSS-HF_FSM Clock to 32MHz for Sleep/Wakeup Routine */
	RSI_ULPSS_EnableRefClks( MCU_ULP_32MHZ_RC_CLK_EN, ULP_PERIPHERAL_CLK, 0 ); 			/* Enable HF-RC Clock */
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);		/* Configuring HF-RC Clock for HF-FSM */
	RSI_PS_FsmHfFreqConfig(MHZ_CLOCK);					/* Configuring HF-FSM Clock Frequency as 32MHz */
	RSI_TIMEPERIOD_TimerClkSel(TIME_PERIOD , 0x003E7FFF);  

#ifdef SEC_BASED_WKP
	RSI_PS_SetWkpSources(SEC_BASED_WAKEUP);
#else
	RSI_PS_SetWkpSources(MSEC_BASED_WAKEUP);
#endif 
	/*Start RTC */
	RTC_Start();
}