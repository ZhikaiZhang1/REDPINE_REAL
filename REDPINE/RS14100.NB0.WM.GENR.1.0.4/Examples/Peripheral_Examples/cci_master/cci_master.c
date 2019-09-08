 /**
 * @file       cci_master.c
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
 * This files contains example code to demonstrate the CCI Master functionality.
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
	/* At this stage the micro-controller clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs9116.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs9116.c file
	 */
	
	
  RSI_CCI_Init_t cci_config;
	cci_config.mode                                  = SDR_MODE;             //!SDR mode selected
	cci_config.slave_enable                          = 0x1;                  //!slave 1 enable
	cci_config.early_bus_termination                 = 0;                    //!early bus termination if enable ,1
	cci_config.address_width_config                  = _40BIT_ADDRESS_WIDTH; //!address width config
	cci_config.interface_width                       = CCI_QUAD_MODE;        //!cci interface width
	cci_config.translation_enable                    = 1;                    //!enable translation bit
	cci_config.slave_lsb_address[0]                  = SLAVE_LSB_ADDRESS;    //!slave lsb address to be load
	cci_config.slave_msb_address[0]                  = SLAVE_MSB_ADDRESS;    //!slave msb address to be load
	cci_config.translation_address                   = TRANSLATION_ADDRESS;  //!cci translation address
	cci_config.slave_timeout                         = SLAVE_TIMEOUT;        //!slave timeout value to be programmable
	cci_config.cci_cntrl_en                          = 1;                    //!enable this bit for cci controller enable   
	
			/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/* Enables the CCI in the master mode(AMS enable). */ 
	RSI_CCI_AmsEnable();
	
	/*  Configure the CCI clocks. */
	RSI_CLK_CciClkConfig(M4CLK ,CCI_M4_SOC_CLK_FOR_OTHER_CLKS ,1,ENABLE_STATIC_CLK);

	/* Enables pad selection bits,enables receive_enable for the cci pins and 
         configures the pins for CCI.(enable gpio muxing ) */
	CCI_PinMux();

	/* Configures the CCI peripheral by initializing the CCI configuration structure 
       members */
	RSI_CCI_AMS_Initialise(&cci_config);

	//!write data(0x12345678)  to slave memory(0x6000 location)	
	  
	 Write_Data(0x6000,0x12345678); 

	//!read data from slave memory
	/*   Read the data from  slave at the address is (offset(0x60000000)+0x6000) and assigns it to "read" variable.	 */
	read=Read_Data(0x6000);
 
 /* while() for ever where sleep until the interrupt occurs  */
  while(1) 
	{
		   /* Prints on hyper-terminal as "Core is Sleeping...\n" */
		DEBUGOUT("Core is Sleeping...\n");
		
			/*wait here  till interrupt occurs*/
		__WFI();
	}
}
