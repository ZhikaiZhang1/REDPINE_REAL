/**
 * @file    rsi_bt_a2dp_source_avrcp_pts.c
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

#include <rsi_bt_sbc_codec.h>

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

#ifdef PTS_A2DP_AVRCP_1_3
#define INIT_TC_ID                     TC_SRC_AS_BV_01_I
#elif (defined PTS_ABS_VOL_FEAT)
#define INIT_TC_ID                     TC_CT_NFR_BV_04_C
#endif
#define RSI_BT_REMOTE_BD_ADDR          "00:1B:DC:F2:1C:53"      /* PTS */
//#define RSI_BT_REMOTE_BD_ADDR          "28:11:A5:CB:D6:6D"      /* PTS */

//#define RSI_BT_REMOTE_BD_ADDR        "B8:D5:0B:9B:D6:B2"      /* SRS-XB10 */
//#define RSI_BT_REMOTE_BD_ADDR        "00:11:67:BE:27:FA"      /* JBL Inspire 500 */
//#define RSI_BT_REMOTE_BD_ADDR        "00:18:09:D4:E2:AD"      /* MDR - XB50XS Sony */
//#define RSI_BT_REMOTE_BD_ADDR        "60:8C:4A:3A:A4:71"      /* Air Pods */
//#define RSI_BT_REMOTE_BD_ADDR        "74:5C:4B:0D:01:6A"      /* Jabra ELITE 65t */
//#define RSI_BT_REMOTE_BD_ADDR        "D0:8A:55:B7:44:15"      /* Method Wireless Skull candy Blue*/
//#define RSI_BT_REMOTE_BD_ADDR        "00:1E:7C:25:E9:4D"      /* HS_Phillips */
//#define RSI_BT_REMOTE_BD_ADDR        "D0:8A:55:74:4E:4A"      /* HS_JIB_Wireless */
//#define RSI_BT_REMOTE_BD_ADDR        "00:1A:7D:DA:71:74"      /* PC */
//#define RSI_BT_REMOTE_BD_ADDR        "30:22:00:00:33:24"      /* iBall */
//#define RSI_BT_REMOTE_BD_ADDR        "00:50:00:00:15:CD"      /* CRASHER Mini */
//#define RSI_BT_REMOTE_BD_ADDR        "00:17:03:01:E1:28"      /* Portornics */
//#define RSI_BT_REMOTE_BD_ADDR        "47:AB:44:D5:39:4B"      /* QY7 Red wireless */
//#define RSI_BT_REMOTE_BD_ADDR        "30:A8:DB:77:54:22"      /* SBH50 avrcp 1.3 */
//#define RSI_BT_REMOTE_BD_ADDR        "10:94:BB:5B:C1:08"      /* Ajay Air Pods */
//#define RSI_BT_REMOTE_BD_ADDR        "50:1A:A5:E0:10:59"      /* Jabra MOVE v.2.5.0 */
//#define RSI_BT_REMOTE_BD_ADDR        "00:58:50:69:4C:C6"      /* HBS - 730 */
//#define RSI_BT_REMOTE_BD_ADDR        "00:00:00:00:8E:FC"      /* X25 - HP -001 */
//#define RSI_BT_REMOTE_BD_ADDR        "50:1A:A5:45:28:86"      /* Jabra Talk - 2 */
//#define RSI_BT_REMOTE_BD_ADDR        "2C:BA:BA:64:3C:D4"      /* Samsung U Flex */
//#define RSI_BT_REMOTE_BD_ADDR        "B8:AD:3E:34:11:A4"      /* LG HBS910 */
//#define RSI_BT_REMOTE_BD_ADDR        "00:00:00:00:8E:FC"	    /* X25 */ 
//#define RSI_BT_REMOTE_BD_ADDR        "D0:8A:55:51:26:EB"    	/* JIB Wireless */
//#define RSI_BT_REMOTE_BD_ADDR        "C0:28:8D:1E:05:61"	    /* Jaybird X3 */


#if (RSI_AUDIO_DATA_SRC == BIN_FILE)
#define ELEMENT_READ_SIZE               1

#if (RSI_AUDIO_DATA_TYPE == SBC_AUDIO)

#define SONG_LIST_MAX_COUNT            3
#define AUDIO_FILE_PATH                "sbc_samples.sbc"
#define AUDIO_FILE_PATH_2              "sbc_samples_2.sbc"
#define AUDIO_FILE_PATH_3              "sbc_samples_3.sbc"
#define RSI_SBC_DATA_SIZE              1100
/* sampling frequency */
#define SBC_FREQ_16000                 0x00
#define SBC_FREQ_32000                 0x01
#define SBC_FREQ_44100                 0x02
#define SBC_FREQ_48000                 0x03

/* blocks */
#define SBC_BLK_4                      0x00
#define SBC_BLK_8                      0x01
#define SBC_BLK_12                     0x02
#define SBC_BLK_16                     0x03

/* channel mode */
#define SBC_MODE_MONO                  0x00
#define SBC_MODE_DUAL_CHANNEL          0x01
#define SBC_MODE_STEREO                0x02
#define SBC_MODE_JOINT_STEREO          0x03

/* allocation method */
#define SBC_AM_LOUDNESS                0x00
#define SBC_AM_SNR                     0x01

/* subbands */
#define SBC_SB_4                       0x00
#define SBC_SB_8                       0x01

//! SBC_FRAME Mapping Headers
#define PKT_HEADER_AND_CRC             11 // 11 --> l2cap+controller header + crc            
#define RTP_HEADER                     13 // 13 --> rtp_header
#define SBC_HEADER_SIZE                4
#define SBC_FRAME_SIZE                 sbc_frame_size
#define SKIP_SIZE                      (SBC_FRAME_SIZE + RTP_HEADER + PKT_HEADER_AND_CRC)
#define SBC_SYNCWORD                   0x9C
#define AVRCP_SUPPORT_EVENTS_CAPABILITIES_ID  3

enum {
  MONO         = SBC_MODE_MONO,
  DUAL_CHANNEL = SBC_MODE_DUAL_CHANNEL,
  STEREO       = SBC_MODE_STEREO,
  JOINT_STEREO = SBC_MODE_JOINT_STEREO
} mode;

uint16_t pkt_size;
uint8_t sbc_frames;
uint32_t sbc_frame_size = 0;
uint8_t sbc_data[RSI_SBC_DATA_SIZE];

#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
#define SONG_LIST_MAX_COUNT            3
#define AUDIO_FILE_PATH                "pcm_samples.wav"
#define AUDIO_FILE_PATH_2              "pcm_samples.wav"
#define AUDIO_FILE_PATH_3              "pcm_samples.wav"
#define RSI_PCM_DATA_SIZE              4000
uint16_t pkt_size;
uint8_t pcm_data[RSI_PCM_DATA_SIZE];
#define AVRCP_SUPPORT_EVENTS_CAPABILITIES_ID  3

#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
#define AUDIO_FILE_PATH                "mp3_samples.mp3"
#define RSI_MP3_DATA_SIZE              512
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

#endif

//! application events list
#define RSI_APP_EVENT_CONNECTED             1
#define RSI_APP_EVENT_PINCODE_REQ           2
#define RSI_APP_EVENT_LINKKEY_SAVE          3
#define RSI_APP_EVENT_AUTH_COMPLT           4
#define RSI_APP_EVENT_DISCONNECTED          5
#define RSI_APP_EVENT_LINKKEY_REQ           6
#define RSI_APP_EVENT_UNBOND_STATUS         7
#define RSI_APP_EVENT_CONFIRM_REQUEST       8

//! a2dp related defines
#define RSI_APP_EVENT_A2DP_CONFIGURE        9
#define RSI_APP_EVENT_A2DP_OPEN             10
#define RSI_APP_EVENT_A2DP_CONN             11
#define RSI_APP_EVENT_A2DP_DISCONN          12

//! avrcp related defines
#define RSI_APP_EVENT_AVRCP_CONN            13
#define RSI_APP_EVENT_AVRCP_DISCONN         14
#define RSI_APP_EVENT_AVRCP_PLAY            15
#define RSI_APP_EVENT_AVRCP_PAUSE           16
#define RSI_APP_EVENT_AVRCP_GET_CAP         17
#if 0
#define RSI_APP_EVENT_AVRCP_GET_ATTS        18
#define RSI_APP_EVENT_AVRCP_GET_ATT_VALS    19
#define RSI_APP_EVENT_AVRCP_CUR_ATT_VALS    20
#define RSI_APP_EVENT_AVRCP_SET_ATT_VALS    21
#define RSI_APP_EVENT_AVRCP_ATT_TEXT        22
#define RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT   23
#define RSI_APP_EVENT_AVRCP_CHAR_SET        24
#define RSI_APP_EVENT_AVRCP_BATT_STATUS     25
#else
#define RSI_APP_EVENT_AVRCP_STOP            18
#define RSI_APP_EVENT_AVRCP_VOL_UP          19
#define RSI_APP_EVENT_AVRCP_VOL_DOWN        20
#define RSI_APP_EVENT_AVRCP_SET_ABS_VOL     21
#define RSI_APP_EVENT_AVRCP_NOTIFY          22
#define RSI_APP_EVENT_AVRCP_NEXT            24
#define RSI_APP_EVENT_AVRCP_PREVIOUS        25
#endif
#define RSI_APP_EVENT_AVRCP_SONG_ATTS       26
#define RSI_APP_EVENT_AVRCP_PLAY_STATUS     27
#define RSI_APP_EVENT_AVRCP_REG_EVENT       28
#define RSI_APP_EVENT_A2DP_START            29
#define RSI_APP_EVENT_A2DP_SUSPEND          30
#define RSI_APP_EVENT_A2DP_MORE_DATA_REQ    31

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
#define RSI_APP_ERR_REMOTE_TERMINATED_CONN_PWR_OFF 0x4E15
#define RSI_APP_ERR_LOCAL_TERMINATED_CONN   0x4E16
#define RSI_APP_ERR_REPEATED_ATTEMPTS       0x4E17
#define RSI_APP_ERR_LMP_RESP_TIMEOUT        0x4E22
#define RSI_APP_ERR_HW_BUFFER_OVERFLOW      0x4057
#define RSI_APP_ERR_A2DP_CONN_ALRDY_EXISTS  0x4511

