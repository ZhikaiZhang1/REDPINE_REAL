/**
 * @file    rsi_bt_a2dp_source_avrcp.c
 * @version 0.1
 * @date    xx Apr 2018
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
 */

/**
 * Include files
 * */

//! BT include file to refer BT APIs
#include<rsi_bt_apis.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_bt_config.h>
#include<rsi_bt.h>
#include <stdio.h>

#ifdef RSI_M4_INTERFACE
#include <rsi_board.h>
#endif

//! Common include file 
#include<rsi_common_apis.h>

//! application defines 
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                      printf
#else 
#define LOG_PRINT
#endif
#define RSI_BT_LOCAL_NAME              "A2DP_AVRCP_SOURCE"
#define PIN_CODE                       "0000"

//#define RSI_BT_REMOTE_BD_ADDR        "00:1E:7C:25:E9:4D"      /* HS_Phillips */
//#define RSI_BT_REMOTE_BD_ADDR        "D0:8A:55:74:4E:4A"      /* HS_JIB_Wireless */
//#define RSI_BT_REMOTE_BD_ADDR        "00:1A:7D:DA:71:74"      /* PC */
//#define RSI_BT_REMOTE_BD_ADDR        "30:22:00:00:33:24"      /* iBall */
//#define RSI_BT_REMOTE_BD_ADDR        "B8:D5:0B:9B:D6:B2"      /* SRS-XB10 */
#define RSI_BT_REMOTE_BD_ADDR          "00:17:03:01:E1:28"      /* Portornics */

#define PCM_AUDIO                      1
#define SBC_AUDIO                      2
#define MP3_AUDIO                      3
#define RSI_AUDIO_DATA_TYPE            MP3_AUDIO

#define BIN_FILE                       1
#define ARRAY                          2
#define RSI_AUDIO_DATA_SRC             BIN_FILE

#define A2DP_BURST_MODE                0  // 0 - Disable, 1 - Enable

#if (RSI_AUDIO_DATA_SRC == BIN_FILE)
#define ELEMENT_READ_SIZE              1

#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
#define AUDIO_FILE_PATH                  "sbc_samples.sbc"
#define RSI_SBC_DATA_SIZE               297
uint16_t pkt_size;
uint8_t  sbc_data[RSI_SBC_DATA_SIZE];
typedef struct rtp_header_s {
	uint8_t csrc_count:4;
	uint8_t header_ex:1;
	uint8_t padding:1;
	uint8_t version:2;
	uint8_t patload_type:7;
	uint8_t marker:1;
	uint8_t seq_num[2];
	uint32_t time_stamp;
	uint32_t sync_source;
}rtp_header_t;

uint32_t pkt_count = 0;
rtp_header_t rtp_header;
#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
#define AUDIO_FILE_PATH                  "pcm_samples.wav"
#define RSI_PCM_DATA_SIZE               512
uint16_t pkt_size;
uint8_t pcm_data[RSI_PCM_DATA_SIZE];

#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
#define AUDIO_FILE_PATH                  "mp3_samples.mp3"
#define RSI_MP3_DATA_SIZE               512
#define RMOVE_ID3_TAG
uint16_t pkt_size;
uint8_t mp3_data[RSI_MP3_DATA_SIZE];
//! TAG is always present at the beggining of a ID3V2 MP3 file
//! Constant size 10 bytes
typedef struct
{
  uint8_t id[3];       //"ID3"
  uint8_t version[2];  // $04 00
  uint8_t flags;       // %abcd0000
  uint32_t size;        //4 * %0xxxxxxx
}__attribute__((__packed__))
ID3TAG;
#endif

#elif (RSI_AUDIO_DATA_SRC == ARRAY)

#if (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
#include "pcm_data_buff.h"
#define RSI_PCM_DATA_SIZE               512
#endif

#endif

//! application events list
#define RSI_APP_EVENT_CONNECTED        1
#define RSI_APP_EVENT_PINCODE_REQ      2
#define RSI_APP_EVENT_LINKKEY_SAVE     3
#define RSI_APP_EVENT_AUTH_COMPLT      4
#define RSI_APP_EVENT_DISCONNECTED     5
#define RSI_APP_EVENT_LINKKEY_REQ      6
#define RSI_APP_EVENT_UNBOND_STATUS    7

//! a2dp related defines
#define RSI_APP_EVENT_A2DP_CONFIGURE   9
#define RSI_APP_EVENT_A2DP_OPEN        10
#define RSI_APP_EVENT_A2DP_CONN        11
#define RSI_APP_EVENT_A2DP_DISCONN     12

#define RSI_APP_EVENT_AVRCP_CONN      13
#define RSI_APP_EVENT_AVRCP_DISCONN   14
#define RSI_APP_EVENT_AVRCP_PLAY      15
#define RSI_APP_EVENT_AVRCP_PAUSE     16

#define RSI_APP_EVENT_AVRCP_GET_CAP        17
#define RSI_APP_EVENT_AVRCP_GET_ATTS       18
#define RSI_APP_EVENT_AVRCP_GET_ATT_VALS   19
#define RSI_APP_EVENT_AVRCP_CUR_ATT_VALS   20
#define RSI_APP_EVENT_AVRCP_SET_ATT_VALS   21
#define RSI_APP_EVENT_AVRCP_ATT_TEXT       22
#define RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT  23
#define RSI_APP_EVENT_AVRCP_CHAR_SET       24
#define RSI_APP_EVENT_AVRCP_BATT_STATUS    25
#define RSI_APP_EVENT_AVRCP_SONG_ATTS      26
#define RSI_APP_EVENT_AVRCP_PLAY_STATUS    27
#define RSI_APP_EVENT_AVRCP_REG_EVENT      28

