 /* --------------------------------------------------------------------------
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * $Date:        02. March 2016
 * $Revision:    V1.1
 *
 * Driver:       Driver_USART
 * Configured:   via RTE_Device.h configuration file
 * Project:      USART Driver for RS1xxxx
 */

/* History:
 * Version 1.0
 * Initial release
 */

#include "USART.h"
#include "UDMA.h"
#include "clock_update.h"


#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(2,10) /* driver version */

// Driver Version
static const ARM_DRIVER_VERSION UsartDriverVersion =
{
		ARM_USART_API_VERSION,
		ARM_USART_DRV_VERSION
};
// USART AND UART1
#if (RTE_USART0)
#define  USART0_IRQ_HANDLER     IRQ038_Handler
static USART_INFO USART0_Info = {0};
#if (RTE_USART0_CHNL_UDMA_TX_EN == 1)
void USART0_UDMA_Tx_Event (uint32_t event ,uint8_t dmaCh);
static USART_DMA USART0_UDMA_TX_CHNL = {
		{
				UDMA_MODE_BASIC,
				0,
				(RTE_USART0_DMA_TX_LEN_PER_DES-1),
				ARBSIZE_1,
				0x0,
				0x0,
				SRC_SIZE_8,
				SRC_INC_8,
				DST_SIZE_8,
				DST_INC_NONE
		},
		RTE_USART0_CHNL_UDMA_TX_CH,
		USART0_UDMA_Tx_Event
};
#endif
#if (RTE_USART0_CHNL_UDMA_RX_EN == 1)
void USART0_UDMA_Rx_Event (uint32_t event ,uint8_t dmaCh);
static USART_DMA USART0_UDMA_RX_CHNL = {
		{
				UDMA_MODE_BASIC,
				0,
				(RTE_USART0_DMA_RX_LEN_PER_DES-1),
				ARBSIZE_1,
				0x0,
				0x0,
				SRC_SIZE_8,
				SRC_INC_NONE,
				DST_SIZE_8,
				DST_INC_8
		},
		RTE_USART0_CHNL_UDMA_RX_CH,
		USART0_UDMA_Rx_Event
};
#endif
//Resources structure
static  USART_RESOURCES USART0_Resources = {

		{                   // Capabilities
				1,              // supports UART(Asynchronous) mode
				1,              // supports Synchronous Master mode
				1,              // supports Synchronous Slave mode
				1,              // supports UART Single-wire mode
				1,              // supports UART IrDA mode(SIR_MODE)
				0,              // supports UART Smart Card mode
				0,              // Smart Card Clock generator
				1,              // RTS Flow Control available
				1,              // CTS Flow Control available
				1,              // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
				1,              // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
				1,              // RTS Line: 0=not available, 1=available.
				1,              // CTS Line: 0=not available, 1=available.
				1,              // DTR Line: 0=not available, 1=available.
				1,              // DSR Line: 0=not available, 1=available.
				1,              // DCD Line: 0=not available, 1=available.
				1,              // RI Line: 0=not available, 1=available.
				1,              // Signal CTS change event(optional)
				1,              // Signal DSR change event(optional)
				1,              // event_dcd(Signal DCD change event)
				1,              // Signal RI change event
		},                  // Capabilities end
#if(RTE_USART_MODE)
		USART0,             // USART ADDRESS
#else
		UART0,              // UART ADDRESS
#endif
		USART0_IRQn,        // IRQn
#if(RTE_USART0_CHNL_UDMA_TX_EN)
		&USART0_UDMA_TX_CHNL,
#else
		NULL,
#endif
#if(RTE_USART0_CHNL_UDMA_RX_EN)
		&USART0_UDMA_RX_CHNL,
#else
		NULL,
#endif
		&USART0_Info
};

#endif

// UART1
#if (RTE_UART1)
#define  UART1_IRQ_HANDLER     IRQ039_Handler
static USART_INFO UART1_Info = {0};
#if (RTE_UART1_CHNL_UDMA_TX_EN == 1)
void UART1_UDMA_Tx_Event (uint32_t event ,uint8_t dmaCh);

static USART_DMA UART1_UDMA_TX_CHNL = {
		{
				UDMA_MODE_BASIC,
				0,
				(RTE_UART1_DMA_TX_LEN_PER_DES-1),
				ARBSIZE_1,
				0x0,
				0x0,
				SRC_SIZE_8,
				SRC_INC_8,
				DST_SIZE_8,
				DST_INC_NONE
		},
		RTE_UART1_CHNL_UDMA_TX_CH,
		UART1_UDMA_Tx_Event
};
#endif
#if (RTE_UART1_CHNL_UDMA_RX_EN == 1)
void UART1_UDMA_Rx_Event (uint32_t event ,uint8_t dmaCh);
static USART_DMA UART1_UDMA_RX_CHNL = {
		{
				UDMA_MODE_BASIC,
				0,
				(RTE_UART1_DMA_RX_LEN_PER_DES-1),
				ARBSIZE_1,
				0x0,
				0x0,
				SRC_SIZE_8,
				SRC_INC_NONE,
				DST_SIZE_8,
				DST_INC_8
		},
		RTE_UART1_CHNL_UDMA_RX_CH,
		UART1_UDMA_Rx_Event
};
#endif

//Resources structure
static  USART_RESOURCES UART1_Resources = {

		{                  // Capabilities
				1,             // supports UART(Asynchronous) mode
				0,             // synchronous_master (not supported)
				0,             // synchronous_slave (not supported)
				1,             // supports UART Single-wire mode
				0,             // IRDA(SIR_MODE) mode (not supported)
				0,             // smart_card (not supported)
				0,             // smart_card_clock (not supported)
				1,             // RTS Flow Control available
				1,             // RTS Flow Control available
				1,             // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
				1,             // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
				1,             // RTS Line: 0=not available, 1=available.
				1,             // CTS Line: 0=not available, 1=available.
				0,             // DTR Line: 0=not available, 1=available.
				0,             // DSR Line: 0=not available, 1=available.
				0,             // DCD Line: 0=not available, 1=available.
				0,             // RI Line: 0=not available, 1=available.
				0,             // Signal CTS change event(optional)
				0,             // Signal DSR change event(optional)
				0,             // event_dcd(Signal DCD change event)
				0,             // Signal RI change event
		},                 // Capabilities end

		UART1,             // ADDRESS
		UART1_IRQn,       // IRQn
#if (RTE_UART1_CHNL_UDMA_TX_EN == 1)
		&UART1_UDMA_TX_CHNL,
#else
		NULL,
#endif
#if (RTE_UART1_CHNL_UDMA_RX_EN == 1)
		&UART1_UDMA_RX_CHNL,
#else
		NULL,
#endif
		&UART1_Info
};

#endif

// ULPSS UART
#if (RTE_ULP_UART)
#define  ULP_UART_IRQ_HANDLER     IRQ012_Handler
static USART_INFO ULP_UART_Info = {0};
#if (RTE_ULPUART_CHNL_UDMA_TX_EN == 1)
void ULPUART_UDMA_Tx_Event (uint32_t event ,uint8_t dmaCh);
static USART_DMA ULPUART_UDMA_TX_CHNL = {
		{
				UDMA_MODE_BASIC,
				0,
				(RTE_ULP_UART_DMA_TX_LEN_PER_DES-1),
				ARBSIZE_1,
				0x0,
				0x0,
				SRC_SIZE_8,
				SRC_INC_8,
				DST_SIZE_8,
				DST_INC_NONE
		},
		RTE_ULPUART_CHNL_UDMA_TX_CH,
		ULPUART_UDMA_Tx_Event
};
#endif
#if (RTE_ULPUART_CHNL_UDMA_RX_EN == 1)
void ULPUART_UDMA_Rx_Event (uint32_t event ,uint8_t dmaCh);
static USART_DMA ULPUART_UDMA_RX_CHNL = {
		{
				UDMA_MODE_BASIC,
				0,
				(RTE_ULP_UART_DMA_RX_LEN_PER_DES-1),
				ARBSIZE_1,
				0x0,
				0x0,
				SRC_SIZE_8,
				SRC_INC_NONE,
				DST_SIZE_8,
				DST_INC_8
		},
		RTE_ULPUART_CHNL_UDMA_RX_CH,
		ULPUART_UDMA_Rx_Event
};
#endif

// Resources structure
static  USART_RESOURCES ULP_UART_Resources = {

		{                   // Capabilities
				1,              // supports UART(Asynchronous) mode
				0,              // synchronous_master (Not supported)
				0,              // synchronous_slave  (Not supported)
				1,              // single_wire
				0,              // IRDA               (Not supported)
				0,              // smart_card         (Not supported)
				0,              // smart_card_clock   (Not supported)
				1,              // RTS Flow Control available
				1,              // CTS Flow Control available
				1,              // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
				1,              // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
				0,              // RTS Line: 0=not available, 1=available.
				0,              // CTS Line: 0=not available, 1=available.
				0,              // DTR Line: 0=not available, 1=available.
				0,              // DSR Line: 0=not available, 1=available.
				0,              // DCD Line: 0=not available, 1=available.
				0,              // RI Line: 0=not available, 1=available.
				0,              // Signal CTS change event(optional)
				0,              // Signal DSR change event(optional)
				0,              // event_dcd(Signal DCD change event)
				0,              // Signal RI change event
		},                  // Capabilities end

		ULP_UART,           // ADDRESS
		ULPSS_UART_IRQn,    // IRQn
#if (RTE_ULPUART_CHNL_UDMA_TX_EN == 1)
		&ULPUART_UDMA_TX_CHNL,
#else
		NULL,
#endif
#if (RTE_ULPUART_CHNL_UDMA_RX_EN == 1)
		&ULPUART_UDMA_RX_CHNL,
#else
		NULL,
#endif
		&ULP_UART_Info
};

#endif

/**
  @fn           void UartIrqHandler (UART_RESOURCES *usart)
  @brief        USART Interrupt handler.
  @param[in]    usart     Pointer to USART resources
 */