#define RSI_APP_ERR_AUDIO_EOF               0x00A0

#define BT_AVRCP_MEDIA_ATTR_ID_MAX_COUNT        0x07

#define BT_AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE      0x01
#define BT_AVRCP_MEDIA_ATTR_ID_ARTIST_NAME      0x02
#define BT_AVRCP_MEDIA_ATTR_ID_ALBUM_NAME       0x03
#define BT_AVRCP_MEDIA_ATTR_ID_TRACK_NUM        0x04
#define BT_AVRCP_MEDIA_ATTR_ID_NUM_OF_TRACK     0x05
#define BT_AVRCP_MEDIA_ATTR_ID_GENRE            0x06
#define BT_AVRCP_MEDIA_ATTR_ID_PLAYING_TIME_MS  0x07

#define AVRCP_EVENT_PLAYBACK_STATUS_CHANGED     0x01
#define AVRCP_EVENT_TRACK_CHANGED               0x02
#define AVRCP_EVENT_TRACK_REACHED_END           0x03 
#define AVRCP_EVENT_TRACK_REACHED_START         0x04
#define AVRCP_EVENT_PLAYBACK_POS_CHANGED        0x05 
#define AVRCP_EVENT_BATT_STATUS_CHANGED         0x06 
#define AVRCP_EVENT_SYSTEM_STATUS_CHANGED       0x07
#define AVRCP_EVENT_PLAYER_APPLICATION_SETTING_CHANGED 0x08 
#define AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED 0x09
#define AVRCP_EVENT_AVAILABLE_PLAYERS_CHANGED   0x0a 
#define AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED    0x0b 
#define AVRCP_EVENT_UIDS_CHANGED                0x0c 
#define AVRCP_EVENT_VOLUME_CHANGED              0x0d

#define AVRCP_CAP_ID_COMPANY_ID                 0x2
#define AVRCP_CAP_ID_EVENTS_SUPPORTED           0x3 

#define SAMPLE_ARTIST_NAME                      "SAMPLE_ARTIST_NAME"
#define SAMPLE_ALBUM_NAME                       "SAMPLE_ALBUM_NAME"
#define SAMPLE_GENRE                            "SAMPLE_GENRE"
#define PCM_DATA_LEN_PER_MS                     (44100 * 2 * 2)

#define REG_EVT_MAX                             5
#define DEFAULT_MTU_SIZE                        310
#define BD_ADDR_ARRAY_LEN                       18
#define RSI_LINKKEY_REPLY_SIZE                  16 

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN             10000

#ifdef RSI_WITH_OS
//! BT task stack size
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

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN] = {0};

//! enum for global state
typedef enum {
  INQUIRY,
  CONNECTED,
  AUTHENTICATED,
  A2DP_CONNECTED,
  A2DP_STREAM_OPEN,
  A2DP_STREAM_START,
  AVRCP_CONNECTED,
} app_state_t;

//! Application global parameters.
static uint32_t rsi_app_async_event_map = 0;
static rsi_bt_resp_get_local_name_t local_name = {0};
static uint8_t str_conn_bd_addr[BD_ADDR_ARRAY_LEN];
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t remote_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static rsi_bt_event_avrcp_notify_t avrcp_notify;
static rsi_bt_rsp_avrcp_get_capabilities_t  cap = {0};

FILE  *fp = NULL;
uint8_t abs_vol;
uint8_t play_count;
uint8_t play_status;
uint16_t sample_freq;
app_state_t app_state = 0;
static uint8_t passkey_data[6];
static uint8_t conn_resp_status;
uint16_t rem_mtu_size = DEFAULT_MTU_SIZE;

uint8_t app_song_list[SONG_LIST_MAX_COUNT][128] = {
  AUDIO_FILE_PATH,
  AUDIO_FILE_PATH_2,
  AUDIO_FILE_PATH_3};
uint8_t app_song_list_idx;

static uint8_t  linkkey[RSI_LINKKEY_REPLY_SIZE];
static uint8_t  dev_mode;
uint8_t app_event_id[REG_EVT_MAX];
uint8_t app_reg_events;

int16_t open_audio_file(FILE **fp, uint8_t *file_name);
int16_t close_audio_file(FILE **fp);
rsi_bt_a2dp_sbc_codec_cap_t set_sbc_cap;
rsi_bt_resp_a2dp_get_config_t get_sbc_cap;
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

uint16_t rel_count;
int8_t disconn_flag;
int8_t pair_mode_flag;
int8_t sbc_file_flag;
int16_t init_test_case (){

  int16_t status = 0;

  if (RSI_APP_CONN_ROLE != MASTER_ROLE && pair_mode_flag == 0){
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
    pair_mode_flag = 1;
  }else if (RSI_APP_CONN_ROLE == MASTER_ROLE && pair_mode_flag == 1){
    //! stop the discover mode
    status = rsi_bt_stop_discoverable();
    if(status != RSI_SUCCESS)
    {
      return status;
    }

    //! stop the connectability mode
    status = rsi_bt_set_non_connectable();
    if(status != RSI_SUCCESS)
    {
      return status;
    }
    pair_mode_flag = 0;
  }

  memset (linkkey, 0, RSI_LINKKEY_REPLY_SIZE);
  rel_count = 0;

  if (RSI_APP_CONN_ROLE != SLAVE_ROLE){
	  status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr,RSI_BT_REMOTE_BD_ADDR));
    if (status != RSI_SUCCESS) {
      rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
    }
  }

  return 0;
}

int16_t set_notify_event_data (uint8_t *p_evt_data)
{
  uint8_t i;
#if 0

  if (TC_ID == TC_TG_NFY_BV_02_C){
    /* 0xFF, 0xFF, 0xFF, 0xFF */
    for (i = 0; i < 4; i++) p_evt_data[i] = 0xFF; 
  }else if (TC_ID == TC_TG_NFY_BV_04_C){
    /* 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF */
    for (i = 0; i < 8; i++) p_evt_data[i] = 0xFF; 
  }else if (TC_ID == TC_TG_NFY_BV_08_C){
     /* 0X01, 0x02, 0x03, 0x04 */
    for (i = 0; i < 4; i++) p_evt_data[i] = 0;//i + 1; 
  }
#endif
  
  return 0;
}

