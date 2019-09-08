/**
 * @file    rsi_ble_per.c
 * @version 0.1
 * @date    24 Nov 2017
 *
 *  Copyright(C) Redpine Signals 2017
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BLE PER Mode.
 *
 *  @section Description  This application serves as a BLE Sample PER Transmit and Receive Mode.
 *
 */

/**
 * Include files
 * */
#include <rsi_common_app.h>
#if BLE_PER
//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_ble.h>
#include<rsi_bt_common.h>

//! Common include file 
#include<rsi_common_apis.h>
//#include "rsi_ble_config_DEMO_32.h"

#include <string.h>


#define RSI_BLE_LOCAL_NAME             "WYZBEE_PERIPHERAL"
#define RSI_SEL_ANTENNA                RSI_SEL_INTERNAL_ANTENNA//RSI_SEL_EXTERNAL_ANTENNA


//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN             10000

#define DISABLE                       0
#define ENABLE                        1

#define RSI_BLE_PER_TRANSMIT_MODE      1
#define RSI_BLE_PER_RECEIVE_MODE       2
#define RSI_PER_STATS                  3

#define RSI_CONFIG_PER_MODE            RSI_BLE_PER_RECEIVE_MODE

#define LE_ONE_MBPS                    1
#define LE_TWO_MBPS                    2
#define LE_125_KBPS_CODED              4
#define LE_500_KBPS_CODED              8


#define DATA_PRBS9                     0x00
#define DATA_FOUR_ONES_FOUR_ZEROES     0x01  
#define DATA_ALT_ONES_AND_ZEROES       0x02
#define DATA_PRSB15                    0x03
#define DATA_ALL_ONES                  0x04
#define DATA_ALL_ZEROES                0x05
#define DATA_FOUR_ZEROES_FOUR_ONES     0x06
#define DATA_ALT_ZEROES_AND_ONES       0x07  

#define LE_ADV_CHNL_TYPE               0
#define LE_DATA_CHNL_TYPE              1

#define BURST_MODE                     0
#define CONTIUOUS_MODE                 1

#define NO_HOPPING                     0
#define FIXED_HOPPING                  1
#define RANDOM_HOPPING                 2

#define BT_PER_STATS_CMD_ID            0x08
#define BLE_TRANSMIT_CMD_ID            0x13
#define BLE_RECEIVE_CMD_ID             0x14

#define BLE_ACCESS_ADDR                0x71764129
#define BLE_TX_PKT_LEN                 255
#define BLE_PHY_RATE                   LE_ONE_MBPS
#define BLE_RX_CHNL_NUM                39
#define BLE_TX_CHNL_NUM                39
#define BLE_TX_POWER_INDEX             31
#define SCRAMBLER_SEED                 0
#define NUM_PKTS                       0
#define RSI_INTER_PKT_GAP              0

#define ONBOARD_ANT_SEL                2
#define EXT_ANT_SEL                    3

#define BLE_EXTERNAL_RF                0
#define BLE_INTERNAL_RF                1

#define NO_CHAIN_SEL                   0
#define WLAN_HP_CHAIN_BIT              0
#define WLAN_LP_CHAIN_BIT              1
#define BT_HP_CHAIN_BIT                2
#define BT_LP_CHAIN_BIT                3

#define PLL_MODE_0                     0
#define PLL_MODE_1                     1

#define LOOP_BACK_MODE_DISABLE         0
#define LOOP_BACK_MODE_ENABLE          1

#define EXT_DATA_LEN_IND               1

#define DUTY_CYCLING_DISABLE           0
#define DUTY_CYCLING_ENABLE            1


#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE  1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY   1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  3000

void rsi_wireless_driver_task(void);

#endif

//! Memory to initialize driver
uint8_t bt_global_buf[BT_GLOBAL_BUFF_LEN] = {0};

//! Application global parameters.
static uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static rsi_ble_per_transmit_t rsi_ble_per_tx;
static rsi_ble_per_receive_t rsi_ble_per_rx;
//static rsi_bt_per_stats_t per_stats;


int32_t rsi_ble_per_transmit (struct rsi_ble_per_transmit_s *rsi_ble_per_tx);

int32_t rsi_ble_per_receive (struct rsi_ble_per_receive_s *rsi_ble_per_rx);


