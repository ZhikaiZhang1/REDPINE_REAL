 /**
 * @file       Sdio_Host_Exp.c
 * @version    1.0
 * @date       10th AUG 2018
 *
 * Copyright(C) Redpine Signals 2018
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
 * This file contains example code to demonstrate the SDIO Host functionality.
 *
 *
 */

/* Includes -------------------------------------------------------------------------------------------------------*/
#include "rsi_chip.h"

/* Private typedef ------------------------------------------------------------------------------------------------*/

/* Private define -------------------------------------------------------------------------------------------------*/

/* Private macro --------------------------------------------------------------------------------------------------*/
#define BLOCK_SIZE                10240
#define SOC_OPER_FREQUENCY        30000000 /*220Mhz*/
#define SOC_OPER_FREQUENCY_SIM    200000000 /*220Mhz*/
#define M4_SLEEP_CLK              (0x46000000 + 0x24)
#define SDMEM_INTFPLLCLK          (0x46000000 + 0x40)  
#define MEMORY_LOCATION           0x16000        //16 bit address and 17th bit 1 for memory data writes
#define SMIH_CLK                  25
#define SMIH_CMD                  26                     
#define SMIH_D0                   27
#define SMIH_D1                   28
#define SMIH_D2                   29
#define SMIH_D3                   30
#define SMIH_CD                   6
#define SMIH_WP                   7

/* Private variables ----------------------------------------------------------------------------------------------*/
uint8_t Write_Data[BLOCK_SIZE];
uint8_t Read_Data[BLOCK_SIZE]={0};
volatile uint32_t match=0,unmatch=0;

