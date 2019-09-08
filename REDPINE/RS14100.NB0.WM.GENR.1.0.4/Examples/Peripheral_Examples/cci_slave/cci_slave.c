 /**
 * @file       cci_slave.c
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
 * This files contains example code to demonstrate the CCI Slave functionality.
 *
 *
 */
/* Includes ---------------------------------------------------------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "rsi_board.h"

/* Private typedef --------------------------------------------------------------------------------------------------------------*/

/* Private define ---------------------------------------------------------------------------------------------------------------*/
#define  CCI_CNTRL               *(volatile uint32_t *)(0x46170000+0x00)
#define  CCI_AHB_SLAVE_ADDR       0x60000000     
#define  CCI_IRQHandler	          IRQ071_Handler
#define  CCI_NVIC_NAME            CCI_IRQn

//!mode to be select 
#define  SDR_MODE                 0
#define  DDR_MODE                 1

//!address width config 
#define _40BIT_ADDRESS_WIDTH      0x3
#define _32BIT_ADDRESS_WIDTH      0x2
#define _24BIT_ADDRESS_WIDTH      0x1
#define _16BIT_ADDRESS_WIDTH      0x0

//!interface width
#define  CCI_QUAD_MODE            0x0
#define  CCI_OCTA_MODE            0x1
#define  CCI_WORD_MODE            0x2

/* Private macro ----------------------------------------------------------------------------------------------------------------*/
#define  TRANSLATION_ADDRESS    0xA0000000
#define  SLAVE_LSB_ADDRESS      0x00000000
#define  SLAVE_MSB_ADDRESS      0x50000000 
#define  SLAVE_TIMEOUT          0x0

#define CCI_D0        12
#define CCI_D1        13
#define CCI_D2        14
#define CCI_D3        15
#define CCI_CLOCK     10
#define CCI_CS        53
#define CCI_RDY       35
#define CCI_DATAVALID 36

/* Private variables ------------------------------------------------------------------------------------------------------------*/
volatile uint32_t read;

/* Private function prototypes --------------------------------------------------------------------------------------------------*/

/* Private functions ------------------------------------------------------------------------------------------------------------*/
void CCI_PinMux()
{
	uint32_t i;
	for(i=0;i<22;i++)
	{
		/*enable pad selection bits*/
		RSI_EGPIO_PadSelectionEnable(i);
	}

	/*enable ren for the cci pins*/
	RSI_EGPIO_PadReceiverEnable(CCI_D0);
	RSI_EGPIO_PadReceiverEnable(CCI_D1);
	RSI_EGPIO_PadReceiverEnable(CCI_D2);
	RSI_EGPIO_PadReceiverEnable(CCI_D3);
	RSI_EGPIO_PadReceiverEnable(CCI_CLOCK);
	RSI_EGPIO_PadReceiverEnable(CCI_CS);
	RSI_EGPIO_PadReceiverEnable(CCI_RDY);
	RSI_EGPIO_PadReceiverEnable(CCI_DATAVALID);

	/*Configure mode for the cci pins*/
	RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , CCI_D0 ,EGPIO_PIN_MUX_MODE9);   
	RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , CCI_D1 ,EGPIO_PIN_MUX_MODE10);   
	RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , CCI_D2 ,EGPIO_PIN_MUX_MODE10);
	RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , CCI_D3 ,EGPIO_PIN_MUX_MODE10);   
	RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , CCI_CLOCK ,EGPIO_PIN_MUX_MODE9);   
	RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , CCI_CS ,EGPIO_PIN_MUX_MODE9);   
	RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , CCI_RDY ,EGPIO_PIN_MUX_MODE9);   
	RSI_EGPIO_SetPinMux(EGPIO ,EGPIO_PORT0 , CCI_DATAVALID ,EGPIO_PIN_MUX_MODE9);   
}

void Write_Data(uint32_t Offset,uint32_t Data)
{
	*( volatile uint32_t *)(CCI_AHB_SLAVE_ADDR+Offset)=Data;
}

uint32_t  Read_Data(uint32_t Offset)
{
	return *( volatile uint32_t *)(CCI_AHB_SLAVE_ADDR+Offset); 
}

/*   Function call back when event is raised    */

/**
 * @brief	Interrupt handler
 * @return	Nothing
 */
void CCI_IRQHandler()
{
	//RSI_CCI_IntClear(0x1);
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
	
			/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/*Enables pad selection bits,enables receive_enable for the cci pins and 
   configures the pins for CCI.(enable gpio muxing )   */
	CCI_PinMux();

  /* Configure the CCI clocks.   */
	RSI_CLK_CciClkConfig(M4CLK ,CCI_M4_SOC_CLK_FOR_OTHER_CLKS ,1,ENABLE_STATIC_CLK);

	//octa enable
	//CCI_CNTRL|=(0x1<<9);

	//cci_en_bit
	CCI_CNTRL|=(0x1U<<31);

 	
	while(1) 
	{
	 /*  Prints on hyper-terminal as "Core is Sleeping...\n" */
		DEBUGOUT("Core is Sleeping...\n");
		
		/*waits here, till interrupt occurs*/
		__WFI();
	}
	/*Statement will never reach here , just to satisfy the standard main*/

}

