/**
 * @file    rsi_bt_a2dp_source_avrcp_DEMO_6.c
 * @version 0.1
 * @date    13 Oct 2018
 *
 *  Copyright(C) Redpine Signals 2018
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BT Classic A2DP Source and AVRCP target
 *
 *  @section Description  This application serves as BT Classic A2DP Source and AVRCP target
 */

/*=======================================================================*/
//   ! INCLUDES
/*=======================================================================*/

#include <rsi_common_app.h>
#if ANT_BLE_CENT_BT_A2DP_SRC_APP
//! BT include file to refer BT APIs
#include <rsi_driver.h>
#include <rsi_bt_common_apis.h>
#include <rsi_bt_apis.h>
#include <rsi_bt.h>
#include <stdio.h>
#include "fsl_lptmr.h"
#ifdef RSI_M4_INTERFACE
#include <rsi_board.h>
#endif
#if (RSI_AUDIO_DATA_SRC == ARRAY)
#if (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
#include "pcm_data_buff.h"
#endif
#elif (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
//! SD card include files
#include "ff.h"
#include "diskio.h"
#include "fsl_debug_console.h"
#endif
#ifdef RSI_WITH_OS
extern rsi_semaphore_handle_t coex_sem;
//rsi_mutex_handle_t bt_event_mutex;
#endif

/*=======================================================================*/
//   ! MACROS
/*=======================================================================*/

#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
#define SONG_LIST_MAX_COUNT                3
#define AUDIO_FILE_PATH                    "/sbc_1.sbc"
#define AUDIO_FILE_PATH_2                  "/sbc_2.sbc"
#define AUDIO_FILE_PATH_3                  "/sbc_3.sbc"
#define ELEMENT_READ_SIZE              	   1
#define RSI_SBC_DATA_SIZE                  1100
/* sampling frequency */
#define SBC_FREQ_16000                     0x00
#define SBC_FREQ_32000                     0x01
#define SBC_FREQ_44100                     0x02
#define SBC_FREQ_48000                     0x03

/* blocks */
#define SBC_BLK_4                          0x00
#define SBC_BLK_8                          0x01
#define SBC_BLK_12                         0x02
#define SBC_BLK_16                         0x03

/* channel mode */
#define SBC_MODE_MONO                      0x00
#define SBC_MODE_DUAL_CHANNEL              0x01
#define SBC_MODE_STEREO                    0x02
#define SBC_MODE_JOINT_STEREO              0x03

/* allocation method */
#define SBC_AM_LOUDNESS                    0x00
#define SBC_AM_SNR                         0x01

/* subbands */
#define SBC_SB_4                           0x00
#define SBC_SB_8                           0x01
//! SBC_FRAME Mapping Headers
#define PKT_HEADER_AND_CRC                 11 // 11 --> l2cap+controller header + crc
#define RTP_HEADER                         13 // 13 --> rtp_header
#define SBC_HEADER_SIZE                    4
#define SBC_FRAME_SIZE                     sbc_frame_size
#define SKIP_SIZE                          (SBC_FRAME_SIZE + RTP_HEADER + PKT_HEADER_AND_CRC)
#define SBC_SYNCWORD                       0x9C

enum {
	MONO = SBC_MODE_MONO,
	DUAL_CHANNEL = SBC_MODE_DUAL_CHANNEL,
	STEREO = SBC_MODE_STEREO,
	JOINT_STEREO = SBC_MODE_JOINT_STEREO
} mode;

#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
#define AUDIO_FILE_PATH                  	"pcm_samples.wav"
#define RSI_PCM_DATA_SIZE              		512

#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
#define AUDIO_FILE_PATH                  	"mp3_samples.mp3"
#define RSI_MP3_DATA_SIZE               	512
#define RMOVE_ID3_TAG
#endif
#if (RSI_AUDIO_DATA_SRC == BIN_FILE)
#define ELEMENT_READ_SIZE                   1
#endif
#define DEFAULT_MTU_SIZE                    310
/*=======================================================================*/
//   ! GLOBAL VARIABLES
/*=======================================================================*/

uint8_t           play_count;
uint8_t           play_pause_count;
uint16_t          sample_freq;
uint16_t          rem_mtu_size = DEFAULT_MTU_SIZE;
uint8_t           app_song_list[SONG_LIST_MAX_COUNT][128] = { AUDIO_FILE_PATH, AUDIO_FILE_PATH_2, AUDIO_FILE_PATH_3 };
uint8_t           app_song_list_idx;
uint8_t           power_save_given = 0;
volatile uint32_t aud_pkts_sent = 0;

static uint32_t   rsi_app_async_event_map = 0;
static uint8_t   str_conn_bd_addr[BD_ADDR_ARRAY_LEN];
static uint8_t   local_dev_addr[RSI_DEV_ADDR_LEN] = { 0 };
static uint8_t   remote_dev_addr[RSI_DEV_ADDR_LEN] = { 0 };
static uint8_t   linkkey[RSI_LINKKEY_REPLY_SIZE];
static uint8_t   dev_mode;

//! Structure variables
app_state_t                         app_state = 0;
static rsi_bt_resp_get_local_name_t local_name = { 0 };

#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
uint16_t         pkt_size;
uint8_t          sbc_frames;
uint32_t         sbc_frame_size = 0;
uint8_t          sbc_data[RSI_SBC_DATA_SIZE];

#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
uint16_t pkt_size;
uint8_t pcm_data[RSI_PCM_DATA_SIZE];

#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
uint16_t pkt_size;
uint8_t mp3_data[RSI_MP3_DATA_SIZE];

//! TAG is always present at the beggining of a ID3V2 MP3 file
//! Constant size 10 bytes
typedef struct
{
	uint8_t id[3];       //"ID3"
	uint8_t version[2];// $04 00
	uint8_t flags;// %abcd0000
	uint32_t size;//4 * %0xxxxxxx
}__attribute__((__packed__))
ID3TAG;
#endif

#if (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
/* File object */
FIL             g_fileObject_sd;
uint8_t         file_idx_sd = 0;
/* Read buffer */
static uint8_t  g_bufferRead[512];
#endif

#if(RSI_BT_DUAL_PAIR_TEST == 1)
uint8_t dual_pair_flag;
#endif

/*=======================================================================*/
//   ! EXTERN VARIABLES
/*=======================================================================*/

/*=======================================================================*/
//   ! EXTERN FUNCTIONS
/*=======================================================================*/

#if RPS_LINK_KEY_SAVE
extern bool rsi_link_key_remove( dev_address_type addr1 );
extern bool rsi_link_key_add( rsi_bt_event_user_linkkey_save_t *   link_key_update );
extern bool rsi_link_key_read( rsi_bt_event_user_linkkey_save_t *   peer_link_key );
extern void rsi_init_link_key_list( void );
#endif
extern int32_t rsi_bt_a2dp_send_pcm_mp3_data(uint8_t *remote_dev_addr,
		uint8_t *pcm_mp3_data, uint16_t pcm_mp3_data_len, uint8_t audio_type);
extern int32_t rsi_bt_set_eir_data(uint8_t *data, uint16_t data_len);
extern int32_t rsi_bt_a2dp_init(void);
extern int32_t rsi_bt_avrcp_init(void);
extern int32_t rsi_bt_a2dp_connect(uint8_t *remote_dev_addr);
extern int32_t rsi_bt_avrcp_conn(uint8_t *remote_dev_addr);
extern int32_t rsi_bt_avrcp_notify(uint8_t *remote_dev_addr, uint8_t event_id,
		notify_val_t *p_notify_val);
extern int32_t rsi_bt_avrcp_cap_resp(uint8_t *remote_dev_addr, uint8_t cap_type,
		uint8_t nbr_caps, uint32_t *p_caps);
extern int32_t rsi_bt_avrcp_att_list_resp(uint8_t *remote_dev_addr,
		uint8_t nbr_atts, uint8_t *p_atts);
extern int32_t rsi_bt_avrcp_att_val_list_resp(uint8_t *remote_dev_addr,
		uint8_t nbr_vals, uint8_t *p_vals);
extern int32_t rsi_bt_avrcp_set_cur_att_val_resp(uint8_t *remote_dev_addr,
		uint8_t status);
extern int32_t rsi_bt_avrcp_char_set_status_resp(uint8_t *remote_dev_addr,
		uint8_t status);
extern int32_t rsi_bt_avrcp_batt_status_resp(uint8_t *remote_dev_addr,
		uint8_t status);
extern int32_t rsi_bt_avrcp_ele_att_resp(uint8_t *remote_dev_addr,
		uint8_t nbr_atts, att_text_t *p_att_list);
extern int32_t rsi_bt_avrcp_play_status_resp(uint8_t *remote_dev_addr,
		uint8_t play_status, uint32_t song_len, uint32_t song_pos);
extern int32_t rsi_bt_avrcp_reg_notify_resp(uint8_t *remote_dev_addr,
		uint8_t status);
extern uint8_t* rsi_itoa(uint32_t val, uint8_t *str);
extern int32_t rsi_bt_power_save_profile(uint8_t psp_mode, uint8_t psp_type);
extern int32_t rsi_bt_a2dp_send_sbc_aac_data(uint8_t *remote_dev_addr,
		uint8_t *sbc_aac_data, uint16_t sbc_aac_data_len, uint8_t audio_type);
extern int32_t rsi_bt_a2dp_start(int8_t *remote_dev_addr);
extern int32_t rsi_bt_a2dp_suspend(int8_t *remote_dev_addr);

/*========================================================================*/
//!  CALLBACK FUNCTIONS
/*=======================================================================*/

/*=======================================================================*/
//   ! PROCEDURES
/*=======================================================================*/

