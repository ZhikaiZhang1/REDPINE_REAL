/**
 * @file    rsi_bt_per_DEMO_28.c
 * @version 0.1
 * @date    xx Feb 2019
 *
 *  Copyright(C) Redpine Signals 2019
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for BT Classic Per Mode validation
 *
 *  @section Description  This application serves as a BT Classic Transmit/ Receive with stats
 *
 */

/**
 * Include files
 * */
#include <rsi_common_app.h>
#if BT_PER
//! BT include file to refer BT APIs
#include<rsi_bt_apis.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_bt_config.h>
#include<rsi_bt.h>
#include <stdio.h>

//! Common include file 
#include<rsi_common_apis.h>

//! application defines 

#define RSI_BT_LOCAL_NAME             "PER"

#define RSI_BT_PER_TRANSMIT_MODE      1
#define RSI_BT_PER_RECEIVE_MODE       2
#define RSI_PER_STATS                 3
#define RSI_CONFIG_PER_MODE            RSI_BT_PER_TRANSMIT_MODE

#define BT_PER_STATS_CMD_ID           0x08
#define BT_TRANSMIT_CMD_ID            0x15
#define BT_RECEIVE_CMD_ID             0x16

#define SEQUENCE_0          0
#define SEQUENCE_1          1
#define SEQUENCE_2          2
#define SEQUENCE_F0         3
#define SEQUENCE_PRBS       4


#define DISABLE                       0
#define ENABLE                        1

#define BR_MODE                       1
#define EDR_MODE                      2
#define EDR_3MBPS_MODE                3

#define SCO_LINK                      0
#define ACL_LINK                      1
#define ESCO_LINK                     2

#define BURST_MODE                     0
#define CONTIUOUS_MODE                 1

#define NO_HOPPING                     0
#define FIXED_HOPPING                  1
#define RANDOM_HOPPING                 2

#define ONBOARD_ANT_SEL                2
#define EXT_ANT_SEL                    3

#define BT_EXTERNAL_RF                0
#define BT_INTERNAL_RF                1

#define NO_CHAIN_SEL                   0
#define WLAN_HP_CHAIN_BIT              0
#define WLAN_LP_CHAIN_BIT              1
#define BT_HP_CHAIN_BIT                2
#define BT_LP_CHAIN_BIT                3


#define PLL_MODE_0                     0
#define PLL_MODE_1                     1

#define LOOP_BACK_MODE_DISABLE         0
#define LOOP_BACK_MODE_ENABLE          1


#define RSI_BT_BD_ADDR_1            0x11111111
#define RSI_BT_BD_ADDR_2            0x1111


#define PACKET_TYPE                   15
#define PACKET_LEN                    1021
#define BT_RX_CHNL_NUM                78
#define BT_TX_CHNL_NUM                78

#define SCRAMBLER_SEED                0

#define PAYLOAD_TYPE                  SEQUENCE_F0
#define BT_TX_POWER                   4
#define RSI_INTER_PKT_GAP             0


//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN            10000

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE 3000

//! BT task priority
#define RSI_BT_TASK_PRIORITY   1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  3000
#endif

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN            10000

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE 3000

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
static rsi_bt_resp_get_local_name_t   local_name = {0};
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static rsi_bt_tx_per_params_t bt_tx_per;
static rsi_bt_rx_per_params_t bt_rx_per;
//static rsi_bt_per_stats_t per_stats;
extern int32_t rsi_bt_per_rx(uint32_t *bt_perrx);
extern int32_t rsi_bt_per_tx(uint32_t *bt_pertx);

/*==============================================*/
/**
 * @fn         rsi_bt_per
 * @brief      Tests the BT Classic Per TX.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the  Classic Per TX.
 */
int32_t rsi_app_task_bt_per(void)
{
  int32_t status = 0;
  uint8_t str_bd_addr[18] = {0};

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(local_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\r\nlocal_bd_address: %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_bd_addr, local_dev_addr));

  //! set the local device name
  status = rsi_bt_set_local_name((int8_t *)RSI_BT_LOCAL_NAME);
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



  if(RSI_CONFIG_PER_MODE == RSI_BT_PER_TRANSMIT_MODE)
  {
    bt_tx_per.cmd_id = BT_TRANSMIT_CMD_ID;
    bt_tx_per.transmit_enable = ENABLE;
    *(uint32_t *)&bt_tx_per.device_Addr[0] = RSI_BT_BD_ADDR_1;
    *(uint16_t *)&bt_tx_per.device_Addr[4] = RSI_BT_BD_ADDR_2;
    *(uint16_t *)&bt_tx_per.pkt_len[0] = PACKET_LEN;
    bt_tx_per.pkt_type = PACKET_TYPE;
    bt_tx_per.br_edr_mode = EDR_3MBPS_MODE;
    bt_tx_per.rx_chnl_in = BT_RX_CHNL_NUM;
    bt_tx_per.tx_chnl_in = BT_TX_CHNL_NUM;
    bt_tx_per.link_type = ACL_LINK;
    bt_tx_per.scrambler_seed = SCRAMBLER_SEED;
    bt_tx_per.payload_type = PAYLOAD_TYPE;
    bt_tx_per.tx_power     = BT_TX_POWER;
    bt_tx_per.transmit_mode = CONTIUOUS_MODE;
    bt_tx_per.hopping_type = NO_HOPPING;
    bt_tx_per.ant_sel = ONBOARD_ANT_SEL;
    bt_tx_per.inter_pkt_gap = RSI_INTER_PKT_GAP;
    bt_tx_per.pll_mode = PLL_MODE_0;
    bt_tx_per.rf_type = BT_INTERNAL_RF;
    bt_tx_per.rf_chain = BT_HP_CHAIN_BIT;
    rsi_bt_per_tx((uint32_t *)&bt_tx_per);
  }
  else if(RSI_CONFIG_PER_MODE == RSI_BT_PER_RECEIVE_MODE)
  {
    bt_rx_per.cmd_id = BT_RECEIVE_CMD_ID;
    bt_rx_per.receive_enable=ENABLE;
    *(uint32_t *)&bt_rx_per.device_Addr[0] = RSI_BT_BD_ADDR_1;
    *(uint16_t *)&bt_rx_per.device_Addr[4] = RSI_BT_BD_ADDR_2;
    bt_rx_per.link_type=ACL_LINK;
    bt_rx_per.pkt_type=PACKET_TYPE;
   *(uint16_t *)&bt_tx_per.pkt_len[0] = PACKET_LEN;   
    bt_rx_per.scrambler_seed=SCRAMBLER_SEED;    
    bt_rx_per.br_edr_mode=EDR_3MBPS_MODE;
    bt_rx_per.rx_chnl_in=BT_RX_CHNL_NUM;
    bt_rx_per.tx_chnl_in=BT_TX_CHNL_NUM;
    bt_rx_per.hopping_type=NO_HOPPING;
    bt_rx_per.ant_sel=ONBOARD_ANT_SEL; 
    bt_rx_per.loop_back_mode=LOOP_BACK_MODE_DISABLE;
    bt_rx_per.pll_mode=PLL_MODE_0; 
    bt_rx_per.rf_type= BT_INTERNAL_RF;
    bt_rx_per.rf_chain= BT_HP_CHAIN_BIT;
    //! start the Receive PER functionality
    rsi_bt_per_rx((uint32_t *)&bt_rx_per);
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
