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
#ifndef ROM_WIRELESS
#include "rsi_driver.h"
#include "rsi_m4.h"
#include <core_cm4.h>

/*==============================================*/
/**
 * @fn           void ROM_WL_rsi_m4_interrupt_isr(global_cb_t *global_cb_p)
 * @brief        This function is called when 
 * @param[in]    global_cb_p,pointer to the global control block
 * @param[out]   none
 * @return       none
 * @section description
 * This function raises the packet pending interrupt to TA
 *
 *
 */

void ROM_WL_rsi_m4_interrupt_isr(global_cb_t *global_cb_p)
{

	if(TASS_P2P_INTR_CLEAR & TX_PKT_TRANSFER_DONE_INTERRUPT)
	{

		//! Call done interrupt isr
		ROM_WL_rsi_transfer_to_ta_done_isr(global_cb_p);

		//! Clear the interrupt
   		ROM_WL_clear_ta_to_m4_interrupt(TX_PKT_TRANSFER_DONE_INTERRUPT);
	
		
	}
	else if(TASS_P2P_INTR_CLEAR & RX_PKT_TRANSFER_DONE_INTERRUPT)
  {
    
   
		//! Call done interrupt isr
		ROM_WL_rsi_receive_from_ta_done_isr(global_cb_p);

       //! Clear the interrupt
       ROM_WL_clear_ta_to_m4_interrupt(RX_PKT_TRANSFER_DONE_INTERRUPT);

		
	}
	else
	{
      RSI_ASSERTION(SAPIS_M4_ISR_UNEXPECTED_INTR, "\nM4 ISR , unexpected interrupt  \n");
		
	}
	
	
  
  return;  
}


/*==============================================*/
/**
 * @fn           void ROM_WL_rsi_raise_pkt_pending_interrupt_to_ta(void)
 * @brief        This function raises the packet pending interrupt to TA
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * This function raises the packet pending interrupt to TA
 *
 *
 */
void ROM_WL_rsi_raise_pkt_pending_interrupt_to_ta(void)
{

  //! Write the packet pending interrupt to TA register
  M4SS_P2P_INTR_SET_REG = TX_PKT_PENDING_INTERRUPT;
  
  return;  
}






/*==============================================*/
/**
 * @fn          int32_t ROM_WL_rsi_send_pkt_to_ta(rsi_m4ta_desc_t *tx_desc)
 * @brief       This function programmes the shared memory between the M4 and
 *              TA to send packet to TA
 * @param[in]   shared memory descriptors
 * @return      0 - Success , -1 - failure
 * @section description
 * This function programmes the shared memory between the M4 and
 *              TA to send packet to TA
 *
 *
 */
int32_t ROM_WL_rsi_send_pkt_to_ta(rsi_m4ta_desc_t *tx_desc)
{
	 
    //!raise interrupt to TA 
    ROM_WL_rsi_raise_pkt_pending_interrupt_to_ta();
	   
	  //! return success
	  return 0;

    

}


/*==============================================*/
/**
 * @fn           void ROM_WL_rsi_transfer_to_ta_done_isr(global_cb_t *global_cb_p)
 * @brief        This function is called when TA has read the
 *               memory content in the shared memory and raises 
 *               the interrupt to M4
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[out]   None
 * @return       NOne
 * @section description
 * This function is called when TA has read the
 * memory content in the shared memory and raises 
 * the interrupt to M4
 *
 *
 */
void ROM_WL_rsi_transfer_to_ta_done_isr(global_cb_t *global_cb_p)
{ 
    //! Mask TX Event
	  ROM_WL_rsi_unmask_event(global_cb_p, RSI_TX_EVENT);
}





/*==============================================*/
/**
 *@fn             rsi_pkt_t* ROM_WL_rsi_get_rx_pkt(global_cb_t *global_cb_p)
 * @brief        This function is called when ever M4 has a packet to read from TA
 * @param[in]    global_cb_p,pointer to the global control block
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * TA raises an interrupt to M4 when there is a packet from TA to read for M4  
 *
 */
