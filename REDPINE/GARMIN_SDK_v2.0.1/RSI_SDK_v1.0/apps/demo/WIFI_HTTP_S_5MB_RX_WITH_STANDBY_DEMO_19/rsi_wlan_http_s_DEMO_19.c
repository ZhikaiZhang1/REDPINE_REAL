/**
 * @file    rsi_wlan_http_s_DEMO_19.c
 * @version 0.1
 * @date    29 Nov 2018
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for HTTP/S client socket
 *
 *  @section Description  This file contains example application for HTTP/S client socket
 *
 *
 */
/**
 * Include files
 * */
#include <rsi_common_app.h>
#if WIFI_HTTP_S_5MB_RX_WITH_STANDBY
#include "stdlib.h"
#include "rsi_driver.h"
#include "rsi_http_client.h"
#include "ff.h"
#include "diskio.h"
#include "math.h"
#include "fsl_common.h"
#include "http_client.h"
#if SSL
//! Include SSL CA certificate
#include "GRootCA.pem"
#endif


//! application control block
rsi_wlan_app_cb_t rsi_wlan_app_cb;

//! client socket id
int32_t client_socket;

//! server and client ip addresses
struct rsi_sockaddr_in server_addr, client_addr;

char *httpreq = NULL;
struct http_response *hresp = NULL;
uint8_t RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;


//! Throughput parameters
#if THROUGHPUT_EN
uint32_t pkts = 0;
uint64_t num_bits = 0;
uint64_t total_bits = 0;
uint32_t xfer_time;
uint32_t total_time = 0;
uint32_t t_start = 0;
uint32_t t_end;
float throughput;
float throughput_mbps;
#endif

#if !SD_DEMO
//! circular buffer parameters
app_data_buff_t cbuff;
//! SD Card parameters
sd_data_buff_t sd_buff;
#endif

//! SD Card parameters
UINT bytesWritten;
FIL g_fileObject_n;   /* File object */

volatile uint8_t data_recvd = 0;
volatile uint8_t socket_closed = 0;
volatile uint64_t num_bytes = 0;
uint8_t stop_download = 0;
uint8_t total_data_received = 0;
uint8_t count = 1;
uint8_t http_resp_ok = 0;

//sdram index
volatile uint32_t sdram_idx = 0;

volatile uint32_t cnt = 0;
volatile uint32_t data_processed = 0;
volatile uint32_t wlan_pkt_missed = 0;

//! extern variables
extern struct parsed_url *purl;
extern uint8_t power_save_given;
extern uint16_t http_body_len;
extern uint8_t *sdram;


#if !SD_DEMO
/*==============================================*/
/**
 * @fn
 * @brief      This function is used to
 * @param[in]
 * @return
 * @section description
 * This function is used to allocate free data packet from given pool,
 *
 */
//void buff_init(app_data_buff_t cbuff)
void buff_init()
{
	cbuff.head = 0;
	cbuff.tail = 0;
	cbuff.len = 0;
	cbuff.size = sizeof(cbuff.buff) -1;
	memset(cbuff.buff, 0, sizeof(cbuff.buff));
}
/*==============================================*/
/**
 * @fn
 * @brief      This function is used to
 * @param[in]
 * @return
 * @section description
 * This function is used to allocate free data packet from given pool,
 *
 */
static volatile uint8_t full = 0;