#define RSI_APP_EVENT_A2DP_MORE_DATA_REQ    30



//! Error codes
#define RSI_APP_ERR_NONE                    0x0000
#define RSI_APP_ERR_SOCK_DISCONN            0x4062
#define RSI_APP_ERR_SOCK_TIMEOUT            0x4063
#define RSI_APP_ERR_SDP_SRV_NOT_FOUND       0x4102
#define RSI_APP_ERR_PAGE_TIMEOUT            0x4E04
#define RSI_APP_ERR_PIN_MISSING             0x4E06
#define RSI_APP_ERR_CONN_TIMEOUT            0x4E08
#define RSI_APP_ERR_ACL_CONN_ALREADY_EXISTS 0x4E0B
#define RSI_APP_ERR_LIMITED_RESOURCE        0x4E0D
#define RSI_APP_ERR_REMOTE_TERMINATED_CONN  0x4E13
#define RSI_APP_ERR_LOCAL_TERMINATED_CONN   0x4E16
#define RSI_APP_ERR_REPEATED_ATTEMPTS       0x4E17

#define RSI_APP_ERR_AUDIO_EOF               0x00A0

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN             10000

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE         3000

//! BT task priority
#define RSI_BT_TASK_PRIORITY           1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS              1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY       2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE     3000

void rsi_wireless_driver_task(void);

#endif

#define BD_ADDR_ARRAY_LEN               18

//! Memory to initialize driver
uint8_t bt_global_buf[BT_GLOBAL_BUFF_LEN] = {0};

//! Application global parameters.
static uint32_t rsi_app_async_event_map = 0;
static rsi_bt_resp_get_local_name_t local_name = {0};
static uint8_t str_conn_bd_addr[BD_ADDR_ARRAY_LEN];
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};

static uint8_t passkey_data[6];
static uint8_t conn_resp_status;
uint8_t play_count;

//! enum for global state
typedef enum {
  INQUIRY,
  CONNECTED,
  AUTHENTICATED,
} app_state_t;

app_state_t app_state = 0;
#define RSI_LINKKEY_REPLY_SIZE                      16 
static uint8_t  linkkey[RSI_LINKKEY_REPLY_SIZE];

#define SONG_NAME      0x01
#define ARTIST_NAME    0x02
#define ALBUM_NAME     0x03
#define SONG_NUMBER    0x04
#define NUM_OF_SONGS   0x05
#define SONG_GENRE     0x06
#define PLAY_TIME      0x07
#define ARTIST_NAME_MALLI    "Mallikarjuna"
#define ALBUM_NAME_LOCAL     "Local"
#define GENRE_NAME_TELUGU     "Telugu"
#define PCM_DATA_LEN_PER_MS    (44100 * 2 * 2)
/*==============================================*/
/**
 * @fn         rsi_bt_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BT initialization.
 */
