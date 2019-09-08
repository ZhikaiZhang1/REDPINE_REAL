/**
 * @file     rsi_bt_config_DEMO_5.h
 * @version  0.1  
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
 *  @brief : This file contain definitions and declarations of BT APIs.
 *
 *  @section Description  This file contains definitions and declarations required to
 *  configure BT module.
 *
 *
 */
#ifndef SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_BT_A2DP_SOURCE_H_
#define SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_BT_A2DP_SOURCE_H_

#include <rsi_common_app.h>
#if (BT_A2DP_SOURCE)
#include <rsi_bt_common.h>
#include <rsi_data_types.h>
/***********************************************************************************************************************************************/
//! BT_A2DP_SOURCE APP CONFIG defines
/***********************************************************************************************************************************************/

#define RSI_BT_REMOTE_BD_ADDR                       (void *)"B8:D5:0B:9B:D6:B2"      /* SRS-XB10 */

//! Enable this for dual pairing        
#define RSI_BT_DUAL_PAIR_TEST                       0

//! Enable this for AVDTP Statistics
#define RSI_BT_AVDTP_STATS                          0

#if (RSI_BT_DUAL_PAIR_TEST == 1)
    #define RSI_BT_REMOTE_BD_ADDR_2                 (void *)"00:1E:7C:25:E9:4D"      /* PHILLIPS 1 */
    #define RSI_BT_REMOTE_BD_ADDR                   (void *)"00:17:03:01:E1:28"      /* Portornics */
#endif

#define RSI_BT_LOCAL_NAME                           (void *)"A2DP_AVRCP_SOURCE_G"
#define PIN_CODE                                    "0000"

#define A2DP_BURST_MODE                        	    1  //!	0 - Disable, 1 - Enable
#define A2DP_BURST_SIZE				           	    1  //!  Number of PCM/MP3 packets buffered

#define PCM_AUDIO                              	    1
#define SBC_AUDIO                              	    2
#define MP3_AUDIO                              	    3
#define RSI_AUDIO_DATA_TYPE            	            SBC_AUDIO

#define BIN_FILE                       	            1
#define ARRAY                          	            2
#define SD_BIN_FILE                    	            3
#define RSI_AUDIO_DATA_SRC             	            SD_BIN_FILE

/***********************************************************************************************************************************************/
//! avdtp related defines
/***********************************************************************************************************************************************/
#define ACCEPTOR_ROLE                               1              
#define INITIATOR_ROLE                              2              
#define ANY_ROLE                                    3              
#define RSI_APP_AVDTP_ROLE                          INITIATOR_ROLE   //! ACCEPTOR_ROLE

#define BD_ADDR_ARRAY_LEN                           18
#define RSI_LINKKEY_REPLY_SIZE                      16

#define RPS_LINK_KEY_SAVE                           (1)
#if RPS_LINK_KEY_SAVE
#define LK_READ_BUF_SZ                              ( 512 )
#define MAX_LINK_KEYS                               10
typedef uint8_t     dev_address_type[RSI_DEV_ADDR_LEN];
#endif


/***********************************************************************************************************************************************/
//! application events list
/***********************************************************************************************************************************************/

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


//! application events list
enum rsi_app_event_e{
	RSI_APP_EVENT_CONNECTED = 1,
	RSI_APP_EVENT_PINCODE_REQ,
	RSI_APP_EVENT_LINKKEY_SAVE,
	RSI_APP_EVENT_AUTH_COMPLT,
	RSI_APP_EVENT_DISCONNECTED,
	RSI_APP_EVENT_LINKKEY_REQ,
	//! ssp related defines
	RSI_APP_EVENT_PASSKEY_DISPLAY = 10,
	RSI_APP_EVENT_PASSKEY_REQUEST,
	RSI_APP_EVENT_SSP_COMPLETE,
	RSI_APP_EVENT_CONFIRM_REQUEST,
	//! a2dp related defines
	RSI_APP_EVENT_A2DP_CONN,
	RSI_APP_EVENT_A2DP_DISCONN,
	RSI_APP_EVENT_A2DP_OPEN = 17,
	RSI_APP_EVENT_A2DP_START,
	RSI_APP_EVENT_A2DP_SUSPEND,
	RSI_APP_EVENT_A2DP_ABORT,
	RSI_APP_EVENT_A2DP_CLOSE,
	RSI_APP_EVENT_A2DP_SBC_DATA,
	RSI_APP_EVENT_A2DP_PCM_DATA,
	RSI_APP_EVENT_A2DP_CONFIGURE,
	RSI_APP_EVENT_WLAN_PCM_DATA,
	RSI_APP_EVENT_WLAN_SBC_DATA,
};