int8_t add_cBuff(uint8_t *buffer, uint32_t length)
{

	//! check for buffer full
	if ((cbuff.head == cbuff.tail) && full)
	{
		LOG_PRINT("Buffer full!!!\n");
		return -1;
	}

	if((cbuff.head < cbuff.tail) && ((cbuff.head + length) >= (cbuff.tail-cbuff.head)))
	{
		LOG_PRINT("Buffer full!!!\n");
		full = 1;
		return -1;
	}


	//! check for cBuff end and wrap around
	if ((cbuff.head + length) >= cbuff.size)
	{
		uint32_t trail_len = cbuff.size-cbuff.head;
		memcpy(&cbuff.buff[cbuff.head], buffer, trail_len);
		cbuff.head = 0;
		memcpy(&cbuff.buff[cbuff.head], buffer+trail_len, length - trail_len);
		cbuff.head += length - trail_len;
		cbuff.len += length;
	}
	else
	{
		//! copy data and move head to next data offset
		memcpy(&cbuff.buff[cbuff.head], buffer, length);
		cbuff.head += length;
		cbuff.len += length;				//! Check for len computation
	}

	//! if head  == tail, buffer is full
	if (cbuff.head == cbuff.tail)
			full = 1;

#if 1//!THROUGHPUT_EN
	//! pkt(s) in the queue, set pkt sweep event
	//if((cbuff.len) >= SD_WRITE_SZ)
		rsi_set_event(RSI_PKT_SWEEP_EVENT);
#endif

	return 0;
}
/*==============================================*/
/**
 * @fn
 * @brief      This function is used to
 * @param[in]
 * @return
 * @section description
 * This function is used to allocate free data packet from given pool,
 *
 */
int8_t get_cBuff(sd_data_buff_t *data_buff)
{
	uint32_t size = 0;

	//! if the head == tail, buff empty
	if ((cbuff.head == cbuff.tail) && !full)
	{
		//LOG_PRINT("CBuff Empty!!!\n");
		return -1;
	}

	//! determine number of bytes to read from cBuff
	if(cbuff.len >= SD_WRITE_SZ)
		size = SD_WRITE_SZ;
	else
	{
		if(socket_closed)
			size = cbuff.len;
		else
			return -1;
	}

	if(cbuff.tail + size >= cbuff.size)  //if (cbuff.head < cbuff.tail)
	{
		uint32_t g_len = cbuff.size - cbuff.tail;
		memcpy(data_buff->buff, &cbuff.buff[cbuff.tail], g_len);
		cbuff.tail = 0;
		memcpy(data_buff->buff+g_len, &cbuff.buff[cbuff.tail], size-g_len);
		cbuff.tail += size-g_len;
		data_buff->len = size;
		cbuff.len -= size;
	}
	else //! if(cbuff.head > cbuff.tail)
	{
		memcpy(data_buff->buff, &cbuff.buff[cbuff.tail], size);
		data_buff->len = size;
		//! move tail to next offset.
		cbuff.tail += size;
		cbuff.len -= size;
	}

	full = 0;

	return 0;
}
#endif

void rsi_wlan_app_callbacks_init(void)
{
  //! Initialze join fail call back
  rsi_wlan_register_callbacks(RSI_JOIN_FAIL_CB, rsi_join_fail_handler);

  //! Initialze ip renewal fail call back
  rsi_wlan_register_callbacks(RSI_IP_FAIL_CB, rsi_ip_renewal_fail_handler);

  //! Initialze remote terminate call back
  rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler);

  //! Initialze ip change notify call back
  rsi_wlan_register_callbacks(RSI_IP_CHANGE_NOTIFY_CB, rsi_ip_change_notify_handler);

  //! Initialze ip change notify call back
  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, rsi_stations_connect_notify_handler);

  //! Initialze ip change notify call back
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, rsi_stations_disconnect_notify_handler);
}