int16_t update_test_case (){
  
  TC_ID++;
  
  if(TC_ID >=TC_SRC_ACP_SIG_SMG_BV_06_C || 
    TC_ID <=TC_SRC_INT_TRA_BTR_BV_01_C){
      RSI_PTS_PROFILE = BT_AVDTP;
  }

  if(TC_ID == TC_SRC_SET_BV_05_I ||
      TC_ID == TC_SRC_SET_BV_06_I){
//    TC_ID=TC_ID + 2;

  }
  if (TC_ID == TC_TG_MAX)
    TC_ID = TC_NORMAL_MODE;
#if 1
  if (TC_ID == TC_SRC_CC_BV_09_I ||
      TC_ID == TC_SRC_CC_BV_10_I ||
      TC_ID == TC_SRC_SET_BV_04_I ||
      TC_ID == TC_SRC_SET_BV_05_I ||
      TC_ID == TC_SRC_SET_BV_06_I ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_18_C ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_20_C ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_22_C)

  {
    int16_t status = 0;
    uint8_t sbc_file_name[64] = {0};
    if (TC_ID == TC_SRC_CC_BV_10_I){
      strcpy(sbc_file_name, "sbc_48_51.sbc");
    }
    else if (TC_ID == TC_SRC_CC_BV_09_I || 
             TC_ID == TC_SRC_SET_BV_05_I) {
      strcpy(sbc_file_name, "sbc_44_53.sbc");
    }
    else if (TC_ID == TC_SRC_SET_BV_04_I ||
             TC_ID == TC_SRC_SET_BV_06_I ||
             TC_ID == TC_SRC_ACP_SIG_SMG_BV_18_C ||
             TC_ID == TC_SRC_ACP_SIG_SMG_BV_20_C ||
             TC_ID == TC_SRC_ACP_SIG_SMG_BV_22_C ) {
      strcpy(sbc_file_name, "sbc_44_35.sbc");
    }

    close_audio_file(&fp);
    status = open_audio_file(&fp, sbc_file_name);
    if(status != RSI_SUCCESS) {
      return 0;
    }
    sbc_file_flag = 1;
  }
  else{
    if (sbc_file_flag == 1){
      int16_t status = 0;
      close_audio_file(&fp);
      status = open_audio_file(&fp, app_song_list[app_song_list_idx]);
      if(status != RSI_SUCCESS) {
        return 0;
      }
      sbc_file_flag = 0;
    }
  }

#endif
  if (TC_ID == TC_TG_CEC_BV_02_I ||
      TC_ID == TC_IOPT_CL_A2DP_SRC_SFC_BV_01_I ||
//      TC_ID == TC_IOCT_CL_AVRCP_TG_SFC_BV_04_I ||
      TC_ID == TC_CT_CEC_BV_01_I ||
      TC_ID == TC_TG_CCM_BV_01_C ||
    //  TC_ID == TC_SRC_REL_BV_01_I ||
      TC_ID == TC_NORMAL_MODE) {
    RSI_APP_CONN_ROLE = MASTER_ROLE;
  }
  else {
    RSI_APP_CONN_ROLE = SLAVE_ROLE; 
  }

  if ((TC_ID == TC_TG_CEC_BV_02_I) ||
      TC_ID == TC_NORMAL_MODE ||
      (TC_ID >= TC_SRC_INT_SIG_SMG_BV_05_C &&
      TC_ID <= TC_SRC_INT_TRA_BTR_BV_01_C) || 
//      TC_ID == TC_IOCT_CL_AVRCP_TG_SFC_BV_04_I ||
      TC_ID == TC_CT_CEC_BV_01_I ||
      TC_ID == TC_NORMAL_MODE ||
      TC_ID == TC_TG_CCM_BV_01_C ||
   //   TC_ID == TC_SRC_REL_BV_01_I ||
      TC_ID == TC_IOPT_CL_A2DP_SRC_SFC_BV_01_I)
  {
      RSI_APP_AVDTP_ROLE = INITIATOR_ROLE;
  }
  else {
    RSI_APP_AVDTP_ROLE = ACCEPTOR_ROLE;
  }

  if ((TC_ID >= TC_SRC_AS_BV_01_I && TC_ID <= TC_SRC_REL_BV_02_I) ||
      TC_ID == TC_CT_CEC_BV_01_I ||
      TC_ID == TC_CT_CEC_BV_02_I ||
      TC_ID == TC_NORMAL_MODE ||
   //   TC_ID == TC_SRC_REL_BV_01_I ||
      TC_ID == TC_SRC_SET_BV_03_I ||
      TC_ID == TC_SRC_SET_BV_05_I ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_16_C ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_22_C ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BI_23_C ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_20_C ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_22_C ||
      TC_ID == TC_SRC_INT_SIG_SMG_BV_17_C ||
      TC_ID == TC_SRC_INT_SIG_SMG_BV_19_C ||
      TC_ID == TC_SRC_INT_SIG_SMG_BV_21_C ||
      TC_ID == TC_SRC_INT_TRA_BTR_BV_01_C ||
//      TC_ID == TC_IOCT_CL_AVRCP_TG_SFC_BV_04_I ||
      TC_ID == TC_SRC_SUS_BV_01_I ||
      TC_ID == TC_SRC_SUS_BV_02_I ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_18_C)
    RSI_APP_A2DP_SEND_AUDIO = ENABLE_AUDIO;
  else
    RSI_APP_A2DP_SEND_AUDIO = DISABLE_AUDIO;

  if ((TC_ID >= TC_SRC_AS_BV_01_I && TC_ID <= TC_SRC_SET_BV_06_I) ||
      TC_ID == TC_SRC_SET_BV_03_I ||
      TC_ID == TC_SRC_SET_BV_05_I ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BI_23_C ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_20_C ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_22_C ||
      TC_ID == TC_SRC_INT_SIG_SMG_BV_17_C ||
      TC_ID == TC_SRC_INT_SIG_SMG_BV_19_C ||
      TC_ID == TC_SRC_INT_SIG_SMG_BV_21_C ||
      TC_ID == TC_SRC_INT_TRA_BTR_BV_01_C ||
      TC_ID == TC_SRC_ACP_SIG_SMG_BV_18_C)
    RSI_APP_AVRCP_CONN = DISABLE_AVRCP;
  else
    RSI_APP_AVRCP_CONN = ENABLE_AVRCP;

  LOG_PRINT ("\n*****************\n");
  LOG_PRINT ("*Test case ID %02d*\n", TC_ID);
  LOG_PRINT ("*****************\n");
#if 0 //NORMAL mode
  RSI_APP_AVDTP_ROLE = INITIATOR_ROLE;
  RSI_APP_CONN_ROLE = MASTER_ROLE;
#endif

  init_test_case ();

  return 0;
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
  if(resp_status == RSI_APP_ERR_NONE)
  {
    app_state |= (1 << CONNECTED);
    rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
  }
  else if(resp_status == RSI_APP_ERR_ACL_CONN_ALREADY_EXISTS)
  {
    app_state |= (1 << CONNECTED);
    app_state |= (1 << AUTHENTICATED);
    rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
  }
  else {
    rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
  }

  memcpy ((int8_t *)remote_dev_addr, conn_event->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on_conn: str_conn_bd_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, conn_event->dev_addr));

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
  memcpy ((int8_t *)remote_dev_addr, user_pincode_request->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on_pin_coe_req: str_conn_bd_addr: %s\r\n",
    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_pincode_request->dev_addr));
}
#if 0
/*==============================================*/
/**
 * @fn         rsi_bt_on_passkey_request
 * @brief      invoked when passkey request event is received
 * @param[out] passkey request, passkey request to remote device 
 * @return     none.
 * @section description
 * This callback function indicates the passkey request event
 */

