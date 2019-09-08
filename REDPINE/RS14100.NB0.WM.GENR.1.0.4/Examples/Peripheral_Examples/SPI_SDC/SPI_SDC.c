
#include "SPI.h"
#include "RTE_Device.h" 
#include "rsi_board.h"

#define  BUFFER_SIZE       96     //Number of data to be sent through SPI
#define	 SPI_BAUD					 5000000 //speed at which data transmitted through SPI
#define  SPI_BIT_WIDTH		 16			 //SPI bit width can be 16/8 for 16/8 bit data transfer 

#define NPSS_GPIO_PIN           3

/* SPI Driver */
extern ARM_DRIVER_SPI Driver_SSI_ULP_MASTER;
volatile uint8_t spi_done = 0;

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
uint16_t testdata_in  [BUFFER_SIZE] __attribute__ ((at(0x24060800)));

/*DS TIMER WAKE UP*/
void IRQ027_Handler(void)
{
	uint32_t wakeUpSrc = 0 , ReadCommIntrSts=0;
	/*Get the wake up source */
	wakeUpSrc = RSI_PS_GetWkpUpStatus();
	/*Deep sleep timer interrupt */
	RSI_PS_ClrWkpUpStatus (NPSS_TO_MCU_DST_INTR);
	return ;
}

volatile uint32_t dbg_toggle = 0,ax=0;

int main(void)
{
	uint16_t transfer_len = 0;

	
	ARM_DRIVER_SPI* SPIdrv = &Driver_SSI_ULP_MASTER;

	ARM_DRIVER_VERSION   version;

	version = SPIdrv->GetVersion();
	if (version.api < 0x200) /* requires at minimum API version 2.00 or higher */
	{                        /* error handling                                 */
		return version.api;
	}

	hardware_setup();
//	*(volatile uint32_t *)(0x24048624) = 0x6;
//	*(volatile uint32_t *)(0x24048610) = ( 0x11110000);

//	RSI_NPSSGPIO_InputBufferEn(NPSS_GPIO_PIN , 1U);
//	RSI_NPSSGPIO_SetPinMux(NPSS_GPIO_PIN , 0);
//	RSI_NPSSGPIO_SetDir(NPSS_GPIO_PIN , NPSS_GPIO_DIR_OUTPUT);
	
  while(1){
//		*(volatile uint32_t *)(0x24048624) = 0x20;
		/* Un-Initialize the SPI driver */
		SPIdrv->Uninitialize();
		/* Initialize the SPI driver */
		SPIdrv->Initialize(mySPI_callback);
		/* Power up the SPI peripheral */
		SPIdrv->PowerControl(ARM_POWER_FULL);
		/* Configure the SPI to Master, 8-bit mode @10000 kBits/sec */
		SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_SS_MASTER_HW_OUTPUT | ARM_SPI_DATA_BITS(SPI_BIT_WIDTH), SPI_BAUD);
		if(SPI_BIT_WIDTH == 16)
			transfer_len = (sizeof(testdata_in)/sizeof(uint16_t));
		else
			transfer_len = sizeof(testdata_in);

		/* Receive some data */
		SPIdrv->Transfer((uint16_t *)0x24061000, testdata_in, transfer_len);
		
		RSI_PS_SetWkpSources(ULPSS_BASED_WAKEUP);
		RSI_PS_ClrWkpSources(DST_BASED_WAKEUP);
		
		/*Configure the wake up paramters for boot loader */
		RSI_PS_RetentionSleepConfig(0 , (uint32_t)RSI_PS_RestoreCpuContext , 0 , 4);
	  /*Goto Sleep with retention */
		RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,DISABLE_LF_MODE);
	
		//	*(volatile uint32_t *)(0x24048624) = 0x0;
		while (!spi_done);
		spi_done = 0;
    
//		dbg_toggle ^= 1;
//		RSI_NPSSGPIO_SetPin(NPSS_GPIO_PIN, dbg_toggle);
		
		
		RSI_PS_SetWkpSources(DST_BASED_WAKEUP);
		RSI_PS_ClrWkpSources(ULPSS_BASED_WAKEUP);
	 // *(volatile uint32_t*)(0x413)
		/*Configure the wake up paramters for boot loader */
		RSI_PS_RetentionSleepConfig(0 , (uint32_t)RSI_PS_RestoreCpuContext , 0 , 4);
	  /*Goto Sleep with retention */
		RSI_PS_EnterDeepSleep(SLEEP_WITH_RETENTION,DISABLE_LF_MODE);
		
		
		
		
	}

}
