/**
 * @file       rsi_rtc.h
 * @version    1.0
 * @date       1 Aug 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains the function prototypes related to RTC
 *
 * @section Description
 * This file contains the list of function prototypes for the RTC and low level function definitions
 * Following are list of API's which need to be defined in this file.
 *
 */


/**
 * Includes
 */
#ifndef  __RSI_RTC_H__
#define  __RSI_RTC_H__
 

#ifdef __cplusplus
extern "C" {
#endif

#include "RS1xxxx.h"

#define RF_AFE_PWRCTRL_REG    *(volatile uint32_t *)(0x41300400 + 0x80)
#define XO_PROG_REG           *(volatile uint32_t *)(0x411381C4)	

/**
 ******************************************************************************
 ** \brief Day of week name definitions
 ******************************************************************************/
typedef enum RTC_DAY_OF_WEEK
{
    Sunday    = 0u,     ///< Sunday   
  	Monday    = 1u,     ///< Monday    
    Tuesday   = 2u,     ///< Tuesday   
    Wednesday = 3u,     ///< Wednesday 
    Thursday  = 4u,     ///< Thursday  
    Friday    = 5u,     ///< Friday    
    Saturday  = 6u,     ///< Saturday  
} RTC_DAY_OF_WEEK_T;

/**
 ******************************************************************************
 ** \brief Month name definitions
 ******************************************************************************/
typedef enum RTC_MONTH
{
    January   = 1u,    ///< January    
    Febuary   = 2u,    ///< February   
    March     = 3u,    ///< March      
    April     = 4u,    ///< April      
    May       = 5u,    ///< May       
    June      = 6u,    ///< June       
    July      = 7u,    ///< July       
    August    = 8u,    ///< August     
    September = 9u,    ///< September  
    October   = 10u,   ///< October    
    November  = 11u,   ///< November   
    December  = 12u,   ///< December   
} RTC_MONTH_T;

/*Interrupt enable*/
#define RTC_MSEC_INTR   BIT(18)
#define RTC_SEC_INTR    BIT(17)
#define RTC_ALARM_INTR  BIT(16)
/**
 ******************************************************************************
 **  Real Time Clock Date and Time structure
 ******************************************************************************/
/**
 * \cond HIDDEN_SYMBOLS 
 */
typedef struct RTC_TIME_CONFIG
{
	  uint16_t MilliSeconds;
    uint8_t  Second;              ///< Second (Format 0...59)         
    uint8_t  Minute;              ///< Minutes (Format 0...59)        
    uint8_t  Hour;                ///< Hour (Format 0...23)           
    uint8_t  Day;	                ///< Day (Format 1...31)            
    RTC_DAY_OF_WEEK_T  DayOfWeek; ///< Day of the week (Format 0...6) 
    RTC_MONTH_T  Month;           ///< Month (Format 1...12)          
    uint8_t  Year;                ///< Year (Format 1...99) + 2000    
    uint8_t  Century;             ///< Century
} RTC_TIME_CONFIG_T;

/**
 * \endcond
 */
/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_RTC_DRIVERS RSI:RS1xxxx RTC 
 *  @{
 *
 */
void RSI_RTC_Start(RTC_Type *Cal) ;
void RSI_RTC_Init(RTC_Type *Cal);
void RSI_RTC_Stop(RTC_Type *Cal);
error_t RSI_RTC_SetDateTime(RTC_Type *Cal , RTC_TIME_CONFIG_T *date);
error_t RSI_RTC_GetDateTime(RTC_Type *Cal ,  RTC_TIME_CONFIG_T *date);
error_t RSI_RTC_SetAlarmDateTime(RTC_Type *Cal , RTC_TIME_CONFIG_T *alarm);
void RSI_RTC_AlamEnable(RTC_Type *Cal , boolean_t val);
error_t RSI_RTC_GetAlarmDateTime(RTC_Type *Cal , RTC_TIME_CONFIG_T *alarm);
void RSI_RTC_SetDayOfWeek(RTC_Type *Cal , RTC_DAY_OF_WEEK_T dayInWeek);
void RSI_RTC_IntrUnMask(uint32_t intr);
void RSI_RTC_IntrMask(uint32_t intr);
void RSI_RTC_IntrClear(uint32_t intr);
uint32_t  RSI_RTC_GetIntrStatus(void);
void RSI_RTC_ROCLK_Calib(TimePeriodCalib_t *rtc,uint8_t enable,uint8_t periodic_en,uint8_t trigger_time,
	                          uint8_t ro_enable,uint8_t ro_periodic_en,uint8_t ro_trigger_time);
void RSI_RTC_RCCLK_Calib(TimePeriodCalib_t *rtc,uint8_t enable,uint8_t periodic_en,uint8_t trigger_time) ;
	/*Initilization RTC CALIBRATION*/
void RSI_RTC_CalibInitilization(void) ;

#ifdef __cplusplus
}
#endif


#endif /*__RSI_RTC_H__*/

/* @} end of RSI_RTC_DRIVERS */