void rsi_bt_on_passkey_request (uint16_t resp_status, rsi_bt_event_user_passkey_request_t *user_passkey_request)
{
  memcpy ((int8_t *)remote_dev_addr, user_passkey_request->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("passkey_request: str_conn_bd_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_passkey_request->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_PASSKEY_REQUEST);
}
#endif
/*==============================================*/
/**
 * @fn         rsi_bt_app_on_linkkey_req
 * @brief      invoked when linkkey request event is received
 * @param[out] user_linkkey_req, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey request from remote device
 */
void rsi_bt_app_on_linkkey_req (uint16_t resp_status, rsi_bt_event_user_linkkey_request_t  *user_linkkey_req)
{
  if (RSI_APP_CONN_ROLE == MASTER_ROLE){
    rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_REQ);
  }
	memcpy ((int8_t *)remote_dev_addr, user_linkkey_req->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("linkkey_req: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_req->dev_addr));
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
void rsi_bt_app_on_linkkey_save (uint16_t resp_status, rsi_bt_event_user_linkkey_save_t *user_linkkey_save)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_SAVE);
	memcpy ((int8_t *)remote_dev_addr, user_linkkey_save->dev_addr, RSI_DEV_ADDR_LEN);
  memcpy (linkkey, user_linkkey_save->linkKey, RSI_LINKKEY_REPLY_SIZE);
	LOG_PRINT ("linkkey_save: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_save->dev_addr));
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
  if(resp_status == 0){
    rsi_bt_app_set_event (RSI_APP_EVENT_AUTH_COMPLT);
    app_state |= (1 << AUTHENTICATED);
  }
  else {
    memset (linkkey, 0, RSI_LINKKEY_REPLY_SIZE);
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
void rsi_bt_app_on_unbond_status (uint16_t resp_status, rsi_bt_event_unbond_t *unbond_status)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_UNBOND_STATUS);
  memcpy((int8_t *)remote_dev_addr, unbond_status->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on unbond status: bd addr %s, resp status 0x%x\r\n", 
  		rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, unbond_status->dev_addr), 
  		resp_status);


  if (resp_status == RSI_APP_ERR_LOCAL_TERMINATED_CONN)
    disconn_flag = 1;
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
	memcpy ((int8_t *)remote_dev_addr, bt_disconnected->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("on_disconn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, bt_disconnected->dev_addr));


  if (resp_status == RSI_APP_ERR_REMOTE_TERMINATED_CONN || 
      resp_status == RSI_APP_ERR_REMOTE_TERMINATED_CONN_PWR_OFF)
    disconn_flag = 1;

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
void rsi_bt_on_confirm_request (uint16_t resp_status, rsi_bt_event_user_confirmation_request_t *user_confirmation_request)
{
  LOG_PRINT ("data: %s\r\n",user_confirmation_request->confirmation_value );
  rsi_bt_app_set_event (RSI_APP_EVENT_CONFIRM_REQUEST);
  memcpy (remote_dev_addr, user_confirmation_request->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on confirm req: bd addr %s, resp status 0x%x\r\n", 
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_confirmation_request->dev_addr), 
      resp_status);
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
void rsi_bt_app_on_a2dp_connect (uint16_t resp_status, rsi_bt_event_a2dp_connect_t *a2dp_connect)
{
  if(resp_status == 0)
  {
    rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_CONN);
    app_state |= (1 << A2DP_CONNECTED);
    memcpy((int8_t *)remote_dev_addr, a2dp_connect->dev_addr, RSI_DEV_ADDR_LEN);
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
void rsi_bt_app_on_a2dp_disconnect (uint16_t resp_status, rsi_bt_event_a2dp_disconnect_t *a2dp_disconnect)
{
  if(resp_status == 0) {
    app_state &= ~(1 << A2DP_CONNECTED);
    rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_DISCONN);
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
void rsi_bt_app_on_a2dp_configure (uint16_t resp_status, rsi_bt_event_a2dp_configure_t *a2dp_configure)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_CONFIGURE);
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
void rsi_bt_on_a2dp_more_data_req (uint16_t resp_status, rsi_bt_event_a2dp_more_data_req_t *a2dp_more_data_req)
{
  if (resp_status == RSI_SUCCESS)
  {
    if (app_state &  (1 << A2DP_STREAM_START))
    {
      rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
      //rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_more_data_req->dev_addr);
      //LOG_PRINT ("a2dp_more_data_req: str_conn_bd_addr: %s\r\n", str_conn_bd_addr);
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
void rsi_bt_app_on_a2dp_open (uint16_t resp_status, rsi_bt_event_a2dp_open_t *a2dp_oen)
{
  if(resp_status == 0)
  {
    rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_OPEN);
    app_state |= (1 << A2DP_STREAM_OPEN);
    memcpy ((int8_t *)remote_dev_addr, a2dp_oen->dev_addr, RSI_DEV_ADDR_LEN);
  }
  LOG_PRINT ("on a2dp open: bd addr %s, resp status 0x%x\r\n", 
  		rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_oen->dev_addr), 
		  resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_a2dp_start
 * @brief      invoked when a2dp profile start event is received
 * @param[out] a2dp_start, a2dp start remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp start remote device information
 */
void rsi_bt_app_on_a2dp_start (uint16_t resp_status, rsi_bt_event_a2dp_start_t *a2dp_start)
{
  uint32_t freq;
  if (resp_status == 0)
  { 
    sample_freq = freq = a2dp_start->sample_freq;
    rem_mtu_size = (a2dp_start->rem_mtu_size > DEFAULT_MTU_SIZE) ? (a2dp_start->rem_mtu_size) : DEFAULT_MTU_SIZE;
    rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_START);
    app_state |= (1 << A2DP_STREAM_START);
  }
  memcpy (remote_dev_addr, a2dp_start->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on a2dp start: bd addr %s, resp status 0x%x\r\n", 
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_start->dev_addr), 
      resp_status);
  LOG_PRINT ("  sample freq %d, rem mtu size %d\r\n", freq, rem_mtu_size);
}


/**
 * @fn         rsi_bt_app_on_a2dp_suspend
 * @brief      invoked when a2dp profile suspend event is received
 * @param[out] a2dp_suspend, a2dp suspend remote device information
 * @return     none.
 * @section description
 * This callback function indicates the a2dp suspend remote device information
 */
void rsi_bt_app_on_a2dp_suspend (uint16_t resp_status, rsi_bt_event_a2dp_suspend_t *a2dp_suspend)
{
  uint32_t freq;
  if (resp_status == 0)
  { 
    rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_SUSPEND);
  }
  memcpy (remote_dev_addr, a2dp_suspend->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on a2dp suspend: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, a2dp_suspend->dev_addr),
      resp_status);
}


/**
 * @fn         rsi_bt_app_a2dp_pause
 * @brief      API to initiate a2dp stream pause
 * @param[out] none
 * @return     none.
 * @section description
 * This function request to susped the a2dp stream
 */
void rsi_bt_app_a2dp_pause ()
{
  int32_t status = 0;

  status = rsi_bt_a2dp_suspend(remote_dev_addr);
  if (status != RSI_SUCCESS)
  {
    LOG_PRINT ("A2DP suspend req: bd addr %s, status err 0x%x\r\n", str_conn_bd_addr, status);
  }
  else
  {
    app_state &= ~(1 << A2DP_STREAM_START);
    rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
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
void rsi_bt_app_a2dp_resume ()
{
  int32_t status = 0;

  status = rsi_bt_a2dp_start(remote_dev_addr);
  if (status != RSI_SUCCESS)
  {
    LOG_PRINT ("A2DP start req: bd addr %s, status err 0x%x\r\n", str_conn_bd_addr, status);
  }
  else
  {
    /*
     * 0x00 Active Mode.
     * 0x01 Hold Mode.
     * 0x02 Sniff Mode. */
    if (dev_mode == 0x02) {
      rsi_bt_sniff_exit_mode(remote_dev_addr);
    }
    app_state |= (1 << A2DP_STREAM_START);
    rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
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
void rsi_bt_app_on_avrcp_connect (uint16_t resp_status, rsi_bt_event_avrcp_connect_t *avrcp_connect)
{
  if(resp_status == 0) {
    app_state |= (1 << AVRCP_CONNECTED);
    rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_CONN);
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
void rsi_bt_app_on_avrcp_disconnect (uint16_t resp_status, rsi_bt_event_avrcp_disconnect_t *avrcp_disconn)
{
  if(resp_status == 0) {
    app_state &= ~(1 << AVRCP_CONNECTED);
    rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_DISCONN);
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
void rsi_bt_app_on_avrcp_play(uint16_t resp_status, rsi_bt_event_avrcp_play_t *avrcp_play)
{
  play_count ++;
  if (play_count % 2 == 0) {
    rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_PLAY);
    play_count = 0;
  }
  memcpy ((int8_t *)remote_dev_addr, avrcp_play->dev_addr, RSI_DEV_ADDR_LEN);
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
void rsi_bt_app_on_avrcp_pause (uint16_t resp_status, rsi_bt_event_avrcp_pause_t *avrcp_pause)
{
  play_count ++;
  if(play_count % 2 == 0){
    rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_PAUSE);
    play_count = 0;
  }
  memcpy ((int8_t *)remote_dev_addr, avrcp_pause->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp pause: bd addr %s, resp status 0x%x\r\n", 
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_pause->dev_addr),
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_avrcp_stop
 * @brief      invoked when avrcp stop event is received
 * @param[out] avrcp stop,remote device bdaddress
 * @return     none.
 * @section description
 * This callback function indicates the avrcp stop event
 */
void rsi_bt_app_on_avrcp_stop (uint16_t resp_status, rsi_bt_event_avrcp_stop_t *avrcp_stop)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_STOP);
  memcpy (remote_dev_addr, avrcp_stop->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp stop: bd addr %s, resp status 0x%x\r\n", 
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_stop->dev_addr),
      resp_status);
 
  return;
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
void rsi_bt_app_on_avrcp_next (uint16_t resp_status, rsi_bt_event_avrcp_next_t *avrcp_next)
{
  play_count ++;
  if(play_count % 2 == 0){
    rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_NEXT);
    play_count = 0;
  }
	memcpy ((int8_t *)remote_dev_addr, avrcp_next->dev_addr, RSI_DEV_ADDR_LEN);
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
void rsi_bt_app_on_avrcp_previous (uint16_t resp_status, rsi_bt_event_avrcp_previous_t *avrcp_prev)
{
  play_count ++;
  if(play_count % 2 == 0){
    rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_PREVIOUS);
    play_count = 0;
  }
	memcpy ((int8_t *)remote_dev_addr, avrcp_prev->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp previous: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_prev->dev_addr),
      resp_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_avrcp_vol_up
 * @brief      invoked when avrcp vol up event is received
 * @param[out] avrcp vol up,remote device bdaddress
 * @return     none.
 * @section description
 * This callback function indicates the avrcp vol up event
 */
void rsi_bt_app_on_avrcp_vol_up (uint16_t resp_status, rsi_bt_event_avrcp_vol_up_t *avrcp_vol_up)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_VOL_UP);
  memcpy (remote_dev_addr, avrcp_vol_up->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp vol up: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_vol_up->dev_addr),
      resp_status);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_avrcp_vol_down
 * @brief      invoked when avrcp vol down event is received
 * @param[out] avrcp vol down,remote device bdaddress
 * @return     none.
 * @section description
 * This callback function indicates the avrcp vol down event
 */
void rsi_bt_app_on_avrcp_vol_down (uint16_t resp_status, rsi_bt_event_avrcp_vol_down_t *avrcp_vol_down)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_VOL_DOWN);
  memcpy (remote_dev_addr, avrcp_vol_down->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp vol down: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, avrcp_vol_down->dev_addr),
      resp_status);
 
  return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_avrcp_notify_event
 * @brief      invoked when avrcp notification event is received 
 * @param[out] avrcp p_notify, player notication event related info
 * @return     none.
 * @section description
 * This callback function indicates the avrcp notification event
 */
void rsi_bt_on_avrcp_notify_event (uint16_t resp_status,rsi_bt_event_avrcp_notify_t *p_notify)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_NOTIFY);
  memcpy (&avrcp_notify, p_notify, sizeof (rsi_bt_event_avrcp_notify_t));
  memcpy (remote_dev_addr, p_notify->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp notify: bd addr %s, resp status 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, p_notify->dev_addr),
      resp_status);
  
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_avrcp_get_cap_event
 * @brief      invoked when AVRCP get capabilities event is received
 * @param[out] resp_status, event status
 * @return     none.
 * @section description
 * This callback function indicates the avrcp get cap event
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
  memcpy (remote_dev_addr, bd_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp get cap: bd addr %s, capability ID %d\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, bd_addr),
      cap_type);
  
  return;
}

#if 0
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
#endif 

void rsi_bt_on_avrcp_get_song_atts_event (rsi_bt_event_avrcp_ele_att_t *p_song_atts)
{
  uint8_t ix;

  memcpy (&app_song_atts, p_song_atts, sizeof (rsi_bt_event_avrcp_ele_att_t));
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_SONG_ATTS);

  if (p_song_atts->nbr_atts == 0){
    /* all attribute information shall be returned */
    app_song_atts.nbr_atts = BT_AVRCP_MEDIA_ATTR_ID_MAX_COUNT;
    for (ix = BT_AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE; ix <= BT_AVRCP_MEDIA_ATTR_ID_PLAYING_TIME_MS; ix++)
      app_song_atts.ele_atts[ix-1] = ix;
  }
  LOG_PRINT ("on avrcp get elem attr\r\n");
  LOG_PRINT ("No. of attrs: %d, attr list:", p_song_atts->nbr_atts);
  for (ix = 0; ix < p_song_atts->nbr_atts; ix++) {
    LOG_PRINT ("%d, ",p_song_atts->ele_atts[ix]);
  }
  LOG_PRINT ("\r\n");

  return;
}

