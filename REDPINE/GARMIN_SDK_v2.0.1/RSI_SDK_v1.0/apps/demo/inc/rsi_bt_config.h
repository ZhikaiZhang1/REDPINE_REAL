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

#ifndef SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_DEF_H_
#define SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_DEF_H_
#include <rsi_common_app.h>

#if BT_A2DP_SOURCE
#include "rsi_bt_config_DEMO_5.h"
#elif BLE_ADV_BT_SPP_THROUGHPUT
#include "rsi_bt_config_DEMO_8.h"
#elif BT_EIR_SNIFF_SPP_SLAVE
#include "rsi_bt_config_DEMO_7.h"
#elif BT_A2DP_SOURCE_WIFI_HTTP_S_RX
#include "rsi_bt_config_DEMO_18.h"
#elif BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX
#include "rsi_bt_config_DEMO_20.h"
#elif BLE_DUAL_MODE_BT_SPP_SLAVE
#include "rsi_bt_config_DEMO_21.h"
#elif BLE_ADV_BT_A2DP_SOURCE
#include "rsi_bt_config_DEMO_6.h"
#elif BLE_PER
#include "rsi_bt_config_DEMO_31.h"
#elif BT_SNIFF
#include "rsi_bt_config_DEMO_26.h"
#elif BT_PAGE_INQUIRY
#include "rsi_bt_config_DEMO_27.h"
#elif BT_PER
#include "rsi_bt_config_DEMO_28.h"
#elif BT_SPP_RX_TX
#include "rsi_bt_config_DEMO_29.h"
#elif BT_INQUIRY_SCAN
#include "rsi_bt_config_DEMO_32.h"
#elif BT_PAGE_SCAN
#include "rsi_bt_config_DEMO_33.h"
#elif BT_SPP_MASTER_SNIFF
#include "rsi_bt_config_DEMO_34.h"
#elif BT_SPP_SLAVE_SNIFF
#include "rsi_bt_config_DEMO_35.h"
#elif (BT_A2DP_SOURCE_SBC_CODEC)
#include "rsi_bt_config_DEMO_42.h"
#elif ANT_APP
#include "rsi_bt_config_ANT.h"
#elif ANT_APP_POWERSAVE
#include "rsi_bt_config_ANT_APP_PWRSAVE.h"
#elif BLE_CENTRAL_ANT_APP
#include "rsi_bt_config_BLE_CENTRAL_ANT_APP.h"
#elif BLE_PERIPHERAL_ANT_APP
#include "rsi_bt_config_BLE_PERIPHERAL_ANT_APP.h"
#elif ANT_BLE_PERI_BT_A2DP_SRC_APP
#include "rsi_bt_config_ANT_BLE_PERI_A2DP_SRC.h"
#elif ANT_BLE_CENT_BT_A2DP_SRC_APP
#include "rsi_bt_config_ANT_BLE_CENT_A2DP_SRC.h"
#else

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
//! A2DP profile enable -> set the 23 bit in bt_feature_bit_map

/*=======================================================================*/

#define RSI_BT_MAX_PAYLOAD_SIZE               1040 //! BT MTU size changes //310 /* Max supported is 200, but 190 gives optimum Tx throughput */
#define BT_BDR_MODE                           0 // 1 - HP chain
/***********************************************************************************************************************************************/
#endif
#endif /* SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BT_CONFIG_DEF_H_ */