static void rsi_bt_app_init_events()
{
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
static void rsi_bt_app_set_event(uint32_t event_num)
{
  rsi_app_async_event_map |= BIT(event_num);
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
static void rsi_bt_app_clear_event(uint32_t event_num)
{
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
static int32_t rsi_bt_app_get_event(void)
{
  uint32_t  ix;

  for (ix = 0; ix < 32; ix++)
  {
    if (rsi_app_async_event_map & (1 << ix))
    {
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

void rsi_bt_app_on_conn (uint16_t resp_status, rsi_bt_event_bond_t *conn_event)
{
  if(resp_status == 0)
  {
    app_state |= (1 << CONNECTED);
    rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
  }
  else if(resp_status == RSI_APP_ERR_PIN_MISSING)
  {
  
  }

  rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, conn_event->dev_addr);
  LOG_PRINT ("on_conn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_pincode_req(uint16_t resp_status, rsi_bt_event_user_pincode_request_t *user_pincode_request)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_PINCODE_REQ);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_pincode_request->dev_addr);
  LOG_PRINT ("on_pin_coe_req: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_on_linkkey_req
 * @brief      invoked when linkkey request event is received
 * @param[out] user_linkkey_req, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey request from remote device
 */
void rsi_ble_app_on_linkkey_req (uint16_t status, rsi_bt_event_user_linkkey_request_t  *user_linkkey_req)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_REQ);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_req->dev_addr);
  LOG_PRINT ("linkkey_req: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_on_linkkey_save
 * @brief      invoked when linkkey save event is received
 * @param[out] user_linkkey_req, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey save from local device
 */
void rsi_ble_app_on_linkkey_save (uint16_t status, rsi_bt_event_user_linkkey_save_t *user_linkkey_save)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_SAVE);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_save->dev_addr);
  memcpy (linkkey, user_linkkey_save->linkKey, 16);
  LOG_PRINT ("linkkey_save: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_auth_complete (uint16_t resp_status, rsi_bt_event_auth_complete_t *auth_complete)
{
  if(resp_status == 0)
  {
    rsi_bt_app_set_event (RSI_APP_EVENT_AUTH_COMPLT);
    app_state |= (1 << AUTHENTICATED);
  }
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, auth_complete->dev_addr);
  LOG_PRINT ("auth_complete: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_unbond_status (uint16_t resp_status, rsi_bt_event_unbond_t *unbond_status)
{

  rsi_bt_app_set_event (RSI_APP_EVENT_UNBOND_STATUS);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, unbond_status->dev_addr);
  LOG_PRINT ("\r\non_unbond_status: str_conn_bd_addr: %s, resp_status 0x%04x\r\n", str_conn_bd_addr, resp_status);
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
void rsi_bt_app_on_disconn (uint16_t resp_status, rsi_bt_event_disconnect_t *bt_disconnected)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, bt_disconnected->dev_addr);
  LOG_PRINT ("on_disconn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_a2dp_connect (uint16_t resp_status, rsi_bt_event_a2dp_connect_t *a2dp_connect)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_CONN);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_connect->dev_addr);
  LOG_PRINT ("a2dp_conn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_a2dp_disconnect (uint16_t resp_status, rsi_bt_event_a2dp_disconnect_t *a2dp_disconnect)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_DISCONN);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_disconnect->dev_addr);
  LOG_PRINT ("a2dp_disconn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_a2dp_configure (uint16_t resp_status, rsi_bt_event_a2dp_configure_t *a2dp_configure)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_CONFIGURE);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_configure->dev_addr);
  //LOG_PRINT ("a2dp_configure: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_on_a2dp_more_data_req (uint16_t resp_status, rsi_bt_event_a2dp_more_data_req_t *a2dp_more_data_req)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_more_data_req->dev_addr);
  //LOG_PRINT ("a2dp_more_data_req: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_a2dp_open (uint16_t resp_status, rsi_bt_event_a2dp_open_t *a2dp_oen)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_OPEN);
  rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_oen->dev_addr);
  LOG_PRINT ("a2dp_open: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_avrcp_connect (uint16_t resp_status, rsi_bt_event_avrcp_connect_t *avrcp_connect)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_CONN);
  rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_connect->dev_addr);
  LOG_PRINT ("AVRCP_conn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_avrcp_disconnect (uint16_t resp_status, rsi_bt_event_avrcp_disconnect_t *avrcp_disconn)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_DISCONN);
  rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_disconn->dev_addr);
  LOG_PRINT ("AVRCP_disconn: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_avrcp_play(uint16_t resp_status, rsi_bt_event_avrcp_play_t *avrcp_play)
{

  play_count ++;
  if (play_count % 2 == 0) {
    rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_PLAY);
    play_count = 0;
  }
  rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_play->dev_addr);
  LOG_PRINT ("AVRCP_play: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_avrcp_pause (uint16_t resp_status, rsi_bt_event_avrcp_pause_t *avrcp_pause)
{
  play_count ++;
  if(play_count % 2 == 0){
    rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_PAUSE);
    play_count = 0;
  }
  rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_pause->dev_addr);
  LOG_PRINT ("AVRCP_pause: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
rsi_bt_event_avrcp_get_cur_att_val_t  app_att_list;
rsi_bt_event_avrcp_get_cur_att_val_t app_att_text_list;
rsi_bt_event_avrcp_vals_text_req_t app_vals_list;

void rsi_bt_on_avrcp_get_cap_event (uint8_t  *bd_addr, uint8_t cap_type)
{
  app_cap_type = cap_type;
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_GET_CAP);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_GET_CAP: cap_type: %d\r\n", cap_type);
  return;
}

void rsi_bt_on_avrcp_get_app_supp_atts_event (uint8_t  *bd_addr)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_GET_ATTS);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_GET_ATTS\r\n");
  return;
}

void rsi_bt_on_avrcp_get_app_suup_att_vals_event (uint8_t  *bd_addr, uint8_t att_id)
{
  app_att_id = att_id;
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_GET_ATT_VALS);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_GET_ATT_VALS: att_id: %d\r\n", att_id);
  return;
}

void rsi_bt_on_avrcp_get_app_cur_att_val_event (rsi_bt_event_avrcp_get_cur_att_val_t *p_att_list)
{
  uint8_t ix;

  memcpy (&app_att_list, p_att_list, sizeof (rsi_bt_event_avrcp_get_cur_att_val_t));
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_CUR_ATT_VALS);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_CUR_ATT_VALS: nbr_atts: %d, atts: ", p_att_list->nbr_atts);
  for (ix = 0; ix < p_att_list->nbr_atts; ix++) {
    LOG_PRINT ("%d, ", p_att_list->atts);
  }
  LOG_PRINT ("\r\n");

  return;
}

void rsi_bt_on_avrcp_set_app_cur_att_val_event (rsi_bt_event_avrcp_set_att_val_t *p_att_list)
{
  uint8_t ix;

  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_SET_ATT_VALS);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_CUR_ATT_VALS: nbr_atts:%d\r\n", p_att_list->nbr_atts);
  for (ix = 0; ix < p_att_list->nbr_atts; ix++) {
    LOG_PRINT ("attid: %d - attvalue: %d \r\n", p_att_list->att_list[ix].attid, p_att_list->att_list[ix].attvalue);
  }
  LOG_PRINT ("\r\n");
  return;
}

void rsi_bt_on_avrcp_get_app_att_text_event (rsi_bt_event_avrcp_get_cur_att_val_t *p_att_text_list)
{
  uint8_t ix;

  memcpy (&app_att_text_list, p_att_text_list, sizeof (rsi_bt_event_avrcp_get_cur_att_val_t));
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_ATT_TEXT);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_ATT_TEXT: nbr_atts: %d, atts: ", p_att_text_list->nbr_atts);
  for (ix = 0; ix < p_att_text_list->nbr_atts ; ix++) {
    LOG_PRINT ("%d, ", p_att_text_list->atts[ix]);
  }
  LOG_PRINT ("\r\n");
  return;
}

void rsi_bt_on_avrcp_get_app_att_vals_text_event (rsi_bt_event_avrcp_vals_text_req_t *p_vals_list)
{
  uint8_t ix;

  memcpy (&app_vals_list, p_vals_list, sizeof (rsi_bt_event_avrcp_vals_text_req_t));
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT: att_id: %d, nbr_vals:%d, vals:", p_vals_list->att_id, p_vals_list->nbr_vals);
  for (ix = 0; ix < p_vals_list->nbr_vals; ix++) {
    LOG_PRINT ("%d, ", p_vals_list->vals);
  }
  LOG_PRINT ("\r\n");
  return;
}

void rsi_bt_on_avrcp_supp_char_sets_event (rsi_bt_event_avrcp_char_sets_t *p_char_sets)
{
  uint8_t ix;

  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_CHAR_SET);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_CHAR_SET: nbr_sets:%d, sets: ", p_char_sets->nbr_sets);
  for (ix = 0; ix < p_char_sets->nbr_sets; ix++) {
    LOG_PRINT ("%d, ", p_char_sets->char_sets[ix]);
  }
  LOG_PRINT ("\r\n");
  return;
}

void rsi_bt_on_avrcp_batt_status_event (uint8_t  *bd_addr, uint8_t batt_status)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_BATT_STATUS);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_BATT_STATUS: batt_status: %d\r\n", batt_status);
  return;
}