static void UartIrqHandler(USART_RESOURCES *usart)
{
	volatile uint32_t int_status,line_status,modem_status;
	uint32_t event,val;

	int_status =0U;
	line_status =0U;
	modem_status=0U;
	event =0U;
	val=0U;

	int_status =  usart->pREGS->IIR;

	if((int_status & USART_RX_LINE_STATUS) == USART_RX_LINE_STATUS)
	{
		line_status = usart->pREGS->LSR;
		// OverRun error
		if((line_status & USART_OVERRUN_ERR) == USART_OVERRUN_ERR)
		{
			if(usart->info->cb_event != NULL)
			{
				usart->info->rx_status.rx_overflow = 1U;
				usart->info->cb_event(  ARM_USART_EVENT_RX_OVERFLOW );
			}
			// Sync Slave mode: If Transmitter enabled, signal TX underflow
			if (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE)
			{
				if (usart->info->xfer.send_active != 0U)
				{
					event |= ARM_USART_EVENT_TX_UNDERFLOW;
				}
			}
		}
		// Parity error
		if((line_status & USART_PARITY_ERR) == USART_PARITY_ERR)
		{
			if(usart->info->cb_event != NULL)
			{
				usart->info->rx_status.rx_parity_error = 1U;
				usart->info->cb_event( ARM_USART_EVENT_RX_PARITY_ERROR );
			}
		}
		// Framing error
		if((line_status & USART_FRAMING_ERR) == USART_FRAMING_ERR)
		{
			if(usart->info->cb_event!= NULL)
			{
				usart->info->rx_status.rx_framing_error=1U;
				usart->info->cb_event(ARM_USART_EVENT_RX_FRAMING_ERROR);
			}
		}
		// Break detected
		else if((line_status & USART_BREAK_ERR) == USART_BREAK_ERR)
		{
			if(usart->info->cb_event!= NULL)
			{
				usart->info->rx_status.rx_break = 1U;
				usart->info->cb_event( ARM_USART_EVENT_RX_BREAK);
			}
		}
	}
	if((int_status & USART_THR_EMPTY) == USART_THR_EMPTY)
	{
		val = 16U;
		while ((val --) && (usart->info->xfer.tx_num != usart->info->xfer.tx_cnt))
		{
			usart->pREGS->THR = usart->info->xfer.tx_buf[usart->info->xfer.tx_cnt];
			usart->info->xfer.tx_cnt++;
		}
		// Check if all data is transmitted
		if (usart->info->xfer.tx_num == usart->info->xfer.tx_cnt)
		{
			// Disable THRE interrupt
			usart->pREGS->IER &= ~USART_INTR_THRE;

			// Clear TX busy flag
			usart->info->xfer.send_active = 0U;

			event |= ARM_USART_EVENT_SEND_COMPLETE;
			usart->info->cb_event(event);
		}
	}
	if((int_status & USART_RX_DATA_AVAILABLE) == USART_RX_DATA_AVAILABLE)
	{
		if(usart->info->cb_event!= NULL)
		{
			//check if receiver contains atleast one char in RBR reg
			if ( (usart->pREGS->LSR_b.DR))
			{
				usart->info->xfer.rx_buf[usart->info->xfer.rx_cnt] = usart->pREGS->RBR;
				usart->info->xfer.rx_cnt++;

				// Check if requested amount of data is received
				if (usart->info->xfer.rx_cnt == usart->info->xfer.rx_num)
				{
					// Disable RDA)rx_data_available) interrupt
					usart->pREGS->IER &= ~USART_INTR_RX_DATA;
					// Clear RX busy flag and set receive transfer complete event
					usart->info->rx_status.rx_busy = 0U;
					if ((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
							(usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE ))
					{
						val = usart->info->xfer.sync_mode;
						usart->info->xfer.sync_mode = 0U;
						switch (val)
						{
						case USART_SYNC_MODE_TX:
							event |= ARM_USART_EVENT_SEND_COMPLETE;
							break;
						case USART_SYNC_MODE_RX:
							event |= ARM_USART_EVENT_RECEIVE_COMPLETE;
							break;
						case USART_SYNC_MODE_TX_RX:
							event |= ARM_USART_EVENT_TRANSFER_COMPLETE;
							break;
						default: break;
						}
					}
					else
					{
						event |= ARM_USART_EVENT_RECEIVE_COMPLETE;
					}
					usart->info->cb_event(event );
					//break;
				}
			}
		}
		//Check if requested amount of data is not received
		if (usart->info->xfer.rx_cnt != usart->info->xfer.rx_num)
		{
			usart->info->cb_event(ARM_USART_EVENT_RX_TIMEOUT);
		}
	}
	if((int_status & 0xF) == USART_MODEM_STATUS_INTR)
	{
		modem_status = usart->pREGS->MSR;

		// CTS state changed
		if ((usart->capabilities.cts) && (modem_status & USART_MSR_DCTS))
		{
			event |= ARM_USART_EVENT_CTS;
			usart->info->cb_event(event);
		}
		// DSR state changed
		if ((usart->capabilities.dsr) && (modem_status & USART_MSR_DDSR))
		{
			event |= ARM_USART_EVENT_DSR;
			usart->info->cb_event(event);
		}
		// Ring indicator
		if ((usart->capabilities.ri)  && (modem_status & USART_MSR_TERI))
		{
			event |= ARM_USART_EVENT_RI;
			usart->info->cb_event(event);
		}
		// DCD state changed
		if ((usart->capabilities.dcd) && (modem_status & USART_MSR_DDCD))
		{
			event |= ARM_USART_EVENT_DCD;
			usart->info->cb_event(event);
		}
	}
}

///////////////////////////////////////////////////////////////
//                         Functions                         //
///////////////////////////////////////////////////////////////
static uint32_t RSI_CLK_CheckPresent(M4Clk_Type *pCLK  ,CLK_PRESENT_T clkPresent)
{
	uint32_t errorReturn = 0;
	switch(clkPresent)
	{
	case SOC_PLL_CLK_PRESENT:
		if(pCLK->PLL_STAT_REG_b.SOCPLL_LOCK == 1)
		{
			errorReturn =  RSI_OK;
		}
		else
		{
			errorReturn = ERROR_CLOCK_NOT_ENABLED;
		}
		break;
	case INTF_PLL_CLK_PRESENT:
		if(pCLK->PLL_STAT_REG_b.INTFPLL_LOCK == 1)
		{
			errorReturn = RSI_OK;
		}
		else
		{
			errorReturn =  ERROR_CLOCK_NOT_ENABLED;
		}
		break;
	case I2S_PLL_CLK_PRESENT:
		if(pCLK->PLL_STAT_REG_b.I2SPLL_LOCK == 1)
		{
			errorReturn = RSI_OK;
		}
		else
		{
			errorReturn =  ERROR_CLOCK_NOT_ENABLED;
		}
		break;
	case MODEM_PLL_CLK_PRESENT :
		if(pCLK->PLL_STAT_REG_b.MODEMPLL_LOCK == 1)
		{
			errorReturn =  RSI_OK;
		}
		else
		{
			errorReturn = ERROR_CLOCK_NOT_ENABLED;
		}
		break;
	}
	return errorReturn;
}

static error_t usart_peripheral_clk_disable(M4Clk_Type *pCLK,PERIPHERALS_CLK_T module)
{
	/*valid parameter check*/
	if(pCLK == NULL)
	{
		return INVALID_PARAMETERS;
	}
	switch(module)
	{
	case USART1_CLK:
		pCLK->CLK_ENABLE_CLEAR_REG1 = (USART1_SCLK_ENABLE | USART1_PCLK_ENABLE);
		pCLK->DYN_CLK_GATE_DISABLE_REG_b.USART1_SCLK_DYN_CTRL_DISABLE_b =1;
		pCLK->DYN_CLK_GATE_DISABLE_REG_b.USART1_PCLK_DYN_CTRL_DISABLE_b =1;
		break;
	case USART2_CLK:
		pCLK->CLK_ENABLE_CLEAR_REG1 = (USART2_SCLK_ENABLE | USART2_PCLK_ENABLE);
		pCLK->DYN_CLK_GATE_DISABLE_REG_b.USART2_SCLK_DYN_CTRL_DISABLE_b =1;
		pCLK->DYN_CLK_GATE_DISABLE_REG_b.USART2_PCLK_DYN_CTRL_DISABLE_b =1;
		break;
	default :
		return INVALID_PARAMETERS;
	}
	return RSI_OK;
}

/**
 * @fn          static error_t USART_SourceClockConfig(M4Clk_Type *pCLK ,
		                                               CLK_ENABLE_T clkType,
		                                               boolean_t FracDivEn ,
		                                               EN_USART_T enUsart,
		                                               USART_CLK_SRC_SEL_T clkSource,
		                                               uint32_t divFactor)
 * @brief		This API is used to configure the Usart clocks
 * @param[in]	pCLK : Pointer to the pll register instance
 * @param[in]	clkType : Boolean value to enable or disable  clock mode
 *                        - 1  Enable : Enables the Usart clock
 *	                      - 0  Disable: Disables the Usart clock
 * @param[in]   FracDivEn : To enable or disable Fractional Division functionality
 *                        - 1 : FracDivEn enabled
 *                        - 0 : FracDivEn disabled
 * @param[in]   enUsart   : Enum values for different Usart instances see possible bypass clock sources at \ref EN_USART_T
 * @param[in]   clkSource : Enum values for Usart clock sources to be selected. see possible Usart clock sources at #USART_CLK_SRC_SEL_T
 * @param[in]   divFactor is the division value for Usart Clock
 * @return 		  returns zero \ref RSI_OK  on success ,on failure return error code.
 * @note        For using UlpRefClk clksource need to configure M4ssRefClk frequency. For that need to call #ROM_CLK_M4ssRefClkConfig Api first
 */
static error_t USART_SourceClockConfig(M4Clk_Type *pCLK ,
		CLK_ENABLE_T clkType,
		boolean_t FracDivEn ,
		EN_USART_T enUsart,
		USART_CLK_SRC_SEL_T clkSource,
		uint32_t divFactor)
{
	/*Parameter validation */
	if(!(pCLK != NULL)  && (divFactor <= USART_MAX_CLK_DIVISION_FACTOR ) )
	{
		return INVALID_PARAMETERS;
	}

	/*Disable the USART clock*/
	if(enUsart == USART1)
	{
		usart_peripheral_clk_disable(pCLK,USART1_CLK);
	}
	else
	{
		usart_peripheral_clk_disable(pCLK,USART2_CLK);
	}

	/*Select clock MUX*/
	switch(clkSource)
	{
	case USART_ULPREFCLK:
		if(enUsart == USART1)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_SEL = 0x00;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART1_SCLK_SWITCHED) != 1) ;
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_DIV_FAC = divFactor;
			/*fractional clock divider select */
			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =1;
			}
			else
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =0;
			}
		}
		else if(enUsart == USART2)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_SEL = 0x00;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART2_SCLK_SWITCHED) != 1) ;
			/*fractional clock divider select */
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_DIV_FAC = divFactor;

			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =1;
			}
			else
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =0;
			}
		}
		break ;

	case USART_MODELPLLCLK2:
		if(RSI_OK != RSI_CLK_CheckPresent(pCLK , MODEM_PLL_CLK_PRESENT))
		{
			return ERROR_CLOCK_NOT_ENABLED;
		}
		if(enUsart == USART1)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_SEL = 0x02;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART1_SCLK_SWITCHED) != 1) ;
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_DIV_FAC = divFactor;
			/*fractional clock divider select */
			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =1;
			}
			else
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =0;
			}
		}
		else if(enUsart == USART2)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_SEL = 0x02;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART2_SCLK_SWITCHED) != 1) ;
			/*fractional clock divider select */
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_DIV_FAC = divFactor;
			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =1;
			}
			else
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =0;
			}
		}
		break ;

	case USART_INTFPLLCLK:
		/*Check clock is present is or not before switching*/
		if(RSI_OK != RSI_CLK_CheckPresent(pCLK , INTF_PLL_CLK_PRESENT))
		{
			return ERROR_CLOCK_NOT_ENABLED;
		}
		if(enUsart == USART1)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_SEL = 0x03;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART1_SCLK_SWITCHED) != 1) ;
			/*fractional clock divider select */
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_DIV_FAC = divFactor;
			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =1;
			}
			else
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =0;
			}
		}
		else if(enUsart == USART2)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_SEL = 0x03;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART2_SCLK_SWITCHED) != 1) ;
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_DIV_FAC = divFactor;

			/*fractional clock divider select */
			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =1;
			}else
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =0;
			}
		}
		break ;

	case USART_SOCPLLCLK:
		/*Check clock is present is or not before switching*/
		if(RSI_OK != RSI_CLK_CheckPresent(pCLK , SOC_PLL_CLK_PRESENT))
		{
			return ERROR_CLOCK_NOT_ENABLED;
		}
		if(enUsart == USART1)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_SEL = 0x01;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART1_SCLK_SWITCHED) != 1) ;
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_DIV_FAC = divFactor;
			/*fractional clock divider select */
			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =1;
			}
			else
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =0;
			}
		}
		else if(enUsart == USART2)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_SEL = 0x01;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART2_SCLK_SWITCHED) != 1) ;
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_DIV_FAC = divFactor;
			/*fractional clock divider select */
			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =0;
			}else
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =1;
			}
		}
		break ;

	case M4_SOCCLKFOROTHERCLOCKS:

		if(enUsart == USART1)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_SEL = 0x04;
			pCLK->CLK_ENABLE_SET_REG3 = M4_SOC_CLK_FOR_OTHER_ENABLE;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART1_SCLK_SWITCHED) != 1) ;
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_DIV_FAC = divFactor;
			/*fractional clock divider select */
			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =1;
			}
			else
			{
				pCLK->CLK_CONFIG_REG2_b.USART1_SCLK_FRAC_SEL =0;
			}
		}
		else if(enUsart == USART2)
		{
			/*select the MUX for the USART 1*/
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_SEL = 0x04;
			pCLK->CLK_ENABLE_SET_REG3 = M4_SOC_CLK_FOR_OTHER_ENABLE;
			/*Wait for the clock MUX switch status */
			while((pCLK->PLL_STAT_REG_b.USART2_SCLK_SWITCHED) != 1) ;
			/*Update the division factor */
			pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_DIV_FAC = divFactor;
			/*fractional clock divider select */
			if(FracDivEn)
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =1;
			}else
			{
				pCLK->CLK_CONFIG_REG2_b.USART2_SCLK_FRAC_SEL =0;
			}
		}
		break;
	default :
		return INVALID_PARAMETERS;
	}
	/*Enable USART clock */
	if(enUsart == USART1)
	{
		clk_peripheral_clk_enable(pCLK,USART1_CLK,clkType);
	}
	else
	{
		clk_peripheral_clk_enable(pCLK,USART2_CLK,clkType);
	}
	return RSI_OK;
}
#if (RTE_USART0_CHNL_UDMA_TX_EN ||RTE_UART1_CHNL_UDMA_TX_EN ||RTE_ULPUART_CHNL_UDMA_TX_EN)
static void USART_UDMA_Tx_Event (uint32_t event,uint8_t dmaCh, USART_RESOURCES *usart)
{
	switch (event)
	{
	case UDMA_EVENT_XFER_DONE:
		// Update TX buffer info
		usart->info->xfer.tx_cnt = usart->info->xfer.tx_num;
		// Clear TX busy flag
		usart->info->xfer.send_active = 0U;
		// Set Send Complete event for asynchronous transfers
		usart->info->cb_event (ARM_USART_EVENT_SEND_COMPLETE);
		break;
	case UDMA_EVENT_ERROR:
		break;
	}

}
#endif
#if (RTE_USART0_CHNL_UDMA_RX_EN ||RTE_UART1_CHNL_UDMA_RX_EN ||RTE_ULPUART_CHNL_UDMA_RX_EN) 
static void USART_UDMA_Rx_Event(uint32_t event, uint8_t dmaCh,USART_RESOURCES *usart)
{
	uint32_t evt1 = 0U;
	uint32_t val;
	switch (event)
	{
	case UDMA_EVENT_XFER_DONE:
		usart->info->xfer.rx_cnt    = usart->info->xfer.rx_num;
		usart->info->rx_status.rx_busy = 0U;

		if ((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
				(usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE ))
		{
			val = usart->info->xfer.sync_mode;
			usart->info->xfer.sync_mode = 0U;
			switch (val)
			{
			case USART_SYNC_MODE_TX:
				evt1 |= ARM_USART_EVENT_SEND_COMPLETE;
				break;
			case USART_SYNC_MODE_RX:
				evt1 |= ARM_USART_EVENT_RECEIVE_COMPLETE;
				break;
			case USART_SYNC_MODE_TX_RX:
				evt1 |= ARM_USART_EVENT_TRANSFER_COMPLETE;
				break;
			default: break;
			}
		}
		else
		{
			evt1 |= ARM_USART_EVENT_RECEIVE_COMPLETE;
		}

		break;
	case UDMA_EVENT_ERROR:
		break;
	}
	if ((evt1 != 0U) && (usart->info->cb_event != NULL))
	{
		usart->info->cb_event (evt1);
	}
}
#endif
/**
 * @fn           USART_SetBaudrate (uint32_t baudrate,uint32_t baseClk,USART_RESOURCES  *usart)
 * @brief        This API programs the baudrate for USART
 * @param[in]    usart     Pointer to USART resources
 * @param[in]    baudrate  baudrate
 * @param[in]    baseClk   base clock for usart
 */
int32_t USART_SetBaudrate (uint32_t baudrate,uint32_t baseClk,USART_RESOURCES  *usart)
{
	uint16_t baud_divisor = 0, dlf;
	uint32_t baud_divisor_frac = 0;

	if((usart == NULL) || (baudrate> 7372800))
	{
		return ERROR_UART_INVALID_ARG;
	}
	else
	{
		// Get the baud divisor
		baud_divisor_frac = ((baseClk * 4) / (baudrate));
		dlf = (baud_divisor_frac & 0x3F);
		baud_divisor = (baud_divisor_frac >> 6);

		// Reset before accessing the DLL and DLH register
		usart->pREGS->LCR = USART_CNTL_DLAB_SET;

		//Reset the registers
		usart->pREGS->DLF_b.DLF = USART_RESET_REG;
		usart->pREGS->DLL = USART_RESET_REG;
		usart->pREGS->DLH = USART_RESET_REG;

		// Reset after accessing the DLL and DLH register
		usart->pREGS->LCR = USART_CNTL_DLAB_RESET;

		// Reset before accessing the DLL and DLH register
		usart->pREGS->LCR = USART_CNTL_DLAB_SET;

		// Fill the DLL and DLH registers with bauddivisor
		usart->pREGS->DLL = (baud_divisor & 0x00FF);
		usart->pREGS->DLH = (baud_divisor >> 8);
		usart->pREGS->DLF = dlf;

		// Reset after accessing the DLL and DLH register
		usart->pREGS->LCR = USART_CNTL_DLAB_RESET;
	}
	usart->info->baudrate = baudrate;
	return RSI_OK;
}

/**
  @fn           ARM_USART_CAPABILITIES USART_GetCapabilities (USART_RESOURCES *usart)
  @brief        Gets driver capabilities
  @param[in]    usart     Pointer to USART resources
  @return       \ref ARM_USART_CAPABILITIES
 */
static ARM_USART_CAPABILITIES USART_GetCapabilities (USART_RESOURCES *usart)
{
	return usart->capabilities;
}

/**
  @fn          int32_t USART_Initialize (ARM_USART_SignalEvent_t  cb_event ,USART_RESOURCES         *usart)
  @brief       Initialize USART Interface.
  @param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  @param[in]   usart     Pointer to USART resources
  @return      \ref execution_status
 */
static int32_t USART_Initialize (ARM_USART_SignalEvent_t  cb_event,USART_RESOURCES *usart)
{
 	RSI_PS_UlpssPeriPowerUp(ULPSS_PWRGATE_ULP_UART);
	RSI_PS_M4ssPeriPowerUp(M4SS_PWRGATE_ULP_PERI1);
	if (usart->info->flags & USART_FLAG_INITIALIZED)
	{
		// Driver is already initialized
		return ARM_DRIVER_OK;
	}
	// Initialize USART Run-time Resources
	usart->info->cb_event = cb_event;
	usart->info->rx_status.rx_busy          = 0U;
	usart->info->rx_status.rx_overflow      = 0U;
	usart->info->rx_status.rx_break         = 0U;
	usart->info->rx_status.rx_framing_error = 0U;
	usart->info->rx_status.rx_parity_error  = 0U;
	usart->info->xfer.send_active           = 0U;
	usart->info->xfer.tx_def_val            = 0U;

	if((usart->pREGS == UART0)||(usart->pREGS == USART0))
	{
		USART_SourceClockConfig(M4CLK ,ENABLE_STATIC_CLK,RTE_USART0_FRAC_DIV_EN,USART1,RTE_USART0_CLK_SRC,RTE_USART0_CLK_DIV_FACT);
	}
	if(usart->pREGS == UART1)
	{
		USART_SourceClockConfig(M4CLK ,ENABLE_STATIC_CLK,RTE_UART1_FRAC_DIV_EN,USART2,RTE_UART1_CLK_SRC,RTE_UART1_CLK_DIV_FACT);
	}
	if(usart->pREGS == ULP_UART)
	{
	 	RSI_ULPSS_UlpUartClkConfig(ULPCLK,ENABLE_STATIC_CLK,RTE_ULP_UART_FRAC_SEL,RTE_ULP_UART_CLK_SRC,RTE_ULP_UART_CLK_DIV_FACT);
 	}
	// DMA Initialize
	if (usart->dma_tx || usart->dma_rx)
	{
		if(RTE_USART0_CHNL_UDMA_TX_EN || RTE_USART0_CHNL_UDMA_RX_EN||RTE_UART1_CHNL_UDMA_TX_EN ||RTE_UART1_CHNL_UDMA_RX_EN ||RTE_ULPUART_CHNL_UDMA_TX_EN ||RTE_ULPUART_CHNL_UDMA_RX_EN)
		{
			if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS == UART1))
			{
				UDMA0_Initialize();
			}
			if(usart->pREGS == ULP_UART)
			{
				UDMA1_Initialize();
			}
		}
		else
		{
			return ARM_DRIVER_ERROR;
		}
	}
	//initialise
	usart->info->flags = USART_FLAG_INITIALIZED;
	return ARM_DRIVER_OK;
}

