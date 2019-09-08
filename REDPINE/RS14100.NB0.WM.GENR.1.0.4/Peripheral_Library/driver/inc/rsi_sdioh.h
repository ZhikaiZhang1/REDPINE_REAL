 
/**
 * @file       rsi_sdioh.h
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
 * @brief This files contains functions prototypes related to SDIOH Master
 *
 * @section Description
 * This file contains the list of function prototypes for the SDIOH Master and low level function definitions
 * Following are list of API's which need to be defined in this file.
 *
 */

/**
 * Includes
 */
#ifndef  __RSI_SDIOH_H__
#define  __RSI_SDIOH_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "RS1xxxx.h"
#include "base_types.h"


#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined(__ICCARM__)
#pragma language=extended
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
#pragma warning 586
#else
#warning Not supported compiler type
#endif

/** generic error codes */
typedef enum status
{
	RSI_SDIOH_OK                          = 0u,  
	RSI_SDIOH_ERROR                       = 1u,           
	RSI_SDIOH_ERROR_ACCESS_RIGHTS         = 2u, 
	RSI_SDIOH_ADDR_ALIGHMENGT_ERROR       = 3u,  
	RSI_SDIOH_INVALID_PARAMETER           = 4u,
	RSI_SDIOH_INAVLID_MODE                = 5u,  
	RSI_SDIOH_OPERATION_INPROGRESS        = 6u, 
	RSI_SDIOH_ERROR_NOT_READY             = 7u,  
	RSI_SDIOH_ERROR_UNINITIALIZED         = 8u,  
	RSI_SDIOH_ERROR_BUFFER_FULL           = 9u,   
	RSI_SDIOH_ERROR_TIMEOUT               = 9u,  
	CARD_NOT_READY_OP                    = 10u,  
	CARD_TYPE_MEMCARD                    = 11u  
} status_t;


/**********SDIOH Controller related defines and structures *******************/
typedef enum SDIOH_CMD_TYPE
{
	ABORT_CMD      = 0,  
	RESUME_CMD     = 1,  
	SUSPEND_CMD    = 2,  
	NORMAL_CMD     = 3,  
}SDIOH_CMD_TYPE_T;

typedef enum SDIOH_RESPONSE_TYPE
{
	SDIOH_RESPONSE_NONE              = 0,
	SDIOH_RESPONSE_136BIT            = 1, 
	SDIOH_RESPONSE_48BIT             = 2, 
	SDIOH_RESPONSE_48BIT_BUSY_CHECK  = 3,     
}SDIOH_RESPONSE_TYPE_T;

typedef enum SDIOH_DATA_TIMEOUT
{
	SDIOH_TIMEOUT_BASE_CLOCK_2_13 = 0, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_14 = 1,  
	SDIOH_TIMEOUT_BASE_CLOCK_2_15 = 2, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_16 = 3,  
	SDIOH_TIMEOUT_BASE_CLOCK_2_17 = 4, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_18 = 5,  
	SDIOH_TIMEOUT_BASE_CLOCK_2_19 = 6, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_20 = 7, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_21 = 8,  
	SDIOH_TIMEOUT_BASE_CLOCK_2_22 = 9, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_23 = 10, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_24 = 11, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_25 = 12, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_26 = 13, 
	SDIOH_TIMEOUT_BASE_CLOCK_2_27 = 14,     
}SDIOH_DATA_TIMEOUT_T;

typedef enum SDIOH_AUTO_CMD
{
	DISABLE_AUTO_CMD   = 0u,  
	ENABLE_AUTO_CMD12  = 1u,  
	ENABLE_AUTO_CMD23  = 2u,    
}SDIOH_AUTO_CMD_T;

