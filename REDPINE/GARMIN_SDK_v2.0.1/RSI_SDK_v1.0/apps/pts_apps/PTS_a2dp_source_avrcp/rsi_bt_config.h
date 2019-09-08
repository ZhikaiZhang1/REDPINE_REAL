/**
 * @file     rsi_bt_config.h
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

#ifndef RSI_BT_CONFIG_H
#define RSI_BT_CONFIG_H

#include<rsi_data_types.h>

/******************************************************
 * *                      Macros
 * ******************************************************/

//! WLAN + BT Classic coex mode
#define RSI_OPERMODE_WLAN_BT_CLASSIC                5

/*=======================================================================*/
//! Discovery command parameters
/*=======================================================================*/

//! Discovery mode
//! Start the discoverable mode
#define START_DISCOVERY                             0x01
//! Start the limited discoverable mode
#define START_LIMITED_DISCOVERY                     0x02
//! Stop the discoverable mode
#define STOP_DISCOVERY                              0x00


/*=======================================================================*/
//! Connectability command parameters
/*=======================================================================*/

//! Connectable mode
//! Start the connectable mode
#define CONNECTABLE                                  0x01
//! Stop the connectable mode
#define NON_CONNECTABLE                              0x00


/*=======================================================================*/
//!  Authentication command parameters
/*=======================================================================*/

//! Pair mode
//! Enable authentication
//#define RSI_ENABLE                                  0x01
//! Enable authentication
//#define RSI_DISABLE                                 0x00


/*=======================================================================*/
//!  SSP Confirmation command parameters
/*=======================================================================*/

//! Confimation type
//! Accept confirmation
#define ACCEPT                                  0x01
//! Reject confirmation
#define REJECT                                  0x00


/*=======================================================================*/
//!  BT classic profile enable BITS
/*=======================================================================*/
#define RSI_SPP_PROFILE_BIT                         BIT(0)
#define RSI_A2DP_PROFILE_BIT                        BIT(1)
#define RSI_AVRCP_PROFILE_BIT                       BIT(2)
#define RSI_HFP_PROFILE_BIT                         BIT(3)
#define RSI_PBAP_PROFILE_BIT                        BIT(4)

/*=======================================================================*/
//!  SSP Confirmation command parameters
/*=======================================================================*/

//! A2DP Profile Enable
#define A2DP_PROFILE_ENABLE
#define A2DP_SINK_ROLE         0
#define A2DP_SOURCE_ROLE       1
#define A2DP_PROFILE_ROLE      A2DP_SOURCE_ROLE

#define PTS_A2DP_AVRCP_1_3 //PTS_ABS_VOL_FEAT 
//#define PTS_ABS_VOL_FEAT 

#define RSI_PTS_A2DP 1
enum test_case_id_e {
  TC_NORMAL_MODE = 0,

  /* A2DP SRC */
  TC_SRC_AS_BV_01_I,
  TC_SRC_CC_BV_09_I,
  TC_SRC_CC_BV_10_I,
  TC_SRC_REL_BV_01_I,
  TC_SRC_REL_BV_02_I,
  TC_SRC_SDP_BV_01_I,
  TC_SRC_SET_BV_01_I,
  TC_SRC_SET_BV_02_I,
  TC_SRC_SET_BV_03_I,
  TC_SRC_SET_BV_04_I,
  TC_SRC_SET_BV_05_I,
  TC_SRC_SET_BV_06_I,
  TC_SRC_SUS_BV_01_I,
  TC_SRC_SUS_BV_02_I,
  TC_IOPT_CL_A2DP_SRC_SFC_BV_01_I,

  /* AVCTP CT */ //1.5
  TC_CT_NFR_BV_04_C, //need to enable PTS_ABS_VOL_FEAT define

  /* AVCTP TG */
  TC_TG_CCM_BV_01_C,
  TC_TG_CCM_BV_02_C,
  TC_TG_CCM_BV_03_C,
  TC_TG_CCM_BV_04_C,
  TC_TG_NFR_BI_01_C,
  TC_TG_NFR_BI_02_C,
  TC_TG_NFR_BV_03_C,
  
  /* AVDTP SRC ACP */
  TC_SRC_ACP_SIG_SMG_BV_06_C,      //17
  TC_SRC_ACP_SIG_SMG_BV_08_C,
  TC_SRC_ACP_SIG_SMG_BV_10_C,
  TC_SRC_ACP_SIG_SMG_BV_12_C,
  TC_SRC_ACP_SIG_SMG_BV_16_C,
  TC_SRC_ACP_SIG_SMG_BV_18_C,
  TC_SRC_ACP_SIG_SMG_BV_20_C,
  TC_SRC_ACP_SIG_SMG_BV_22_C,
  TC_SRC_ACP_SIG_SMG_BV_24_C,
  TC_SRC_ACP_SIG_SMG_BV_26_C,
//  TC_SRC_ACP_SIG_SMG_BI_02_C,
  TC_SRC_ACP_SIG_SMG_BI_05_C,
  TC_SRC_ACP_SIG_SMG_BI_08_C,
  TC_SRC_ACP_SIG_SMG_BI_11_C,
  TC_SRC_ACP_SIG_SMG_BI_17_C,
  TC_SRC_ACP_SIG_SMG_BI_20_C,
  TC_SRC_ACP_SIG_SMG_BI_23_C,
  TC_SRC_ACP_SIG_SMG_BI_26_C,
  TC_SRC_ACP_SIG_SMG_BI_33_C,
  TC_SRC_ACP_TRA_BTR_BI_01_C,