/**
  @fn          int32_t USART_Uninitialize (USART_RESOURCES *usart)
  @brief       De-initialize USART Interface.
  @param[in]   usart     Pointer to USART resources
  @return      \ref execution_status
 */
static int32_t USART_Uninitialize (USART_RESOURCES *usart)
{
  RSI_PS_UlpssPeriPowerDown(ULPSS_PWRGATE_ULP_UART);
	RSI_PS_M4ssPeriPowerDown(M4SS_PWRGATE_ULP_PERI1);
	
	if((usart->pREGS == UART0)||(usart->pREGS == USART0)) //NEED TO KEEP DEFINES
	{
		usart_peripheral_clk_disable(M4CLK,USART1_CLK);
	}
	if(usart->pREGS == UART1)
	{
		usart_peripheral_clk_disable(M4CLK,USART2_CLK);
	}
	if(usart->pREGS == ULP_UART)
	{
		ulpss_peripheral_disable(ULPCLK, ULP_UART_CLK);		
	}
	// DMA Uninitialize
	if (usart->dma_tx || usart->dma_rx)
	{
		if(RTE_USART0_CHNL_UDMA_TX_EN || RTE_USART0_CHNL_UDMA_RX_EN||RTE_UART1_CHNL_UDMA_TX_EN ||RTE_UART1_CHNL_UDMA_RX_EN ||RTE_ULPUART_CHNL_UDMA_TX_EN ||RTE_ULPUART_CHNL_UDMA_RX_EN)
		{
			if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS == UART1))
			{
				//unintialise dma
				UDMA0_Uninitialize();
			}
			if(usart->pREGS == ULP_UART)
			{
				UDMA1_Uninitialize();
			}
		}
		else
		{
			return ARM_DRIVER_ERROR;
		}

	}
	// Reset USART status flags
	usart->info->flags = 0U;
	return ARM_DRIVER_OK;
}

/**
  @fn          int32_t USART_PowerControl (ARM_POWER_STATE  state,USART_RESOURCES *usart)
  @brief       Control USART Interface Power.
  @param[in]   state  Power state
  @param[in]   usart  Pointer to USART resources
  @return      \ref execution_status
 */
static int32_t USART_PowerControl (ARM_POWER_STATE  state,USART_RESOURCES *usart)
{
	switch (state)
	{
	case ARM_POWER_OFF:
		// Disable USART IRQ
		NVIC_DisableIRQ(usart->irq_num);

		// If DMA mode - disable TX DMA channel
		if ((usart->dma_tx) && (usart->info->xfer.send_active != 0U))
		{
			if(RTE_USART0_CHNL_UDMA_TX_EN || RTE_UART1_CHNL_UDMA_TX_EN || RTE_ULPUART_CHNL_UDMA_TX_EN )
			{
				if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS ==UART1))
				{
					UDMA0_ChannelEnable (usart->dma_tx->channel);
				}
				else
				{
					UDMA1_ChannelEnable (usart->dma_tx->channel);
				}
			}
			else
			{
				return ARM_DRIVER_ERROR;
			}
		}
		// If DMA mode - disable DMA channel
		if ((usart->dma_rx) && (usart->info->rx_status.rx_busy))
		{
			if(RTE_USART0_CHNL_UDMA_RX_EN || RTE_UART1_CHNL_UDMA_RX_EN || RTE_ULPUART_CHNL_UDMA_RX_EN )
			{

				if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS ==UART1))
				{
					UDMA0_ChannelEnable (usart->dma_rx->channel);
				}
				else
				{
					UDMA1_ChannelEnable (usart->dma_rx->channel);
				}
			}
			else
			{
				return ARM_DRIVER_ERROR;
			}
		}
		// Clear pending USART interrupts in NVIC
		NVIC_ClearPendingIRQ(usart->irq_num);

		// Clear driver variables
		usart->info->rx_status.rx_busy          = 0U;
		usart->info->rx_status.rx_overflow      = 0U;
		usart->info->rx_status.rx_break         = 0U;
		usart->info->rx_status.rx_framing_error = 0U;
		usart->info->rx_status.rx_parity_error  = 0U;
		usart->info->xfer.send_active           = 0U;

		usart->info->flags &= ~USART_FLAG_POWERED;
		break;

	case ARM_POWER_LOW:
		return ARM_DRIVER_ERROR_UNSUPPORTED;

	case ARM_POWER_FULL:
		if ((usart->info->flags & USART_FLAG_INITIALIZED) == 0U)
		{
			return ARM_DRIVER_ERROR;
		}
		if ((usart->info->flags & USART_FLAG_POWERED)     != 0U)
		{
			return ARM_DRIVER_OK;
		}

		// Disable interrupts
		usart->pREGS->IER = 0U;

		// Clear driver variables
		usart->info->rx_status.rx_busy          = 0U;
		usart->info->rx_status.rx_overflow      = 0U;
		usart->info->rx_status.rx_break         = 0U;
		usart->info->rx_status.rx_framing_error = 0U;
		usart->info->rx_status.rx_parity_error  = 0U;
		usart->info->mode                       = 0U;
		usart->info->flags                      = 0U;
		usart->info->xfer.send_active           = 0U;

		usart->info->flags = USART_FLAG_POWERED | USART_FLAG_INITIALIZED;

		// Clear and Enable USART IRQ
		NVIC_ClearPendingIRQ(usart->irq_num);

		NVIC_EnableIRQ(usart->irq_num);
		break;
	default: return ARM_DRIVER_ERROR_UNSUPPORTED;
	}
	return ARM_DRIVER_OK;
}

