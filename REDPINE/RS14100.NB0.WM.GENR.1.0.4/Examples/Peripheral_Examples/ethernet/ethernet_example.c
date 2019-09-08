
/** @file main.c
 * 
 *  @brief This file contains functions regarding hardware 
 *     initialization ans main function. 
 * 
 *
 * Copyright (c) RSI India Pvt Ltd. 
 * All rights reserved
 *
 * This software embodies materials and concepts which are 
 * confidential to RSI and is made available solely pursuant 
 * to the terms of a written license agreement with RSI.
 */

#include "rsi_chip.h"
#include "rsi_ethernet.h"
 
#define LOOPBACK_MODE 0

#define MISC_BASE_ADDR                    0x46008000
#define MISC_CFG_HOST_INTR_SET_OFFSET     (0x04)
#define HOST_INTR_STATUS_REG              *(volatile uint16_t *)(MISC_BASE_ADDR + MISC_CFG_HOST_INTR_SET_OFFSET)


#define CLK_BIT     (*(volatile uint32_t *)0x46000000)

#define CRC_ENABLE   1
#define PMT_INTERRUPT_IRQ        ETHERNET_IRQn 
 
#define DMA_INTERRUPT_IRQ        ETHERNET_PMT_IRQn 

//! Set default PMT IRQ handler
#define RSI_PMT_IRQ_HANDLER     ETH_PMT_IRQHandler

//! Set default DMA IRQ handler
#define RSI_DMA_IRQ_HANDLER     ETH_DMA_IRQHandler


//! Global memory
#define DRIVER_MEM               100
#define BUFFER_SIZE             2000

/*ETHERNET*/
#define RMII_MDC               11
#define RMII_MDC_PAD           2
#define RMII_MDC_MODE          EGPIO_PIN_MUX_MODE7

#define RMII_MDO               12
#define RMII_MDO_PAD           3
#define RMII_MDO_MODE          EGPIO_PIN_MUX_MODE7

#define RMII_REF_CLK           8 
#define RMII_REF_CLK_PAD       1 
#define RMII_REF_CLK_MODE      EGPIO_PIN_MUX_MODE14

#define RMII_CRS_DV            16
#define RMII_CRS_DV_PAD        5 
#define RMII_CRS_DV_MODE       EGPIO_PIN_MUX_MODE14

#define RMII_RXD0              15
#define RMII_RXD0_PAD          5
#define RMII_RXD0_MODE         EGPIO_PIN_MUX_MODE7

#define RMII_RXD1              10 
#define RMII_RXD1_PAD          2 
#define RMII_RXD1_MODE         EGPIO_PIN_MUX_MODE7

#define RMII_TXEN              9 
#define RMII_TXEN_PAD          1 
#define RMII_TXEN_MODE         EGPIO_PIN_MUX_MODE14 

#define RMII_TXD1              6 
#define RMII_TXD1_PAD          1
#define RMII_TXD1_MODE         EGPIO_PIN_MUX_MODE14 

#define RMII_TXD0              7 
#define RMII_TXD0_PAD          1 
#define RMII_TXD0_MODE         EGPIO_PIN_MUX_MODE14 


uint8_t received = 1;

//! transmit buffer 
uint8_t tx_buf[BUFFER_SIZE];
uint8_t tx_buf1[BUFFER_SIZE];
uint8_t cnt, first; 
//! Receive buffer 
uint8_t rx_buf[BUFFER_SIZE] __attribute__((aligned (32)));
uint8_t rx_buffer1[BUFFER_SIZE] __attribute__((aligned (32)));

uint32_t driver_memory[DRIVER_MEM]; 

RSI_ETH_HANDLE_T eth_handle;

////! TX DMA descriptor  
RSI_ETH_DMA_TXDESC_t tx_desc[2] __attribute__((aligned (16)));
RSI_ETH_DMA_TXDESC_t tx_desc1[2] __attribute__((aligned (16)));
//! RX DMA descriptor
RSI_ETH_DMA_RXDESC_t rx_desc[2]__attribute__((aligned (16)));