#if !THROUGHPUT_EN
//! Create a new file on SD card
int8_t createFile_sd()
{
	FRESULT error;

	PRINTF("\r\nCreate a file in that directory......\r\n");

	//! Create a file on SD card
	error = f_open(&g_fileObject_n, _T(AUDIO_FILE_NAME), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
	if (error)
	{
		if (error == FR_EXIST)
		{
			PRINTF("File exists.\r\n");
			// Open file and skip writing
			error = f_open(&g_fileObject_n, _T(AUDIO_FILE_NAME), (FA_WRITE | FA_READ | FA_CREATE_NEW));
			if(error)
			{
				PRINTF("Open existing file failed??\r\n");
			}
		}
		else
		{
			PRINTF("Open file failed.\r\n");
			return -1;
		}
		return -1;
	}

	return 0;
}
#endif

//! Call back for Socket async
void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{

	num_bytes += length;

#if !THROUGHPUT_EN
#if SD_DEMO
	memcpy( (uint8_t *)(sdram + sdram_idx),buffer, length);
	sdram_idx += length;
#else
	//Copy to linear buffer
	if(add_cBuff(buffer, length) < 0)
	{
		wlan_pkt_missed++;
		//LOG_PRINT("Data dropped!\n");
	}
#endif
#endif
}

void  rsi_app_task_wifi_https_rx_standby(void)
{
	int32_t		status = RSI_SUCCESS;
	uint8_t 	power_saved = 0;
#if !THROUGHPUT_EN
	int8_t 		ret = 0;
	FRESULT 	error;
#endif
#if !(DHCP_MODE)
	uint32_t    ip_addr      = DEVICE_IP;
	uint32_t    network_mask = NETMASK;
	uint32_t    gateway      = GATEWAY;
#endif
#ifdef RSI_WITH_OS
	//! Redpine module initialization
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	  //! WiSeConnect initialization
	  status = rsi_wireless_init(RSI_WLAN_MODE, RSI_COEX_MODE);
	  if(status != RSI_SUCCESS)
	  {
	    return status;
	  }

	  //! Send Feature frame
	  status = rsi_send_feature_frame();
	  if(status != RSI_SUCCESS)
	  {
		  return status;
	  }
while(1)	  
#endif
{
	switch(rsi_wlan_app_cb.state)
	{
	case RSI_WLAN_INITIAL_STATE:
	{
		//! register call backs
		rsi_wlan_app_callbacks_init();

		//! update wlan application state
		rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;
	}
	case RSI_WLAN_UNCONNECTED_STATE:
	{
		//! Connect to an Access point
		status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("wlan connection failed\n");
			break;
		}
		else
		{
			//! update wlan application state
			rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;
			LOG_PRINT("wlan connected state\n");
		}
	}
	case RSI_WLAN_CONNECTED_STATE:
	{
		//! Configure IP
#if DHCP_MODE
		status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP,0, 0, 0, NULL, 0,0);
#else
		status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("IP Config failed\n");
			break;
		}
		else
		{
			stop_download = 0;
			//! update wlan application state
			rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;

			LOG_PRINT("wlan ipconfig done state\n");

			//! build HTTP Request
			LOG_PRINT("Preparing HTTP Request\n");

			httpreq = build_http_get_req((char *)HTTP_URL, NULL);
			if(httpreq == NULL)
			{
				LOG_PRINT("http_get request build failed\n");
			}

#if !SD_DEMO
			//! Circular buffer initialization
			buff_init();
#endif

#if ENABLE_POWER_SAVE
			//!Broadcast filter
			status = rsi_wlan_filter_broadcast(5000, 1, 1);
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("Filter broadcast failure\n");
				return;
			}
			if(!power_saved)
			{
				status = rsi_wlan_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
				if(status != RSI_SUCCESS)
				{
					break;
				}
				power_saved = 1;
			}
#endif
		}
	}
	case RSI_WLAN_IPCONFIG_DONE_STATE:
	{
		if(stop_download)
			break;

		if(data_recvd)
		{
			RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;
			LOG_PRINT("HTTP/TCP client closed\n");

			//! Clear data receive flag
			data_recvd = 0;

#if 0
#if !THROUGHPUT_EN
			error = f_close(&g_fileObject_n);
			if (error)
			{
				PRINTF("file close failed. \r\n");
				return;
			}
#endif
#endif
			PRINTF("Going into sleep mode for 5 mins \r\n");
			//! wait for 5min
			rsi_delay_ms((5*60*1000) - (t_end - t_start));

			PRINTF("Back from sleep mode\r\n");
			PRINTF("Starting download...\r\n");
			t_start = 0;
			t_end = 0;

#if !CONTINUOUS_HTTP_DOWNLOAD
			stop_download = 1;
			break;
#endif
		}

		data_processed = 0;
		num_bytes = 0;
		cnt++;

#if (SSL && LOAD_CERTIFICATE)
		status = rsi_app_load_ssl_cert();
		if(status != RSI_SUCCESS)
		{
			return;
		}
#endif

	//! Connect to TCP/SSL Server
#if HIGH_PERFORMANCE_ENABLE
        status = rsi_socket_config();
		if(status < 0)
		{
			LOG_PRINT("high-performance socket config failed\n");
			status = rsi_wlan_get_status();
			return;
		}
#endif
		//! Create socket
		status = rsi_app_wlan_socket_create();
		if(status != RSI_SUCCESS)
		{
			return;
		}
		else
		{
#if !THROUGHPUT_EN
			//! Create a file on SD card to store Audio File
			status = createFile_sd();
			if(status < 0)
			{
				LOG_PRINT("File Open on SD Card failed\n");
				return;
			}
#endif
			//! update wlan application state
			rsi_wlan_app_cb.state = RSI_WLAN_SOCKET_CONNECTED_STATE;
			LOG_PRINT("wlan socket connected state\n");
		}


	}
	case RSI_WLAN_SOCKET_CONNECTED_STATE:
	{
		//send HTTP request
		status = rsi_send(client_socket, (const int8_t *) httpreq, strlen(httpreq), 0);
		if(status < 0)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(client_socket, 0);
			LOG_PRINT("send failed\n");
			rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
			return;
		}

		socket_closed = 0;
		sdram_idx = 0;
		http_resp_ok = 0;