void rsi_bt_on_avrcp_get_play_status_event (uint8_t  *bd_addr)
{
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_PLAY_STATUS);
  memcpy (remote_dev_addr, bd_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp get play status: bd addr %s\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, bd_addr));
  
  return;
}

void rsi_bt_on_avrcp_reg_notify_event (uint8_t  *bd_addr, uint8_t event_id)
{
  app_event_id[app_reg_events++] = event_id;
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_REG_EVENT);
  memcpy (remote_dev_addr, bd_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on avrcp get play status: bd addr %s, event ID %d\r\n",
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, bd_addr),
      event_id);

  return;
}

void rsi_bt_on_avrcp_set_abs_vol_event (rsi_bt_event_avrcp_set_abs_vol_t *p_abs_vol)
{
  uint8_t abs_vol_per;
  rsi_bt_app_set_event (RSI_APP_EVENT_AVRCP_SET_ABS_VOL);
  memcpy (remote_dev_addr, p_abs_vol->dev_addr, RSI_DEV_ADDR_LEN);
  abs_vol = p_abs_vol->abs_vol;
  /* The value 0x0 corresponds to 0%.
   * The value 0x7F corresponds to 100%. */
  abs_vol_per = (abs_vol * 100) / 0x7f;

  LOG_PRINT ("on avrcp set abs vol: bd addr %s, Vol %d%\r\n", 
      rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, p_abs_vol->dev_addr),
      abs_vol_per);
 
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
int32_t sbc_calc_frame_len(uint8_t *data)
{
  int32_t ret = 0;
  uint8_t block_mode, mode, subband_mode, subbands, channels, blocks, joint, bitpool;

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
    case DUAL_CHANNEL:  /* fall-through */
    case STEREO:
    case JOINT_STEREO:
      channels = 2;
      break;
  }

  subband_mode = (data[1] & 0x01);
  subbands = subband_mode ? 8 : 4;

  bitpool = data[2];

  if ((mode == MONO || mode == DUAL_CHANNEL) &&
      bitpool > 16 * subbands)
    return -4;

  if ((mode == STEREO || mode == JOINT_STEREO) &&
      bitpool > 32 * subbands)
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

int16_t read_sbc_data(FILE *fp)
{
  int16_t fread_resp = 0;
  uint8_t p_sbc_data[RSI_SBC_DATA_SIZE] = {0};

  pkt_size = 0;
  sbc_frames = 0;

  while(pkt_size < (rem_mtu_size - SKIP_SIZE)){ 
    //! read file 1 byte
    fread_resp = fread (&p_sbc_data[pkt_size], ELEMENT_READ_SIZE, SBC_HEADER_SIZE, fp);
    if(fread_resp < SBC_HEADER_SIZE)
    {
      //! error ...
      LOG_PRINT (" fread err\n\n");
      return RSI_APP_ERR_AUDIO_EOF;
    }
    else{
      pkt_size += fread_resp;
    }

    //! check SW == 0x9C
    if(p_sbc_data[0] == SBC_SYNCWORD){
      //! read file <size-1>
      sbc_frame_size = sbc_calc_frame_len(&p_sbc_data[0]);
      fread_resp = fread (&p_sbc_data[pkt_size], ELEMENT_READ_SIZE, (sbc_frame_size - SBC_HEADER_SIZE), fp);
      if(fread_resp < (sbc_frame_size - SBC_HEADER_SIZE))
      {
        //! error ...
        LOG_PRINT (" fread err\n\n");
        return RSI_APP_ERR_AUDIO_EOF;
      }
      else{
        pkt_size += fread_resp;
      }
    }
    else{
      //! error ...
      LOG_PRINT (" fread err\n\n");
      return RSI_APP_ERR_AUDIO_EOF;
    }
    sbc_frames++;
  }
  memset (sbc_data, 0, sizeof (sbc_data));
  memcpy (sbc_data, p_sbc_data, pkt_size);

  return 0;
}

