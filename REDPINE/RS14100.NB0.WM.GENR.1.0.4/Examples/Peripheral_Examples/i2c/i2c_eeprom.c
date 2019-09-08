/**
 * @file       i2c_eeprom.c
 * @version    1.0
 * @date       20 Dec 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example describing usage of I2C CMSIS APIs.
 *
 * @section Description
 * I2C read and write operations with EEPROM Slave device example.
 */
 

/* Includes ------------------------------------------------------------------*/
#include "I2C.h"

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/ 
#ifndef EEPROM_I2C_PORT
#define EEPROM_I2C_PORT       0         /* I2C Port number                    */
#endif
 
#define SIZE_BUFFERS          16
#define EEPROM_I2C_ADDR       0x50      /* 24LC128 EEPROM I2C address         */
 
#define EEPROM_MAX_ADDR       16384     /* Max memory locations available     */
#define EEPROM_MAX_WRITE      16        /* Max bytes to write in one step     */
 
#define A_WR                  0         /* Master will write to the I2C       */
#define A_RD                  1         /* Master will read from the I2C      */
#define OFFSET_ADDR           0x0
#define TX_LEN                16
#define RX_LEN                16

/* Private variables ---------------------------------------------------------*/
static uint16_t DeviceAddr;
static uint8_t wr_buf[EEPROM_MAX_WRITE + 2];
static uint8_t rd_buf[SIZE_BUFFERS];
static uint8_t buf[SIZE_BUFFERS];

extern ARM_DRIVER_I2C Driver_I2C0;
static ARM_DRIVER_I2C *I2Cdrv = &Driver_I2C0;
extern ARM_DRIVER_I2C Driver_I2C0;
ARM_DRIVER_I2C *drv_info; 
uint32_t tx_done = 0,rx_done = 0,xfer_done = 0, stop_det =0; 
uint32_t cnt_num =0;
I2C_RESOURCES i2c;
ARM_I2C_CAPABILITIES drv_capabilities;
/* Private function prototypes -----------------------------------------------*/
void ErrorHandler(void);
void WriteDataGenerate(void);
int32_t EEPROM_WriteBuf (uint8_t sub_addr, const uint8_t *buf, uint32_t len);
int32_t EEPROM_ReadBuf (uint16_t addr, uint8_t *buf, uint32_t len);
int32_t EEPROM_Initialize (void);
uint32_t EEPROM_GetSize (void);
void setup_i2c (void);
void read_capabilities (void);
void ARM_I2C_SignalEvent(uint32_t event);

/* Private functions ---------------------------------------------------------*/


void ErrorHandler(void)
{
  while(1);
}  

/**
 * @brief	 Fills transmit buffer data
 * @return none
 */
void WriteDataGenerate(void)
{
	uint32_t loop;
	
	/* Put some data in the source buffer for */
	for (loop = 0; loop < SIZE_BUFFERS; loop++) 
	{
		buf[loop] = (loop + 0x1);
		rd_buf[loop] = 0;
	}	
} 
 
/**
 * @brief	 Writes data to EEPROM slave
 * @param[in]  sub_addr EEPROM offset to write
 * @param[in]  buf  write data buffer
 * @param[in]  len  number bytes to write
 * @return write success or fail status
 */
int32_t EEPROM_WriteBuf (uint8_t sub_addr, const uint8_t *buf, uint32_t len) 
{ 
  wr_buf[0] = sub_addr;
 
  memcpy ((void  *)&wr_buf[1], &buf[0], 16);
	
  I2Cdrv->MasterTransmit (DeviceAddr, wr_buf, len+1 , false);
  while (I2Cdrv->GetStatus().busy);
	
	cnt_num = I2Cdrv->GetDataCount ();
  if (cnt_num != (len + 1))
  {		
		return -1;
    /* Acknowledge polling */
	}
  else
	{
		tx_done = 1;
	}
  return 0;
}
 
/**
 * @brief	 Reads data from EEPROM slave
 * @param[in]  addr EEPROM offset to read
 * @param[in]  buf  Read data buffer
 * @param[in]  len  number bytes to read
 * @return Read success or fail status
 */
