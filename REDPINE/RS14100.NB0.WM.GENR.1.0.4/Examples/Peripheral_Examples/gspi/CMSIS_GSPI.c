 #include "GSPI.h"
#include "RTE_Device.h" 
#include "rsi_board.h"

#define  BUFFER_SIZE      1024      //Number of data to be sent through SPI
#define	 SPI_BAUD					1000000  //speed at which data transmitted through SPI
#define  SPI_BIT_WIDTH		16				//SPI bit width can be 16/8 for 16/8 bit data transfer 

/* SPI Driver */
extern ARM_DRIVER_SPI Driver_GSPI_MASTER;
uint8_t spi_done = 0;

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
uint16_t testdata_out[BUFFER_SIZE]; 
uint16_t testdata_in [BUFFER_SIZE];

int main(void)
{

	uint16_t  i = 0;
	ARM_DRIVER_SPI* SPIdrv = &Driver_GSPI_MASTER;

	SystemCoreClockUpdate();

	for(i=0;i<BUFFER_SIZE;i++)
	{
		testdata_out[i]=i+1;
	}

	/*program intf pll to 180MHZ*/
	SPI_MEM_MAP_PLL(INTF_PLL_500_CTRL_REG9) = 0xD900 ;   
	RSI_CLK_SetIntfPllFreq(M4CLK,180000000,40000000);

	/*Configure m4 soc to 180mhz*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_INTFPLLCLK,0);

	/*configure socpll to 20mhz*/  
	RSI_CLK_SocPllLockConfig(1,1,0xA4);
	RSI_CLK_SetSocPllFreq(M4CLK,20000000,40000000);

  #ifdef DEBUG_UART
  DEBUGINIT();
  #endif
	/* Initialize the SPI driver */
	SPIdrv->Initialize(mySPI_callback);

	/* Power up the SPI peripheral */
	SPIdrv->PowerControl(ARM_POWER_FULL);

	/* Configure the SPI to Master, 16-bit mode @10000 kBits/sec */
	SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_SS_MASTER_HW_OUTPUT | ARM_SPI_DATA_BITS(SPI_BIT_WIDTH), SPI_BAUD);	 

	/* SS line = ACTIVE = LOW */
	SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE); 

	SPIdrv->Transfer(testdata_out, testdata_in, sizeof(testdata_out));

	/* Waits until spi_done=0 */
	while (!spi_done);	
	spi_done = 0;

	/* SS line = ACTIVE = LOW */
	SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);

	for(i=0;i<BUFFER_SIZE;i++)
	{
		if(testdata_out[i]==testdata_in[i])
		{
			//DEBUGOUT("Transmission Successful");
		}
		else
		{
			//DEBUGOUT("Transmission failed");
		}
	}

	while(1);

}
