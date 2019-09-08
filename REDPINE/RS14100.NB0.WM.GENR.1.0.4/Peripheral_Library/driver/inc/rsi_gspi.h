/**
 * @file       rsi_gspi.h
 * @version    0.1
 * @date       20 dec 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes releated to gspi peripheral
 *
 * @section Description
 * This file contains the list of function prototypes for the gspi and low level function definations
 * Following are list of API's which need to be defined in this file.
 *
 */

 #ifndef __RSI_GSPI_H_
#define __RSI_GSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

	
#include "RS1xxxx.h"
#include "rsi_error.h"	
#include "rsi_packing.h"

/** 
 *\cond HIDDEN_SYMBOLS
 */
typedef void *GSPI_HANDLE_T;

/**
 * Macro defines for GSPI MANUAL STATUS Register
 */

#define GSPI_MANUAL_STATUS_BITMASK   0x07AF   /*!< GSPI Manual Status reg bitmask*/
#define GSPI_BUSY_F                  0x01     /*!< A read,write or dummy cycle operation is in process in manual mode*/
#define GSPI_FIFO_FULL_WFIFO_S       0x02     /*!< Full status indication for Wfifo in manual mode*/
#define GSPI_FIFO_AFULL_WFIFO_S      0x04     /*!< AFull status indication for Wfifo in manual mode*/
#define GSPI_FIFO_EMPTY_WFIFO_S      0x08     /*!< Empty status indication for Wfifo in manual mode*/
#define GSPI_FIFO_EMPTY_RFIFO_S      0x80     /*!< Empty status indication for Rfifo in manual mode*/
#define GSPI_FIFO_AEMPTY_WFIFO_S     0x100    /*!< AEmpty status indication for Rfifo in manual mode*/
#define GSPI_MAN_CSN                 0x400    /*!< Status of chip select signal*/

/**
 * Macro defines for GSPI INTR MASK Register
 */
#define GSPI_INTR_MASK_BITMASK         0x7F 	/*!< GSPI Interrupt mask reg bitmask*/
#define GSPI_INTR_MASK_BIT             0x01 	/*!< Mask the GSPI intr*/
#define GSPI_FIFO_AEMPTY_RFIFO_MASK    0x02 	/*!< read fifo almost empty intr mask */
#define GSPI_FIFO_AFULL_RFIFO_MASK     0x04		/*!< read fifo almost full intr mask*/
#define GSPI_FIFO_AEMPTY_WFIFO_MASK    0x08	  /*!< write fifo almost empty intr mask*/
#define GSPI_FIFO_AFULL_WFIFO_MASK     0x10 	/*!< Write Fifo almost full intr mask*/
#define GSPI_FIFO_FULL_WFIFO_MASK      0x20   /*!< write fifo is full intr mask*/
#define GSPI_FIFO_EMPTY_RFIFO_MASK     0x40   /*!< read fifo is empty intr mask*/
/**
 * Macro defines for GSPI INTR UNMASK Register
 */
#define GSPI_INTR_UNMASK_BITMASK         0x7F   /*!< GSPI Interrupt Unmask reg bitmask*/
#define GSPI_INTR_UNMASK_BIT             0x01   /*!< Unmask the GSPI intr*/
#define GSPI_FIFO_AEMPTY_RFIFO_UNMASK    0x02   /*!< read fifo almost empty intr unmask */
#define GSPI_FIFO_AFULL_RFIFO_UNMASK     0x04   /*!< read fifo almost full intr unmask*/
#define GSPI_FIFO_AEMPTY_WFIFO_UNMASK    0x08   /*!< write fifo almost empty intr unmask*/
#define GSPI_FIFO_AFULL_WFIFO_UNMASK     0x10   /*!< Write Fifo almost full intr unmask*/
#define GSPI_FIFO_FULL_WFIFO_UNMASK      0x20   /*!< write fifo is full intr unmask*/
#define GSPI_FIFO_EMPTY_RFIFO_UNMASK     0x40   /*!< read fifo is empty intr unmask*/