/**
  @fn          static int32_t USART_Send_Data(  const void *data, uint32_t dataLen,USART_RESOURCES *usart)
  @brief       Start sending data to USART transmitter.
  @param[in]   data  Pointer to buffer with data to send to USART transmitter
  @param[in]   num   Number of data items to send
  @param[in]   usart Pointer to USART resources
  @return      \ref execution_status
 */
static int32_t USART_Send_Data(  const void *data, uint32_t num,USART_RESOURCES *usart)
{
	int32_t stat;
	uint32_t val=0;

	RSI_UDMA_CHA_CFG_T chnl_cfg = {0};

	if(data == NULL || num < 1)
	{
		return ARM_DRIVER_ERROR_PARAMETER;
	}
	if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U)
	{
		// USART is not configured (mode not selected)
		return ARM_DRIVER_ERROR;
	}

	if (usart->info->xfer.send_active != 0U)
	{
		// Send is not completed yet
		return ARM_DRIVER_ERROR_BUSY;
	}

	// Set Send active flag
	usart->info->xfer.send_active = 1U;
	// Save transmit buffer info
	usart->info->xfer.tx_buf = (uint8_t *)data;
	usart->info->xfer.tx_num = num;
	usart->info->xfer.tx_cnt = 0U;
	if (usart->dma_tx)
	{
		if(RTE_USART0_CHNL_UDMA_TX_EN ||RTE_UART1_CHNL_UDMA_TX_EN ||RTE_ULPUART_CHNL_UDMA_TX_EN)
		{
			//DMA
			chnl_cfg.altStruct = 0;
			chnl_cfg.burstReq = 1;
			if((usart->pREGS == UART0)||(usart->pREGS == USART0))
			{
				chnl_cfg.channelPrioHigh = UDMA0_CHNL_PRIO_LVL;
				chnl_cfg.dmaCh  = RTE_USART0_CHNL_UDMA_TX_CH;
				chnl_cfg.periAck = USART0_ACK;
				chnl_cfg.periphReq = 1;
				chnl_cfg.reqMask = 0;
			}
			if(usart->pREGS ==UART1)
			{
				chnl_cfg.channelPrioHigh = UDMA0_CHNL_PRIO_LVL;
				chnl_cfg.dmaCh  = RTE_UART1_CHNL_UDMA_TX_CH;
				chnl_cfg.periAck = UART1_ACK;
				chnl_cfg.periphReq = 1;
				chnl_cfg.reqMask = 0;
			}
			if(usart->pREGS ==ULP_UART)
			{
				chnl_cfg.channelPrioHigh = UDMA1_CHNL_PRIO_LVL;
				chnl_cfg.dmaCh  = RTE_ULPUART_CHNL_UDMA_TX_CH;
				chnl_cfg.periAck = UART3_ACK;
				chnl_cfg.periphReq = 1;
				chnl_cfg.reqMask = 0;
			}

			// Configure DMA channel
			if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS ==UART1))
			{
				stat = UDMA0_ChannelConfigure( usart->dma_tx->channel,
						(uint32_t)(usart->info->xfer.tx_buf),
						(uint32_t)&(usart->pREGS->THR),
						num,
						usart->dma_tx->control,
						&chnl_cfg,
						usart->dma_tx->cb_event);
				if (stat == -1)
				{
					return ARM_DRIVER_ERROR;
				}
				UDMA0_ChannelEnable (usart->dma_tx->channel);
				UDMA0_DMAEnable();
			}

			if(usart->pREGS ==ULP_UART)
			{
				stat = UDMA1_ChannelConfigure( usart->dma_tx->channel,
						(uint32_t)(usart->info->xfer.tx_buf),
						(uint32_t)&(usart->pREGS->THR),
						num,
						usart->dma_tx->control,
						&chnl_cfg,
						usart->dma_tx->cb_event);
				if (stat == -1)
				{
					return ARM_DRIVER_ERROR;
				}
				UDMA1_ChannelEnable (usart->dma_tx->channel);
				UDMA1_DMAEnable();
			}
		}
		else
		{
			return ARM_DRIVER_ERROR;
		}
	}
	else
	{
		// Fill TX FIFO
		if (usart->pREGS->LSR & USART_LSR_THRE)
		{
			val = 16U;
			while ((val--) && (usart->info->xfer.tx_cnt != usart->info->xfer.tx_num))
			{
				usart->pREGS->THR = usart->info->xfer.tx_buf[usart->info->xfer.tx_cnt++];
			}
		}
		// Enable transmit holding register empty interrupt
		usart->pREGS->IER |= USART_INTR_THRE;
	}
	return ARM_DRIVER_OK;
}

/**
  @fn          static int32_t USART_Receive_Data( const void *data,uint32_t num,USART_RESOURCES *usart)
  @brief       Start receiving data from USART receiver.
  @param[out]  data  Pointer to buffer for data to receive from USART receiver
  @param[in]   num   Number of data items to receive
  @param[in]   usart Pointer to USART resources
  @return      \ref execution_status
 */
static int32_t USART_Receive_Data(const void *data,uint32_t num,USART_RESOURCES *usart)
{
	int32_t stat;

	RSI_UDMA_CHA_CFG_T chnl_cfg = {0};

	if(data == NULL || num < 1)
	{
		return ARM_DRIVER_ERROR_PARAMETER;
	}

	if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U)
	{
		// USART is not configured (mode not selected)
		return ARM_DRIVER_ERROR;
	}
	// Check if receiver is busy
	if (usart->info->rx_status.rx_busy == 1U)
	{
		return ARM_DRIVER_ERROR_BUSY;
	}

	// Set RX busy flag
	usart->info->rx_status.rx_busy = 1U;

	// Save number of data to be received
	usart->info->xfer.rx_num = num;

	// Clear RX statuses
	usart->info->rx_status.rx_break          = 0U;
	usart->info->rx_status.rx_framing_error  = 0U;
	usart->info->rx_status.rx_overflow       = 0U;
	usart->info->rx_status.rx_parity_error   = 0U;

	// Save receive buffer info
	usart->info->xfer.rx_buf = (uint8_t *)data;
	usart->info->xfer.rx_cnt = 0U;

	// DMA mode
	if (usart->dma_rx)
	{
		if(RTE_USART0_CHNL_UDMA_RX_EN ||RTE_UART1_CHNL_UDMA_RX_EN ||RTE_ULPUART_CHNL_UDMA_RX_EN)
		{
			chnl_cfg.altStruct = 0;
			chnl_cfg.burstReq = 1;

			if((usart->pREGS == UART0)||(usart->pREGS == USART0))
			{
				chnl_cfg.channelPrioHigh = UDMA0_CHNL_PRIO_LVL;
				chnl_cfg.dmaCh  = RTE_USART0_CHNL_UDMA_RX_CH;
				chnl_cfg.periAck = USART0_ACK;
				chnl_cfg.periphReq = 1;
				chnl_cfg.reqMask = 0;

			}
			if(usart->pREGS == UART1)
			{
				chnl_cfg.channelPrioHigh = UDMA0_CHNL_PRIO_LVL;
				chnl_cfg.dmaCh  = RTE_UART1_CHNL_UDMA_RX_CH;
				chnl_cfg.periAck = UART1_ACK;
				chnl_cfg.periphReq = 1;
				chnl_cfg.reqMask = 0;
			}
			if(usart->pREGS == ULP_UART)
			{
				chnl_cfg.channelPrioHigh = UDMA1_CHNL_PRIO_LVL;
				chnl_cfg.dmaCh  = RTE_ULPUART_CHNL_UDMA_RX_CH;
				chnl_cfg.periAck = UART3_ACK;
				chnl_cfg.periphReq = 1;
				chnl_cfg.reqMask = 0;
			}
			// Configure DMA channel
			if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS ==UART1))
			{
				stat = UDMA0_ChannelConfigure(usart->dma_rx->channel,
						(uint32_t)&(usart->pREGS->RBR),
						(uint32_t)(usart->info->xfer.rx_buf),
						num,
						usart->dma_rx->control,
						&chnl_cfg,
						usart->dma_rx->cb_event);
				if (stat == -1)
				{
					return ARM_DRIVER_ERROR;
				}
				UDMA0_ChannelEnable (usart->dma_rx->channel);
			}

			if(usart->pREGS == ULP_UART)
			{
				stat = UDMA1_ChannelConfigure(usart->dma_rx->channel,
						(uint32_t)&(usart->pREGS->RBR),
						(uint32_t)(usart->info->xfer.rx_buf),
						num,
						usart->dma_rx->control,
						&chnl_cfg,
						usart->dma_rx->cb_event);
				if (stat == -1)
				{
					return ARM_DRIVER_ERROR;
				}
				UDMA1_ChannelEnable (usart->dma_rx->channel);
			}
		}
		else
		{
			return ARM_DRIVER_ERROR;
		}
	}
	else
	{
		// Enable receive data available interrupt
		usart->pREGS->IER |= USART_INTR_RX_DATA;
	}
	return ARM_DRIVER_OK;
}

/**
  @fn          static int32_t USART_Transfer (const void *data_out, void *data_in, uint32_t num,USART_RESOURCES *usart)
  @brief       Start sending/receiving data to/from USART transmitter/receiver.
  @param[in]   data_out  Pointer to buffer with data to send to USART transmitter
  @param[out]  data_in   Pointer to buffer for data to receive from USART receiver
  @param[in]   num       Number of data items to transfer
  @param[in]   usart     Pointer to USART resources
  @return      \ref execution_status
 */
static int32_t USART_Transfer (const void *data_out, void *data_in, uint32_t num,USART_RESOURCES *usart)
{
	int32_t status;
	if ((data_out == NULL) || (data_in == NULL) || (num == 0U))
	{
		// Invalid parameters
		return ARM_DRIVER_ERROR_PARAMETER;
	}
	if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U)
	{
		// USART is not configured
		return ARM_DRIVER_ERROR;
	}
	// for only usart mode
	if(RTE_USART_MODE)
	{
		if((usart->capabilities.synchronous_master==1)||(usart->capabilities.synchronous_slave==1))
		{
			// Set xfer mode
			usart->info->xfer.sync_mode = USART_SYNC_MODE_TX_RX;

			// Receive
			status = USART_Receive_Data (data_in, num, usart);
			if (status != ARM_DRIVER_OK)
			{
				return status;
			}
			// Send
			status = USART_Send_Data (data_out, num, usart);
			if (status != ARM_DRIVER_OK)
			{
				return status;
			}
		}
		else
		{
			// Only in synchronous mode
			return ARM_DRIVER_ERROR;
		}
	}
	return ARM_DRIVER_OK;
}

/**
  @fn          static uint32_t USART_GetTxCount (USART_RESOURCES *usart)
  @brief       Get transmitted data count.
  @param[in]   usart     Pointer to USART resources
  @return      number of data items transmitted
 */
static uint32_t USART_GetTxCount (USART_RESOURCES *usart)
{
	uint32_t cnt;
	cnt = usart->info->xfer.tx_cnt;
	return cnt;
}

/**
  @fn          uint32_t USART_GetRxCount (USART_RESOURCES *usart)
  @brief       Get received data count.
  @param[in]   usart     Pointer to USART resources
  @return      number of data items received
 */
static uint32_t USART_GetRxCount (USART_RESOURCES *usart)
{
	uint32_t cnt;
	cnt = usart->info->xfer.rx_cnt;
	return cnt;
}

/**
  @fn          static int32_t USART_Control (uint32_t  control, uint32_t  arg,uint32_t baseClk, USART_RESOURCES  *usart)
  @brief       Control USART Interface.
  @param[in]   control  Operation
  @param[in]   arg      Argument of operation (optional)
  @param[in]   usart    Pointer to USART resources
  @return      common \ref execution_status and driver specific \ref usart_execution_status
 */