/* Private functions ----------------------------------------------------------------------------------------------*/
void M4_SDIOH_PinMux()
{
	/*Pad selection Enable*/
	RSI_EGPIO_PadSelectionEnable(1);

	/*Enable Host Pad Gpio modes*/
	RSI_EGPIO_HostPadsGpioModeEnable(SMIH_CLK);
	RSI_EGPIO_HostPadsGpioModeEnable(SMIH_CMD);
	RSI_EGPIO_HostPadsGpioModeEnable(SMIH_D0);
	RSI_EGPIO_HostPadsGpioModeEnable(SMIH_D1);
	RSI_EGPIO_HostPadsGpioModeEnable(SMIH_D2);
	RSI_EGPIO_HostPadsGpioModeEnable(SMIH_D3);

	/*Ren enables for Gpios*/ 
	RSI_EGPIO_PadReceiverEnable(SMIH_CLK);
	RSI_EGPIO_PadReceiverEnable(SMIH_CMD);
	RSI_EGPIO_PadReceiverEnable(SMIH_D0);
	RSI_EGPIO_PadReceiverEnable(SMIH_D1);
	RSI_EGPIO_PadReceiverEnable(SMIH_D2);
	RSI_EGPIO_PadReceiverEnable(SMIH_D3);
  RSI_EGPIO_PadReceiverEnable(SMIH_CD);
	RSI_EGPIO_PadReceiverEnable(SMIH_WP);

	/*SDIO Connected to Tass*/
	(*(volatile uint32_t *) (0x41300004)) =(0x1<<5);

	/*Configure software pull ups for cmd ,d0,d1,d2,d3*/
	RSI_EGPIO_PadDriverDisableState(SMIH_CMD,Pullup);
	RSI_EGPIO_PadDriverDisableState(SMIH_D0,Pullup);
	RSI_EGPIO_PadDriverDisableState(SMIH_D1,Pullup);
	RSI_EGPIO_PadDriverDisableState(SMIH_D2,Pullup);
	RSI_EGPIO_PadDriverDisableState(SMIH_D3,Pullup);

  RSI_EGPIO_PadDriverDisableState(SMIH_WP,Pulldown);
  RSI_EGPIO_PadDriverDisableState(SMIH_CD,Pulldown);

	/*Configure Mux*/
	RSI_EGPIO_SetPinMux(EGPIO ,0 , SMIH_CLK ,8);  
	RSI_EGPIO_SetPinMux(EGPIO ,0 , SMIH_CMD ,8);  
	RSI_EGPIO_SetPinMux(EGPIO ,0 , SMIH_D0  ,8);  
	RSI_EGPIO_SetPinMux(EGPIO ,0 , SMIH_D1  ,8);  
	RSI_EGPIO_SetPinMux(EGPIO ,0 , SMIH_D2  ,8);  
	RSI_EGPIO_SetPinMux(EGPIO ,0 , SMIH_D3  ,8);  
	RSI_EGPIO_SetPinMux(EGPIO ,0 , SMIH_CD  ,8); 
	RSI_EGPIO_SetPinMux(EGPIO ,0 , SMIH_WP  ,8);  
}
/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */ 
int main(void)
{
	/* At this stage the microcontroller clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs9116.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs9116.c file
	 */

	SDIOH_CARD_CONFIG_T      SdiohConfg ;
	status_t              Stat           = RSI_SDIOH_OK;
	uint32_t              i                 = 0;
	int forever = 1;
    
  // Clear structure
  memset(&SdiohConfg, 0, sizeof(SdiohConfg));
  
	SdiohConfg.highSpeedEnable    = 0;
	SdiohConfg.uhsModeSelect      = UHS_NONE;      //uhs mode will work only in 4bit mode
	SdiohConfg.admaMode           = 0;
	SdiohConfg.busWidthMode       = SDIOH_BUS_WIDTH4;
	SdiohConfg.clock              = SDIOH_CLOCK_25MHZ;
	SdiohConfg.voltage            = 0; //set this bit in case of 1.8v
	SdiohConfg.funcNum            = 1;
	SdiohConfg.blockModeEnable    = 1; //enable in case of block mode
	SdiohConfg.opCode             = 1;
	SdiohConfg.byteBlockSize      = 256;
	SdiohConfg.numberOfBlocks     = 40;

	/*Configures the system default clock and power configurations*/
	SystemCoreClockUpdate();

	/*Systic Config*/
	SysTick_Config(SOC_OPER_FREQUENCY_SIM / 1000);

	/*Configure Source clock and  module clock for SDMEM */
	RSI_CLK_SetSocPllFreq(M4CLK,50000000,40000000);
	RSI_CLK_M4SocClkConfig(M4CLK,M4_SOCPLLCLK,0);
	RSI_CLK_SdMemClkConfig(M4CLK ,1,SDMEM_SOCPLLCLK,1);

	/*Initialize GPIO pins */
	M4_SDIOH_PinMux();

	/*Smih IO Card Initialization*/
	Stat = RSI_SDIOH_Enumeration(&SdiohConfg);

	if(Stat == RSI_SDIOH_OK)
	{
		//init success
		for(i = 0; i<BLOCK_SIZE;i++)
		{      
			Write_Data[i] =i;			
		}
	}
	else
	{
		while(1); // Initialization failed
	}

	Stat =  RSI_SDIOH_ClockEnable(&SdiohConfg,SDIOH_CLOCK_25MHZ);

	/*Write Data to memory*/
	Stat =  RSI_SDIOH_ByteBlockWriteCmd53 (&SdiohConfg ,Write_Data,MEMORY_LOCATION);
	if(Stat != 0)
	{
		//write fail
		return Stat;
	}

	/*read data from memory*/
	Stat = RSI_SDIOH_ByteBlockReadCmd53(&SdiohConfg ,Read_Data,MEMORY_LOCATION);
	if(Stat != 0)
	{
		//read fail
		return Stat;
	}

	//compare data
	for(i = 0; i<BLOCK_SIZE;i++)
	{
		if(Write_Data[i]==Read_Data[i])
		{
			match++;
		}
		else
		{
			//data mismatch
			unmatch++;
		}
	}
	while (forever)		
	{

	}
	/*Statement will never reach here , just to satisfy the standard main*/
	return 0;
}
