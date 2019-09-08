#include "base_types.h"
typedef enum PERI_CLKS
{
	M4_USART0,           /*!< Enables or Disables USART1 Peripheral clock when it is passed */
	M4_UART1,           /*!< Enables or Disables USART2 Master Peripheral clock when it is passed */ 
	M4_SSI_MST,           /*!< Enables or Disables SSI Master Peripheral clock when it is passed */
	M4_CT,              /*!< Enables or Disables CT Peripheral clock when it is passed */
	M4_SD_MEM,           /*!< Enables or Disables SD_MEM Peripheral clock when it is passed */
	M4_CCI,              /*!< Enables or Disables CCI Peripheral clock when it is passed */
	M4_QSPI,             /*!< Enables or Disables QSPI Peripheral clock when it is passed */
	M4_GSPI,             /*!< Enables or Disables GSPI Peripheral clock when it is passed */
	M4_ETHERNET,         /*!< Enables or Disables ETHERNET Peripheral clock when it is passed */
	M4_I2SM,             /*!< Enables or Disables I2SM Peripheral clock when it is passed */
	ULPSS_SSI,
	ULPSS_I2S,
	ULPSS_UART,
	ULPSS_TIMER,
	ULPSS_AUX,
	//ULPSS_VAD,
	ULPSS_TOUCH,
}PERI_CLKS_T;

uint32_t RSI_CLK_GetBaseClock(PERI_CLKS_T peri_src);