void rsi_bt_on_avrcp_get_song_atts_event (rsi_bt_event_avrcp_ele_att_t *p_song_atts)
{
  uint8_t ix;

  memcpy (&app_song_atts, p_song_atts, sizeof (rsi_bt_event_avrcp_ele_att_t));
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_SONG_ATTS);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_SONG_ATTS: nbr_atts: %d, att_list:", p_song_atts->nbr_atts);
  for (ix = 0; ix < p_song_atts->nbr_atts; ix++) {
    LOG_PRINT ("%d, ",p_song_atts->ele_atts[ix]);
  }
  LOG_PRINT ("\r\n");

  return;
}
void rsi_bt_on_avrcp_get_play_status_event (uint8_t  *bd_addr)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_PLAY_STATUS);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_PLAY_STATUS: \r\n");
  return;
}

uint8_t app_event_id[5];
uint8_t app_reg_events;
void rsi_bt_on_avrcp_reg_notify_event (uint8_t  *bd_addr, uint8_t event_id)
{
  app_event_id[app_reg_events++] = event_id;
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_REG_EVENT);
  LOG_PRINT ("RSI_APP_EVENT_AVRCP_REG_EVENT: event_id: %d\r\n", event_id);
  return;
}

#if (RSI_AUDIO_DATA_SRC == BIN_FILE) 
int16_t open_audio_file(FILE **fp, uint8_t *file_name)
{
  *fp = fopen (file_name, "rb");
  if (*fp == NULL) {
    LOG_PRINT (" file open fail\r\n");
    return 1;
  }
  LOG_PRINT (" file opend fp: 0x%x\r\n", *fp);
  fseek(*fp, 0, SEEK_SET);
  
  return 0;
}

int16_t close_audio_file(FILE **fp)
{
  if (*fp == NULL) {
    LOG_PRINT (" fp invalid\r\n");
    return 1;
  }

  fclose(*fp);
  LOG_PRINT (" file closed fp: 0x%x\r\n", *fp);
  
  return 0;
}


#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)

void rsi_bt_prepare_rtp_header (rtp_header_t  *rtp_header, uint32_t  pkt_count)
{
	memset (rtp_header, 0, sizeof (rtp_header_t));
	
	rtp_header->version = 2;
	rtp_header->patload_type = 0x60;
	rtp_header->seq_num[0] = ((pkt_count >> 8)& (0xFF));
	rtp_header->seq_num[1] = (pkt_count & 0xFF);
	//rtp_header->time_stamp = pkt_count * 5000;
	
	return;
}
int16_t read_sbc_data(FILE *fp)
{
  uint8_t  p_sbc_data[RSI_SBC_DATA_SIZE];
  uint8_t sbc_frames;
  
  pkt_size = 0;
  sbc_frames = 0;
  
  while(pkt_size < 200){
    //! read file 1 byte
    pkt_size += fread (&p_sbc_data[pkt_size], ELEMENT_READ_SIZE, 1, fp);
    //! check SW == 0x9c
    if(p_sbc_data[pkt_size-1] == 0x9c){
      //! read file <size-1>
      pkt_size += fread (&p_sbc_data[pkt_size], ELEMENT_READ_SIZE, (82), fp);
    }
    else{
      //! error ...
      return RSI_APP_ERR_AUDIO_EOF;
    }
    sbc_frames++;
  }
  pkt_count++;
  rsi_bt_prepare_rtp_header (&rtp_header, pkt_count);
  
	memset (sbc_data, 0, sizeof (sbc_data));
  memcpy (sbc_data, &rtp_header, sizeof (rtp_header_t));
  sbc_data[sizeof (rtp_header_t)] = sbc_frames;
  memcpy (&sbc_data[13], p_sbc_data, sizeof (p_sbc_data));
  pkt_size += 13;

  return 0;
}