#define RSI_APP_EVENT_CONNECTED                     1
#define RSI_APP_EVENT_PINCODE_REQ                   2
#define RSI_APP_EVENT_LINKKEY_SAVE                  3
#define RSI_APP_EVENT_AUTH_COMPLT                   4
#define RSI_APP_EVENT_DISCONNECTED                  5
#define RSI_APP_EVENT_LINKKEY_REQ                   6
#define RSI_APP_EVENT_UNBOND_STATUS                 7
#define RSI_APP_EVENT_CONFIRM_REQUEST               8

//! a2dp related defines
#define RSI_APP_EVENT_A2DP_CONFIGURE                9
#define RSI_APP_EVENT_A2DP_OPEN                     10
#define RSI_APP_EVENT_A2DP_CONN                     11
#define RSI_APP_EVENT_A2DP_DISCONN                  12

//! avrcp related defines
#define RSI_APP_EVENT_AVRCP_CONN                    13
#define RSI_APP_EVENT_AVRCP_DISCONN                 14
#define RSI_APP_EVENT_AVRCP_PLAY                    15
#define RSI_APP_EVENT_AVRCP_PAUSE                   16
#define RSI_APP_EVENT_AVRCP_GET_CAP                 17
#define RSI_APP_EVENT_AVRCP_GET_ATTS                18
#define RSI_APP_EVENT_AVRCP_GET_ATT_VALS            19
#define RSI_APP_EVENT_AVRCP_CUR_ATT_VALS            20
#define RSI_APP_EVENT_AVRCP_SET_ATT_VALS            21
#define RSI_APP_EVENT_AVRCP_ATT_TEXT                22
#define RSI_APP_EVENT_AVRCP_ATT_VALS_TEXT           23
#define RSI_APP_EVENT_AVRCP_NEXT		        	24
#define RSI_APP_EVENT_AVRCP_PREVIOUS            	25
//#define RSI_APP_EVENT_AVRCP_CHAR_SET                24
//#define RSI_APP_EVENT_AVRCP_BATT_STATUS             25
#define RSI_APP_EVENT_AVRCP_SONG_ATTS               26
#define RSI_APP_EVENT_AVRCP_PLAY_STATUS             27
#define RSI_APP_EVENT_AVRCP_REG_EVENT               28
#define RSI_APP_EVENT_A2DP_START                    29
#define RSI_APP_EVENT_A2DP_SUSPEND                  30
#define RSI_APP_EVENT_A2DP_MORE_DATA_REQ            31
#define RSI_APP_EVENT_AVDTP_STATS					32


/***********************************************************************************************************************************************/
//! Error codes
/***********************************************************************************************************************************************/
#define RSI_APP_ERR_NONE                            0x0000
#define RSI_APP_ERR_SOCK_DISCONN                    0x4062
#define RSI_APP_ERR_SOCK_TIMEOUT                    0x4063
#define RSI_APP_ERR_SDP_SRV_NOT_FOUND               0x4102
#define RSI_APP_ERR_PAGE_TIMEOUT                    0x4E04
#define RSI_APP_ERR_PIN_MISSING                     0x4E06
#define RSI_APP_ERR_CONN_TIMEOUT                    0x4E08
#define RSI_APP_ERR_ACL_CONN_ALREADY_EXISTS         0x4E0B
#define RSI_APP_ERR_LIMITED_RESOURCE                0x4E0D
#define RSI_APP_ERR_REMOTE_TERMINATED_CONN          0x4E13
#define RSI_APP_ERR_LOCAL_TERMINATED_CONN           0x4E16
#define RSI_APP_ERR_REPEATED_ATTEMPTS               0x4E17
#define RSI_APP_ERR_LMP_RESP_TIMEOUT                0x4E22
#define RSI_APP_ERR_HW_BUFFER_OVERFLOW              0x4057
#define RSI_APP_ERR_A2DP_CONN_ALRDY_EXISTS          0x4511

