/**
 * @file       rsi_ethernet.h
 * @version    0.1
 * @date       20 jan 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes releated to Ethernet peripheral
 * 
 * @section Description
 * This file contains the list of function prototypes for the EThernet and low level function definations 
 * Following are list of API's which need to be defined in this file.
 *
 */


/* Includes */

#ifndef __RSI_ETHERNET_H
#define __RSI_ETHERNET_H

#ifdef __cplusplus
extern "C" {
#endif


#include "RS1xxxx.h"
#include "base_types.h"

/******************************************************
 * *                      Macros
 * ******************************************************/


/*******************************************************************************/
/************************** Call Back Macros**************************************/
/*******************************************************************************/
#define RSI_NULL       0

#define GMAC_INTERRUPT 0x0000 /*!< GMAC interrupt*/
#define DMA_INTERRUPT  0x0001 /*!< DMA interrupt*/

 
#define ETH_EVENT_WAKEUP_FRAME_RECVD   0x0001 /*!< This event occurs when wakeup frame is received  */
#define ETH_EVENT_MAGIC_PACKET_RECVD   0x0002 /*!< This event occurs when magic packet is received  */

#define ETH_EVENT_TI            0x0001   /*!< This event indicates that frame transmission is finished and TDES1[31] is set in the First Descriptor*/
#define ETH_EVENT_TPS           0x0002   /*!< This event is set when the transmission is stopped  */
#define ETH_EVENT_TU            0x0003   /*!< This event indicates that the Next Descriptor in the Transmit List is owned by
																					    the host and cannot be acquired by the DMA. Transmission is suspended*/
#define ETH_EVENT_TJT           0x0004	 /*!< This event indicates that the Transmit Jabber Timer expired, meaning that the
																				      transmitter had been excessively active*/																				 
#define ETH_EVENT_OVF           0x0005   /*!< This event indicates that the Receive Buffer had an Overflow during frame
																				    	reception. */																					
#define ETH_EVENT_UNF           0x0006   /*!< This event indicates that the Transmit Buffer had an Underflow during frame
																						  transmission*/     																					
#define ETH_EVENT_RI            0x0007   /*!< This event indicates the completion of frame reception. Specific frame status
																							information has been posted in the descriptor */																							
#define ETH_EVENT_RU            0x0008   /*!< This event indicates that the Next Descriptor in the Receive List is owned by the
																							host and cannot be acquired by the DMA*/																					
#define ETH_EVENT_RPS           0x0009   /*!< This event is asserted when the Receive Process enters the Stopped state*/ 
#define ETH_EVENT_RWT           0x000A   /*!< This bit is asserted when a frame with a length greater than 2,048 bytes is
																							received*/
#define ETH_EVENT_ETI           0x000B   /*!< This bit indicates that the frame to be transmitted was fully transferred to the
																						   MTL Transmit FIFO*/																							
																							 
#define ETH_EVENT_BUS_ERR      0x000C    /*!< BUS Error*/																							 																							 
#define ETH_EVENT_ERI          0x000D    /*!< This event indicates that the DMA had filled the first data buffer of the packet */ 









/*******************************************************************************/
/************************** DMA Registers **************************************/
/*******************************************************************************/
#define MAX_ENET_CALL_BACKS   2
/*!< DMA Configuration macros */


/*!< BUS mode register configuration*/
#define DMA_BUS_MODE_AAL              0x02000000   /*!< When this bit is set high and the FB bit equals 1, the AHB interface generates all 
                                                      bursts aligned to the start address LS bits*/
#define DMA_BUS_MODE_PBL              0x01000000   /*!< When set high, this bit multiplies the PBL value programmed (bits [22:17] and
                                                      bits [13:8]) four times. Thus the DMA will transfer data in to a maximum of 4, 8,
                                                      16, 32, 64 and 128 beats depending on the PBL value*/
#define DMA_BUS_MODE_USP              0x00800000   /*!< When set high, it configures the RxDMA to use the value configured in bits
                                                      [22:17] as PBL while the PBL value in bits [13:8] is applicable to TxDMA
                                                      operations only. When reset to low, the PBL value in bits [13:8] is applicable for
                                                      both DMA engines */
#define DMA_BUS_MODE_FB               0x00010000   /*!< This bit controls whether the AHB Master interface performs fixed burst transfers 
                                                      or not*/

#define DMA_BUS_MODE_SOFTREST         0x00000001    /*!< When this bit is set, the MAC DMA Controller resets all GMAC Subsystem
                                                    internal registers and logic */


/*! < DMA interrupt status register macros */

#define DMA_PMT_INTR_STATUS     0x10000000  /*!< This bit indicates an interrupt event in the GMAC core’s PMT module  */ 
#define DMA_MMC_INTR_STATUS     0x08000000  /*!< This bit reflects an interrupt event in the MMC module of the GMAC core*/ 
#define DMA_GLI_INTR_STATUS     0x04000000  /*!< This bit reflects an interrupt event in the GMAC Core’s PCS or RGMII
                                                      interface block*/
