/**
 * @file     rsi_ble_config_DEMO_38.h
 * @version  0.1  
 * @date     22 Feb 2019
 *
 *  Copyright(C) Redpine Signals 2019
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contain definitions and declarations of BLE APIs.
 *
 *  @section Description  This file contains definitions and declarations required to
 *  configure BLE module.
 *
 *
 */
#include "rsi_common_app.h"
#if BLE_PRIVACY

#ifndef RSI_BLE_CONFIG_H
#define RSI_BLE_CONFIG_H

#include<rsi_data_types.h>

//! user defined structure
//LE resolvlist group.
typedef struct rsi_ble_resolvlist_group_s {
    uint8_t     used;
    uint8_t     remote_dev_addr_type;
    uint8_t     remote_dev_addr[18];
    uint8_t     peer_irk[16];
    uint8_t     local_irk[16];
    uint8_t  Identity_addr_type;
    uint8_t  Identity_addr[18];
} rsi_ble_resolvlist_group_t;

//LE resolvlist.
typedef struct rsi_ble_resolve_key_s {
  uint8_t  remote_dev_addr_type;
  uint8_t  remote_dev_addr[18];
  uint8_t  peer_irk[16];
  uint8_t  local_irk[16];
  uint16_t remote_ediv;
  uint8_t  remote_rand[16];
  uint8_t  remote_ltk[16];
  uint8_t  Identity_addr_type;
  uint8_t  Identity_addr[18];
} rsi_ble_resolve_key_t;

//LE resolvlist.
typedef struct rsi_ble_dev_ltk_list_s {
  uint8_t  sc_enable;
  uint8_t  remote_dev_addr_type;
  uint8_t  remote_dev_addr[6];
  uint8_t  peer_irk[16];
  uint8_t  local_irk[16];
  uint16_t remote_ediv;
  uint16_t local_ediv;
  uint8_t  remote_rand[8];
  uint8_t localrand[8];
  uint8_t  remote_ltk[16];
  uint8_t  localltk[16];
  uint8_t  Identity_addr_type;
  uint8_t  Identity_addr[6];
  uint8_t  used;
} rsi_ble_dev_ltk_list_t;

/*****************************************************
 *                 PRE-DEFINED MACROS
 *****************************************************/

//! application event list
#define RSI_APP_EVENT_ADV_REPORT       				0x00
#define  RSI_BLE_CONN_EVENT                         0x01
#define  RSI_BLE_DISCONN_EVENT                      0x02
#define  RSI_BLE_SMP_REQ_EVENT                      0x03
#define  RSI_BLE_SMP_RESP_EVENT                     0x04
#define  RSI_BLE_SMP_PASSKEY_EVENT                  0x05
#define  RSI_BLE_SMP_FAILED_EVENT                   0x06
#define  RSI_BLE_ENCRYPT_STARTED_EVENT              0x07
#define  RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT          0x08
#define  RSI_BLE_SC_PASSKEY_EVENT                   0x09
#define  RSI_BLE_LTK_REQ_EVENT                      0x0A
#define  RSI_BLE_SECURITY_KEYS_EVENT                0x0B
#define  RSI_BLE_ENHANCE_CONNECTED_EVENT            0x0C

//! process type 1-add device to resolvlist, 2-remove device from resolvlist, 3-clear the resolvlist
#define RSI_BLE_ADD_TO_RESOLVE_LIST                  1
#define RSI_BLE_REMOVE_FROM_RESOLVE_LIST             2
#define RSI_BLE_CLEAR_RESOLVE_LIST                   3

#define MITM_REQ                                   0x01
#define RSI_MAX_LIST_SIZE                          0x05

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE                      1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                        1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS                           1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                    2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE                  3000

void rsi_wireless_driver_task(void);

#endif


/******************************************************
 * *                      Macros
 * ******************************************************/
//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME                      "BLE_SIMPLE_PRIVACY"

#define RSI_SLAVE                                   0
#define RSI_MASTER                                  1

//!local device role
#define RSI_DEVICE_ROLE                             RSI_SLAVE

//! local device name
#define RSI_BLE_DEVICE_NAME                        "RSI_BLE_SIMPLE_PRIVACY"

//! local device IO capability
#define  RSI_BLE_SMP_IO_CAPABILITY                  0x00

//! Address type of the device to connect
#define RSI_BLE_REMOTE_ADDR_TYPE                    LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_REMOTE_ADDR                          "00:23:A7:56:77:77"

//!address resolution enable
#define RSI_BLE_SET_RESOLVABLE_PRIV_ADDR_TOUT        120

//! privacy mode 0-Network privacy mode 1-Device privacy mode
#define RSI_BLE_NETWORK_PRIVACY_MODE                0
#define RSI_BLE_DEVICE_PRIVACY_MODE                 1

#define RSI_BLE_PRIVACY_MODE                        RSI_BLE_DEVICE_PRIVACY_MODE

#define RSI_BLE_RESOLVING_LIST_SIZE                 5

#define RSI_BLE_SET_RAND_ADDR                       "00:23:A7:12:34:56"

#define CLEAR_WHITELIST                              0x00
#define ADD_DEVICE_TO_WHITELIST                      0x01
#define DELETE_DEVICE_FROM_WHITELIST                 0x02

#define ALL_PHYS                                     0x00

#define RSI_BLE_DEV_ADDR_RESOLUTION_ENABLE           1

#define RSI_OPERMODE_WLAN_BLE                        13

#define RSI_BLE_MAX_NBR_ATT_REC                      80
#define RSI_BLE_MAX_NBR_ATT_SERV                     10