#if THROUGHPUT_EN
		/* Start counting */
		t_start = rsi_hal_gettickcount();
#else //! !THROUGHPUT_EN
		RSI_HTTP_STATE = RSI_HTTP_RECV_STATUS;
#endif

		rsi_wlan_app_cb.state = RSI_WLAN_DATA_RECEIVE_STATE;
		LOG_PRINT("wlan data receive state\n");

		break;
	}
	case RSI_WLAN_DATA_RECEIVE_STATE:
	{
#if !THROUGHPUT_EN
#if SD_DEMO
		if(total_data_received)
#endif
		ret = rsi_app_http_parse();
		if(ret<0){
			LOG_PRINT("HTTP parsing failed\n");
			return;
		}
#endif
		break;
	}
	case RSI_SD_WRITE_STATE:
	{
#if !THROUGHPUT_EN
		uint32_t bytes_write;
		//! write from SDRAM to SDCard
		if(sdram_idx >= data_processed)
		{
			//rsi_bt_app_unmask_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);
			//rsi_bt_app_set_event(RSI_APP_EVENT_A2DP_MORE_DATA_REQ);

			error = f_close(&g_fileObject_n);
			if (error)
			{
				PRINTF("file close failed. \r\n");
				return;
			}

			LOG_PRINT("SD WRITE Complete!!!\n");

			sdram_idx = 0;

			rsi_wlan_app_cb.state = RSI_WLAN_DEMO_COMPLETE_STATE;
		}
		else
		{
			if((data_processed - sdram_idx) > MAX_DATA_TO_WRITE_INTO_SDCARD)
				bytes_write = MAX_DATA_TO_WRITE_INTO_SDCARD;
			else
				bytes_write = (data_processed - sdram_idx);

			error = f_write(&g_fileObject_n, (sdram + sdram_idx), bytes_write, &bytesWritten);
			if(error != FR_OK)
			{
				PRINTF("Write file failed. \r\n");
			}
			sdram_idx += bytesWritten;
			//LOG_PRINT("\n bytes to write : %d, bytes written : %d \n",bytes_write,bytesWritten);
		}
#endif
		break;
	}
	case RSI_WLAN_DEMO_COMPLETE_STATE:
	{
#if 0
		uint8_t usr_ip[20];

		ret = DbgConsole_Scanf(usr_ip);

		//! Do Nothing or wait for user i/p to re-start WLAN
#if 0
		if(SD_DEMO && INITIATE_DOWNLOAD)
		{
			rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
		}
#if ENABLE_POWER_SAVE
		if(RESUME_DOWNLOAD)
			rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;
#endif
#endif
#endif
		LOG_PRINT("DEMO Complete!!!\n");
		while(1);
		break;
	}
	default:
		break;
	}

}
}
#if !THROUGHPUT_EN
#if !SD_DEMO
void rsi_pkt_sweep_event_handler(void)
{
	FRESULT 	error;

	if(get_cBuff(&sd_buff) < 0)
	{
		if(socket_closed && ((cbuff.head == cbuff.tail) && !full))
		{
			sdram_idx = 0;
			total_data_received = 1;
			rsi_clear_event(RSI_PKT_SWEEP_EVENT);
			RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;
			return -1;
		}
		return -1;
	}

	if(RSI_HTTP_STATE != RSI_HTTP_RECV_BODY)
	{
		hresp = parse_http_resp((char*)sd_buff.buff);
		if(hresp == NULL)
		{
			LOG_PRINT("HTTP GET response parsing failed\n");
			LOG_PRINT("Socket shutdown\n");
			rsi_shutdown(client_socket, 0);
			rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
			http_response_free(hresp);
			RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;
			error = f_close(&g_fileObject_n);
			if (error)
			{
				PRINTF("file close failed. \r\n");
				return -1;
			}
			return -1;
		}
	}

	switch(RSI_HTTP_STATE)
	{
	case RSI_HTTP_RECV_STATUS:
	{

		if(hresp->status_code_int == 200)
		{
			RSI_HTTP_STATE = RSI_HTTP_RECV_HEADERS;
		}
		else
		{
			LOG_PRINT("HTTP Response: %d\n",hresp->status_code_int);
			LOG_PRINT("HTTP GET response status failed\n");
			rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
			http_response_free(hresp);
			LOG_PRINT("Socket shutdown\n");
			rsi_shutdown(client_socket, 0);
			RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;
			error = f_close(&g_fileObject_n);
			if (error)
			{
				PRINTF("file close failed. \r\n");
				return -1;
			}
			return -1;
		}
	}
	case RSI_HTTP_RECV_HEADERS:
	{
		if(hresp->response_headers != NULL)
		{
			//! Handling for HTTP Body received with resp Headers
			if(http_body_len)
			{
				//memcpy( (uint8_t *)(sdram + sdram_idx),(uint8_t *)strstr((char *)sd_buff.buff, "\r\n\r\n")+strlen("\r\n\r\n"), http_body_len);
				//sdram_idx += http_body_len;
				//data_processed += http_body_len;
				error = f_write(&g_fileObject_n, sd_buff.buff+(sd_buff.len - http_body_len), http_body_len, &bytesWritten);
				if(error != FR_OK)
				{
					PRINTF("Write file failed. \r\n");
				}
				data_processed += bytesWritten;
				http_response_free(hresp);
				LOG_PRINT("HTTP GET response body\n");
				RSI_HTTP_STATE = RSI_HTTP_RECV_BODY;
				break;
			}
			else
			{
				http_response_free(hresp);
				break;
			}
		}
		else
		{
			http_response_free(hresp);
			break;
		}
	}
	case RSI_HTTP_RECV_BODY:
	{
		//! write to SD Card
		//memcpy((uint8_t *)(sdram + sdram_idx),sd_buff.buff, sd_buff.len);
		//sdram_idx += sd_buff.len;
		//data_processed += sd_buff.len;
		error = f_write(&g_fileObject_n, sd_buff.buff, sd_buff.len, &bytesWritten);
		if(error != FR_OK)
		{
			PRINTF("Write file failed. \r\n");
		}
		data_processed += bytesWritten;
		break;
	}
	default:
	{
		if(data_recvd)
		{
			error = f_close(&g_fileObject_n);
			if (error)
			{
				PRINTF("file close failed. \r\n");
				return -1;
			}

			RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;
		}
	}
	break;
	}
}
#endif
#endif

