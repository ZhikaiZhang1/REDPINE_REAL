/**
 * @file    rsi_a2dp_i2s.h
 * @version 0.1
 * @date    xx jan 2018
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for I2S configuration for audio using A2DP
 *
 *  @section Description  This application serves as a I2S configuration.
 *
 */

/**
 * Include files
 * */
 

#include <rsi_bt_config.h>
#ifdef RSI_M4_INTERFACE
#include <rsi_board.h>
#include "SAI.h"
#endif

#define  MAX_DATA_PKTS  0x6
typedef  struct  data_pkt_s {
   uint8_t   used;
   uint8_t   reserved;
   uint16_t  data_len;
   uint8_t   data[512];
   struct data_pkt_s  *next;
} data_pkt_t;

#ifdef RSI_M4_INTERFACE
void rsi_i2s_slave_init ();
#endif

data_pkt_t  *allocate_data_pkt (data_pkt_t *p_data_pool);
void free_data_pkt (data_pkt_t  *p_data_pool, data_pkt_t *p_data_pkt);
void add_packet_to_queue (data_pkt_t **queue, data_pkt_t  *data_pkt);
data_pkt_t  *get_packet_from_queue (data_pkt_t **queue);