#define DMA_ERI_INTR_STATUS     0x00004000  /*!< This bit indicates that the DMA had filled the first data buffer of the packet*/        
#define DMA_FBI_INTR_STATUS     0x00002000  /*!< This bit indicates that a Bus Error occurred */        
#define DMA_ETI_INTR_STATUS     0x00000400  /*!< This bit indicates that the frame to be transmitted was fully transferred to the
                                                      MTL Transmit FIFO.*/       
#define DMA_RWT_INTR_STATUS     0x00000200  /*!< This bit is asserted when a frame with a length greater than 2,048 bytes is
                                                      received*/
#define DMA_RPS_INTR_STATUS     0x00000100  /*!< This bit is asserted when the Receive Process enters the Stopped state*/
#define DMA_RU_INTR_STATUS      0x00000080  /*!< This bit indicates that the Next Descriptor in the Receive List is owned by the
                                                      host and cannot be acquired by the DMA.*/ 
#define DMA_RI_INTR_STATUS      0x00000040  /*!< This bit indicates the completion of frame reception*/ 
#define DMA_UNF_INTR_STATUS     0x00000020  /*!< This bit indicates that the Transmit Buffer had an Underflow during frame
                                                      transmission*/
#define DMA_OVF_INTR_STATUS     0x00000010  /*!< This bit indicates that the Receive Buffer had an Overflow during frame
                                                      reception */
#define DMA_TJT_INTR_STATUS     0x00000008  /*!< This bit indicates that the Transmit Jabber Timer expired, meaning that the
                                                      transmitter had been excessively active.*/ 
#define DMA_TU_INTR_STATUS      0x00000004  /*!< This bit indicates that the Next Descriptor in the Transmit List is owned by
                                                      the host and cannot be acquired by the DMA */
#define DMA_TPS_INTR_STATUS     0x00000002  /*!< This bit is set when the transmission is stopped*/ 
#define DMA_TI_INTR_STATUS      0x00000001  /*!< This bit indicates that frame transmission is finished  */ 





/*!< operation mode register */

#define DMA_DT_DISABLE               0x04000000   /*!<  Disable Dropping of TCP/IP Checksum Error Frames*/ 
#define DMA_RSF_ENABLE               0x02000000   /*!<  When this bit is set, the MTL only reads a frame from the Rx FIFO after the
                                                               complete frame has been written to it, ignoring RTC bits */
#define DMA_DFF_DISABLE              0x01000000   /*!<  When this bit is set, the RxDMA does not flush any frames due to the 
                                                               unavailability of receive descriptors/buffers */
#define DMA_TSF_ENABLE               0x00200000   /*!<  When this bit is set, transmission starts when a full frame resides in the MTL
                                                      Transmit FIFO*/
#define DMA_FTF_RESET                0x00100000   /*!<  When this bit is set, the transmit FIFO controller logic is reset to its default
                                                      values and thus all data in the Tx FIFO is lost/flushed*/
#define DMA_START_TRANSMISSION       0x00002000   /*!<  When this bit is set, transmission is placed in the Running state, and the DMA
                                                      checks the Transmit List at the current position for a frame to be transmitted.*/
#define DMA_HFC_ENABLE               0x00000100   /*!<  When this bit is set, the flow control signal operation based on fill-level of Rx
                                                      FIFO is enabled*/
#define DMA_FEF_ENABLE               0x00000080   /*!<  When this bit is reset, the Rx FIFO drops frames with error status (CRC error,
                                                        collision error, GMII_ER, giant frame, watchdog timeout, overflow*/
#define DMA_FUF_ENABLE               0x00000040   /*!<  When set, the Rx FIFO will forward Undersized frames (frames with no Error
                                                        and length less than 64 bytes) including pad-bytes and CRC)*/

#define DMA_OSF_ENABLE               0x00000004   /*!<  When this bit is set, this bit instructs the DMA to process a second frame of
                                                        Transmit data even before status for first frame is obtained*/
#define DMA_START_RECEPTION          0x00000002   /*!<  When this bit is set, the Receive process is placed in the Running state. The
                                                      DMA attempts to acquire the descriptor from the Receive list and processes incoming frames*/




/*!< Interrupt enable register */
 
#define  DMA_NIE_INTR              0x00010000   /*!<  When this bit is set, a normal interrupt is enabled
                                                      Transmit Interrupt
                                                      Transmit Buffer Unavailable
                                                      Receive Interrupt
                                                      Early Receive Interrupt */
#define  DMA_AIE_INTR              0x00008000   /*!< When this bit is set, an Abnormal Interrupt is enabled
                                                      Transmit Process Stopped
                                                      Transmit Jabber Timeout
                                                      Receive Overflow
                                                      Transmit Underflow
                                                      Receive Buffer Unavailable
                                                      Receive Process Stopped
                                                      Receive Watchdog Timeout
                                                      Early Transmit Interrupt
                                                      Fatal Bus Error*/