#if SSL
int32_t rsi_app_load_ssl_cert()
{
	int32_t	status = RSI_SUCCESS;

	//! erase existing certificate
	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, NULL ,0);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("CA cert erase failed\n");
		return status;
	}

	//! Load SSL CA certificate
	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, GRootCA, (sizeof(GRootCA)-1));
	//status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, AmazonRootCA1, (sizeof(AmazonRootCA1)-1));
	//status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, COMODOECCCert, (sizeof(COMODOECCCert)-1));
	//status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, DSTRootCA, (sizeof(DSTRootCA)-1));
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("CA cert load failed\n");
		return status;
	}
}
#endif

int32_t rsi_app_wlan_socket_create()
{
	int32_t	status = RSI_SUCCESS;

#if SOCKET_ASYNC_FEATURE
#if SSL
		client_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 1, socket_async_recive);
#else
		client_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 0, socket_async_recive);
#endif
#else
		client_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
#endif
		if(client_socket < 0)
		{
			LOG_PRINT("socket open failed\n");
			status = rsi_wlan_get_status();
			return status;
		}

		LOG_PRINT("socket create\n");

		//! Memset client structruersi_wlan_app_task
		memset(&client_addr, 0, sizeof(client_addr));

		//! Set family type
		client_addr.sin_family= AF_INET;

		//! Set local port number
		client_addr.sin_port = htons(DEVICE_PORT);

		LOG_PRINT("socket bind\n");

		//! Bind socket
		status = rsi_bind(client_socket, (struct rsi_sockaddr *) &client_addr, sizeof(client_addr));
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("bind failed\n");
			rsi_shutdown(client_socket, 0);
			return status;
		}

		//! Reset server structure
		memset(&server_addr, 0, sizeof(server_addr));

		//! Set server address family
		server_addr.sin_family = AF_INET;

		//! Set server port number, using htons function to use proper byte order
		server_addr.sin_port = htons(atoi(purl->port));

		server_addr.sin_addr.s_addr = purl->ip;

		LOG_PRINT("socket connect\n");

		//! Connect to server socket
		status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
		if(status != RSI_SUCCESS)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(client_socket, 0);
			LOG_PRINT("socket connect failed\n");
			return status;
		}

		return status;
}
int32_t rsi_app_wlan_data_recvd()
{
	int32_t	status = RSI_SUCCESS;

	if(data_recvd)
	{
		//! Clear data receive flag
		data_recvd = 0;

		sdram_idx = 0;
		total_data_received = 1;
		data_processed = num_bytes;

		status = rsi_shutdown(client_socket, 0);
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("WLAN shutdown failed\n");
			return status;
		}

		LOG_PRINT("HTTP Client closed!!!\n");
		rsi_wlan_app_cb.state = RSI_WLAN_DATA_RECEIVE_STATE;

#if !CONTINUOUS_HTTP_DOWNLOAD
#endif

#if ENABLE_POWER_SAVE
		status = rsi_wlan_disconnect();
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("WLAN Disconnect failed\n");
			return status;
		}

		free(httpreq);
		parsed_url_free(purl);

		status = rsi_wlan_power_save_profile(RSI_SLEEP_MODE_8, PSP_TYPE);
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("wlan powersave failed\n");
			return status;
		}
		status = rsi_bt_power_save_profile(RSI_SLEEP_MODE_8, PSP_TYPE);
		if (status != RSI_SUCCESS)
		{
			LOG_PRINT("wlan powersave failed\n");
			return status;
		}
		LOG_PRINT("Powersave Mode ON\n");
