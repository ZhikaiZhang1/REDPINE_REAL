 /**
 * @file       usart_master.c
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
 * This file contains example code to demonstrate the USART Master functionality.
 */
/* Includes -------------------------------------------------------------------------------------------------------*/
#include "USART.h"
#include "rsi_board.h"

/* Private typedef ------------------------------------------------------------------------------------------------*/

/* Private define -------------------------------------------------------------------------------------------------*/

/* Private macro --------------------------------------------------------------------------------------------------*/
#define BUFFER_SIZE                     1024

#define BAUD_VALUE                      9600

extern ARM_DRIVER_USART Driver_USART0;

static ARM_DRIVER_USART * USARTdrv   =  &Driver_USART0;

ARM_USART_CAPABILITIES  drv_capabilities;

ARM_USART_STATUS   status;

/* Private variables ----------------------------------------------------------------------------------------------*/
uint8_t tx_buffer[BUFFER_SIZE];
uint8_t rx_buffer[BUFFER_SIZE];
volatile uint32_t read_tx_cnt=0;
volatile uint32_t read_rx_cnt=0;

/* Private function prototypes ------------------------------------------------------------------------------------*/
void ARM_USART_SignalEvent(uint32_t event);

/* Private functions ----------------------------------------------------------------------------------------------*/

//!read capabilities
void Read_Capabilities(void)
{
	drv_capabilities =USARTdrv->GetCapabilities ();
}

//event callback handler
void ARM_USART_SignalEvent(uint32_t event)
{
	switch(event)
	{
	case ARM_USART_EVENT_SEND_COMPLETE:
		read_tx_cnt=USARTdrv->GetTxCount();
		break;
	case ARM_USART_EVENT_RECEIVE_COMPLETE:
		read_rx_cnt=USARTdrv->GetRxCount();
		break;
	case ARM_USART_EVENT_TRANSFER_COMPLETE:
		read_rx_cnt=USARTdrv->GetRxCount();
		break;
	case ARM_USART_EVENT_TX_COMPLETE:
		break;
	case ARM_USART_EVENT_TX_UNDERFLOW:
		break;
	case ARM_USART_EVENT_RX_OVERFLOW:
		break;
	case ARM_USART_EVENT_RX_TIMEOUT:
		break;
	case ARM_USART_EVENT_RX_BREAK:
		break;
	case ARM_USART_EVENT_RX_FRAMING_ERROR:
		break;
	case ARM_USART_EVENT_RX_PARITY_ERROR:
		break;
	case ARM_USART_EVENT_CTS:
		break;
	case ARM_USART_EVENT_DSR:
		break;
	case ARM_USART_EVENT_DCD:
		break;
	case ARM_USART_EVENT_RI:
		break;
	}
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
	int forever = 1;
	uint32_t i=0;
	 /*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/*fill some data in input buffer*/
	for(i=0;i<BUFFER_SIZE;i++)
	{
		tx_buffer[i]=i;
	}
	/*Read capabilities of USART*/
	Read_Capabilities();

	/*Initialise USART(Enable Clock)*/
	USARTdrv->Initialize(ARM_USART_SignalEvent);

	/*Power up the USART peripheral */
	USARTdrv->PowerControl(ARM_POWER_FULL);

	/* Enable Receiver and Transmitter lines */
	USARTdrv->Control (ARM_USART_CONTROL_TX, 1);
	USARTdrv->Control (ARM_USART_CONTROL_RX, 1);

	/*Configure the USART to 9600 Bits/sec */
	USARTdrv->Control(ARM_USART_MODE_SYNCHRONOUS_MASTER|
			ARM_USART_DATA_BITS_8 |
			ARM_USART_PARITY_NONE |
			ARM_USART_STOP_BITS_1 |
			ARM_USART_FLOW_CONTROL_NONE,BAUD_VALUE);
	/*Receives data*/
	//USARTdrv->Receive(rx_buffer,sizeof(rx_buffer));

	/*sends data*/
	//USARTdrv->Send(tx_buffer, sizeof(tx_buffer));

	/*transfer data in USART mode*/
	USARTdrv->Transfer(tx_buffer, rx_buffer,sizeof(tx_buffer));
  
    /*wait for send and receive data done*/
    while(read_rx_cnt==0);
	
    while (forever) 
	{

	}
	/*Statement will never reach here , just to satisfy the standard main*/
	return 0;
}