#define  DMA_ERE_INTR              0x00004000  /*!< Early Receive Interrupt Enable */
#define  DMA_FBE_INTR              0x00002000  /*!< When this bit is set with Abnormal Interrupt Summary Enable (Register7[15]), the
                                                    Fatal Bus Error Interrupt is enabled*/
#define  DMA_ETE_INTR              0x00000400  /*!< Early Transmit Interrupt Enable */

#define  DMA_RWE_INTR              0x00000200  /*!< Receive Watchdog Timeout Enable */
#define  DMA_RSE_INTR              0x00000100  /*!< Receive Stopped Interrupt is enabled. */
#define  DMA_RUE_INTR              0x00000080  /*!< Receive Buffer Unavailable Interrupt is enabled */
#define  DMA_RIE_INTR              0x00000040  /*!< Receive Interrupt is enabled */
#define  DMA_UNE_INTR              0x00000020  /*!< Underflow Interrupt Enable */
#define  DMA_OVE_INTR              0x00000010  /*!< verflow Interrupt Enable */ 
#define  DMA_TJE_INTR              0x00000008  /*!< Transmit Jabber Timeout Enable*/ 
#define  DMA_TUE_INTR              0x00000004  /*!< Transmit Buffer Unavailable Enable  */
#define  DMA_TSE_INTR              0x00000002  /*!< Transmit Stopped Enable */
#define  DMA_TIE_INTR              0x00000001  /*!< Transmit Interrupt Enable */

#define  PMT_WFR_INTR              0x00000040   /*!< This bit indicates the power management event was generated
																										 due to reception of a wake-up frame */
#define  PMT_MPR_INTR              0x00000080   /*!< This bit indicates the power management event was generated by
																										 the reception of a Magic Packet*/


/*!<Receive  DMA Descriptors configuration macros*/

/*!<RDES0 */

#define DMA_RXDESC_OWN        0x80000000         /*!< When set, this bit indicates that the descriptor is owned by the DMA */
#define DMA_RXDESC_AFM        0x40000000         /*!< When set, this bit indicates a frame that failed in the DA Filter in the GMAC Core.*/ 
#define DMA_RXDESC_ERR        0x00004000         /*!< When set, this bit indicates a frame truncation caused by a frame that does not fit within the current
                                                    descriptor buffers, and that the DMA does not own the Next Descriptor. The frame is truncated*/
#define DMA_RXDESC_SAF        0x00002000         /*!< When set, this bit indicates that the SA field of frame failed the SA Filter in the GMAC Core*/   
#define DMA_RXDESC_LE         0x00001000         /*!< When set, this bit indicates that the actual length of the frame received and that the Length/ Type field
                                                    does not match*/
#define DMA_RXDESC_OE         0x00000800         /*!< When set, this bit indicates that the received frame was damaged due to buffer overflow in MTL */
#define DMA_RXDESC_VLAN       0x00000400         /*!< When set, this bit indicates that the frame pointed to by this descriptor is a VLAN frame tagged by the
                                                    GMAC Core*/
#define DMA_RXDESC_FS         0x00000200         /*!< When set, this bit indicates that this descriptor contains the first buffer of the frame. If the size of the first
                                                    buffer is 0, the second buffer contains the beginning of the frame */ 
#define DMA_RXDESC_LS         0x00000100         /*!< When set, this bit indicates that the buffers pointed to by this descriptor are the last buffers of the frame */

#define DMA_RXDESC_IPC        0x00000080         /*!< When set, this bit indicates that the 16-bit IPv4 Header checksum calculated by the core did not match
                                                    the received checksum bytes.  */
#define DMA_RXDESC_LC         0x00000040         /*!< When set, this bit indicates that a late collision has occurred while receiving the
                                                    frame in Half-Duplex mode */
#define DMA_RXDESC_FT         0x00000020         /*!< When set, this bit indicates that the Receive Frame is an Ethernet-type frame */

#define DMA_RXDESC_RWT        0x00000010         /*!< When set, this bit indicates that the Receive Watchdog Timer has expired while receiving the current
                                                    frame and the current frame is truncated after the Watchdog Timeout */
#define DMA_RXDESC_RE         0x00000008         /*!< When set, this bit indicates that the gmii_rxer_i signal is asserted while gmii_rxdv_i is asserted during
                                                    frame reception */
#define DMA_RXDESC_DE         0x00000004         /*!< When set, this bit indicates that the received frame has a non-integer multiple of bytes */
#define DMA_RXDESC_CE         0x00000002         /*!< When set, this bit indicates that a Cyclic Redundancy Check (CRC) Error occurred on the received
                                                    frame*/
#define DMA_RXDESC_CRCERR     0x00000001         /*!< When set, this bit indicates that the Rx MAC Address registers value (1 to 15)
                                                    matched the frame’s DA field*/