#define SPI_CSN                   0x0001   /*!<  CSN Enable */
#define SPI_WR                    0x0002   /*!<  Write enable*/
#define SPI_RD                    0x0004   /*!<  Read Enable*/
#define SPI_RD_LEN                0x1ff8   /*!< Length of the Read data*/
#define SPI_CSN_SEL               0x6000   /*!< chip select signal number*/
#define SPI_RD_LEN_POS             3       /*!< read length position*/
#define SPI_CSN_SEL_POS            13      /*!< Chip select position*/

/**
 * Macro defines for SPI Specific Functionality
 */
#define FULL_TIME_CLK                 0x1     /*!< Full time Clock Enable for GSPI Controller */
#define DYNAMIC_CLK                 0x0   /*!< Dynamic Clock Enable for GSPI Controller */
#define GSPI_BUS_CLK_ENABLE		     		0x1     /*!< Bus Clock Enable for GSPI Controller */
#define POS_EDGE_CLK                  0x00    /*!< PosEdge Clock Enabling*/ 
#define NEG_EDGE_CLK                  0x01    /*!< NegEdge Clock Enabling */ 
#define M4_SOC_CLK_FOR_OTHER_CLOCK   ((~(0x7))<<24)	  /*!< Enabling the M4 SOC Clock for GSPI Controller */
#define HOST_PADS_GPIO_PIN             0xF6000   /*!< Selecting the host pads for GPIO Pins */
#define ENABLE_SW_BASED_HOST1          0x0400    /*!< Enabling the Software based host 1 Selection */
#define ENABLE_SW_BASED_HOST2          0x4000    /*!< Enabling the Software based host 2 Selection */

typedef GSPI0_Type RSI_GSPI_T;
/**
 * @brief	Event handler ID passed during a call-back event
 */
typedef enum {
	SPIM_EVT_ASSERT,			  /*!< Event identifier for Slave Assert */
	SPIM_EVT_DEASSERT,			/*!< Event identifier for Slave deassert */
	SPIM_EVT_TXDONE,				/*!< Event identifier for TX complete */
	SPIM_EVT_RXDONE,				/*!< Event identifier for RX complete */
} SPIM_EVENT_T;
/**
** @brief SPI Master XFER states 
*/
#define SPIM_XFER_STATE_IDLE         0	/*!< SPI XFER is IDLE */
#define SPIM_XFER_STATE_BUSY         1	/*!< SPI XFER is busy transfering data */
#define SPIM_XFER_STATE_DONE         2	/*!< SPI XFER is complete */

/** @brief	SPI master xfer options */
#define SPIM_XFER_OPTION_EOT         (1 << 0x1F)		       	  /*!< SPI SLAVE Select will be deasserted when xfer is done */
#define SPIM_XFER_OPTION_EOF         (1 << 5)		    	        /*!< Insert a frame delay */
#define SPIM_XFER_OPTION_SIZE(x)     ((((x) - 1) & 0xF) << 8)	/*!< Number of bits in transfer data */


/* ROM driver handle for SPI master */

#define FULL_TIME_CLOCK_ENABLE  1
#define FULL_TIME_CLOCK_DISABLE (!FULL_TIME_CLOCK_ENABLE)
#define GSPIM_MAXSELECTS        4


/** @brief SPI master optional transfer flags */
#define GSPI_FLAG_BLOCKING      (1UL << 31)		/*!< Transfer function will block until complete */
#define GSPI_FLAG_NONBLOCKING   (1UL << 30)		/*!< Transfer function will block until complete */
#define GSPI_FLAG_DMATX         (1UL << 29)		/*!< DMA will be used for TX, requires DMA setup outside of the driver */
#define GSPI_FLAG_DMARX         (1UL << 28)		/*!< DMA will be used for RX, requires DMA setup outside of the driver */
#define GSPI_FLAG_TXIGNORE      (1UL << 26)		/*!< Ignores receive data regardless of txBuff, transmit data callback will not be called, stops when txSz = 0 */
#define GSPI_FLAG_RXIGNORE      (1UL << 25)		/*!< Ignores receive data regardless of rxBuff/rxSz, receive data callback will not be called */
#define GSPI_FLAG_USERBITS      (0xFFFF)		/*!< Application can safely use the flag bits designated by this mask */