int32_t EEPROM_ReadBuf (uint16_t addr, uint8_t *buf, uint32_t len) 
{
  uint8_t a[2];

  a[0] = (uint8_t)(addr & 0xFF);
 
  I2Cdrv->MasterTransmit (DeviceAddr, a, 1, false);
  while (I2Cdrv->GetStatus().busy);
	
  I2Cdrv->MasterReceive (DeviceAddr, rd_buf, len, false);
  while (I2Cdrv->GetStatus().busy);
	
  cnt_num = I2Cdrv->GetDataCount ();
	
  if (cnt_num != (len))
  {		
		return -1;
    /* Acknowledge polling */
	}
	else
  {
		rx_done = 1;
	}
  return 0;
}

/*	 Initializes I2C interface   */
int32_t EEPROM_Initialize (void) 
{
  I2Cdrv->Initialize   (ARM_I2C_SignalEvent);
  I2Cdrv->PowerControl (ARM_POWER_FULL);
  I2Cdrv->Control      (ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);

  DeviceAddr = EEPROM_I2C_ADDR;
	return 0;
}

/**
 * @brief	 Gets EEPROM Max size 
 * @return EEPROM size
 */
uint32_t EEPROM_GetSize (void) 
{
  return EEPROM_MAX_ADDR;
}

/**
 * @brief	 I2C driver version check
 * @return	none
 */
void setup_i2c (void)  
{
  ARM_DRIVER_VERSION  version;
 
  drv_info = &Driver_I2C0;  
  version = drv_info->GetVersion ();
  if (version.api < 0x10A)   
  {
    // error handling
    ErrorHandler();
    return;
  }
}

/**
 * @brief	 Capabilities of I2C driver
 * @param  none   
 * @return none
 */
void read_capabilities (void)  
{
   // interrogate capabilities
  drv_info = &Driver_I2C0;  
  drv_capabilities = drv_info->GetCapabilities ();
}

/**
 * @brief	 I2C event handler
 * @param  event I2C transmit and receive events
 * @return none
 */
void ARM_I2C_SignalEvent(uint32_t event)
{
   switch(event)
   {
     case ARM_I2C_EVENT_TRANSFER_DONE:
      break;
     case ARM_I2C_EVENT_TRANSFER_INCOMPLETE:
       break;
     case ARM_I2C_EVENT_SLAVE_TRANSMIT:
       break;    
     case ARM_I2C_EVENT_SLAVE_RECEIVE:
       break;
     case ARM_I2C_EVENT_ADDRESS_NACK:
       break;
     case ARM_I2C_EVENT_GENERAL_CALL:
       break;
     case ARM_I2C_EVENT_ARBITRATION_LOST:
       break;
     case ARM_I2C_EVENT_BUS_ERROR:
       break;
     case ARM_I2C_EVENT_BUS_CLEAR:
       break;
   }
   return;
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{	
  /* At this stage the MICROCONTROLLER clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs9116.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs9116.c file
	 */
   uint32_t loop =0;
	
	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();
	/* check's I2C driver version */
   setup_i2c();
	
	/* Fills transmit buffer with data*/
	 WriteDataGenerate();
	
	/*Read capabilities of I2C driver*/
   read_capabilities();
	
	/* Initializes I2C interface  */
   EEPROM_Initialize();
	
	/* Gets EEPROM Max size */
   EEPROM_GetSize();
	
	/* Writes data to EEPROM slave */
   EEPROM_WriteBuf(OFFSET_ADDR,buf,TX_LEN);	
	 //1ms delay after stop to start 
	 for(loop =0; loop < 10000; )
	 {
		 loop++;
	 }	
	 /* Reads data from EEPROM slave */
   EEPROM_ReadBuf(OFFSET_ADDR, rd_buf, RX_LEN);
	
	 /*waits until rx_done=0  */
	 while(!rx_done);

	 /*  Uninitialized I2C */
   I2Cdrv->Uninitialize();
   while(1);
}

