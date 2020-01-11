#include <stdio.h>
#include <stdlib.h>
//#include <sys/select.h>

//#include "GRM_pub.h"
//#include "UTL_pub_assert.h"
//#include "SCL_pub_errno.h"


#include "rsi_apis_rom.h"
#include "rsi_nwk.h"
#include "rsi_socket.h"
#include "rsi_user.h"

#include "sys/socket.h"

int errno;
#define CONVERT_BE_TO_LE_SOCKADDR  change_sockaddr_endianness
typedef struct
    {
    char                hostname[256];  /* Hostname that was resolved. Max 255 Characters */
    struct in_addr      ip_addr;        /* Resolved IP address */
    struct in_addr *    addr_list[2];   /* [0] to point to ip_addr, [1] is set to null */
    char *              aliases;        /* set to NULL */
    } hostbyname_r_helper_t;
		
static void change_sockaddr_endianness
    (
    const struct sockaddr * sockaddr_in,
    struct sockaddr *       sockaddr_out
    );
		
ssize_t RSI_bsd_sendto
    (
    int	                    s,
    const void *            msg,
    size_t                  len,
    int                     flags,
    const struct sockaddr * to,
    socklen_t               tolen
    )
{
struct sockaddr le_to;
CONVERT_BE_TO_LE_SOCKADDR( to, &le_to );
return rsi_sendto( s, (int8_t*)msg, len, flags, &le_to, tolen );
} /* RSI_bsd_sendto() 

/*********************************************************************
*
*   PROCEDURE NAME:
*       RSI_bsd_gethostbyname
*
*   DESCRIPTION:
*       RSI implementation of gethostbyname
*       This is not thread safe as the returned data can be overwritten
*       by subsequent requests.
*
*********************************************************************/
struct hostent * RSI_bsd_gethostbyname
    (
    const char *            name
    )
{
		static struct hostent           host_data;
		static struct hostent *         host_data_result;
		static hostbyname_r_helper_t    helper_buf;

		if( 0 != RSI_bsd_gethostbyname_r( name, &host_data, (char*)&helper_buf, sizeof( helper_buf ), &host_data_result, NULL ) )
				{
				errno = EINVAL;
				host_data_result = NULL;
				}
		return host_data_result;
} /* RSI_bsd_gethostbyname() */

/*********************************************************************
*
*   PROCEDURE NAME:
*       RSI_bsd_gethostbyname_r
*
*   DESCRIPTION:
*       RSI implementation of gethostbyname_r
*       https://linux.die.net/man/3/gethostbyname_r
*
*********************************************************************/
int RSI_bsd_gethostbyname_r
    (
    const char *        name,
    struct hostent *    ret,
    char *              buf,
    size_t              buflen,
    struct hostent **   result,
    int *               h_errnop
    )
{
hostbyname_r_helper_t * helper;
int32_t                 dns_result;
rsi_rsp_dns_query_t     response = {0};
uint8_t                 ip_index;

(void)h_errnop;

if( !result )
    {
    /* Invalid params */
    errno = EINVAL;
    return -1;
    }
*result = NULL;

if( !name ||
    ( strlen(name) > sizeof(helper->hostname) ) ||
    !ret ||
    !buf )
    {
    /* Invalid params */
    errno = ERANGE;
    return -1;
    }

/* Make sure work buffer has enough space for the hostname, addr_list, and aliases */
if( buflen < sizeof( hostbyname_r_helper_t ) )
    {
    //UTL_assert_continue( FALSE );
    errno = EINVAL;
    return -1;
    }
helper = (hostbyname_r_helper_t*)buf;

/* Get IP address */
dns_result = rsi_dns_req( RSI_IP_VERSION_4,
                          (uint8_t*)name,
                          NULL,    /* Primary DNS server. Not required if using DHCP */
                          &response,
                          sizeof( response )
                          );

if( dns_result != 0 )
    {
    /* Failed to get IP address */
    errno = EINVAL;
    return -1;
    }

/* Find IP index */
if( RSI_IP_VERSION_4 == response.ip_version[0] )
    {
    ip_index = 0;
    }
else if( RSI_IP_VERSION_4 == response.ip_version[1] )
    {
    ip_index = response.ip_count[0]; /* Offset past the IPV6 addresses */
    }
else
    {
    /* No IPV4 address available */
    return -1;
    }

/* Start populating helper */
strcpy( helper->hostname, name );
/* Create host representation of the ip address */
helper->ip_addr.s_addr = ((uint32_t)response.ip_address[ip_index].ipv4_address[0]) << 24 |
                         ((uint32_t)response.ip_address[ip_index].ipv4_address[1]) << 16 |
                         ((uint32_t)response.ip_address[ip_index].ipv4_address[2]) << 8 |
                         (uint32_t)response.ip_address[ip_index].ipv4_address[3];
helper->ip_addr.s_addr = UTL_htob_u32( helper->ip_addr.s_addr ); /* Convert from host to big endian */
helper->addr_list[0] = &helper->ip_addr;
helper->addr_list[1] = NULL;
helper->aliases = NULL;

/* Populate hostent data to be returned */
ret->h_name = helper->hostname;
ret->h_addrtype = AF_INET;
ret->h_length = sizeof( helper->ip_addr );
ret->h_addr_list = (char**)helper->addr_list;

/* Configure result */
*result = ret;

return 0;
} /* RSI_bsd_gethostbyname_r() */

static void change_sockaddr_endianness
    (
    const struct sockaddr * sockaddr_in,
    struct sockaddr *       sockaddr_out
    )
{
const struct sockaddr_in *  in = (const struct sockaddr_in*)sockaddr_in;
struct sockaddr_in *        out = (struct sockaddr_in*)sockaddr_out;

if( !sockaddr_in ||
    !sockaddr_out )
    {
    //UTL_assert( FALSE );
    return;
    }

/* Copy bulk of the items */
memcpy( out, in, sizeof( *out ) );

/* Convert Port to LE */
out->sin_port = UTL_swap_u16( in->sin_port );
} /* change_sockaddr_endianness() */