RSI_ETH_DMA_TxXferConfig_t tx_dma_config;

RSI_ETH_DMA_RxXferConfig_t rx_dma_config;

void IRQ063_Handler()
{
	//! This occurs when MGMT frame is received or Wakeup frame is received
  //! Call the PMT interrupt handler 
  RSI_ETH_PMTIntrHandler(eth_handle);
}


void IRQ062_Handler()
{
  //! Call DMA interrupt handler
  RSI_ETH_DMAIntrHandler(eth_handle);
}

/*!< Function call back when PMT interrupt is generated*/
void rsi_pmt_cb_handler(RSI_ETH_HANDLE_T pHandle, uint32_t event,void *arg)
{
	switch(event)
	{
    case ETH_EVENT_WAKEUP_FRAME_RECVD:
      //!This event occurs when wakeup frame is received 
      break;
    case ETH_EVENT_MAGIC_PACKET_RECVD:
      //! This event occurs when magic packet is received  
      break;
    default:
      break;
  }
}

/*!< Function call back when event is raised*/
void rsi_dma_event_cb_handler(RSI_ETH_HANDLE_T pHandle, uint32_t event, void *arg)
{
  switch(event)
  {
    case ETH_EVENT_TI:     
		{
			cnt=1;
			/*!< This event indicates that frame 
        transmission is finished and TDES1[31] is set in the First Descriptor*/
		}
      break;
    case ETH_EVENT_TPS:   
		{
			/*!< This event is set when the transmission is stopped  */
		}
      break;
    case ETH_EVENT_TU:    
		{			
			  /*!< This event indicates that the Next Descriptor in the Transmit List is owned by
        the host and cannot be acquired by the DMA. Transmission is suspended*/
		}
      break;
    case ETH_EVENT_TJT:  
		{		
	      /*!< This event indicates that the Transmit Jabber Timer expired, meaning that the
        transmitter had been excessively active*/																				 
		}
     break;
    case ETH_EVENT_OVF:    
		{
			  /*!< This event indicates that the Receive Buffer had an Overflow during frame
        reception. */																					
		}
      break;
    case ETH_EVENT_UNF:
		{
			 /*!< This event indicates that the Transmit Buffer had an Underflow during frame
        transmission*/     																					
		}
      break;

    case ETH_EVENT_RI:     
		{
   /* This bit indicates the completion of frame reception. Specific frame status
      information has been posted in the descriptor. Reception remains in the
       Running state.																		*/
				received = 0;
		}
      break;

    case ETH_EVENT_RU:   
		{		
			  /*!< This event indicates that the Next Descriptor in the Receive List is owned by the
        host and cannot be acquired by the DMA*/																					
		}
      break;

    case ETH_EVENT_RPS:    
		{
			 /*!< This event is asserted when the Receive Process enters the Stopped state*/ 
		}
      break;
    case ETH_EVENT_RWT:    
		{
			 /*!< This bit is asserted when a frame with a length greater than 2,048 bytes is
        received*/
		}
      break;
    case ETH_EVENT_ETI:    
		{
        /*!< This bit indicates that the frame to be transmitted was fully transferred to the
        MTL Transmit FIFO*/																							
		}
      break;
    case ETH_EVENT_BUS_ERR:
		{
      /*!< BUS Error*/																							 																							 
		}
      break;
    case ETH_EVENT_ERI:    
		{	
      /*!< This event indicates that the DMA had filled the first data buffer of the packet */ 
      break;
		}
    default:
      break;
  }
}
 
void RSI_ETH_InitClock()
{
	//! Enable clock to Ethernet peripheral
	CLK_BIT |= (1 << 21);
	
	return;
}