#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
int16_t read_pcm_data(FILE *fp)
{
  pkt_size = fread (pcm_data, ELEMENT_READ_SIZE, 512*5, fp);
  //LOG_PRINT ("\r\npcm pkt len read: %d\r\n", pkt_size);
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
  if(err) {
    return err;
  }

  err = rsi_bt_a2dp_send_sbc_aac_data (remote_dev_addr, sbc_data, pkt_size, 0);
#elif (RSI_AUDIO_DATA_TYPE == PCM_AUDIO)
  err = read_pcm_data(fp);

  if(err)
    return err;
  err = rsi_bt_a2dp_send_pcm_mp3_data (remote_dev_addr, pcm_data, 512*5, RSI_AUDIO_DATA_TYPE);

#elif (RSI_AUDIO_DATA_TYPE == MP3_AUDIO)
  err = read_mp3_data(fp);
  if(err)
    return err;
  err = rsi_bt_a2dp_send_pcm_mp3_data (remote_dev_addr, mp3_data, pkt_size, RSI_AUDIO_DATA_TYPE);

#endif

#endif

  return err;
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
uint8_t count ;
int32_t rsi_bt_a2dp_source (void)
{
  int32_t status = 0,set_config_status=0;
  uint32_t file_size = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[BD_ADDR_ARRAY_LEN] = {0};
  uint8_t eir_data[64] = {2,1,0};
  uint8_t ix;
  
#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! module intialisation 
  status = rsi_device_init(LOAD_NWP_FW);
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
  status = open_audio_file(&fp, app_song_list[app_song_list_idx]);
  if(status != RSI_SUCCESS) {
    return 0;
  } 

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
      NULL,
      rsi_bt_app_on_conn, 
      rsi_bt_app_on_unbond_status,
      rsi_bt_app_on_disconn,
      NULL,
      NULL,      
      NULL, 
      NULL,
      rsi_bt_on_confirm_request,
      rsi_bt_app_on_pincode_req,
      NULL, 
      NULL,
      rsi_bt_app_on_auth_complete,
      rsi_bt_app_on_linkkey_req,
      NULL,
      rsi_bt_app_on_linkkey_save,
      NULL, 
      NULL,
      rsi_bt_on_mode_change,
      NULL);
 
  //! initialize the event map
  rsi_bt_app_init_events ();

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(local_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\r\nlocal_bd_address: %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_bd_addr, local_dev_addr));

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

  //! start the ssp mode
  status = rsi_bt_set_ssp_mode(1,0);
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

  //! register the A2DP profile callback's
  rsi_bt_a2dp_register_callbacks (
      rsi_bt_app_on_a2dp_connect,
      rsi_bt_app_on_a2dp_disconnect,
      rsi_bt_app_on_a2dp_configure,
      rsi_bt_app_on_a2dp_open,
      rsi_bt_app_on_a2dp_start,
      rsi_bt_app_on_a2dp_suspend,
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
      rsi_bt_app_on_avrcp_stop,
      rsi_bt_app_on_avrcp_next,
      rsi_bt_app_on_avrcp_previous,
      rsi_bt_app_on_avrcp_vol_up,
      rsi_bt_app_on_avrcp_vol_down,
      NULL,
      rsi_bt_on_avrcp_notify_event);

  rsi_bt_avrcp_target_register_callbacks(
      rsi_bt_on_avrcp_get_cap_event,
      NULL, //rsi_bt_on_avrcp_get_app_supp_atts_event,
      NULL, //rsi_bt_on_avrcp_get_app_suup_att_vals_event,
      NULL, //rsi_bt_on_avrcp_get_app_cur_att_val_event,
      NULL, //rsi_bt_on_avrcp_set_app_cur_att_val_event,
      NULL, //rsi_bt_on_avrcp_get_app_att_text_event,
      NULL, //rsi_bt_on_avrcp_get_app_att_vals_text_event,
      NULL, //rsi_bt_on_avrcp_supp_char_sets_event,
      NULL, //rsi_bt_on_avrcp_batt_status_event,
      rsi_bt_on_avrcp_get_song_atts_event,
      rsi_bt_on_avrcp_get_play_status_event,
      rsi_bt_on_avrcp_reg_notify_event,
      rsi_bt_on_avrcp_set_abs_vol_event);

  TC_ID = INIT_TC_ID - 1;
  update_test_case ();

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
          if ((app_state & (1 << CONNECTED)) &&
              (app_state & (1 << AUTHENTICATED))){
            if (RSI_APP_CONN_ROLE != SLAVE_ROLE){
              usleep(500000);
              status = rsi_bt_a2dp_connect (remote_dev_addr);
              if(status != RSI_SUCCESS)
              {
                if(status == RSI_APP_ERR_SOCK_DISCONN){
                  rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
                  app_state = 0; 
                }
                else if (status == RSI_APP_ERR_A2DP_CONN_ALRDY_EXISTS){}
                else
                  rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
                LOG_PRINT("a2dp conn resp status 0x%04x\r\n", status);
              }
            }
          }
        }
        break;

      case RSI_APP_EVENT_LINKKEY_REQ:
        {
          //! linkkey request event

          //! clear the linkkey request event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_REQ);
        
          if ((linkkey[0] == 0) && (linkkey[15] == 0))
          {
            LOG_PRINT("linkkey request negative reply\r\n");
            //! sending the linkkey request negative reply
            rsi_bt_linkkey_request_reply ((int8_t *)remote_dev_addr, NULL, 0);
          } 
          else
          {
            LOG_PRINT("linkkey request positive reply\r\n");
            //! sending the linkkey request positive reply
            rsi_bt_linkkey_request_reply ((int8_t *)remote_dev_addr, linkkey, 1);
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
          status = rsi_bt_pincode_request_reply((int8_t *)remote_dev_addr, pin_code, 1);
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
          if(TC_ID == TC_SRC_ACP_SIG_SMG_BI_08_C){
            sleep(5);
          }
        }
        break;

      case RSI_APP_EVENT_AUTH_COMPLT:
        {
          //! authentication complete event

          //! clear the authentication complete event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AUTH_COMPLT);
          
          if ((app_state & (1 << CONNECTED)) &&
              (app_state & (1 << AUTHENTICATED))){
            if (RSI_APP_CONN_ROLE != SLAVE_ROLE){
              usleep(500000);
              status = rsi_bt_a2dp_connect (remote_dev_addr);
              if(status != RSI_SUCCESS)
              {
                if(status == RSI_APP_ERR_SOCK_DISCONN){
                  rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
                  app_state = 0; 
                }
                else if (status == RSI_APP_ERR_A2DP_CONN_ALRDY_EXISTS){}
                else
                  rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);

                LOG_PRINT("a2dp conn resp status 0x%04x\n", status);
              }

            }
          }

        }
        break;

      case RSI_APP_EVENT_UNBOND_STATUS:
        {
          //! remote device unbond event

          //! clear the unbond event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_UNBOND_STATUS);
          
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
          rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
          app_state = 0; 
        }
        break;

      case RSI_APP_EVENT_DISCONNECTED:
        {
          //! remote device disconnect event

          //! clear the disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_DISCONNECTED);

          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);

          app_reg_events = 0;
          for (ix = 0; ix < REG_EVT_MAX; ix++)
            app_event_id[ix] = 0;
          app_state = 0; 
          rem_mtu_size = DEFAULT_MTU_SIZE;

          if (disconn_flag){
            //sleep (3);
            disconn_flag = 0;
            update_test_case();
          }

          if (RSI_APP_CONN_ROLE != SLAVE_ROLE){
            LOG_PRINT("looking for device\n");
	          status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr,RSI_BT_REMOTE_BD_ADDR));
            LOG_PRINT("reconnect status = %x \n", status);
            if (status != RSI_SUCCESS) {
              if(status == RSI_APP_ERR_LIMITED_RESOURCE){ 
                LOG_PRINT (" Disconn !!\n");
              }
              else if (status == RSI_APP_ERR_ACL_CONN_ALREADY_EXISTS){
                app_state |= (1 << CONNECTED);
                app_state |= (1 << AUTHENTICATED);
                rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
                LOG_PRINT ("Already conn \n");
              }
              else
                rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
            }
          }
        }
        break;

      case RSI_APP_EVENT_CONFIRM_REQUEST:
        {
          //! confirm req event
          
          //! clear the ssp confirmation event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_CONFIRM_REQUEST);

          rsi_bt_accept_ssp_confirm((int8_t *)remote_dev_addr);
        }
        break;

      case RSI_APP_EVENT_A2DP_CONN:
        {
          //! a2dp connected event

          //! clear the a2dp connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONN);
          if (RSI_APP_AVRCP_CONN == DISABLE_AVRCP)
            app_state |= (1 << AVRCP_CONNECTED);
        }
        break;

      case RSI_APP_EVENT_A2DP_DISCONN:
        {
          //! a2dp disconnected event

          //! clear the a2dp disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_DISCONN);
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
        }
        break;

      case RSI_APP_EVENT_A2DP_CONFIGURE:
        {
          //! a2dp configured event

          //! clear the a2dp configured event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_CONFIGURE);
        }
        break;

      case RSI_APP_EVENT_A2DP_OPEN:
        {
          //! a2dp open event

          //! clear the a2dp open event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_OPEN);
          sleep(1);
          if (RSI_APP_A2DP_SEND_AUDIO == ENABLE_AUDIO){
            status = rsi_bt_a2dp_start(remote_dev_addr);
            if (status != RSI_SUCCESS)
            {
              LOG_PRINT("a2dp start resp %x\n", status);
            }
            else{
              if(TC_ID == TC_SRC_SUS_BV_01_I||
                  TC_ID == TC_SRC_SUS_BV_02_I){
                rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
              }
            } 
          } 
              status = rsi_bt_a2dp_get_config(remote_dev_addr, &get_sbc_cap);
              LOG_PRINT("\nChannelMode %d, Freq %d, AllocMethod %d, MaxBitPool %d\n",
                  get_sbc_cap.sbc_cap.ChannelMode,
                  get_sbc_cap.sbc_cap.Freq,
                  get_sbc_cap.sbc_cap.AllocMethod,
                  get_sbc_cap.sbc_cap.MaxBitPool);
              memcpy (&set_sbc_cap, &get_sbc_cap.sbc_cap, sizeof(set_sbc_cap));
          if (TC_ID == TC_SRC_CC_BV_10_I || TC_ID == TC_SRC_SET_BV_04_I || RSI_PTS_PROFILE == BT_AVDTP ){
            if (BT_HOST_SBC == 1){
              status = rsi_bt_a2dp_get_config(remote_dev_addr, &get_sbc_cap);
              LOG_PRINT("\nChannelMode %d, Freq %d, AllocMethod %d, MaxBitPool %d\n",
                  get_sbc_cap.sbc_cap.ChannelMode,
                  get_sbc_cap.sbc_cap.Freq,
                  get_sbc_cap.sbc_cap.AllocMethod,
                  get_sbc_cap.sbc_cap.MaxBitPool);
              memcpy (&set_sbc_cap, &get_sbc_cap.sbc_cap, sizeof(set_sbc_cap));
              status = rsi_bt_a2dp_set_config(remote_dev_addr, &set_sbc_cap, &set_config_status);
              if(TC_ID == TC_SRC_CC_BV_10_I){
                /* RE INIT SBC */
                set_sbc_cap.Freq = SBC_SAMPLING_FREQ_48000;
                set_sbc_cap.MaxBitPool = 51;
              }
              if(TC_ID == TC_SRC_SET_BV_04_I){
                /* RE INIT SBC */
                set_sbc_cap.Freq = SBC_SAMPLING_FREQ_44100;
                set_sbc_cap.MaxBitPool = 35;
              }
              rsi_bt_cmd_sbc_reinit ((void *)&set_sbc_cap); 

            }
            if(RSI_PTS_PROFILE == BT_AVDTP){
                set_sbc_cap.MaxBitPool = 35;
            }
          }
          if (TC_ID == TC_TG_CRC_BV_02_C || TC_ID == TC_CT_CRC_BV_01_C){
            sleep (3);
            rsi_bt_disconnect (remote_dev_addr); 
          }
          else if (TC_ID == TC_TG_CEC_BV_02_I || TC_ID == TC_TG_CCM_BV_01_C){
            if (!(app_state & (1 << AVRCP_CONNECTED))){
              if (RSI_APP_CONN_ROLE != SLAVE_ROLE){
                status = rsi_bt_avrcp_conn (remote_dev_addr);
                if(status != RSI_SUCCESS)
                {
                  LOG_PRINT("avrcp conn resp status 0x%04x\r\n", status);
                }
              }
            }
          }
          else if (TC_ID == TC_SRC_INT_SIG_SMG_BV_23_C){
            status = rsi_bt_a2dp_abort(remote_dev_addr);
            //            status = rsi_bt_a2dp_close(remote_dev_addr);
            if(status != RSI_SUCCESS)
              LOG_PRINT("a2dp abort status 0x%04x\r\n", status);
          }
          if(TC_ID == TC_CT_CFG_BV_01_C){
            status = rsi_bt_avrcp_get_capabilities(remote_dev_addr,AVRCP_SUPPORT_EVENTS_CAPABILITIES_ID,&cap);
            if(status !=RSI_SUCCESS){
              break;}
          }
          if(TC_ID == TC_CT_PTH_BV_01_C || TC_ID == TC_CT_PTT_BV_01_I || TC_ID == TC_CT_PTT_BV_02_I){
            status = rsi_bt_avrcp_play(remote_dev_addr);
            if(status != RSI_SUCCESS){
              break;
            }
          }
          if(TC_ID == TC_TG_CCM_BV_02_C){
            sleep(5);
            rsi_bt_disconnect(remote_dev_addr);            
          }
        }
        break;
      case RSI_APP_EVENT_A2DP_START:
        {
          //! a2dp start event

          //! clear the a2dp start event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_START);
          if (!(app_state & (1 << AVRCP_CONNECTED))){
            if (RSI_APP_CONN_ROLE != SLAVE_ROLE){
              usleep(500000);
              status = rsi_bt_avrcp_conn (remote_dev_addr);
              if(status != RSI_SUCCESS)
              {
                LOG_PRINT("avrcp conn resp status 0x%04x\r\n", status);
              }
            }
          }
          else{
            usleep(500000);
            if(TC_ID != TC_CT_PTH_BV_01_C && TC_ID != TC_CT_PTT_BV_01_I && TC_ID != TC_CT_PTT_BV_02_I){
              rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
            }
            play_status = 1;
            LOG_PRINT("a2dp send audio\n");
          }
          if(TC_ID == TC_SRC_SUS_BV_01_I ||
              TC_ID == TC_SRC_SUS_BV_02_I){
            rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
          }
        }
        break;
      case RSI_APP_EVENT_A2DP_SUSPEND:
        {
          //! a2dp suspend event         
          rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_SUSPEND);
		  rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
          LOG_PRINT("a2dp suspend\n");
        }
        break;
    
    case RSI_APP_EVENT_A2DP_MORE_DATA_REQ:
        {
          //! a2dp more data event

          if (!(app_state & (1 << CONNECTED)) &&
              !(app_state & (1 << A2DP_CONNECTED))){
            rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
            break;
          }

          if(TC_ID == TC_SRC_REL_BV_01_I ||
              TC_ID == TC_SRC_INT_SIG_SMG_BV_19_C ||
              TC_ID == TC_SRC_INT_SIG_SMG_BV_21_C|| 
              TC_ID == TC_SRC_SUS_BV_01_I){
            if (rel_count == 200){

              rel_count = 0;
              if (TC_ID == TC_SRC_REL_BV_01_I)
                rsi_bt_a2dp_close(remote_dev_addr);
              else if(TC_ID == TC_SRC_INT_SIG_SMG_BV_19_C)
                rsi_bt_a2dp_close(remote_dev_addr);
              else if(TC_ID == TC_SRC_INT_SIG_SMG_BV_21_C || TC_ID == TC_SRC_SUS_BV_01_I){
                status = rsi_bt_a2dp_suspend(remote_dev_addr);
                if (status != RSI_SUCCESS)
                {
                  LOG_PRINT ("A2DP suspend req: bd addr %s, status err 0x%x\r\n", str_conn_bd_addr, status);
                }
                else
                {
                  app_state &= ~(1 << A2DP_STREAM_START);
                  rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
                }
              }
            }
            else
              rel_count++;
          }
          //! Send audio data
          status = send_audio_data(fp);

          if(status == RSI_APP_ERR_AUDIO_EOF)
          {
            //! close current and open next song
#if (RSI_AUDIO_DATA_SRC == BIN_FILE) 
            close_audio_file(&fp);
            if(app_song_list_idx==(SONG_LIST_MAX_COUNT - 1))
              app_song_list_idx = 0;
            else
              app_song_list_idx++;
            
            status = open_audio_file(&fp, app_song_list[app_song_list_idx]);
            if(status != RSI_SUCCESS) {
              return 0;
            } 

#ifdef RMOVE_ID3_TAG
            rsi_mp3_remove_id3_tag(fp);
            LOG_PRINT("mp3 data withou id3 tag\n");
#endif
#endif
          }
          else if(status)
            rsi_bt_app_clear_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
          if(TC_ID == TC_CT_PTH_BV_01_C || TC_ID == TC_CT_PTT_BV_01_I || TC_ID == TC_CT_PTT_BV_02_I ){
            status = rsi_bt_avrcp_pause(remote_dev_addr);
            if(status != RSI_SUCCESS){
              break;
            }
            rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
          }
          count++;
          if(TC_ID == TC_CT_PTH_BV_01_C || TC_ID == TC_CT_PTT_BV_01_I || TC_ID == TC_CT_PTT_BV_02_I){
            if(count==50){
              count = 0;
              status =  rsi_bt_avrcp_vol_up(remote_dev_addr);
              sleep(2);
              status =  rsi_bt_avrcp_vol_down(remote_dev_addr);

            }
          }
        }
        break;
     
    case RSI_APP_EVENT_AVRCP_CONN:
        {
          //! avrcp conn event
          
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_CONN);

          if (!(app_state & (1 << A2DP_CONNECTED))){
            if (RSI_APP_CONN_ROLE != SLAVE_ROLE){
              usleep (500000);
              status = rsi_bt_a2dp_connect (remote_dev_addr);
              if(status != RSI_SUCCESS)
              {
                if(status == RSI_APP_ERR_SOCK_DISCONN){
                  rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
                  app_state = 0; 
                }
                else if (status == RSI_APP_ERR_A2DP_CONN_ALRDY_EXISTS){}
                else
                  rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
                LOG_PRINT("a2dp conn resp status 0x%04x\r\n", status);
              }
            }
          }
          else if(app_state & (1 << A2DP_STREAM_OPEN)){
            usleep (500000);
            rsi_bt_app_set_event (RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
            LOG_PRINT("a2dp send audio\n");
          }
          else
            LOG_PRINT("waiting for a2dp config\n");

#ifdef PTS_ABS_VOL_FEAT 
          if (TC_ID == TC_CT_VLH_BI_03_C || TC_ID == TC_CT_NFR_BV_04_C){
            uint8_t resp_abs_vol = 0;
            status = rsi_bt_avrcp_reg_notification (remote_dev_addr, AVRCP_EVENT_VOLUME_CHANGED);
            if (status != 0) {
              LOG_PRINT ("status - 0x%x\r\n", status);
            }
            sleep(5);
            status = rsi_bt_avrcp_set_abs_vol (remote_dev_addr, 0x70, &resp_abs_vol);
            if (status != 0) {
              LOG_PRINT ("status - 0x%x\r\n", status);
            }
            else
              LOG_PRINT ("Set Abs vol : %d% \r\n", (resp_abs_vol * 100) / 0x7f);
          }
          else if (TC_ID == TC_CT_VLH_BI_04_C || TC_ID == TC_CT_VLH_BV_03_C || TC_ID == TC_CT_VLH_BV_01_I){
            sleep(3);
            status = rsi_bt_avrcp_reg_notification (remote_dev_addr, AVRCP_EVENT_VOLUME_CHANGED);
            if (status != 0) {
              LOG_PRINT ("status - 0x%x\r\n", status);
            }
            sleep(3);
          }
          else if (TC_ID == TC_CT_VLH_BV_01_C || TC_ID == TC_CT_VLH_BV_02_I){
            uint8_t resp_abs_vol = 0;

            status = rsi_bt_avrcp_set_abs_vol (remote_dev_addr, 0x60, &resp_abs_vol);
            if (status != 0) {
              LOG_PRINT ("status - 0x%x\r\n", status);
            }
            else
              LOG_PRINT ("Set Abs vol : %d% \r\n", (resp_abs_vol * 100) / 0x7f);
          }
          else
#endif
          {
            status = rsi_bt_avrcp_reg_notification (remote_dev_addr, AVRCP_EVENT_VOLUME_CHANGED);
            if (status != 0) {
              LOG_PRINT ("reg noti cmd status: 0x%x\r\n", status);
              break;
            }
          }

        }
        break;


     case RSI_APP_EVENT_AVRCP_DISCONN:
        {
          //! avrcp disconn event

          //! clear the avrcp disconn event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_DISCONN);
        }
        break;

     case RSI_APP_EVENT_AVRCP_PAUSE:
        {
          notify_val_t  notify = {0};
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_PAUSE);
          LOG_PRINT(" avrcp pause\r\n");
          if ((app_reg_events != 0) && (app_event_id[0] == 1)) {
            notify.player_status = 0;
            rsi_bt_avrcp_notify (remote_dev_addr, 1, &notify);
            app_event_id[0] = 0;
            app_reg_events--;
          }
          if(TC_ID != TC_CT_PTH_BV_01_C && TC_ID != TC_CT_PTT_BV_01_I && TC_ID != TC_CT_PTT_BV_02_I){
            rsi_bt_app_a2dp_pause();
          }
          play_status = 0;
        }
        break;
     case RSI_APP_EVENT_AVRCP_PLAY:
        {
          notify_val_t  notify = {0};
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_PLAY);
          LOG_PRINT(" avrcp play\r\n");
          if ((app_reg_events != 0) && (app_event_id[0] == 1)) {
            notify.player_status = 1;
            rsi_bt_avrcp_notify (remote_dev_addr, 1, &notify);
            app_event_id[0] = 0;
            app_reg_events--;
          }
          rsi_bt_app_a2dp_resume();
          play_status = 1;
        }
        break;

     case RSI_APP_EVENT_AVRCP_STOP:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_STOP);
        } 
        break;
      
      case RSI_APP_EVENT_AVRCP_NEXT:
      case RSI_APP_EVENT_AVRCP_PREVIOUS:
        {
          //! clear the avrcp next/prev receive event.
          if(temp_event_map == RSI_APP_EVENT_AVRCP_NEXT)
          {
            LOG_PRINT(" avrcp next\r\n");
            rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_NEXT);
            if(app_song_list_idx==(SONG_LIST_MAX_COUNT - 1))
              app_song_list_idx = 0;
            else
              app_song_list_idx++;

          }
          else if(temp_event_map == RSI_APP_EVENT_AVRCP_PREVIOUS)
          {
            LOG_PRINT(" avrcp previous\r\n");
            rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_PREVIOUS);
            if(app_song_list_idx==0)
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
          LOG_PRINT("mp3 data withou id3 tag\n");
