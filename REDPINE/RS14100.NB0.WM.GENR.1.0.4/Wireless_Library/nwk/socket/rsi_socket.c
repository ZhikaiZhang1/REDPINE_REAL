/**
 * @file     rsi_socket.c
 * @version  0.1
 * @date     15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains socket APIs
 *
 *  @section Description  This file contains socket APIs
 *
 *
 */
#include "rsi_driver.h"
#include "rsi_wlan_non_rom.h"


//! Socket information pool pointer
rsi_socket_info_t *rsi_socket_pool;
rsi_socket_info_non_rom_t *rsi_socket_pool_non_rom;
extern rsi_wlan_cb_non_rom_t wlan_cb_non_rom;

/*==============================================*/
/**
 * @fn         int32_t rsi_socket_async(int32_t protocolFamily, int32_t type, int32_t protocol, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length))
 * @brief      Creates socket
 * @param[in]  protocolFamily , Type of the socket family to create
 * @param[in]  type , type of the socket to create 
 * @param[in]  protocol , to enable SSL over TCP 
 * @param[in]  callback , callback function to read data ayncronously from socket 
 *
 * @param[out] None
 * @return     
 *             <0 - If fails
 *             >= - If success
 *
 * @section description
 * This function creates socket
 *
 */

int32_t  rsi_socket_async(int32_t protocolFamily, int32_t type, int32_t protocol, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length))
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->socket_async(global_cb_p, protocolFamily, type, protocol, callback);
#else
  return api_wl->socket_async(global_cb_p, protocolFamily, type, protocol, callback);
#endif

}
/*==============================================*/
/**
 * @fn          int32_t rsi_socket(int32_t protocolFamily, int32_t type, int32_t protocol)
 * @brief       Creates socket
 * @param[in]   protocolFamily , Type of the socket family to create
 * @param[in]   type , type of the socket to create 
 * @param[in]   protocol , to enable SSL over TCP 
 * @param[out]  None
 * @return      
 *              <0  - If fails
 *              >=0 - If success
 *
 * @section description
 * This function creates socket
 *
 */

int32_t rsi_socket(int32_t protocolFamily, int32_t type, int32_t protocol)
{
	if (protocol  & BIT(0))
	{
		if(protocol & BIT(13))
		{	
			wlan_cb_non_rom.tls_version = (RSI_SOCKET_FEAT_SSL | RSI_SSL_V_1_0);
		}
		else if (protocol & BIT(14))			
		{
			wlan_cb_non_rom.tls_version = (RSI_SOCKET_FEAT_SSL | RSI_SSL_V_1_1);
		}
		else if (protocol & BIT(15))
		{
			wlan_cb_non_rom.tls_version = (RSI_SOCKET_FEAT_SSL |RSI_SSL_V_1_2);
		}
		else
		{
			wlan_cb_non_rom.tls_version = RSI_SOCKET_FEAT_SSL;
		}
	}


#ifdef ROM_WIRELESS
	return ROMAPI_WL->socket_async(global_cb_p, protocolFamily, type, protocol, NULL);
#else
	return api_wl->socket_async(global_cb_p, protocolFamily, type, protocol, NULL);
#endif

}

/*==============================================*/
/**
 * @fn          int32_t  rsi_bind(int32_t sockID, struct rsi_sockaddr *localAddress, int32_t addressLength)
 * @brief       Assign address to socket
 * @param[in]   sockID, socket descriptor
 * @param[in]   localAddress, address which needs to be assign 
 * @param[in]   addressLength, length of the socket address 
 * @param[out]  None
 * @return
 *              <0 - If fails
 *               0 - If success
 *
 * @section description
 * This assigns address to the socket
 *
 *
 */
int32_t  rsi_bind(int32_t sockID, struct rsi_sockaddr *localAddress, int32_t addressLength)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->bind(global_cb_p, sockID, localAddress, addressLength);
#else
  return api_wl->bind(global_cb_p, sockID, localAddress, addressLength);