  /* AVDTP SRC INT */
  TC_SRC_INT_SIG_SMG_BV_05_C,     //35
  TC_SRC_INT_SIG_SMG_BV_07_C,
  TC_SRC_INT_SIG_SMG_BV_09_C,
  TC_SRC_INT_SIG_SMG_BV_11_C,
  TC_SRC_INT_SIG_SMG_BV_15_C,
  TC_SRC_INT_SIG_SMG_BV_17_C,
  TC_SRC_INT_SIG_SMG_BV_19_C,
  TC_SRC_INT_SIG_SMG_BV_21_C,
  TC_SRC_INT_SIG_SMG_BV_23_C,
  TC_SRC_INT_SIG_SMG_BV_25_C,
  TC_SRC_INT_SIG_SMG_BV_28_C,
  TC_SRC_INT_SIG_SMG_BV_31_C,
  TC_SRC_INT_SIG_SMG_BI_30_C,
  TC_SRC_INT_SIG_SMG_BI_35_C,
  TC_SRC_INT_SIG_SMG_BI_36_C,
  TC_SRC_INT_TRA_BTR_BV_01_C,
 
  /* AVRCP CT */
  TC_CT_CEC_BV_01_I,             
  TC_CT_CEC_BV_02_I,             
  TC_CT_CFG_BV_01_C,             
  TC_CT_CRC_BV_01_C,             
  TC_CT_CRC_BV_02_C,             
  TC_CT_ICC_BV_01_C,             
  TC_CT_ICC_BV_02_C,             
  TC_CT_NFY_BV_01_C,             
  TC_CT_PTH_BV_01_C,             
  TC_CT_PTT_BV_01_I,             
  TC_CT_PTT_BV_02_I,        

 
  /* AVRCP TG */
  TC_TG_CEC_BV_01_I,             //47
  TC_TG_CEC_BV_02_I,
  TC_TG_CRC_BV_01_C, 
  TC_TG_CRC_BV_02_C,
  TC_TG_ICC_BV_01_I, 
  TC_TG_ICC_BV_02_I, 
  TC_TG_INV_BI_02_C,
  TC_TG_NFY_BI_01_C, 
  TC_TG_PTT_BV_01_I, 
  TC_TG_PTT_BV_02_I,  


  /* AVRCP CT Role ABS Volume */
  TC_CT_VLH_BI_03_C,
  TC_CT_VLH_BI_04_C,
  TC_CT_VLH_BV_01_C,
  TC_CT_VLH_BV_03_C,
  TC_CT_VLH_BV_01_I,
  TC_CT_VLH_BV_02_I,
  
  /* AVRCP TG Role ABS Volume */
  TC_TG_VLH_BI_01_C,
  TC_TG_VLH_BI_02_C,
  TC_TG_VLH_BV_02_C,
  TC_TG_VLH_BV_04_C,
  TC_TG_VLH_BV_01_I,
  TC_TG_VLH_BV_02_I,

  TC_TG_MAX
} TC_ID;

enum rsi_app_conn_role_e {
  SLAVE_ROLE = 1,
  MASTER_ROLE
} RSI_APP_CONN_ROLE;
enum rsi_app_profile_e {
  BT_AVDTP = 1,
  BT_A2DP
} RSI_PTS_PROFILE;
enum rsi_app_avdtp_role_e {
  ACCEPTOR_ROLE = 1,
  INITIATOR_ROLE
} RSI_APP_AVDTP_ROLE;

enum rsi_app_a2dp_send_audio_e {
  ENABLE_AUDIO = 1,
  DISABLE_AUDIO
} RSI_APP_A2DP_SEND_AUDIO;

enum rsi_app_avrcp_conn_e {
  ENABLE_AVRCP = 1,
  DISABLE_AVRCP
} RSI_APP_AVRCP_CONN;

//! A2DP profile enable -> set the 23 bit in bt_feature_bit_map

//#define A2DP_POWER_SAVE_ENABLE
//#define BT_PWR_CTRL_DISABLE
//#define BT_PWR_CTRL 1
//#define BT_PWR_INX 13

#define PCM_AUDIO              1
#define SBC_AUDIO              2
#define MP3_AUDIO              3
//#define RSI_AUDIO_DATA_TYPE    SBC_AUDIO
#define RSI_AUDIO_DATA_TYPE    PCM_AUDIO

#if(RSI_AUDIO_DATA_TYPE == SBC_AUDIO)
#define BT_HOST_SBC            0
#else
#define BT_HOST_SBC            1
#endif

#define BIN_FILE               1
#define RSI_AUDIO_DATA_SRC     BIN_FILE

/*=======================================================================*/

#define RSI_BT_MAX_PAYLOAD_SIZE              4000 //310 /* Max supported is 200, but 190 gives optimum Tx throughput */
#define BT_BDR_MODE                           0 // 1 - HP chain

#endif