typedef PRE_PACK struct POST_PACK {
	uint32_t    base;					/*!< Base address of SPI peripheral to use */
	uint32_t    baseClockRate;			/*!< SPI base clock rate in Hz, call Init() again if this rate changes */
	uint8_t     spiMode;				/*!< 0: for SPI mode 3 , 1 for SPI mode 0 */
  uint8_t     fullDuplexMode;
	uint8_t     dataSampleEdge;
  uint16_t    fifoAfull;
  uint16_t    fifoAEmpty;
  uint8_t     fullTimeClkEn;       /*!<0 – Dynamic clock gating is enabled in side GSPI controller. */
                                        /*!<1 – Full time clock is enabled for GSPI controller.  */
  uint8_t     clockSync;              /*!< If the clock frequency to FLASH (spi_clk) and SOC clk is same.
                                             1: SCLK clock and SOC clock are same.
                                             0: Divided SOC clock is connected SCLK. Division value is programmable.*/
} GSPI_INIT_T;

/** @brief SPI Master transfer descriptor */
typedef PRE_PACK struct POST_PACK {
	const void                 *txBuff;	/*!< Pointer to array of datum to be transmitted, use NULL for receive only, must be 16-bit aligned if datum bits >8 */
	uint32_t                    txSz;		/*!< Size of the transmit buffer (txBuff) in items (not bytes) before transmit callback is called */
	void                       *rxBuff;	/*!< Pointer memory where datum received from SPI be stored, use NULL for transmit only, must be 16-bit aligned if datum bits >8 */
	uint32_t                    rxSz;		/*!< Size of the receive buffer (rxBuff) in items (not bytes) before receive callback is called */
	uint32_t                    flags;		/*!< Optional transfer flags, may be modified by driver */
	error_t                     status;		/*!< Status of the current SPI transfer (ErrorCode_t), must be 32-bits */
	uint8_t                     sselNum;	/*!< SPI master SSEL number for this transfer, 0 - 3 */
} GSPI_XFER_T;

/** @brief SPI Master transfer descriptor */
typedef PRE_PACK struct POST_PACK {
	uint8_t                 sselNum;	/*!< SPI master SSEL number for this transfer, 0 - 3 */
	uint8_t                 dataBits;		/*!< Data transfer size in bits, between 1 and 16, txBuff and rxBuff must be 16-bit aligned if >8 */
	uint8_t                 writeSwap;	    /*!< */
	uint8_t                 readSwap;	    /*!< */
	uint32_t                dXferBitRate;	/*!< Desired bit transfer rate (SPI clock) in Hz */
	uint32_t                rXferBitRate;	/*!< Real bit transfer rate (SPI clock) in Hz, modified by driver to actual rate */
	uint32_t                rdDataLen;   /*!< Number of bytes contineously read from FIFO*/
} GSPI_XFER_CONFIG_T;

/** @brief SPI master callback IDs */
typedef enum {
	GSPIM_ASSERTSSEL_CB = 0,			/*!< Callback ID for SPI master SSEL assertion */
	GSPIM_DATATRANSMIT_CB,			  /*!< Callback ID for SPI master transmit start */
	GSPIM_DATATRECEIVE_CB,			  /*!< Callback ID for SPI master receive start */
	GSPIM_DEASSERTSSEL_CB		    	/*!< Callback ID for SPI master SSEL de-assertion */
} GSPIM_CALLBACK_T;




/** @brief SPI master select assert callback
 * This callback is called from the SPI master handler when the SPI master
 * selects the slave (asserts SSEL). The callback is only called if it has been registered.<br>
 */
typedef void (*spiMasterXferCSAssertCB)(GSPI_HANDLE_T spimHandle, GSPI_XFER_T *pXfer);