/*==============================================*/
/**
 * @fn         rsi_bt_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BT initialization.
 */
static void rsi_bt_app_init_events() {
	rsi_app_async_event_map = 0;
	return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_set_event
 * @brief      sets the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
static void rsi_bt_app_set_event(uint32_t event_num) {
#ifdef RSI_WITH_OS
	  //rsi_mutex_lock(&bt_event_mutex);
#endif
	rsi_app_async_event_map |= BIT(event_num);
#ifdef RSI_WITH_OS
	//rsi_mutex_unlock(&bt_event_mutex);
#endif
	return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_bt_app_clear_event(uint32_t event_num) {
	rsi_app_async_event_map &= ~BIT(event_num);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
static int32_t rsi_bt_app_get_event(void) {
	uint32_t ix;

	for (ix = 0; ix < 32; ix++) {
		if (rsi_app_async_event_map & (1 << ix)) {
			return ix;
		}
	}

	return (RSI_FAILURE);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_conn
 * @brief      invoked when connection complete event is received
 * @param[out] resp_status, connection status of the connected device.
 * @param[out] conn_event, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */

void rsi_bt_app_on_conn(uint16_t resp_status, rsi_bt_event_bond_t *conn_event) {
	if (resp_status == RSI_APP_ERR_NONE) {
		app_state |= (1 << CONNECTED);
		rsi_bt_app_set_event(RSI_APP_EVENT_CONNECTED);
	} else if (resp_status == RSI_APP_ERR_ACL_CONN_ALREADY_EXISTS) {
		app_state |= (1 << CONNECTED);
		app_state |= (1 << AUTHENTICATED);
		rsi_bt_app_set_event(RSI_APP_EVENT_CONNECTED);
	} else {
		rsi_bt_app_set_event(RSI_APP_EVENT_DISCONNECTED);
	}

  memcpy ((int8_t *)remote_dev_addr, conn_event->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on conn: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, conn_event->dev_addr),
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_pincode_req
 * @brief      invoked when pincode request event is received
 * @param[out] user_pincode_request, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the pincode request from remote device
 */
void rsi_bt_app_on_pincode_req(uint16_t resp_status,
		rsi_bt_event_user_pincode_request_t *user_pincode_request) {
	rsi_bt_app_set_event(RSI_APP_EVENT_PINCODE_REQ);
  memcpy ((int8_t *)remote_dev_addr, user_pincode_request->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on pincode: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_pincode_request->dev_addr),
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_linkkey_req
 * @brief      invoked when linkkey request event is received
 * @param[out] user_linkkey_req, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey request from remote device
 */
void rsi_bt_app_on_linkkey_req(uint16_t resp_status,
		rsi_bt_event_user_linkkey_request_t *user_linkkey_req) {
	rsi_bt_app_set_event(RSI_APP_EVENT_LINKKEY_REQ);
  memcpy ((int8_t *)remote_dev_addr, user_linkkey_req->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on linkkey req: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_req->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_linkkey_save
 * @brief      invoked when linkkey save event is received
 * @param[out] user_linkkey_req, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey save from local device
 */
void rsi_bt_app_on_linkkey_save(uint16_t resp_status,
		rsi_bt_event_user_linkkey_save_t *user_linkkey_save) {
	rsi_bt_app_set_event(RSI_APP_EVENT_LINKKEY_SAVE);
  memcpy ((int8_t *)remote_dev_addr, user_linkkey_save->dev_addr, RSI_DEV_ADDR_LEN);
#if RPS_LINK_KEY_SAVE
	rsi_link_key_add((rsi_bt_event_user_linkkey_save_t *) user_linkkey_save);
#else
	memcpy(linkkey, user_linkkey_save->linkKey, RSI_LINKKEY_REPLY_SIZE);
#endif
  LOG_PRINT ("on linkkey save: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_save->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_auth_complete
 * @brief      invoked when authentication event is received
 * @param[out] resp_status, authentication status
 * @param[out] auth_complete, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the pairing status and remote device information
 */
void rsi_bt_app_on_auth_complete(uint16_t resp_status,
		rsi_bt_event_auth_complete_t *auth_complete) {
	if (resp_status == 0) {
		rsi_bt_app_set_event(RSI_APP_EVENT_AUTH_COMPLT);
		app_state |= (1 << AUTHENTICATED);
	} else {
		//! remove link key from flash(SD card)
#if RPS_LINK_KEY_SAVE
		rsi_link_key_remove(auth_complete->dev_addr);
#else
		memset(linkkey, 0, RSI_LINKKEY_REPLY_SIZE);
#endif
	}
  memcpy ((int8_t *)remote_dev_addr, auth_complete->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on auth complete: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, auth_complete->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_unbond_status
 * @brief      invoked when disconnect event is received
 * @param[out] resp_status, disconnect status/error
 * @param[out] bt_disconnected, disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the disconnected device information
 */
void rsi_bt_app_on_unbond_status(uint16_t resp_status,
		rsi_bt_event_unbond_t *unbond_status) {

	rsi_bt_app_set_event(RSI_APP_EVENT_UNBOND_STATUS);
  memcpy ((int8_t *)remote_dev_addr, unbond_status->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on unbond status: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, unbond_status->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_disconn
 * @brief      invoked when disconnect event is received
 * @param[out] resp_status, disconnect status/error
 * @param[out] bt_disconnected, disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the disconnected device information
 */
void rsi_bt_app_on_disconn(uint16_t resp_status,
		rsi_bt_event_disconnect_t *bt_disconnected) {
	rsi_bt_app_set_event(RSI_APP_EVENT_DISCONNECTED);
  memcpy ((int8_t *)remote_dev_addr, bt_disconnected->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on disconn: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, bt_disconnected->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_confirm_request
 * @brief      invoked when confirmation request event is received
 * @param[out] confirmation request,confirmation request to remote device  
 * @return     none.
 * @section description
 * This callback function indicates the confirmation request event
 */
void rsi_bt_on_confirm_request(uint16_t resp_status,
		rsi_bt_event_user_confirmation_request_t *user_confirmation_request) {
	LOG_PRINT("\n data: %s", user_confirmation_request->confirmation_value);
	rsi_bt_app_set_event(RSI_APP_EVENT_CONFIRM_REQUEST);
	LOG_PRINT("\n on confirm req: bd addr %s, resp status 0x%x\r\n",
			str_conn_bd_addr, resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_mode_change
 * @brief      invoked when mode chande event is received
 * @param[out] mode change,mode change request to remote device
 * @return     none.
 * @section description
 * This callback function indicates the mode change event
 */
void rsi_bt_on_mode_change (uint16_t resp_status, 
    rsi_bt_event_mode_change_t  *mode_change) {
  memcpy ((int8_t *)remote_dev_addr, mode_change->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("mode_change_event: str_conn_bd_addr: %s, resp_status 0x%x\r\n", 
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, mode_change->dev_addr), 
      resp_status);
  LOG_PRINT ("mode : %d \r\n", mode_change->current_mode);
  dev_mode = mode_change->current_mode;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_connect
 * @brief      invoked when a2dp profile connected event is received
 * @param[out] a2dp_connect, a2dp connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp connected remote device information
 */
void rsi_bt_app_on_a2dp_connect(uint16_t resp_status,
		rsi_bt_event_a2dp_connect_t *a2dp_connect) {
	if (resp_status == 0) {
		rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_CONN);
		app_state |= (1 << A2DP_CONNECTED);
    memcpy ((int8_t *)remote_dev_addr, a2dp_connect->dev_addr, RSI_DEV_ADDR_LEN);
	}
  LOG_PRINT ("on a2dp conn: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_connect->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_disconnect
 * @brief      invoked when a2dp profile disconnected event is received
 * @param[out] a2dp_disconnect, a2dp disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp disconnected remote device information
 */
void rsi_bt_app_on_a2dp_disconnect(uint16_t resp_status,
		rsi_bt_event_a2dp_disconnect_t *a2dp_disconnect) {
	if (resp_status == 0) {
		app_state &= ~(1 << A2DP_CONNECTED);
		rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_DISCONN);
	}
  memcpy ((int8_t *)remote_dev_addr, a2dp_disconnect->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on a2dp disconn: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_disconnect->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_configure
 * @brief      invoked when a2dp profile configured event is received
 * @param[out] a2dp_connect, a2dp configured remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp configured remote device information
 */
void rsi_bt_app_on_a2dp_configure(uint16_t resp_status,
		rsi_bt_event_a2dp_configure_t *a2dp_configure) {
	rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_CONFIGURE);
  memcpy ((int8_t *)remote_dev_addr, a2dp_configure->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on a2dp configure: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_configure->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_a2dp_more_data_req
 * @brief      invoked when a2dp profile configured event is received
 * @param[out] a2dp_connect, a2dp configured remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp configured remote device information
 */
void rsi_bt_on_a2dp_more_data_req(uint16_t resp_status,
		rsi_bt_event_a2dp_more_data_req_t *a2dp_more_data_req) {
	if (resp_status == RSI_SUCCESS) {
		if (app_state & (1 << A2DP_STREAM_START)) {
			rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
      memcpy ((int8_t *)remote_dev_addr, a2dp_more_data_req->dev_addr, RSI_DEV_ADDR_LEN);
		}
	}
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_open
 * @brief      invoked when a2dp profile open event is received
 * @param[out] a2dp_open, a2dp open remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp open remote device information
 */
void rsi_bt_app_on_a2dp_open(uint16_t resp_status,
		rsi_bt_event_a2dp_open_t *a2dp_oen) {
	if (resp_status == 0) {
		rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_OPEN);
		app_state |= (1 << A2DP_STREAM_OPEN);
    memcpy ((int8_t *)remote_dev_addr, a2dp_oen->dev_addr, RSI_DEV_ADDR_LEN);
	}
  LOG_PRINT ("on a2dp open: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_oen->dev_addr), 
      resp_status);
}
/**
 * @fn         rsi_bt_app_on_a2dp_start
 * @brief      invoked when a2dp profile start event is received
 * @param[out] a2dp_start, a2dp start remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp start remote device information
 */
void rsi_bt_app_on_a2dp_start(uint16_t resp_status,
		rsi_bt_event_a2dp_start_t *a2dp_start) {
	uint32_t freq;

	if (resp_status == RSI_SUCCESS) {
		sample_freq = freq = a2dp_start->sample_freq;
		rem_mtu_size =
				(a2dp_start->rem_mtu_size > DEFAULT_MTU_SIZE) ?
						(a2dp_start->rem_mtu_size) : DEFAULT_MTU_SIZE;
		rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_START);
		app_state |= (1 << A2DP_STREAM_START);
	}

	LOG_PRINT("\n on a2dp start: bd addr %s, resp status 0x%x\r\n",
			str_conn_bd_addr, resp_status);
	LOG_PRINT("\n sample freq %d, rem mtu size %d\r\n", freq, rem_mtu_size);
}

/**
 * @fn         rsi_bt_app_on_a2dp_suspend
 * @brief      invoked when a2dp profile suspend event is received
 * @param[out] a2dp_suspend, a2dp suspend remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp suspend remote device information
 */
void rsi_bt_app_on_a2dp_suspend(uint16_t resp_status,
		rsi_bt_event_a2dp_suspend_t *a2dp_suspend) {
	if (resp_status == 0) {
		rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_SUSPEND);
	}

	LOG_PRINT("\n on a2dp suspend: bd addr %s, resp status 0x%x\r\n",
			str_conn_bd_addr, resp_status);
}

/**
 * @fn         rsi_bt_app_a2dp_pause
 * @brief      API to initiate a2dp stream pause
 * @param[out] none
 * @return     none.
 * @section description
 * This function request to susped the a2dp stream
 */
void rsi_bt_app_a2dp_pause() {
	int32_t status = 0;

	status = rsi_bt_a2dp_suspend(remote_dev_addr);
	if (status != RSI_SUCCESS) {
		LOG_PRINT("\n A2DP suspend req: bd addr %s, status err 0x%x\r\n",
				str_conn_bd_addr, status);
	} else {
		app_state &= ~(1 << A2DP_STREAM_START);
		rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
	}
}

/**
 * @fn         rsi_bt_app_a2dp_resume
 * @brief      API to initiate a2dp stream resume
 * @param[out] none
 * @return     none.
 * @section description
 * This function request to susped the a2dp stream
 */
void rsi_bt_app_a2dp_resume() {
	int32_t status = 0;

	status = rsi_bt_a2dp_start((uint8_t *)remote_dev_addr);
	if (status != RSI_SUCCESS) {
		LOG_PRINT("\n A2DP start req: bd addr %s, status err 0x%x\r\n",
				str_conn_bd_addr, status);
	} else {
    /*
     * 0x00 Active Mode.
     * 0x01 Hold Mode.
     * 0x02 Sniff Mode. */
    if (dev_mode == 0x02) {
      rsi_bt_sniff_exit_mode(remote_dev_addr);
    }
		app_state |= (1 << A2DP_STREAM_START);
		rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
	}
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_avrcp_connect
 * @brief       invoked when avrcp connected event is received
 * @param[out] avrcp_conn, remote device bdaddress
 * @return     none.
 * @section description
 * This callback function indicates the avrcp connected event
 */
void rsi_bt_app_on_avrcp_connect(uint16_t resp_status,
		rsi_bt_event_avrcp_connect_t *avrcp_connect) {

	if (resp_status == 0) {
		app_state |= (1 << AVRCP_CONNECTED);
		rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_CONN);
	}
  memcpy (remote_dev_addr, avrcp_connect->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp conn: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_connect->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_avrcp_disconnect
 * @brief       invoked when avrcp disconnected event is received
 * @param[out] avrcp_disconn, remote device bdaddress
 * @return     none.
 * @section description
 * This callback function indicates the avrcp disconnected event
 */
void rsi_bt_app_on_avrcp_disconnect(uint16_t resp_status,
		rsi_bt_event_avrcp_disconnect_t *avrcp_disconn) {
	if (resp_status == 0) {
		app_state &= ~(1 << AVRCP_CONNECTED);
		rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_DISCONN);
	}
  memcpy (remote_dev_addr, avrcp_disconn->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp disconn: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_disconn->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_avrcp_play
 * @brief      invoked when avrcp previous event is play 
 * @param[out] avrcp play,remote device bdaddress
 * @return     none.
 * @section description
 * This callback function indicates the avrcp play event
 */
void rsi_bt_app_on_avrcp_play(uint16_t resp_status,
		rsi_bt_event_avrcp_play_t *avrcp_play) {
	play_count++;
	if (play_count % 2 == 0) {
		rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_PLAY);
		play_count = 0;
	}
  memcpy (remote_dev_addr, avrcp_play->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp play: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_play->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_avrcp_pause
 * @brief      invoked when avrcp pause event is received
 * @param[out] avrcp pause,remote device bdaddress
 * @return     none.
 * @section description
 * This callback function indicates the avrcp pause event
 */
void rsi_bt_app_on_avrcp_pause(uint16_t resp_status,
		rsi_bt_event_avrcp_pause_t *avrcp_pause) {
	play_count++;
	if (play_count % 2 == 0) {
		rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_PAUSE);
		play_count = 0;
	}
  memcpy (remote_dev_addr, avrcp_pause->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp pause: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_pause->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_avrcp_next
 * @brief      invoked when avrcp next event is received
 * @param[out] avrcp pause,remote device bdaddress
 * @return     none.
 * @section description
 * This callback function indicates the avrcp next event
 */
void rsi_bt_app_on_avrcp_next(uint16_t resp_status,
		rsi_bt_event_avrcp_next_t *avrcp_next) {
	play_count++;
	if (play_count % 2 == 0) {
		rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_NEXT);
		play_count = 0;
	}
  memcpy (remote_dev_addr, avrcp_next->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp next: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_next->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_avrcp_previous
 * @brief      invoked when avrcp previous event is received
 * @param[out] avrcp pause,remote device bdaddress
 * @return     none.
 * @section description
 * This callback function indicates the avrcp previous event
 */
void rsi_bt_app_on_avrcp_previous(uint16_t resp_status,
		rsi_bt_event_avrcp_previous_t *avrcp_prev) {
	play_count++;
	if (play_count % 2 == 0) {
		rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_PREVIOUS);
		play_count = 0;
	}
  memcpy (remote_dev_addr, avrcp_prev->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp previous: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_prev->dev_addr), 
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_avrcp_get_cap_event
 * @brief      invoked when AVRCP get capabilities event is received
 * @param[out] resp_status, event status
 * @param[out] ssp_complete, secure simple pairing requested remote device information
 * @return     none.
 * @section description
 * This callback function indicates the ssp confirm event
 */
uint8_t app_cap_type;
uint8_t app_att_id;
rsi_bt_event_avrcp_ele_att_t app_song_atts;
rsi_bt_event_avrcp_get_cur_att_val_t app_att_list;
rsi_bt_event_avrcp_get_cur_att_val_t app_att_text_list;
rsi_bt_event_avrcp_vals_text_req_t app_vals_list;

void rsi_bt_on_avrcp_get_cap_event(uint8_t *bd_addr, uint8_t cap_type) {
	app_cap_type = cap_type;
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_GET_CAP);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_GET_CAP: cap_type: %d\r\n", cap_type);
	return;
}

void rsi_bt_on_avrcp_get_app_supp_atts_event(uint8_t *bd_addr) {
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_GET_ATTS);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_GET_ATTS\r\n");
	return;
}

void rsi_bt_on_avrcp_get_app_suup_att_vals_event(uint8_t *bd_addr,
		uint8_t att_id) {
	app_att_id = att_id;
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_GET_ATT_VALS);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_GET_ATT_VALS: att_id: %d\r\n", att_id);
	return;
}

void rsi_bt_on_avrcp_get_app_cur_att_val_event(
		rsi_bt_event_avrcp_get_cur_att_val_t *p_att_list) {
	uint8_t ix;

	memcpy(&app_att_list, p_att_list,
			sizeof(rsi_bt_event_avrcp_get_cur_att_val_t));
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_CUR_ATT_VALS);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_CUR_ATT_VALS: nbr_atts: %d, atts: ",
			p_att_list->nbr_atts);
	for (ix = 0; ix < p_att_list->nbr_atts; ix++) {
		LOG_PRINT("%d, ", p_att_list->atts);
	}
	LOG_PRINT("\r\n");

	return;
}

void rsi_bt_on_avrcp_set_app_cur_att_val_event(
		rsi_bt_event_avrcp_set_att_val_t *p_att_list) {
	uint8_t ix;

	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_SET_ATT_VALS);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_CUR_ATT_VALS: nbr_atts:%d\r\n",
			p_att_list->nbr_atts);
	for (ix = 0; ix < p_att_list->nbr_atts; ix++) {
		LOG_PRINT("attid: %d - attvalue: %d \r\n",
				p_att_list->att_list[ix].attid,
				p_att_list->att_list[ix].attvalue);
	}
	LOG_PRINT("\r\n");
	return;
}

void rsi_bt_on_avrcp_get_app_att_text_event(
		rsi_bt_event_avrcp_get_cur_att_val_t *p_att_text_list) {
	uint8_t ix;

	memcpy(&app_att_text_list, p_att_text_list,
			sizeof(rsi_bt_event_avrcp_get_cur_att_val_t));
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_ATT_TEXT);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_ATT_TEXT: nbr_atts: %d, atts: ",
			p_att_text_list->nbr_atts);
	for (ix = 0; ix < p_att_text_list->nbr_atts; ix++) {
		LOG_PRINT("%d, ", p_att_text_list->atts[ix]);
	}
	LOG_PRINT("\r\n");
	return;
}

void rsi_bt_on_avrcp_get_app_att_vals_text_event(
		rsi_bt_event_avrcp_vals_text_req_t *p_vals_list) {
	uint8_t ix;

	memcpy(&app_vals_list, p_vals_list,
			sizeof(rsi_bt_event_avrcp_vals_text_req_t));
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT);
	LOG_PRINT(
			"\n RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT: att_id: %d, nbr_vals:%d, vals:",
			p_vals_list->att_id, p_vals_list->nbr_vals);
	for (ix = 0; ix < p_vals_list->nbr_vals; ix++) {
		LOG_PRINT("%d, ", p_vals_list->vals);
	}
	LOG_PRINT("\r\n");
	return;
}
#if 0

void rsi_bt_on_avrcp_supp_char_sets_event(
		rsi_bt_event_avrcp_char_sets_t *p_char_sets) {
	uint8_t ix;

	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_CHAR_SET);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_CHAR_SET: nbr_sets:%d, sets: ",
			p_char_sets->nbr_sets);
	for (ix = 0; ix < p_char_sets->nbr_sets; ix++) {
		LOG_PRINT("%d, ", p_char_sets->char_sets[ix]);
	}
	LOG_PRINT("\r\n");
	return;
}

void rsi_bt_on_avrcp_batt_status_event(uint8_t *bd_addr, uint8_t batt_status) {
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_BATT_STATUS);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_BATT_STATUS: batt_status: %d\r\n",
			batt_status);
	return;
}
#endif 

void rsi_bt_on_avrcp_get_song_atts_event(
		rsi_bt_event_avrcp_ele_att_t *p_song_atts) {
	uint8_t ix;

	memcpy(&app_song_atts, p_song_atts, sizeof(rsi_bt_event_avrcp_ele_att_t));
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_SONG_ATTS);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_SONG_ATTS: nbr_atts: %d, att_list:",
			p_song_atts->nbr_atts);
	for (ix = 0; ix < p_song_atts->nbr_atts; ix++) {
		LOG_PRINT("%d, ", p_song_atts->ele_atts[ix]);
	}
	LOG_PRINT("\r\n");

	return;
}
void rsi_bt_on_avrcp_get_play_status_event(uint8_t *bd_addr) {
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_PLAY_STATUS);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_PLAY_STATUS: \r\n");
	return;
}

uint8_t app_event_id[5];
uint8_t app_reg_events;
void rsi_bt_on_avrcp_reg_notify_event(uint8_t *bd_addr, uint8_t event_id) {
	app_event_id[app_reg_events++] = event_id;
	rsi_bt_app_set_event(RSI_APP_EVENT_AVRCP_REG_EVENT);
	LOG_PRINT("\n RSI_APP_EVENT_AVRCP_REG_EVENT: event_id: %d\r\n", event_id);
	return;
}

#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
int32_t sbc_calc_frame_len(uint8_t *data) {
	int32_t ret = 0;
	uint8_t block_mode, mode, subband_mode, subbands, channels, blocks, joint,
			bitpool;

	if (data[0] != SBC_SYNCWORD)
		return -2;

	block_mode = (data[1] >> 4) & 0x03;

	switch (block_mode) {
	case SBC_BLK_4:
		blocks = 4;
		break;
	case SBC_BLK_8:
		blocks = 8;
		break;
	case SBC_BLK_12:
		blocks = 12;
		break;
	case SBC_BLK_16:
		blocks = 16;
		break;
	}

	mode = (data[1] >> 2) & 0x03;

	switch (mode) {
	case MONO:
		channels = 1;
		break;
	case DUAL_CHANNEL: /* fall-through */
	case STEREO:
	case JOINT_STEREO:
		channels = 2;
		break;
	}

	subband_mode = (data[1] & 0x01);
	subbands = subband_mode ? 8 : 4;

	bitpool = data[2];

	if ((mode == MONO || mode == DUAL_CHANNEL) && bitpool > 16 * subbands)
		return -4;

	if ((mode == STEREO || mode == JOINT_STEREO) && bitpool > 32 * subbands)
		return -4;

	joint = mode == SBC_MODE_JOINT_STEREO ? 1 : 0;

	ret = 4 + (4 * subbands * channels) / 8;
	/* This term is not always evenly divide so we round it up */
	if (channels == 1)
		ret += ((blocks * channels * bitpool) + 7) / 8;
	else
		ret += (((joint ? subbands : 0) + blocks * bitpool) + 7) / 8;

	return ret;
}

#if (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
int16_t read_sbc_data()
#else
int16_t read_sbc_data(FILE *fp)
#endif
{
	FRESULT error;
	uint8_t p_sbc_data[RSI_SBC_DATA_SIZE];
	UINT bytesRead;
	pkt_size = 0;
	sbc_frames = 0;

	while (pkt_size < (rem_mtu_size - (SKIP_SIZE))) {
		//! read file 1 byte
#if (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
		error = f_read(&g_fileObject_sd, &p_sbc_data[pkt_size], 4, &bytesRead);
		if (error) {
			PRINTF("\n Read file failed. \r\n");
			return error;
		} else {
			if (bytesRead < SBC_HEADER_SIZE) {
				return RSI_APP_ERR_AUDIO_EOF;
			} else {
				pkt_size += bytesRead;
			}
		}
#else
		pkt_size += fread (&p_sbc_data[pkt_size], ELEMENT_READ_SIZE, 4, fp);
#endif
		//! check SW == 0x9C
		if (p_sbc_data[0] == SBC_SYNCWORD) {
			sbc_frame_size = sbc_calc_frame_len(&p_sbc_data[0]);
			sbc_frames = rem_mtu_size/sbc_frame_size;
			//! read file <size-1>
#if (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
			//rsi_hal_set_gpio(RSI_HAL_RESET_PIN);
			error = f_read(&g_fileObject_sd, &p_sbc_data[pkt_size],
					((sbc_frames * sbc_frame_size) - 4), &bytesRead);
			//rsi_hal_clear_gpio(RSI_HAL_RESET_PIN);
			if (error) {
				PRINTF("\n Read file failed. \r\n");
				return error;
			} else {
				if (bytesRead < (sbc_frame_size - SBC_HEADER_SIZE)) {
					return RSI_APP_ERR_AUDIO_EOF;
				} else {
					pkt_size += bytesRead;
				}
			}
#else
			pkt_size += fread (&p_sbc_data[pkt_size], ELEMENT_READ_SIZE, (sbc_frame_size - 4), fp);
#endif
		} else {
			//! error ...
			LOG_PRINT(" fread err\n\n");
			return RSI_APP_ERR_AUDIO_EOF;
		}
		//sbc_frames++;
	}
	memset(sbc_data, 0, sizeof(sbc_data));
	memcpy(sbc_data, p_sbc_data, pkt_size);
#if 0
	while (pkt_size < (rem_mtu_size - (SKIP_SIZE))) {
		//! read file 1 byte
#if (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
		error = f_read(&g_fileObject_sd, &p_sbc_data[pkt_size], 4, &bytesRead);
		if (error) {
			PRINTF("\n Read file failed. \r\n");
			return error;
		} else {
			if (bytesRead < SBC_HEADER_SIZE) {
				return RSI_APP_ERR_AUDIO_EOF;
			} else {
				pkt_size += bytesRead;
			}
		}
#else
		pkt_size += fread (&p_sbc_data[pkt_size], ELEMENT_READ_SIZE, 4, fp);
#endif
		//! check SW == 0x9C
		if (p_sbc_data[0] == SBC_SYNCWORD) {
			sbc_frame_size = sbc_calc_frame_len(&p_sbc_data[0]);
			//! read file <size-1>
#if (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
			error = f_read(&g_fileObject_sd, &p_sbc_data[pkt_size],
					(sbc_frame_size - 4), &bytesRead);
			if (error) {
				PRINTF("\n Read file failed. \r\n");
				return error;
			} else {
				if (bytesRead < (sbc_frame_size - SBC_HEADER_SIZE)) {
					return RSI_APP_ERR_AUDIO_EOF;
				} else {
					pkt_size += bytesRead;
				}
			}
#else
			pkt_size += fread (&p_sbc_data[pkt_size], ELEMENT_READ_SIZE, (sbc_frame_size - 4), fp);
#endif
		} else {
			//! error ...
			LOG_PRINT("\n fread err\n\n");
			return RSI_APP_ERR_AUDIO_EOF;
		}
		sbc_frames++;
	}
	memset(sbc_data, 0, sizeof(sbc_data));
	memcpy(sbc_data, p_sbc_data, pkt_size);
#endif
	return 0;
}
#endif
#if (RSI_AUDIO_DATA_SRC == BIN_FILE) 
int16_t open_audio_file(FILE **fp, uint8_t *file_name) {
	*fp = fopen(file_name, "rb");
	if (*fp == NULL) {
		LOG_PRINT("\n file open fail\r\n");
		return 1;
	}
	LOG_PRINT("\n file opend fp: 0x%x\r\n", *fp);
	fseek(*fp, 0, SEEK_SET);

	return 0;
}

int16_t close_audio_file(FILE **fp) {
	if (*fp == NULL) {
		LOG_PRINT("\n fp invalid\r\n");
		return 1;
	}

	fclose(*fp);
	LOG_PRINT("\n file closed fp: 0x%x\r\n", *fp);

	return 0;
}

#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
int32_t sbc_calc_frame_len(uint8_t *data) {
	int32_t ret = 0;
	uint8_t block_mode, mode, subband_mode, subbands, channels, blocks, joint,
	bitpool;

	if (data[0] != SBC_SYNCWORD)
	return -2;

	block_mode = (data[1] >> 4) & 0x03;

	switch (block_mode) {
		case SBC_BLK_4:
		blocks = 4;
		break;
		case SBC_BLK_8:
		blocks = 8;
		break;
		case SBC_BLK_12:
		blocks = 12;
		break;
		case SBC_BLK_16:
		blocks = 16;
		break;
	}

	mode = (data[1] >> 2) & 0x03;

	switch (mode) {
		case MONO:
		channels = 1;
		break;
		case DUAL_CHANNEL: /* fall-through */
		case STEREO:
		case JOINT_STEREO:
		channels = 2;
		break;
	}

	subband_mode = (data[1] & 0x01);
	subbands = subband_mode ? 8 : 4;

	bitpool = data[2];

	if ((mode == MONO || mode == DUAL_CHANNEL) && bitpool > 16 * subbands)
	return -4;

	if ((mode == STEREO || mode == JOINT_STEREO) && bitpool > 32 * subbands)
	return -4;

	joint = mode == SBC_MODE_JOINT_STEREO ? 1 : 0;

	ret = 4 + (4 * subbands * channels) / 8;
	/* This term is not always evenly divide so we round it up */
	if (channels == 1)
	ret += ((blocks * channels * bitpool) + 7) / 8;
	else
	ret += (((joint ? subbands : 0) + blocks * bitpool) + 7) / 8;

	return ret;
}

int16_t read_sbc_data(FILE *fp) {
	int16_t fread_resp = 0;
	uint8_t p_sbc_data[RSI_SBC_DATA_SIZE] = {0};

	pkt_size = 0;
	sbc_frames = 0;

	while (pkt_size < (rem_mtu_size - SKIP_SIZE)) {
		//! read file 1 byte
		fread_resp = fread(&p_sbc_data[pkt_size], ELEMENT_READ_SIZE,
				SBC_HEADER_SIZE, fp);
		if (fread_resp < SBC_HEADER_SIZE) {
			//! error ...
			LOG_PRINT("\n fread err\n\n");
			return RSI_APP_ERR_AUDIO_EOF;
		} else {
			pkt_size += fread_resp;
		}

		//! check SW == 0x9C
		if (p_sbc_data[0] == SBC_SYNCWORD) {
			//! read file <size-1>
			sbc_frame_size = sbc_calc_frame_len(&p_sbc_data[0]);
			fread_resp = fread(&p_sbc_data[pkt_size], ELEMENT_READ_SIZE,
					(sbc_frame_size - SBC_HEADER_SIZE), fp);
			if (fread_resp < (sbc_frame_size - SBC_HEADER_SIZE)) {
				//! error ...
				LOG_PRINT("\n fread err\n\n");
				return RSI_APP_ERR_AUDIO_EOF;
			} else {
				pkt_size += fread_resp;
			}
		} else {
			//! error ...
			LOG_PRINT("\n fread err\n\n");
			return RSI_APP_ERR_AUDIO_EOF;
		}
		sbc_frames++;
	}
	memset(sbc_data, 0, sizeof(sbc_data));
	memcpy(sbc_data, p_sbc_data, pkt_size);

	return 0;
}

#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
int16_t read_pcm_data(FILE *fp)
{
	pkt_size = fread (pcm_data, ELEMENT_READ_SIZE, sizeof (pcm_data), fp);
	if (pkt_size < 512) {
		return RSI_APP_ERR_AUDIO_EOF;
	}

	return 0;
}

#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)

unsigned int unsynchsafe(uint32_t be_in)
{
	unsigned int out = 0ul, mask = 0x7F000000ul;
	unsigned int in = 0ul;

	/* be_in is now big endian */
	/* convert to little endian */
	in = ((be_in >> 24) | ((be_in >> 8) & 0xFF00ul) |
			((be_in << 8) & 0xFF0000ul) | (be_in << 24));

	while (mask) {
		out >>= 1;
		out |= (in & mask);
		mask >>= 8;
	}

	return out;
}

//! Makes sure the file is supported and return the correct size
int mp3Header(FILE* media, ID3TAG* tag)
{
	unsigned int tag_size;

	fread(tag, sizeof(ID3TAG), 1, media);

	if(memcmp ((tag->id), "ID3", 3))
	{
		return -1;
	}

	tag_size = unsynchsafe(tag->size);
	LOG_PRINT("\n tag_size = %x\n", tag_size);
	return tag_size;
}

void rsi_mp3_remove_id3_tag(FILE *fp)
{
	unsigned int id3_size,offset;
	ID3TAG mp3_tag;

	rewind(fp);
	id3_size = mp3Header(fp, &mp3_tag);
	LOG_PRINT("\n id3_size = %x\n", id3_size);

	if(mp3_tag.id[0] == 'I' &&
			mp3_tag.id[1] == 'D' &&
			mp3_tag.id[2] == '3')
	{
		fseek(fp, id3_size, SEEK_CUR);
		LOG_PRINT("\nid3 matched\n");
	}
	else
	{
		LOG_PRINT("\nid3 did not matched\n");
	}
	return;
}

int16_t read_mp3_data(FILE *fp)
{
	pkt_size = fread (mp3_data, 1, sizeof (mp3_data), fp);
	// LOG_PRINT ("\r\nmp3 pkt len read: %d\r\n", pkt_size);
	if (pkt_size == 0) {
		return RSI_APP_ERR_AUDIO_EOF;
	}

	return 0;
}
#endif
#elif (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
int16_t open_audio_file_from_sd() {
	FRESULT error;
#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
	// Open sbc audio file
	error = f_open(&g_fileObject_sd, (const TCHAR*)_T(app_song_list[app_song_list_idx]),
			(FA_WRITE | FA_READ | FA_OPEN_EXISTING));

	if (error) {
		PRINTF("\n Open existing file failed\r\n");
		return error;
	}
#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)

	// Open pcm audio file
	error = f_open(&g_fileObject_sd, _T("/pcm.wav"), (FA_WRITE | FA_READ | FA_OPEN_EXISTING));
	if(error)
	{
		PRINTF("\n Open existing file failed\r\n");
		return error;
	}
#endif

#if (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
	// Open mp3 audio file
	error = f_open(&g_fileObject_sd, _T("/mp3.mp3"), (FA_WRITE | FA_READ | FA_OPEN_EXISTING));
	if(error)
	{
		PRINTF("\n Open existing file failed\r\n");
		return error;
	}
#endif
	/* Move the file pointer */
	error = f_lseek(&g_fileObject_sd, 0U);
	if (error) {
		PRINTF("\n Set file pointer position failed. \r\n");
		return error;
	}

	return 0;
}
#if (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
int16_t read_pcm_data_from_sd()
{
	uint32_t error = 0;
	UINT bytesRead;

	memset(g_bufferRead, '\0', sizeof(g_bufferRead));
	error = f_read(&g_fileObject_sd, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
	if (error || (bytesRead == 0))
	{
		PRINTF("\n Read file failed. \r\n");
		return error;
	}

	rsi_delay_ms(sbc_frames*24000); //! WO PS

	return 0;
}
#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
int16_t read_mp3_data_from_sd()
{
	uint32_t error = 0;
	UINT bytesRead;
	reread:
	memset(g_bufferRead, '\0', sizeof(g_bufferRead));
	error = f_read(&g_fileObject_sd, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
	if (error)
	{
		PRINTF("\n Read file failed. \r\n");
		return error;
	}

	if (bytesRead == 0) {
		f_lseek(&g_fileObject_sd, 0U);
		goto reread;
	}
	return 0;
}
#elif (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
int16_t read_sbc_data_from_sd() {
	uint32_t error = 0;
	UINT bytesRead;
	reread: memset(g_bufferRead, '\0', sizeof(g_bufferRead));
	error = f_read(&g_fileObject_sd, g_bufferRead, sizeof(g_bufferRead),
			&bytesRead);
	if (error) {
		PRINTF("\n Read file failed. \r\n");
		return error;
	}

	if (bytesRead == 0) {
		f_lseek(&g_fileObject_sd, 0U);
		goto reread;
	}
	return 0;
}
#endif

#endif

int16_t send_audio_data(FILE *fp) {
#if	ENABLE_POWER_SAVE
	uint32_t status = 0;
#endif
	int16_t err = 0;

#if (RSI_AUDIO_DATA_SRC == BIN_FILE)

#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
	err = read_sbc_data(fp);
	if (err) {
		return err;
	}

	err = rsi_bt_a2dp_send_sbc_aac_data(remote_dev_addr, sbc_data, pkt_size,
			0);
#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
	err = read_pcm_data(fp);

	if(err)
	return err;
#if (A2DP_BURST_MODE == 1)
	if (pkt_size >= 512)
#endif
	err = rsi_bt_a2dp_send_pcm_mp3_data (remote_dev_addr, pcm_data, pkt_size, RSI_AUDIO_DATA_TYPE);

#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
	err = read_mp3_data(fp);
	if(err)
	return err;
	err = rsi_bt_a2dp_send_pcm_mp3_data (remote_dev_addr, mp3_data, pkt_size, RSI_AUDIO_DATA_TYPE);

#endif

#elif (RSI_AUDIO_DATA_SRC == ARRAY)

#if (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
	static int16_t ix = 0;

	err = rsi_bt_a2dp_send_pcm_mp3_data (remote_dev_addr, pcm_data_buff + (ix * RSI_PCM_DATA_SIZE), RSI_PCM_DATA_SIZE, RSI_AUDIO_DATA_TYPE);
	ix = (ix>587) ? 0 : (ix + 1);

#endif

#elif (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
#if (ENABLE_POWER_SAVE)
	if (!power_save_given)
	{
		status = rsi_bt_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
		if (status != RSI_SUCCESS)
		{
			return status;
		}
		power_save_given = 1;
	}
#endif

	err = read_sbc_data();
	if (err)
		return err;

	err = rsi_bt_a2dp_send_sbc_aac_data(remote_dev_addr, sbc_data, pkt_size,
			0);
#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
#if (ENABLE_POWER_SAVE)
	if (!power_save_given)
	{
		status = rsi_bt_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
		if (status != RSI_SUCCESS)
		{
			return status;
		}
		power_save_given = 1;
	}
#endif
	err = read_pcm_data_from_sd();

	if(err)
	return 1;

	err = rsi_bt_a2dp_send_pcm_mp3_data (remote_dev_addr, g_bufferRead, 512, RSI_AUDIO_DATA_TYPE);
#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
	err = read_mp3_data_from_sd();

	if(err)
	return 1;
	err = rsi_bt_a2dp_send_pcm_mp3_data (remote_dev_addr, g_bufferRead, 512, RSI_AUDIO_DATA_TYPE);
#endif
#endif

#if (A2DP_BURST_MODE == 0)
#ifdef __linux__
#if (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
	usleep(2200);
#elif (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
	usleep(3000);
#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
	usleep(12000);
#endif
#elif (defined RSI_M4_INTERFACE)
#if (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
	rsi_delay_ms(3);
#elif (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
	rsi_delay_ms(3);
#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
	rsi_delay_ms(12);
#endif
#endif

	return 0;
#else
	return err;
#endif
}
#if 0
uint8_t set_cnt = 0;
uint8_t reset_cnt = 0;
uint8_t gpio_value = 0;
void gpio_test()
{
	uint8_t ix = 0;
	while (ix < 10)
	{

		GPIO_WritePinOutput(BOARD_INITPINS_PS_OUT_GPIO, BOARD_INITPINS_PS_OUT_PIN, 1);
		rsi_delay_ms(1);
		gpio_value = GPIO_ReadPinInput(BOARD_INITPINS_PS_IN_GPIO, BOARD_INITPINS_PS_IN_PIN);
		if (gpio_value)
		set_cnt++;
		rsi_delay_ms(1);
		GPIO_WritePinOutput(BOARD_INITPINS_PS_OUT_GPIO, BOARD_INITPINS_PS_OUT_PIN, 0);
		rsi_delay_ms(1);
		gpio_value = GPIO_ReadPinInput(BOARD_INITPINS_PS_IN_GPIO, BOARD_INITPINS_PS_IN_PIN);
		if (!gpio_value)
		reset_cnt++;
		rsi_delay_ms(1);
		ix++;
	}
}
#endif
/*==============================================*/
/**
 * @fn         rsi_bt_a2dp_source
 * @brief      Tests the BT Classic A2DP Source.
 * @param[in]  none
 * @return    none.
 * @section description
 * This function is used to test the A2DP Source.
 */
int32_t rsi_bt_a2dp_source(void) {
	int32_t status = 0;
	uint8_t str_bd_addr[BD_ADDR_ARRAY_LEN] = { 0 };
	uint8_t eir_data[64] = { 2, 1, 0 };

#if (RSI_AUDIO_DATA_SRC == BIN_FILE) 
	status = open_audio_file(&fp, app_song_list[app_song_list_idx]);
	if(status != RSI_SUCCESS) {
		return 0;
	}

#ifdef RMOVE_ID3_TAG
	rsi_mp3_remove_id3_tag(fp);
	LOG_PRINT("mp3 data without id3 tag\n");
#endif

#elif (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
	status = open_audio_file_from_sd();
	if (status)
		return 0;

#else

#if (RSI_AUDIO_DATA_TYPE != PCM_AUDIO)
	LOG_PRINT("audio type combination not supported\n");
	return 0;
#endif

#endif

#ifdef RSI_UART_FLOW_CTRL_ENABLE
	rsi_cmd_uart_flow_ctrl (RSI_ENABLE);
#endif
#if RPS_LINK_KEY_SAVE
	rsi_init_link_key_list();
#endif

#ifdef RSI_WITH_OS
  //rsi_mutex_create (&bt_event_mutex);
#endif
	//! BT register GAP callbacks:
	rsi_bt_gap_register_callbacks(
			NULL, //role_change
			rsi_bt_app_on_conn, rsi_bt_app_on_unbond_status,
			rsi_bt_app_on_disconn,
			NULL, //scan_resp
			NULL, //remote_name_req
			NULL, //rsi_bt_on_passkey_display,//passkey_display
			NULL, //remote_name_req+cancel
			rsi_bt_on_confirm_request, //confirm req
			rsi_bt_app_on_pincode_req,
			NULL, //rsi_bt_on_passkey_request,//passkey request
			NULL, //inquiry complete
			rsi_bt_app_on_auth_complete, rsi_bt_app_on_linkkey_req, //linkkey request
			NULL, //rsi_bt_on_ssp_complete,//ssp coplete
			rsi_bt_app_on_linkkey_save,
			NULL, //get services
			NULL,
			rsi_bt_on_mode_change,
			NULL); //search service

	//! initialize the event map
	rsi_bt_app_init_events();

	//! get the local device address(MAC address).
	status = rsi_bt_get_local_device_address(local_dev_addr);
	if (status != RSI_SUCCESS) {
		return status;
	}
	LOG_PRINT("\r\nlocal_bd_address: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *) str_bd_addr, local_dev_addr));

	//! set the local device name
	status = rsi_bt_set_local_name(RSI_BT_LOCAL_NAME);
	if (status != RSI_SUCCESS) {
		return status;
	}

	//! get the local device name
	status = rsi_bt_get_local_name(&local_name);
	if (status != RSI_SUCCESS) {
		return status;
	}
	LOG_PRINT("\r\nlocal_name: %s\r\n", local_name.name);

	//! prepare Extended Response Data
	eir_data[3] = strlen(RSI_BT_LOCAL_NAME) + 1;
	eir_data[4] = 9;
	strcpy((char *) &eir_data[5], RSI_BT_LOCAL_NAME);
	//! set eir data
	rsi_bt_set_eir_data(eir_data, strlen(RSI_BT_LOCAL_NAME) + 5);
#if (RSI_APP_AVDTP_ROLE != INITIATOR_ROLE)
	//! start the discover mode
	status = rsi_bt_start_discoverable();
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! start the connectability mode
	status = rsi_bt_set_connectable();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#else
	//! stop the discover mode
	status = rsi_bt_stop_discoverable();
	if (status != RSI_SUCCESS) {
		return status;
	}

	//! stop the connectability mode
	status = rsi_bt_set_non_connectable();
	if (status != RSI_SUCCESS) {
		return status;
	}
#endif

//! start the ssp mode
	status = rsi_bt_set_ssp_mode(1, 1);
	if (status != RSI_SUCCESS) {
		return status;
	}
	//! initialize the A2DP profile
	status = rsi_bt_a2dp_init();
	if (status != RSI_SUCCESS) {
		return status;
	}

	//! start the ssp mode
	status = rsi_bt_set_ssp_mode(1, 1);
	if (status != RSI_SUCCESS) {
		return status;
	}

	//! register the A2DP profile callback's
	rsi_bt_a2dp_register_callbacks(rsi_bt_app_on_a2dp_connect,
			rsi_bt_app_on_a2dp_disconnect, rsi_bt_app_on_a2dp_configure,
			rsi_bt_app_on_a2dp_open, rsi_bt_app_on_a2dp_start,
			rsi_bt_app_on_a2dp_suspend,
			NULL, //rsi_bt_app_on_a2dp_abort,
			NULL, //rsi_bt_app_on_a2dp_close,
			NULL, //rsi_bt_on_a2dp_encode_data,
			NULL, //rsi_bt_on_a2dp_pcm_data
			rsi_bt_on_a2dp_more_data_req);

	//! initialize the AVRCP profile
	status = rsi_bt_avrcp_init();
	if (status != RSI_SUCCESS) {
		return status;
	}

	rsi_bt_avrcp_register_callbacks(rsi_bt_app_on_avrcp_connect,
			rsi_bt_app_on_avrcp_disconnect, rsi_bt_app_on_avrcp_play,
			rsi_bt_app_on_avrcp_pause, NULL, rsi_bt_app_on_avrcp_next,
			rsi_bt_app_on_avrcp_previous,
			NULL,NULL,NULL,NULL);

	rsi_bt_avrcp_target_register_callbacks(rsi_bt_on_avrcp_get_cap_event,
			rsi_bt_on_avrcp_get_app_supp_atts_event,
			rsi_bt_on_avrcp_get_app_suup_att_vals_event,
			rsi_bt_on_avrcp_get_app_cur_att_val_event,
			rsi_bt_on_avrcp_set_app_cur_att_val_event,
			rsi_bt_on_avrcp_get_app_att_text_event,
			rsi_bt_on_avrcp_get_app_att_vals_text_event,
			NULL, //rsi_bt_on_avrcp_supp_char_sets_event,
			NULL, //rsi_bt_on_avrcp_batt_status_event,
			rsi_bt_on_avrcp_get_song_atts_event,
			rsi_bt_on_avrcp_get_play_status_event,
			rsi_bt_on_avrcp_reg_notify_event,
			NULL);

#if (RSI_APP_AVDTP_ROLE != ACCEPTOR_ROLE)
	status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr,RSI_BT_REMOTE_BD_ADDR));
	if (status != RSI_SUCCESS) {
		rsi_bt_app_set_event(RSI_APP_EVENT_DISCONNECTED);
	}
#endif

	return 0;
}

/*==============================================*/
/**
 * @fn         rsi_app_bt_task_WIFI_6
 * @brief      this function is used to handle the BT events.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This is used to check the bt events and process the events
 */
int32_t rsi_bt_app_task() {
	int32_t status = 0;
	int32_t temp_event_map = 0;
	uint8_t ix;
	FILE *fp = NULL;
	uint32_t file_size = 0;
#ifdef RSI_WITH_OS
	rsi_semaphore_wait(&coex_sem, 0);

	//! BT A2DP Initialization
	status = rsi_bt_a2dp_source();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("bt_a2dp init failed\n");
		return;
	}
	while(1)
#endif
	{
		//! Application main loop

		//! checking for received events
		temp_event_map = rsi_bt_app_get_event();
		if (temp_event_map == RSI_FAILURE) {
			//! if events are not received loop will be continued.
#ifdef RSI_WITH_OS
			continue;
#else
			return 0;			
#endif
		}

		//! if any event is received, it will be served.
		switch (temp_event_map) {
		case RSI_APP_EVENT_CONNECTED: {
			//! remote device connected event

			//! clear the connected event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_CONNECTED);
			if ((app_state & (1 << CONNECTED))
					&& (app_state & (1 << AUTHENTICATED))) {
#if (RSI_APP_AVDTP_ROLE != ACCEPTOR_ROLE)

				rsi_delay_ms(500);

				status = rsi_bt_a2dp_connect(remote_dev_addr);
				if (status != RSI_SUCCESS) {
					if (status == RSI_APP_ERR_SOCK_DISCONN) {
						rsi_bt_app_set_event(RSI_APP_EVENT_DISCONNECTED);
						app_state = 0;
					} 
					else if (status == RSI_APP_ERR_A2DP_CONN_ALRDY_EXISTS){}
					else
						rsi_bt_app_set_event(RSI_APP_EVENT_CONNECTED);
					LOG_PRINT("\n a2dp conn resp status 0x%04x\r\n", status);
				}

#endif
			}
		}
			break;

		case RSI_APP_EVENT_LINKKEY_REQ: {
			//! linkkey request event

			//! clear the linkkey request event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_LINKKEY_REQ);
#if RPS_LINK_KEY_SAVE
			rsi_bt_event_user_linkkey_save_t peer_link;
			//rsi_ascii_dev_address_to_6bytes_rev(peer_link.dev_addr, (int8_t *) str_conn_bd_addr);
			memcpy (peer_link.dev_addr, (int8_t *) remote_dev_addr, 6);
			if (rsi_link_key_read(&peer_link)) {
				memcpy(linkkey, peer_link.linkKey, RSI_LINK_KEY_LEN);
			} else {
				memset(linkkey, 0, RSI_LINK_KEY_LEN);
			}
#endif
			if ((linkkey[0] == 0) && (linkkey[15] == 0)) {
				LOG_PRINT("\n linkkey request negative reply\r\n");
				//! sending the linkkey request negative reply
				rsi_bt_linkkey_request_reply((int8_t *) remote_dev_addr,
				NULL, 0);
			} else {
				LOG_PRINT("\n linkkey request positive reply\r\n");
				//! sending the linkkey request positive reply
				rsi_bt_linkkey_request_reply((int8_t *) remote_dev_addr,
						linkkey, 1);
			}
		}
			break;

		case RSI_APP_EVENT_PINCODE_REQ: {
			//! pincode request event
			uint8_t *pin_code = (uint8_t *) PIN_CODE;

			//! clear the pincode request event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_PINCODE_REQ);

			//! sending the pincode requet reply
			status = rsi_bt_pincode_request_reply((int8_t *) remote_dev_addr,
					pin_code, 1);
			if (status != RSI_SUCCESS) {
				//return status;
			}
		}
			break;

		case RSI_APP_EVENT_LINKKEY_SAVE: {
			//! linkkey save event

			//! clear the linkkey save event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_LINKKEY_SAVE);
		}
			break;
		case RSI_APP_EVENT_AUTH_COMPLT: {
			//! authentication complete event

			//! clear the authentication complete event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_AUTH_COMPLT);
			if ((app_state & (1 << CONNECTED))
					&& (app_state & (1 << AUTHENTICATED))) {
#if (RSI_APP_AVDTP_ROLE != ACCEPTOR_ROLE)
				rsi_delay_ms(500);
				status = rsi_bt_a2dp_connect(remote_dev_addr);
				if (status != RSI_SUCCESS) {
					if (status == RSI_APP_ERR_SOCK_DISCONN) {
						rsi_bt_app_set_event(RSI_APP_EVENT_DISCONNECTED);
						app_state = 0;
					}
					else if (status == RSI_APP_ERR_A2DP_CONN_ALRDY_EXISTS){}
					else
						rsi_bt_app_set_event(RSI_APP_EVENT_CONNECTED);

					LOG_PRINT("\n a2dp conn resp status 0x%04x\n", status);
				}

#endif
			}
		}
			break;

		case RSI_APP_EVENT_UNBOND_STATUS: {
			//! remote device unbond event

			//! clear the unbond event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_UNBOND_STATUS);

			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
			rsi_bt_app_set_event(RSI_APP_EVENT_DISCONNECTED);

			app_state = 0;
		}
			break;

		case RSI_APP_EVENT_DISCONNECTED: {
			//! remote device disconnect event

			//! clear the disconnected event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_DISCONNECTED);
			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
			app_state = 0;
			play_pause_count = 0;
			rem_mtu_size = DEFAULT_MTU_SIZE;

#if (RSI_APP_AVDTP_ROLE != ACCEPTOR_ROLE)
            LOG_PRINT("\n looking for device\n");
#if(RSI_BT_DUAL_PAIR_TEST == 1)
			if(dual_pair_flag == 0)
      {
			  status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr,RSI_BT_REMOTE_BD_ADDR));
      }
			else
      {
			  status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr,RSI_BT_REMOTE_BD_ADDR_2));
      }
#else
			status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr,RSI_BT_REMOTE_BD_ADDR));
#endif
			LOG_PRINT("\n reconnect status = %x \n", status);
			if (status != RSI_SUCCESS) {
				if (status == RSI_APP_ERR_LIMITED_RESOURCE) {
					LOG_PRINT("\n Disconn !!\n");
				} else if (status == RSI_APP_ERR_ACL_CONN_ALREADY_EXISTS) {
					app_state |= (1 << CONNECTED);
					app_state |= (1 << AUTHENTICATED);
					rsi_bt_app_set_event(RSI_APP_EVENT_CONNECTED);
					LOG_PRINT("\n Already conn \n");
				} else
					rsi_bt_app_set_event(RSI_APP_EVENT_DISCONNECTED);
			} else {
#if(RSI_BT_DUAL_PAIR_TEST == 1)
				dual_pair_flag = !dual_pair_flag;
#endif
			}

#endif
		}
			break;
		case RSI_APP_EVENT_CONFIRM_REQUEST: {
			//! confirm req event

			//! clear the ssp confirmation event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_CONFIRM_REQUEST);

			rsi_bt_accept_ssp_confirm((int8_t *) remote_dev_addr);
		}
			break;

		case RSI_APP_EVENT_A2DP_CONN: {
			//! a2dp connected event

			//! clear the a2dp connected event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_CONN);
		}
			break;

		case RSI_APP_EVENT_A2DP_DISCONN: {
			//! a2dp disconnected event

			//! clear the a2dp disconnected event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_DISCONN);
			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
		}
			break;

		case RSI_APP_EVENT_A2DP_CONFIGURE: {
			//! a2dp configured event

			//! clear the a2dp configured event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_CONFIGURE);
		}
			break;

		case RSI_APP_EVENT_A2DP_OPEN: {
			//! a2dp open event

			//! clear the a2dp open event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_OPEN);

			rsi_delay_ms(1000);
			status = rsi_bt_a2dp_start(remote_dev_addr);
			if (status != RSI_SUCCESS) {
				LOG_PRINT("\n a2dp start resp %x\n", status);
			}

		}
			break;
		case RSI_APP_EVENT_A2DP_START: {
			//! a2dp start event

			//! clear the a2dp start event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_START);

			if (!(app_state & (1 << AVRCP_CONNECTED))) {
#if (RSI_APP_AVDTP_ROLE != ACCEPTOR_ROLE)
				rsi_delay_ms(500);
				status = rsi_bt_avrcp_conn(remote_dev_addr);
				if (status != RSI_SUCCESS) {
					LOG_PRINT("\n avrcp conn resp status 0x%04x\r\n", status);
				}
#endif
			} else {
				rsi_delay_ms(500);
				rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
				LOG_PRINT("\n a2dp send audio\n");
			}
		}
			break;
		case RSI_APP_EVENT_A2DP_SUSPEND: {
			//! a2dp suspend event
			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_SUSPEND);
			rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
			LOG_PRINT("\n a2dp suspend\n");

		}
			break;

		case RSI_APP_EVENT_A2DP_MORE_DATA_REQ: {
			//! a2dp more data event
#ifdef RSI_WITH_OS
     // rsi_mutex_lock(&bt_event_mutex);
#endif
            if (!(app_state & (1 << CONNECTED)) &&
                !(app_state & (1 << A2DP_CONNECTED))){
              rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
              break;
            }
//			if ((play_pause_count % 2) == 1)
//				break;

#if A2DP_BURST_MODE
			aud_pkts_sent = 0;
			do {
#endif
				//! Send audio data
				status = send_audio_data(fp);

				if (status == RSI_APP_ERR_AUDIO_EOF) {
					//! close current and open next song
					if (app_song_list_idx == (SONG_LIST_MAX_COUNT - 1))
						app_song_list_idx = 0;
					else
						app_song_list_idx++;
#if (RSI_AUDIO_DATA_SRC == BIN_FILE) 
					close_audio_file(&fp);

					status = open_audio_file(&fp,
							app_song_list[app_song_list_idx]);
					if(status != RSI_SUCCESS) {
						return 0;
					}

#ifdef RMOVE_ID3_TAG
					rsi_mp3_remove_id3_tag(fp);
					LOG_PRINT("\n mp3 data without id3 tag\n");
#endif

#elif (RSI_AUDIO_DATA_SRC == SD_BIN_FILE)
					status = f_close(&g_fileObject_sd);

					status = open_audio_file_from_sd();
					if (status != RSI_SUCCESS) {
						return 0;
					}
#endif
				} else if (status)
					rsi_bt_app_clear_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
#if A2DP_BURST_MODE
				aud_pkts_sent++;
			} while (aud_pkts_sent < A2DP_BURST_SIZE);
#endif
#ifdef RSI_WITH_OS
			//rsi_mutex_unlock(&bt_event_mutex);
#endif
		}
			break;

		case RSI_APP_EVENT_AVRCP_CONN: {
			//! avrcp conn event

			//! clear the ssp receive event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_CONN);
			LOG_PRINT("\r\n RSI_APP_EVENT_AVRCP_CONN\r\n");

			if (!(app_state & (1 << A2DP_CONNECTED))) {
#if (RSI_APP_AVDTP_ROLE != ACCEPTOR_ROLE)
				rsi_delay_ms(500);
				status = rsi_bt_a2dp_connect(remote_dev_addr);
				if (status != RSI_SUCCESS) {
					if (status == RSI_APP_ERR_SOCK_DISCONN) {
						rsi_bt_app_set_event(RSI_APP_EVENT_DISCONNECTED);
						app_state = 0;
					}
					else if (status == RSI_APP_ERR_A2DP_CONN_ALRDY_EXISTS){}
					else
						rsi_bt_app_set_event(RSI_APP_EVENT_CONNECTED);
					LOG_PRINT("\n a2dp conn resp status 0x%04x\r\n", status);
				}
#endif

			} else if (app_state & (1 << A2DP_STREAM_OPEN)) {
				rsi_delay_ms(500);
				rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
				LOG_PRINT("\n a2dp send audio\n");
			} else
				LOG_PRINT("\n waiting for a2dp config\n");
		}
			break;

		case RSI_APP_EVENT_AVRCP_DISCONN: {
			//! avrcp disconn event

			//! clear the avrcp disconn event.
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_DISCONN);
		}
			break;

		case RSI_APP_EVENT_AVRCP_PAUSE: {
			notify_val_t notify = { 0 };
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_PAUSE);
			LOG_PRINT("\n avrcp pause\r\n");
			if ((app_reg_events != 0) && (app_event_id[0] == 1)) {
				notify.player_status = 0;
				rsi_bt_avrcp_notify(remote_dev_addr, 1, &notify);
				app_event_id[0] = 0;
				app_reg_events--;
			}
			rsi_bt_app_a2dp_pause();
		}
			break;
		case RSI_APP_EVENT_AVRCP_PLAY: {
			notify_val_t notify = { 0 };
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_PLAY);
			LOG_PRINT("\n avrcp play\r\n");
			if ((app_reg_events != 0) && (app_event_id[0] == 1)) {
				notify.player_status = 1;
				rsi_bt_avrcp_notify(remote_dev_addr, 1, &notify);
				app_event_id[0] = 0;
				app_reg_events--;
			}
			rsi_bt_app_a2dp_resume();
		}
			break;

		case RSI_APP_EVENT_AVRCP_NEXT:
		case RSI_APP_EVENT_AVRCP_PREVIOUS: {
			//! clear the avrcp next/prev receive event.
			if (temp_event_map == RSI_APP_EVENT_AVRCP_NEXT) {
				LOG_PRINT("\n avrcp next\r\n");
				rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_NEXT);
				if (app_song_list_idx == (SONG_LIST_MAX_COUNT - 1))
					app_song_list_idx = 0;
				else
					app_song_list_idx++;

			} else if (temp_event_map == RSI_APP_EVENT_AVRCP_PREVIOUS) {
				LOG_PRINT("\n avrcp previous\r\n");
				rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_PREVIOUS);
				if (app_song_list_idx == 0)
					app_song_list_idx = (SONG_LIST_MAX_COUNT - 1);
				else
					app_song_list_idx--;
			}

			//! close current song and open next song
#if (RSI_AUDIO_DATA_SRC == BIN_FILE) 
			close_audio_file(&fp);
			status = open_audio_file(&fp, app_song_list[app_song_list_idx]);

			if(status != RSI_SUCCESS) {
				return 0;
			}

#ifdef RMOVE_ID3_TAG
			rsi_mp3_remove_id3_tag(fp);
			LOG_PRINT("\n mp3 data withou id3 tag\n");
#endif
#else
			status = f_close(&g_fileObject_sd);
			status = open_audio_file_from_sd();
			if (status != RSI_SUCCESS) {
				return 0;
			}
#endif

		}
			break;

		case RSI_APP_EVENT_AVRCP_GET_CAP: {
			uint32_t cap;
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_GET_CAP);
			switch (app_cap_type) {
			case 2:
				cap = 0x1958;
				break;
			case 3:
				cap = 1;
				break;
			}
			rsi_bt_avrcp_cap_resp(remote_dev_addr, app_cap_type, 1, &cap);
		}
			break;

		case RSI_APP_EVENT_AVRCP_GET_ATTS: {
			uint8_t atts[5] = { 1 };

			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_GET_ATTS);
			rsi_bt_avrcp_att_list_resp(remote_dev_addr, 1, atts);
		}
			break;

		case RSI_APP_EVENT_AVRCP_GET_ATT_VALS: {
			uint8_t vals[5] = { 0, 1 };
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_GET_ATT_VALS);
			rsi_bt_avrcp_att_val_list_resp(remote_dev_addr, 2, vals);
		}
			break;

		case RSI_APP_EVENT_AVRCP_CUR_ATT_VALS: {
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_CUR_ATT_VALS);
		}
			break;

		case RSI_APP_EVENT_AVRCP_SET_ATT_VALS: {
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_SET_ATT_VALS);
			rsi_bt_avrcp_set_cur_att_val_resp(remote_dev_addr, 0);
		}
			break;

		case RSI_APP_EVENT_AVRCP_ATT_TEXT: {
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_ATT_TEXT);
		}
			break;

		case RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT: {
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT);
		}
			break;