#endif
}
/*==============================================*/
/**
 * @fn          int32_t  rsi_connect(int32_t sockID, struct rsi_sockaddr *remoteAddress, int32_t addressLength)
 * @brief       connect the socket to specified remoteAddress
 * @param[in]   sockID, socket descriptor 
 * @param[in]   remoteAddress, remote peer address structure 
 * @param[in]   addressLength, remote peer address structrue length 
 * @param[out]  None
 * @return
 *
 *              <0 - If fails
 *               0 - If success
 *
 * @section description
 * This function is use to connect the socket to specified remote address
 *
 */

int32_t  rsi_connect(int32_t sockID, struct rsi_sockaddr *remoteAddress, int32_t addressLength)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->connect(global_cb_p, sockID, remoteAddress, addressLength);
#else
  return api_wl->connect(global_cb_p, sockID, remoteAddress, addressLength);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_listen(int32_t sockID, int32_t backlog)
 * @brief      This function is used to make socket to listen for remote connecion request in passive mode
 * @param[in]  sockID, socket descriptor 
 * @param[in]  backlog, maximum number of pending connections requests 
 * @param[out] None
 * @return
 *              <0 - If fails
 *               0 - If success
 *
 * @section description
 * This function is used to make socket to listen for remote connecion request in passive mode
 *
 */

int32_t  rsi_listen(int32_t sockID, int32_t backlog)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->listen(global_cb_p, sockID, backlog);
#else
  return api_wl->listen(global_cb_p, sockID, backlog);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_accept(int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength)
 * @brief      This function is used to accept connection request from remote peer
 * @param[in]  sockID, socket descriptor 
 * @param[in]  ClientAddress, Remote peer address 
 * @param[in]  addressLength, Remote peer address length
 * @param[out] None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to accept connection request from remote peer
 *
 */

