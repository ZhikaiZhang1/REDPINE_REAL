/**
 * @file       rsi_Ps2_Sleep.c
 * @version    0.9
 * @date       27 Jun 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example code of all end to end functionality
 *
 * @section Description
 * This files contains example Ps2 sleep
 *
 *
 */
/* Includes ------------------------------------------------------------------*/


#define NVIC_RTC                MCU_CAL_RTC_IRQn    /*<! RTC NVIC enable   */
#define NPSS_GPIO_PIN           3
//#define SEC_BASED_WKP           /*<! Default sec based wake-up              */ 
#define NVIC_RTC_ALARM          MCU_CAL_ALARM_IRQn 
#define PMU_GOOD_TIME           31    /*Duration in us*/
#define XTAL_GOOD_TIME          31    /*Duration in us*/
#define MHZ_CLOCK               32

//=============================================================================
// board/chip defines
#include "rsi_board.h"
#include "rsi_chip.h"
//=============================================================================

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

/*Application starts here*/
int main()
{
	/*Configures the system default clock and power configurations*/
	SystemCoreClockUpdate();
	
	/*Configure the NPSS-GPIO for sleep wake up indication*/
	RSI_NPSSGPIO_InputBufferEn(NPSS_GPIO_PIN , 1U);
	RSI_NPSSGPIO_SetPinMux(NPSS_GPIO_PIN , 0);
	RSI_NPSSGPIO_SetDir(NPSS_GPIO_PIN , NPSS_GPIO_DIR_OUTPUT);
  
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
			//|	ULP_ANG_PWRSUPPLY_PG_ENB
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

	RSI_PS_WirelessShutdown(); 			/* Shutdown Wireless since Wireless Processor is not present */

	RSI_PS_FsmHfFreqConfig(MHZ_CLOCK);					/* Configuring HF-FSM Clock Frequency as 32MHz */
	RSI_CLK_M4ssRefClkConfig( M4CLK, ULP_32MHZ_RC_CLK ); /* Configuring ULPSS Reference Clock to HF-RO */
	RSI_ULPSS_DisableRefClks(	MCU_ULP_32MHZ_RC_CLK_EN );		/* Disabling HF_RC Clock */		
	RSI_PS_EnableFirstBootUp(1);	/* Enable first boot up */
	RSI_PS_SkipXtalWaitTime(1);		/* XTAL wait time is skipped since RC_32MHZ Clock is used for Processor on wakeup */
	RSI_PS_SetRamRetention(	M4ULP_RAM16K_RETENTION_MODE_EN | ULPSS_RAM_RETENTION_MODE_EN ); /* Enable SRAM Retention of 16KB during Sleep */

	RSI_TIMEPERIOD_TimerClkSel(TIME_PERIOD , 0x003E7FFF);  
	
#ifdef SEC_BASED_WKP
  RSI_PS_SetWkpSources(SEC_BASED_WAKEUP);
#else
  RSI_PS_SetWkpSources(MSEC_BASED_WAKEUP);
#endif 
	/*Start RTC */
	RTC_Start();

	while(1)
	{
		/*Make GPIO-Low */
	  RSI_NPSSGPIO_SetPin(NPSS_GPIO_PIN,0U);
	  
		/*Configure the wake up paramters for boot loader */
		RSI_PS_RetentionSleepConfig(0x24061000 , (uint32_t)RSI_PS_RestoreCpuContext, 0x8012000, 1);   
	  /*Goto Sleep with retention */
		RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,DISABLE_LF_MODE);
		/*Up on wake up execution resumes from here*/
		
		/*Make GPIO-High */
		RSI_NPSSGPIO_SetPin(NPSS_GPIO_PIN,1U);		
	}
}


//=============================================================================



