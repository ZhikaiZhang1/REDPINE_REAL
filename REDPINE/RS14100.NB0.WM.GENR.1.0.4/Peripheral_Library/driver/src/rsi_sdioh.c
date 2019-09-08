 /**
 * @file       rsi_sdioh.c
 * @version    1.0
 * @date       10th AUG 2018
 *
 * Copyright(C) Redpine Signals 2018
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes related to SDIOH peripheral
 * 
 * @section Description
 * This file contains the list of function prototypes for the SDIOH and low level function definitions
 * Following are list of API's which need to be defined in this file.
 *
 */

/**
 * Includes
 */
#include "rsi_chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
extern void SysTick_Handler(void);
volatile uint32_t 		_dwTickCount=0 ;		//systick cout variable

//! System tick timer initialization
void SysTick_Handler(void)
{
	_dwTickCount++;
}

//! returns the systick current tick count
uint32_t GetTickCount( void )
{
	return _dwTickCount ;					
}

/**
 * @fn           void rsi_delay_ms(uint32_t delay_ms)
 * @brief        This API is used create delay in  milli seconds.              
 * @param[in]    delay_ms  : timer delay in milli seconds
 * @return       None              
 */
void rsi_delay_ms(uint32_t delay_ms)
{
	uint32_t start;

	if (delay_ms == 0)
		return;
	start = GetTickCount();
	do {
	} while (GetTickCount() - start < delay_ms);
	return ;
}


/*****************************************************************************
 * Public functions
 ****************************************************************************/
static SDIOH_CMD_T                               *pCurrentCommand        = 0;
static SDIOH_COMMAND_CONFIG_STRUCT_T          *pCommandConfig         = 0;
static SDIOH_COMMAND_CONFIG_STRUCT_T           CommandConfig;
static SDIOH_ADMA_DESC_TABLE_T ADMADescriptTable[2] = { 0 };


/****************************SDIOH COMMAND FUNCTIONS************************************/

/**
 * @fn           status_t RSI_SDIOH_SdioCommandInitialization( SDIOH_COMMAND_CONFIG_STRUCT_T *pSdiohConfig )
 * @brief        This API is used to initialize the SDIO command configuration structure.              
 * @param[in]    pSdiohConfig              : Pointer to the IO card configuration
 * @return       RSI_SDIOH_OK                 IO command configuration structure initialized successfully
 *               RSI_SDIOH_INVALID_PARAMETER  In case of Invalid parameter              
 */
status_t RSI_SDIOH_SdioCommandInitialization( SDIOH_COMMAND_CONFIG_STRUCT_T *pSdiohConfig )
{
	if (pCommandConfig == NULL)
	{
		pCommandConfig = &CommandConfig;
	}

	if (pSdiohConfig !=NULL)
	{
		memcpy(pCommandConfig, pSdiohConfig, sizeof(SDIOH_COMMAND_CONFIG_STRUCT_T));
		return RSI_SDIOH_OK;
	}

	return RSI_SDIOH_ERROR;
}

/**
 * @fn         void sdiohCardInsertEventCB(void)
 * @brief      This is the card insert interrupt callback function.
 */
void sdiohCardInsertEventCB( void )
{
	//get the current command and set the flag.
	if (pCurrentCommand != NULL)
	{
		pCurrentCommand->event.cardInsert++;
	}
}

/**
 * @fn         void sdiohCardRemoveEventCB( void )
 * @brief      This is the card remove interrupt callback function.
 */
void sdiohCardRemoveEventCB( void )
{
	//get the current command and set the flag.
	if (pCurrentCommand != NULL)
	{
		pCurrentCommand->event.cardRemove++;
	}
}
/**
 * @fn         void sdiohBufferReadReadyEventCB(void)
 * @brief      This is the buffer read ready interrupt callback function.
 */
void sdiohBufferReadReadyEventCB( void )
{
	//get the current command and set the flag.
	if (pCurrentCommand != NULL)
	{
		pCurrentCommand->event.bufferReadReady++;
	}
}

/**
 * @fn         void sdiohBufferWriteReadyEventCB(void)
 * @brief      This is the buffer write ready interrupt callback function.
 */
void sdiohBufferWriteReadyEventCB( void )
{
	if (pCurrentCommand != NULL)
	{
		pCurrentCommand->event.bufferWriteReady++;
	}
}

/**
 * @fn         void sdiohCommandCompleteEventCB(void)
 * @brief      This is the command complete interrupt callback function.
 */
void sdiohCommandCompleteEventCB( void )
{
	if (pCurrentCommand !=NULL)
	{
		pCurrentCommand->event.commandComplete++;
	}
}

/**
 * @fn         void sdiohTransferCompleteEventCB(void)
 * @brief      This is the transfer complete interrupt callback function.
 */
void sdiohTransferCompleteEventCB( void )
{
	if (pCurrentCommand !=NULL)
	{
		pCurrentCommand->event.transferComplete++;
	}
}

/**
 * @fn         void sdiohAdmaCompleteEventCB(void)
 * @brief      This is the ADMA complete interrupt callback function.
 */
void sdiohAdmaCompleteEventCB(void)
{
	if (pCurrentCommand !=NULL)
	{
		pCurrentCommand->event.admaComplete++;
	}
}

/**
 * @fn         void sdiohCommandTimeoutEventCB(void)
 * @brief      This is the Command timeout interrupt callback function.
 */
void sdiohCommandTimeoutEventCB(void)
{
	//get the current command and set the flag.
	if (pCurrentCommand !=NULL)
	{
		pCurrentCommand->event.cmdTimeOut++;
	}
}

/**
 * @fn           status_t RSI_SDIOH_ClockEnable( uint32_t freq )
 * @brief        This API is used to change the host controller clock              
 * @param[in]    freq  : New frequency to the host
 * @return       Ok      If new frwquency is set
 */