int32_t  rsi_accept(int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->accept(global_cb_p, sockID, ClientAddress, addressLength);
#else
  return api_wl->accept(global_cb_p, sockID, ClientAddress, addressLength);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_recvfrom(int32_t sockID, int8_t *buffer, int32_t buffersize, int32_t flags,struct rsi_sockaddr *fromAddr, int32_t *fromAddrLen)
 * @brief      This function is used to receive data from remote peer 
 * @param[in]  sockID, socket descriptor 
 * @param[in]  buffer, application buffer pointer to hold receive data
 * @param[in]  buffersize, requested bytes for read
 * @param[in]  flags, reserved
 * @param[in]  fromAddr, remote peer address 
 * @param[in]  fromAddrLen, remote peer address length
 * @param[out] None 
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 *
 * @section description
 * This function is used to receive data from remote peer 
 *
 */

int32_t  rsi_recvfrom(int32_t sockID, int8_t *buffer, int32_t buffersize, int32_t flags,struct rsi_sockaddr *fromAddr, int32_t *fromAddrLen)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->recvfrom(global_cb_p, sockID, buffer, buffersize, flags, fromAddr, fromAddrLen);
#else
  return api_wl->recvfrom(global_cb_p, sockID, buffer, buffersize, flags, fromAddr, fromAddrLen);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_recv(int32_t sockID, void *rcvBuffer, int32_t bufferLength, int32_t flags)
 * @brief      This function is used to receive data from remote peer 
 * @param[in]  sockID, socket descriptor 
 * @param[in]  buffer, application buffer pointer to hold receive data
 * @param[in]  buffersize, requested bytes for read
 * @param[in]  flags, reserved
 * @return
 *
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to receive data from remote peer 
 *
 */
int32_t  rsi_recv(int32_t sockID, void *rcvBuffer, int32_t bufferLength, int32_t flags)
{
  int32_t fromAddrLen = 0;
  
#ifdef ROM_WIRELESS
  return ROMAPI_WL->recvfrom(global_cb_p, sockID, rcvBuffer, bufferLength, flags, NULL, &fromAddrLen);
#else
  return api_wl->recvfrom(global_cb_p, sockID, rcvBuffer, bufferLength, flags, NULL, &fromAddrLen);
#endif
}

/*==============================================*/
/**
 * @fn          int32_t rsi_sendto(int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen)
 * @brief       This function is used to send data to specific remote peer on a given socket synchronously
 * @param[in]   sockID, socket descriptor 
 * @param[in]   msg, pointer to data which needs to be send to remote peer 
 * @param[in]   msgLength, length of data to send 
 * @param[in]   flags, reserved 
 * @param[in]   destAddr, remote peer address to send data 
 * @param[in]   destAddrLen, rmeote peer address length
 * @param[out]  None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to send data to specific remote peer on a given socket
 *
 */

int32_t rsi_sendto(int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->sendto_async(global_cb_p, sockID, msg, msgLength, flags, destAddr, destAddrLen, NULL);
#else
  api_wl->sendto_async(global_cb_p, sockID, msg, msgLength, flags, destAddr, destAddrLen, NULL);
#endif
}

/*==============================================*/
/**
 * @fn          int32_t rsi_sendto_async(int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen,
                     void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
 * @brief       This function is used to send data to specific remote peer on a given socket asynchronously
 * @param[in]   sockID, socket descriptor 
 * @param[in]   msg, pointer to data which needs to be send to remote peer 
 * @param[in]   msgLength, length of data to send 
 * @param[in]   flags, reserved 
 * @param[in]   destAddr, remote peer address to send data 
 * @param[in]   destAddrLen, rmeote peer address length
 * @param[in]   data_transfer_complete_handler, pointer to the callback function which will be called after data transfer completion
 * @param[out]  None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to send data to specific remote peer on a given socket
 *
 */
int32_t rsi_sendto_async(int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen,
                     void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->sendto_async(global_cb_p, sockID, msg, msgLength, flags, destAddr, destAddrLen, data_transfer_complete_handler);
#else
  return api_wl->sendto_async(global_cb_p, sockID, msg, msgLength, flags, destAddr, destAddrLen, data_transfer_complete_handler);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t rsi_send(int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags)
 * @brief      This function is used to send data on a given socket syncronously
 * @param[in]  sockID, socket descriptor 
 * @param[in]  msg, pointer to data which needs to be send to remote peer 
 * @param[in]  msgLength, length of data to send 
 * @param[in]  flags, reserved 
 * @param[out] None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to send data on a given socket
 *
 */

int32_t rsi_send(int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->send_async(global_cb_p, sockID, msg, msgLength, flags, NULL);
#else
  return api_wl->send_async(global_cb_p, sockID, msg, msgLength, flags, NULL);
#endif
}



/*==============================================*/
/**
 * @fn         int32_t rsi_send_async(int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags, 
                   void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
 * @brief      This function is used to send data on a given socket asynchronously
 * @param[in]  sockID, socket descriptor 
 * @param[in]  msg, pointer to data which needs to be send to remote peer 
 * @param[in]  msgLength, length of data to send 
 * @param[in]  flags, reserved 
 * @param[in]  data_transfer_complete_handler, pointer to the callback function which will be called after data transfer completion
 * @param[out] None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to send data on a given socket
 *
 */
int32_t rsi_send_async(int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags, 
    void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->send_async(global_cb_p, sockID, msg, msgLength, flags, data_transfer_complete_handler);
#else
  return api_wl->send_async(global_cb_p, sockID, msg, msgLength, flags, data_transfer_complete_handler);
#endif
}

/*==============================================*/
/**
 * @fn          int32_t  rsi_shutdown(int32_t sockID, int32_t how)
 * @brief       This function is used to close the socket
 * @param[in]   sockID, socket descriptor 
 * @param[in]   how, to select type of socket close 
 * @param[out]  None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to close the socket
 *
 */
int32_t  rsi_shutdown(int32_t sockID, int32_t how)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->shutdown(global_cb_p, sockID, how);
#else
  return api_wl->shutdown(global_cb_p, sockID, how);
#endif
}


/*==============================================*/
/**
 * @fn          int32_t rsi_check_state(int32_t type)
 * @brief       Checks wlan state
 * @param[in]   type, socket family ttype 
 * @param[out]  None
 * @return
 *              1 - If not in IP config state
 *              0 - If in IP cofig state
 *
 * @section description
 * This function is used to check the wlan state
 *
 */
int32_t rsi_check_state(int32_t type)
{
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;

  if(rsi_driver_cb->wlan_cb->opermode == RSI_WLAN_ACCESS_POINT_MODE)
  {
    if(RSI_CHECK_WLAN_STATE() != RSI_WLAN_STATE_CONNECTED)
    {
      return 1;
    }
  }
  else
  {
    if(type == AF_INET)
    {
      if(RSI_CHECK_WLAN_STATE() != RSI_WLAN_STATE_IP_CONFIG_DONE)
      {
        return 1;
      }
    }
    else
    {
      if(RSI_CHECK_WLAN_STATE() != RSI_WLAN_STATE_IPV6_CONFIG_DONE)
      {
        return 1;
      }
    }
  }
  return 0;
}


/*==============================================*/
/**
 * @fn          int32_t rsi_get_application_socket_descriptor(int32_t sock_id)
 * @brief       Gets application socket descriptor from module socket descriptor
 * @param[in]   sock_id, module's socket descriptor
 * @param[out]  None
 * @return
 *             <0 - If index is not found
 *             >0 - application index
 *
 * @section description
 * This function is used to get the application socket descriptor from module socket descriptor
 *
 */
int32_t rsi_get_application_socket_descriptor(int32_t sock_id)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_get_application_socket_descriptor(global_cb_p, sock_id);
#else
  return api_wl->rsi_get_application_socket_descriptor(global_cb_p, sock_id);
#endif
}