/*!< RDES1  */


#define DMA_RXDESC_DIOC       0x80000000         /*!< Disable Interrupt on Completion */
#define DMA_RXDESC_RER        0x02000000         /*!< When set, this bit indicates that the descriptor list reached its final descriptor */
#define DMA_RXDESC_RCH        0x01000000          /*!< When set, this bit indicates that the second address in the descriptor is the Next Descriptor address
                                                    rather than the second buffer address */


/*!< Transmit DMA Descriptors configuration macros*/

/*!< TDES0 */

#define DMA_TXDESC_OWN        0x80000000         /*!< When set, this bit indicates that the descriptor is owned by the DMA */
#define DMA_TXDESC_IHE        0x00010000         /*!< When set, this bit indicates that the Checksum Offload engine detected an IP header error*/ 
#define DMA_TXDESC_JT         0x00004000         /*!< When set, this bit indicates the GMAC transmitter has experienced a jabber time-out */  
#define DMA_TXDESC_FF         0x00002000         /*!< When set, this bit indicates that the DMA/MTL flushed the frame due to a SW flush command given by
                                                      the CPU */
#define DMA_TXDESC_PCE        0x00001000         /*!< This bit, when set, indicates that the Checksum Offload engine had a failure and did not insert any
                                                      checksum into the encapsulated TCP, UDP, or ICMP payload */
#define DMA_TXDESC_LC         0x00000800         /*!< When set, this bit indicates that Loss of Carrier occurred during frame transmission */
#define DMA_TXDESC_NC         0x00000400         /*!< When set, this bit indicates that the carrier sense signal form the PHY was not asserted during
                                                      transmission */
#define DMA_TXDESC_LTC        0x00000200         /*!< When set, this bit indicates that frame transmission was aborted due to a collision occurring after the
                                                      collision window */
#define DMA_TXDESC_EC         0x00000100         /*!< When set, this bit indicates that the transmission was aborted after 16 successive collisions while
                                                      attempting to transmit the current frame.*/
#define DMA_TXDESC_VF         0x00000080         /*!< When set, this bit indicates that the transmitted frame was a VLAN-type frame*/
#define DMA_TXDESC_ED         0x00000004         /*!< When set, this bit indicates that the transmission has ended because of excessive deferral of over
                                                      24,288 bit times  */
#define DMA_TXDESC_UF         0x00000002         /*!< When set, this bit indicates that the GMAC aborted the frame because data arrived late
                                                      from the Host memory  */
#define DMA_TXDESC_DB         0x00000001         /*!< When set, this bit indicates that the GMAC defers before transmission because 
                                                      of the presence of carrier  */


/*!< TDES1 */

#define DMA_TXDESC_IC         0x80000000         /*!< Interrupt on Completion */
#define DMA_TXDESC_LS         0x40000000         /*!< When set, this bit indicates that the buffer contains the last segment of the frame */
#define DMA_TXDESC_FS         0x20000000         /*!< When set, this bit indicates that the buffer contains the first segment of a frame */
#define DMA_TXDESC_DC         0x04000000         /*!< When set, the GMAC does not append the Cyclic Redundancy Check (CRC) to the end of the
                                                      transmitted frame */ 
#define DMA_TXDESC_TER        0x02000000         /*!< When set, this bit indicates that the descriptor list reached its final descriptor */
#define DMA_TXDESC_TCH        0x01000000         /*!< When set, this bit indicates that the second address in the descriptor is the Next Descriptor address
                                                      rather than the second buffer address */
#define DMA_TXDESC_DP         0x00800000         /*!< When set, the GMAC does not automatically add padding to a frame shorter than 64 bytes */


/*******************************************************************************/
/************************** GMAC Registers**************************************/
/*******************************************************************************/



/*!< MAC configuration register macros */

#define MAC_DISABLE_WATCHDOG       0x00800000   /*!< When this bit is set, the GMAC disables the watchdog timer on the receiver, and
                                                     can receive frames of up to 16,384 bytes*/
#define MAC_DISABLE_JABBER         0x00400000   /*!< When this bit is set, the GMAC disables the jabber timer on the transmitter, and can
                                                     transfer frames of up to 16,384 bytes*/
#define MAC_ENABLE_JUMBO_FRAME     0x00100000   /*!< When this bit is set, GMAC allows Jumbo frames of 9,018 bytes (9,022 bytes for
                                                     VLAN tagged frames*/
#define MAC_DISABLE_DCRS           0x00010000   /*!< Disable Carrier Sense During Transmission */
#define MAC_FES_100                0x00004000   /*!< Indicates the speed in Fast Ethernet (MII) mode*/
#define MAC_DISABLE_DO             0x00002000   /*!< When this bit is set, the GMAC disables the reception of frames*/
#define MAC_ENABLE_LM              0x00001000   /*!< When this bit is set, the GMAC operates in loop-back mode at GMII/MII */
#define MAC_ENABLE_FULL_DUPLEX     0x00000800   /*!< When this bit is set, the GMAC operates in a Full-Duplex mode where it can
                                                     transmit and receive simultaneously*/