#define RSI_BLE_MAX_NBR_SLAVES                       3
#define RSI_BLE_MAX_NBR_MASTERS                      1

#define RSI_BLE_GATT_ASYNC_ENABLE                    0

/* Number of BLE notifications */
#define RSI_BLE_NUM_CONN_EVENTS                      20

/* Number of BLE GATT RECORD SIZE IN (n*16 BYTES), eg:(0x40*16)=1024 bytes */
#define RSI_BLE_NUM_REC_BYTES                      0x40


#define ENABLE_POWER_SAVE                            1
#define	PSP_MODE                        RSI_SLEEP_MODE_2        //! Power Save Profile Mode
#define PSP_TYPE                        RSI_MAX_PSP             //! Power Save Profile type

/*=======================================================================*/
//! Advertising command parameters
/*=======================================================================*/

#define RSI_BLE_ADV_TYPE                              UNDIR_CONN
#define RSI_BLE_ADV_FILTER_TYPE                       ALLOW_SCAN_REQ_ANY_CONN_REQ_ANY  
#define RSI_BLE_ADV_DIR_ADDR_TYPE                     LE_PUBLIC_ADDRESS
#define RSI_BLE_ADV_DIR_ADDR                          "00:15:83:6A:64:17"  

#define  RSI_BLE_ADV_INT_MIN                           0x100
#define  RSI_BLE_ADV_INT_MAX                           0x200
#define  RSI_BLE_ADV_CHANNEL_MAP                       0x07 

//!Advertise status
//! Start the advertising process
#define RSI_BLE_START_ADV                             0x01
//! Stop the advertising process
#define RSI_BLE_STOP_ADV                              0x00


//! BLE Tx Power Index On Air
#define RSI_BLE_PWR_INX                               30

//! BLE Active H/w Pwr Features
#define BLE_DISABLE_DUTY_CYCLING                      0
#define BLE_DUTY_CYCLING                              1
#define BLR_DUTY_CYCLING                              2
#define BLE_4X_PWR_SAVE_MODE                          4
#define RSI_BLE_PWR_SAVE_OPTIONS                      BLE_DISABLE_DUTY_CYCLING

//!Advertise types

/* Advertising will be visible(discoverable) to all the devices.
 * Scanning/Connection is also accepted from all devices
 * */
#define UNDIR_CONN                                    0x80

/* Advertising will be visible(discoverable) to the particular device 
 * mentioned in RSI_BLE_ADV_DIR_ADDR only. 
 * Scanning and Connection will be accepted from that device only.
 * */
#define DIR_CONN                                      0x81

/* Advertising will be visible(discoverable) to all the devices.
 * Scanning will be accepted from all the devices.
 * Connection will be not be accepted from any device.
 * */
#define UNDIR_SCAN                                    0x82

/* Advertising will be visible(discoverable) to all the devices.
 * Scanning and Connection will not be accepted from any device
 * */
#define UNDIR_NON_CONN                                0x83

/* Advertising will be visible(discoverable) to the particular device 
 * mentioned in RSI_BLE_ADV_DIR_ADDR only. 
 * Scanning and Connection will be accepted from that device only.
 * */
#define DIR_CONN_LOW_DUTY_CYCLE                       0x84

//!Advertising flags
#define LE_LIMITED_DISCOVERABLE                       0x01
#define LE_GENERAL_DISCOVERABLE                       0x02
#define LE_BR_EDR_NOT_SUPPORTED                       0x04

//!Advertise filters
#define ALLOW_SCAN_REQ_ANY_CONN_REQ_ANY               0x00
#define ALLOW_SCAN_REQ_WHITE_LIST_CONN_REQ_ANY        0x01
#define ALLOW_SCAN_REQ_ANY_CONN_REQ_WHITE_LIST        0x02
#define ALLOW_SCAN_REQ_WHITE_LIST_CONN_REQ_WHITE_LIST 0x03

//! Address types
#define LE_PUBLIC_ADDRESS                             0x00
#define LE_RANDOM_ADDRESS                             0x01
#define LE_RESOLVABLE_PUBLIC_ADDRESS                  0x02
#define LE_RESOLVABLE_RANDOM_ADDRESS                  0x03

/*=======================================================================*/

/*=======================================================================*/
//! Connection parameters
/*=======================================================================*/
#define LE_SCAN_INTERVAL                             0x0100
#define LE_SCAN_WINDOW                               0x0050

#define CONNECTION_INTERVAL_MIN                      0x00A0
#define CONNECTION_INTERVAL_MAX                      0x00A0

#define CONNECTION_LATENCY                           0x0000
#define SUPERVISION_TIMEOUT                          0x07D0 //2000

/*=======================================================================*/

/*=======================================================================*/
//! Scan command parameters
/*=======================================================================*/

#define RSI_BLE_SCAN_TYPE                             SCAN_TYPE_ACTIVE
#define RSI_BLE_SCAN_FILTER_TYPE                      SCAN_FILTER_TYPE_ALL

//!Scan status
#define RSI_BLE_START_SCAN                            0x01
#define RSI_BLE_STOP_SCAN                             0x00

//!Scan types
#define SCAN_TYPE_ACTIVE                              0x01
#define SCAN_TYPE_PASSIVE                             0x00

//!Scan filters
#define SCAN_FILTER_TYPE_ALL                          0x00
#define SCAN_FILTER_TYPE_ONLY_WHITE_LIST              0x01

#define RSI_SEL_INTERNAL_ANTENNA                      0x00
#define RSI_SEL_EXTERNAL_ANTENNA                      0x01

#endif
#endif
