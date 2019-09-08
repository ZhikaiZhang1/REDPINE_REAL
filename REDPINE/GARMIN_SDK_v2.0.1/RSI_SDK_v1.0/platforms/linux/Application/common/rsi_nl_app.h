/**
 * @file     rsi_nl_app.h
 * @version  3.6
 * @date     2013-May-16
 *
 * Copyright(C) Redpine Signals 2013
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief HEADER  
 *
 * @section Description
 * 
 *
 */

#ifndef __RSI_GNUSER_H_
#define __RSI_GNUSER_H_

#include <stdint.h>

#include "rsi_linux_app_init.h"
#include <errno.h>
#include <sys/socket.h>
#include <linux/genetlink.h>

/*
 * Generic macros for dealing with netlink sockets. Might be duplicated
 * elsewhere. It is recommended that commercial grade applications use
 * libnl or libnetlink and use the interfaces provided by the library
 */
#define GENLMSG_DATA(glh) ((void *)(NLMSG_DATA(glh) + GENL_HDRLEN))
#define GENLMSG_PAYLOAD(glh) (NLMSG_PAYLOAD(glh, 0) - GENL_HDRLEN)
#define NLA_DATA(na) ((void *)((char*)(na) + NLA_HDRLEN))
#define MAX_RCV_SIZE  1600
#define RSI_NL_HEAD_SIZE         (sizeof(struct nlmsghdr) + sizeof(struct genlmsghdr) + sizeof(struct nlattr))
#define RSI_STATUS_OFFSET         12
#define RSI_TWOBYTE_STATUS_OFFSET 12
#define RSI_RSP_TYPE_OFFSET       2
#define GET_SEND_LENGTH(a) ((uint16_t)(*(uint32_t *)(a)))
/* User to Kernel Update Types */
enum {
  MODULE_POWER_CYCLE               = 0x01,
  UPDATE_JOIN_DONE                 = 0x02,
  PS_CONTINUE                      = 0x03,
  WKP_FROM_HOST                    = 0x04,
  UPDATE_CONCURRENT_AP_JOIN_DONE   = 0x05,
};



/* type defines */
typedef struct {
  struct nlmsghdr n;
  struct genlmsghdr g;
} rsi_nlPkt_t;

/* Function prototypes */


#endif //__RSI_GNUSER_H_