#define MAC_ENABLE_IPC             0x00000400   /*!< when this bit is set, the GMAC calculates the 16-bit 1’s complement of the 1’s
                                                     complement sum of all received Ethernet frame payloads (16-bit)*/ 
#define MAC_DISABLE_RETRY          0x00000200   /*!< When this bit is set, the GMAC will attempt only 1 transmission*/
#define MAC_ENABLE_DC              0x00000010   /*!< When this bit is set, the deferral check function is enabled in the GMAC  */
#define MAC_ENABLE_TE              0x00000008   /*!< When this bit is set, the transmit state machine of the GMAC is enabled for transmission on the GMII/MII */
#define MAC_ENABLE_RE              0x00000004   /*!< When this bit is set, the receiver state machine of the GMAC is enabled forreceiving frames from the GMII/MII*/


/*!< MAC Frame Filter */

#define MAC_FILTER_RA          0x80000000       /*!< When this bit is set, the GMAC Receiver module passes to the Application all
                                                     frames received irrespective of whether they pass the address filter*/  
#define MAC_FILTER_HPF         0x00000400       /*!< When set, this bit configures the address filter to pass a frame if it matches
                                                     either the perfect filtering or the hash filtering as set by HMC or HUC bits*/
#define MAC_FILTER_SAF         0x00000200       /*!< The GMAC core compares the SA field of the received frames with the values
                                                     programmed in the enabled SA registers */ 
#define MAC_FILTER_SAIF        0x00000100       /*!< When this bit is set, the Address Check block operates in inverse filtering
                                                     mode for the SA address comparison*/ 
#define MAC_FILTER_DBF         0x00000020       /*!< When this bit is set, the AFM module filters all incoming broadcast frames*/
#define MAC_FILTER_PM          0x00000010       /*!< When set, this bit indicates that all received frames with a multicast*/ 
#define MAC_FILTER_DAIF        0x00000008       /*!< When this bit is set, the Address Check block operates in inverse filtering
                                                     mode for the DA address comparison for both unicast and multicast frames*/
#define MAC_FILTER_HMC         0x00000004       /*!< When set, MAC performs destination address filtering of received multicast
                                                     frames according to the hash table */
#define MAC_FILTER_HUC         0x00000002       /*!< When set, MAC performs destination address filtering of unicast frames
                                                     according to the hash table */
#define MAC_FILTER_PR          0x00000001       /*!< When this bit is set, the Address Filter module passes all incoming frames
                                                     regardless of its destination or source address */



/*!< Flow Control Register */

#define GMAC_FLOW_CNTL_DZPQ    0x00000040       /*!< When set, this bit disables the automatic generation of Zero-Quanta
                                                     Pause Control frames on the deassertion of the flow-control signal from
                                                     the FIFO layer */

#define GMAC_FLOW_CNTL_UP      0x00000008       /*!< When this bit is set, the GMAC will detect the Pause frames with the
                                                     station’s unicast address specified in MAC Address0 High Register 
                                                     and MAC Address0 Low Register */
#define GMAC_FLOW_CNTL_RFE     0x00000004       /*!< When this bit is set, the GMAC will decode the received Pause frame and
                                                     disable its transmitter for a specified (Pause Time) time */ 

#define GMAC_FLOW_CNTL_TFE     0x00000002       /*!< In Full-Duplex mode, when this bit is set, the GMAC enables the flow
                                                     control operation to transmit Pause frames*/

#define GMAC_FLOW_CNTL_FCB     0x00000001       /*!< This bit initiates a Pause Control frame in Full-Duplex mode and activates
                                                     the backpressure function in Half-Duplex mode if TFE bit is set*/
/*!< GMAC Interrupts*/

#define   GMAC_PMT_INTR            0x00000008        /*!< PMT Interrupt Mask , This bit when set, will disable the assertion of the interrupt signal */
/*!< Interrupt Status Register */

#define   GMAC_INT_STATUS_MMC_CRC  0x00000080        /*!< MMC Receive Checksum Offload Interrupt Status 0 RO
                                                          This bit is set high whenever an interrupt is generated in the MMC Receive
                                                          Checksum Offload Interrupt Register */   
#define   GMAC_INT_STATUS_MMCT     0x00000040        /*!< This bit is set high whenever an interrupt is generated in the MMC Transmit
                                                          Interrupt Register */
#define   GMAC_INT_STATUS_MMCR     0x00000020        /*!< This bit is set high whenever an interrupt is generated in the MMC Receive
                                                          Interrupt Register */
#define   GMAC_INT_STATUS_MMC      0x00000010        /*!< This bit is set high whenever any of bits 7:5 is set high and cleared only 
                                                          when all of these bits are low */