rsi_pkt_t* ROM_WL_rsi_get_rx_pkt(global_cb_t *global_cb_p)
{

  rsi_m4ta_desc_t *rx_desc       = global_cb_p->rx_desc; 

  if(rx_desc[0].addr == (uint32_t )NULL)
  {
   
    RSI_ASSERTION(SAPIS_M4_RX_BUFF_ADDR_NULL, "\nIn get  rx pkt,Rx Buffer in rx dma desc is NULL\n");

    //! ASSERTION
    return NULL;

  }

  return  ((rsi_pkt_t *)((rx_desc[0].addr - M4_MEMORY_OFFSET_ADDRESS) - 4));
  

}


/*==============================================*/
/** 
 * @fn           void ROM_WL_rsi_receive_from_ta_done_isr(global_cb_t *global_cb_p)
 * @brief        This function is called when DMA done for RX packet is received
 * @param[in]    global_cb_p,pointer to the global control block
 * @param[in]    none
 * @param[out]   none 
 * @return       none
 *
 *
 * @section description
 * This function is called when DMA done for RX packet is received
 *
 */
void ROM_WL_rsi_receive_from_ta_done_isr(global_cb_t *global_cb_p)
{

  rsi_pkt_t *rx_pkt = NULL;
  rsi_driver_cb_t *rsi_driver_cb = global_cb_p->rsi_driver_cb;

  rx_pkt = ROM_WL_rsi_get_rx_pkt(global_cb_p);
  if(rx_pkt != NULL)
  {
    //! Enqueue the packet
    ROM_WL_rsi_enqueue_pkt(global_cb_p, &rsi_driver_cb->m4_rx_q,rx_pkt);
  }
  else
  {

    RSI_ASSERTION(SAPIS_M4_RX_BUFF_NULL_RECIEVED, "\n receive_from_ta_done_isr, Received NULL Packet \n");
  }

  ROM_WL_rsi_submit_rx_pkt(global_cb_p);

  //! Set event RX pending from device
  ROM_WL_rsi_set_event(global_cb_p, RSI_RX_EVENT);
 
  return;
  
}

int ROM_WL_rsi_submit_rx_pkt(global_cb_t *global_cb_p)
{

  rsi_pkt_t *rx_pkt = NULL;

  rsi_driver_cb_t *rsi_driver_cb = global_cb_p->rsi_driver_cb;
  rsi_m4ta_desc_t *rx_desc       = global_cb_p->rx_desc;  

  int8_t *pkt_buffer = NULL;
  //!Get commmon cb pointer

  if(M4SS_P2P_INTR_SET_REG & RX_BUFFER_VALID)
  {
      RSI_ASSERTION(SAPIS_M4_RX_BUFF_ALREDY_VALID, "\nIn submit rx pkt , RX buffer is already valid\n");

	  return -2;

  }

  //! Allocate packet to receive packet from module
  rx_pkt = ROM_WL_rsi_pkt_alloc(global_cb_p, &rsi_driver_cb->rx_pool);


  if(rx_pkt == NULL)
  {
    RSI_ASSERTION(SAPIS_M4_DEBUG_OUT, "\nIn submit rx pkt , RX buffer is not available\n");

    global_cb_p->submit_rx_pkt_to_ta = 1;

    return -1;
  }

  pkt_buffer = (int8_t *)&rx_pkt->desc[0];
  
  //! Fill source address in the TX descriptors
  rx_desc[0].addr = (M4_MEMORY_OFFSET_ADDRESS + (uint32_t)pkt_buffer);

  //! Fill source address in the TX descriptors
  rx_desc[0].length = (16);

  //! Fill source address in the TX descriptors
  rx_desc[1].addr = (M4_MEMORY_OFFSET_ADDRESS + (uint32_t)(pkt_buffer+16));

  //! Fill source address in the TX descriptors
  rx_desc[1].length = (1600);


  ROM_WL_raise_m4_to_ta_interrupt(RX_BUFFER_VALID);

  return 0;
}




