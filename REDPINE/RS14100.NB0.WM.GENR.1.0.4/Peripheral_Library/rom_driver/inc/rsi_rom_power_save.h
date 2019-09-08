

/**
 * Includes
 */
#ifndef  __RSI_ROM_POWER_SAVE_H__
#define  __RSI_ROM_POWER_SAVE_H__



#include "rsi_packing.h"
#include "rsi_error.h"
#include "rsi_power_save.h"
#include "rsi_rom_table_RS1xxxx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_POWER_SAVE RSI:RS1xxxx POWER SAVE 
 *  @{
 *
 */
/**
 * @fn            STATIC INLINE error_t RSI_PS_PowerStateChangePs4toPs2(ULP_MODE_T enCtxSel          ,
 *		                                          uint8_t PwrMuxSelUlpssRam    ,
 *                                                        uint8_t pwrMuxSelM4UlpRam    ,
 *							  uint8_t pwrMuxSelM4UlpRam16K ,
 *					   		  uint8_t pwrMuxSelM4Ulp       ,
 *                                                        uint8_t pwrMuxSelUlpss       ,
 *						          uint8_t bgSampleEnable       ,
 *		                                          uint8_t dcDcEnable           ,
 *		                                          uint8_t socLdoEnable         ,
 *		                                          uint8_t standByDc
 *                                                        )
 * @brief         This API is used to used to change the power transition state from Power save state 4/3 to power save state 2
 * @param[in]     enCtxSel : is Select enum for the context top ULP mode
 *                \n  00, 10 - \ref HP-MCU/LP-MCU Mode
 *                \n  01 - \ref ULP-MCU Mode
 *                \n  11 - \ref UULP-MCU Mode
 *                \n  (1st 16K of M4 RAM is dedicated to IM,
 *                \n  2nd 16K of M4 RAM is dedicated to DM)
 * @param[in]     pwrMuxSelM4UlpRam : Select value for M4 ULP RAM Power MUX
 *                \n  3 \ref  SOC LDO
 *                \n  1 \ref  SCDCDC 0.9
 *                \n  0 \ref  SCDCDC 0.6
 * @param[in]    PwrMuxSelUlpssRam :Select value for ULPSS RAM Power MUX
 *                \n  3 \ref  SOC LDO
 *                \n  1 \ref  SCDCDC 0.9
 *                \n  0 \ref  SCDCDC 0.6
 * @param[in]    PwrMuxSelUlpssRam :Select
 * @param[in]     pwrMuxSelM4UlpRam16K : is Select value for M4 ULP RAM 16K Power MUX
 *                \n  3 \ref  SOC LDO
 *                \n  1 \ref  SCDCDC 0.9
 *                \n  0  \ref SCDCDC 0.6
 * @param[in]     pwrMuxSelM4Ulp : is Select value for M4 ULP (Peripherals + CORTEX Core )Power MUX
 *                \n  3 \ref SOC LDO
 *                \n  1 \ref SCDCDC 0.9
 *                \n  0 \ref SCDCDC 0.6
 * @param[in]     pwrMuxSelUlpss : is Select value for ULPSS(Peripherals) Power MUX
 *                \n  1 \ref SOC LDO
 *                \n  0 \ref SCDCDC 0.9
 * @param[in]     bgSampleEnable  : Value to enable or disable the bg Sample
 *                                  \n 0 :Disable
 *                                  \n 1 :Enale 
 * @param[in]     dcDcEnable      : Value to enable or disable the dcDcEnable 
 *                                  \n 0 :Disable
 *                                  \n 1 :Enale 
 * @param[in]     socLdoEnable    : Value to enable or disable the socLdoEnable 
 *                                  \n 0 :Disable
 *                                  \n 1 :Enale 
 * @param[in]     standByDc       : Value to enable or disable the standByDc
 *                                  \n 0 :Disable
 *                                  \n 1 :Enale 
 * @return        returns 0 \ref RSI_OK on success,return error code on error
 */
STATIC INLINE error_t RSI_PS_PowerStateChangePs4toPs2
(
		ULP_MODE_T enCtxSel          ,
		uint8_t PwrMuxSelUlpssRam    ,
		uint8_t pwrMuxSelM4UlpRam    ,
		uint8_t pwrMuxSelM4UlpRam16K ,
		uint8_t pwrMuxSelM4Ulp       ,
		uint8_t pwrMuxSelUlpss       ,
		uint8_t bgSampleEnable       ,
		uint8_t dcDcEnable           ,
		uint8_t socLdoEnable         ,
		uint8_t standByDc
)
{
#if defined(PS_ROMDRIVER_PRESENT)
	return ROMAPI_PWR_API->ps_power_state_change_ps4tops2
			(
					enCtxSel,
					PwrMuxSelUlpssRam,
					pwrMuxSelM4UlpRam,
					pwrMuxSelM4UlpRam16K,
					pwrMuxSelM4Ulp,
					pwrMuxSelUlpss,
					bgSampleEnable,
					dcDcEnable,
					socLdoEnable,
					standByDc
			);
#else
	return ps_power_state_change_ps4tops2
			(
					enCtxSel ,
					PwrMuxSelUlpssRam ,
					pwrMuxSelM4UlpRam,
					pwrMuxSelM4UlpRam16K,
					pwrMuxSelM4Ulp,
					pwrMuxSelUlpss,
					bgSampleEnable,
					dcDcEnable,
					socLdoEnable,
					standByDc
			);
#endif
}

/*change power state from PS2 to PS4 */


/**
 * @fn          STATIC INLINE error_t RSI_PS_PowerStateChangePs2toPs4(uint32_t PmuBuckTurnOnWaitTime , uint32_t SocLdoTurnOnWaitTime)
 * @brief	      This API is used to change the power state from PS2 to PS4
 * @param[in]	  PmuBuckTurnOnWaitTime :  PMU buck time
 * @param[in]	  SocLdoTurnOnWaitTime : soc ldo turn on time
 * @return       returns 0 \ref RSI_OK on success,return error code on error
 */
STATIC INLINE error_t RSI_PS_PowerStateChangePs2toPs4(uint32_t PmuBuckTurnOnWaitTime , uint32_t SocLdoTurnOnWaitTime)
{
#if defined(PS_ROMDRIVER_PRESENT)
	return ROMAPI_PWR_API->ps_power_state_change_ps2_to_Ps4(PmuBuckTurnOnWaitTime , SocLdoTurnOnWaitTime);
#else
	return ps_power_state_change_ps2_to_Ps4(PmuBuckTurnOnWaitTime , SocLdoTurnOnWaitTime);
#endif
}

/*clears the RAM retention configuration */

/**
 * @fn            void RSI_PS_ClrWkpUpStatus(uint32_t wakeUpIntrClear)
 * @brief	        This API is used clear the NPSS/wake up interrupts.
 * @param         wakeUpIntrClear :  OR'ed value of register bits of NPSS interrupt register
 * @return        none
 */
STATIC INLINE void RSI_PS_ClrWkpUpStatus(uint32_t wakeUpIntrClear)
{
#if defined(PS_ROMDRIVER_PRESENT)
	ROMAPI_PWR_API->ps_clr_wkp_up_status(wakeUpIntrClear);
#else
	ps_clr_wkp_up_status(wakeUpIntrClear);
#endif
}

/**
 * @fn            STATIC INLINE void RSI_PS_RetentionSleepConfig(uint32_t stack_address, uint32_t jump_cb_address, uint32_t vector_offset,uint32_t mode)
 * @brief	        This API is used configure the wake up parameter for retention sleep
 * @param         stack_address   :
 * @param         jump_cb_address :
 * @param         stack_address   :
 * @param         vector_offset   :
 * @param         mode            : Following are the possible parameters for this parameter
 *                                  \n \ref RSI_WAKEUP_FROM_FLASH_MODE    : Wakes from flash with retention. Upon wake up control jumps to wake up handler in flash.
 *                                                                 In this mode ULPSS RAMs are used to store the stack pointer and wake up handler address.
 *                                  \n \ref RSI_WAKEUP_WITH_OUT_RETENTION : Without retention sleep common for both FLASH/RAM based execution.
 *                                                                  In this mode ULPSS RAMs are used to store the stack pointer and control block address.
 *                                                                  if stack_addr and jump_cb_addr are not valid then 0x2404_0C00 and 0x2404_0000 are used
 *                                                                  for stack and control block address respectively.
 *
 *                                  \n \ref RSI_WAKEUP_WITH_RETENTION     : With retention branches to wake up handler in RAM.
 *                                                                  In this mode ULPSS RAMs are used to store the wake up handler address.
 *                                  \n \ref RSI_WAKEUP_WITH_RETENTION_WO_ULPSS_RAM : In this mode ULPSS RAMs are not used by boot-loader instead it uses the NPSS battery flip flops.
 *                                  \n \ref RSI_WAKEUP_WO_RETENTION_WO_ULPSS_RAM   :In this mode ULPSS RAMs are not used by boot-loader.
 *                                                                          Instead it uses the NPSS battery flip flops to store the stack and derives the control block address by adding 0XC00
 *                                                                          to the stack address stored in battery flops.
 * @return        none
 */
STATIC INLINE void RSI_PS_RetentionSleepConfig(uint32_t stack_address, uint32_t jump_cb_address, uint32_t vector_offset,uint32_t mode)
{
	ROMAPI_PWR_API->RSI_GotoSleepWithRetention(stack_address, jump_cb_address, vector_offset,mode);
}

/**
 * @fn            STATIC INLINE void RSI_PS_BgLdoConfig(uint8_t ldo_0p6_ctrl, uint8_t ldo_0p6_lp_mode)
 * @brief	        This API is used configure the LP low power mode and vref for DCDC1p1_lp_500uA
 * @param         ldo_0p6_ctrl :  vref for DCDC1p1_lp_500uA
 *                                0 - 0.8V
 *                                1 - 0.75V
 *                                2 - 0.7V
 *                                3 - 0.65V
 *                                4 - 0.6V
 *                                5 - 0.55V
 * @param         ldo_0p6_lp_mode : 1:enable low power mode, 0:otherwise in high power mode
 * @return        none
 */
STATIC INLINE void RSI_PS_BgLdoConfig(uint8_t ldo_0p6_ctrl, uint8_t ldo_0p6_lp_mode)
{
	ps_bg_ldo_config(ldo_0p6_ctrl , ldo_0p6_lp_mode);
}


/**
 * @fn            STATIC INLINE void RSI_PS_ConfigurTrimValues(uint16_t lf_ro_trim ,uint16_t lf_rc_trim , uint16_t hf_ro_trim ,uint16_t hf_rc_trim ,uint16_t bg_ptat_trim , uint16_t bg_trim)
 * @brief	        This API is used configure the clock and bg trim values
 * @param         lf_ro_trim : trim value for low frequency RO clock
 * @param         lf_rc_trim : trim value for low frequency RC clock
 * @param         hf_ro_trim : trim value for high frequency RO clock
 * @param         hf_rc_trim : trim value for high frequency RC clock
 * @param         bg_ptat_trim : trim value for bg ptat 
 * @param         bg_trim    : trim value for bg(Band Gap)
 * @return        none
 */
STATIC INLINE void RSI_PS_ConfigurTrimValues(uint16_t lf_ro_trim ,uint16_t lf_rc_trim , uint16_t hf_ro_trim ,uint16_t hf_rc_trim ,uint16_t bg_ptat_trim , uint16_t bg_trim)
{
	ps_configure_trim_values( lf_ro_trim , lf_rc_trim ,  hf_ro_trim , hf_rc_trim , bg_ptat_trim, bg_trim);
}

/**
 * @fn            STATIC INLINE void RSI_PS_WirelessShutdown(void)
 * @brief	        This API is used shut-down the wireless
 * @return        none
 */
STATIC INLINE void RSI_PS_WirelessShutdown(void)
{
	ps_wireless_shutdown();
}
/*end of file*/

#ifdef __cplusplus
}
#endif

#endif /*__RSI_ROM_POWER_SAVE_H__*/

/* @}end of RSI_POWER_SAVE */