static int32_t USART_Control (uint32_t  control, uint32_t  arg,uint32_t baseClk, USART_RESOURCES  *usart)
{
	uint32_t   mode;
	uint16_t fcr = 0,lcr = 0,lcr_ext=0;
	if ((usart->info->flags & USART_FLAG_POWERED) == 0U)
	{
		// USART not powered
		return ARM_DRIVER_ERROR;
	}
	switch (control & ARM_USART_CONTROL_Msk)
	{
	case ARM_USART_CONTROL_TX:
		if (arg)
		{
			if((usart->pREGS == UART0)||(usart->pREGS == USART0))
			{
				if(RTE_USART0_TX_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_USART0_TX_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_USART0_TX_PORT,(uint8_t)(RTE_USART0_TX_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_TX_PAD);
					//configure TX pin
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_TX_PORT,RTE_USART0_TX_PIN,RTE_USART0_TX_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_TX_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_USART0_TX_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_TX_PORT,RTE_USART0_TX_PIN,RTE_USART0_TX_MUX);
				}
			}
			if(usart->pREGS == UART1)
			{
				if(RTE_UART1_TX_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_UART1_TX_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_UART1_TX_PORT,(uint8_t)(RTE_UART1_TX_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_TX_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_TX_PORT,RTE_UART1_TX_PIN,RTE_UART1_TX_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_TX_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_UART1_TX_PIN);
					//configure TX pin
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_TX_PORT,RTE_UART1_TX_PIN,RTE_UART1_TX_MUX);
				}
			}
			if(usart->pREGS == ULP_UART)
			{
				//configure TX pin
				RSI_EGPIO_UlpPadReceiverEnable(RTE_ULP_UART_TX_PIN);
				RSI_EGPIO_SetPinMux(EGPIO1,RTE_ULP_UART_TX_PORT,RTE_ULP_UART_TX_PIN,RTE_ULP_UART_TX_MUX);
			}
			usart->info->flags |= USART_FLAG_TX_ENABLED;
		}
		else
		{
			if((usart->pREGS == UART0)||(usart->pREGS == USART0))
			{
				if(RTE_USART0_TX_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverDisable((uint8_t)(RTE_USART0_TX_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_USART0_TX_PORT,(uint8_t)(RTE_USART0_TX_PIN-64),6);
					RSI_EGPIO_PadSelectionDisable(RTE_USART0_TX_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_TX_PORT,RTE_USART0_TX_PIN,0);
				}
				else
				{
					RSI_EGPIO_PadSelectionDisable(RTE_USART0_TX_PAD);
					RSI_EGPIO_PadReceiverDisable(RTE_USART0_TX_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_TX_PORT,RTE_USART0_TX_PIN,0);
				}
			}
			if(usart->pREGS == UART1)
			{
				if(RTE_UART1_TX_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverDisable((uint8_t)(RTE_UART1_TX_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_UART1_TX_PORT,(uint8_t)(RTE_UART1_TX_PIN-64),6);
					RSI_EGPIO_PadSelectionDisable(RTE_UART1_TX_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_TX_PORT,RTE_UART1_TX_PIN,0);
				}
				else
				{
					RSI_EGPIO_PadSelectionDisable(RTE_UART1_TX_PAD);
					RSI_EGPIO_PadReceiverDisable(RTE_UART1_TX_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_TX_PORT,RTE_UART1_TX_PIN,0);
				}
			}
			if(usart->pREGS == ULP_UART)
			{
				//configure TX pin
				RSI_EGPIO_UlpPadReceiverDisable(RTE_ULP_UART_TX_PIN);
				RSI_EGPIO_SetPinMux(EGPIO1,RTE_ULP_UART_TX_PORT,RTE_ULP_UART_TX_PIN,0);
			}
			usart->info->flags &= ~USART_FLAG_TX_ENABLED;
		}
		return ARM_DRIVER_OK;

	case ARM_USART_CONTROL_RX:

		if (arg)
		{
			if((usart->pREGS == UART0)||(usart->pREGS == USART0))
			{
				if(RTE_USART0_RX_PIN > 63)
				{
					RSI_EGPIO_UlpPadDriverDisableState((uint8_t)(RTE_USART0_RX_PIN-64),ulp_Pullup);
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_USART0_RX_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_USART0_RX_PORT,(uint8_t)(RTE_USART0_RX_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_RX_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_RX_PORT,RTE_USART0_RX_PIN,RTE_USART0_RX_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_RX_PAD);
					RSI_EGPIO_PadDriverDisableState(RTE_USART0_RX_PIN,Pullup);
					RSI_EGPIO_PadReceiverEnable(RTE_USART0_RX_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_RX_PORT,RTE_USART0_RX_PIN,RTE_USART0_RX_MUX);
				}
			}
			if(usart->pREGS == UART1)
			{
				if(RTE_UART1_RX_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_UART1_RX_PIN-64));
					RSI_EGPIO_UlpPadDriverDisableState((uint8_t)(RTE_UART1_RX_PIN-64),ulp_Pullup);
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_UART1_RX_PORT,(uint8_t)(RTE_UART1_RX_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_RX_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_RX_PORT,RTE_UART1_RX_PIN,RTE_UART1_RX_MUX);
				}
				else
				{
					RSI_EGPIO_PadDriverDisableState(RTE_UART1_RX_PIN,Pullup);
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_RX_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_UART1_RX_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_RX_PORT,RTE_UART1_RX_PIN,RTE_UART1_RX_MUX);
				}
			}
			if(usart->pREGS == ULP_UART)
			{
				RSI_EGPIO_UlpPadDriverDisableState(RTE_ULP_UART_RX_PIN,ulp_Pullup);
				RSI_EGPIO_UlpPadReceiverEnable(RTE_ULP_UART_RX_PIN);
				//configure RX pin
				RSI_EGPIO_SetPinMux(EGPIO1,RTE_ULP_UART_RX_PORT,RTE_ULP_UART_RX_PIN,RTE_ULP_UART_RX_MUX);
			}
			usart->info->flags |= USART_FLAG_RX_ENABLED;
		}
		else
		{
			if((usart->pREGS == UART0)||(usart->pREGS == USART0))
			{
				if(RTE_USART0_RX_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverDisable((uint8_t)(RTE_USART0_RX_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_USART0_RX_PORT,(uint8_t)(RTE_USART0_RX_PIN-64),6);
					RSI_EGPIO_PadSelectionDisable(RTE_USART0_RX_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_RX_PORT,RTE_USART0_RX_PIN,0);
				}
				else
				{
					RSI_EGPIO_PadSelectionDisable(RTE_USART0_RX_PAD);
					RSI_EGPIO_PadReceiverDisable(RTE_USART0_RX_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_RX_PORT,RTE_USART0_RX_PIN,0);
				}
			}
			if(usart->pREGS == UART1)
			{
				if(RTE_UART1_RX_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverDisable((uint8_t)(RTE_UART1_RX_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_UART1_RX_PORT,(uint8_t)(RTE_UART1_RX_PIN-64),6);
					RSI_EGPIO_PadSelectionDisable(RTE_UART1_RX_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_RX_PORT,RTE_UART1_RX_PIN,0);
				}
				else
				{
					RSI_EGPIO_PadSelectionDisable(RTE_UART1_RX_PAD);
					RSI_EGPIO_PadReceiverDisable(RTE_UART1_RX_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_RX_PORT,RTE_UART1_RX_PIN,0);
				}
			}
			if(usart->pREGS == ULP_UART)
			{
				RSI_EGPIO_UlpPadReceiverDisable(RTE_ULP_UART_RX_PIN);
				RSI_EGPIO_SetPinMux(EGPIO1,RTE_ULP_UART_RX_PORT,RTE_ULP_UART_RX_PIN,0);
			}
			usart->info->flags &= ~USART_FLAG_RX_ENABLED;
		}
		return ARM_DRIVER_OK;

	case ARM_USART_CONTROL_BREAK:
		if (arg)
		{
			if (usart->info->xfer.send_active != 0U)
			{
				return ARM_DRIVER_ERROR_BUSY;
			}
			usart->pREGS->LCR |= USART_LCR_BC;

			// Set Send active flag
			usart->info->xfer.send_active = 1U;
		}
		else
		{
			usart->pREGS->LCR &= ~USART_LCR_BC;
			// Clear Send active flag
			usart->info->xfer.send_active = 0U;
		}
		return ARM_DRIVER_OK;

	case ARM_USART_ABORT_SEND:
		// Disable transmit holding register empty interrupt
		usart->pREGS->IER &= ~USART_INTR_THRE;

		if (usart->dma_rx || usart->dma_tx)
		{
			fcr |= USART_DMA_MODE_EN;
		}
		// Transmit FIFO reset
		fcr = USART_FIFO_ENABLE;
		fcr |= USART_FIFO_TX_RESET;

		usart->pREGS->FCR = fcr;

		// If DMA mode - disable DMA channel
		if ((usart->dma_tx) && (usart->info->xfer.send_active != 0U))
		{
			if(RTE_USART0_CHNL_UDMA_TX_EN ||RTE_UART1_CHNL_UDMA_TX_EN ||RTE_ULPUART_CHNL_UDMA_TX_EN)
			{
				//DISABLE DMa
				if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS == UART1))
				{
					UDMA0_ChannelDisable (usart->dma_tx->channel);
				}

				if(usart->pREGS == ULP_UART)
				{
					UDMA1_ChannelDisable (usart->dma_tx->channel);
				}
			}
		}
		// Clear Send active flag
		usart->info->xfer.send_active = 0U;
		return ARM_DRIVER_OK;

		// Abort receive
	case ARM_USART_ABORT_RECEIVE:
		// Disable receive data available interrupt
		usart->pREGS->IER &= ~USART_INTR_RX_DATA;

		if (usart->dma_rx || usart->dma_tx)
		{
			fcr |= USART_DMA_MODE_EN;
		}
		// Receive FIFO reset
		fcr = USART_FIFO_ENABLE;
		fcr |= USART_FIFO_RX_RESET;
		usart->pREGS->FCR = fcr;

		// If DMA mode - disable DMA channel
		if ((usart->dma_rx) && (usart->info->rx_status.rx_busy))
		{
			if(RTE_USART0_CHNL_UDMA_RX_EN ||RTE_UART1_CHNL_UDMA_RX_EN ||RTE_ULPUART_CHNL_UDMA_RX_EN)
			{
				//DISABLE DMa
				if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS == UART1))
				{
					UDMA0_ChannelDisable (usart->dma_tx->channel);
				}

				if(usart->pREGS == ULP_UART)
				{
					UDMA1_ChannelDisable (usart->dma_tx->channel);
				}
			}
		}
		// Clear RX busy status
		usart->info->rx_status.rx_busy = 0U;

		return ARM_DRIVER_OK;


		// Abort transfer
	case ARM_USART_ABORT_TRANSFER:
		// Disable transmit holding register empty and
		// receive data available interrupts
		usart->pREGS->IER &= ~(USART_INTR_THRE | USART_INTR_RX_DATA);

		// If DMA mode - disable DMA channel
		if ((usart->dma_tx) && (usart->info->xfer.send_active != 0U))
		{
			if(RTE_USART0_CHNL_UDMA_TX_EN ||RTE_UART1_CHNL_UDMA_TX_EN ||RTE_ULPUART_CHNL_UDMA_TX_EN)
			{
				//DISABLE DMa
				if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS == UART1))
				{
					UDMA0_ChannelDisable (usart->dma_tx->channel);
				}

				if(usart->pREGS == ULP_UART)
				{
					UDMA1_ChannelDisable (usart->dma_tx->channel);
				}
			}
		}
		if ((usart->dma_rx) && (usart->info->rx_status.rx_busy))
		{
			if(RTE_USART0_CHNL_UDMA_RX_EN ||RTE_UART1_CHNL_UDMA_RX_EN ||RTE_ULPUART_CHNL_UDMA_RX_EN)
			{
				//DISABLE DMa
				if((usart->pREGS == UART0)||(usart->pREGS == USART0) ||(usart->pREGS == UART1))
				{
					UDMA0_ChannelDisable (usart->dma_tx->channel);
				}

				if(usart->pREGS == ULP_UART)
				{
					UDMA1_ChannelDisable (usart->dma_tx->channel);
				}
			}
		}
		// Set trigger level
		if (usart->dma_rx || usart->dma_tx)
		{
			fcr |= USART_DMA_MODE_EN;
		}
		// Transmit and receive FIFO reset
		fcr = USART_FIFO_ENABLE;
		fcr |= USART_FIFO_RX_RESET | USART_FIFO_TX_RESET;
		usart->pREGS->FCR = fcr;

		// Clear busy statuses
		usart->info->rx_status.rx_busy = 0U;
		usart->info->xfer.send_active  = 0U;
		return ARM_DRIVER_OK;
	default: break;
	}
	switch (control & ARM_USART_CONTROL_Msk)
	{
	case ARM_USART_MODE_ASYNCHRONOUS:
		mode = ARM_USART_MODE_ASYNCHRONOUS;
		break;
	case ARM_USART_MODE_SYNCHRONOUS_MASTER:
		if(RTE_USART_MODE)
		{
			if(usart->pREGS == USART0)
			{
				if(usart->capabilities.synchronous_master)
				{
					//configure clock pin
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_CLK_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_USART0_CLK_PIN);
					RSI_EGPIO_SetPinMux(EGPIO, RTE_USART0_CLK_PORT,  RTE_USART0_CLK_PIN, RTE_USART0_CLK_MUX);

					//enable Sync mode
					usart->pREGS->SMCR_b.SYNC_MODE = 1;

					//enable master mode
					usart->pREGS->SMCR_b.MST_MODE = 1;

					//SYNC CKLCOK ENABLE
					if(RTE_CONTINUOUS_CLOCK_MODE)
					{
						usart->pREGS->SMCR_b.CONTI_CLK_MODE = 1U;
					}
					else
					{
						usart->pREGS->SMCR_b.CONTI_CLK_MODE = 0U;
					}
					usart->pREGS->SMCR_b.START_STOP_EN = 1;
				}
			}
			else
			{
				return ARM_USART_ERROR_MODE;
			}
		}
		else
		{
			return ARM_USART_ERROR_MODE;
		}
		mode = ARM_USART_MODE_SYNCHRONOUS_MASTER;

		break;
	case ARM_USART_MODE_SYNCHRONOUS_SLAVE:
		if(RTE_USART_MODE)
		{
			if(usart->pREGS == USART0)
			{
				if(usart->capabilities.synchronous_slave)
				{
					//configure clock pin
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_CLK_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_USART0_CLK_PIN);
					RSI_EGPIO_SetPinMux(EGPIO, RTE_USART0_CLK_PORT,  RTE_USART0_CLK_PIN, RTE_USART0_CLK_MUX);

					//enable sync mode
					usart->pREGS->SMCR_b.SYNC_MODE = 1;

					//enable slave mode
					usart->pREGS->SMCR_b.MST_MODE = 0;
					//SYNC CKLCOK ENABLE
					if(RTE_CONTINUOUS_CLOCK_MODE)
					{
						usart->pREGS->SMCR_b.CONTI_CLK_MODE = 1U;
					}
					else
					{
						usart->pREGS->SMCR_b.CONTI_CLK_MODE = 0U;
					}
					usart->pREGS->SMCR_b.START_STOP_EN = 1;
				}
			}
		}
		else
		{
			return ARM_USART_ERROR_MODE;
		}
		mode = ARM_USART_MODE_SYNCHRONOUS_SLAVE;
		break;

	case ARM_USART_MODE_SINGLE_WIRE:
		if(usart->capabilities.single_wire==1)
		{
			//!disable full duplex mode
			usart->pREGS->HDEN_b.FULL_DUPLEX_MODE = 1;
			if(usart->capabilities.synchronous_master)
			{
				usart->pREGS->HDEN_b.TX_MODE_RX_MODE = 0;
			}
			else
			{
				usart->pREGS->HDEN_b.TX_MODE_RX_MODE = 1;
			}
		}
		mode = ARM_USART_MODE_SINGLE_WIRE;
		break;
	case ARM_USART_MODE_IRDA:
		/*the number of data bits(8 DATA BITS) that can be sent is fixed. No parity information can be supplied,
	     and only one stop bit is used in this mode. Trying to adjust the number of data bits sent or enable
	     parity with the Line Control Register (LCR) has no effect*/
		if((usart->pREGS == UART0)||(usart->pREGS == USART0))
		{
			if (usart->capabilities.irda)
			{
				//IR TX PIN
				RSI_EGPIO_PadSelectionEnable(RTE_USART0_IR_TX_PAD);
				RSI_EGPIO_PadReceiverEnable(RTE_USART0_IR_TX_PIN);
				RSI_EGPIO_SetPinMux(EGPIO, RTE_USART0_IR_TX_PORT, RTE_USART0_IR_TX_PIN, RTE_USART0_IR_TX_MUX);

				//IR RX PIN
				RSI_EGPIO_PadSelectionEnable(RTE_USART0_IR_RX_PAD);
				RSI_EGPIO_PadReceiverEnable(RTE_USART0_IR_RX_PIN);
				RSI_EGPIO_SetPinMux(EGPIO, RTE_USART0_IR_RX_PORT, RTE_USART0_IR_RX_PIN, RTE_USART0_IR_RX_MUX);
			}
			if(usart->capabilities.irda==1)
			{
				//Enable SIR mode
				usart->pREGS->MCR_b.SIRE=1;
			}
			else
			{
				return ARM_USART_ERROR_MODE;
			}
		}
		else
		{
			return ARM_USART_ERROR_MODE;
		}
		mode =ARM_USART_MODE_IRDA;
		break;
	case ARM_USART_MODE_SMART_CARD:
		if(usart->capabilities.smart_card==1)
		{
			return ARM_USART_ERROR_MODE;
		}
		break;
	case ARM_USART_SET_DEFAULT_TX_VALUE:
		usart->info->xfer.tx_def_val = arg;
		return ARM_DRIVER_OK;

	case ARM_USART_SET_IRDA_PULSE:

		if(usart->capabilities.irda)
		{
			return ARM_USART_ERROR_MODE;
		}
		else
		{
			return ARM_USART_ERROR_MODE;;
		}
	case ARM_USART_SET_SMART_CARD_GUARD_TIME:
		if(usart->capabilities.smart_card)
		{
			return ARM_USART_ERROR_MODE;
		}
		break;
	case ARM_USART_SET_SMART_CARD_CLOCK:
		if(usart->capabilities.smart_card_clock)
		{
			return ARM_USART_ERROR_MODE;
		}
		break;
	case ARM_USART_CONTROL_SMART_CARD_NACK:
		if(usart->capabilities.smart_card)
		{
			return ARM_USART_ERROR_MODE;
		}
		break;
	default: return ARM_DRIVER_ERROR_UNSUPPORTED;
	}

	// USART Data bits
	switch (control & ARM_USART_DATA_BITS_Msk)
	{
	case ARM_USART_DATA_BITS_5:
		lcr |= USART_CNTL_DATALEN_5;
		break;
	case ARM_USART_DATA_BITS_6:
		lcr |= USART_CNTL_DATALEN_6;
		break;
	case ARM_USART_DATA_BITS_7:
		lcr |=USART_CNTL_DATALEN_7;
		break;
	case ARM_USART_DATA_BITS_8:
		lcr |=USART_CNTL_DATALEN_8;
		break;
	case ARM_USART_DATA_BITS_9:
		lcr_ext = USART_CNTL_DATALEN_9;
		lcr_ext |= (1 << 3);
		break;
	default: return ARM_USART_ERROR_DATA_BITS;
	}

	//USART Parity
	switch (control & ARM_USART_PARITY_Msk)
	{
	case ARM_USART_PARITY_NONE:
		lcr &= ~(USART_PARITY_ENABLE);
		break;
	case ARM_USART_PARITY_EVEN:
		lcr |= USART_PARITY_ENABLE;
		lcr |= USART_CNTL_PARITY_EVEN;
		break;
	case ARM_USART_PARITY_ODD:
		lcr |= USART_PARITY_ENABLE;
		lcr |= USART_CNTL_PARITY_ODD;
		break;
	default: return (ARM_USART_ERROR_PARITY);
	}

	// USART Stop bits
	switch (control & ARM_USART_STOP_BITS_Msk)
	{
	case ARM_USART_STOP_BITS_1:
		lcr |=USART_CNTL_STOPBIT_1;
		break;
	case ARM_USART_STOP_BITS_1_5:
	case ARM_USART_STOP_BITS_2:
		lcr |=USART_CNTL_STOPBIT_2;
		break;
	default: return ARM_USART_ERROR_STOP_BITS;
	}