typedef struct SDIOH_ERROR_INTR
{
	union 
	{
		uint16_t errorStatus;     
		struct
		{
			uint16_t commandTimeoutError    : 1;  
			uint16_t commandCrcError        : 1;  
			uint16_t commandEndBitError     : 1; 
			uint16_t commandIndexError      : 1;  
			uint16_t dataTimeoutError       : 1;  
			uint16_t dataCrcError           : 1; 
			uint16_t dataEndBitError        : 1;  
			uint16_t currentLimitError      : 1; 
			uint16_t autoCommandError       : 1;  
			uint16_t admaError              : 1;  
			uint16_t tuningError            : 1;  
			uint16_t Reserved               : 5;  
		}errorIntrStatus;
	};

}SDIOH_ERROR_INTR_STATUS_T, SDIOH_ERROR_INTR_ENABLE_T;

typedef enum SDIOH_ERROR_INTR_SEL
{
	SDIOH_COMMAND_TIMEOUT_ERROR_INTR   = 0,  
	SDIOH_COMMAND_CRC_ERROR_INTR       = 1,  
	SDIOH_COMMAND_END_BIT_ERROR_INTR    = 2,   
	SDIOH_COMMAND_INDEX_ERROR_INTR     = 3,  
	SDIOH_COMMAND_DATA_TIMEOUT_ERROR_INTR      = 4,  
	SDIOH_DATA_CRC_ERROR_INTR          = 5,   
	SDIOH_DATA_ENDBIT_ERROR_INTR       = 6,   
	SDIOH_CURRENT_LIMIT_ERROR_INTR     = 7,  
	SDIOH_AUTO_CMD_ERROR_INTR      = 8,   
	SDIOH_ADMA_ERROR_INTR             = 9,   
	SDIOH_TUNING_ERROR_INTR           = 10, 
}SDIOH_ERROR_INTR_SEL_T;

typedef struct SDIOH_ERROR_INTR_CB
{
	func_ptr_t pCommandTimeoutErrorIntrCb;    
	func_ptr_t pCommandCrcErrorIntrCb;         
	func_ptr_t pCommandEndBitErrorIntrCb;   
	func_ptr_t pCommandIndexErrorIntrCb;     
	func_ptr_t pDataTimeoutErrorIntrCb;   
	func_ptr_t pDataEndBitErrorIntrCb;     
	func_ptr_t pDataCrcErrorIntrCb;       
	func_ptr_t pCurrentLimitErrorIntrCb;   
	func_ptr_t pAutoCommandErrorIntrCb;      
	func_ptr_t pAdmaErrorIntrCb;          
	func_ptr_t pTuningErrorIntrCb;         
}SDIOH_ERROR_INTR_CB_T;

typedef struct SDIOH_NORMAL_INTR
{
	union 
	{
		uint16_t sdiohNormalIntr;             
		struct
		{
			uint16_t commandComplete  : 1;   
			uint16_t transferComplete : 1;  
			uint16_t blockGapEvent    : 1;  
			uint16_t dmaIntr          : 1;   
			uint16_t bufferWriteReady : 1;   
			uint16_t bufferReadReady  : 1;  
			uint16_t cardInsertion    : 1;   
			uint16_t cardRemoval      : 1;  
			uint16_t cardInterrupt    : 1;   
			uint16_t Reserved0        : 7;  
		}sdiohNormalIntrstatus;
	};    
}SDIOH_NORMAL_INTR_STATUS_T, SDIOH_NORMAL_INTR_ENABLE_T;

typedef enum SDIOH_NORMAL_INTR_SEL
{
	SDIOH_COMMAND_COMPLETE_INTR      = 0,   
	SDIOH_TRANSFER_COMPLETE_INTR     = 1,   
	SDIOH_BLOCK_GAP_EVENT_INTR        = 2,   
	SDIOH_DMA_INTR                  = 3,  
	SDIOH_BUFFER_WRITE_READY_INTR     = 4,   
	SDIOH_BUFFER_READ_READY_INTR      = 5, 
	SDIOH_CARD_INSERTION_INTR        = 6,  
	SDIOH_CARD_REMOVAL_INTR          = 7,  
	SDIOH_CARD_INTR                 = 8,   
}SDIOH_NORMAL_INTR_SEL_T;