#define   GMAC_INT_STATUS_PMT      0x00000008        /*!< This bit is set whenever a Magic packet or Wake-on-LAN frame is received in
                                                          Power-Down mode*/
#define   GMAC_INT_STATUS_PCS_ANC  0x00000004        /*!< This bit is set when the Auto-negotiation is completed in the TBI/RTBI/SGMII PHY
                                                          interface  */
#define   GMAC_INT_STATUS_PCS_LSC  0x00000002        /*!< This bit is set due to any change in Link Status in the TBI/RTBI/SGMII PHY
                                                          interface */
#define   GMAC_INT_STATUS_RGMII    0x00000001        /*!< This bit is set due to any change in value of the Link Status of RGMII interface*/



/*!< Power control macros*/
#define ETH_POWER_OFF           0          /*!< Power off: no operation possible */ 
#define ETH_POWER_LOW           1          /*!< Low Power mode: retain state, detect and signal wake-up events */
#define ETH_POWER_FULL          2          /*!< Power on: full operation at maximum performance */

/*!< Ethernet link state*/
#define RSI_ETH_LINK_DOWN 	    0          /*!< Link is down */
#define RSI_ETH_LINK_UP 	      1          /*!< Link is up */



#define RSI_CONFIG_MAC          0 
#define RSI_CONFIG_FLOWCONTRL   1



#define POWER_DOWN                  0x00000001     /*!< When set, all received frames will be dropped. This bit is cleared
                                                         automatically when a magic packet or Wake-Up frame is received */
#define WAKEUP_ON_MAGIC_PACKET      0x00000002     /*!< When set, enables generation of a power management event due to Magic Packet reception*/
#define WAKEUP_ON_WAKEUP_FRAME      0x00000004     /*!< Wake-Up Frame Enable When set, enables generation of a power management event due to wake- up frame reception*/
#define WAKEUP_FILTER_RESET         0x80000000     /*!< Wake-Up Frame Filter Register Pointer Reset*/





/******************************************************
 * *                    Constants
 * ******************************************************/

/******************************************************
 * *                   Enumerations
 * ******************************************************/



/******************************************************
 * *                 Type Definitions
 * ******************************************************/
typedef struct RSI_ETH_DMA_TXDESC_s 
{

 uint32_t TXDESC0;
 uint32_t TXDESC1;
 uint32_t TXDESC2;
 uint32_t TXDESC3;

}RSI_ETH_DMA_TXDESC_t;

 typedef union STATUS_REG5_BITS{
    __IO uint32_t  STATUS_REG5;                     /*!< The Status register contains all the status bits that the DMA
                                                         reports to the host                                                   */
    
    struct {
      __IO  uint32_t  TI         :  1;               /*!< Transmit Interrupt.This bit indicates that frame transmission
                                                         is finished and TDES1 31 is set inthe First Descriptor.               */
      __IO  uint32_t  TPS        :  1;               /*!< Transmit Process Stopped                                              */
      __IO  uint32_t  TU         :  1;               /*!< Transmit Buffer Unavailable                                           */
      __IO  uint32_t  TJT        :  1;               /*!< Transmit Jabber Time out                                              */
      __IO  uint32_t  OVF        :  1;               /*!< Receive Overflow                                                      */
      __IO  uint32_t  UNF        :  1;               /*!< Transmit Underflow                                                    */
      __IO  uint32_t  RI         :  1;               /*!< Receive Interrupt                                                     */
      __IO  uint32_t  RU         :  1;               /*!< Receive Buffer Unavailable                                            */
      __IO  uint32_t  RPS        :  1;               /*!< Receive Process Stopped                                               */
      __IO  uint32_t  RWT        :  1;               /*!< Receive Watchdog Time out                                             */
      __IO  uint32_t  ETI        :  1;               /*!< Early Transmit Interrupt                                              */
           uint32_t             :  2;
      __IO  uint32_t  FBI        :  1;               /*!< Fatal Bus Error Interrupt                                             */
      __IO  uint32_t  ERI        :  1;               /*!< Early Receive Interrupt                                               */
      __IO  uint32_t  AIS        :  1;               /*!< Abnormal Interrupt Summary                                            */
      __IO  uint32_t  NIS        :  1;               /*!< Normal Interrupt Summary                                              */
      __I  uint32_t  RS         :  3;               /*!< Receive Process State                                                 */
      __I  uint32_t  TS         :  3;               /*!< Transmit Process State                                                */
      __I  uint32_t  EB_23      :  1;               /*!< Error Bits.These bits indicate the type of error that caused
                                                         a Bus Error                                                           */
      __I  uint32_t  EB_24      :  1;               /*!< Error Bits.These bits indicate the type of error that caused
                                                         a Bus Error                                                           */
      __I  uint32_t  EB_25      :  1;               /*!< Error Bits.These bits indicate the type of error that caused
                                                         a Bus Error                                                           */
      __I  uint32_t  GLI        :  1;               /*!< GMAC Line interface Interrupt                                         */
      __I  uint32_t  GMI        :  1;               /*!< GMAC MMC Interrupt                                                    */
      __I  uint32_t  GPI        :  1;               /*!< GMAC PMT Interrupt                                                    */
    } STATUS_REG5_b;                                /*!< BitSize                                                               */
  }STATUS_REG5_BITS_T;
  