#ifdef RTE_ENABLE_FIFO
	//enable FIFO mode
	fcr = USART_FIFO_ENABLE;

	// Set trigger level
	if (usart->dma_rx || usart->dma_tx)
	{
		if(RTE_USART0_DMA_MODE1_EN || RTE_UART1_DMA_MODE1_EN ||RTE_ULP_UART_DMA_MODE1_EN)
		{
			fcr |= USART_DMA_MODE1_EN;
		}
		else
		{
			fcr |= USART_DMA_MODE0_EN;
		}
	}
	if((usart->pREGS == UART0)||(usart->pREGS == USART0))
	{
		switch(RTE_USART0_TX_FIFO_THRESHOLD)
		{
		// Set the TX FIFO threshold level
		case  USART_TRIGGER_TX_EMPTY:
			fcr |= USART_FIFO_TX_EMPTY;
			break;
		case  USART_TRIGGER_TX_AEMPTY:
			fcr |=  USART_FIFO_TX_AEMPTY;
			break;
		case  USART_TRIGGER_TX_QUARTER_FULL:
			fcr |= USART_FIFO_TX_QUARTER_FULL;
			break;
		case USART_TRIGGER_TX_HALF_FULL:
			fcr |= USART_FIFO_TX_HALF_FULL;
			break;
		default:
			fcr |= USART_FIFO_TX_EMPTY;
			break;
		}
		switch(RTE_USART0_RX_FIFO_THRESHOLD)
		{
		// Set the RX FIFO threshold level
		case USART_TRIGGER_RX_AEMPTY:
			fcr |= USART_FIFO_RX_AEMPTY;
			break;
		case USART_TRIGGER_RX_QUARTER_FULL:
			fcr |= USART_FIFO_RX_QUARTER_FULL;
			break;
		case USART_TRIGGER_RX_HALF_FULL:
			fcr |= USART_FIFO_RX_HALF_FULL;
			break;
		case USART_TRIGGER_RX_AFULL:
			fcr |= USART_FIFO_RX_AFULL;
			break;
		default:
			fcr |= USART_FIFO_RX_AEMPTY;
			break;
		}
	}
	else if(usart->pREGS == UART1)
	{
		switch(RTE_UART1_TX_FIFO_THRESHOLD)
		{
		// Set the TX FIFO threshold level
		case  USART_TRIGGER_TX_EMPTY:
			fcr |= USART_FIFO_TX_EMPTY;
			break;
		case  USART_TRIGGER_TX_AEMPTY:
			fcr |=  USART_FIFO_TX_AEMPTY;
			break;
		case  USART_TRIGGER_TX_QUARTER_FULL:
			fcr |= USART_FIFO_TX_QUARTER_FULL;
			break;
		case USART_TRIGGER_TX_HALF_FULL:
			fcr |= USART_FIFO_TX_HALF_FULL;
			break;
		default:
			fcr |= USART_FIFO_TX_EMPTY;
			break;
		}
		switch(RTE_UART1_RX_FIFO_THRESHOLD)
		{
		// Set the RX FIFO threshold level
		case USART_TRIGGER_RX_AEMPTY:
			fcr |= USART_FIFO_RX_AEMPTY;
			break;
		case USART_TRIGGER_RX_QUARTER_FULL:
			fcr |= USART_FIFO_RX_QUARTER_FULL;
			break;
		case USART_TRIGGER_RX_HALF_FULL:
			fcr |= USART_FIFO_RX_HALF_FULL;
			break;
		case USART_TRIGGER_RX_AFULL:
			fcr |= USART_FIFO_RX_AFULL;
			break;
		default:
			fcr |= USART_FIFO_RX_AEMPTY;
			break;
		}
	}
	else if(usart->pREGS == ULP_UART)
	{
		switch(RTE_ULP_UART_TX_FIFO_THRESHOLD)
		{
		// Set the TX FIFO threshold level
		case  USART_TRIGGER_TX_EMPTY:
			fcr |= USART_FIFO_TX_EMPTY;
			break;
		case  USART_TRIGGER_TX_AEMPTY:
			fcr |=  USART_FIFO_TX_AEMPTY;
			break;
		case  USART_TRIGGER_TX_QUARTER_FULL:
			fcr |= USART_FIFO_TX_QUARTER_FULL;
			break;
		case USART_TRIGGER_TX_HALF_FULL:
			fcr |= USART_FIFO_TX_HALF_FULL;
			break;
		default:
			fcr |= USART_FIFO_TX_EMPTY;
			break;
		}
		switch(RTE_ULP_UART_RX_FIFO_THRESHOLD)
		{
		// Set the RX FIFO threshold level
		case USART_TRIGGER_RX_AEMPTY:
			fcr |= USART_FIFO_RX_AEMPTY;
			break;
		case USART_TRIGGER_RX_QUARTER_FULL:
			fcr |= USART_FIFO_RX_QUARTER_FULL;
			break;
		case USART_TRIGGER_RX_HALF_FULL:
			fcr |= USART_FIFO_RX_HALF_FULL;
			break;
		case USART_TRIGGER_RX_AFULL:
			fcr |= USART_FIFO_RX_AFULL;
			break;
		default:
			fcr |= USART_FIFO_RX_AEMPTY;
			break;
		}
	}