typedef struct SDIOH_NORMAL_INTR_CB
{
	func_ptr_t pCommandCompleteIntrCb;    
	func_ptr_t pTransferCompleteIntrCb;    
	func_ptr_t pBlockGapIntrCb;            
	func_ptr_t pDmaIntrCb;                 
	func_ptr_t pBufferWriteReadyIntrCb;   
	func_ptr_t pBufferReadReadyIntrCb;     
	func_ptr_t pCardInsertIntrCb;         
	func_ptr_t pCardRemovalIntrCb;         
	func_ptr_t pfnCardIntrCb;               
}SDIOH_NORMAL_INTR_CB_T;

typedef struct SDIOH_AUTO_COMMAND_ERROR
{
	union 
	{
		uint16_t autoCmdError;
		struct autoCommandError
		{
			uint16_t autoCommand12NotExecuted    : 1;     
			uint16_t autoCommandTimeoutError     : 1;     
			uint16_t autoCommandCrcError         : 1;    
			uint16_t autoCommandEndBitError      : 1;     
			uint16_t autoCommandIndexError       : 1;   
			uint16_t Reserved0                       : 2;   
			uint16_t commandNotIssuedByAutoCmd12 : 1;  
			uint16_t Reserved1                       : 8;     
		}autoCommandError;
	};

}SDIOH_AUTO_COMMAND_ERROR_T;

typedef enum SDIOH_UHS_MODE
{
	UHS_NONE      = 0,
	UHS_SDR12     = 1,
	UHS_SDR25     = 2,
	UHS_SDR50     = 3,
	UHS_SDR104    = 4,
	UHS_DDR50     = 5,
}SDIOH_UHS_MODE_T;

typedef struct SDIOH_CONFIG
{
	boolean_t                       selectAdma;              
	boolean_t                       highSpeedEnable ;   
	SDIOH_UHS_MODE_T                uhsMode;   
	boolean_t                       enable4BitMode;   
	SDIOH_NORMAL_INTR_ENABLE_T*     normalIntrEnable;    
	SDIOH_NORMAL_INTR_CB_T*         normalIntrCb;     
	SDIOH_ERROR_INTR_ENABLE_T*      errorIntrEnable;       
	SDIOH_ERROR_INTR_CB_T*          errorIntrCb;       
}SDIOH_CONFIG_T;

typedef struct SDIOH_COMMAND_CONFIG
{
	uint8_t                  cmdIndex;          
	uint32_t                 argument;         
	SDIOH_CMD_TYPE_T         cmdType;           
	boolean_t                dataPresent;         
	boolean_t                checkCmdIndex;      
	boolean_t                checkCmdCrc;           
	SDIOH_RESPONSE_TYPE_T    responseType;    
	SDIOH_AUTO_CMD_T         autoCmdType;     

}SDIOH_COMMAND_CONFIG_T;

typedef struct SDIOH_DATA_CONFIG
{
	boolean_t  multiBlock;          
	boolean_t  read;                   
	uint16_t   blockSize;            
	uint16_t   blockCount;            
	boolean_t  enableBlockCount;       
	boolean_t  dmaEnable;               
	uint32_t   admaDespTableAddress;  
	uint8_t    dataTimeout;                
}SDIOH_DATA_CONFIG_T;

typedef struct SDIOH_COMMAND_FIELD
{
	uint16_t respType                : 2;
	uint16_t resrvd                   : 1;
	uint16_t cmdCrcCheckEnable     : 1;
	uint16_t cmdIndexCheckEnable   : 1;
	uint16_t dataPresentSelect      : 1;
	uint16_t cmdType                 : 2;
	uint16_t cmdIndex                : 6;
} SDIOH_COMMAND_FIELD_T;

typedef enum SDIOH_VOLTAGE_SEL
{
	VOLTAGE_18V = 0,    
	VOLTAGE_30V = 1,   
	VOLTAGE_33V = 2,       
}SDIOH_VOLTAGE_SEL_T;

