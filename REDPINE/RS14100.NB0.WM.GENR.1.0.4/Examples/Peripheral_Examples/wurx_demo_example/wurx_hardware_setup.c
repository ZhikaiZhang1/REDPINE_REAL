
#include "rsi_chip.h"

void hardware_setup()
{
	    RSI_ChangeTassRefClock();
		  ps_wireless_shutdown(); 				/* Shutdown Wireless since Wireless Processor is not present */
			/*Power down NPSS peripherals*/
			RSI_PS_NpssPeriPowerDown(
				SLPSS_PWRGATE_ULP_TIMEPERIOD |
				SLPSS_PWRGATE_ULP_MCUSTORE3 |
				SLPSS_PWRGATE_ULP_MCUSTORE2 |
				SLPSS_PWRGATE_ULP_MCUTS     |
				SLPSS_PWRGATE_ULP_MCUPS     |
				SLPSS_PWRGATE_ULP_MCUWDT		|
				SLPSS_PWRGATE_ULP_MCURTC);

		/*Disable deep sleep timer in sleep and time stamp*/
		RSI_PS_PowerSupplyDisable(POWER_ENABLE_TIMESTAPING | POWER_ENABLE_DEEPSLEEP_TIMER);

		/*Power gate the sub RAM sections of M4SS*/
		RSI_PS_M4ssRamBanksPowerDown(
				RAM_BANK_8 | /* M4ULP     */
				RAM_BANK_9 | /* M4ULP     */
				RAM_BANK_10| /* TASS RAM  */
				RAM_BANK_11| /* TASS RAM  */
				RAM_BANK_12| /* TASS RAM  */
				RAM_BANK_13| /* TASS RAM  */
				RAM_BANK_0 | /* M4SS      */
				RAM_BANK_1 | /* M4SS      */
				RAM_BANK_2   /* M4SS      */
		);
			
			
		/*Turn off TASS SRAM PERI domains*/
		(*(volatile uint32_t *)0x2404841C) = (BIT(16) | BIT(17) | BIT(18) | BIT(19));

		/*Clock cleaner settings*/
		MCU_FSM->MCU_FSM_REF_CLK_REG_b.M4SS_REF_CLK_CLEANER_OFF_b  = 1 ;
		MCU_FSM->MCU_FSM_REF_CLK_REG_b.ULPSS_REF_CLK_CLEANER_OFF_b = 1 ;
		MCU_FSM->MCU_FSM_REF_CLK_REG_b.ULPSS_REF_CLK_CLEANER_ON_b  = 0 ;
		MCU_FSM->MCU_FSM_REF_CLK_REG_b.M4SS_REF_CLK_CLEANER_ON_b   = 0 ;
			
		/*M4SS peripheral power gates */
		RSI_PS_M4ssPeriPowerDown(
				M4SS_PWRGATE_ULP_ETHERNET |
				M4SS_PWRGATE_ULP_SDIO_SPI |
				M4SS_PWRGATE_ULP_USB      |
				M4SS_PWRGATE_ULP_RPDMA    |
				M4SS_PWRGATE_ULP_PERI2    |
				M4SS_PWRGATE_ULP_PERI3    |
				M4SS_PWRGATE_ULP_CCI      |
				M4SS_PWRGATE_ULP_SD_MEM);

		/*Ideally OFF every thing what is not required*/
		RSI_IPMU_PowerGateClr(
				AUXDAC_PG_ENB |
				AUXADC_PG_ENB |
				CMP_NPSS_PG_ENB
		);

		ULP_SPI_MEM_MAP(0x129) = 0x048000;	// BG_LDO_IN_LP_MODe, [16] - LP_mode EN, [20:18] - LDO Output voltage(2(D):0.7v)
		ULP_SPI_MEM_MAP(0x127) &= ~(BIT(3)); //an_perif_ptat_en = 0;

			 /*Ideally OFF every thing what is not required*/
		RSI_IPMU_PowerGateClr(
			//	ULP_ANG_PWRSUPPLY_PG_ENB |
				ULP_ANG_CLKS_PG_ENB 			
		);

		/*Before going to power save shift to the Reference clock*/
		RSI_CLK_M4SocClkConfig(M4CLK,M4_ULPREFCLK,0);

		/*select wurx based wakeup source*/
		RSI_PS_SetWkpSources(WAKEIF_BASED_WAKEUP);
		
		RSI_PS_SetRamRetention(
						ULPSS_RAM_RETENTION_MODE_EN    | /*For S port RAM access*/
						M4ULP_RAM_RETENTION_MODE_EN    |		
						M4ULP_RAM16K_RETENTION_MODE_EN  /*From BANK3 to BANK 6*/						
					);
}