#endif
	// USART Flow control (RTS and CTS lines are only available on USART0)
	switch (control & ARM_USART_FLOW_CONTROL_Msk)
	{
	case ARM_USART_FLOW_CONTROL_NONE:
		break;
	case ARM_USART_FLOW_CONTROL_RTS:
		if (usart->capabilities.rts)
		{
			if((usart->pREGS == UART0)||(usart->pREGS == USART0))
			{
				if(RTE_USART0_RTS_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_USART0_RTS_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_USART0_RTS_PORT,(uint8_t)(RTE_USART0_RTS_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_RTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_RTS_PORT,RTE_USART0_RTS_PIN,RTE_USART0_RTS_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_RTS_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_USART0_RTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_RTS_PORT,RTE_USART0_RTS_PIN,RTE_USART0_RTS_MUX);
				}
			}
			if(usart->pREGS == UART1)
			{
				if(RTE_UART1_RTS_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_UART1_RTS_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_UART1_RTS_PORT,(uint8_t)(RTE_UART1_RTS_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_RTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_RTS_PORT,RTE_UART1_RTS_PIN,RTE_UART1_RTS_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_RTS_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_UART1_RTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_RTS_PORT,RTE_UART1_RTS_PIN,RTE_UART1_RTS_MUX);
				}
			}

			if(usart->pREGS == ULP_UART)
			{
				RSI_EGPIO_UlpPadReceiverEnable(RTE_ULP_UART_RTS_PIN);
				RSI_EGPIO_SetPinMux(EGPIO,RTE_ULP_UART_RTS_PORT,RTE_ULP_UART_RTS_PIN,RTE_ULP_UART_RTS_MUX);
			}
		}
		if (usart->capabilities.flow_control_rts)
		{
			usart ->pREGS->MCR |= (USART_MODEM_AFCE_ENABLE|USART_MODEM_RTS_SET);
		}
		else
		{
			return ARM_USART_ERROR_FLOW_CONTROL;
		}
		break;
	case ARM_USART_FLOW_CONTROL_CTS:

		if(usart->capabilities.cts)
		{
			if((usart->pREGS == UART0)||(usart->pREGS == USART0))
			{
				if(RTE_USART0_RTS_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_USART0_CTS_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_USART0_CTS_PORT,(uint8_t)(RTE_USART0_CTS_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_CTS_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_CTS_PORT,RTE_USART0_CTS_PIN,RTE_USART0_CTS_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_CTS_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_USART0_CTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_CTS_PORT,RTE_USART0_CTS_PIN,RTE_USART0_CTS_MUX);
				}
			}
			if(usart->pREGS == UART1)
			{
				if(RTE_UART1_RTS_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_UART1_CTS_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_UART1_CTS_PORT,(uint8_t)(RTE_UART1_CTS_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_CTS_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_CTS_PORT,RTE_UART1_CTS_PIN,RTE_USART0_CTS_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_CTS_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_UART1_CTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_CTS_PORT,RTE_UART1_CTS_PIN,RTE_UART1_CTS_MUX);
				}
			}

			if(usart->pREGS == ULP_UART)
			{
				RSI_EGPIO_UlpPadReceiverEnable(RTE_ULP_UART_CTS_PIN);
				RSI_EGPIO_SetPinMux(EGPIO,RTE_ULP_UART_CTS_PORT,RTE_ULP_UART_CTS_PIN,RTE_ULP_UART_CTS_MUX);
			}
		}
		if (usart->capabilities.flow_control_cts)
		{
			usart ->pREGS->MCR |= (USART_MODEM_AFCE_ENABLE);
		}
		else
		{
			return ARM_USART_ERROR_FLOW_CONTROL;
		}
		break;
	case ARM_USART_FLOW_CONTROL_RTS_CTS:

		if(usart->capabilities.cts && usart->capabilities.rts)
		{
			if((usart->pREGS == UART0)||(usart->pREGS == USART0))
			{
				if((RTE_USART0_RTS_PIN > 63) || (RTE_USART0_CTS_PIN > 63))
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_USART0_RTS_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_USART0_RTS_PORT,(uint8_t)(RTE_USART0_RTS_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_RTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_RTS_PORT,RTE_USART0_RTS_PIN,RTE_USART0_RTS_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_RTS_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_USART0_RTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_RTS_PORT,RTE_USART0_RTS_PIN,RTE_USART0_RTS_MUX);
				}
				if(RTE_USART0_RTS_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_USART0_CTS_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_USART0_CTS_PORT,(uint8_t)(RTE_USART0_CTS_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_CTS_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_CTS_PORT,RTE_USART0_CTS_PIN,RTE_USART0_CTS_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_USART0_CTS_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_USART0_CTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_USART0_CTS_PORT,RTE_USART0_CTS_PIN,RTE_USART0_CTS_MUX);
				}
			}
			if(usart->pREGS == UART1)
			{
				if(RTE_UART1_RTS_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_UART1_RTS_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_UART1_RTS_PORT,(uint8_t)(RTE_UART1_RTS_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_RTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_RTS_PORT,RTE_UART1_RTS_PIN,RTE_UART1_RTS_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_RTS_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_UART1_RTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_RTS_PORT,RTE_UART1_RTS_PIN,RTE_UART1_RTS_MUX);
				}
				if(RTE_UART1_RTS_PIN > 63)
				{
					RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_UART1_CTS_PIN-64));
					RSI_EGPIO_SetPinMux(EGPIO1,RTE_UART1_CTS_PORT,(uint8_t)(RTE_UART1_CTS_PIN-64),6);
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_CTS_PAD);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_CTS_PORT,RTE_UART1_CTS_PIN,RTE_USART0_CTS_MUX);
				}
				else
				{
					RSI_EGPIO_PadSelectionEnable(RTE_UART1_CTS_PAD);
					RSI_EGPIO_PadReceiverEnable(RTE_UART1_CTS_PIN);
					RSI_EGPIO_SetPinMux(EGPIO,RTE_UART1_CTS_PORT,RTE_UART1_CTS_PIN,RTE_UART1_CTS_MUX);
				}
			}

			if(usart->pREGS == ULP_UART)
			{
				//RTS
				RSI_EGPIO_UlpPadReceiverEnable(RTE_ULP_UART_RTS_PIN);
				RSI_EGPIO_SetPinMux(EGPIO,RTE_ULP_UART_RTS_PORT,RTE_ULP_UART_RTS_PIN,RTE_ULP_UART_RTS_MUX);

				//CTS
				RSI_EGPIO_UlpPadReceiverEnable(RTE_ULP_UART_CTS_PIN);
				RSI_EGPIO_SetPinMux(EGPIO,RTE_ULP_UART_CTS_PORT,RTE_ULP_UART_CTS_PIN,RTE_ULP_UART_CTS_MUX);
			}
		}
		if (usart->capabilities.flow_control_rts && usart->capabilities.flow_control_cts)
		{
			usart->pREGS->MCR |= (USART_MODEM_AFCE_ENABLE|USART_MODEM_RTS_SET);
		}
		else
		{
			return ARM_USART_ERROR_FLOW_CONTROL;
		}
		break;
	default:
	{
		return ARM_USART_ERROR_FLOW_CONTROL;
	}
	}

	// Clock setting for synchronous mode
	if ((mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||(mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE ))
	{
		// Only CPOL0 - CPHA1 combination available
		// USART clock polarity
		if ((control & ARM_USART_CPOL_Msk) != ARM_USART_CPOL0)
		{
			return ARM_USART_ERROR_CPOL;
		}
		// USART clock phase
		if ((control & ARM_USART_CPHA_Msk) != ARM_USART_CPHA0)
		{
			return ARM_USART_ERROR_CPHA;
		}
	}
	if((RTE_USART0_LOOPBACK )||(RTE_UART1_LOOPBACK) ||(RTE_ULP_UART_LOOPBACK))
	{
		/* Set Loop back flag if loop back is enabled */
		usart->pREGS->MCR  |= (USART_MODEM_LOOP_BACK_ENABLE);
	}
	else
	{
		usart->pREGS->MCR &= ~(USART_MODEM_LOOP_BACK_ENABLE);
	}
	if(RTE_USART0_DTR_EANBLE)
	{
		//Enable DTR bit in MCR reg
		usart->pREGS->MCR_b.DTR = 0x1;
	}
	// baud rate calculation
	USART_SetBaudrate (arg,baseClk,usart);

	// Configuration is OK - Mode is valid
	usart->info->mode = mode;
	usart->pREGS->FCR = fcr;
	usart->pREGS->LCR = lcr;
	usart->pREGS->LCR_EXT = lcr_ext;

	// Set configured flag
	usart->info->flags |= USART_FLAG_CONFIGURED;

	return ARM_DRIVER_OK;
}

/**
  @fn          ARM_USART_STATUS USART_GetStatus (USART_RESOURCES *usart)
  @brief       Get USART status.
  @param[in]   usart     Pointer to USART resources
  @return      USART status \ref ARM_USART_STATUS
 */
static ARM_USART_STATUS USART_GetStatus (USART_RESOURCES *usart)
{
	ARM_USART_STATUS stat;
	stat.tx_busy          = (usart->pREGS->LSR & USART_LCR_TEMT ? (0U) : (1U));
	stat.rx_busy          = usart->info->rx_status.rx_busy;
	stat.tx_underflow     = 0U;
	stat.rx_overflow      = usart->info->rx_status.rx_overflow;
	stat.rx_break         = usart->info->rx_status.rx_break;
	stat.rx_framing_error = usart->info->rx_status.rx_framing_error;
	stat.rx_parity_error  = usart->info->rx_status.rx_parity_error;
	return stat;
}

/**
  @fn          static int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL  control,USART_RESOURCES *usart)
  @brief       Set USART Modem Control line state.
  @param[in]   control   \ref ARM_USART_MODEM_CONTROL
  @param[in]   usart     Pointer to USART resources
  @return      \ref execution_status
 */
static int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL  control,USART_RESOURCES *usart)
{
	if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U)
	{
		// USART is not configured
		return ARM_DRIVER_ERROR;
	}
	// Only UART1 supports modem lines
	if (usart->pREGS == NULL)
	{
		return ARM_DRIVER_ERROR_UNSUPPORTED;
	}

	if (control == ARM_USART_RTS_CLEAR)
	{
		if (usart->capabilities.rts)
		{
			usart->pREGS->MCR &= ~USART_MODEM_RTS_SET;
		}
		else
		{ return ARM_DRIVER_ERROR_UNSUPPORTED;
		}
	}
	if (control == ARM_USART_RTS_SET)
	{
		if (usart->capabilities.rts)
		{
			usart->pREGS->MCR |=  USART_MODEM_RTS_SET;
		}
		else
		{
			return ARM_DRIVER_ERROR_UNSUPPORTED;
		}
	}
	if (control == ARM_USART_DTR_CLEAR)
	{
		if((usart->pREGS == UART0)||(usart->pREGS == USART0))
		{
			if (usart->capabilities.dtr)
			{
				usart->pREGS->MCR &= ~USART_MODEM_DTR_SET;
			}
			else
			{
				return ARM_DRIVER_ERROR_UNSUPPORTED;
			}
		}
		else
		{
			return ARM_DRIVER_ERROR_UNSUPPORTED;
		}
	}
	if (control == ARM_USART_DTR_SET)
	{
		if((usart->pREGS == UART0)||(usart->pREGS == USART0))
		{
			if (usart->capabilities.dtr)
			{
				usart->pREGS->MCR |=  USART_MODEM_DTR_SET;
			}
			else
			{
				return ARM_DRIVER_ERROR_UNSUPPORTED;
			}
		}
		else
		{
			return ARM_DRIVER_ERROR_UNSUPPORTED;
		}
	}
	return ARM_DRIVER_OK;
}

/**
  @fn          ARM_USART_MODEM_STATUS USART_GetModemStatus (USART_RESOURCES *usart)
  @brief       Get USART Modem Status lines state.
  @param[in]   usart     Pointer to USART resources
  @return      modem status \ref ARM_USART_MODEM_STATUS
 */
