/**
* @file     rsi_ant.h
* @version  1.0
* @date     03 Sep 2015
*
*  Copyright(C) Redpine Signals 2015
*  All rights reserved by Redpine Signals.
*
*  @section License
*  This program should be used on your own responsibility.
*  Redpine Signals assumes no responsibility for any losses
*  incurred by customers or third parties arising from the use of this file.
*
*  @brief : This file contains redpine related structures information
*
*  @section Description  This file contains redpine related structures,macros and functions
*
*/


#ifndef RSI_ANT_H
#define RSI_ANT_H

#include "data_types.h"
#include "defines.h"

struct bt_rx_pkt_s;
struct bt_tx_pkt_s;
struct bt_common_info_s;
struct remote_le_dev_info_s;
struct soft_timer_task_s;
struct common_info_s;

#define ANT_PKT_PREAMBLE_TIME_IN_USEC  8

#define ANT_PKT_RADIO_POST_PROCESSING  500

#define ANT_PKT_TYPE_1_SYNC_WORD       2
#define ANT_PKT_TYPE_2_SYNC_WORD       4



#define ANT_PACKET_TYPE_2_SCRAMBLER  0x01
#define ANT_PACKET_TYPE_1_SCRAMBLER  0x00

#define ANT_PACKET_TYPE_1  1
#define ANT_PACKET_TYPE_2  2
#define RF_CHAN_FREQ_OFFSET 2400

#define ANT_CRC_WIDTH_RESET               0x1F
#define ANT_NW_ADDR_PKT_TYPE_2_LEN        0x4
#define ANT_NW_ADDR_PKT_TYPE_1_LEN        0x2

/* ANT PolyNonimals */
#define ANT_PACKET_TYPE1_CRC_POLYNOMINAL  0x1021
#define ANT_PACKET_TYPE2_CRC_POLYNOMINAL  0x17B01BD

#define ANT_PACKET_TYPE1_CRC_WIDTH        (0x10 - 1)
#define ANT_PACKET_TYPE2_CRC_WIDTH        (0x18 - 1)

#define ANT_PACKET_TYPE1_CRC_INIT         0xFFFF
#define ANT_PACKET_TYPE2_CRC_INIT         0xFFFFFF


#define CLEAR_LFSR                        BIT(0)
#define INIT_LFSR                         BIT(1)
#define SWAP_INIT_LFSR_VAL                BIT(2)

#define DIN_WIDTH_FROM_REG                BIT(5)
#define DIN_WIDTH_FROM_CNT                BIT(6)


#define AEMPTY_THRESHOLD                  (6 << 24)
#define AFULL_THRESHOLD                   (2 << 28)
#define SWAP_DATA_FILLED                  (1 << 7)

#define ANT_PKT_TYPE2_CRC_LEN             3
#define ANT_PKT_TYPE1_CRC_LEN             2

#define CRC_CALC_DONE                     BIT(0)
#define CRC_DIN_NUM_BYTES_DONE            BIT(1)


#define CRC_BASE                                0x20400800


/* Register Sets */
#define CRC_GEN_CTRL_RESET_REG                  (CRC_BASE + 0x04)
#define CRC_GEN_STS_REG                         (CRC_BASE + 0x08)
#define CRC_POLYNOMINAL                         (CRC_BASE + 0x0C) 
#define CRC_POLYNOMINAL_CTRL_SET                (CRC_BASE + 0x10) 
#define CRC_POLYNOMINAL_CTRL_RESET              (CRC_BASE + 0x14)
#define CRC_LFSR_INIT_VAL                       (CRC_BASE + 0x18)
#define CRC_LFSR_INIT_CTRL_SET                  (CRC_BASE + 0x1C)
#define CRC_LFSR_INIT_CTRL_RESET                (CRC_BASE + 0x20)
#define CRC_DIN_FIFO_REG                        (CRC_BASE + 0x24)
#define CRC_DIN_CTRL_SET                        (CRC_BASE + 0x28)
#define CRC_DIN_CTRL_RESET_REG                  (CRC_BASE + 0x2C)
#define CRC_DIN_NUM_BYTES                       (CRC_BASE + 0x30)
#define CRC_DIN_STS_REG                         (CRC_BASE + 0x34)
#define CRC_LFSR_STATE_REG                      (CRC_BASE + 0x38)
#define ANT_ENCRYPT_BLOCK_LEN      16


#define ANT_PKT_TYPE_1_SYNC_WORD_TIME           16
#define ANT_PKT_TYPE_1_PREAMBLE_TIME            8

#define ANT_PKT_TYPE_1_RX_OFFSET                40