typedef enum SDIOH_RESET
{
	SDIOH_DATA_LINE_RESET     = 0, 
	SDIOH_COMMAND_LINE_RESET  = 1, 
	SDIOH_ALL_RESET          = 2,     
}SDIOH_RESET_T;

/// Enumeration to define an index for each enabled SDIOH instance
typedef enum EN_SDIOH_INSTANCE_INDEX
{
	SDIOH_INSTANCE_INDEX_MAX
} EN_SDIOH_INSTANCE_INDEX_T;

typedef struct SDIOH_INTERN_DATA
{
	SDIOH_NORMAL_INTR_CB_T   normalIntrCb;
	SDIOH_ERROR_INTR_CB_T    ErrIrqCb;

}SDIOH_INTERN_DATA_T;

/// SDIOH instance data type
typedef struct SDIOH_INSTANCE_INTERNAL_DATA
{
	SDIOH_INTERN_DATA_T internalData; ///< module internal data of instance
} SDIOH_INSTANCE_INTERNAL_DATA_T;


/****************SDIOH command related defines and structures**************************/
#define RETRY_TIME              (500)
#define BIT4_BUS_WIDTH_ARG        (0x80000E82)
#define BIT4_BUS_WIDTH_SET_ARG    (0x00000E00)
#define LOW_SPEED_CHECK_ARG       (0x00001000)
#define I_O_BLOCK_SIZE_ARG        (0x80022000)
#define I_O_BLOCK_SIZE_ARG_1      (0x80022201)
#define HIGH_SPEED_SPRT_ARG       (0x00002600)
#define SELECT_FUNC_ARG           (0x00001A00)
#define RESET_ARG                 (0x80000C08)
#define CCCR_BYTE_READ            (0x04000016)
#define CD_DISABLE_ARG            (0x80000E80)

//Function1 argument
#define FUCNTION1_CHECK_ARG       (0x00000400)
#define FUCNTION1_ENB_ARG         (0x80000402)
#define FUCNTION1_READY_ARG       (0x00000600)
#define FUNC1_INTR_ENB_ARG        (0x80000803)
#define FUNC1_INTR_CHECK_ARG      (0x00000800)
#define FUNC1_READY               (1 << 1)
#define FUNC1_ENABLE              (1 << 1)
#define CSA_SUPPORT_ARG		        (0x00020000)
#define CSA_ENABLE_ARG		        (0x80020080)

// Function2 argument
#define FUCNTION2_CHECK_ARG       (0x00000400)
#define FUCNTION2_ENB_ARG         (0x80000404)
#define FUCNTION2_READY_ARG       (0x00000600)
#define FUNC2_INTR_ENB_ARG        (0x80000805)
#define FUNC2_READY               (1 << 2)
#define FUNC2_ENABLE              (1 << 2)

#define LOW_SPEED_CHECK           (1 << 6)
#define BIT4_MODE_CHECK           (1 << 7)
#define BUS_BIT				            (1 << 1)

#define SD_ACMD_OFFSET            (0x40)
#define MMC_CMD_TAG               (0x80)
#define ARG_ACMD41_BUSY           (0x80000000)       
#define OCR_CAPACITY_MASK         (0x40000000)

#define CHECK_HIGH_SPEED_SUPPORT        (0x00002600)
#define ENABLE_HIGH_SPEED_MODE_ARG      (0x80002602)    //CMD52 ARG(CCR REG OFFSET IS 0X13 IE:13 LEFT SHIF BY 9,HERE 9 MEANS 8 BITS DATA AND 1 BIT STUFF IN CMD 52(SETTING BIT 1 IN 13TH PFFSET))