/*==============================================*/
/**
 * @fn          void rsi_clear_sockets(uint8_t sockID)
 * @brief       function to clear socket information
 * @param[in]   sockID, socket descriptor
 * @param[out]  None
 * @return      void
 *
 * @section description
 * This function is used to clear socket information
 *
 */
void rsi_clear_sockets(uint8_t sockID)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_clear_sockets(global_cb_p, sockID);
#else
  api_wl->rsi_clear_sockets(global_cb_p, sockID);
#endif
}



/*==============================================*/
/**
 * @fn           int rsi_setsockopt(int sockID, int level, int option_name,const void *option_value, socklen_t option_len)
 * @brief        This function is used to set the socket options 
 * @param[in]    sockID, socket descriptor 
 * @param[in]    level,
 * @param[in]    option_name,
 * @param[in]    option_value,
 * @param[in]    option_len, 
 * @return        
 *
 *
 * @section description
 * This API is used to set the socket options
 *
 *
 */

int rsi_setsockopt(int sockID, int level, int option_name,const void *option_value, rsi_socklen_t option_len)
{

  struct rsi_timeval *timeout = NULL;
 
  uint16_t timeout_val;
  

  //! If sockID is not in available range
  if(sockID < 0 || sockID >= RSI_NUMBER_OF_SOCKETS)
  {
    //! Set error
    rsi_wlan_set_status(EBADF);

    return RSI_SOCK_ERROR;
  }


  //! Configure per socket maximum tcp retries count
  if((option_name == SO_MAXRETRY ))
  {
     rsi_socket_pool_non_rom[sockID].max_tcp_retries = *(uint8_t *)option_value;
    return RSI_SUCCESS;
  }

  //! Configure tcp keep alive time
  if((option_name == SO_TCP_KEEP_ALIVE ))
  {
     rsi_uint16_to_2bytes(rsi_socket_pool_non_rom[sockID].tcp_keepalive_initial_time,*(uint16_t *)option_value);
    return RSI_SUCCESS;
  }

  if((option_name == SO_SOCK_VAP_ID ))
  {
    rsi_socket_pool_non_rom[sockID].vap_id =  *(uint8_t *)option_value;
    return RSI_SUCCESS;
  }


  if((option_name != SO_RCVTIMEO ) || (level != SOL_SOCKET))
  {
    rsi_wlan_set_status(EINVAL);

    return RSI_SOCK_ERROR;
  }  
  //! If socket is in not created state
  if(rsi_socket_pool[sockID].sock_state == RSI_SOCKET_STATE_INIT) 
  {
    //! Set error
    rsi_wlan_set_status(EBADF);

    return RSI_SOCK_ERROR;
  }
  
  timeout = (struct rsi_timeval *)option_value;
  
  timeout_val = (timeout->tv_usec/1000) + (timeout->tv_sec * 1000);

    //! This feature is available only if Synchrounous bitmap is set
  if(rsi_socket_pool[sockID].sock_bitmap & RSI_SOCKET_FEAT_SYNCHRONOUS)
  {
    rsi_socket_pool[sockID].read_time_out = timeout_val;
  }
  else
  {
    //! Set error
    rsi_wlan_set_status(ENOPROTOOPT);

    return RSI_SOCK_ERROR;
  }
   return RSI_SUCCESS;
}