/** @brief SPI master data receive start callback
 * The SPI master state machine will call this optional function when it needs a buffer to
 * receive data. The callback can be used to setup DMA or alter the receive buffer size. The
 * callback is only called if it has been registered, the buffer has been fully used, and
 * data is needed to populate the transmit FIFO. When using DMA, this callback must be enabled
 * and used for DMA setup.
 */
typedef void (*spiMasterReceiveCB)(GSPI_HANDLE_T spimHandle, GSPI_XFER_T *pXfer);

/** @brief SPI master data transmit start callback
 * The SPI master state machine will call this optional function for data transfer.
 * The callback can be used to setup DMA or alter the receive buffer size. The callback is
 * only called if it has been registered, the buffer has been fully used, and data is pending
 * is the receive FIFO. When using DMA, this callback must be enabled and used for DMA setup.
 */
typedef void (*spiMasterTransmitCB)(GSPI_HANDLE_T spimHandle, GSPI_XFER_T *pXfer);

/** @brief SPI master transfer select deassert data callback
 * This callback is called from the SPI master handler when the SPI master
 * deasserts the slave select. The callback is only called if it has been registered.<br>
 */
typedef void (*spiMMasterXferCSDeAssertCB)(GSPI_HANDLE_T spimHandle, GSPI_XFER_T *pXfer);

/**
 * \endcond
 */


/*========================================================================*/
/*========================================================================*/

/**************************************************************/
/*								GSPI FUNCTION PROTOTYPES											*/
/**************************************************************/

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_GSPI_DRIVER RSI:RS1xxxx GSPI 
 *  @{
 *
 */
error_t RSI_Clock_EnablePeriphClock(GSPI_HANDLE_T pHandle,uint8_t fulltime_clk_en);
void RSI_GSPI_CsAssert(GSPI_HANDLE_T pHandle , uint8_t  csNum);
void RSI_GSPI_CsDeAssert(GSPI_HANDLE_T pHand,uint8_t  csNum);
uint32_t RSI_GSPI_GetStatus(GSPI_HANDLE_T pHandle);
error_t RSI_GSPI_EnableInts(GSPI_HANDLE_T pHandle, uint8_t intMask);
error_t RSI_GSPI_DisableInts(GSPI_HANDLE_T pHandle, uint8_t intMask);
uint8_t RSI_GSPI_GetEnabledInts(GSPI_HANDLE_T pHandle);
void RSI_GSPI_FlushFifos(GSPI_HANDLE_T pHandle);
uint32_t RSI_GSPI_GetMemSize(void);
void RSI_GSPI_Close_PendingTransfer(GSPI_HANDLE_T pHandle);
error_t RSI_GSPI_SetWriteSwapData(GSPI_HANDLE_T pHandle);
error_t RSI_GSPI_ClrWriteSwapData(GSPI_HANDLE_T pHandle);
error_t RSI_GSPI_SetReadSwapData(GSPI_HANDLE_T pHandle);
error_t RSI_GSPI_ClrReadSwapData(GSPI_HANDLE_T pHandle);
void RSI_GSPI_SetClockRate(GSPI_HANDLE_T pHandle);
GSPI_HANDLE_T RSI_GSPI_Init(void *mem, const GSPI_INIT_T *pInit);
void RSI_GSPI_RegisterCallback(GSPI_HANDLE_T pHandle, uint32_t cbIndex, void *pCB);
error_t RSI_GSPI_SetUpTransfer(GSPI_HANDLE_T pHandle, GSPI_XFER_CONFIG_T *pCfg);
error_t RSI_GSPI_Transfer(GSPI_HANDLE_T pHandle, GSPI_XFER_T *pXfer);
void RSI_GSPI_TransferHandler(GSPI_HANDLE_T pHandle);
void RSI_GSPI_Send(GSPI_HANDLE_T pHandle,uint32_t len_in_bits, uint32_t data, uint8_t cs_no);
uint32_t RSI_GSPI_Receive(GSPI_HANDLE_T pHandle, uint32_t len_in_bits, uint8_t cs_no);
/* @}end of RSI_GSPI_DRIVER */

#ifdef __cplusplus
}
#endif
#endif /* __RSI_GSPI_H_ */