#define CHECK_UHS_SUPPORT_MODES   (0x00002800)    //CMD52 ARG(CCR REG OFFSET IS 0X14 IE:14 LEFT SHIF BY 9,HERE 9 MEANS 8 BITS DATA AND 1 BIT STUFF IN CMD 52) (CHECK ARGMENT)
#define UHS_1_SDR25_MODE_ARG      (0x80002602)    //CMD52 ARG(CCR REG OFFSET IS 0X13 IE:13 LEFT SHIF BY 9,HERE 9 MEANS 8 BITS DATA AND 1 BIT STUFF IN CMD 52(SETTING BIT 1 IN 13TH PFFSET))
#define UHS_1_SDR50_MODE_ARG      (0x80002604)    //BIT 2 SETTING IN 13TH OFFSET
#define UHS_1_SDR104_MODE_ARG     (0x80002606)    //SETTING  BIT 1 AND 2 IN 13TH OFFSET (CCCR REG)
#define UHS_1_DDR50_MODE_ARG      (0x80002608)     //SETTING BIT 3 IN 13TH OFFSET

#define HIGH_SPEED_BIT				    BIT(0)
#define UHS_SUPPORT_BITS			    0x7    //bit 0,1,2

#define CMD0             (0)
#define CMD3             (3)
#define CMD5             (5)
#define CMD7             (7)
#define CMD11            (11)
#define CMD52            (52) 
#define CMD53            (53)  

typedef enum SDIOH_CLOCK
{
	SDIOH_CLOCK_400KHZ    = (400 * 1000),
	SDIOH_CLOCK_20MHZ     = (20 * 1000 * 1000),
	SDIOH_CLOCK_25MHZ     = (25 * 1000 * 1000),
	SDIOH_CLOCK_12MHZ     = (12 * 1000 * 1000),
	SDIOH_CLOCK_15MHZ     = (15 * 1000 * 1000),
	SDIOH_CLOCK_30MHZ     = (30 * 1000 * 1000),
	SDIOH_CLOCK_35MHZ     = (35 * 1000 * 1000),
	SDIOH_CLOCK_40MHZ     = (40 * 1000 * 1000),
	SDIOH_CLOCK_45MHZ     = (45 * 1000 * 1000),
	SDIOH_CLOCK_50MHZ     = (50 * 1000 * 1000),
	SDIOH_CLOCK_80MHZ     = (80 * 1000 * 1000),
}SDIOH_CLOCK_T;

typedef enum
{
	SDIOH_CARD_STANDARD    = 0u,
	SDIOH_CARD_HIGH_CAPACITY     = 1u,
}SDIOH_CARD_TYPE_T;

typedef enum
{
	SDIOH_CMD_LINE      = 1u,
	SDIOH_DATA_LINE     = 2u,
}SDIOH_REG_TYPE_T;

typedef enum
{
	SDIOH_WRITE_DIR         = 0u,         
	SDIOH_READ_DIR          = 1u,        
}SDIOH_READ_WRITE_DIR;

typedef enum SDIOH_RESPONSE
{
	SDIOH_RSP_NONE                = 0u,         
	SDIOH_RSP_R2                  = 1u,
	SDIOH_RSP_R3R4                = 2u,
	SDIOH_RSP_R1NORMALR5R6R7      = 3u,
	SDIOH_RSP_R1AUTO              = 4u,
	SDIOH_RSP_R1BNORMAL           = 5u,
	SDIOH_RSP_R1BAUTO             = 6u,
	SDIOH_RSP_R5B                 = 7u,
}SDIOH_RESPONSE_T;

typedef enum SDIOH_CARD_STATUS
{
	SDIOH_CARD_STATE_IDLE     = 0u,
	SDIOH_CARD_STATE_READY    = 1u,
	SDIOH_CARD_STATE_IDENT    = 2u,
	SDIOH_CARD_STATE_STBY     = 3u,
	SDIOH_CARD_STATE_TRAN     = 4u,
	SDIOH_CARD_STATE_DATA     = 5u,
	SDIOH_CARD_STATE_RCV      = 6u,
	SDIOH_CARD_STATE_PRG      = 7u,
	SDIOH_CARD_STATE_DIS      = 8u,
	SDIOH_CARD_STATE_REV      = 9u,
}SDIOH_CARD_STATUS_T;

