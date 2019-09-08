/**
 * @file     rsi_hal.h
 * @version  0.1
 * @date     15 Aug 2015
 *
 * Copyright(C) Redpine Signals 2015
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief HAL HEADER: Function prototypes for HAL
 *
 * @section Description
 * This file contains the function definition prototypes, defines used in HAL and related functions
 *
 *
 */

#ifndef _RSI_M4_HAL_H_
#define _RSI_M4_HAL_H_
#ifdef RSI_M4_INTERFACE

#include "rsi_pkt_mgmt.h"
#include "RS1xxxx.h"
#include "core_cm4.h"
/******************************************************
 * *                    Constants
 * ******************************************************/

#define M4_ISR_IRQ  74

 #define  TA_MEMORY_OFFSET_ADDRESS      0x00400000
#define   M4_MEMORY_OFFSET_ADDRESS      0x00200000



 
 #define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address  */
 
 #define NVIC_BASE           (SCS_BASE +  0x0100UL)                    /*!< NVIC Base Address                  */
 #define   NVIC                ((NVIC_Type      *)     NVIC_BASE     )   /*!< NVIC configuration struct          */

 
 #define  M4SS_P2P_INT_BASE_ADDRESS       0x46008000
 #define 	MCU_PWR_CTRL_BASE_ADDR				  0x24048400

 #define  MISC_CFG_HOST_CTRL             *(volatile uint32_t *)(M4SS_P2P_INT_BASE_ADDRESS + 0x0C)

 #define  M4SS_P2P_INTR_SET_REG          *(volatile uint32_t *)(M4SS_P2P_INT_BASE_ADDRESS + 0x16C)
 #define  M4SS_P2P_INTR_CLR_REG          *(volatile uint32_t *)(M4SS_P2P_INT_BASE_ADDRESS + 0x170)
 #define  P2P_STATUS_REG                 *(volatile uint32_t *)(M4SS_P2P_INT_BASE_ADDRESS + 0x174)
 #define  TASS_P2P_INTR_MASK_SET         *(volatile uint32_t *)(M4SS_P2P_INT_BASE_ADDRESS + 0x178)
 #define  TASS_P2P_INTR_MASK_CLR         *(volatile uint32_t *)(M4SS_P2P_INT_BASE_ADDRESS + 0x17C)  
 #define  TASS_P2P_INTR_CLEAR            *(volatile uint32_t *)(M4SS_P2P_INT_BASE_ADDRESS + 0x180)
 
 
 #define TASS_P2P_INT_BASE_ADDRESS       0x41050000
 
 #define TASS_P2P_INTR_SET_REG           *(volatile uint32_t *)(TASS_P2P_INT_BASE_ADDRESS + 0x8C)
 #define TASS_P2P_INTR_CLR_REG           *(volatile uint32_t *)(TASS_P2P_INT_BASE_ADDRESS + 0x90)
 
 #define M4_TX_DMA_DESC_REG              *(volatile uint32_t *)(TASS_P2P_INT_BASE_ADDRESS + 0x34)
 #define M4_RX_DMA_DESC_REG              *(volatile uint32_t *)(TASS_P2P_INT_BASE_ADDRESS + 0x5C)	 
 #define HOST_INTR_STATUS_REG            *(volatile uint32_t *)(TASS_P2P_INT_BASE_ADDRESS + 0x04)		

#define DMA_DESC_REG_VALID               (0xA0 << 8)

#define TA_wakeup_M4                     BIT(2)
#define TA_is_active                     BIT(3)
#define M4_wakeup_TA                     BIT(0)
#define M4_is_active                     BIT(1)
 


 #define ARM_MASK_1     0xE000E100
 #define ARM_MASK_1     0xE000E100
 #define ARM_MASK_1     0xE000E100
 
 #ifndef BIT
 #define  BIT(x)    (1<<(x))
 #endif
 
 //! This interrupt is raised by M4 to TA when there is a TX packet from M4 to read
 #define   RX_BUFFER_VALID                     BIT(1)
 #define   TX_PKT_PENDING_INTERRUPT            BIT(2)
 
 #define  TX_PKT_TRANSFER_DONE_INTERRUPT     BIT(2)
 //! This interrupt is received from TA when RX packet is pending from TA
 #define  RX_PKT_TRANSFER_DONE_INTERRUPT     BIT(1)
 //! This interrupt is received from TA when TX packet transfer from M4 to TA is done 