#endif
#endif

        }
        break;

      case RSI_APP_EVENT_AVRCP_VOL_UP:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_VOL_UP);
        } 
        break;

      case RSI_APP_EVENT_AVRCP_VOL_DOWN:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_VOL_DOWN);
        } 
        break;

      case RSI_APP_EVENT_AVRCP_GET_CAP:
        {
          uint32_t cap[2] = {0};
          uint8_t cap_cnt = 0;

          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_GET_CAP);
          switch (app_cap_type) {
            case AVRCP_CAP_ID_COMPANY_ID:
              cap[cap_cnt++] = 0x1958;
              break;
            case AVRCP_CAP_ID_EVENTS_SUPPORTED:
              cap[cap_cnt++] = AVRCP_EVENT_PLAYBACK_STATUS_CHANGED;
              cap[cap_cnt++] = AVRCP_EVENT_TRACK_CHANGED;
              break;
          }
          rsi_bt_avrcp_cap_resp (remote_dev_addr, app_cap_type, cap_cnt, &cap);
        }
        break;

#if 0       
      case RSI_APP_EVENT_AVRCP_GET_ATTS:
        {
          uint8_t atts[5] = {1};

          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_GET_ATTS);
          rsi_bt_avrcp_att_list_resp (remote_dev_addr, 1, atts);
        }
        break;

      case RSI_APP_EVENT_AVRCP_GET_ATT_VALS:
        {
          uint8_t vals[5] = {0,1};
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_GET_ATT_VALS);
          rsi_bt_avrcp_att_val_list_resp (remote_dev_addr, 2, vals);
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
          rsi_bt_avrcp_set_cur_att_val_resp ((int8_t *)remote_dev_addr, 0);
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
          rsi_bt_avrcp_char_set_status_resp ((int8_t *)remote_dev_addr, 0);
        break;

      case RSI_APP_EVENT_AVRCP_BATT_STATUS:
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_BATT_STATUS);
          rsi_bt_avrcp_batt_status_resp ((int8_t *)remote_dev_addr, 0);
        break;