typedef enum SDIOH_CMD_EVENT
{
	SDIOH_CMD_COMPLETE_EVENT       = 1u,          
	SDIOH_TRANSFER_COMPLETE_EVENT  = 2u,
	SDIOH_READ_READY_EVENT         = 3u,
	SDIOH_WRITE_READY_EVENT        = 4u,
	SDIOH_ADMA_COMPLETE_EVENT      = 5u,
}SDIOH_CMD_EVENT_T;

typedef enum SDIOH_BUS_MODE
{
	SDIOH_BUS_WIDTH1     = 0u,
	SDIOH_BUS_WIDTH4     = 1u,
	SDIOH_BUS_WIDTH8     = 2u,
}SDIOH_BUS_MODE_T;


typedef struct SDIOH_COMMAND_CONFIG_STRUCT
{
	SDIOH_BUS_MODE_T        busWidthMode;        
	uint32_t                clock;          
	boolean_t               highSpeedEnable;
	boolean_t               admaMode;        
}SDIOH_COMMAND_CONFIG_STRUCT_T;

typedef struct SDIOH_ADMA_DESC_TABLE
{
	uint16_t attrValid   :1;
	uint16_t attrEnd     :1;
	uint16_t attrInt     :1;
	uint16_t reserved1   :1;
	uint16_t act         :2;
	uint16_t reserved2   :10;
	uint16_t len;
	uint32_t addr;
} SDIOH_ADMA_DESC_TABLE_T;

typedef struct SDIOH_EVENT
{
  uint8_t commandComplete;
  uint8_t transferComplete;
	uint8_t bufferReadReady;
	uint8_t bufferWriteReady;
 	uint8_t admaComplete;
	uint8_t cardInsert;
	uint8_t cardRemove;
	uint8_t cmdTimeOut;
	uint8_t crcError;
	uint8_t transferTimeOut;
	uint8_t dataComplete;
	uint8_t txTimeOut;
	uint8_t rxTimeOut;
}SDIOH_EVENT_T;

typedef struct SDIOH_CMD
{
	uint32_t                cmdIdx;
	uint32_t                cmdArg;
	uint32_t                response[4];
	SDIOH_RESPONSE_T        responseType;
	uint8_t                 *data;
	uint8_t                 direction;
	uint32_t                blockCount;
	uint32_t                blockSize;
	SDIOH_EVENT_T           event;
}SDIOH_CMD_T;

typedef struct SDIOH_CMD_OCR
{
	uint32_t reserved1   :8;    
	uint32_t v20_21      :1;
	uint32_t v21_22      :1;
	uint32_t v22_23      :1;
	uint32_t v23_24      :1;
	uint32_t v24_25      :1;
	uint32_t v25_26      :1;
	uint32_t v26_27      :1;
	uint32_t v27_28      :1;
	uint32_t v28_29      :1;
	uint32_t v29_30      :1;
	uint32_t v30_31      :1;
	uint32_t v31_32      :1;
	uint32_t v32_33      :1;
	uint32_t v33_34      :1;
	uint32_t v34_35      :1;
	uint32_t v35_36      :1;
	uint32_t s18A        :1;
	uint32_t reserved2   :2; 
	uint32_t memPresent  :1;
	uint32_t ioNum       :3;
	uint32_t cardReady   :1;
}SDIOH_CMD_OCR_T;

typedef struct SDIOH_CMD_INFO
{
	SDIOH_CARD_TYPE_T       cardType;          
	uint32_t                maxSectorNum;       
	SDIOH_CMD_OCR_T         ocr;                   
	uint8_t                 cid[16];                   
	uint8_t                 csd[16];               
	uint16_t                rca;                      
	uint8_t                 response[4];
} SDIOH_CMD_INFO_T;

