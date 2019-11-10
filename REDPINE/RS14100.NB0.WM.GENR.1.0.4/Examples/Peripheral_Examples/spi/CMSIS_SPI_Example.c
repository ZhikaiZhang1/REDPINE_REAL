#include "SPI.h"
#include "RTE_Device.h" 
#include "rsi_board.h"

#define  BUFFER_SIZE      1024      //Number of data to be sent through SPI
#define	 SPI_BAUD					12000000  //speed at which data transmitted through SPI
#define  SPI_BIT_WIDTH		16				//SPI bit width can be 16/8 for 16/8 bit data transfer 

/* SPI Driver */
extern ARM_DRIVER_SPI Driver_SSI_SLAVE;
uint8_t spi_done = 1;

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
		spi_done=1;  /* Error: Call debugger or replace with custom error handling */
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
ARM_SPI_STATUS spi_status;

int main(void)
{
    
  uint16_t  i = 0;
	int err_bit = 0;
	ARM_DRIVER_SPI* SPIdrv = &Driver_SSI_SLAVE;
 
 	SystemCoreClockUpdate();
  
  
  for(i=0;i<BUFFER_SIZE;i++)
  {
		if (i < 50)
     testdata_out[i]=i;
		else
			testdata_out[i] = 50;
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
	err_bit = SPIdrv->Control(ARM_SPI_MODE_SLAVE | ARM_SPI_SS_SLAVE_HW | ARM_SPI_CPOL0_CPHA0  | ARM_SPI_DATA_BITS(SPI_BIT_WIDTH), SPI_BAUD);	 
	if (err_bit != 0){
		
		while(1){
			
		}
	}
  
  /* SS line = ACTIVE = LOW Used to be here for master, but now slave*/
  
	err_bit = SPIdrv->Transfer(testdata_out, testdata_in, BUFFER_SIZE);
	
	if (err_bit != 0){
		
		while(1){
			
		}
	}
	
  /* Waits until spi_done=0 */
		
  //spi_done = 0;
  
  /* SS line = ACTIVE = LOW */
  //SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
  
  /*for(i=0;i<BUFFER_SIZE;i++)
  {
    if(testdata_out[i]==testdata_in[i])
    {
      continue; 
    }
    else
    {
       break; 
    }
  }*/
	while(1){
		//while (!spi_done);
		spi_status = SPIdrv -> GetStatus();
		if (spi_done == 1 || spi_status.busy == 0){
			spi_done = 0;
			SPIdrv->Transfer(testdata_out, testdata_in, BUFFER_SIZE);
			
		}
		/*for(i=0;i<BUFFER_SIZE;i++)
		{
			if(testdata_out[i]==testdata_in[i])
			{
				continue; 
			}
			else
			{
				//break; 
			}
		}*/
	}
}
