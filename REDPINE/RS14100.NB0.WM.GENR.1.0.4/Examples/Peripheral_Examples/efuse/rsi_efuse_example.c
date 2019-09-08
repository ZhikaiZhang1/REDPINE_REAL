 /**
 * @file       rsi_efuse_example.c
 * @version    0.9
 * @date       20th Dec 2017
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
 * This file contains example code to demonstrate the Efuse functionality.
 *
 *
 */
/* Includes -------------------------------------------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"

/* Private typedef ------------------------------------------------------------------------------------------------*/

/* Private define -------------------------------------------------------------------------------------------------*/
#define ICACHE2_ADDR_TRANSLATE_1_REG  *(volatile uint32_t *)(0x20280000 + 0x24)
#define MISC_CFG_SRAM_REDUNDANCY_CTRL *(volatile uint32_t *)(0x46008000 + 0x18)
#define MISC_CONFIG_MISC_CTRL1        *(volatile uint32_t *)(0x46008000 + 0x44)
#define MISC_QUASI_SYNC_MODE          *(volatile uint32_t *)(0x46008000 + 0x84) 
/* Private macro --------------------------------------------------------------------------------------------------*/
/*eFUSE addresses*/
#define WRITE_ADD_0   0x00000
#define WRITE_ADD_1   0x00001
#define WRITE_ADD_2   0x00002
#define HOLD 0x140
#define CLOCK 160 

/* Private variables ----------------------------------------------------------------------------------------------*/
volatile int i=1;

/* Private function prototypes ------------------------------------------------------------------------------------*/

/* Private functions ----------------------------------------------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	/* At this stage the micro-controller clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs9116.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs9116.c file
	 */
	uint8_t u8DataZero,u8DataOne ,u8DataTwo;
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	while(i)
	{
		/*NOTE : in eFUSE programming, programming only onces is possible by default all locations are zeros. 
		   Each write operation sets the bits in particular location it is only one time programmable and those
    	 particular locations will be corrupted ,If you wants to run these example do manually "i" value to zero */
	}
	if(CLOCK > 120000000)
	{
		/*Configure the prefetch and registering when SOC clock is more than 120Mhz*/
		/*Configure the SOC PLL to 220MHz*/
		ICACHE2_ADDR_TRANSLATE_1_REG  = BIT(21);  //Icache registering when clk freq more than 120
		/*When set, enables registering in M4-TA AHB2AHB. This will have performance penalty. This has to be set above 100MHz*/
		MISC_CFG_SRAM_REDUNDANCY_CTRL = BIT(4);
		MISC_CONFIG_MISC_CTRL1 |= BIT(4); //Enable Register RSI as clock frequency is 200 Mhz

		MISC_QUASI_SYNC_MODE |= BIT(6);
		/*Enable Inter subsystem memory Registering as m4_soc_clk clock is going to TAss. above 120Mhz we have to enable this.Also enabling prefetch as when registering by default prefetch is expected to be enabled to save the cycles which are lost in registering*/
		MISC_QUASI_SYNC_MODE |= (BIT(6) | BIT(7)); 
	}
	/* Switch M4 SOC clock to Reference clock */
	/* Default keep M4 in reference clock     */

	/* Configure the m4_soc clocks */
	RSI_CLK_M4SocClkConfig(M4CLK,M4_ULPREFCLK,0);

	/* Set the Soc PLL clock to particular frequency */
	RSI_CLK_SetSocPllFreq(M4CLK,CLOCK,40000000);

	/* Configure the m4_soc clocks */
	RSI_CLK_M4SocClkConfig(M4CLK,M4_SOCPLLCLK,0);

	/*  Enable the peripheral clocks for SET3 register */
	RSI_CLK_PeripheralClkEnable3(M4CLK,(BIT(5)|BIT(19)));

	/*Writing 0XAF Data at address 0x00000*/
	/*Addredd :0x00000 -> Data : 1010 1111  */

	/* Write WRITE_ADD_0 , bit location 0*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_0 , 0,HOLD); 

	/* Write WRITE_ADD_0 , bit location 1*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_0 , 1,HOLD);

	/* Write WRITE_ADD_0 , bit location 2*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_0 , 2,HOLD);

	/* Write WRITE_ADD_0 , bit location 3*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_0 , 3,HOLD);

	/* Write WRITE_ADD_0 , bit location 5*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_0 , 4,HOLD);

	/* Write WRITE_ADD_0 , bit location 7*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_0 , 5,HOLD);


	/*Writing 0X2A Data at address 0x00001*/
	/*Addredd :0x00001 -> Data : 0010 1010  */

	/* Write WRITE_ADD_1 , bit location 1*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_1 , 1,HOLD);

	/* Write WRITE_ADD_1 , bit location 3*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_1 , 3,HOLD);

	/* Write WRITE_ADD_1 , bit location 5*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_1 , 5,HOLD);


	/* Writing 0X55 Data at address 0x00001*/
	/* Addredd :0x00002 -> Data : 0101 0101  */
	/* Write WRITE_ADD_2 , bit location 0*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_2 , 0,HOLD);

	/* Write WRITE_ADD_2 , bit location 2*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_2 , 2,HOLD);

	/* Write WRITE_ADD_2 , bit location 4*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_2 , 4,HOLD);

	/* Write WRITE_ADD_2 , bit location 6*/
	RSI_EFUSE_WriteBit(EFUSE ,WRITE_ADD_2 , 6,HOLD);

	/*eFUSE read Operation*/

	/* Read byte fRSI address 0 in FSM read mode */
	RSI_EFUSE_FsmReadByte(EFUSE ,WRITE_ADD_0, &u8DataZero,CLOCK);
	if(u8DataZero==0xaf)
	{ 
		/*  Prints on hyper-terminal  */
		DEBUGOUT("MATCH\n");
	}
	else
	{
		/*  Prints on hyper-terminal  */
		DEBUGOUT("UNMATCH\n");
	}

	/* Read byte fRSI address 1 in FSM read mode */
	RSI_EFUSE_FsmReadByte(EFUSE ,WRITE_ADD_1, &u8DataOne,CLOCK);

	if(u8DataOne==0x2a)
	{
		/*  Prints on hyper-terminal  */
		DEBUGOUT("MATCH\n");
	}
	else
	{ 
		/*  Prints on hyper-terminal  */
		DEBUGOUT("UNMATCH\n");
	}
	/* Read byte fRSI address 2 in FSM read mode */
	RSI_EFUSE_FsmReadByte(EFUSE ,WRITE_ADD_2, &u8DataTwo,CLOCK);
	if(u8DataTwo==0x55)
	{
		/*  Prints on hyper-terminal  */
		DEBUGOUT("MATCH\n");
	}
	else
	{
		/*  Prints on hyper-terminal  */
		DEBUGOUT("UNMATCH\n");
	}

	while(1);
}