void rsi_eth_setuptx()
{
  int32_t retval;
		//TCP packet
	uint8_t temp_buffer[] = {0xF0,0x4D,0xA2,0xBf,0x69,0xb5,0x11, 0x22, 0x33, 0x44, 0x55, 0x66,0x08,0x06,0x00,0x01,0x08,0x00,0x06,0x04,0x00,0x01,0x12, 0x22, 0x33, 0x34, 0x55, 0x66,0xc0,0xa8,0x46,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xa8,0x46,0x01};
		
  memcpy(tx_buf, temp_buffer, sizeof(temp_buffer));
	
	
	#if CRC_ENABLE

	tx_dma_config.checksum = 1;
	tx_dma_config.dc  =  0;
 #else
  tx_dma_config.checksum = 0;
	tx_dma_config.dc  =  1;
 #endif
	tx_dma_config.eof = 1;
	tx_dma_config.fireDescCB = 1;
	tx_dma_config.sof = 1;
	tx_dma_config.src = &tx_buf;
	tx_dma_config.xferCount = 1000;
	
	retval = RSI_ETH_MAC_BuildTXDescChain(eth_handle, &tx_dma_config, &tx_desc[0], NULL);

//! Error check
	if(retval != 0)
	{
		while(1);
	}
}	

void rsi_eth_setuprx()
{
	int32_t retval;
	memset(rx_buf,0,sizeof(rx_buf));
	rx_dma_config.dst = &rx_buf;
	rx_dma_config.fireDescCB = 1;
	rx_dma_config.recvCount = 1000;//1500;
	
	//! Build descriptor chain RX
	retval = RSI_ETH_MAC_BuildRXDescChain(eth_handle, &rx_dma_config, &rx_desc[0], &rx_desc[1]);
	//! Error check
	if(retval != 0)
	{
		while(1);
	}
	
}

#define ETHERNET_BASE_ADDR_OFF_0 *(volatile uint32_t *)0x20240000
#define ETHERNET_BASE_ADDR_OFF_4 *(volatile uint32_t *)(0x20240000+0x0004)
#define ETHERNET_BASE_ADDR_OFF_1018 *(volatile uint32_t *)(0x20240000+0x1018)
#define GMMI_ADDR_REG *(volatile uint32_t *)(0x20240000+0x10)
#define MAC_ADDR0_HIGH *(volatile uint32_t *)(0x20240000+0x0040)
#define MAC_ADDR0_LOW *(volatile uint32_t *)(0x20240000+0x0044)
#define MAC_FILTER_REG *(volatile uint32_t *)(0x20240000+0x0004)
#define ETH_CFG_REG   *(volatile uint32_t *)(0x46008000+0x40)

/*!< Main function*/
	volatile uint32_t read;
	volatile uint32_t r;
  uint8_t toggle;


void M4_ETHERNET_PinMux()
{
	RSI_EGPIO_PadSelectionEnable(RMII_MDC_PAD);
	RSI_EGPIO_PadReceiverEnable(RMII_MDC);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , RMII_MDC ,RMII_MDC_MODE);

	RSI_EGPIO_PadSelectionEnable(RMII_MDO_PAD);
	RSI_EGPIO_PadReceiverEnable(RMII_MDO);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , RMII_MDO ,RMII_MDO_MODE);

	RSI_EGPIO_PadSelectionEnable(RMII_REF_CLK_PAD);
	RSI_EGPIO_PadReceiverEnable(RMII_REF_CLK);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , RMII_REF_CLK ,RMII_REF_CLK_MODE);

	RSI_EGPIO_PadSelectionEnable(RMII_CRS_DV_PAD);
	RSI_EGPIO_PadReceiverEnable(RMII_CRS_DV);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , RMII_CRS_DV ,RMII_CRS_DV_MODE);

	RSI_EGPIO_PadSelectionEnable(RMII_RXD0_PAD);
	RSI_EGPIO_PadReceiverEnable(RMII_RXD0);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , RMII_RXD0 ,RMII_RXD0_MODE);
	
	RSI_EGPIO_PadSelectionEnable(RMII_RXD1_PAD);
	RSI_EGPIO_PadReceiverEnable(RMII_RXD1);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , RMII_RXD1 ,RMII_RXD1_MODE);

	RSI_EGPIO_PadSelectionEnable(RMII_TXEN_PAD);
	RSI_EGPIO_PadReceiverEnable(RMII_TXEN);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , RMII_TXEN ,RMII_TXEN_MODE);

	RSI_EGPIO_PadSelectionEnable(RMII_TXD1_PAD);
	RSI_EGPIO_PadReceiverEnable(RMII_TXD1);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , RMII_TXD1 ,RMII_TXD1_MODE);

	RSI_EGPIO_PadSelectionEnable(RMII_TXD0_PAD);
	RSI_EGPIO_PadReceiverEnable(RMII_TXD0);
	RSI_EGPIO_SetPinMux(EGPIO ,0 , RMII_TXD0 ,RMII_TXD0_MODE);
}

