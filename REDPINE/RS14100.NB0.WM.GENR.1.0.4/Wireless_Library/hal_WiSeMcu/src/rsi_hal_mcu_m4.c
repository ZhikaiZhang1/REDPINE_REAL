/**
 * @file      rsi_hal_mcu_m4.c
 * @version   0.1
 * @date      15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains common(protocol independent) functionality
 *
 *  @section Description  This file contains common(protocol independent) functionality
 *
 *
 */

#ifdef RSI_M4_INTERFACE
#include "rsi_driver.h"
#include "rsi_m4.h"
#ifdef ROM_WIRELESS
#include "rsi_board.h"
#endif

rsi_m4ta_desc_t tx_desc[2];
rsi_m4ta_desc_t rx_desc[2];

uint32_t NVIC_GetIRQEnable(IRQn_Type IRQn)
{
	return ((NVIC->ICER[((uint32_t)(IRQn) >> 5)] & (1 << ((uint32_t)(IRQn) & 0x1F))) ? 1: 0);
}

void rsi_assertion(uint16_t assertion_val, const char *string )
{
	uint16_t i;
  if(assertion_val == 0)
	{
		for (i = 0; i < strlen(string); i++) 
		{
#ifdef DEBUG_UART
#ifdef DEBUG_ASSERTION
	  	Board_UARTPutChar(string[i]);
#endif
#endif
  		}

		return;
	}
	else
	{
		for (i = 0; i < strlen(string); i++) 
		{
#ifdef DEBUG_UART		
#ifdef DEBUG_ASSERTION
	  	Board_UARTPutChar(string[i]);
#endif		
#endif		
  	}

		while(1);
	}
	
}

void IRQ074_Handler()
{
#ifdef ROM_WIRELESS
   ROMAPI_WL->rsi_m4_interrupt_isr(global_cb_p);
#else
   api_wl->rsi_m4_interrupt_isr(global_cb_p);
#endif
}

/*==============================================*/
/**
 * @fn           void rsi_raise_pkt_pending_interrupt_to_ta()
 * @brief        This function raises the packet pending interrupt to TA
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * This function raises the packet pending interrupt to TA
 *
 *
 */
void rsi_m4_ta_interrupt_init()
{
  //! Unmask the interrupt
  unmask_ta_interrupt(TX_PKT_TRANSFER_DONE_INTERRUPT  | RX_PKT_TRANSFER_DONE_INTERRUPT);

  P2P_STATUS_REG |= M4_is_active;

  *(volatile uint32_t *)0xE000E108 = 0x00000400;
   return;  
}


void mask_ta_interrupt(uint32_t interrupt_no)
{
#ifdef ROM_WIRELESS
   ROMAPI_WL->mask_ta_interrupt(interrupt_no);
#else
   api_wl->mask_ta_interrupt(interrupt_no);
#endif
}

void unmask_ta_interrupt(uint32_t interrupt_no)
{
#ifdef ROM_WIRELESS
   ROMAPI_WL->unmask_ta_interrupt(interrupt_no);
#else
   api_wl->unmask_ta_interrupt(interrupt_no);
#endif
}

int rsi_submit_rx_pkt()
{
#ifdef ROM_WIRELESS
   return ROMAPI_WL->rsi_submit_rx_pkt(global_cb_p);
#else
   return api_wl->rsi_submit_rx_pkt(global_cb_p);
#endif
}




/*====================================================*/
/**
 * @fn          int16_t rsi_frame_read(uint8_t *pkt_buffer)
 * @brief This  function is used to read the response from module.
 * @param[in]   uint8_t *pkt_buffer, pointer to the buffer to which packet has to read
 *              which is used to store the response from the module
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = SPI Failure
 *              0  = SUCCESS
 * @section description 
 * This is a common function to read response for all the command and data from Wi-Fi module.
 */


rsi_pkt_t* rsi_frame_read()
{
#ifdef ROM_WIRELESS
   return ROMAPI_WL->rsi_frame_read(global_cb_p);
#else
   return api_wl->rsi_frame_read(global_cb_p);
#endif
}


/*====================================================*/
/**
 * @fn          int16_t rsi_frame_write(rsi_frame_desc_t *uFrameDscFrame,uint8_t *payloadparam,uint16_t size_param)
 * @brief       Common function for all the commands.
 * @param[in]   uFrameDsc uFrameDscFrame, frame descriptor
 * @param[in]   uint8_t *payloadparam, pointer to the command payload parameter structure
 * @param[in]   uint16_t size_param, size of the payload for the command
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = SPI Failure
 *              0  = SUCCESS
 * @section description 
 * This is a common function used to process a command to the Wi-Fi module.
 */



int16_t rsi_frame_write(rsi_frame_desc_t *uFrameDscFrame,uint8_t *payloadparam,uint16_t size_param)
{
#ifdef ROM_WIRELESS
   return ROMAPI_WL->rsi_frame_write(global_cb_p, uFrameDscFrame, payloadparam, size_param);
#else
   return api_wl->rsi_frame_write(global_cb_p, uFrameDscFrame, payloadparam, size_param);
#endif
}
#endif
/*==============================================*/
/**
 * @fn           void rsi_update_tx_dma_desc(uint8 skip_dma_valid)
 * @brief        This function updates the TX DMA descriptor address
 * @param[in]    skip_dma_valid 
 * @param[out]   none
 * @return       none
 * @section description
 * This function updates the TX DMA descriptor address
 *
 *
 */

void rsi_update_tx_dma_desc(uint8_t skip_dma_valid)
{
  if(!skip_dma_valid)
  {
    while(M4_TX_DMA_DESC_REG & DMA_DESC_REG_VALID);
  }
  M4_TX_DMA_DESC_REG = (uint32_t)&tx_desc;
}

/*==============================================*/
/**
 * @fn           void rsi_update_rx_dma_desc()
 * @brief        This function updates the RX DMA descriptor address
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * This function updates the RX DMA descriptor address
 *
 *
 */
void rsi_update_rx_dma_desc()
{
	M4_RX_DMA_DESC_REG = (uint32_t)&rx_desc;
}




/*==============================================*/
/**
 * @fn           void rsi_config_m4_dma_desc_on_reset()
 * @brief        This function updates the RX DMA and TX DMA descriptor address after reset
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * This function updates the RX DMA  and TX DMA descriptor address
 *
 *
 */
void rsi_config_m4_dma_desc_on_reset()
{
  //! Wait for TA to go to sleep
  while(P2P_STATUS_REG & TA_is_active);
  //! Wait for TA to wakeup and should be in bootloader
  while(!(P2P_STATUS_REG & TA_is_active));
  //! UPdate M4 TX and RX dma descriptors
  M4_TX_DMA_DESC_REG = (uint32_t)&tx_desc;
  M4_RX_DMA_DESC_REG = (uint32_t)&rx_desc;
}


/*==================================================*/
/**
 * @fn          int16_t rsi_device_interrupt_status(uint8_t *int_status)
 * @brief       Returns the value of the Interrupt register
 * @param[in]   status
 * @param[out]  buffer full status reg value 
 * @return      errorcode
 *               0 = Success
 *              -2 = Reg read failure  
 */
int16_t rsi_device_interrupt_status(uint8_t *int_status)
{

  //! Check for TA active .If it is not active Buffer full status is not valid,
  //! SO return fail from here
  if(!(P2P_STATUS_REG & TA_is_active))
  {
    return RSI_FAIL;
  }
  //! copy buffer full status reg value 
  *int_status = (uint8_t)HOST_INTR_STATUS_REG;

  return RSI_SUCCESS;

}