#define RSI_APP_ERR_AUDIO_EOF                       0x00A0

#define RSI_LINKKEY_REPLY_SIZE                  	16
/***********************************************************************************************************************************************/
//! avrcp related defines
/***********************************************************************************************************************************************/
#define SONG_NAME                                   0x01
#define ARTIST_NAME                                 0x02
#define ALBUM_NAME                                  0x03
#define SONG_NUMBER                                 0x04
#define NUM_OF_SONGS                                0x05
#define SONG_GENRE                                  0x06
#define PLAY_TIME                                   0x07
#define ARTIST_NAME_1                               "BT_AUDIO"
#define ALBUM_NAME_LOCAL                            "Local"
#define GENRE_NAME_TELUGU                           "Telugu"
#define PCM_DATA_LEN_PER_MS                         (44100 * 2 * 2)

/***********************************************************************************************************************************************/
//! User defined data types
/***********************************************************************************************************************************************/

/***********************************************************************************************************************************************/
//! BT SAPI CONFIG DEFINES
/***********************************************************************************************************************************************/

//! WLAN + BT Classic coex mode
#define RSI_OPERMODE_WLAN_BT_CLASSIC                5


/***********************************************************************************************************************************************/
//! Discovery command parameters
/***********************************************************************************************************************************************/

//! Discovery mode
//! Start the discoverable mode
#define START_DISCOVERY                             0x01
//! Start the limited discoverable mode
#define START_LIMITED_DISCOVERY                     0x02
//! Stop the discoverable mode
#define STOP_DISCOVERY                              0x00


/***********************************************************************************************************************************************/
//! Connectability command parameters
/***********************************************************************************************************************************************/

//! Connectable mode
//! Start the connectable mode
#define CONNECTABLE                                 0x01
//! Stop the connectable mode
#define NON_CONNECTABLE                             0x00


/***********************************************************************************************************************************************/
//!  Authentication command parameters
/***********************************************************************************************************************************************/

//! Pair mode
//! Enable authentication
//#define RSI_ENABLE                                0x01
//! Enable authentication
//#define RSI_DISABLE                               0x00


/***********************************************************************************************************************************************/
//!  SSP Confirmation command parameters
/***********************************************************************************************************************************************/

//! Confimation type
//! Accept confirmation
#define ACCEPT                                      0x01
//! Reject confirmation
#define REJECT                                      0x00


/***********************************************************************************************************************************************/
//!  BT classic profile enable BITS
/***********************************************************************************************************************************************/
#define RSI_SPP_PROFILE_BIT                         BIT(0)
#define RSI_A2DP_PROFILE_BIT                        BIT(1)
#define RSI_AVRCP_PROFILE_BIT                       BIT(2)
#define RSI_HFP_PROFILE_BIT                         BIT(3)
#define RSI_PBAP_PROFILE_BIT                        BIT(4)

/***********************************************************************************************************************************************/
//!  SSP Confirmation command parameters
/***********************************************************************************************************************************************/

//! A2DP Profile Enable
#define A2DP_PROFILE_ENABLE
#define A2DP_SINK_ROLE                              0
#define A2DP_SOURCE_ROLE                            1
#define A2DP_PROFILE_ROLE                           A2DP_SOURCE_ROLE
//! A2DP profile enable -> set the 23 bit in bt_feature_bit_map

/***********************************************************************************************************************************************/

#define RSI_BT_MAX_PAYLOAD_SIZE                     1040 //! BT MTU size changes //310 /* Max supported is 200, but 190 gives optimum Tx throughput */
#define BT_BDR_MODE                                 0 // 1 - HP chain
/***********************************************************************************************************************************************/
#endif
#endif