typedef struct RSI_ETH_DMA_RXDESC_s 
{
  #if 0 
	union{
		  uint32_t RXDESC0;
		 struct{
			 uint32_t  CRC   : 1;
			 uint32_t  CE    : 1;
			 uint32_t  DE    : 1;
			 uint32_t  RE    : 1;
			 uint32_t  RWT   : 1;
			 uint32_t  FT    : 1;
			 uint32_t  LC    : 1;
       uint32_t  IPC   : 1;
			 uint32_t  LS    : 1;
			 uint32_t  FS    : 1;
			 uint32_t  VLAN  : 1;
			 uint32_t  OE    : 1;
			 uint32_t  LE    : 1;
			 uint32_t  SAF    : 1;
			 
			 
		 }RXDESC0_b;
	 };
	#endif
uint32_t RXDESC0;
 uint32_t RXDESC1;
 uint32_t RXDESC2;
 uint32_t RXDESC3;
  

}RSI_ETH_DMA_RXDESC_t;



/*!< Ethernet link information*/ 
typedef struct RSI_ETH_PHY_GetLinkInfo_s
{

  uint8_t speed; /*!< Link speed: 0= 10 MBit, 1= 100 MBit */ 

  uint8_t mode;  /*!< Duplex mode: 0= Half, 1= Full */

}RSI_ETH_PHY_GetLinkInfo_t;



typedef struct RSI_ETH_MAC_TIME_s
{
  uint32_t 	ns; 	/*!< Nano seconds */

  uint32_t 	sec; 	/*!< Seconds */ 

}RSI_ETH_MAC_TIME_t;






typedef struct RSI_ETH_DMA_RxXferConfig_s
{
  void           *dst; 			       	/*!< DMA destination address */
  uint8_t        fireDescCB;	    	/*!< DMA interrupt fires after this descriptor is complete and calls descriptor complete callback */
  uint16_t       recvCount;         /*!< Data length to receive*/
}RSI_ETH_DMA_RxXferConfig_t;


typedef struct RSI_ETH_DMA_TxXferConfig_s
{
	void           *src;			       	/*!< DMA source address */
	uint16_t       xferCount;	      	/*!< DMA transfer size in size of data width */
	uint8_t        fireDescCB;	    	/*!<  DMA interrupt fires after this descriptor is complete and calls descriptor complete callback */
  uint8_t        sof;               /*!< if 1 - Start of frame  */                    
  uint8_t        eof;               /*!< if 1 - end of frame  */                    
  uint8_t        checksum;          /*!< 0 - no insertion of checksum
                                         1 - Insert IPv4 header checksum
                                         2 - Insert TCP/UDP/ICMP checksum
                                         3 - Insert a TCP/UDP/ICMP checksum that is fully calculated in this engine*/
  uint8_t        dc;                /*!< Disable CRC,When set, the GMAC does not append the Cyclic Redundancy Check (CRC) to the end of the
                                         transmitted frame*/
}RSI_ETH_DMA_TxXferConfig_t;





typedef struct RSI_ETH_PMT_WakeupFrameFilter_s
{
  uint32_t byte_mask[4]; /*!< This register defines which bytes of the frame are examined by filter i (0, 1, 2, and 3) in order to
    determine whether or not the frame is a wake-up frame. The MSB (thirty-first bit) must be zero. Bit j
   [30:0] is the Byte Mask. If bit j (byte number) of the Byte Mask is set, then Filter i Offset + j of the
  incoming frame is processed by the CRC block; otherwise Filter i Offset + j is ignored. */
  uint8_t  command[4];
  uint8_t  offset[4];
  uint16_t crc[4];
}RSI_ETH_PMT_WakeupFrameFilter_t;



/*!<Driver UART instance  */
typedef void *RSI_ETH_HANDLE_T;           


/*!<Ethernet data structure */
typedef struct 
{

  ETHERNET_Type *pREGS;                                                     /*!< Structure pointer for Ethernet register instance */

  void (*cbTable[MAX_ENET_CALL_BACKS])(RSI_ETH_HANDLE_T ,uint32_t, void *);   /* Callback */

}RSI_ETH_DRIVER_T;




/******************************************************
 * *                    Structures
 * ******************************************************/



/******************************************************
 * *                 Global Variables
 * ******************************************************/
/******************************************************
 * *               Function Declarations
 * ****************************************************/