#if 0
			case RSI_APP_EVENT_AVRCP_CHAR_SET:
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_CHAR_SET);
			rsi_bt_avrcp_char_set_status_resp(remote_dev_addr, 0);
			break;

			case RSI_APP_EVENT_AVRCP_BATT_STATUS:
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_BATT_STATUS);
			rsi_bt_avrcp_batt_status_resp(remote_dev_addr, 0);
			break;
#endif
		case RSI_APP_EVENT_AVRCP_SONG_ATTS: {
			att_text_t att_text[8] = { { 0, { 0 } } };

			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_SONG_ATTS);

			for (ix = 0; ix < app_song_atts.nbr_atts; ix++) {

				att_text[ix].id = (uint8_t) (app_song_atts.ele_atts[ix]);

				switch (app_song_atts.ele_atts[ix]) {
				case SONG_NAME:
#if (RSI_AUDIO_DATA_SRC == BIN_FILE)
					memcpy(&att_text[ix].att_text[0],
							(void *) AUDIO_FILE_PATH,
							strlen((int8_t *) AUDIO_FILE_PATH));
#endif
					break;
				case ARTIST_NAME:
					memcpy(att_text[ix].att_text, ARTIST_NAME_1,
							strlen(ARTIST_NAME_1));
					break;

				case ALBUM_NAME:
					memcpy(att_text[ix].att_text, ALBUM_NAME_LOCAL,
							strlen(ALBUM_NAME_LOCAL));
					break;

				case SONG_NUMBER:
					rsi_itoa(1, att_text[ix].att_text);
					break;

				case NUM_OF_SONGS:
					rsi_itoa(2, att_text[ix].att_text);
					break;

				case SONG_GENRE:
					memcpy(att_text[ix].att_text, GENRE_NAME_TELUGU,
							strlen(GENRE_NAME_TELUGU));
					break;

				case PLAY_TIME:
					rsi_itoa(((file_size / PCM_DATA_LEN_PER_MS) * 1000),
							(uint8_t *) att_text[ix].att_text);
					break;

				}
			}
			rsi_bt_avrcp_ele_att_resp(remote_dev_addr, app_song_atts.nbr_atts,
					att_text);
		}
			break;

		case RSI_APP_EVENT_AVRCP_PLAY_STATUS: {
			uint32_t pos, size;
			uint8_t play_status;

			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_PLAY_STATUS);
			pos = f_tell(&g_fileObject_sd);
			size = file_size;
			play_status = rsi_app_async_event_map
					& BIT(RSI_APP_EVENT_A2DP_CONFIGURE);
			rsi_bt_avrcp_play_status_resp(remote_dev_addr, play_status,
					((size / PCM_DATA_LEN_PER_MS) * 1000),
					((pos / PCM_DATA_LEN_PER_MS) * 1000));
		}
			break;

		case RSI_APP_EVENT_AVRCP_REG_EVENT: {
			rsi_bt_app_clear_event(RSI_APP_EVENT_AVRCP_REG_EVENT);
			rsi_bt_avrcp_reg_notify_resp(remote_dev_addr, 0);
		}
			break;
		}
	}
	return 0;
}
#endif