/*====================================================*/
/**
 * @fn          rsi_pkt_t* ROM_WL_rsi_frame_read(global_cb_t *global_cb_p)
 *
 * @brief This  function is used to read the response from module.
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[out]  none
 * @return      Packet which is read from the  module
 * @section description 
 * This is a common function to read response for all the command and data from Wi-Fi module.
 */


rsi_pkt_t* ROM_WL_rsi_frame_read(global_cb_t *global_cb_p)
{
	
	return ROM_WL_rsi_dequeue_pkt(global_cb_p, &global_cb_p->rsi_driver_cb->m4_rx_q);
	
}


/*====================================================*/
/**
 * @fn          int16_t ROM_WL_rsi_frame_write(global_cb_t *global_cb_p, rsi_frame_desc_t *uFrameDscFrame,uint8_t *payloadparam,uint16_t size_param)
 * @brief       Common Frame write API
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   uFrameDsc uFrameDscFrame, frame descriptor
 * @param[in]   uint8_t *payloadparam, pointer to the command payload parameter structure
 * @param[in]   uint16_t size_param, size of the payload for the command
 * @return      errCode or success
 * @section description 
 * This is a common function used to process a command to the Wi-Fi module.
 */



int16_t ROM_WL_rsi_frame_write(global_cb_t *global_cb_p, rsi_frame_desc_t *uFrameDscFrame,uint8_t *payloadparam,uint16_t size_param)
{
  rsi_m4ta_desc_t   *tx_desc = global_cb_p->tx_desc;  
  
  if(((uFrameDscFrame->frame_len_queue_no[1]) >> 4) == 0x0)
  {
    RSI_ASSERTION(SAPIS_M4_TX_INVALID_DESC, "\nIn frame write , Invalid TX frame descriptor\n");

     return -1; 
   }

	//! Fill source address in the TX descriptors
	tx_desc[0].addr = (M4_MEMORY_OFFSET_ADDRESS + (uint32_t)uFrameDscFrame);
	
	//! Fill source address in the TX descriptors
	tx_desc[0].length = (16);

	//! Fill source address in the TX descriptors
	tx_desc[1].addr = (M4_MEMORY_OFFSET_ADDRESS + (uint32_t)payloadparam);

		//! Fill source address in the TX descriptors
	tx_desc[1].length = (size_param);


	ROM_WL_rsi_send_pkt_to_ta(&tx_desc[0]);

	return 0;
}




void ROM_WL_raise_m4_to_ta_interrupt(uint32_t interrupt_no)
{
  M4SS_P2P_INTR_SET_REG = interrupt_no;   
}

#if 0
void ROM_WL_clear_m4_to_ta_interrupt(uint32_t interrupt_no)
{
  M4SS_P2P_INTR_CLR_REG = interrupt_no;   
}

void ROM_WL_clear_ta_interrupt_mask()
{
  TASS_P2P_INTR_MASK_CLR  = ~0;
}

void ROM_WL_set_ta_interrupt_mask()
{
  TASS_P2P_INTR_MASK_SET  = ~0;
}
#endif

void ROM_WL_mask_ta_interrupt(uint32_t interrupt_no)
{
  TASS_P2P_INTR_MASK_SET  = interrupt_no;
}

void ROM_WL_unmask_ta_interrupt(uint32_t interrupt_no)
{
  TASS_P2P_INTR_MASK_CLR  = interrupt_no;
}

void ROM_WL_clear_ta_to_m4_interrupt(uint32_t interrupt_no)
{
  TASS_P2P_INTR_CLEAR = interrupt_no;  
  TASS_P2P_INTR_CLR_REG = interrupt_no;
}
#endif
#endif