int main()
{
		/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/* Initialize GPIO pins required for Ethernet */
	M4_ETHERNET_PinMux();
	
	/* Clock to Ethernet peripheral */
	RSI_ETH_InitClock();
	
	//100 mbps set bit
  
	ETH_CFG_REG|=(0x1<<4) ;
  
	//! Init Ethernet MAC layer
  eth_handle = RSI_ETH_MAC_Initialize(&driver_memory, ETHERNET_BASE);
	
	//! registering event callback for 
	RSI_ETH_MAC_RegisterCallBacks(eth_handle,GMAC_INTERRUPT,rsi_pmt_cb_handler);	

  //! registering e callback
	RSI_ETH_MAC_RegisterCallBacks(eth_handle,DMA_INTERRUPT,rsi_dma_event_cb_handler);	

  //! DMA initialization
	RSI_ETH_MAC_DMAInit(eth_handle);

  //! Enable the required DMA interrupts	
	//! Enable Normal ,abnormal interrupt summary bits,
	//! transmit ,receive and receive buffer unavailable interrupts 
	RSI_ETH_DMA_EnableInt(eth_handle,(DMA_ERE_INTR|DMA_AIE_INTR|DMA_TIE_INTR|DMA_NIE_INTR|DMA_RUE_INTR|DMA_RIE_INTR|DMA_OVE_INTR|DMA_TIE_INTR));
  
	NVIC_EnableIRQ( ETHERNET_IRQn);
	
	NVIC_EnableIRQ( ETHERNET_PMT_IRQn);
	
  //! Set up RX descriptors
	rsi_eth_setuprx();
  //! Set up TX descriptors
	rsi_eth_setuptx();
	
  MAC_FILTER_REG |=((0x1U<<0)|(0x1U<<31));
	GMMI_ADDR_REG |=0xD;
	while(GMMI_ADDR_REG & BIT(0)==1);
	read=(*(volatile uint32_t *)(0x20240000+0x0014));
	MAC_ADDR0_HIGH |=0x0202;
	MAC_ADDR0_LOW  |=0x02020201;

 #if LOOPBACK_MODE
  ETHERNET_BASE_ADDR_OFF_0 |=0x00C0980C;
  #else
	ETHERNET_BASE_ADDR_OFF_0 |=0x00C0880C;
  #endif
  //crc_en
	ETHERNET_BASE_ADDR_OFF_0 |=0x00C08C0C;

  //! Set opermode options for DMA
	RSI_ETH_MAC_SetOPerMode(eth_handle, (DMA_RSF_ENABLE|DMA_TSF_ENABLE));

	//! Trigger DMA to start RX
	RSI_ETH_MAC_StartRx(eth_handle, &rx_desc[0]);

	while(1)
	{		 
		if(!first)
		{
			toggle = 1;
			
			/* Set up the transmission buffer */
			rsi_eth_setuptx();
			
			/*  Indicate signal to dma to start the TX transmission of frame  */
			RSI_ETH_MAC_StartTx(eth_handle,&tx_desc[0]);
			first = 1;
		}
		if(cnt)
		{
			/* Indicate signal to DMA to stop the TX transmission of frame  */
			RSI_ETH_MAC_StopTx(eth_handle);
			
			/* Set up the transmission buffer */
			rsi_eth_setuptx();
			
			/*  Indicate signal to DMA to start the TX transmission of frame  */
			RSI_ETH_MAC_StartTx(eth_handle,&tx_desc[0]);
			cnt = 0;
		}
	}
	
}