#endif
      case RSI_APP_EVENT_AVRCP_SONG_ATTS:
        {
          attr_list_t attr_list[BT_AVRCP_MEDIA_ATTR_ID_MAX_COUNT] = {0, {0}};

          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_SONG_ATTS);

          for (ix = 0; ix < app_song_atts.nbr_atts; ix++) {
            attr_list[ix].id = (uint8_t )(app_song_atts.ele_atts[ix]);
            switch (app_song_atts.ele_atts[ix]) {
              case BT_AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE:
#if (RSI_AUDIO_DATA_SRC == BIN_FILE)
                attr_list[ix].attr_len = strlen(app_song_list[app_song_list_idx]);
                memcpy (&attr_list[ix].attr_val[0], app_song_list[app_song_list_idx], attr_list[ix].attr_len);
#if 0
                if ((TC_ID == TC_TG_RCR_BV_02_C) || (TC_ID == TC_TG_RCR_BV_04_C))
                  /* for verifying request continues response feature */
                  attr_list[ix].attr_len = 400; 
#endif
#endif
                break;

              case BT_AVRCP_MEDIA_ATTR_ID_ARTIST_NAME:
                attr_list[ix].attr_len = strlen(SAMPLE_ARTIST_NAME);
                memcpy (attr_list[ix].attr_val, SAMPLE_ARTIST_NAME, attr_list[ix].attr_len);
                break;

              case BT_AVRCP_MEDIA_ATTR_ID_ALBUM_NAME:
                attr_list[ix].attr_len = strlen(SAMPLE_ALBUM_NAME);
                memcpy (attr_list[ix].attr_val, SAMPLE_ALBUM_NAME, attr_list[ix].attr_len);
                break;

              case BT_AVRCP_MEDIA_ATTR_ID_TRACK_NUM:
                rsi_itoa (app_song_list_idx, attr_list[ix].attr_val);
                attr_list[ix].attr_len = strlen(attr_list[ix].attr_val);
                break;

              case BT_AVRCP_MEDIA_ATTR_ID_NUM_OF_TRACK:
                rsi_itoa (SONG_LIST_MAX_COUNT, attr_list[ix].attr_val);
                attr_list[ix].attr_len = strlen(attr_list[ix].attr_val);
                break;

              case BT_AVRCP_MEDIA_ATTR_ID_GENRE:
                attr_list[ix].attr_len = strlen(SAMPLE_GENRE);
                memcpy (attr_list[ix].attr_val, SAMPLE_GENRE, strlen(SAMPLE_GENRE));
                break;

              case BT_AVRCP_MEDIA_ATTR_ID_PLAYING_TIME_MS:
                rsi_itoa (((file_size/ PCM_DATA_LEN_PER_MS) * 1000), (uint8_t *)attr_list[ix].attr_val);
                attr_list[ix].attr_len = strlen(attr_list[ix].attr_val);
                break;
            }
          }
          rsi_bt_avrcp_ele_att_resp (remote_dev_addr, app_song_atts.nbr_atts, attr_list);
        }
        break;

      case RSI_APP_EVENT_AVRCP_PLAY_STATUS:
        {
          uint32_t pos, size;

          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_PLAY_STATUS);
          pos = ftell (fp);
          size = file_size;
          LOG_PRINT ("play status %d\r\n", play_status);
          rsi_bt_avrcp_play_status_resp ((int8_t *)remote_dev_addr, play_status, ((size/ PCM_DATA_LEN_PER_MS) * 1000), ((pos/ PCM_DATA_LEN_PER_MS) * 1000));
        }
        break;

      case RSI_APP_EVENT_AVRCP_REG_EVENT:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_REG_EVENT);

          if(app_event_id[app_reg_events-1] == AVRCP_EVENT_TRACK_CHANGED)
          {
            uint8_t event_data[8] = {0}; 

            set_notify_event_data (event_data);
            notify_val_t  notify = {0};
            rsi_bt_avrcp_reg_notify_resp ((int8_t *)remote_dev_addr,
                AVRCP_EVENT_TRACK_CHANGED, sizeof(event_data), event_data);
            LOG_PRINT ("wait ..\n");
//            sleep (3);
            LOG_PRINT ("sending track change notification \r\n");
            notify.track_num = 0x4321; 
            rsi_bt_avrcp_notify (remote_dev_addr, AVRCP_EVENT_TRACK_CHANGED, &notify);
          }
          else if (app_event_id[app_reg_events-1] == AVRCP_EVENT_VOLUME_CHANGED)
          { 
            uint8_t event_data = 0x60; 

            notify_val_t  notify = {0};
            rsi_bt_avrcp_reg_notify_resp ((int8_t *)remote_dev_addr,
                AVRCP_EVENT_VOLUME_CHANGED, sizeof(event_data), &event_data);

            if (TC_ID == TC_TG_VLH_BV_04_C || TC_ID == TC_TG_VLH_BV_01_I){
              LOG_PRINT ("wait ..\n");
              sleep (3);
              notify.abs_vol = 0x64;
              rsi_bt_avrcp_notify (remote_dev_addr, AVRCP_EVENT_VOLUME_CHANGED, &notify);
            }

          }
        }
        break;

      case RSI_APP_EVENT_AVRCP_NOTIFY:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_NOTIFY);

          status = rsi_bt_avrcp_reg_notification (remote_dev_addr, avrcp_notify.notify_val.eventid);
          if (status != 0) {
            LOG_PRINT ("reg noti cmd status: 0x%x\r\n", status);
            break;
          }

          LOG_PRINT ("notify event id: %d \r\n", avrcp_notify.notify_val.eventid);
          switch (avrcp_notify.notify_val.eventid) {
            case AVRCP_EVENT_VOLUME_CHANGED:
              {
                uint8_t abs_vol_per;
                uint8_t abs_vol = avrcp_notify.notify_val.notify_val.abs_vol;
                abs_vol_per = (abs_vol * 100) / 0x7f;
                LOG_PRINT ("Abs vol: %d% (0x%x)\r\n", abs_vol_per, abs_vol);
              }
              break;
          }
        }
        break;

      case RSI_APP_EVENT_AVRCP_SET_ABS_VOL:
        {
          rsi_bt_app_clear_event (RSI_APP_EVENT_AVRCP_SET_ABS_VOL);
          /* send abs vol resp */
          rsi_bt_avrcp_set_abs_vol_resp (remote_dev_addr, abs_vol);
        }
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
  
  //! Call BT A2DP application
  status = rsi_bt_a2dp_source();

  //! Application main loop
  main_loop();

  return status;
#endif

#ifdef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! RS9113 initialization
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT-BLE Stack
  intialize_bt_stack(STACK_BT_MODE);

  //! OS case
  //! Task created for BT task
  rsi_task_create(rsi_bt_a2dp_source, "bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  rsi_start_os_scheduler();
  return status;
#endif
}