//! buffer full indication register value from module
 #define RSI_BUFFER_FULL     BIT(0)

/******************************************************
 * *                   Enumerations
 * ******************************************************/

/******************************************************
 * *                 Type Definitions
 * ******************************************************/
typedef struct rsi_m4ta_desc_dword1_s
{
  //! Reserved 
  uint32_t reserved  : 15;

  //! 1 bit : indicates the presence of more descriptors
  //! 1 -  last descriptor
  //! 0 -  more descriptors are present
  uint32_t last_desc : 1;

	//! Buffer queue_no to be transfered for this descriptor 
  uint32_t queue_no    : 4;

  //! Buffer length to be transfered for this descriptor 
  uint32_t length    : 12;

}rsi_m4ta_desc_dword1_t;




typedef struct rsi_m4ta_desc_s
{
  //! source address
  uint32_t addr;

  uint16_t length;
  //! descriptor control fields
  //rsi_m4ta_desc_dword1_t dword1;

}rsi_m4ta_desc_t;


//! host descriptor structure
typedef struct rsi_frame_desc_s {
  //! Data frame body length. Bits 14:12=queue, 000 for data, Bits 11:0 are the length
  uint8_t   frame_len_queue_no[2];            
  //! Frame type
  uint8_t   frame_type;                
  //! Unused , set to 0x00
  uint8_t   reserved[9];                       
  //! Management frame descriptor response status, 0x00=success, else error
  uint8_t   status;
  uint8_t   reserved1[3];
} rsi_frame_desc_t;



//! P2P registers Backup structure
typedef struct rsi_p2p_intr_status_bkp_s
{
  uint32_t   tass_p2p_intr_mask_clr_bkp;
  uint32_t   m4ss_p2p_intr_set_reg_bkp;	
}rsi_p2p_intr_status_bkp_t;	


/******************************************************
 * *                    Structures
 * ******************************************************/

/******************************************************
 * *                 Global Variables
 * ******************************************************/
/******************************************************
 * *               Function Declarations
 * ******************************************************/
int16_t rsi_frame_write(rsi_frame_desc_t *uFrameDscFrame,uint8_t *payloadparam,uint16_t size_param);
rsi_pkt_t* rsi_frame_read();
int16_t rsi_device_interrupt_status(uint8_t *int_status);

void rsi_m4_interrupt_isr();
void rsi_m4_ta_interrupt_init();
void rsi_raise_pkt_pending_interrupt_to_ta();
int32_t rsi_send_pkt_to_ta(rsi_m4ta_desc_t *tx_desc);
void rsi_transfer_to_ta_done_isr();
void rsi_pkt_pending_from_ta_isr();
void rsi_receive_from_ta_done_isr();
int16_t rsi_device_buffer_full_status();
int rsi_submit_rx_pkt();
void unmask_ta_interrupt(uint32_t interrupt_no);
void raise_m4_to_ta_interrupt(uint32_t interrupt_no);
void clear_m4_to_ta_interrupt(uint32_t interrupt_no);
void clear_ta_interrupt_mask();
void set_ta_interrupt_mask();
void mask_ta_interrupt(uint32_t interrupt_no);
void unmask_ta_interrupt(uint32_t interrupt_no);
void clear_ta_to_m4_interrupt(uint32_t interrupt_no);
void ROM_WL_raise_m4_to_ta_interrupt(uint32_t interrupt_no);
void ROM_WL_clear_m4_to_ta_interrupt(uint32_t interrupt_no);
void ROM_WL_clear_ta_interrupt_mask();
void ROM_WL_set_ta_interrupt_mask();
void ROM_WL_mask_ta_interrupt(uint32_t interrupt_no);
void ROM_WL_unmask_ta_interrupt(uint32_t interrupt_no);
void ROM_WL_clear_ta_to_m4_interrupt(uint32_t interrupt_no);


#endif
#endif