typedef struct ble_ant_info_s {
  uint8 *bt_tx_pkt;
  struct bt_rx_pkt_s *bt_rx_pkt;

  /* These structures are used for the Internal Test mode
   * verification
   */
  antsys_atm_config_t rsi_ant_atm_config;

#define NO_MODE   0
#define E2E_MODE  1
#define TEST_MODE 2
  uint8 curr_ant_mode;
  uint16 tx_power;
  uint8 curr_pkt_type;
  uint8 curr_data_len;

  /* These structures are used for the ant stack Integration
   * verification
   */
  antsys_atm_config_t *ant_atm_config;
  antsys_radio_config_t *ant_radio_config;
  antsys_radio_result_t radio_result;

  uint8 per_mode_enable;
  int8 rssi;
  uint8 enc_key[ANT_ENCRYPT_BLOCK_LEN];
  uint8 enc_ctr[ANT_ENCRYPT_BLOCK_LEN];
  uint16 enc_offset;
  antsys_core_callbacks_t *antsys_core_callbacks;
  antsys_sched_params_t *ant_sched_params;
  antsys_coex_event_t *ant_coex_event;
  antsys_activity_info_t info;
  antsys_blocked_info_t blocked_info;
  uint8 sleep_enable;
#define  ANT_RADIO_COMPLETE     2
#define  ANT_RUN_STACK_TASK     1
#define  ANT_ACTIVITY_STARTED   3
#define  ANT_ACTIVITY_ABORTED   4  
  uint32 ant_intr_order;
}ble_ant_info_t;

#define ANT_SOFT_TIMER_START(A, B, C, D)     program_soft_timer(A, B, (C | SOFT_TIMER_TSF_MODE), D) 
#define ANT_SOFT_TIMER_STOP(A, B)            delete_node_from_soft_timer(A, B)
#define ANT_SOFT_TIMER_READ(A, B)            get_remaining_time(A, B)
#define ANT_SOFT_TIMER_EXPIRY_HANDLER(A, B)   soft_timer_expiry_task_handler(A, B)
#define ANT_SOFT_TIMER_RESTART(A, B, C, D)   reprogram_soft_timer(A, B, (C | SOFT_TIMER_TSF_MODE), D)

#define ANT_DEBUG_THRESHOLD                  10000


void ant_debug_expiry_handler();
uint8 ant_config_pkt_whiten(uint8 *tx_pkt, uint8 pkt_len_in_bytes);
void ant_convert_lsb_to_msb(uint8 *data, uint8 data_len);
uint16 ant_calc_pkt_tout(antsys_mode_flags_t mode_flags, uint32 data_len);
antsys_result_t rsi_config_ant_radio(const antsys_radio_config_t *config);
antsys_result_t rsi_abort_ant_radio_op(void);
antsys_time_t rsi_get_sys_time(void);
antsys_result_t rsi_arm_timer(antsys_timer_id_t timer, antsys_time_t trigger_time);
antsys_result_t rsi_cancel_timer(antsys_timer_id_t timer);
void rsi_ant_timer_expiry_handler(struct common_info_s *common_info, uint32 node_id);
void ant_tmr_intr_handler(struct bt_common_info_s *bt_common_info);
void ant_tx_done_handler(struct bt_common_info_s *bt_common_info);
uint8 ant_config_pkt_txn(struct bt_common_info_s *bt_common_info,
    uint8 *tx_pkt,
    uint16 tx_power,
    uint16 pkt_len_in_bytes,
    uint8 tx_power_index);
void ant_compute_crc(uint8 *data, uint16 data_len, uint16 mode_flags, uint32 sync_word);
antsys_result_t ant_form_tx_pkt(struct bt_common_info_s *bt_common_info, 
                     uint32 sync_word,
                     uint16 mode_flags, 
                     uint8 *data,
                     uint16 data_len, 
                     uint8 use_random);
antsys_result_t rsi_start_atm(antsys_atm_config_t *config);
antsys_result_t stop_atm(void);
void rcv_ant_pkt(struct bt_common_info_s *bt_common_info, struct bt_tx_pkt_s *tx_pkt);
void rcv_atm_data(struct bt_common_info_s *bt_common_info, struct bt_tx_pkt_s *tx_pkt);
void rcv_encryption_data(struct bt_common_info_s *bt_common_info, struct bt_tx_pkt_s *tx_pkt);
antsys_result_t start_atm(antsys_atm_config_t *config);
void ant_rx_pkt_hdr_rcvd_intr_handler(struct bt_common_info_s *bt_common_info);
void per_proc_ant_rx_pkt(struct bt_common_info_s *bt_common_info);
void ant_rx_dma_transfer_done_isr(uint32 arg);
void ant_encryption(uint8 *key, uint8 *ctr_iv, uint8 *tx_pkt, uint8 data_len, uint16 enc_offset);

antsys_time_t rsi_get_sys_time(void);
antsys_result_t rsi_arm_timer(antsys_timer_id_t timer, antsys_time_t trigger_time);
void process_ant_tx_pkt();
antsys_result_t rsi_init_interface();
antsys_result_t rsi_cleanup_interface();
antsys_result_t disable_interrupts(antsys_context_id_t context);
antsys_result_t enable_interrupts(antsys_context_id_t context);
void rsi_pend_stack_task();
antsys_result_t rsi_send_ant_rx_pkt_to_host(uint8 *data, uint16 data_len);
void ant_wait_for_rx_completed(struct bt_common_info_s *bt_common_info);
antsys_result_t rsi_request_activity(const antsys_sched_params_t *params,
                                        antsys_blocked_info_t *blocked_info);
antsys_result_t rsi_complete_activity(void);
antsys_result_t rsi_on_coex_event(const antsys_coex_event_t *event);
antsys_result_t rsi_get_rand(uint8 *data,uint32 len);

#endif