typedef struct SDIOH_CARD_CONFIG
{
	SDIOH_BUS_MODE_T        busWidthMode;                 
	uint32_t                clock;                    
	boolean_t               highSpeedEnable;          
	SDIOH_UHS_MODE_T        uhsModeSelect;
	uint32_t                voltage;    
	boolean_t               admaMode;                
	SDIOH_CMD_INFO_T        infoFromCard;                   
	uint16_t                byteBlockSize;
	uint16_t                numberOfBlocks;
	uint8_t                 funcNum   : 3; 
	uint8_t                 blockModeEnable : 1;
	uint8_t                 opCode    : 1;
  
}SDIOH_CARD_CONFIG_T;

typedef struct SDIOH_CCCR_REG_INFO
{
	__IO uint8_t  ccidSdioRevReg;                
	__IO uint8_t  sdSpecRevReg;                
	__IO uint8_t  ioEnableReg;                
	__IO uint8_t  ioReady;
	__IO uint8_t  intEnable;
	__IO uint8_t  intrPending;
	__IO uint8_t  ioAbort;
	__IO uint8_t  busControl;
	__IO uint8_t  cardCapacity;
	__IO uint8_t  commonCisPointer;
	__IO uint8_t  commonCisPointer1;
	__IO uint8_t  commonCisPointer2;
	__IO uint8_t  busSped;
	__IO uint8_t  funcSelect;
	__IO uint8_t  execFlags;
	__IO uint8_t  redayFlags;
	__IO uint16_t funcoBlockSize;
	__IO uint8_t  powerControl;
	__IO uint8_t  busSpeddSelect;
	__IO uint8_t  uhs1Support;
	__IO uint8_t  driverStrength;
	__IO uint8_t  intrExtension;
} SDIOH_CCCR_REG_INFO_T;

#if defined(__CC_ARM)
#pragma pop
#elif defined(__ICCARM__)
/* leave anonymous unions enabled */
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
#pragma warning restore
#else
#warning Not supported compiler type
#endif


/****************smih command related function prototypes**************************/

void usdelay( uint32_t Delay );

void rsi_delay_ms(uint32_t delay_ms);

void sdiohBufferReadReadyEventCB( void );

void sdiohBufferWriteReadyEventCB( void );

void sdiohCommandCompleteEventCB( void );

void sdiohTransferCompleteEventCB( void );

void sdiohAdmaCompleteEventCB( void );

void sdiohCommandTimeoutEventCB( void );

void sdiohCardRemoveEventCB( void );

void sdiohCardInsertEventCB( void );

status_t RSI_SDIOH_RegisterInfo(SDIOH_CCCR_REG_INFO_T *pRegInfo);

status_t RSI_SDIOH_BusWidthConfig( SDIOH_CMD_INFO_T *pcmdInfo,SDIOH_BUS_MODE_T BusWidth );

status_t RSI_SDIOH_ClockEnable(SDIOH_CARD_CONFIG_T *pSdiohConfig, uint32_t req );

status_t RSI_SDIOH_GoIdleStateCmd0( void );                                                     

status_t RSI_SDIOH_SendRelativeAddressCmd3( SDIOH_CMD_INFO_T *pcmdInfo, uint32_t CmdArg );      

status_t RSI_SDIOH_SwitchVoltageCmd11( SDIOH_CMD_INFO_T *pcmdInfo, uint32_t cmdArg ); 

status_t RSI_SDIOH_SelectCardCmd7( uint32_t u32RCA );            

status_t RSI_SDIOH_StopTransmissionCmd12( void );                         

status_t RSI_SDIOH_SetBusWidthCmd52( SDIOH_CMD_INFO_T *pcmdInfo, uint32_t cmdArg );                               

status_t RSI_SDIOH_SdioCommandInitialization(SDIOH_COMMAND_CONFIG_STRUCT_T *pSdiohConfig );

status_t RSI_SDIOH_SendCommand(SDIOH_CMD_T *pCommand);

status_t RSI_SDIOH_InitializationCmd5( SDIOH_CMD_INFO_T *pcmdInfo);

