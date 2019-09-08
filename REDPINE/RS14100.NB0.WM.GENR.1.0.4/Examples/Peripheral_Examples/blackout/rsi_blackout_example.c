/**
 * @file       Deepsleep.c
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
 * @brief This files contains example code for DeepSleep Mode.
 *
 * @section Description
 * This files contains example code for Deepsleep Mode. RAM is not retained in this mode. Only UULP_GPIO_BASED_WAKEUP is possible in this state
 *
 *
 */
/*Includes*/

#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_ps_ram_func.h"
#include "rsi_ds_timer.h"
#include "rsi_bod.h"
#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_ps_ram_func.h"
#include "rsi_ds_timer.h"

#define PMU_GOOD_TIME        31    /*Duration in us*/
#define XTAL_GOOD_TIME       31    /*Duration in us*/
#define  MANUAL_MODE          0
#define AUTOMATIC_MODE        1
#define ENABLE          1
#define BOD_THRESHOLD   2.2
#define SLOT_VALUE      3
#define NVIC_BOD          NPSS_TO_MCU_BOD_INTR_IRQn
#define  NPSS_TO_MCU_BOD_IRQHandler   IRQ023_Handler 
#define NVIC_RTC                              MCU_CAL_RTC_IRQn  

#define PMU_GOOD_TIME        31    /*Duration in us*/
#define XTAL_GOOD_TIME       31    /*Duration in us*/

void NPSS_TO_MCU_BOD_IRQHandler(void)
{
	volatile uint32_t  statusRead = 0 ;

	statusRead = RSI_BOD_GetIntrStatus();

	RSI_NPSSGPIO_SetPin(4,1);
	RSI_BOD_IntrClr();
}

int main()
{
	volatile static uint32_t loop=1;
	/**Must be called*/
	SystemCoreClockUpdate();
	/*Change the TASS reference clock to 32MHz RC clock 
NOTE: This should not be used in WiSeMCU mode , should be used in MCU only mode
*/
	RSI_ChangeTassRefClock();

	/* Configuring RO-32KHz Clock for BG_PMU */
	RSI_IPMU_ClockMuxSel(1);
	/* Configuring RO-32KHz Clock for LF-FSM */
	RSI_PS_FsmLfClkSel(KHZ_RO_CLK_SEL);
	/* Configuring RC-32MHz Clock for HF-FSM */
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);

	RSI_CLK_PeripheralClkDisable3(M4CLK , M4_SOC_CLK_FOR_OTHER_ENABLE); /* Disable OTHER_CLK which is enabled at Start-up */
	ulpss_time_clk_disable( ULPCLK ); /* Disable Timer clock which is enabled in Bootloader */

	RSI_ULPSS_DisableRefClks( MCU_ULP_40MHZ_CLK_EN ); 			/* Disabling 40MHz Clocks */
	RSI_ULPSS_DisableRefClks( MCU_ULP_32KHZ_RC_CLK_EN );		/* Disabling RC_32KHZ Clocks */
	////	
	////	/* Power-Down Domains in NPSS */
	RSI_PS_NpssPeriPowerDown(
			SLPSS_PWRGATE_ULP_MCURTC
			|	SLPSS_PWRGATE_ULP_MCUWDT
			|	SLPSS_PWRGATE_ULP_MCUPS
			|	SLPSS_PWRGATE_ULP_MCUTS
			//|	SLPSS_PWRGATE_ULP_MCUSTORE1
			//|	SLPSS_PWRGATE_ULP_MCUSTORE2
			//|	SLPSS_PWRGATE_ULP_MCUSTORE3
			//|	SLPSS_PWRGATE_ULP_TIMEPERIOD
			);

	RSI_PS_PowerSupplyDisable(
			POWER_ENABLE_TIMESTAPING
			|	POWER_ENABLE_DEEPSLEEP_TIMER
			|	POWER_ENABLE_RETENTION_DM
			);

	RSI_PS_SocPllSpiDisable();			/* Power-Down High-Frequency PLL Domain */
	RSI_PS_QspiDllDomainDisable();	/* Power-Down QSPI-DLL Domain */
	RSI_PS_XtalDisable();						/* Power-Down XTAL-OSC Domain */

	ps_wireless_shutdown(); 				/* Shutdown Wireless since Wireless Processor is not present */

	RSI_PS_PmuGoodTimeDurationConfig(PMU_GOOD_TIME);		/* Configure PMU Start-up Time to be used on Wakeup */
	RSI_PS_XtalGoodTimeDurationConfig(XTAL_GOOD_TIME);	/* Configure XTAL Start-up Time to be used on Wakeup */

	RSI_PS_EnableFirstBootUp(1);	/*Enable first boot up */
	RSI_PS_SkipXtalWaitTime(1);		/* XTAL wait time is skipped since RC_32MHZ Clock is used for Processor on wakeup */

	//  /*select which banks needs to be powered on during sleep*/
	//  /*RAM retention controls */        
	RSI_PS_SetRamRetention(M4ULP_RAM16K_RETENTION_MODE_EN       
			| ULPSS_RAM_RETENTION_MODE_EN          
			| TA_RAM_RETENTION_MODE_EN             
			| M4ULP_RAM_RETENTION_MODE_EN          
			| M4SS_RAM_RETENTION_MODE_EN   
			);

	/* clock enable */
	RSI_IPMU_ClockMuxSel(1);
	/* selecting the fsm lf clock*/
	RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL);
	/* selecting the fsm Hf clock*/
	RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);
	/*BOD enable and set the threshold value */
	RSI_BOD_Enable(ENABLE,BOD_THRESHOLD) ;
	/* set the slot value */
	RSI_BOD_ConfigSlotValue(SLOT_VALUE) ;
	/*set the BOD to automatic  mode*/
	RSI_BOD_SetMode(AUTOMATIC_MODE) ;
	/* Enable the blackout reset */
	RSI_BOD_BlackOutReset(ENABLE);
	/*Un mask the BOD inter+rupt*/
	RSI_PS_NpssIntrUnMask(NPSS_TO_MCU_BOD_INTR);
	NVIC_EnableIRQ(NVIC_BOD);
	//  /*Configure the BOD as wake up source */
	//  RSI_PS_SetWkpSources(COMPR_BASED_WAKEUP);

	while(1){
		/* */
		RSI_PS_RetentionSleepConfig(0 , (uint32_t)RSI_PS_RestoreCpuContext, 0 , 4);   
		/* */
		RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,DISABLE_LF_MODE);

	}
}