/*==============================================*/
/**
 * @fn          int32_t  rsi_select(int32_t nfds, rsi_fd_set *readfds, rsi_fd_set *writefds, rsi_fd_set *exceptfds, struct rsi_timeval *timeout) 
 * @brief
 * @param[in]   int32_t nfds
 * @param[in]   rsi_fd_set *readfds
 * @param[in]   rsi_fd_set *writefds
 * @param[in]   rsi_fd_set *exceptfds
 * @param[out]
 * @return
 *
 *
 * @section description
 * This 
 *
 *
 */

int32_t  rsi_select(int32_t nfds, rsi_fd_set *readfds, rsi_fd_set *writefds, rsi_fd_set *exceptfds, struct rsi_timeval *timeout)
{

  return RSI_SOCK_ERROR;
  //! TODO: Later
}

uint8_t calculate_buffers_required(uint8_t type, uint16_t length)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->calculate_buffers_required(global_cb_p, type, length);
#else
  return api_wl->calculate_buffers_required(global_cb_p, type, length);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_accept_async(int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength )
 * @brief      This function is used to accept_async connection request from remote peer  
 * @param[in]  sockID, socket descriptor 
 * @param[in]  ClientAddress, Remote peer address 
 * @param[in]  addressLength, Remote peer address length
 * @param[out] None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to accept_async connection request from remote peer
 *
 */