void RSI_ETH_SetVlanTag(RSI_ETH_HANDLE_T pETHHandle, uint16_t VLANTag);
void RSI_ETH_ResetVlanTag(RSI_ETH_HANDLE_T pETHHandle);
void RSI_ETH_TXEnable(RSI_ETH_HANDLE_T pETHHandle);
void RSI_ETH_TXDisable(RSI_ETH_HANDLE_T pETHHandle);
void RSI_ETH_RXEnable(RSI_ETH_HANDLE_T pETHHandle);
void RSI_ETH_RXDisable(RSI_ETH_HANDLE_T pETHHandle);
void RSI_ETH_EnableRXFilter(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask);
void RSI_ETH_DisableRXFilter(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask);
void RSI_ETH_SetHashFilter(RSI_ETH_HANDLE_T pETHHandle, uint32_t HashFilterL, uint32_t HashFilterH);
void RSI_ETH_GMACEnableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask);
void RSI_ETH_DisableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask);
void RSI_ETH_MAC_EnableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask);
void RSI_ETH_MAC_DisableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask);
RSI_ETH_HANDLE_T RSI_ETH_MAC_Initialize(void *drv_mem, uint32_t base_address);
error_t RSI_ETH_MAC_Uninitialize 	( 	void  		); 	
error_t RSI_ETH_MAC_SetMacAddress(RSI_ETH_HANDLE_T pETHHandle, int8_t *mac); 	
error_t RSI_ETH_MAC_GetMacAddress(RSI_ETH_HANDLE_T pETHHandle,  int8_t *mac);	
error_t RSI_ETH_MAC_ResetAddressFilter(RSI_ETH_HANDLE_T pETHHandle,uint32_t num_addr); 	
error_t RSI_ETH_MAC_SetSourceAddressFilter(RSI_ETH_HANDLE_T pETHHandle, const uint8_t *mac_addr,uint32_t num_addr, uint8_t addr_mask); 	
error_t RSI_ETH_MAC_SetDestinationAddressFilter(RSI_ETH_HANDLE_T pETHHandle,const uint8_t *mac_addr, uint32_t 	num_addr, uint8_t addr_mask);
error_t RSI_ETH_MAC_DMAInit(RSI_ETH_HANDLE_T pETHHandle) ;
error_t RSI_ETH_MAC_BuildRXDescChain(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_RxXferConfig_t *rcvcfg, RSI_ETH_DMA_RXDESC_t  *pDesc, RSI_ETH_DMA_RXDESC_t  *pNextDesc); 
error_t RSI_ETH_MAC_BuildTXDescChain(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_TxXferConfig_t *xfercfg, RSI_ETH_DMA_TXDESC_t  *pDesc, RSI_ETH_DMA_TXDESC_t  *pNextDesc); 
error_t RSI_ETH_MAC_StopTx(RSI_ETH_HANDLE_T pETHHandle);
error_t RSI_ETH_MAC_StartTx(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_TXDESC_t  *pDesc);
error_t RSI_ETH_MAC_StartRx(RSI_ETH_HANDLE_T pETHHandle, RSI_ETH_DMA_RXDESC_t  *pDesc);
error_t RSI_ETH_MAC_RegisterCallBacks(RSI_ETH_HANDLE_T pETHHandle, uint32_t callback_index, void (*eth_cb)(RSI_ETH_HANDLE_T , uint32_t event , void *arg) );
error_t RSI_ETH_MAC_StopRx(RSI_ETH_HANDLE_T pETHHandle);
error_t RSI_ETH_MAC_Control(RSI_ETH_HANDLE_T pETHHandle, uint32_t  control, uint32_t	arg); 		
void RSI_ETH_PHY_Initialize(RSI_ETH_HANDLE_T pETHHandle);
void RSI_ETH_PHY_UnInitialize(RSI_ETH_HANDLE_T pETHHandle);
error_t RSI_ETH_PHY_PowerControl(RSI_ETH_HANDLE_T pETHHandle, int8_t enable, int8_t  control); 
error_t RSI_ETH_PHY_SetWakeUpFilter(RSI_ETH_HANDLE_T pETHHandle,RSI_ETH_PMT_WakeupFrameFilter_t *filter); 
uint32_t RSI_ETH_DMA_GetTXStatus(RSI_ETH_HANDLE_T pETHHandle);
uint32_t RSI_ETH_DMA_GetRXStatus(RSI_ETH_HANDLE_T pETHHandle);
void RSI_ETH_DMAIntrHandler(RSI_ETH_HANDLE_T pETHHandle);
void RSI_ETH_PMTIntrHandler(RSI_ETH_HANDLE_T pETHHandle);
void RSI_ETH_DMA_EnableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask);
void RSI_ETH_DMA_DisableInt(RSI_ETH_HANDLE_T pETHHandle, uint32_t mask);
error_t RSI_ETH_MAC_SetOPerMode(RSI_ETH_HANDLE_T pETHHandle, uint32_t control);

#ifdef __cplusplus
}
#endif


#endif


