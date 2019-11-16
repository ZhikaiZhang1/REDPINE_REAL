#include "SPI.h"
#include "RTE_Device.h" 
#include "rsi_board.h"
#include "rsi_chip.h"

#define  BUFFER_SIZE      1024      //Number of data to be sent through SPI
#define	 SPI_BAUD					1000000  //speed at which data transmitted through SPI
#define  SPI_BIT_WIDTH		8				//SPI bit width can be 16/8 for 16/8 bit data transfer 

#define PININT_IRQ_HANDLER         IRQ058_Handler                   /* GPIO interrupt IRQ function name            */
#define PININT_NVIC_NAME           EGPIO_PIN_6_IRQn                 /* GPIO interrupt NVIC interrupt name          */
#define M4_GPIO_PORT               0                                /* GPIO port number                            */
#define M4_GPIO_PIN                14                               /* GPIO pin number                             */
#define PIN_INT                    6

/* SPI Driver */
extern ARM_DRIVER_SPI Driver_SSI_MASTER;
volatile bool spi_receive_event = false;
volatile uint8_t spi_done = 0;

//interrupt handler
void PININT_IRQ_HANDLER(void)
{
	uint32_t gintStatus;

	/*get interrupt status*/
	gintStatus=RSI_EGPIO_GetIntStat(EGPIO,PIN_INT);

	if((gintStatus &EGPIO_PIN_INT_CLR_RISING ))// || (gintStatus &EGPIO_PIN_INT_CLR_FALLING ))
	{
		/*clear interrupt*/
		RSI_EGPIO_IntClr(EGPIO, PIN_INT ,INTERRUPT_STATUS_CLR);
		spi_receive_event = true;
	}
	else
	{
		RSI_EGPIO_IntMask(EGPIO , PIN_INT);
	}
	return ;
}

static void Set_Up_INT(void){
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	
	/*Enable clock for EGPIO module*/
	RSI_CLK_PeripheralClkEnable(M4CLK,EGPIO_CLK,ENABLE_STATIC_CLK);

	/*PAD selection*/
	RSI_EGPIO_PadSelectionEnable(1);

	/*REN enable */
	RSI_EGPIO_PadReceiverEnable(M4_GPIO_PIN);

	/*Configure default GPIO mode(0) */
	RSI_EGPIO_SetPinMux(EGPIO,M4_GPIO_PORT ,M4_GPIO_PIN,EGPIO_PIN_MUX_MODE0);

	/*Selects the pin interrupt for the GPIO*/
	RSI_EGPIO_PinIntSel(EGPIO, PIN_INT , M4_GPIO_PORT, M4_GPIO_PIN);

	/*Configures the edge /level interrupt*/
	RSI_EGPIO_SetIntLowLevelEnable(EGPIO,PIN_INT);

	/*Unmask the  interrupt*/
	RSI_EGPIO_IntUnMask(EGPIO , PIN_INT);

	/*NVIC enable */
	NVIC_EnableIRQ(PININT_NVIC_NAME);
	//return;
}
void mySPI_callback(uint32_t event)
{
	switch (event)
	{
	case ARM_SPI_EVENT_TRANSFER_COMPLETE:
		spi_done=1;
		break;
	case ARM_SPI_EVENT_DATA_LOST:
		/*  Occurs in slave mode when data is requested/sent by master
            but send/receive/transfer operation has not been started
            and indicates that data is lost. Occurs also in master mode
            when driver cannot transfer data fast enough. */
		__breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
		break;
	case ARM_SPI_EVENT_MODE_FAULT:
		/*  Occurs in master mode when Slave Select is deactivated and
            indicates Master Mode Fault. */
		__breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
		break;
	}
}

/* Test data buffers */
uint8_t testdata_out[BUFFER_SIZE]; 
uint16_t testdata_in [BUFFER_SIZE];

int main(void)
{
    
  uint16_t  i = 0;
	ARM_DRIVER_SPI* SPIdrv = &Driver_SSI_MASTER;
 
 	SystemCoreClockUpdate();
  
  
  for(i=0;i<BUFFER_SIZE;i++)
  {
     testdata_out[i]=102;
  }
   /*program intf pll to 180MHZ*/
  SPI_MEM_MAP_PLL(INTF_PLL_500_CTRL_REG9) = 0xD900 ;   
  RSI_CLK_SetIntfPllFreq(M4CLK,180000000,40000000);
  
  /*Configure m4 soc to 180mhz*/
  RSI_CLK_M4SocClkConfig(M4CLK,M4_INTFPLLCLK,0);

  /*configure socpll to 20mhz*/  
  RSI_CLK_SocPllLockConfig(1,1,0xA4);
  RSI_CLK_SetSocPllFreq(M4CLK,20000000,40000000);
	
  /* Initialize the SPI driver */
	SPIdrv->Initialize(mySPI_callback);
	
  /* Power up the SPI peripheral */
	SPIdrv->PowerControl(ARM_POWER_FULL);
  
	/* Configure the SPI to Master, 16-bit mode @10000 kBits/sec */
	SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_SS_MASTER_HW_OUTPUT | ARM_SPI_DATA_BITS(SPI_BIT_WIDTH), SPI_BAUD);	 
  Set_Up_INT();
	while (1){
		if (spi_receive_event){
		while (!spi_done);	
		  spi_done = 0;
			SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE); 
			
		  SPIdrv->Receive(testdata_in, BUFFER_SIZE);
			while (!spi_done);	
		  spi_done = 0;
			SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
			//spi_receive_event = false;
		}
		
		/* SS line = ACTIVE = LOW */
		SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE); 
		
		SPIdrv->Transfer(testdata_out, testdata_in, BUFFER_SIZE);
		
		/* Waits until spi_done=0 */
		while (!spi_done){
		}
		spi_done = 0;
		
		/* SS line = ACTIVE = LOW */
		SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
		if(spi_receive_event){
			spi_receive_event = false;
			for(i=0;i<BUFFER_SIZE;i++)
			{
				
				if(testdata_out[i]==testdata_in[i])
				{
					//continue; 
					//do this later
				}
				/*else
				{
					 break; 
				}*/
				
			}
		RSI_EGPIO_IntUnMask(EGPIO , PIN_INT);
	}
}
//while(1);
	
}