#endif


#if THROUGHPUT_EN
		//! num of bits
		num_bits = num_bytes * 8;

		//! data transfer time
		xfer_time = t_end - t_start;

		throughput = ((float)(num_bits)/xfer_time);

		LOG_PRINT("Time taken in msec: %d\n", xfer_time);

		LOG_PRINT("Throughput: %d kbps \r\n", (uint32_t) (num_bits / xfer_time));
#endif

#if !CONTINUOUS_HTTP_DOWNLOAD
		stop_download = 1;
		return -1;
#endif
	}

	return status;
}

int8_t rsi_app_http_parse()
{
	FRESULT 	error;

#if !SD_DEMO
	if(get_cBuff(&sd_buff) < 0)
	{
		if(socket_closed && ((cbuff.head == cbuff.tail) && !full))
		{
			sdram_idx = 0;
			total_data_received = 1;

			RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;
			return -1;
		}
		return -1;
	}
#endif
	if(RSI_HTTP_STATE != RSI_HTTP_RECV_BODY)
	{
#if !SD_DEMO
		hresp = parse_http_resp((char*)sd_buff.buff);
#else
		hresp = parse_http_resp((char*)(sdram + sdram_idx));
#endif
		if(hresp == NULL)
		{
			LOG_PRINT("HTTP GET response parsing failed\n");
			LOG_PRINT("Socket shutdown\n");
			rsi_shutdown(client_socket, 0);
			rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
			http_response_free(hresp);
			RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;
			error = f_close(&g_fileObject_n);
			if (error)
			{
				PRINTF("file close failed. \r\n");
				return -1;
			}
			return -1;
		}
	}

	switch(RSI_HTTP_STATE)
	{
	case RSI_HTTP_RECV_STATUS:
	{

		if(hresp->status_code_int == 200)
		{
			RSI_HTTP_STATE = RSI_HTTP_RECV_HEADERS;
			LOG_PRINT("HTTP response success\n");
			http_resp_ok = 1;
		}
		else
		{
			LOG_PRINT("HTTP Response: %d\n",hresp->status_code_int);
			LOG_PRINT("HTTP GET response status failed\n");
			rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
			http_response_free(hresp);
			LOG_PRINT("Socket shutdown\n");
			rsi_shutdown(client_socket, 0);
			RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;
			error = f_close(&g_fileObject_n);
			if (error)
			{
				PRINTF("file close failed. \r\n");
				return -1;
			}
			return -1;
		}
	}
	case RSI_HTTP_RECV_HEADERS:
	{
		if(hresp->response_headers != NULL)
		{
			//! Handling for HTTP Body received with resp Headers
			if(http_body_len)
			{
				//memcpy( (uint8_t *)(sdram + sdram_idx),(uint8_t *)strstr((char *)sd_buff.buff, "\r\n\r\n")+strlen("\r\n\r\n"), http_body_len);
				//sdram_idx += http_body_len;
				//data_processed += http_body_len;
#if !SD_DEMO
				error = f_write(&g_fileObject_n, sd_buff.buff+(sd_buff.len - http_body_len), http_body_len, &bytesWritten);
				if(error != FR_OK)
				{
					PRINTF("Write file failed. \r\n");
				}
				data_processed += bytesWritten;
#endif
				http_response_free(hresp);
				//LOG_PRINT("HTTP GET response body\n");
				RSI_HTTP_STATE = RSI_HTTP_RECV_BODY;
				break;
			}
			else
			{
				http_response_free(hresp);
				break;
			}
		}
		else
		{
			http_response_free(hresp);
			break;
		}
	}
	case RSI_HTTP_RECV_BODY:
	{
		//! write to SD Card
		//memcpy((uint8_t *)(sdram + sdram_idx),sd_buff.buff, sd_buff.len);
		//sdram_idx += sd_buff.len;
		//data_processed += sd_buff.len;
#if !SD_DEMO
		error = f_write(&g_fileObject_n, sd_buff.buff, sd_buff.len, &bytesWritten);
		if(error != FR_OK)
		{
			PRINTF("Write file failed. \r\n");
		}
		data_processed += bytesWritten;
#endif
		break;
	}
	default:
	{
		if(data_recvd)
		{
			error = f_close(&g_fileObject_n);
			if (error)
			{
				PRINTF("file close failed. \r\n");
				return -1;
			}

			RSI_HTTP_STATE = RSI_HTTP_RECV_NONE;
		}
	}
	break;
	}

	return 0;
}
//! callback functions
void rsi_app_event1_handler(void)
{
#if (!THROUGHPUT_EN && !SD_DEMO)
	rsi_pkt_sweep_event_handler();
#endif
}

void rsi_app_event2_handler(void) 
{

}
//! rejoin failure call back handler in station mode
void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  //! update wlan application state
  rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;
}

//! ip renewal failure call back handler in station mode
void rsi_ip_renewal_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  //! update wlan appication state
  rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;
}

//! stations remote socket terminate call back handler
void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	data_recvd = 1;

	socket_closed = 1;

	//! update wlan application state
	rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
#if THROUGHPUT_EN
	//! capture timestamp after data transfer is completed
	t_end = rsi_hal_gettickcount();
#endif
}

//! stations ip change notify call back handler in station mode
void rsi_ip_change_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  //! update wlan appication state
  rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
}

//! stations connect notify call back handler in AP mode
void rsi_stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{

}

//! stations disconnect notify call back handler in AP mode
void rsi_stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{

}

#endif