static ARM_USART_MODEM_STATUS USART_GetModemStatus (USART_RESOURCES *usart)
{
	ARM_USART_MODEM_STATUS modem_status;
	uint32_t msr;

	if (usart->pREGS &&
			(usart->info->flags & USART_FLAG_CONFIGURED))
	{
		msr = usart->pREGS->MSR;

		if (usart->capabilities.cts)
		{
			if(msr & UART_MSR_CTS)
			{
				modem_status.cts=1U;
			}
			else
			{
				modem_status.cts=0U;
			}
		}
		else
		{
			modem_status.cts = 0U;
		}
		if (usart->capabilities.dsr)
		{
			if(msr & UART_MSR_DSR)
			{
				modem_status.dsr=1U;
			}
			else
			{
				modem_status.dsr=0U;
			}
		}
		else
		{
			modem_status.dsr = 0U;
		}
		if (usart->capabilities.ri )
		{
			if(msr & UART_MSR_RI)
			{
				modem_status.ri=1U;
			}
			else
			{
				modem_status.ri=0U;
			}
		}
		else
		{
			modem_status.ri  = 0U;
		}
		if (usart->capabilities.dcd)
		{
			if(msr & UART_MSR_DCD)
			{
				modem_status.dcd=1U;
			}
			else
			{
				modem_status.dcd=0U;
			}
		}
		else
		{
			modem_status.dcd = 0U;
		}
	}
	else
	{
		modem_status.cts = 0U;
		modem_status.dsr = 0U;
		modem_status.ri  = 0U;
		modem_status.dcd = 0U;
	}
	return modem_status;
}
static ARM_DRIVER_VERSION ARM_USARTx_GetVersion(void)
{
	return UsartDriverVersion;
}
//USART0
#if(RTE_USART0)

static ARM_USART_CAPABILITIES ARM_USART0_GetCapabilities (void)
{
	return USART_GetCapabilities (&USART0_Resources);
}
static int32_t ARM_USART0_Initialize (ARM_USART_SignalEvent_t cb_event)
{
	return USART_Initialize (cb_event, &USART0_Resources);
}
static int32_t ARM_USART0_Uninitialize (void)
{
	return USART_Uninitialize(&USART0_Resources);
}
static int32_t ARM_USART0_PowerControl (ARM_POWER_STATE state)
{
	return USART_PowerControl (state, &USART0_Resources);
}
static int32_t ARM_USART0_Send (const void *data, uint32_t num)
{
	return USART_Send_Data (data, num, &USART0_Resources);
}
static int32_t ARM_USART0_Receive (void *data, uint32_t num)
{
	return USART_Receive_Data ( data, num,&USART0_Resources);
}
static int32_t ARM_USART0_Transfer (const void *data_out, void *data_in, uint32_t num)
{
	return USART_Transfer (data_out, data_in, num, &USART0_Resources);
}
static uint32_t ARM_USART0_GetTxCount (void)
{
	return USART_GetTxCount (&USART0_Resources);
}
static uint32_t ARM_USART0_GetRxCount (void)
{
	return USART_GetRxCount (&USART0_Resources);
}
static int32_t ARM_USART0_Control (uint32_t control, uint32_t arg)
{
  uint32_t usart0_get_clock=0;
  usart0_get_clock = RSI_CLK_GetBaseClock(M4_USART0);
	return USART_Control (control, arg, usart0_get_clock, &USART0_Resources);
}
static ARM_USART_STATUS ARM_USART0_GetStatus (void)
{
	return USART_GetStatus(&USART0_Resources);
}
static int32_t ARM_USART0_SetModemControl (ARM_USART_MODEM_CONTROL control)
{
	return USART_SetModemControl (control, &USART0_Resources);
}
static ARM_USART_MODEM_STATUS ARM_USART0_GetModemStatus (void)
{
	return USART_GetModemStatus (&USART0_Resources);
}
void RSI_M4SSUsart0Handler(void)
{
	UartIrqHandler(&USART0_Resources);
}
#if (RTE_USART0_CHNL_UDMA_TX_EN == 1)
void USART0_UDMA_Tx_Event (uint32_t event, uint8_t dmaCh)
{
	USART_UDMA_Tx_Event (event,dmaCh, &USART0_Resources);
}
#endif

#if (RTE_USART0_CHNL_UDMA_RX_EN == 1)
void USART0_UDMA_Rx_Event (uint32_t event,uint8_t dmaCh)
{
	USART_UDMA_Rx_Event (event,dmaCh, &USART0_Resources);
}
#endif

// USART driver functions structure
ARM_DRIVER_USART Driver_USART0 =
{
		ARM_USARTx_GetVersion,
		ARM_USART0_GetCapabilities,
		ARM_USART0_Initialize,
		ARM_USART0_Uninitialize,
		ARM_USART0_PowerControl,
		ARM_USART0_Send,
		ARM_USART0_Receive,
		ARM_USART0_Transfer,
		ARM_USART0_GetTxCount,
		ARM_USART0_GetRxCount,
		ARM_USART0_Control,
		ARM_USART0_GetStatus,
		ARM_USART0_SetModemControl,
		ARM_USART0_GetModemStatus
};
// USART_IRQ_HANDLER
void USART0_IRQ_HANDLER (void)
{
	UartIrqHandler(&USART0_Resources);
}
#endif

//UART1
#if(RTE_UART1)

static ARM_USART_CAPABILITIES ARM_UART1_GetCapabilities (void)
{
	return USART_GetCapabilities (&UART1_Resources);
}
static int32_t ARM_UART1_Initialize (ARM_USART_SignalEvent_t cb_event)
{
	return USART_Initialize (cb_event, &UART1_Resources);
}
static int32_t ARM_UART1_Uninitialize (void)
{
	return USART_Uninitialize(&UART1_Resources);
}
static int32_t ARM_UART1_PowerControl (ARM_POWER_STATE state)
{
	return USART_PowerControl (state, &UART1_Resources);
}
static int32_t ARM_UART1_Send (const void *data, uint32_t num)
{
	return USART_Send_Data (data, num, &UART1_Resources);
}
static int32_t ARM_UART1_Receive (void *data, uint32_t num)
{
	return USART_Receive_Data ( data, num,&UART1_Resources);
}
static int32_t ARM_UART1_Transfer (const void *data_out,void *data_in,uint32_t num)
{
	return USART_Transfer (data_out, data_in, num, &UART1_Resources);
}
static uint32_t ARM_UART1_GetTxCount (void)
{
	return USART_GetTxCount (&UART1_Resources);
}
static uint32_t ARM_UART1_GetRxCount (void)
{
	return USART_GetRxCount (&UART1_Resources);
}
static int32_t ARM_UART1_Control (uint32_t control, uint32_t arg)
{
  uint32_t uart1_get_clock;
  uart1_get_clock = RSI_CLK_GetBaseClock(M4_UART1);
	return USART_Control (control, arg, uart1_get_clock, &UART1_Resources);
}
static ARM_USART_STATUS ARM_UART1_GetStatus (void)
{
	return USART_GetStatus(&UART1_Resources);
}
static int32_t ARM_UART1_SetModemControl (ARM_USART_MODEM_CONTROL control)
{
	return USART_SetModemControl (control, &UART1_Resources);
}
static ARM_USART_MODEM_STATUS ARM_UART1_GetModemStatus (void)
{
	return USART_GetModemStatus (&UART1_Resources);
}
void RSI_M4SSUart1Handler(void)
{
	UartIrqHandler(&UART1_Resources);
}
#if (RTE_UART1_CHNL_UDMA_TX_EN == 1)
void UART1_UDMA_Tx_Event (uint32_t event,uint8_t dmaCh)
{
	USART_UDMA_Tx_Event (event,dmaCh,&UART1_Resources);
}
#endif

#if (RTE_UART1_CHNL_UDMA_RX_EN == 1)
void UART1_UDMA_Rx_Event (uint32_t event,uint8_t dmaCh)
{
	USART_UDMA_Rx_Event (event,dmaCh, &UART1_Resources);
}
#endif
// USART driver functions structure
ARM_DRIVER_USART Driver_UART1 = {
		ARM_USARTx_GetVersion,
		ARM_UART1_GetCapabilities,
		ARM_UART1_Initialize,
		ARM_UART1_Uninitialize,
		ARM_UART1_PowerControl,
		ARM_UART1_Send,
		ARM_UART1_Receive,
		ARM_UART1_Transfer,
		ARM_UART1_GetTxCount,
		ARM_UART1_GetRxCount,
		ARM_UART1_Control,
		ARM_UART1_GetStatus,
		ARM_UART1_SetModemControl,
		ARM_UART1_GetModemStatus
};

// USART_IRQ_HANDLER
void UART1_IRQ_HANDLER (void)
{
	UartIrqHandler(&UART1_Resources);
}
#endif

//ULP_UART
#if(RTE_ULP_UART)
static ARM_USART_CAPABILITIES ARM_ULP_UART_GetCapabilities (void)
{
	return USART_GetCapabilities (&ULP_UART_Resources);
}
static int32_t ARM_ULP_UART_Initialize (ARM_USART_SignalEvent_t cb_event)
{
	return USART_Initialize (cb_event, &ULP_UART_Resources);
}
static int32_t ARM_ULP_UART_Uninitialize (void)
{
	return USART_Uninitialize(&ULP_UART_Resources);
}
static int32_t ARM_ULP_UART_PowerControl (ARM_POWER_STATE state)
{
	return USART_PowerControl (state, &ULP_UART_Resources);
}
static int32_t ARM_ULP_UART_Send (const void *data, uint32_t num)
{
	return USART_Send_Data (data, num, &ULP_UART_Resources);
}
static int32_t ARM_ULP_UART_Receive (void *data, uint32_t num)
{
	return USART_Receive_Data ( data, num,&ULP_UART_Resources);
}
static int32_t ARM_ULP_UART_Transfer (const void *data_out,void *data_in,uint32_t num)
{
	return USART_Transfer (data_out, data_in, num, &ULP_UART_Resources);
}
static uint32_t ARM_ULP_UART_GetTxCount (void)
{
	return USART_GetTxCount (&ULP_UART_Resources);
}
static uint32_t ARM_ULP_UART_GetRxCount (void)
{
	return USART_GetRxCount (&ULP_UART_Resources);
}
static int32_t ARM_ULP_UART_Control (uint32_t control, uint32_t arg)
{
  uint32_t ulp_uart_get_clock;
  ulp_uart_get_clock = RSI_CLK_GetBaseClock(ULPSS_UART);
	return USART_Control (control, arg, ulp_uart_get_clock, &ULP_UART_Resources);
}
static ARM_USART_STATUS ARM_ULP_UART_GetStatus (void)
{
	return USART_GetStatus(&ULP_UART_Resources);
}
static int32_t ARM_ULP_UART_SetModemControl (ARM_USART_MODEM_CONTROL control)
{
	return USART_SetModemControl (control, &ULP_UART_Resources);
}
static ARM_USART_MODEM_STATUS ARM_ULP_UART_GetModemStatus (void)
{
	return USART_GetModemStatus (&ULP_UART_Resources);
}
void RSI_ULPUartHandler(void)
{
	UartIrqHandler(&ULP_UART_Resources);
}

#if (RTE_ULPUART_CHNL_UDMA_TX_EN == 1)
void ULPUART_UDMA_Tx_Event (uint32_t event,uint8_t dmaCh)
{
	USART_UDMA_Tx_Event (event,dmaCh, &ULP_UART_Resources);
}
#endif

#if (RTE_ULPUART_CHNL_UDMA_RX_EN == 1)
void ULPUART_UDMA_Rx_Event (uint32_t event,uint8_t dmaCh)
{
	USART_UDMA_Rx_Event (event,dmaCh, &ULP_UART_Resources);
}
#endif
// USART driver functions structure
ARM_DRIVER_USART Driver_ULP_UART = {
		ARM_USARTx_GetVersion,
		ARM_ULP_UART_GetCapabilities,
		ARM_ULP_UART_Initialize,
		ARM_ULP_UART_Uninitialize,
		ARM_ULP_UART_PowerControl,
		ARM_ULP_UART_Send,
		ARM_ULP_UART_Receive,
		ARM_ULP_UART_Transfer,
		ARM_ULP_UART_GetTxCount,
		ARM_ULP_UART_GetRxCount,
		ARM_ULP_UART_Control,
		ARM_ULP_UART_GetStatus,
		ARM_ULP_UART_SetModemControl,
		ARM_ULP_UART_GetModemStatus
};

// USART_IRQ_HANDLER
void ULP_UART_IRQ_HANDLER (void)
{
	UartIrqHandler(&ULP_UART_Resources);
}
#endif