status_t RSI_SDIOH_ReInitializationCmd5( SDIOH_CMD_INFO_T *pcmdInfo,SDIOH_CARD_CONFIG_T *pSdiohConfig);

status_t RSI_SDIOH_SendCommand52(SDIOH_CMD_INFO_T *pcmdInfo,uint32_t Argument);

status_t RSI_SDIOH_ByteBlockWriteCmd53(SDIOH_CARD_CONFIG_T *pSdiohConfig,uint8_t *pBuffer ,uint32_t Addr);

status_t RSI_SDIOH_ByteBlockReadCmd53(SDIOH_CARD_CONFIG_T *pSdiohConfig,uint8_t *pBuffer  ,uint32_t Addr);

status_t RSI_SDIOH_Enumeration(SDIOH_CARD_CONFIG_T *pSdiohConfig );


/**********SDIOH Controller related function prototypes*******************/
/* Int Enable/Disable */
status_t               sdioh_error_interrupt_signal_enable(SDIOH_ERROR_INTR_SEL_T ErrorIrqSel);
status_t               sdioh_error_interrupt_signal_disable(SDIOH_ERROR_INTR_SEL_T ErrorIrqSel);
status_t               sdioh_normal_interrupt_signal_enable(SDIOH_NORMAL_INTR_SEL_T NormalIrqSel);
status_t               Smih_NormalIrqDisable(SDIOH_NORMAL_INTR_SEL_T NormalIrqSel);
/* Sdioh interrupt */
void                   sdioh_irq_handler( SDIOH_INTERN_DATA_T* pSdiohInternData );
status_t               sdioh_host_initialization(SDIOH_CONFIG_T* pConfig );
status_t               Smih_DeInitialization(boolean_t Intr,boolean_t IntCb);
status_t               sdioh_internal_clock_enable(void);
status_t               sdioh_internal_clock_disable(void);
status_t               sdioh_clock_enable(void);
status_t               sdioh_clock_disable(void);
status_t               sdioh_clock_division_set(uint16_t Div);
status_t               sdioh_bus_width_set(boolean_t Enable4BitMode);
status_t               sdioh_bus_speed_mode_set(boolean_t HighSpeedMode);
status_t               sdioh_bus_voltage_select(SDIOH_VOLTAGE_SEL_T enVoltage);
status_t               sdioh_bus_power_on(void);
status_t               sdioh_bus_power_off(void);
status_t               sdioh_send_command(SDIOH_COMMAND_CONFIG_T* pConfig );
status_t               sdioh_get_response(uint16_t* pResponseData,uint8_t ResponseRegCount);
status_t               sdioh_data_transfer_initialization(SDIOH_DATA_CONFIG_T* pDataConfig);
status_t               sdioh_stop_at_block_gap(void);
status_t               sdioh_transfer_restart(void);
status_t               sdioh_error_interrupt_status_get (SDIOH_ERROR_INTR_STATUS_T* pStatus);
status_t               sdioh_error_interrupt_status_clear(SDIOH_ERROR_INTR_STATUS_T* pStatus);
status_t               sdioh_normal_interrupt_status_get(SDIOH_NORMAL_INTR_STATUS_T* pStatus);
status_t               sdioh_normal_interrupt_status_clear(SDIOH_NORMAL_INTR_STATUS_T* pStatus);
status_t               sdioh_auto_command_error_status_get(SDIOH_AUTO_COMMAND_ERROR_T* pStatus);
boolean_t              sdioh_wait_for_card_insert(void);
status_t               sdioh_software_reset(SDIOH_RESET_T resetEn);
status_t               sdioh_set_wakeup_factor(boolean_t CardRemovalWakeup,boolean_t CardInsertWakeup,boolean_t CardIntrWakeup);
status_t               sdioh_uhs_mode_select(SDIOH_UHS_MODE_T enUhsMode);
void                   sdioh_18v_signal_enable(void);

#ifdef __cplusplus
}
#endif

#endif  /*__RSI_SDIOH_H__*/

/* @}end of  RSI_EGPIO_DRIVERS */
