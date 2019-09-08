/**
 * @file       main.c
 * @version    0.1
 * @date       20 jan 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief EGPIO interrupt Example
 *
 * @section Description
 * This file contains the example usgae of EGPIO peripheral.
 *
 */


/**
 * Includes
 */

#include "rsi_chip.h"



#define SPI_BUFFSENDSIZE 255
volatile int32_t result;

/*Global buffers to hold the TX and RX data */
uint16_t u16SpiTxBuf[SPI_BUFFSENDSIZE] , u16SpiRxBuf[SPI_BUFFSENDSIZE];

volatile uint32_t spi_tx_dne= 0 , spi_rx_dne = 0;

/*Configuration and Transfer structure*/
stc_sio_spi_cfg_t  pstcSpiConfig ={0};
stc_sio_spi_xfer_t pstcSpiXfer ={0};

/*SIO-SPI Configuration parameters */
#define SIO_SPI_CLK_FREQUENCY  1000000 // 5Mhz
#define SIO_SPI_CLK_CH         SioCh1
#define SIO_SPI_MOSI_CH        SioCh3
#define SIO_SPI_MISO_CH        SioCh2
#define SIO_SPI_BIT_LEN        16
#define SIO_SPI_CS_CH          SioCh0
#define SIO_SPI_MODE           SioSpiMode3


static void RSI_SIO_PinInit(void)
{
  RSI_EGPIO_PadSelectionEnable(1);
	RSI_EGPIO_PadReceiverEnable(6);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , 6 ,  EGPIO_PIN_MUX_MODE1);  // SIO 0
//	RSI_EGPIO_PadSelectionEnable(9);
//	RSI_EGPIO_PadReceiverEnable(36);
	RSI_EGPIO_PadSelectionEnable(1);
	RSI_EGPIO_PadReceiverEnable(7);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , 7 , EGPIO_PIN_MUX_MODE1);  // SIO 1
	RSI_EGPIO_PadSelectionEnable(1);
	RSI_EGPIO_PadReceiverEnable(8);	
	RSI_EGPIO_SetPinMux(EGPIO ,0 , 8 , EGPIO_PIN_MUX_MODE1);  // SIO 2
	RSI_EGPIO_PadSelectionEnable(1);
	RSI_EGPIO_PadReceiverEnable(9);	
	RSI_EGPIO_SetPinMux(EGPIO ,0 , 9 ,  EGPIO_PIN_MUX_MODE1);  // SIO 3	
	return ;
}


void SioSpiCbFn(en_sio_spi_events_t en_event)
{
	switch(en_event)
	{
	/*Handle SPI TX done event here */
	case  SioSpiTxDone :
		spi_tx_dne++;
		break;

		/*Handle SPI RX done event here */
	case  SioSpiRxDone :
		spi_rx_dne++;
		break;
	}
	return ;
}

/* main function (C entry point) */
int main(void)
{
 volatile  uint8_t i;
	uint16_t cnt;
	/*SPI configuration*/
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	pstcSpiConfig.u32SpiClockFrq = SIO_SPI_CLK_FREQUENCY;

	/*Default MSB First */
	pstcSpiConfig.u8BitOrder     = 1;
	pstcSpiConfig.u8SpiClkCh     = SIO_SPI_CLK_CH ;
	pstcSpiConfig.u8SpiMosiCh    = SIO_SPI_MOSI_CH;
	pstcSpiConfig.u8SpiMisoCh    = SIO_SPI_MISO_CH;
	pstcSpiConfig.u8SpiCsCh      = SIO_SPI_CS_CH;
	pstcSpiConfig.u8BitLen       = SIO_SPI_BIT_LEN;
	pstcSpiConfig.u8Mode         = SIO_SPI_MODE;

	/* Enable the SIO pin muxing*/
	RSI_SIO_PinInit();

	/* Enable the SIO*/
	RSI_SIO_Init(SIO);

	/*Initialize the SIO-SPI */
	RSI_SIO_InitSpi(SIO , &pstcSpiConfig);

	/*Enable the SIO NIV Interrupt */
	NVIC_EnableIRQ(SIO_IRQn);

	while(1)
	{
		/* Populate some TX data and clear RX data */
		for (cnt = 0; cnt < SPI_BUFFSENDSIZE; cnt++) {
			u16SpiTxBuf[cnt] = cnt + 1 + ((cnt + 1) << 8);
			u16SpiRxBuf[cnt] =  0;
		}

		/*SPI Transfer configuration */
		pstcSpiXfer.rxBuff           = u16SpiRxBuf;
		pstcSpiXfer.rxCount          = SPI_BUFFSENDSIZE ;
		pstcSpiXfer.sselNum          = 0;
		pstcSpiXfer.txBuff           = u16SpiTxBuf;
		pstcSpiXfer.txCount          = SPI_BUFFSENDSIZE ;
		pstcSpiXfer.u8BitLen         = SIO_SPI_BIT_LEN;
		pstcSpiXfer.u8Status         = SioSpiIdle;
		pstcSpiXfer.pfnCb            = SioSpiCbFn;
    
		/* Assert the SPI master chip select */
		RSI_SIO_SpiCsAssert(SIO,pstcSpiConfig.u8SpiCsCh);
   
		/*  Make the SIO-SPI Transfer in non blocking mode. */
		RSI_SIO_SpiTrasnfer(SIO , &pstcSpiXfer);
     
		/*Wait for TX completion*/
	  while(!spi_tx_dne);  
		spi_tx_dne =0;
		 
		 /* Deassert the SPI master chip select */
		RSI_SIO_SpiCsDeAssert(SIO,pstcSpiConfig.u8SpiCsCh);
					
		for(i=0;i<SPI_BUFFSENDSIZE;i++)
			{
				if(u16SpiRxBuf[i]==u16SpiTxBuf[i])
				{
					continue;
				}
				else
				{
					break;
				}
			}
			memset(u16SpiRxBuf ,  0x00, sizeof(u16SpiRxBuf));
		if(i==SPI_BUFFSENDSIZE)	
		{
			result=0;
		}
		else
    {
      result=-1;    
		}	
   cnt++;		
 }

}