int32_t  rsi_accept_async(int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength)
{
  struct rsi_sockaddr_in  peer4_address;
  struct rsi_sockaddr_in6 peer6_address;
  int32_t status = RSI_SUCCESS;
  rsi_req_socket_accept_t *accept;
  rsi_pkt_t *pkt = NULL;
  rsi_socket_info_t *sock_info;
  int8_t accept_sock_id;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! If sockID is not in available range
  if(sockID < 0 || sockID >= NUMBER_OF_SOCKETS)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! Check socket is TCP or not
  if((rsi_socket_pool[sockID].sock_type & 0xF) != SOCK_STREAM)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EPROTOTYPE);

    return RSI_SOCK_ERROR;
  }

  //! If socket is not binded
  if(rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_LISTEN && \
      (rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_CONNECTED))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }
  if(rsi_socket_pool[sockID].ltcp_socket_type != RSI_LTCP_PRIMARY_SOCKET)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EINVAL);

    return RSI_SOCK_ERROR;
  }

  sock_info = &rsi_socket_pool[sockID];

  //! check maximum backlogs count
  if(sock_info->backlogs == sock_info->backlog_current_count)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(ENOBUFS);

    return RSI_SOCK_ERROR;
  }

  if(rsi_socket_pool[sockID].sock_state == RSI_SOCKET_STATE_CONNECTED)
  {
    //! Create new instance for socket
    accept_sock_id = rsi_socket_async((sock_info->sock_type >> 4), (sock_info->sock_type & 0xF), (sock_info->sock_bitmap & RSI_SOCKET_FEAT_SSL), sock_info->sock_receive_callback);

    if((accept_sock_id >= 0) && (accept_sock_id < NUMBER_OF_SOCKETS))
    {

      //! Set socket as secondary socket
      rsi_socket_pool[accept_sock_id].ltcp_socket_type = RSI_LTCP_SECONDARY_SOCKET;

      //! Save local port number
      rsi_socket_pool[accept_sock_id].source_port = rsi_socket_pool[sockID].source_port;
    }
    else
    {

      return RSI_SOCK_ERROR;
    }
  }
  else
  {
    accept_sock_id = sockID;
  }

  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! Allocate packet
  pkt = rsi_pkt_alloc(&rsi_driver_cb->wlan_cb->wlan_tx_pool);
  if(pkt == NULL)
  {
    //! Release mutex lock
    RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

    //! Set error
    RSI_WLAN_SET_STATUS(RSI_ERROR_PKT_ALLOCATION_FAILURE);

    return RSI_SOCK_ERROR;
  }

  //! send socket accept command
  accept = (rsi_req_socket_accept_t *)pkt->data;

  //! Fill socket descriptor
  accept->socket_id = rsi_socket_pool[sockID].sock_id;

  //! Fill local port number
  rsi_uint16_to_2bytes(accept->source_port, rsi_socket_pool[sockID].source_port);

  //! Set waiting socket ID
  rsi_driver_cb->wlan_cb->waiting_socket_id = accept_sock_id;

  rsi_driver_cb->wlan_cb->expected_response =RSI_WLAN_RSP_ASYNCHRONOUS; 
  //! Send socket accept command
  status = RSI_DRIVER_WLAN_SEND_CMD(RSI_WLAN_REQ_SOCKET_ACCEPT, pkt);

  //! check status,fill out params with remote peer address details if success and return status  
  if(status == RSI_SUCCESS)
  {
    //! Copy remote peer ip address
    if(ClientAddress && *addressLength != 0)
    {
      //! Handle the IPv4 socket type
      if((rsi_socket_pool[accept_sock_id].sock_type >> 4) == AF_INET)
      {

        //! Update the Client address with socket family, remote host IPv4 address and port
        peer4_address.sin_family      = AF_INET;
        memcpy(&peer4_address.sin_addr.s_addr, (ntohl(rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv4)), RSI_IPV4_ADDRESS_LENGTH);
        peer4_address.sin_port        = ntohs((uint16_t)rsi_socket_pool[accept_sock_id].destination_port);

        //! Copy the peer address/port info to the ClientAddress.  
        //! Truncate if addressLength is smaller than the size of struct rsi_sockaddr_in
        if(*addressLength > sizeof(struct rsi_sockaddr_in))
        {
          *addressLength = sizeof(struct rsi_sockaddr_in);
        }
        memcpy(ClientAddress, &peer4_address, *addressLength);
      }
      else if((rsi_socket_pool[accept_sock_id].sock_type >> 4) == AF_INET6)
      {
        peer6_address.sin6_family = AF_INET6;
        peer6_address.sin6_port = ntohs((uint16_t)rsi_socket_pool[accept_sock_id].destination_port);        

        peer6_address.sin6_addr._S6_un._S6_u32[0] = (uint32_t)ntohl(&rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv6[0]);
        peer6_address.sin6_addr._S6_un._S6_u32[1] = (uint32_t)ntohl(&rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv6[4]);
        peer6_address.sin6_addr._S6_un._S6_u32[2] = (uint32_t)ntohl(&rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv6[8]);
        peer6_address.sin6_addr._S6_un._S6_u32[3] = (uint32_t)ntohl(&rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv6[12]);

        if((*addressLength) > sizeof(struct rsi_sockaddr_in6))
        {
          *addressLength = sizeof(struct rsi_sockaddr_in6);
        }

        memcpy(ClientAddress, &peer6_address, *addressLength);
      }
    }

    //! Change state to connected
    rsi_socket_pool[accept_sock_id].sock_state = RSI_SOCKET_STATE_CONNECTED;

    //! Increase backlog current count
    rsi_socket_pool[sockID].backlog_current_count++;
  }

  //! Free packet
  rsi_pkt_free(&rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(status != RSI_SUCCESS)
  {
    return RSI_SOCK_ERROR;
  }

  //! Return status
  return accept_sock_id;

}
