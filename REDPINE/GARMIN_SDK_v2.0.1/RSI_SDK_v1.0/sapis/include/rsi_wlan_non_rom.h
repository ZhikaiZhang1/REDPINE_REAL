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
//! Max chunk size 
#define CHUNK_SIZE 1472
#define MIN(x,y) ((x)<(y)?(x):(y))

//! driver WLAN control block
typedef struct rsi_wlan_cb_non_rom_s 
{
	uint32_t tls_version;
  uint8_t join_bssid_non_rom[6];
  //! to identify on which socket data is present
	uint32_t socket_bitmap;
  uint32_t *fd_set_ptr; 
  void (* sock_select_callback)(uint32_t fd_read);
}rsi_wlan_cb_non_rom_t;

typedef struct rsi_socket_info_non_rom_s
{
  uint8_t max_tcp_retries;

  uint8_t vap_id;

  //! TCP keepalive initial timeout
  uint8_t     tcp_keepalive_initial_time[2];
	
	//! Asynchronous accpet respone handler
	void   (* accept_call_back_handler)(int32_t sock_id, int16_t dest_port, uint8_t *ip_addr, int16_t ip_version);
	//! Asynchronous data tx done call back handler
	void (*rsi_sock_data_tx_done_cb)(uint8_t sockID,int16_t status,uint16_t total_data_sent);

  //! total data to be send
	int8_t	*buffer;
  //! total data sent
	int32_t offset;
  //! remaining data to be send
	int32_t rem_len;
  //! flag which indicates more data is present 
	uint32_t more_data;
  //! user flags 
	int32_t flags;
}rsi_socket_info_non_rom_t;
#ifndef RSI_M4_INTERFACE
int32_t rsi_socket_create_async(global_cb_t *global_cb_p, int32_t sockID, int32_t type, int32_t backlog);
int32_t rsi_get_socket_descriptor(uint8_t *src_port, uint8_t *dst_port, uint8_t *ip_addr, uint16_t ip_version);
#endif
int rsi_fd_isset(uint32_t fd, struct rsi_fd_set_s *fds_p);
void rsi_set_fd(uint32_t fd, struct rsi_fd_set_s *fds_p);
void rsi_fd_clr(uint32_t fd, struct rsi_fd_set_s *fds_p);
#endif