#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
int16_t read_pcm_data(FILE *fp)
{
  pkt_size = fread (pcm_data, ELEMENT_READ_SIZE, sizeof (pcm_data), fp);
  //LOG_PRINT ("\r\npcm pkt len read: %d\r\n", pkt_size);
  if (pkt_size == 0) {
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
  LOG_PRINT("tag_size = %x\n", tag_size);
  return tag_size;   
}

void rsi_mp3_remove_id3_tag(FILE *fp)
{
  unsigned int id3_size,offset;
  ID3TAG mp3_tag;

  rewind(fp);
  id3_size = mp3Header(fp, &mp3_tag);
  LOG_PRINT("id3_size = %x\n", id3_size);

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

#endif

int16_t send_audio_data(FILE *fp)
{
  int16_t err = 0;

#if (RSI_AUDIO_DATA_SRC == BIN_FILE)

#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
  err = read_sbc_data(fp);
  if(err)
    return err;
  rsi_bt_a2dp_send_sbc_aac_data (str_conn_bd_addr, sbc_data, pkt_size, 0);
#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
  err = read_pcm_data(fp);

  if(err)
    return err;
#if (A2DP_BURST_MODE == 1)
  if (pkt_size >= 512)
#endif
    err = rsi_bt_a2dp_send_pcm_mp3_data (str_conn_bd_addr, pcm_data, pkt_size, RSI_AUDIO_DATA_TYPE);

#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
  err = read_mp3_data(fp);
  if(err)
    return err;
  err = rsi_bt_a2dp_send_pcm_mp3_data (str_conn_bd_addr, mp3_data, pkt_size, RSI_AUDIO_DATA_TYPE);

#endif

#elif (RSI_AUDIO_DATA_SRC == ARRAY)

#if (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
  static int16_t ix = 0;

  err = rsi_bt_a2dp_send_pcm_mp3_data (str_conn_bd_addr, pcm_data_buff + (ix * RSI_PCM_DATA_SIZE), RSI_PCM_DATA_SIZE, RSI_AUDIO_DATA_TYPE);
  ix = (ix>587) ? 0 : (ix + 1); 

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

/*==============================================*/
/**
 * @fn         rsi_bt_a2dp_source
 * @brief      Tests the BT Classic A2DP Source.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the A2DP Source.
 */
int32_t rsi_bt_a2dp_source (void)
{
  int32_t status = 0;
  uint32_t file_size = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[BD_ADDR_ARRAY_LEN] = {0};
  uint8_t eir_data[64] = {2,1,0};
  uint8_t local_play_count = 0;
  uint8_t ix;
  FILE  *fp = NULL;
  
#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(bt_global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! module intialisation 
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BT_CLASSIC);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#if (RSI_AUDIO_DATA_SRC == BIN_FILE) 
  status = open_audio_file(&fp, AUDIO_FILE_PATH);
  if(status)
    return 0;

#ifdef RMOVE_ID3_TAG
  rsi_mp3_remove_id3_tag(fp);
  LOG_PRINT("mp3 data withou id3 tag\n");
#endif

#else

#if (RSI_AUDIO_DATA_TYPE != PCM_AUDIO)
  LOG_PRINT("audio type combination not supported\n");
  return 0;
#endif

#endif

#ifdef RSI_UART_FLOW_CTRL_ENABLE
  rsi_cmd_uart_flow_ctrl (RSI_ENABLE); 
#endif

  //! BT register GAP callbacks:
  rsi_bt_gap_register_callbacks(
      NULL, //role_change
      rsi_bt_app_on_conn, 
      rsi_bt_app_on_unbond_status,
      rsi_bt_app_on_disconn,
      NULL,//scan_resp
      NULL,//remote_name_req
      NULL, //rsi_bt_on_passkey_display,//passkey_display
      NULL,//remote_name_req+cancel
      NULL, //rsi_bt_on_confirm_request,//confirm req
      rsi_bt_app_on_pincode_req,
      NULL, //rsi_bt_on_passkey_request,//passkey request
      NULL,//inquiry complete
      rsi_bt_app_on_auth_complete,
      rsi_ble_app_on_linkkey_req,//linkkey request
      NULL, //rsi_bt_on_ssp_complete,//ssp coplete
      rsi_ble_app_on_linkkey_save,
      NULL, //get services
      NULL,
      NULL,
      NULL); //search service
 
  //! initialize the event map
  rsi_bt_app_init_events ();

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(local_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  rsi_6byte_dev_address_to_ascii ((int8_t *)str_bd_addr, local_dev_addr);
  LOG_PRINT ("\r\nlocal_bd_address: %s\r\n", str_bd_addr);

  //! set the local device name
  status = rsi_bt_set_local_name(RSI_BT_LOCAL_NAME);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! get the local device name
  status = rsi_bt_get_local_name(&local_name);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\r\nlocal_name: %s\r\n", local_name.name);
  
  //! prepare Extended Response Data 
  eir_data[3] = strlen (RSI_BT_LOCAL_NAME) + 1;
  eir_data[4] = 9;
  strcpy ((char *)&eir_data[5], RSI_BT_LOCAL_NAME);
  //! set eir data
  rsi_bt_set_eir_data (eir_data, strlen (RSI_BT_LOCAL_NAME) + 5);
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

  //! initilize the A2DP profile
  status = rsi_bt_a2dp_init();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#if (A2DP_BURST_MODE == 1)
  status = rsi_bt_a2dp_burst_mode (RSI_BT_REMOTE_BD_ADDR, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif
  //! register the A2DP profile callback's
  rsi_bt_a2dp_register_callbacks (
      rsi_bt_app_on_a2dp_connect,
      rsi_bt_app_on_a2dp_disconnect,
      rsi_bt_app_on_a2dp_configure,
      rsi_bt_app_on_a2dp_open,
      NULL, //rsi_bt_app_on_a2dp_start,
      NULL, //rsi_bt_app_on_a2dp_suspend,
      NULL, //rsi_bt_app_on_a2dp_abort,
      NULL, //rsi_bt_app_on_a2dp_close,
      NULL, //rsi_bt_on_a2dp_encode_data,
      NULL, //rsi_bt_on_a2dp_pcm_data
      rsi_bt_on_a2dp_more_data_req);

  //! initilize the AVRCP profile
  status = rsi_bt_avrcp_init ();
  if(status != RSI_SUCCESS)
  {
    return status;
  }


  rsi_bt_avrcp_register_callbacks (
      rsi_bt_app_on_avrcp_connect,
      rsi_bt_app_on_avrcp_disconnect,
      rsi_bt_app_on_avrcp_play,
      rsi_bt_app_on_avrcp_pause,
      NULL, //rsi_bt_app_on_avrcp_next,
      NULL, //rsi_bt_app_on_avrcp_previous,
      NULL);

  rsi_bt_avrcp_target_register_callbacks(
      rsi_bt_on_avrcp_get_cap_event,
      rsi_bt_on_avrcp_get_app_supp_atts_event,
      rsi_bt_on_avrcp_get_app_suup_att_vals_event,
      rsi_bt_on_avrcp_get_app_cur_att_val_event,
      rsi_bt_on_avrcp_set_app_cur_att_val_event,
      rsi_bt_on_avrcp_get_app_att_text_event,
      rsi_bt_on_avrcp_get_app_att_vals_text_event,
      rsi_bt_on_avrcp_supp_char_sets_event,
      rsi_bt_on_avrcp_batt_status_event,
      rsi_bt_on_avrcp_get_song_atts_event,
      rsi_bt_on_avrcp_get_play_status_event,
      rsi_bt_on_avrcp_reg_notify_event);

reconn:
  status = rsi_bt_connect (RSI_BT_REMOTE_BD_ADDR);
  if (status != 0) {
    goto reconn;
  }

  while(1)
  {
    //! Application main loop

#ifndef RSI_WITH_OS
    //! run the non OS scheduler
    rsi_wireless_driver_task();
#endif
    //! checking for received events
    temp_event_map = rsi_bt_app_get_event ();
    if (temp_event_map == RSI_FAILURE) {
      //! if events are not received loop will be continued.
      continue;
    }

    //! if any event is received, it will be served.
    switch(temp_event_map)
    {
      case RSI_APP_EVENT_CONNECTED:
        {
          //! remote device connected event

          //! clear the connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_CONNECTED);
          if ((app_state & (1 << CONNECTED))) 
              //&& (app_state & (1 << AUTHENTICATED)))
            status = rsi_bt_a2dp_connect (str_conn_bd_addr);
        }
        break;

      case RSI_APP_EVENT_LINKKEY_REQ:
        {
          //! linkkey request event

          //! clear the linkkey request event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_REQ);

          if (linkkey[0] == 0) 
          {
            LOG_PRINT("linkkey request negative reply\r\n");
            //! sending the linkkey request negative reply
            rsi_bt_linkkey_request_reply ((int8_t *)str_conn_bd_addr, NULL, 0);
          } 
          else
          {
            LOG_PRINT("linkkey request positive reply\r\n");
            //! sending the linkkey request positive reply
            rsi_bt_linkkey_request_reply ((int8_t *)str_conn_bd_addr, linkkey, 1);
          }
        }
        break;

      case RSI_APP_EVENT_PINCODE_REQ:
        {
          //! pincode request event
          uint8_t *pin_code = (uint8_t *)PIN_CODE;

          //! clear the pincode request event.
          rsi_bt_app_clear_event(RSI_APP_EVENT_PINCODE_REQ);

          //! sending the pincode requet reply
          status = rsi_bt_pincode_request_reply((int8_t *)str_conn_bd_addr, pin_code, 1);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
        }
        break;

      case RSI_APP_EVENT_LINKKEY_SAVE:
        {
          //! linkkey save event

          //! clear the likkey save event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_SAVE);
        }
        break;
      case RSI_APP_EVENT_AUTH_COMPLT:
        {
          //! authentication complete event

          //! clear the authentication complete event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AUTH_COMPLT);
          
          //if ((app_state & (1 << CONNECTED)) && (app_state & (1 << AUTHENTICATED)))
            //status = rsi_bt_a2dp_connect (str_conn_bd_addr);
        }
        break;

      case RSI_APP_EVENT_UNBOND_STATUS:
        {
          //! remote device connected event

          //! clear the disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_UNBOND_STATUS);
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONFIGURE);
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_OPEN);
          app_state = 0; 
        }
        break;

      case RSI_APP_EVENT_DISCONNECTED:
        {
          //! remote device connected event

          //! clear the disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_DISCONNECTED);
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONFIGURE);
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_OPEN);
          app_state = 0; 
/*
reconn1:
          status = rsi_bt_connect (RSI_BT_REMOTE_BD_ADDR);
          //status = rsi_bt_a2dp_connect (RSI_BT_REMOTE_BD_ADDR);
          if (status != 0) {
            goto reconn1;
          }*/
        }
        break;

      case RSI_APP_EVENT_A2DP_CONN:
        {
          //! a2dp connected event

          //! clear the a2dp connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONN);
          rsi_bt_avrcp_conn (str_conn_bd_addr);
        }
        break;

      case RSI_APP_EVENT_A2DP_DISCONN:
        {
          //! a2dp disconnected event

          //! clear the a2dp disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_DISCONN);
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONFIGURE);
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_OPEN);
        }
        break;

      case RSI_APP_EVENT_A2DP_CONFIGURE:
        {
          //! a2dp configured event

          //! clear the a2dp configured event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONFIGURE);
          
          //! setting the data_req event
          rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);


        }
        break;

      case RSI_APP_EVENT_A2DP_OPEN:
        {
          //! a2dp open event

          //! clear the a2dp open event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_OPEN);
          
          //! setting the data_req event
          rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);

        }
        break;
		
		case RSI_APP_EVENT_A2DP_MORE_DATA_REQ:
        {
		
		if ((local_play_count % 2) == 1) 
            break;
			
			//! Send audio data
          status = send_audio_data(fp);
          if(status == RSI_APP_ERR_AUDIO_EOF)
          {
            //! close current and open next song
#if (RSI_AUDIO_DATA_SRC == BIN_FILE) 
            close_audio_file(&fp);
            status = open_audio_file(&fp, AUDIO_FILE_PATH);
            if(status)
              return 0;

#ifdef RMOVE_ID3_TAG
            rsi_mp3_remove_id3_tag(fp);
            LOG_PRINT("mp3 data withou id3 tag\n");
#endif
#endif
          }
          else if(status)
            rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
			
			
        }
        break;
     
    case RSI_APP_EVENT_AVRCP_CONN:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_CONN);
          LOG_PRINT("\r\n RSI_APP_EVENT_AVRCP_CONN\r\n");
        }
        break;


     case RSI_APP_EVENT_AVRCP_DISCONN:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_DISCONN);
          //rsi_bt_avrcp_conn (str_conn_bd_addr);
          LOG_PRINT("\r\n RSI_APP_EVENT_AVRCP_DISCONN\r\n");

        }
        break;

     case RSI_APP_EVENT_AVRCP_PAUSE:
     case RSI_APP_EVENT_AVRCP_PLAY:
        {
          notify_val_t  notify = {0};

          //! clear the ssp receive event.
          if(temp_event_map == RSI_APP_EVENT_AVRCP_PLAY)
          {
            rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_PLAY);
          }
          else if(temp_event_map == RSI_APP_EVENT_AVRCP_PAUSE)
          {
            rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_PAUSE);
          }
          local_play_count++;
          if ((local_play_count % 2) == 1) {
            //rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONFIGURE);
            //rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_OPEN);
            LOG_PRINT("\r\nRSI_APP_EVENT_AVRCP_PAUSE\r\n");
            if ((app_reg_events != 0) && (app_event_id[0] == 1)) {
              notify.player_status = 0;
              rsi_bt_avrcp_notify (str_conn_bd_addr, 1, &notify);
              app_event_id[0] = 0;
              app_reg_events--;
            }
          } else {
            //rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_CONFIGURE);
            LOG_PRINT("\r\nRSI_APP_EVENT_AVRCP_PLAY\r\n");
            if ((app_reg_events != 0) && (app_event_id[0] == 1)) {
              notify.player_status = 1;
              rsi_bt_avrcp_notify (str_conn_bd_addr, 1, &notify);
              app_event_id[0] = 0;
              app_reg_events--;
            }
          }

            // rsi_bt_avrcp_pause();
            //rsi_bt_avrcp_next();
            //usleep(100000);
            // status = rsi_bt_avrcp_play();
        }
        break;
