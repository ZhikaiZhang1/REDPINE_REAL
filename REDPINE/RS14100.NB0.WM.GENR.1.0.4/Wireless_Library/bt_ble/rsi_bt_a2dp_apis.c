/**
 * @file     rsi_bt_a2dp_apis.c
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
 *  @brief : This file contains BT A2DP API's which needs to be called from application
 *
 *  @section Description  This file contains BT A2DP API's called from application
 *
 */
#ifdef RSI_BT_ENABLE
#include "rsi_driver.h" 
#include "rsi_bt.h"
#include "rsi_bt_apis.h"
#include "rsi_bt_config.h"

/* NOTE***********************************************
 * For all the API's device address shall be provided as ASCII string.
 * Ex: "00:12:34:AB:CD:EF"
 ****************************************************/

/*==============================================*/
/**
 * @fn         rsi_bt_a2dp_init
 * @brief      sets profile mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the profile mode.
 */
int32_t rsi_bt_a2dp_init(void)
{ 
  rsi_bt_req_profile_mode_t bt_req_a2dp_init = {RSI_A2DP_PROFILE_BIT};
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_PROFILE_MODE, &bt_req_a2dp_init, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_a2dp_connect
 * @brief      requests A2DP connection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to initiate A2DP connection.
 * */
int32_t rsi_bt_a2dp_connect(uint8_t *remote_dev_addr)
{
  rsi_bt_req_a2dp_connect_t bt_req_a2dp_connect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_a2dp_connect.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_A2DP_CONNECT, &bt_req_a2dp_connect, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_a2dp_disconnect
 * @brief      requests A2DP disconnection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used for A2DP disconnection.
 * */
int32_t rsi_bt_a2dp_disconnect(uint8_t *remote_dev_addr)
{
  rsi_bt_req_a2dp_disconnect_t bt_req_a2dp_disconnect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_a2dp_disconnect.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_A2DP_DISCONNECT, &bt_req_a2dp_disconnect, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_a2dp_send_pcm_mp3_data
 * @brief      requests to send the PCM/MP3 data to BT stack.
 * @param[in]  remote_dev_address, remote device address
 * @param[in]  pcm_data, pcm data buffer.
 * @param[in]  pcm_data_len, pcm data length.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used for send the PCM data to bt stack.
 * */
int32_t rsi_bt_a2dp_send_pcm_mp3_data(uint8_t *remote_dev_addr, uint8_t *pcm_mp3_data, uint16_t pcm_mp3_data_len, uint8_t audio_type)
{
  rsi_bt_req_a2dp_pcm_mp3_data_t bt_req_a2dp_pcm_mp3_pkt = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_a2dp_pcm_mp3_pkt.dev_addr, (int8_t *)remote_dev_addr);
	bt_req_a2dp_pcm_mp3_pkt.pcm_mp3_data_len = RSI_MIN (sizeof (bt_req_a2dp_pcm_mp3_pkt.pcm_mp3_data), pcm_mp3_data_len);
  bt_req_a2dp_pcm_mp3_pkt.audio_type = audio_type;
	memcpy (bt_req_a2dp_pcm_mp3_pkt.pcm_mp3_data, pcm_mp3_data, bt_req_a2dp_pcm_mp3_pkt.pcm_mp3_data_len);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_A2DP_PCM_MP3_DATA, &bt_req_a2dp_pcm_mp3_pkt, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_a2dp_send_sbc_aac_data
 * @brief      requests to send the SBC/AAC data to BT stack.
 * @param[in]  remote_dev_address, remote device address
 * @param[in]  sbc_aac_data, sbc/aac data buffer.
 * @param[in]  sbc_aac_data_len, sbc/aac data length.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used for send the SBC/AAC data to bt stack.
 * */
int32_t rsi_bt_a2dp_send_sbc_aac_data(uint8_t *remote_dev_addr, uint8_t *sbc_aac_data, uint16_t sbc_aac_data_len, uint8_t audio_type)
{
  rsi_bt_req_a2dp_sbc_aac_data_t bt_req_a2dp_sbc_aac_pkt = {{0}};
	
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_a2dp_sbc_aac_pkt.dev_addr, (int8_t *)remote_dev_addr);
	bt_req_a2dp_sbc_aac_pkt.sbc_aac_data_len = RSI_MIN (sizeof (bt_req_a2dp_sbc_aac_pkt.sbc_aac_data), sbc_aac_data_len);
  bt_req_a2dp_sbc_aac_pkt.audio_type = audio_type;
	memcpy (bt_req_a2dp_sbc_aac_pkt.sbc_aac_data, sbc_aac_data, bt_req_a2dp_sbc_aac_pkt.sbc_aac_data_len);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_A2DP_SBC_AAC_DATA, &bt_req_a2dp_sbc_aac_pkt, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_a2dp_burst_mode
 * @brief      to enable/disable a2dp burst mode to BT stack.
 * @param[in]  remote_dev_address 
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used for send the SBC/AAC data to bt stack.
 * */
int32_t rsi_bt_a2dp_burst_mode (uint8_t *remote_dev_addr, uint8_t enable)
{
  rsi_bt_req_a2dp_burst_mode_t bt_req_a2dp_burst_mode = {{0}};
	
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_a2dp_burst_mode.dev_addr, (int8_t *)remote_dev_addr);
  bt_req_a2dp_burst_mode.enable = enable;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_A2DP_BURST_MODE, &bt_req_a2dp_burst_mode, NULL);
}

#endif
