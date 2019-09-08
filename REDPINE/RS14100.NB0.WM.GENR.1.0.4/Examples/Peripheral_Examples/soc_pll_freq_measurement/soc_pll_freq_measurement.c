/**
 * @file       soc_pll_freq_measurement.c
 * @version    1.0
 * @date       20 Dec 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example code to measure the soc pll frequency.
 *
 * @section Description
 * This files contains example code to measure the soc pll frequency.
 *
 *
 */
/*Includes*/

#include "rsi_chip.h"

/*Pre-fetch and regestring */
#define ICACHE2_ADDR_TRANSLATE_1_REG  *(volatile uint32_t *)(0x20280000 + 0x24)
#define MISC_CFG_SRAM_REDUNDANCY_CTRL *(volatile uint32_t *)(0x46008000 + 0x18)
#define MISC_CONFIG_MISC_CTRL1        *(volatile uint32_t *)(0x46008000 + 0x44)
#define MISC_QUASI_SYNC_MODE          *(volatile uint32_t *)(0x46008000 + 0x84)

#define PLL_REFERENCE_FREQUENCY         40000000  /*<! PLL input REFERENCE clock 40MHZ */ 

//Note: Change this macro to required PLL frequency in hertz
#define PLL_OUT_FREQUENCY                  5000000 /*<! PLL out clock 5MHz            */
int main(void)
{
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/*pad selection*/
	RSI_EGPIO_PadSelectionEnable(3);
	/*Receive enable*/
	RSI_EGPIO_PadReceiverEnable(12);
	/*Configure the GPIO pin mux to MCU clock Out */
	RSI_EGPIO_SetPinMux(EGPIO , 0 , 12 , EGPIO_PIN_MUX_MODE10);
	/*Switch M4 SOC clock to Reference clock*/
	/*Default keep M4 in reference clock*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_ULPREFCLK,0);

	/*Enable fre-fetch and register if SOC-PLL frequency is more than or equal to 120M*/
#if (PLL_OUT_FREQUENCY >= 120000000)
	/*Configure the prefetch and registering when SOC clock is more than 120Mhz*/
	/*Configure the SOC PLL to 220MHz*/
	ICACHE2_ADDR_TRANSLATE_1_REG  = BIT(21);  //Icache registering when clk freq more than 120
	/*When set, enables registering in M4-TA AHB2AHB. This will have performance penalty. This has to be set above 100MHz*/
	MISC_CFG_SRAM_REDUNDANCY_CTRL = BIT(4);
	MISC_CONFIG_MISC_CTRL1 |= BIT(4); //Enable Register ROM as clock frequency is 200 Mhz
	/*Enable Inter subsystem memory Registering as m4_soc_clk clock is going to tass. above 120Mhz we have to enable this.Also enabling prefetch as when registering by default prefetch is expected to be enabled to save the cycles which are lost in registering*/
#if !(defined WISE_AOC_4)
	MISC_QUASI_SYNC_MODE |= BIT(6);
	/*Enable Inter subsystem memory Registering as m4_soc_clk clock is going to tass. above 120Mhz we have to enable this.Also enabling prefetch as when registering by default prefetch is expected to be enabled to save the cycles which are lost in registering*/
	MISC_QUASI_SYNC_MODE |= (BIT(6) | BIT(7));
#endif //(defined WISE_AOC_4)

#endif
	/*Configure the PLL frequency*/
	RSI_CLK_SetSocPllFreq(M4CLK , PLL_OUT_FREQUENCY , PLL_REFERENCE_FREQUENCY);

#if (PLL_OUT_FREQUENCY >= 120000000)
	/*Enable MCU clock OUT divide/2*/
	RSI_CLK_McuClkOutConfig(M4CLK,MCUCLKOUT_SOC_PLL_CLK,1);
#else
	/*Enable MCU clock OUT divide/1*/
	RSI_CLK_McuClkOutConfig(M4CLK,MCUCLKOUT_SOC_PLL_CLK,0);
#endif

	while(1){
		// Wait for ever to keep system halt
		// Connect logic analyzer at GPIO_12 to check the configured SOC-PLL frequency.
	}
}