#if 0
      case RSI_APP_EVENT_AVRCP_NEXT:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_NEXT);
        }
        break;

      case RSI_APP_EVENT_AVRCP_PREVIOUS:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_PREVIOUS);
        }
        break;
#endif
      case RSI_APP_EVENT_AVRCP_GET_CAP:
        {
          uint32_t cap;
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_GET_CAP);
          switch (app_cap_type) {
            case 2:
              cap = 0x1958;
              break;
            case 3:
              cap = 1;
              break;
          }
          rsi_bt_avrcp_cap_resp (str_conn_bd_addr, app_cap_type, 1, &cap);
        }
        break;

      case RSI_APP_EVENT_AVRCP_GET_ATTS:
        {
          uint8_t atts[5] = {1};

          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_GET_ATTS);
          rsi_bt_avrcp_att_list_resp (str_conn_bd_addr, 1, atts);
        }
        break;

      case RSI_APP_EVENT_AVRCP_GET_ATT_VALS:
        {
          uint8_t vals[5] = {0,1};
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_GET_ATT_VALS);
          rsi_bt_avrcp_att_val_list_resp (str_conn_bd_addr, 2, vals);
        }
        break;

      case RSI_APP_EVENT_AVRCP_CUR_ATT_VALS:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_CUR_ATT_VALS);
        }
        break;

      case RSI_APP_EVENT_AVRCP_SET_ATT_VALS:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_SET_ATT_VALS);
          rsi_bt_avrcp_set_cur_att_val_resp ((int8_t *)str_conn_bd_addr, 0);
        }
        break;

      case RSI_APP_EVENT_AVRCP_ATT_TEXT:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_ATT_TEXT);
        }
        break;

      case RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT);
        }
        break;
        
      case RSI_APP_EVENT_AVRCP_CHAR_SET:
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_CHAR_SET);
          rsi_bt_avrcp_char_set_status_resp ((int8_t *)str_conn_bd_addr, 0);
        break;

      case RSI_APP_EVENT_AVRCP_BATT_STATUS:
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_BATT_STATUS);
          rsi_bt_avrcp_batt_status_resp ((int8_t *)str_conn_bd_addr, 0);
        break;

      case RSI_APP_EVENT_AVRCP_SONG_ATTS:
        {
          att_text_t  att_text[8] = {0, {0}};

          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_SONG_ATTS);

          for (ix = 0; ix < app_song_atts.nbr_atts; ix++) {
            att_text[ix].id = (uint8_t )(app_song_atts.ele_atts[ix]);
            switch (app_song_atts.ele_atts[ix]) {
              case SONG_NAME:
#if (RSI_AUDIO_DATA_SRC == BIN_FILE)
                memcpy (&att_text[ix].att_text[0], (void *)AUDIO_FILE_PATH, strlen((int8_t *)AUDIO_FILE_PATH));
#endif
                break;
              case ARTIST_NAME:
                memcpy (att_text[ix].att_text, ARTIST_NAME_MALLI, strlen(ARTIST_NAME_MALLI));
                break;

              case ALBUM_NAME:
                memcpy (att_text[ix].att_text, ALBUM_NAME_LOCAL, strlen(ALBUM_NAME_LOCAL));
                break;

              case SONG_NUMBER:
                rsi_itoa (1, att_text[ix].att_text);
                break;

              case NUM_OF_SONGS:
                rsi_itoa (2, att_text[ix].att_text);
                break;

              case SONG_GENRE:
                memcpy (att_text[ix].att_text, GENRE_NAME_TELUGU, strlen(GENRE_NAME_TELUGU));
                break;

              case PLAY_TIME:
                rsi_itoa (((file_size/ PCM_DATA_LEN_PER_MS) * 1000), (uint8_t *)att_text[ix].att_text);
                break;

            }
          }
          rsi_bt_avrcp_ele_att_resp (str_conn_bd_addr, app_song_atts.nbr_atts, att_text);
        }
        break;

      case RSI_APP_EVENT_AVRCP_PLAY_STATUS:
        {
          uint32_t pos, size;
          uint8_t play_status;

          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_PLAY_STATUS);
          pos = ftell (fp);
          size = file_size;
          play_status = rsi_app_async_event_map & BIT (RSI_APP_EVENT_A2DP_CONFIGURE);
          rsi_bt_avrcp_play_status_resp ((int8_t *)str_conn_bd_addr, play_status, ((size/ PCM_DATA_LEN_PER_MS) * 1000), ((pos/ PCM_DATA_LEN_PER_MS) * 1000));
        }
        break;

      case RSI_APP_EVENT_AVRCP_REG_EVENT:
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_REG_EVENT);
          rsi_bt_avrcp_reg_notify_resp ((int8_t *)str_conn_bd_addr, 0);
        break;

    }
  }

  return 0;
}

/*==============================================*/
/**
 * @fn         main_loop
 * @brief      Schedules the Driver task.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function schedules the Driver task.
 */
void main_loop (void)
{
  while (1) {
    rsi_wireless_driver_task ();
  }
}


/*==============================================*/
/**
 * @fn         main
 * @brief      main function
 * @param[in]  none.
 * @return     none.
 * @section description
 * This is the main function to call Application
 */
#ifdef STM32F411xE
int main_tcp (void)
#else 
int main (void)
#endif
{

  int32_t status;
#ifdef RSI_WITH_OS
  rsi_task_handle_t bt_task_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;
#endif
#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

#ifndef RSI_WITH_OS
  //Start BT-BLE Stack
  intialize_bt_stack(STACK_BT_MODE);
  
  //! Call BLE Peripheral application
  status = rsi_bt_a2dp_source();

  //! Application main loop
  main_loop();

  return status;
#endif

#ifdef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(bt_global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! RS9113 initialization
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT-BLE Stack
  intialize_bt_stack(STACK_BT_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_bt_a2dp_source, "bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  rsi_start_os_scheduler();
  return status;
#endif
}