/*==============================================*/
/**
 * @fn         rsi_ble_per
 * @brief      Tests the BLE PER Modes.
 * @param[in]  none
 * @return    none.
 * @section description
 * This function is used to test the different BLE PER Modes.
 */
int32_t rsi_ble_per(void)
{
  int32_t status = 0;

#if 0
#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(bt_global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! Redpine module intialisation 
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! initialize the event map
  rsi_ble_app_init_events ();
#endif

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  if(RSI_CONFIG_PER_MODE == RSI_BLE_PER_TRANSMIT_MODE)
  {
    rsi_ble_per_tx.cmd_ix = BLE_TRANSMIT_CMD_ID;
    rsi_ble_per_tx.transmit_enable = ENABLE;
    *(uint32_t *)&rsi_ble_per_tx.access_addr[0] = BLE_ACCESS_ADDR;
    *(uint16_t *)&rsi_ble_per_tx.pkt_len[0] = BLE_TX_PKT_LEN;
    rsi_ble_per_tx.phy_rate = BLE_PHY_RATE;
    rsi_ble_per_tx.rx_chnl_num = BLE_RX_CHNL_NUM;
    rsi_ble_per_tx.tx_chnl_num = BLE_TX_CHNL_NUM;
    rsi_ble_per_tx.scrambler_seed = SCRAMBLER_SEED;
    rsi_ble_per_tx.payload_type = DATA_FOUR_ZEROES_FOUR_ONES;
    rsi_ble_per_tx.le_chnl_type = LE_DATA_CHNL_TYPE;
    rsi_ble_per_tx.tx_power     = BLE_TX_POWER_INDEX;
    rsi_ble_per_tx.transmit_mode = CONTIUOUS_MODE;
    rsi_ble_per_tx.freq_hop_en = NO_HOPPING;
    rsi_ble_per_tx.ant_sel = ONBOARD_ANT_SEL;
    rsi_ble_per_tx.inter_pkt_gap = RSI_INTER_PKT_GAP;
    rsi_ble_per_tx.pll_mode = PLL_MODE_0;
    rsi_ble_per_tx.rf_type = BLE_INTERNAL_RF;
    rsi_ble_per_tx.rf_chain = BT_LP_CHAIN_BIT;

    //! start the Transmit PER functionality
    rsi_ble_per_transmit(&rsi_ble_per_tx);
  }
  else if(RSI_CONFIG_PER_MODE == RSI_BLE_PER_RECEIVE_MODE)
  {
    rsi_ble_per_rx.cmd_ix = BLE_RECEIVE_CMD_ID;
    rsi_ble_per_rx.receive_enable = ENABLE;
    *(uint32_t *)&rsi_ble_per_rx.access_addr[0] = BLE_ACCESS_ADDR;
    rsi_ble_per_rx.ext_data_len_indication = EXT_DATA_LEN_IND;
    rsi_ble_per_rx.phy_rate = BLE_PHY_RATE;
    rsi_ble_per_rx.rx_chnl_num = BLE_RX_CHNL_NUM;
    rsi_ble_per_rx.tx_chnl_num = BLE_TX_CHNL_NUM;
    rsi_ble_per_rx.scrambler_seed = SCRAMBLER_SEED;
    rsi_ble_per_rx.le_chnl_type = LE_DATA_CHNL_TYPE;
    rsi_ble_per_rx.loop_back_mode = LOOP_BACK_MODE_DISABLE;
    rsi_ble_per_rx.freq_hop_en = NO_HOPPING;
    rsi_ble_per_rx.ant_sel = ONBOARD_ANT_SEL;
    rsi_ble_per_rx.duty_cycling_en = DUTY_CYCLING_DISABLE;
    rsi_ble_per_rx.pll_mode = PLL_MODE_0;
    rsi_ble_per_rx.rf_type = BLE_INTERNAL_RF;
    rsi_ble_per_rx.rf_chain = BT_LP_CHAIN_BIT;

    //! start the Receive PER functionality
    rsi_ble_per_receive(&rsi_ble_per_rx);
  }

  while(1);
#if 0
  {
    status = rsi_bt_per_stats(BT_PER_STATS_CMD_ID,  &per_stats);
    if(status != RSI_SUCCESS)
    {
      return status;
    }
  }
#endif
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
#endif