status_t RSI_SDIOH_ClockEnable(SDIOH_CARD_CONFIG_T *pSdiohConfig, uint32_t freq )
{
	status_t Stat;
	uint16_t    u16Div        = 0;

	uint32_t  u32ClockInput = pSdiohConfig->clock;  

	u16Div   = u32ClockInput /2/  (freq);

	/*disables sdio clock*/
	Stat = sdioh_clock_disable();

	/*set division value to the card*/
	Stat = sdioh_clock_division_set(u16Div);

	/*Enable Smih internal clock*/
	Stat = sdioh_internal_clock_enable();

	/*enables sdio clock*/
	Stat = sdioh_clock_enable();

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_WaitForCmdLineFree( void )
 * @brief        This API is used wait for the command line to get free              
 * @return       RSI_SDIOH_OK            If command line is free within the time.
 *               RSI_SDIOH_ERROR_TIMEOUT If command line is busy after certain time.
 */
status_t RSI_SDIOH_WaitForCmdLineFree( void )
{
	status_t Stat    = RSI_SDIOH_ERROR_TIMEOUT;
	uint32_t    Retry_Time = RETRY_TIME;

	while(Retry_Time)
	{
		if (SDIOH->SMIH_PRESENT_STATE_REGISTER_b.COMMAND_INHIBIT_CMD == 0)
		{
			Stat = RSI_SDIOH_OK;
			break;
		}
		Retry_Time --;
	}

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_WaitForDataLineFree( void )
 * @brief        This API is used wait for the data line to get free              
 * @return       RSI_SDIOH_OK            If data line is free within the time.
 *               RSI_SDIOH_ERROR_TIMEOUT If data line is busy after certain time.
 */
status_t RSI_SDIOH_WaitForDataLineFree( void )
{
	status_t Stat    = RSI_SDIOH_ERROR_TIMEOUT;
	uint32_t    Retry_Time = RETRY_TIME * 30;

	while(Retry_Time)
	{
		if (SDIOH->SMIH_PRESENT_STATE_REGISTER_b.COMMAND_INHIBIT_DAT == 0)
		{
			Stat = RSI_SDIOH_OK;
			break;
		}
		Retry_Time --;
	}

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_CommandEventWaitAndReset( SDIOH_CMD_EVENT_T EventType )
 * @brief        This API is used check perticular event happens or not.              
 * @return       Ok                          If the event is occured.
 *               RSI_SDIOH_ERROR_TIMEOUT     If the event is not available within a period.
 *               RSI_SDIOH_INVALID_PARAMETER If the input parameter in invalid.
 */
status_t RSI_SDIOH_CommandEventWaitAndReset( SDIOH_CMD_EVENT_T EventType )
{
	status_t     Stat     = RSI_SDIOH_ERROR;  
	SDIOH_EVENT_T *pEvent = NULL;

	if (pCurrentCommand == NULL)
	{
		return RSI_SDIOH_INVALID_PARAMETER;
	}

	pEvent = &pCurrentCommand->event;

	while (Stat == RSI_SDIOH_ERROR)
	{
		switch ( EventType )
		{
		case SDIOH_CMD_COMPLETE_EVENT:
			if (pEvent->commandComplete > 0)
			{
				Stat = RSI_SDIOH_OK;
				pEvent->commandComplete--;
			}
			break;
		case SDIOH_TRANSFER_COMPLETE_EVENT:
			if (pEvent->transferComplete)
			{
				Stat = RSI_SDIOH_OK;
				pEvent->transferComplete--;
			}
			break;
		case SDIOH_READ_READY_EVENT:
			if (pEvent->bufferReadReady > 0)
			{
				Stat = RSI_SDIOH_OK;
				pEvent->bufferReadReady--;
			}
			break;
		case SDIOH_WRITE_READY_EVENT:
			if (pEvent->bufferWriteReady)
			{
				Stat = RSI_SDIOH_OK;
				pEvent->bufferWriteReady--;
			}
			break;

		case SDIOH_ADMA_COMPLETE_EVENT:
			if (pEvent->admaComplete)
			{
				Stat = RSI_SDIOH_OK;
				pEvent->admaComplete--;
			}
			break;

		default:

			break;
		}

		if (pEvent->cardRemove)
		{
			pEvent->cardRemove--;
			Stat = RSI_SDIOH_ERROR_ACCESS_RIGHTS;
		}
		else if (pEvent->transferTimeOut || pEvent->crcError || pEvent->cmdTimeOut)
		{
			Stat = RSI_SDIOH_ERROR_TIMEOUT;
		}

		// after interrupt, check card is stable or not
		if (sdioh_wait_for_card_insert() == 0)
		{
			Stat = RSI_SDIOH_ERROR_TIMEOUT;
		}
	}
	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_ReceiveData(SDIOH_CMD_T *pCommand)    
 * @brief        This API is used read the data from fifo
 * @param[in]    pCommand                 Pointer to the data structure     
 * @return       RSI_SDIOH_OK              If the data read properly.
 *               RSI_SDIOH_ERROR_TIMEOUT   If the read data time out occurs.              
 */
status_t RSI_SDIOH_ReceiveData( SDIOH_CMD_T *pCommand )
{
	status_t         Stat         = RSI_SDIOH_ERROR;
	uint32_t         blocksize    = 0;
	uint32_t         blockcnt     = 0;
	uint32_t         i            = 0;
	uint32_t         *pBuffer     = NULL;
	uint32_t         Retry_Time   = RETRY_TIME;

	blockcnt  = pCommand->blockCount;
	blocksize = pCommand->blockSize;
	pBuffer = (uint32_t *)pCommand->data;

	if (pCommandConfig->admaMode)
	{
		while (Stat != 0 && (Retry_Time-- > 0u))
		{
			Stat = RSI_SDIOH_CommandEventWaitAndReset(SDIOH_TRANSFER_COMPLETE_EVENT);
		}

		if (Stat != 0)
		{
			return Stat;
		}
	}
	else
	{
		Stat = RSI_SDIOH_ERROR_TIMEOUT;
		Retry_Time = RETRY_TIME;
		// dummy code.
		while (Stat != 0  && (Retry_Time-- > 0u))
		{
			Stat = RSI_SDIOH_CommandEventWaitAndReset(SDIOH_READ_READY_EVENT);
		}

		if (Stat != 0)
		{
			return Stat;
		}

		Retry_Time = RETRY_TIME;
		while (blockcnt > 0)
		{
			while (FALSE == SDIOH->SMIH_PRESENT_STATE_REGISTER_b.BUFFER_READ_ENABLE);

			if ( FALSE == SDIOH->SMIH_PRESENT_STATE_REGISTER_b.BUFFER_READ_ENABLE )
			{
				break;
			}
			for ( i = blocksize >> 2; i != 0; i-- )
			{
				*pBuffer++ = SDIOH->SMIH_BUFFER_DATA_PORT_REGISTER;
			}
			blockcnt--;
		}

		Stat = RSI_SDIOH_ERROR;

		Retry_Time = RETRY_TIME;

		while (Stat != 0)
		{
			Stat = RSI_SDIOH_CommandEventWaitAndReset(SDIOH_TRANSFER_COMPLETE_EVENT);
		}
	}

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_SendData( SDIOH_CMD_T *pCommand )   
 * @brief        This API is used to write the data to the Fifo
 * @param[in]    pCommand              Pointer to the data structure     
 * @return       RSI_SDIOH_OK                    If the data sent properly.
 *               ErrorTimeout          If the write data time out occurs.              
 */
status_t RSI_SDIOH_SendData( SDIOH_CMD_T *pCommand )
{
	status_t    Stat      = RSI_SDIOH_ERROR;
	uint32_t    blocksize    = 0;
	uint32_t    blockcnt     = 0;
	uint32_t    i            = 0;
	uint32_t    *pBuffer     = NULL;

	blockcnt = pCommand->blockCount;
	blocksize = pCommand->blockSize;
	pBuffer = (uint32_t *)pCommand->data;
	if (pCommandConfig->admaMode)
	{
		while (Stat != 0)
		{
			Stat = RSI_SDIOH_CommandEventWaitAndReset(SDIOH_TRANSFER_COMPLETE_EVENT);
		}
		if (Stat != 0)
		{
			return Stat;
		}
	}
	else
	{
		Stat = RSI_SDIOH_ERROR_TIMEOUT;

		//wait for read buffer is ready
		while (Stat != 0) 
		{
			Stat = RSI_SDIOH_CommandEventWaitAndReset(SDIOH_WRITE_READY_EVENT);
		}

		if (Stat != 0)
		{
			return Stat;
		}

		while (blockcnt > 0)
		{

			if (Stat != 0)
			{
				break;
			}

			while (0x0 == SDIOH->SMIH_PRESENT_STATE_REGISTER_b.BUFFER_WRITE_ENABLE); 

			if (0x0 == SDIOH->SMIH_PRESENT_STATE_REGISTER_b.BUFFER_WRITE_ENABLE)
			{
				Stat = RSI_SDIOH_ERROR_TIMEOUT;
				return Stat;
			}
			for (i = blocksize >> 2; i != 0; i--)
			{
				SDIOH->SMIH_BUFFER_DATA_PORT_REGISTER = (*pBuffer++);
			}
			blockcnt--;
		}

		while (Stat != 0) 
		{
			Stat = RSI_SDIOH_CommandEventWaitAndReset(SDIOH_TRANSFER_COMPLETE_EVENT);
		}

		if (Stat != 0)
		{
			return Stat;
		}
	}

	while (0x1 == (SDIOH->SMIH_PRESENT_STATE_REGISTER_b.DAT_LINE_ACTIVE)); 
	if ( 0x1 == (SDIOH->SMIH_PRESENT_STATE_REGISTER_b.DAT_LINE_ACTIVE) )
	{
		Stat = RSI_SDIOH_ERROR_TIMEOUT;

	}
	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_SendCommand( SDIOH_CMD_T *pCommand )   
 * @brief        This API is used to send the command.
 * @param[in]    pCommand                 Pointer to the data structure     
 * @return       RSI_SDIOH_ERROR           If Parameter is invalid.
 *               RSI_SDIOH_ERROR_TIMEOUT   If the command error timeout occures.              
 */
status_t RSI_SDIOH_SendCommand( SDIOH_CMD_T *pCommand )
{
	status_t             Stat         = RSI_SDIOH_ERROR;
	SDIOH_DATA_CONFIG_T     CmdDataCfg      = { 0 };
	SDIOH_COMMAND_CONFIG_T  CommandCfg      = { 0 };

	if (NULL == pCommandConfig)
	{
		Stat = RSI_SDIOH_INVALID_PARAMETER;
		return Stat;
	}

	memset(ADMADescriptTable, 0x0, sizeof(ADMADescriptTable));

	if (pCommandConfig->admaMode &&
			NULL != pCommand->data)
	{
		ADMADescriptTable[0].act = 2;
		ADMADescriptTable[0].attrEnd = 1;
		ADMADescriptTable[0].attrValid = 1;
		ADMADescriptTable[0].attrInt = 0;
		ADMADescriptTable[0].len = pCommand->blockSize * pCommand->blockCount;
		ADMADescriptTable[0].addr = (uint32_t)pCommand->data;
		CmdDataCfg.admaDespTableAddress = (uint32_t)&ADMADescriptTable[0];
	}

	Stat = RSI_SDIOH_WaitForCmdLineFree();
	if (Stat != 0)
	{
		return Stat;
	}

	Stat = RSI_SDIOH_WaitForDataLineFree();
	if (Stat != 0)
	{
		return Stat;
	}

	CmdDataCfg.blockSize = pCommand->blockSize;
	CommandCfg.argument = pCommand->cmdArg;
	CmdDataCfg.read = pCommand->direction;

	if (pCommandConfig->admaMode && NULL != pCommand->data)
	{
		CmdDataCfg.dmaEnable = TRUE;
	}
	// set the CMD register to issue the command.
	CommandCfg.dataPresent = (NULL == pCommand->data) ? 0 : 1;

	switch ( pCommand->responseType )
	{
	case SDIOH_RSP_NONE:
		CommandCfg.checkCmdCrc    = 0;
		CommandCfg.checkCmdIndex  = 0;
		CommandCfg.responseType   = SDIOH_RESPONSE_NONE;
		break;
	case SDIOH_RSP_R2:
		CommandCfg.checkCmdCrc    = 1;
		CommandCfg.checkCmdIndex  = 0;
		CommandCfg.responseType   = SDIOH_RESPONSE_136BIT;
		break;
	case SDIOH_RSP_R3R4:
		CommandCfg.checkCmdCrc    = 0;
		CommandCfg.checkCmdIndex  = 0;
		CommandCfg.responseType   = SDIOH_RESPONSE_48BIT;
		break;
	case SDIOH_RSP_R1NORMALR5R6R7:
		CommandCfg.checkCmdCrc   = 1;
		CommandCfg.checkCmdIndex  = 1;
		CommandCfg.responseType = SDIOH_RESPONSE_48BIT;
		break;
	case SDIOH_RSP_R1BNORMAL:
	case SDIOH_RSP_R1AUTO :
	case SDIOH_RSP_R5B:
		CommandCfg.checkCmdCrc  = 1;
		CommandCfg.checkCmdIndex  = 1;
		CommandCfg.responseType = SDIOH_RESPONSE_48BIT_BUSY_CHECK;
		break;
	default :
		return RSI_SDIOH_INVALID_PARAMETER;
	}

	CommandCfg.cmdIndex = pCommand->cmdIdx & 0x3F; ///< filter ACMD offset.
	if (CMD53 == CommandCfg.cmdIndex && (pCommand->cmdArg & (1 << 27)))
	{
		//multi-block mode.
		CmdDataCfg.multiBlock = 1;
		CmdDataCfg.enableBlockCount = 1;
		CommandCfg.autoCmdType = DISABLE_AUTO_CMD ;
		CmdDataCfg.blockCount = pCommand->blockCount;
	}
	else
	{
		//single block mode.
		CmdDataCfg.multiBlock = 0;
		CmdDataCfg.enableBlockCount = 0;
		CommandCfg.autoCmdType = DISABLE_AUTO_CMD ;
		CmdDataCfg.blockCount = pCommand->blockCount;
	}
	if (pCurrentCommand != NULL)
	{
		return RSI_SDIOH_OPERATION_INPROGRESS;
	}

	// hard code
	CommandCfg.cmdType = NORMAL_CMD;
	CmdDataCfg.dataTimeout = 0xc;

	// set current command to global value for callback function.
	pCurrentCommand = pCommand;

	// send the command
	sdioh_data_transfer_initialization(&CmdDataCfg);

	sdioh_send_command(&CommandCfg);

	// wait command line to be free.
	Stat = RSI_SDIOH_CommandEventWaitAndReset(SDIOH_CMD_COMPLETE_EVENT);
	if (Stat != 0)
	{
		pCurrentCommand = NULL;
		return Stat;
	}
	// read response data.
	Stat = sdioh_get_response((uint16_t *)pCommand->response, sizeof(pCommand->response) / 2);

	if (NULL != pCommand->data)
	{
		switch ( pCommand->cmdIdx )
		{
		case CMD53:
			if((CommandCfg.argument )& (1 << 31))
			{
				Stat = RSI_SDIOH_SendData(pCommand);
			}
			else
			{
				Stat = RSI_SDIOH_ReceiveData(pCommand);                
			}
			break;
		default:
			break;
		}
	}

	memset(ADMADescriptTable, 0x0, sizeof(ADMADescriptTable));

	pCurrentCommand = NULL;

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_RegisterInfo(SDIOH_CCCR_REG_INFO_T *pRegInfo)  
 * @brief        This API is used to enable high speed of the card. 
 * @return       RSI_SDIOH_OK     if Command sent successfully
 */
status_t RSI_SDIOH_RegisterInfo(SDIOH_CCCR_REG_INFO_T *pRegInfo)
{
	SDIOH_CMD_T CmdData           = { 0 };
	status_t Stat                 = RSI_SDIOH_ERROR;
	uint8_t cccr[22]              = {0};

	CmdData.cmdIdx               = CMD53;
	CmdData.cmdArg               = CCCR_BYTE_READ;
	CmdData.data                 = cccr;
	CmdData.blockSize            = 0x16;
	CmdData.blockCount           = 1;
	CmdData.responseType         = SDIOH_RSP_R1NORMALR5R6R7;
	CmdData.direction            = SDIOH_READ_DIR;

	Stat = RSI_SDIOH_SendCommand(&CmdData);

	memset((void *)pRegInfo, 0x0, 22);
	memcpy((void *)pRegInfo, &CmdData.data, 22);

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_SendCommand52(SDIOH_CMD_INFO_T *pcmdInfo,uint32_t Argument)   
 * @brief        This API is used to send the CMD52 to the card.
 * @param[in]    pcmdInfo               Pointer to the command information strut
 * @param[in]    Argument               Argument to the command
 * @return       RSI_SDIOH_OK            In case of command sent succesfullt
                 RSI_SDIOH_ERROR         If Parameter is invalid.
 *               RSI_SDIOH_ERROR_TIMEOUT If the command error timeout occures.              
 */
status_t RSI_SDIOH_SendCommand52(SDIOH_CMD_INFO_T *pcmdInfo,uint32_t Argument)
{
	SDIOH_CMD_T CmdData          = { 0 };
	uint32_t   cmdArg            = Argument;
	status_t   Stat              = RSI_SDIOH_ERROR;

	CmdData.cmdIdx               = CMD52;
	CmdData.cmdArg               = cmdArg;
	CmdData.data                 = 0;
	CmdData.blockSize            = 0;
	CmdData.blockCount           = 0;
	CmdData.responseType         = SDIOH_RSP_R1NORMALR5R6R7;
	CmdData.direction            = SDIOH_WRITE_DIR; 

	/*Send command */
	Stat = RSI_SDIOH_SendCommand(&CmdData);

	memset((void *)&pcmdInfo->response[0], 0x0, sizeof(pcmdInfo->response));
	memcpy((void *)&pcmdInfo->response[0], &CmdData.response[0], sizeof(pcmdInfo->response));

	return Stat;

}

/**
 * @fn           status_t RSI_SDIOH_BusWidthConfig( SDIOH_CMD_INFO_T *pcmdInfo, SDIOH_BUS_MODE_T BusWidth )   
 * @brief        This API is used to change the bus width.
 * @param[in]    pcmdInfo              Pointer to the command information strut
 * @param[in]    BusWidth              Width of the bus
 *               \n                    possible values are
 *               \n                    SDIOH_BUS_WIDTH1     = 0u,
 *               \n                    SDIOH_BUS_WIDTH4     = 1u,
 *               \n                    SDIOH_BUS_WIDTH8     = 2u,
 * @return       RSI_SDIOH_OK           If  bus width set properly
 *               RSI_SDIOH_ERROR        If Parameter is invalid.
 */
status_t RSI_SDIOH_BusWidthConfig( SDIOH_CMD_INFO_T *pcmdInfo, SDIOH_BUS_MODE_T BusWidth )
{
	status_t Stat         = RSI_SDIOH_ERROR;
	uint32_t    cmdArg        = 0;
	boolean_t   bHost4BitMode = FALSE;

	switch ( BusWidth )
	{
	case SDIOH_BUS_WIDTH1:
		cmdArg = 0;                
		bHost4BitMode = FALSE;
		break;
	case SDIOH_BUS_WIDTH4:
		cmdArg  = BIT4_BUS_WIDTH_ARG;                 
		bHost4BitMode = TRUE;
		break;
	default:
		return RSI_SDIOH_INVALID_PARAMETER;
	}

	if (RSI_SDIOH_SetBusWidthCmd52(pcmdInfo, cmdArg) == RSI_SDIOH_OK)
	{
		/* host side setting */
		Stat = sdioh_bus_width_set(bHost4BitMode);
	}
	return Stat;
}

/**
 * @fn           RSI_SDIOH_GoIdleStateCmd0(void)   
 * @brief        This API is used to send the CMD0 to the io card.
 * @return       Ok                    If  command sent successfully
 *               ErrorTimeout          If the command error timeout occures.              
 */
status_t RSI_SDIOH_GoIdleStateCmd0( void )
{
	SDIOH_CMD_T  CmdData     = { 0 };
	status_t     Stat        = RSI_SDIOH_ERROR;
	CmdData.cmdIdx           = CMD0;
	CmdData.cmdArg           = 0x0;
	CmdData.data             = NULL;
	CmdData.blockSize        = 0x0;
	CmdData.blockCount       = 0x0;
	CmdData.responseType     = SDIOH_RSP_NONE;
	CmdData.direction        = SDIOH_WRITE_DIR;

	/*Send Command*/
	Stat = RSI_SDIOH_SendCommand(&CmdData);

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_SendRelativeAddr_Cmd3( cmd_info_t *pcmdInfo, uint32_t CmdArg )   
 * @brief        This API is used to send the CMD3(Get relative card address) from the IO card.
 * @param[in]    pcmdInfo              Pointer to the command info struct
 *               CmdArg                Command argument to send
 * @return       Ok                    If  command sent successfully
 *               ErrorTimeout          If the command error timeout occures.              
 */
status_t RSI_SDIOH_SendRelativeAddressCmd3( SDIOH_CMD_INFO_T *pcmdInfo, uint32_t CmdArg )
{
	SDIOH_CMD_T   CmdData  = { 0 };
	status_t     Stat  = RSI_SDIOH_ERROR;

	CmdData.cmdIdx        = CMD3;
	CmdData.cmdArg        = 0;
	CmdData.data          = NULL;
	CmdData.blockSize     = 0;
	CmdData.blockCount    = 0;
	CmdData.responseType  = SDIOH_RSP_R1NORMALR5R6R7;
	CmdData.direction     = SDIOH_WRITE_DIR;

	/*Send Command*/
	Stat = RSI_SDIOH_SendCommand(&CmdData);

	if (CmdArg ==0x0)
	{
		// set rca
		pcmdInfo->rca = CmdData.response[0] >> 16;
	}

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_SwitchVoltageCmd11( SDIOH_CMD_INFO_T *pcmdInfo, uint32_t CmdArg )   
 * @brief        This API is used to send the CMD11(Voltage switch command in case of uhs modes) to the io card.
 * @param[in]    pcmdInfo              Pointer to the command info struct
 *               CmdArg                Command argument to send
 * @return       Ok                    If  command sent successfully
 *               ErrorTimeout          If the command error timeout occures.              
 */
status_t RSI_SDIOH_SwitchVoltageCmd11( SDIOH_CMD_INFO_T *pcmdInfo, uint32_t CmdArg )
{
	SDIOH_CMD_T   CmdData   = { 0 };
	status_t     Stat   = RSI_SDIOH_ERROR;

	CmdData.cmdIdx         = CMD11;
	CmdData.cmdArg         = 0;
	CmdData.data           = NULL;
	CmdData.blockSize      = 0;
	CmdData.blockCount     = 0;
	CmdData.responseType   = SDIOH_RSP_R1NORMALR5R6R7;
	CmdData.direction      = SDIOH_WRITE_DIR;

	/* Send Command */
	Stat = RSI_SDIOH_SendCommand(&CmdData);

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_SelectCard_Cmd7(uint32_t u32RCA)   
 * @brief        This API is used to send the CMD7( select the cards) to the IO card.
 * @param[in]    u32RCA                Relative card address to the selected card
 * @return       Ok                    If  command sent successfully
 *               ErrorTimeout          If the command error timeout occures.              
 */
status_t RSI_SDIOH_SelectCardCmd7(uint32_t u32RCA)
{
	SDIOH_CMD_T CmdData = { 0 };
	status_t     Stat  = RSI_SDIOH_ERROR;
	CmdData.cmdIdx = CMD7;
	CmdData.cmdArg = u32RCA << 16;
	CmdData.data = NULL;
	CmdData.blockSize = 0;
	CmdData.blockCount = 0;
	CmdData.responseType = SDIOH_RSP_R1BNORMAL;
	CmdData.direction = SDIOH_WRITE_DIR;
	Stat = RSI_SDIOH_SendCommand(&CmdData);
	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_SetBusWidthCmd52( SDIOH_CMD_INFO_T *pcmdInfo, uint32_t CmdArg)   
 * @brief        This API is used to send the CMD52 to set bus width to the IO card.
 * @param[in]    pcmdInfo              Pointer to the command info struct
 * @param[in]    CmdArg                Command Argument
 * @return       Ok                    If  command sent successfully
 *               ErrorTimeout          If the command error timeout occures.              
 */
status_t RSI_SDIOH_SetBusWidthCmd52( SDIOH_CMD_INFO_T *pcmdInfo, uint32_t CmdArg )
{
	SDIOH_CMD_T     CmdData  = { 0 };
	status_t       Stat  = RSI_SDIOH_ERROR;

	CmdData.cmdIdx          = CMD52;
	CmdData.cmdArg          = CmdArg;
	CmdData.data            = NULL;
	CmdData.blockSize       = 0;
	CmdData.blockCount      = 0;
	CmdData.responseType    = SDIOH_RSP_R1NORMALR5R6R7;
	CmdData.direction       = SDIOH_WRITE_DIR;

	/*Send Command*/
	Stat = RSI_SDIOH_SendCommand(&CmdData);

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_ByteBlockWriteCmd53(SDIOH_CARD_CONFIG_T *pSdiohConfig,uint8_t *pBuffer ,uint32_t Addr)  
 * @brief        This API is used to send the CMD53 to write data to the IO card.
 * @param[in]    pSdiohConfig              Pointer to the sdio config struct
 * @param[in]    pBuffer                Pointer to the buffer data to write
 * @param[in]    Addr                     Address to write the data
 * @return       Ok                       If  command sent successfully
 *               ErrorTimeout             If the command error timeout occures.              
 */
status_t RSI_SDIOH_ByteBlockWriteCmd53(SDIOH_CARD_CONFIG_T *pSdiohConfig,uint8_t *pBuffer ,uint32_t Addr)
{
	SDIOH_CMD_T    CmdData  = { 0 };
	status_t      Stat  = RSI_SDIOH_ERROR;

	if ((NULL == pBuffer ))
	{
		return RSI_SDIOH_INVALID_PARAMETER;
	}

	CmdData.cmdIdx    = CMD53;   
	if(pSdiohConfig->blockModeEnable == 1)
	{
		CmdData.cmdArg  = (pSdiohConfig->numberOfBlocks);
	}
	else
	{
		CmdData.cmdArg  = (pSdiohConfig->byteBlockSize); 
	}
	CmdData.cmdArg        |= (((Addr) & 0x0001FFFF) << 9);
	CmdData.cmdArg        |= (pSdiohConfig->opCode << 26);
	CmdData.cmdArg        |= (pSdiohConfig->blockModeEnable << 27);
	CmdData.cmdArg        |= (pSdiohConfig->funcNum << 28);
	CmdData.cmdArg        |= (((1 << 0)) << 31);
	CmdData.data           = pBuffer ;
	CmdData.blockSize      = pSdiohConfig->byteBlockSize;
	CmdData.blockCount     = pSdiohConfig->numberOfBlocks;		
	CmdData.responseType   = SDIOH_RSP_R1NORMALR5R6R7;
	CmdData.direction      = SDIOH_WRITE_DIR;

	/*Send Command*/
	Stat = RSI_SDIOH_SendCommand(&CmdData);

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_ByteBlockReadCmd53(SDIOH_CARD_CONFIG_T *pSdiohConfig,uint8_t *pBuffer  ,uint32_t Addr)   
 * @brief        This API is used to send the CMD53 to read data from the IO card.
 * @param[in]    pSdiohConfig             Pointer to the sdio config struct
 * @param[in]    pBuffer                  Pointer to the buffer data to read
 * @param[in]    Addr                     Address to read the data
 * @return       Ok                       If  command sent successfully
 *               ErrorTimeout             If the command error timeout occures.
 */
status_t RSI_SDIOH_ByteBlockReadCmd53(SDIOH_CARD_CONFIG_T *pSdiohConfig,uint8_t *pBuffer  ,uint32_t Addr)
{
	SDIOH_CMD_T      CmdData = { 0 };
	status_t       Stat  = RSI_SDIOH_ERROR;

	if ((NULL == pBuffer ))
	{
		return RSI_SDIOH_INVALID_PARAMETER;
	}

	CmdData.cmdIdx    = CMD53;   

	if(pSdiohConfig->blockModeEnable == 1)
	{
		CmdData.cmdArg  = (pSdiohConfig->numberOfBlocks);
	}
	else
	{
		CmdData.cmdArg  = (pSdiohConfig->byteBlockSize); 
	}

	CmdData.cmdArg      |= (((Addr) & 0x0001FFFF) << 9);
	CmdData.cmdArg      |= (pSdiohConfig->opCode << 26);
	CmdData.cmdArg      |= (pSdiohConfig->blockModeEnable << 27);
	CmdData.cmdArg      |= (pSdiohConfig->funcNum << 28);
	CmdData.cmdArg      |= (((0 << 0)) << 31);
	CmdData.data         = pBuffer ;
	CmdData.blockSize    = pSdiohConfig->byteBlockSize;
	CmdData.blockCount   = pSdiohConfig->numberOfBlocks;
	CmdData.responseType = SDIOH_RSP_R1NORMALR5R6R7;
	CmdData.direction    = SDIOH_READ_DIR;

	/*Send Command */
	Stat = RSI_SDIOH_SendCommand(&CmdData);

	return Stat;
}

/***************************SDIOH CARD ENUMERATION FUNCTIONS******************************/
/**
 * @fn           status_t RSI_SDIOH_ReInitializationCmd5( SDIOH_CMD_INFO_T *pcmdInfo,SDIOH_CARD_CONFIG_T *pSdiohConfig) 
 * @brief        This API is used to send the CMD5 to reinilialize the SDIO card.
 * @param[in]    pcmdInfo                 Pointer to the command information struct
 * @param[in]    pSdiohConfig              Pointer to the sdio config struct
 * @return       Ok                       If  command sent successfully
 *               ErrorTimeout             If the command error timeout occures.
 */
status_t RSI_SDIOH_ReInitializationCmd5( SDIOH_CMD_INFO_T *pcmdInfo,SDIOH_CARD_CONFIG_T *pSdiohConfig)
{
	SDIOH_CMD_T     CmdData  = { 0 };
	status_t       Stat  = RSI_SDIOH_ERROR;

	CmdData.cmdIdx        = CMD5;
	CmdData.cmdArg        = *(uint32_t *)(&pcmdInfo->ocr);
	if(pSdiohConfig->voltage ==1)
	{
		//voltage switch request
		CmdData.cmdArg       |=0x01100000;
	}
	else
	{
		CmdData.cmdArg  |=0x00100000;
	}  
	CmdData.data          = NULL;
	CmdData.blockSize     = 0;
	CmdData.blockCount    = 0;
	CmdData.responseType  = SDIOH_RSP_R3R4;
	CmdData.direction     = SDIOH_WRITE_DIR;

	/*Send Command*/
	Stat = RSI_SDIOH_SendCommand(&CmdData);

	if (RSI_SDIOH_OK == Stat)
	{
		memcpy((void *)&pcmdInfo->ocr, &CmdData.response[0], sizeof(pcmdInfo->ocr));
	}
	return Stat;		    
}

/**
 * @fn           status_t RSI_SDIOH_Initialization_Cmd5( cmd_info_t *pcmdInfo)
 * @brief        This API is used send the CMD5 to inilialize the SDIO card.
 * @param[in]    pcmdInfo                 Pointer to the command information struct
 * @return       Ok                       If  command sent successfully
 *               ErrorTimeout             If the command error timeout occures.
 */
status_t RSI_SDIOH_InitializationCmd5( SDIOH_CMD_INFO_T *pcmdInfo)
{
	SDIOH_CMD_T      CmdData  = { 0 };
	status_t        Stat  = RSI_SDIOH_ERROR;

	CmdData.cmdIdx           = CMD5;
	CmdData.cmdArg           = *(uint32_t *)&pcmdInfo->ocr;
	CmdData.cmdArg           = (CmdData.cmdArg & 0x00000000);
	CmdData.data             = NULL;
	CmdData.blockSize        = 0;
	CmdData.blockCount       = 0;
	CmdData.responseType     = SDIOH_RSP_R3R4;
	CmdData.direction        = SDIOH_WRITE_DIR;

	/*Send Command*/
	Stat = RSI_SDIOH_SendCommand(&CmdData);

	if (RSI_SDIOH_OK == Stat)
	{
		memcpy((void *)&pcmdInfo->ocr, &CmdData.response[0], sizeof(pcmdInfo->ocr));
	}

	return Stat;
}

/**
 * @fn           status_t RSI_SDIOH_Enumeration(SDIOH_CARD_CONFIG_T *pSdiohConfig)
 * @brief        This API is used to initialize the card 
 * @param[in]    pSdiohConfig               : Pointer to the sdioh card configuration
 * @return       RSI_SDIOH_OK                : IO card initialized successfully
 * @return       RSI_SDIOH_INVALID_PARAMETER : if  pSdiohConfig==0
 */
status_t RSI_SDIOH_Enumeration(SDIOH_CARD_CONFIG_T *pSdiohConfig )
{
	status_t         Stat  = RSI_SDIOH_ERROR;
	SDIOH_CONFIG_T                             Config           = { 0 };
	SDIOH_ERROR_INTR_CB_T                      ErrIrqCb         = { 0 };
	SDIOH_ERROR_INTR_ENABLE_T                  ErrIrqEnable     = { 0 };
	SDIOH_NORMAL_INTR_ENABLE_T                 normalIntrEnable  = { 0 };
	SDIOH_NORMAL_INTR_CB_T                     normalIntrCb      = { 0 };
	SDIOH_COMMAND_CONFIG_STRUCT_T              CommandCfg;
	SDIOH_CMD_INFO_T *SmihInfo = NULL;
	if (pSdiohConfig == 0)
	{
		return RSI_SDIOH_INVALID_PARAMETER;
	}

	// Clear structure
	memset(&CommandCfg, 0, sizeof(CommandCfg));

	/*sleepclock prog*/
	*(volatile uint32_t *) (0x46000024) = (0x0<<21);

	/*wait for clock switch */
	while((M4CLK->PLL_STAT_REG_b.SLEEP_CLK_SWITCHED) != 1);

	//Wait for card insert
	while (sdioh_wait_for_card_insert()==0);

	// Initialize interrupts
	ErrIrqEnable.errorIntrStatus.commandTimeoutError = 1;
	ErrIrqCb.pCommandTimeoutErrorIntrCb    = sdiohCommandTimeoutEventCB;

	normalIntrCb.pCommandCompleteIntrCb    = sdiohCommandCompleteEventCB;
	normalIntrCb.pTransferCompleteIntrCb   = sdiohTransferCompleteEventCB;
	normalIntrCb.pBufferReadReadyIntrCb    = sdiohBufferReadReadyEventCB;
	normalIntrCb.pBufferWriteReadyIntrCb   = sdiohBufferWriteReadyEventCB;
	normalIntrCb.pCardRemovalIntrCb        = sdiohCardRemoveEventCB;    

	normalIntrEnable.sdiohNormalIntrstatus.commandComplete  = 1;
	normalIntrEnable.sdiohNormalIntrstatus.transferComplete = 1;
	normalIntrEnable.sdiohNormalIntrstatus.bufferReadReady  = 1;
	normalIntrEnable.sdiohNormalIntrstatus.bufferWriteReady = 1;
	normalIntrEnable.sdiohNormalIntrstatus.cardRemoval      = 1;  

	// Initialize configuration
	Config.enable4BitMode      = pSdiohConfig->busWidthMode;
	Config.highSpeedEnable     = pSdiohConfig->highSpeedEnable;
	Config.uhsMode             = pSdiohConfig->uhsModeSelect;
	Config.selectAdma          = pSdiohConfig->admaMode;
	Config.errorIntrCb         = &ErrIrqCb;
	Config.errorIntrEnable     = &ErrIrqEnable;
	Config.normalIntrCb        = &normalIntrCb;
	Config.normalIntrEnable    = &normalIntrEnable;

	/* Initialize SDIOH controller */
	Stat = sdioh_host_initialization(&Config);

	sdioh_bus_voltage_select(VOLTAGE_33V);

	sdioh_bus_power_on();

	Stat = RSI_SDIOH_ClockEnable(pSdiohConfig,SDIOH_CLOCK_400KHZ);

	if (Stat != 0)
	{
		return Stat;
	}

	CommandCfg.highSpeedEnable = pSdiohConfig->highSpeedEnable;
	CommandCfg.admaMode = pSdiohConfig->admaMode;
	CommandCfg.busWidthMode = pSdiohConfig->busWidthMode;
	CommandCfg.clock = pSdiohConfig->clock;

	Stat = RSI_SDIOH_SdioCommandInitialization(&CommandCfg);
	if (Stat != 0)
	{
		return Stat;
	}
	else
	{
		/*Commands for sdio enumeration*/
		SmihInfo = &pSdiohConfig->infoFromCard;

		init:
		//Reset Card
		Stat = RSI_SDIOH_GoIdleStateCmd0();

		if (Stat != 0)
		{
			goto init;
		}

		//(CMD5 )Get Operational voltage
		Stat = RSI_SDIOH_InitializationCmd5(SmihInfo);

		if (Stat != 0)
		{
			goto init;
		}

		//(CMD5 )Set Operational & get ocr
		Stat = RSI_SDIOH_ReInitializationCmd5(SmihInfo,pSdiohConfig);
		if (Stat != 0)
		{
			return Stat;
		}

		if(!(SmihInfo->ocr.cardReady))
		{
			goto init;
		}

		if((SmihInfo->ocr.memPresent))
		{
			return CARD_TYPE_MEMCARD;
		}

		if(!(SmihInfo->ocr.ioNum))
		{
			return RSI_SDIOH_INAVLID_MODE;
		}

		if(pSdiohConfig->highSpeedEnable == 1)      
		{
			M4CLK->SD_MEM_CLOCK_REG_b.SD_MEM_INTF_CLK_SEL = 0x7;

			rsi_delay_ms(5);

			sdioh_bus_speed_mode_set(1); 

			RSI_CLK_SdMemClkConfig(M4CLK ,1,SDMEM_SOCPLLCLK,1);

			rsi_delay_ms(2);
		}
		if(pSdiohConfig->uhsModeSelect != 0) 
		{
			//works only with 1.8 volatge level    
			if(!(SmihInfo->ocr.s18A))
			{
				return RSI_SDIOH_INAVLID_MODE;
			}

			Stat = RSI_SDIOH_SwitchVoltageCmd11(SmihInfo,0x0);
			if (Stat != 0)
			{
				return Stat;
			}

			M4CLK->SD_MEM_CLOCK_REG_b.SD_MEM_INTF_CLK_SEL = 0x7;

			rsi_delay_ms(5);

			sdioh_18v_signal_enable();

			sdioh_uhs_mode_select(pSdiohConfig->uhsModeSelect);

			RSI_CLK_SdMemClkConfig(M4CLK ,1,SDMEM_SOCPLLCLK,1);

			rsi_delay_ms(2);

		}
		//(CMD3 ) Get rca
		Stat = RSI_SDIOH_SendRelativeAddressCmd3(SmihInfo, 0x0);
		if (Stat != 0)
		{
			return Stat;
		}

		//(CMD7) Select Card
		Stat = RSI_SDIOH_SelectCardCmd7(SmihInfo->rca);
		if (Stat != 0)
		{
			return Stat;
		}

		Stat = RSI_SDIOH_SendCommand52(SmihInfo,CD_DISABLE_ARG);
		if (Stat != 0)
		{
			return Stat;
		}
		bus_send_again:

		Stat = RSI_SDIOH_SendCommand52(SmihInfo,LOW_SPEED_CHECK_ARG);
		if(SmihInfo->response[0] & LOW_SPEED_CHECK)
		{
			if(SmihInfo->response[0] & BIT4_MODE_CHECK)
			{
				if (SDIOH_BUS_WIDTH4 == pSdiohConfig->busWidthMode)
				{
					Stat = RSI_SDIOH_BusWidthConfig(SmihInfo, SDIOH_BUS_WIDTH4);
				}
			}
			else
			{
			}
		}
		else
		{
			if (SDIOH_BUS_WIDTH4 == pSdiohConfig->busWidthMode)
			{
				Stat = RSI_SDIOH_BusWidthConfig(SmihInfo, SDIOH_BUS_WIDTH4);
				if(Stat != 0)
				{
				}
			}
		}
		// Enable Function.
		while(1)
		{
			Stat = RSI_SDIOH_SendCommand52(SmihInfo,FUCNTION1_CHECK_ARG);
			if (RSI_SDIOH_OK == Stat)
			{
				if(!((SmihInfo->response[0]) & FUNC1_ENABLE))
				{
					Stat = RSI_SDIOH_SendCommand52(SmihInfo,FUCNTION1_ENB_ARG);
					if(Stat != 0)
					{
						return Stat;
					}
					else
					{
						break;
					}
				}
				else
					break;
			}
		}
		// I/O Function Ready
		while(1)
		{
			Stat = RSI_SDIOH_SendCommand52(SmihInfo,FUCNTION1_READY_ARG);
			if (RSI_SDIOH_OK == Stat)
			{
				if(SmihInfo->response[0] & FUNC1_READY)
				{
					break;
				}
			}
		}
		// Interrupt Enable Master & Interrupt Enable for function.
		Stat = RSI_SDIOH_SendCommand52(SmihInfo,FUNC1_INTR_ENB_ARG);
		if (Stat != 0)
		{
			return Stat;
		}
		//Check if Interrupts are Enabled or not.
		Stat = RSI_SDIOH_SendCommand52(SmihInfo,FUNC1_INTR_CHECK_ARG);
		if (Stat != 0)
		{
			return Stat;
		}
		//Check for 1-bit or 4-bit mode of I/O. 
		Stat = RSI_SDIOH_SendCommand52(SmihInfo,BIT4_BUS_WIDTH_SET_ARG);
		if (Stat != 0)
		{
			return Stat;
		}
		if (SDIOH_BUS_WIDTH4 == pSdiohConfig->busWidthMode)
		{
			if(!(SmihInfo->response[0] & BUS_BIT))
			{
				goto bus_send_again;
			}
		}
		//Check for CD Disable
		if(!(SmihInfo->response[0] & (1 << 7)))
		{
			return 0;
		}
		Stat = RSI_SDIOH_SendCommand52(SmihInfo,CSA_SUPPORT_ARG);
		if (RSI_SDIOH_OK == Stat)
		{
			if(SmihInfo->response[0] & (1 << 6))
			{
				Stat = RSI_SDIOH_SendCommand52(SmihInfo,CSA_ENABLE_ARG);
				if (Stat != 0)
				{
					return Stat;
				}
			}
			Stat = RSI_SDIOH_SendCommand52(SmihInfo,I_O_BLOCK_SIZE_ARG);
			if (Stat != 0)
			{
				return Stat;
			}
			Stat = RSI_SDIOH_SendCommand52(SmihInfo,I_O_BLOCK_SIZE_ARG_1);
			if (Stat != 0)
			{
				return Stat;
			}
		}

		if(pSdiohConfig->highSpeedEnable)
		{
			while(1)
			{ 
				Stat = RSI_SDIOH_SendCommand52(SmihInfo,CHECK_HIGH_SPEED_SUPPORT);
				if (Stat != 0)
				{
					return Stat;
				}

				if((SmihInfo->response[0] & HIGH_SPEED_BIT))
				{
					break;
				}
			}

			//high speed mode switching command 
			Stat = RSI_SDIOH_SendCommand52(SmihInfo,ENABLE_HIGH_SPEED_MODE_ARG);
			if (Stat != 0)
			{
				return Stat;
			} 

		}
		if(pSdiohConfig->uhsModeSelect != 0) 
		{ 

			while(1)
			{
				Stat = RSI_SDIOH_SendCommand52(SmihInfo,CHECK_UHS_SUPPORT_MODES);
				if (Stat != 0)
				{
					return Stat;
				}   
				if((SmihInfo->response[0] & UHS_SUPPORT_BITS))
				{
					break;
				}    
			}

			//Send UHS mode to the slave 
			switch(pSdiohConfig->uhsModeSelect)
			{
			case UHS_SDR12:     
				break;
			case UHS_SDR25:
				Stat = RSI_SDIOH_SendCommand52(SmihInfo,UHS_1_SDR25_MODE_ARG);
				if (Stat != 0)
				{
					return Stat;
				}     
				break;
			case UHS_SDR50:
				Stat = RSI_SDIOH_SendCommand52(SmihInfo,UHS_1_SDR50_MODE_ARG);
				if (Stat != 0)
				{
					return Stat;
				}     
				break;
			case UHS_SDR104:
				Stat = RSI_SDIOH_SendCommand52(SmihInfo,UHS_1_SDR104_MODE_ARG);
				if (Stat != 0)
				{
					return Stat;
				}     
				break;
			case UHS_DDR50:
				Stat = RSI_SDIOH_SendCommand52(SmihInfo,UHS_1_DDR50_MODE_ARG);
				if (Stat != 0)
				{
					return Stat;
				}     
				break;
			default:
				return RSI_SDIOH_INVALID_PARAMETER ; 
			}  

		}    
	}
	return Stat;
}



/****************SDIOH CONTROLLER FUNCTIONS****************************/

#define SD_IRQHandler                IRQ068_Handler

SDIOH_INSTANCE_INTERNAL_DATA_T SdiohData =
{
		0u,
		0u,
};

static SDIOH_INTERN_DATA_T* SdiohGetInternDataPtr() 
{
	return &SdiohData.internalData;
}

/**
 * @fn           status_t  sdioh_host_initialization( SDIOH_CONFIG_T*  pConfig )
 * @brief        This API is used to initialize the SDIO Card.              
 * @param[in]    pConfig  : Pointer to the SMIH config struct.
 * @return       RSI_SDIOH_OK   sdioh initialized successfully.
 */
status_t  sdioh_host_initialization( SDIOH_CONFIG_T*  pConfig )
{
	SDIOH_INTERN_DATA_T* pSdiohInternData ; // Pointer to internal data

	// Check for NULL pointer
	if(pConfig ==0)
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	} 

	// Get pointer to internal data structure 
	pSdiohInternData = SdiohGetInternDataPtr( ) ;

	// Check for instance available or not
	if(NULL == pSdiohInternData)
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	}

	// Select ADMA or not
	if((pConfig->selectAdma) == 1)
	{

		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.DMA_SELECT =0x2;
	}
	else
	{
		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.DMA_SELECT = 0x0;

	}

	// Set bus speed mode        
	if((pConfig->highSpeedEnable ) == 0x1)
	{
		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.HIGH_SPEED_ENABLE = 0x1;
	}
	else
	{
		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.HIGH_SPEED_ENABLE = 0x0;
	}  

	// Set bus width   
	if((pConfig->enable4BitMode) == 0x1)
	{
		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.DATA_TRANSFER_WIDTH = 0x1; 
	}
	else
	{
		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.DATA_TRANSFER_WIDTH = 0x0; 
	}

	// Enable all status interrupts
	SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_ENABLE_REGISTER = 0x1FFFu;
	SDIOH->SMIH_ERROR_INTERRUPT_STATUS_ENABLE_REGISTER = 0x07FFu; 

	// Enable normal interrupt signal
	if(pConfig->normalIntrEnable != 0)
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER = pConfig->normalIntrEnable->sdiohNormalIntr;
	}

	// Set normal interrupt callback functions
	if(pConfig->normalIntrCb !=0)
	{
		pSdiohInternData->normalIntrCb.pCommandCompleteIntrCb      = pConfig->normalIntrCb->pCommandCompleteIntrCb;
		pSdiohInternData->normalIntrCb.pTransferCompleteIntrCb      = pConfig->normalIntrCb->pTransferCompleteIntrCb;
		pSdiohInternData->normalIntrCb.pBlockGapIntrCb             = pConfig->normalIntrCb->pBlockGapIntrCb;
		pSdiohInternData->normalIntrCb.pDmaIntrCb                  = pConfig->normalIntrCb->pDmaIntrCb;
		pSdiohInternData->normalIntrCb.pBufferWriteReadyIntrCb     = pConfig->normalIntrCb->pBufferWriteReadyIntrCb;
		pSdiohInternData->normalIntrCb.pBufferReadReadyIntrCb      = pConfig->normalIntrCb->pBufferReadReadyIntrCb;
		pSdiohInternData->normalIntrCb.pCardInsertIntrCb           = pConfig->normalIntrCb->pCardInsertIntrCb;
		pSdiohInternData->normalIntrCb.pCardRemovalIntrCb          = pConfig->normalIntrCb->pCardRemovalIntrCb;
		pSdiohInternData->normalIntrCb.pfnCardIntrCb               = pConfig->normalIntrCb->pfnCardIntrCb;
	}
	// Enable error interrupt signal
	if(pConfig->errorIntrEnable != 0)
	{
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER = pConfig->errorIntrEnable->errorStatus;
	}

	// Set error interrupt callback functions
	if(pConfig->errorIntrCb !=0 )
	{
		pSdiohInternData->ErrIrqCb.pCommandTimeoutErrorIntrCb       = pConfig->errorIntrCb->pCommandTimeoutErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pCommandCrcErrorIntrCb           = pConfig->errorIntrCb->pCommandCrcErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pCommandEndBitErrorIntrCb        = pConfig->errorIntrCb->pCommandEndBitErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pCommandIndexErrorIntrCb         = pConfig->errorIntrCb->pCommandIndexErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pDataTimeoutErrorIntrCb      = pConfig->errorIntrCb->pDataTimeoutErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pDataEndBitErrorIntrCb       = pConfig->errorIntrCb->pDataEndBitErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pDataCrcErrorIntrCb          = pConfig->errorIntrCb->pDataCrcErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pCurrentLimitErrorIntrCb     = pConfig->errorIntrCb->pCurrentLimitErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pAutoCommandErrorIntrCb          = pConfig->errorIntrCb->pAutoCommandErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pAdmaErrorIntrCb             = pConfig->errorIntrCb->pAdmaErrorIntrCb;
		pSdiohInternData->ErrIrqCb.pTuningErrorIntrCb           = pConfig->errorIntrCb->pTuningErrorIntrCb;
	}

	/*Enable Nvic*/ 
	NVIC_ClearPendingIRQ(SDMEM_IRQn);
	NVIC_EnableIRQ(SDMEM_IRQn);


	return RSI_SDIOH_OK;
} 

/**
 * @fn           status_t  Smih_DeInitialization(	boolean_t Intr,boolean_t IntCb)
 * @brief        This API is used to Deinitialize the SDIO Card.               *
 * @return       RSI_SDIOH_OK    sdioh initialized successfully.
 */
status_t  Smih_DeInitialization(  
		boolean_t Intr,
		boolean_t IntCb)
{
	// Pointer to internal data
	SDIOH_INTERN_DATA_T* pSdiohInternData ; 

	// Get pointer to internal data structure ...
	pSdiohInternData = SdiohGetInternDataPtr() ;

	if(pSdiohInternData == 0x0)
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	}

	// Clear clock control and power control registers
	SDIOH->SMIH_CLOCK_CONTROL_REGISTER = 0x0000;
	SDIOH->SMIH_POWER_CONTROL_REGISTER = 0x00;

	if(Intr == 0x1)
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER = 0x0000;
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER  = 0x0000;
	}

	if(IntCb == 0x1)
	{
		pSdiohInternData->normalIntrCb.pCommandCompleteIntrCb      = NULL;
		pSdiohInternData->normalIntrCb.pCommandCompleteIntrCb     = NULL;
		pSdiohInternData->normalIntrCb.pBlockGapIntrCb             = NULL;
		pSdiohInternData->normalIntrCb.pDmaIntrCb                  = NULL;
		pSdiohInternData->normalIntrCb.pBufferWriteReadyIntrCb     = NULL;
		pSdiohInternData->normalIntrCb.pBufferReadReadyIntrCb      = NULL;
		pSdiohInternData->normalIntrCb.pCardInsertIntrCb           = NULL;
		pSdiohInternData->normalIntrCb.pCardRemovalIntrCb          = NULL;
		pSdiohInternData->normalIntrCb.pfnCardIntrCb                 = NULL;
		pSdiohInternData->ErrIrqCb.pCommandTimeoutErrorIntrCb       = NULL;
		pSdiohInternData->ErrIrqCb.pCommandCrcErrorIntrCb           = NULL;
		pSdiohInternData->ErrIrqCb.pCommandEndBitErrorIntrCb        = NULL;
		pSdiohInternData->ErrIrqCb.pCommandIndexErrorIntrCb         = NULL;
		pSdiohInternData->ErrIrqCb.pDataTimeoutErrorIntrCb      = NULL;
		pSdiohInternData->ErrIrqCb.pDataEndBitErrorIntrCb       = NULL;
		pSdiohInternData->ErrIrqCb.pDataCrcErrorIntrCb          = NULL;
		pSdiohInternData->ErrIrqCb.pCurrentLimitErrorIntrCb     = NULL;
		pSdiohInternData->ErrIrqCb.pAutoCommandErrorIntrCb          = NULL;
		pSdiohInternData->ErrIrqCb.pAdmaErrorIntrCb             = NULL;
		pSdiohInternData->ErrIrqCb.pTuningErrorIntrCb           = NULL;
	}

	/*disable nvic*/
	NVIC_ClearPendingIRQ(SDMEM_IRQn);
	NVIC_DisableIRQ(SDMEM_IRQn);

	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_normal_interrupt_signal_enable(SDIOH_NORMAL_INTR_SEL_T enNormalIntrSel)
 * @brief        This API is used to enable the SDIOH selected interrupt.         
 * @return       RSI_SDIOH_OK    Enabled sdioh normal interrupt successfully.
 */
status_t  sdioh_normal_interrupt_signal_enable(SDIOH_NORMAL_INTR_SEL_T enNormalIntrSel)
{
	switch (enNormalIntrSel)
	{
	case SDIOH_COMMAND_COMPLETE_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_COMPLETE_SIGNAL_ENABLE   = 0x1;
		break;
	case SDIOH_TRANSFER_COMPLETE_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.TRANSFER_COMPLETE_SIGNAL_ENABLE    = 0x1;
		break;
	case SDIOH_BLOCK_GAP_EVENT_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.BLCOK_GAP_EVENT_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_DMA_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.DMA_INTERRUPT_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_BUFFER_WRITE_READY_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.BUFFER_WRITE_READY_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_BUFFER_READ_READY_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.BUFFER_READ_READY_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_CARD_INSERTION_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.CARD_INSERTION_SIGNAL__ENABLE = 0x1;
		break;
	case SDIOH_CARD_REMOVAL_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.CARD_REMOVAL_SIGNAL__ENABLE = 0x1;
		break;    
	case SDIOH_CARD_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.CARD_INTERRUPT_SIGNAL_ENABLE = 0x1;
		break;  
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;
	}

	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_normal_interrupt_signal_disable( SDIOH_NORMAL_INTR_SEL_T enNormalIntrSel)
 * @brief        This API is used to disable the SDIOH selected interrupt.         
 * @return       RSI_SDIOH_OK    Disabled sdioh normal interrupt successfully.
 */
status_t  sdioh_normal_interrupt_signal_disable( SDIOH_NORMAL_INTR_SEL_T enNormalIntrSel)
{
	switch (enNormalIntrSel)
	{
	case SDIOH_COMMAND_COMPLETE_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_COMPLETE_SIGNAL_ENABLE   = 0x0;
		break;
	case SDIOH_TRANSFER_COMPLETE_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.TRANSFER_COMPLETE_SIGNAL_ENABLE    = 0x0;
		break;
	case SDIOH_BLOCK_GAP_EVENT_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.BLCOK_GAP_EVENT_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_DMA_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.DMA_INTERRUPT_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_BUFFER_WRITE_READY_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.BUFFER_WRITE_READY_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_BUFFER_READ_READY_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.BUFFER_READ_READY_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_CARD_INSERTION_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.CARD_INSERTION_SIGNAL__ENABLE = 0x0;
		break;
	case SDIOH_CARD_REMOVAL_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.CARD_REMOVAL_SIGNAL__ENABLE = 0x0;
		break;    
	case SDIOH_CARD_INTR:
		SDIOH->SMIH_NORMAL_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.CARD_INTERRUPT_SIGNAL_ENABLE = 0x0;
		break;  
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;
	}
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_error_interrupt_signal_enable(SDIOH_ERROR_INTR_SEL_T enErrIntrEnable)
 * @brief        This API is used to enable the SDIOH selected error interrupt.         
 * @return       RSI_SDIOH_OK    Enabled sdioh error interrupt successfully.
 */
status_t  sdioh_error_interrupt_signal_enable(SDIOH_ERROR_INTR_SEL_T enErrIntrEnable)
{
	switch (enErrIntrEnable)
	{
	case SDIOH_COMMAND_TIMEOUT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_TIMEOUT_ERROR_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_COMMAND_CRC_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_CRC_ERROR_SIGNAL_ENABLE= 0x1;
		break;
	case SDIOH_COMMAND_END_BIT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_END_BIT_ERROR_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_COMMAND_INDEX_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_INDEX_ERROR_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_COMMAND_DATA_TIMEOUT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.DATA_TIMEOUT_ERROR_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_DATA_CRC_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.DATA_CRC_ERROR_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_DATA_ENDBIT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.DATA_END_BIT_ERROR_SIGNAL_ENABLE = 0x1;
		break;
	case SDIOH_CURRENT_LIMIT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.CURRENT_LIMIT_ERROR_SIGNAL_ENABLE = 0x1;
		break;    
	case SDIOH_AUTO_CMD_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.AUTO_CMD_ERROR_SIGNAL_ENABLE = 0x1;
		break;  
	case SDIOH_ADMA_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.ADMA_ERROR_SIGNAL_ENABLE = 0x1;
		break;  
	case SDIOH_TUNING_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.TUNING_ERROR_SIGNAL_ENABLE = 0x1;
		break;              
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;
	}
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_error_interrupt_signal_disable(SDIOH_ERROR_INTR_SEL_T enErrIntrEnable)
 * @brief        This API is used to disable the SDIOH selected error interrupt.         
 * @return       RSI_SDIOH_OK    Disabled Smih error interrupts successfully.
 */
status_t  sdioh_error_interrupt_signal_disable(SDIOH_ERROR_INTR_SEL_T enErrIntrEnable)
{
	switch (enErrIntrEnable)
	{
	case SDIOH_COMMAND_TIMEOUT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_TIMEOUT_ERROR_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_COMMAND_CRC_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_CRC_ERROR_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_COMMAND_END_BIT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_END_BIT_ERROR_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_COMMAND_INDEX_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.COMMAND_INDEX_ERROR_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_COMMAND_DATA_TIMEOUT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.DATA_TIMEOUT_ERROR_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_DATA_CRC_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.DATA_CRC_ERROR_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_DATA_ENDBIT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.DATA_END_BIT_ERROR_SIGNAL_ENABLE = 0x0;
		break;
	case SDIOH_CURRENT_LIMIT_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.CURRENT_LIMIT_ERROR_SIGNAL_ENABLE = 0x0;
		break;    
	case SDIOH_AUTO_CMD_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.AUTO_CMD_ERROR_SIGNAL_ENABLE = 0x0;
		break;  
	case SDIOH_ADMA_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.ADMA_ERROR_SIGNAL_ENABLE = 0x0;
		break;  
	case SDIOH_TUNING_ERROR_INTR:
		SDIOH->SMIH_ERROR_INTERRUPT_SIGNAL_ENABLE_REGISTER_b.TUNING_ERROR_SIGNAL_ENABLE = 0x0;
		break;              
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;
	}
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_error_interrupt_status_get (SDIOH_ERROR_INTR_STATUS_T* pStatus)
 * @brief        This API is used to get the SDIOH error interrupt status.         
 * @param[in]    pStatus         Pointer to error interrupt status selection structure
 * @return       RSI_SDIOH_OK    Get error status interrupt normally.
 */
status_t  sdioh_error_interrupt_status_get (SDIOH_ERROR_INTR_STATUS_T* pStatus)
{
	pStatus->errorStatus = SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER;

	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_error_interrupt_status_clear(SDIOH_ERROR_INTR_STATUS_T* pStatus)
 * @brief        This API is used to clear the SDIOH selected error interrupt.         
 * @param[in]    pStatus         Pointer to error interrupt status selection structure
 * @return       RSI_SDIOH_OK    Get error status interrupt normally.
 */
status_t  sdioh_error_interrupt_status_clear(SDIOH_ERROR_INTR_STATUS_T* pStatus)
{
	// Check for NULL pointer
	if (pStatus == 0)
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	}
	SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = pStatus->errorStatus;
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t sdioh_normal_interrupt_status_get(SDIOH_NORMAL_INTR_STATUS_T* pStatus)
 * @brief        This API is used to get the SDIOH normal interrupt status.         
 * @param[in]    pStatus         Pointer to normal interrupt status selection structure
 * @return       RSI_SDIOH_OK    Get normal interrupt status interrupt normally.
 */
status_t sdioh_normal_interrupt_status_get(SDIOH_NORMAL_INTR_STATUS_T* pStatus)
{
	pStatus->sdiohNormalIntr = SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER;
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t sdioh_normal_interrupt_status_clear(SDIOH_NORMAL_INTR_STATUS_T* pStatus)
 * @brief        This API is used to clear the SDIOH normal interrupt status.         
 * @param[in]    pStatus         Pointer to normal interrupt status selection structure
 * @return       RSI_SDIOH_OK    clears the  error status interrupt normally.
 */
status_t sdioh_normal_interrupt_status_clear(SDIOH_NORMAL_INTR_STATUS_T* pStatus)
{
	// Check for NULL pointer
	if (pStatus == 0)
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	}
	SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER = pStatus->sdiohNormalIntr;
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_auto_command_error_status_get(SDIOH_AUTO_COMMAND_ERROR_T* pStatus)
 * @brief        This API is used to get the SDIOH error interrupt status.         
 * @param[in]    pStatus         Pointer to error interrupt status selection structure
 * @return       RSI_SDIOH_OK    Gets the  error status interrupt normally.
 */
status_t  sdioh_auto_command_error_status_get(SDIOH_AUTO_COMMAND_ERROR_T* pStatus)
{
	// Check for NULL pointer
	if (pStatus ==0)
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	}
	pStatus->autoCmdError = SDIOH->SMIH_AUTO_CMD_ERROR_STATUS_REGISTER;
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_internal_clock_enable()
 * @brief        This API is used to enable the SDIOH internal clock.         
 * @return       RSI_SDIOH_OK   If clock is enabled properly.
 */
status_t  sdioh_internal_clock_enable(  )
{
	SDIOH->SMIH_CLOCK_CONTROL_REGISTER_b.INTERNAL_CLOCK_ENABLE = 0x1;
	while(0x1 != SDIOH->SMIH_CLOCK_CONTROL_REGISTER_b.INTERNAL_CLOCK_STABLE);
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_internal_clock_disable(  )
 * @brief        This API is used to disable the SDIOH internal clock.         
 * @return       RSI_SDIOH_OK   If clock is disabled properly.
 */
status_t  sdioh_internal_clock_disable(  )
{    
	SDIOH->SMIH_CLOCK_CONTROL_REGISTER_b.INTERNAL_CLOCK_ENABLE = 0x0;  
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_clock_enable()
 * @brief        This API is used to enable the SDIOH clock output.         
 * @return       RSI_SDIOH_OK   If clock is disabled properly.
 */
status_t  sdioh_clock_enable(  )
{
	SDIOH->SMIH_CLOCK_CONTROL_REGISTER_b.SD_CLOCK_ENABLE = 0x1;
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_clock_enable()
 * @brief        This API is used to disable the SDIOH clock output.         
 * @return       RSI_SDIOH_OK   If clock is disabled properly.
 */
status_t  sdioh_clock_disable(  )
{   
	SDIOH->SMIH_CLOCK_CONTROL_REGISTER_b.SD_CLOCK_ENABLE  = 0u;
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t sdioh_clock_division_set(uint16_t Div)
 * @brief        This API is used to change the SDIOH clock division.
 * @param[in]    Division value
 * @return       RSI_SDIOH_OK   Clock division is changed normally.
 */
status_t sdioh_clock_division_set(  uint16_t Div)
{   
	// Set clock division (10-bit)
	SDIOH->SMIH_CLOCK_CONTROL_REGISTER_b.SDCLK_FREQUENCY_SELECT = (Div & 0xFFu);
	SDIOH->SMIH_CLOCK_CONTROL_REGISTER_b.UPPER_BITS_OF_SDCLK_FREQUENCY_SELECT = ((Div >> 8u) &  0x03u);
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t sdioh_bus_width_set( boolean_t Enable4BitMode )
 * @brief        This API is used to set the SDIOH clock division.
 * @param[in]    Division value
 * @return       RSI_SDIOH_OK   Clock division is changed normally.
 */
status_t sdioh_bus_width_set( boolean_t Enable4BitMode )
{
	// Set bit width
	if(Enable4BitMode ==1)
	{
		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.DATA_TRANSFER_WIDTH =0x1;
	}
	else
	{
		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.DATA_TRANSFER_WIDTH =0x0; 
	}
	return RSI_SDIOH_OK;
}
/**
 * @fn           status_t sdioh_bus_speed_mode_set(boolean_t HighSpeedMode)
 * @brief        This API is used to set the SDIOH speed mode.
 * @param[in]    HighSpeedMode  1 for high speed mode
                                0 for full speed mode
 * @return       RSI_SDIOH_OK   If speed configure properly.
 */
status_t sdioh_bus_speed_mode_set(boolean_t HighSpeedMode )
{
	// Set bit width    
	if(HighSpeedMode == 1)
	{
		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.HIGH_SPEED_ENABLE = 0x1;
	}
	else
	{
		SDIOH->SMIH_HOST_CONTROL_1_REGISTER_b.HIGH_SPEED_ENABLE = 0x0;
	}

	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_bus_voltage_select(SDIOH_VOLTAGE_SEL_T enVoltage)
 * @brief        This API is used to select the SDIOH voltage.
 * @param[in]    enVoltage       voltage selection
 *                               possbile selections are
 *                               VOLTAGE_18V   1.8v voltage selection for sdio interface
 *                               VOLTAGE_30V   3.0v voltage selection for sdio interface
 *                               VOLTAGE_33V   3.3v voltage selection for sdio interface
 * @return       RSI_SDIOH_OK   If configured voltage properly.
 */
status_t  sdioh_bus_voltage_select(SDIOH_VOLTAGE_SEL_T enVoltage)
{
	switch (enVoltage)
	{
	case VOLTAGE_18V:
		SDIOH->SMIH_POWER_CONTROL_REGISTER_b.SD_BUS_VOLTAGE_SELECT = 0x5;
		break;
	case VOLTAGE_30V:
		SDIOH->SMIH_POWER_CONTROL_REGISTER_b.SD_BUS_VOLTAGE_SELECT  = 0x6u;
		break;
	case VOLTAGE_33V:
		SDIOH->SMIH_POWER_CONTROL_REGISTER_b.SD_BUS_VOLTAGE_SELECT = 0x7u;
		break;
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;
	}
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t sdioh_bus_power_on()
 * @brief        This API is used to power on the sdio bus.
 * @return       RSI_SDIOH_OK   for SD powered on.
 */
status_t sdioh_bus_power_on( )
{  
	SDIOH->SMIH_POWER_CONTROL_REGISTER_b.SD_BUS_POWER  = 0x1;   
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_bus_power_off()
 * @brief        This API is used to power off the sdio bus.
 * @return       RSI_SDIOH_OK   RSI_SDIOH_OK   for SD powered off.
 */
status_t  sdioh_bus_power_off( )
{
	SDIOH->SMIH_POWER_CONTROL_REGISTER_b.SD_BUS_POWER  = 0x0;   
	return RSI_SDIOH_OK;
}


/**
 * @fn           status_t sdioh_send_command( SDIOH_COMMAND_CONFIG_T* pConfig )
 * @brief        This API is used to send the command on command line.
 * @param[in]    pConfig                      pointer to the command structure
 * @return       RSI_SDIOH_OK                 If command sent properly.
 *               RSI_SDIOH_INVALID_PARAMETER  If pConfig==NULL
 */
status_t sdioh_send_command( SDIOH_COMMAND_CONFIG_T* pConfig )
{
	SDIOH_COMMAND_FIELD_T CmdData;

	// Clear structure
	memset(&CmdData, 0, sizeof(CmdData));

	// Check for NULL pointer
	if (pConfig == 0)
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	}

	// Set command type
	switch (pConfig->cmdType)
	{
	case NORMAL_CMD:
		CmdData.cmdType = 0u;
		break;
	case SUSPEND_CMD:
		CmdData.cmdType = 1u;
		break;
	case RESUME_CMD:
		CmdData.cmdType = 2u;
		break;
	case  ABORT_CMD:
		CmdData.cmdType = 3u;
		break;
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;
	}

	// Set data present during the command    
	if(pConfig->dataPresent == 1)
	{
		CmdData.dataPresentSelect = 0x1;
	}
	else
	{
		CmdData.dataPresentSelect = 0x0;
	}

	// Set command index check 
	if(pConfig->checkCmdIndex == 1)
	{
		CmdData.cmdIndexCheckEnable = 0x1;
	}
	else
	{
		CmdData.cmdIndexCheckEnable  = 0x0;
	}

	// Set command CRC check
	if(pConfig->checkCmdCrc ==1)
	{
		CmdData.cmdCrcCheckEnable = 0x1; 
	}
	else
	{
		CmdData.cmdCrcCheckEnable = 0x0;
	}

	// Set command response type
	switch (pConfig->responseType)
	{
	case SDIOH_RESPONSE_NONE:
		CmdData.respType = 0u;
		break;
	case SDIOH_RESPONSE_136BIT:
		CmdData.respType = 1u;
		break;
	case SDIOH_RESPONSE_48BIT:
		CmdData.respType = 2u;
		break;
	case SDIOH_RESPONSE_48BIT_BUSY_CHECK:
		CmdData.respType = 3u;
		break;
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;            
	}

	// Set command index
	CmdData.cmdIndex = pConfig->cmdIndex;

	// Auto command setting
	switch (pConfig->autoCmdType)
	{
	case DISABLE_AUTO_CMD :
		SDIOH->TRANSFER_MODE_REGISTER_b.AUTO_CMD_ENABLE = 0u;
		break;
	case ENABLE_AUTO_CMD12 :
		SDIOH->TRANSFER_MODE_REGISTER_b.AUTO_CMD_ENABLE  = 1u;
		break;
	case ENABLE_AUTO_CMD23 :
		SDIOH->TRANSFER_MODE_REGISTER_b.AUTO_CMD_ENABLE = 2u;
		break;
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;     
	}

	// Set augument derectly to hardware register
	SDIOH->SMIH_ARGUMENT1_REGISTER = pConfig->argument;

	if(pConfig->cmdIndex == CMD5)
	{
		if((pConfig->argument & (1 << 24)))
		{
			SDIOH->SMIH_HOST_CONTROL_2_REGISTER = (1 << 3); 
		}
	}
	// Write command  to the hardware register (Command is sent)
	//according to the spec, this register should be  write only once.
	SDIOH->SMIH_COMMAND_REGISTER = *((uint16_t *)&CmdData);

	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_get_response( uint16_t* pResponseData, uint8_t ResponseRegCount)
 * @brief        This API is used to receive response on cmd line.
 * @param[in]    pResponseData                pointer to the response data
 * @return       RSI_SDIOH_OK                 If command sent properly.
 *               RSI_SDIOH_INVALID_PARAMETER  If pConfig==NULL or ResponseRegCount >8
 */
status_t  sdioh_get_response( uint16_t* pResponseData, uint8_t ResponseRegCount)
{
	uint16_t* pResponseBaseAddr;
	uint8_t i;

	// Check for NULL pointer
	if ( (pResponseData == 0) || (ResponseRegCount > 8u))
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	}

	pResponseBaseAddr = (uint16_t*)&SDIOH->SMIH_RESPONSE_REGISTER0;

	for(i=0; i<ResponseRegCount; i++)
	{
		*pResponseData++ = *pResponseBaseAddr++;
	}

	return RSI_SDIOH_OK;
}


/**
 * @fn           status_t  sdioh_data_transfer_initialization(SDIOH_DATA_CONFIG_T* pDataConfig)
 * @brief        This API is used to initialize the data transfer(this must be called before data transfer).
 * @param[in]    pDataConfig                  pointer to the data transfer configuration
 * @return       RSI_SDIOH_OK                 If data is initialized properly.
 *               RSI_SDIOH_INVALID_PARAMETER  If pDataConfig==NULL 
 */
status_t  sdioh_data_transfer_initialization(SDIOH_DATA_CONFIG_T* pDataConfig)
{
	// Check for NULL pointer
	if (pDataConfig == 0)
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	}

	// Set multi-block or single block transfer    
	if(pDataConfig->multiBlock == 1)
	{
		SDIOH->TRANSFER_MODE_REGISTER_b.MULTI_OR_SINGLE_BLOCK_SELECT = 0x1;
	}
	else
	{
		SDIOH->TRANSFER_MODE_REGISTER_b.MULTI_OR_SINGLE_BLOCK_SELECT = 0x0;
	}

	// Set data transfer direction
	if(pDataConfig->read == 1)
	{
		SDIOH->TRANSFER_MODE_REGISTER_b.DATA_TRANSFER_DIRECTION_SELECT = 0x1;
	}
	else
	{
		SDIOH->TRANSFER_MODE_REGISTER_b.DATA_TRANSFER_DIRECTION_SELECT = 0x0;
	}

	// Set block size
	SDIOH->SMIH_BLOCK_SIZE_REGISTER_b.TRANSFER_BLOCK_SIZE = pDataConfig->blockSize;

	// Set block count  
	SDIOH->SMIH_BLOCK_COUNT_REGISTER= pDataConfig->blockCount;

	// Set block count enable
	if(pDataConfig->enableBlockCount == 1)
	{
		SDIOH->TRANSFER_MODE_REGISTER_b.BLOCK_COUNT_ENABLE = 0x1;
	}
	else
	{
		SDIOH->TRANSFER_MODE_REGISTER_b.BLOCK_COUNT_ENABLE = 0x0;
	}

	// Enable DMA or not
	if(pDataConfig->dmaEnable == 1)
	{
		SDIOH->TRANSFER_MODE_REGISTER_b.DMA_ENABLE = 0x1;
	}
	else
	{
		SDIOH->TRANSFER_MODE_REGISTER_b.DMA_ENABLE = 0x0;
	}

	// Set descriptor table of ADMA
	SDIOH->SMIH_ADMA_SYSTEM_ADDRESS0_REGISTER = (uint16_t)pDataConfig->admaDespTableAddress;
	SDIOH->SMIH_ADMA_SYSTEM_ADDRESS1_REGISTER = (uint16_t)(pDataConfig->admaDespTableAddress >> 16u);

	// Set data timeout time
	if(pDataConfig->dataTimeout > SDIOH_TIMEOUT_BASE_CLOCK_2_27)
	{
		return RSI_SDIOH_INVALID_PARAMETER ;
	}   
	SDIOH->SMIH_TIMEOUT_CONTROL_REGISTER_b.DATA_TIMEOUT_COUNTER_VALUE  = pDataConfig->dataTimeout;

	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_stop_at_block_gap()
 * @brief        This API is used to stop multi block transfer.
 * @param[in]    pDataConfig                  pointer to the data transfer configuration
 * @return       RSI_SDIOH_OK                 If data stops transfer.
 */
status_t  sdioh_stop_at_block_gap( )
{
	SDIOH->SMIH_BLOCK_GAP_CONTROL_REGISTER_b.STOP_AT_BLOCK_GAP_REQUEST = 0x1;
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_transfer_restart( )
 * @brief        This API is used to restart the transfer when transfer is pending.
 * @return       RSI_SDIOH_OK                 If data restarts transfer successfully.
 */
status_t  sdioh_transfer_restart( )
{
	SDIOH->SMIH_BLOCK_GAP_CONTROL_REGISTER_b.CONTINUE_REQUEST =0x1;    
	return RSI_SDIOH_OK;
}

/**
 * @fn           boolean_t sdioh_wait_for_card_insert( )
 * @brief        This API is used wait for card stable,and to check card is inserted or not.
 */
boolean_t sdioh_wait_for_card_insert( )
{
	// Wait until card is stable   
	while(SDIOH->SMIH_PRESENT_STATE_REGISTER_b.CARD_STATE_STABLE != 0x1);
	return (boolean_t)SDIOH->SMIH_PRESENT_STATE_REGISTER_b.CARD_INSERTED;
}

/**
 * @fn           void sdioh_18v_signal_enable()
 * @brief        This API is used to enable 1.8v signal enable bit.
 */
void sdioh_18v_signal_enable()
{
	SDIOH->SMIH_HOST_CONTROL_2_REGISTER_b._1_8V_SIGNALING_ENABLE = 0x1;
}

/**
 * @fn           status_t sdioh_uhs_mode_select(SDIOH_UHS_MODE_T enUhsMode)
 * @brief        This API is used to select the SDIOH UHS(ULTRA HIGH SPEED) mode.
 * @param[in]    enUhsMode       Uhs mode selection
 *                               possbile selections are
 *                               UHS_NONE    no uhs mode
 *                               UHS_SDR12   in case of SDR12 mode requirement
 *                               UHS_SDR25   in case of SDR25 mode requirement
 *                               UHS_SDR50   in case of SDR50 mode requirement
 *                               UHS_SDR104  in case of SDR104 mode requirement
 *                               UHS_DDR50   in case of DDR50 mode requirement
 * @return       RSI_SDIOH_OK   If uhs mode configured  properly.
 */
status_t sdioh_uhs_mode_select(SDIOH_UHS_MODE_T enUhsMode)
{
	switch(enUhsMode) 
	{
	case UHS_SDR12:
		SDIOH->SMIH_HOST_CONTROL_2_REGISTER_b.UHS_MODE_SELECT = 0x0;
		break;
	case UHS_SDR25:
		SDIOH->SMIH_HOST_CONTROL_2_REGISTER_b.UHS_MODE_SELECT = 0x1;
		break;
	case UHS_SDR50:
		SDIOH->SMIH_HOST_CONTROL_2_REGISTER_b.UHS_MODE_SELECT = 0x2;
		break;
	case UHS_SDR104:
		SDIOH->SMIH_HOST_CONTROL_2_REGISTER_b.UHS_MODE_SELECT = 0x3;
		break;
	case UHS_DDR50:
		SDIOH->SMIH_HOST_CONTROL_2_REGISTER_b.UHS_MODE_SELECT = 0x4;
		break;
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;
	}
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t sdioh_software_reset(SDIOH_RESET_T resetEn)
 * @brief        This API is used to issue the software reset for all the cmd and data.
 * @param[in]    resetEn       Rest type
 *                             possbile selections are
 *                             SDIOH_DATA_LINE_RESET      for data line reset
 *                             SDIOH_COMMAND_LINE_RESET   for command line reset
 *                             SDIOH_ALL_RESET            for all software rest
 * @return       RSI_SDIOH_OK    If software reset is done successfully.
 */
status_t sdioh_software_reset(SDIOH_RESET_T resetEn)
{
	switch (resetEn)
	{
	case SDIOH_DATA_LINE_RESET:
		SDIOH->SMIH_SOFTWARE_RESET_REGISTER_b.SOFTWARE_RESET_FOR_DAT_LINE = 0x1;
		while(0 != SDIOH->SMIH_SOFTWARE_RESET_REGISTER_b.SOFTWARE_RESET_FOR_DAT_LINE); // Wait until reset finish
		break;
	case SDIOH_COMMAND_LINE_RESET:
		SDIOH->SMIH_SOFTWARE_RESET_REGISTER_b.SOFTWARE_RESET_FOR_CMD_LINE = 0x1;
		while(0 != SDIOH->SMIH_SOFTWARE_RESET_REGISTER_b.SOFTWARE_RESET_FOR_CMD_LINE); // Wait until reset finish
		break;
	case SDIOH_ALL_RESET:
		SDIOH->SMIH_SOFTWARE_RESET_REGISTER_b.SOFTWARE_RESET_FOR_ALL = 0x1;
		while(0 !=  SDIOH->SMIH_SOFTWARE_RESET_REGISTER_b.SOFTWARE_RESET_FOR_ALL); // Wait until reset finish
		break;
	default:
		return RSI_SDIOH_INVALID_PARAMETER ;
	}
	return RSI_SDIOH_OK;
}

/**
 * @fn           status_t  sdioh_set_wakeup_factor(boolean_t CardRemovalWakeup,
 *                                             		 boolean_t CardInsertWakeup, 
 *                                              	 boolean_t CardIntrWakeup)
 * @brief        This API is used to set sd host wakeup factors.
 * @param[in]    CardRemovalWakeup  If card removal triggers sdio host wakeup    
 * @param[in]    CardInsertWakeup   If card insert triggers sdio host wakeup                  
 * @param[in]    CardIntrWakeup     If card interrupt triggers sdio host wakeuo                                                  
 * @return       RSI_SDIOH_OK       If sd host wakeup fact configured normally.
 */
status_t  sdioh_set_wakeup_factor( 
		boolean_t CardRemovalWakeup,
		boolean_t CardInsertWakeup, 
		boolean_t CardIntrWakeup)
{
	if(CardRemovalWakeup ==1)
	{
		SDIOH->SMIH_WAKEUP_CONTROL_REGISTER_b.WAKEUP_EVENT_ENABLE_ON_SDCARD_REMOVAL = 0x1;
	}
	else
	{
		SDIOH->SMIH_WAKEUP_CONTROL_REGISTER_b.WAKEUP_EVENT_ENABLE_ON_SDCARD_REMOVAL = 0x0;
	}

	if(CardInsertWakeup == 1)
	{
		SDIOH->SMIH_WAKEUP_CONTROL_REGISTER_b.WAKEUP_EVENT_ENABLE_ON_SDCARD_INSERTION = 0x1;
	}
	else
	{
		SDIOH->SMIH_WAKEUP_CONTROL_REGISTER_b.WAKEUP_EVENT_ENABLE_ON_SDCARD_INSERTION = 0x0;
	}

	if(CardIntrWakeup == 1)
	{
		SDIOH->SMIH_WAKEUP_CONTROL_REGISTER_b.WAKEUP_EVENT_ENABLE_ON_CARD_INTERRUPT =0x1;
	}
	else
	{
		SDIOH->SMIH_WAKEUP_CONTROL_REGISTER_b.WAKEUP_EVENT_ENABLE_ON_CARD_INTERRUPT =0x0;
	}

	return RSI_SDIOH_OK;
}



/**
 * @fn           void sdioh_irq_handler(SDIOH_INTERN_DATA_T* pSdiohInternData)
 * @brief        This is SDIO host Irq Handler          
 * @param[in]    pSdiohInternData   Pointer to the SDIOH internal data.
 */ 
void sdioh_irq_handler(SDIOH_INTERN_DATA_T* pSdiohInternData)
{
	/*Command complete*/
	if(SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER_b.COMMAND_COMPLETE ==0x1)      
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER = 0x0001;// Clear interrupt flag
		if (pSdiohInternData->normalIntrCb.pCommandCompleteIntrCb != 0)
		{
			pSdiohInternData->normalIntrCb.pCommandCompleteIntrCb();
		}
	}

	/*Transfer complete*/
	if(SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER_b.TRANSFER_COMPLETE == 0x1)      
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER = 0x0002; // Clear interrupt flag
		if (pSdiohInternData->normalIntrCb.pTransferCompleteIntrCb != 0)
		{
			pSdiohInternData->normalIntrCb.pTransferCompleteIntrCb();
		}
	}

	/*block gap event*/    
	if(SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER_b.BLOCK_GAP_EVENT ==0x1)      
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER = 0x0004; // Clear interrupt flag
		if (pSdiohInternData->normalIntrCb.pBlockGapIntrCb != 0)
		{
			pSdiohInternData->normalIntrCb.pBlockGapIntrCb();
		}
	}

	/*DMA  event*/
	if (SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER_b.SMIH_DMA_INTERRUPT== 0x1)    
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER = 0x0008; // Clear interrupt flag
		if (pSdiohInternData->normalIntrCb.pDmaIntrCb != 0)
		{
			pSdiohInternData->normalIntrCb.pDmaIntrCb();
		}
	}

	/*Buffer write ready*/
	if (SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER_b.BUFFER_WRITE_READY == 0x1)      
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER =  0x0010; // Clear interrupt flag
		if (pSdiohInternData->normalIntrCb.pBufferWriteReadyIntrCb != 0)
		{
			pSdiohInternData->normalIntrCb.pBufferWriteReadyIntrCb();
		}
	}

	/*Buffer read ready*/
	if (SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER_b.BUFFER_READ_READY == 0x1)     
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER  = 0x0020; // Clear interrupt flag
		if (pSdiohInternData->normalIntrCb.pBufferReadReadyIntrCb != 0)
		{
			pSdiohInternData->normalIntrCb.pBufferReadReadyIntrCb();
		}
	}

	/*Card insetion*/
	if (SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER_b.CARD_INSERTION == 0x1)     
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER   = 0x0040; // Clear interrupt flag
		if ( pSdiohInternData->normalIntrCb.pCardInsertIntrCb != 0)
		{
			pSdiohInternData->normalIntrCb.pCardInsertIntrCb();
		}
	}

	/* Card removal */
	if (SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER_b.CARD_REMOVAL== 0x1)    
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER = 0x0080; // Clear interrupt flag
		if (pSdiohInternData->normalIntrCb.pCardRemovalIntrCb != 0)
		{
			pSdiohInternData->normalIntrCb.pCardRemovalIntrCb();
		}
	}

	/*Card interrupt*/
	if (SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER_b.CARD_INTERRUPT ==0x1)     
	{
		SDIOH->SMIH_NORMAL_INTERRUPT_STATUS_REGISTER  = 0x0100; // Clear interrupt flag
		if (pSdiohInternData->normalIntrCb.pfnCardIntrCb != 0)
		{
			pSdiohInternData->normalIntrCb.pfnCardIntrCb();
		}
	}

	/* RSI_SDIOH_ERROR interrupt handler */

	/*Command timeout error*/
	if(SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.COMMAND_TIMEOUT_ERROR == 0x1 )   
	{     
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0001; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pCommandTimeoutErrorIntrCb != NULL)
		{
			pSdiohInternData->ErrIrqCb.pCommandTimeoutErrorIntrCb();
		}
	}

	/*Command CRC error*/    
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.COMMAND_CRC_ERROR ==0x1)     
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0002; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pCommandCrcErrorIntrCb != 0)
		{
			pSdiohInternData->ErrIrqCb.pCommandCrcErrorIntrCb();
		}
	}

	/*Command end bit error*/
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.COMMAND_END_BIT_ERROR == 0x1)      
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0004; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pCommandEndBitErrorIntrCb != 0)
		{
			pSdiohInternData->ErrIrqCb.pCommandEndBitErrorIntrCb();
		}
	}

	/*Command index error*/
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.COMMAND_INDEX_ERROR == 0x1)    
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0008; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pCommandIndexErrorIntrCb != 0x0)
		{
			pSdiohInternData->ErrIrqCb.pCommandIndexErrorIntrCb();
		}
	}

	/*data timeout error*/    
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.DATA_TIMEOUT_ERROR == 0x1)    
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0010; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pDataTimeoutErrorIntrCb != 0)
		{
			pSdiohInternData->ErrIrqCb.pDataTimeoutErrorIntrCb();
		}
	}

	/*data end bit error*/
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.DATA_END_BIT_ERROR== 0x1)    
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0020; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pDataEndBitErrorIntrCb != 0)
		{
			pSdiohInternData->ErrIrqCb.pDataEndBitErrorIntrCb();
		}
	}

	/*data CRC error*/
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.DATA_CRC_ERROR == 0x1)      
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER= 0x0040; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pDataCrcErrorIntrCb != 0)
		{
			pSdiohInternData->ErrIrqCb.pDataCrcErrorIntrCb();
		}
	}

	/*Current limitation error*/
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.CURRENT_LIMIT_ERROR == 0x1)      
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0080; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pCurrentLimitErrorIntrCb != 0)
		{
			pSdiohInternData->ErrIrqCb.pCurrentLimitErrorIntrCb();
		}
	}

	/*Auto CMD12 error*/    
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.AUTO_CMD_ERROR==0x1)    
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0100; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pAutoCommandErrorIntrCb != 0)
		{
			pSdiohInternData->ErrIrqCb.pAutoCommandErrorIntrCb();
		}
	}

	/*ADMA error*/
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.ADMA_ERROR==0x1)      
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0200; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pAdmaErrorIntrCb != 0)
		{
			pSdiohInternData->ErrIrqCb.pAdmaErrorIntrCb();
		}
	}

	/*Tuning error*/    
	if (SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER_b.TUNING_ERROR ==1)     
	{
		SDIOH->SMIH_ERROR_INTERRUPT_STATUS_REGISTER = 0x0400; // Clear interrupt flag
		if (pSdiohInternData->ErrIrqCb.pTuningErrorIntrCb != 0)
		{                                                   
			pSdiohInternData->ErrIrqCb.pTuningErrorIntrCb();
		}
	}

	return;
} // sdioh_irq_handler


/*SDIOH Interrupt Handler*/
void SD_IRQHandler()
{
	sdioh_irq_handler(&SdiohData.internalData);
}

/*End of file not truncated */
