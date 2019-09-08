/**
 * @file    rsi_wlan_non_rom.h
 * @version 0.1
 * @date    15 July,2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contain declarations for non rom structures for wlan
 *
 *  @section Description  This file contains declarations for non rom structures for wlan
 *  implementation.
 *
 *
 */

#ifndef RSI_WLAN_NON_ROM_H
#define RSI_WLAN_NON_ROM_H


//! driver WLAN control block
typedef struct rsi_wlan_cb_non_rom_s 
{
	uint32_t tls_version;
  uint8_t join_bssid_non_rom[6];
}rsi_wlan_cb_non_rom_t;

#endif

typedef struct rsi_socket_info_non_rom_s
{
  uint8_t max_tcp_retries;

  uint8_t vap_id;

  //! TCP keepalive initial timeout
  uint8_t     tcp_keepalive_initial_time[2];

}rsi_socket_info_non_rom_t;


