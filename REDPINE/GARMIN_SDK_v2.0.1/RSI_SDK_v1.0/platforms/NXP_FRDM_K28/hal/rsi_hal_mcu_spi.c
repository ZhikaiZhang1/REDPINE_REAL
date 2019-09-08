/**
 * @file       rsi_hal_mcu_spi.c
 * @version    0.1
 * @date       18 sept 2015
 *
 * Copyright(C) Redpine Signals 2015
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief: HAL SPI API
 *
 * @Description:
 * This file Contains all the API's related to HAL 
 *
 */


/**
 * Includes
 */

#include "rsi_driver.h"


#ifdef FRDM_K28F
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_dspi.h"
#include "fsl_dspi_edma.h"
/**
 * Global Variables
 */
 
#define EXAMPLE_DSPI_MASTER_BASEADDR SPI0
#define EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER kDSPI_MasterPcs0

extern dspi_master_edma_handle_t g_dspi_edma_m_handle;
extern volatile bool isTransferCompleted;
#endif



#ifdef FRDM_K28F
/*==================================================================*/
/**
 * @fn         int16_t rsi_spi_transfer(uint8_t *ptrBuf,uint16_t bufLen,uint8_t *valBuf,uint8_t mode)
 * @param[in]  uint8_t *tx_buff, pointer to the buffer with the data to be transfered
 * @param[in]  uint8_t *rx_buff, pointer to the buffer to store the data received
 * @param[in]  uint16_t transfer_length, Number of bytes to send and receive
 * @param[in]  uint8_t mode, To indicate mode 8 BIT/32 BIT mode transfers.
 * @param[out] None
 * @return     0, 0=success
 * @section description  
 * This API is used to tranfer/receive data to the Wi-Fi module through the SPI interface.
 */
int16_t rsi_spi_transfer(uint8_t *tx_buff, uint8_t *rx_buff, uint16_t transfer_length,uint8_t mode)
{
    dspi_transfer_t masterXfer;

    isTransferCompleted = false;

	/* Start master transfer */
	masterXfer.txData = tx_buff;
	masterXfer.rxData = rx_buff;
	masterXfer.dataSize = transfer_length;
	masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;

#ifndef SPI_DMA
	DSPI_MasterTransferBlocking(EXAMPLE_DSPI_MASTER_BASEADDR, &masterXfer);
#else
	status_t status = kStatus_Success;

    while(g_dspi_edma_m_handle.state == kDSPI_Busy)
    {

    }

	status = DSPI_MasterTransferEDMA(EXAMPLE_DSPI_MASTER_BASEADDR, &g_dspi_edma_m_handle, &masterXfer);
	if (kStatus_Success != status)
	{
		//PRINTF("There is error when start DSPI_MasterTransferEDMA \r\n ");
		return -1;
	}

	/* Wait until transfer completed */
	while(!isTransferCompleted)
	{
	}

#endif
  return 0;
}
#endif

