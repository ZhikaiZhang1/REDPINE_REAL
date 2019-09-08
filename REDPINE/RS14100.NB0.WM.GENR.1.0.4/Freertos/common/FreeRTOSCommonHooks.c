

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSCommonHooks.h"

#include "rsi_chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#if defined ( __ICCARM__ )
#define __WEAK__   __weak
#else
#define __WEAK__   __attribute__((weak))
#endif

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Delay for the specified number of milliSeconds */
void FreeRTOSDelay(uint32_t ms)
{
	TickType_t xDelayTime;

	xDelayTime = xTaskGetTickCount();
	vTaskDelayUntil(&xDelayTime, ms);
}

/* FreeRTOS malloc fail hook */
__WEAK__ void vApplicationMallocFailedHook(void)
{
	taskDISABLE_INTERRUPTS();
	//FIXME: updated thin core_cmInstr.h file in cmsis2.0 version Need to update the CMSIS files 
  //__BKPT(0x01);
	for (;; ) {}
}

/* FreeRTOS application idle hook */
__WEAK__ void vApplicationIdleHook(void)
{
	/* Best to sleep here until next systick */
	__WFI();
}

/* FreeRTOS stack overflow hook */
__WEAK__ void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	(void) pxTask;
	(void) pcTaskName;

	/* Run time stack overflow checking is performed if
	   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	   function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	//FIXME: updated thin core_cmInstr.h file in cmsis2.0 version Need to update the CMSIS files 
  //__BKPT(0x02);
	for (;; ) {}
}

/* FreeRTOS application tick hook */
__WEAK__ void vApplicationTickHook(void)
{}

#ifdef __CC_ARM
#ifndef EXTRA_HEAP_SZ
#define EXTRA_HEAP_SZ 0x6000
#endif
static uint32_t extra_heap[EXTRA_HEAP_SZ / sizeof(uint32_t)];
__attribute__((used)) unsigned __user_heap_extend(int var0, void **base, unsigned requested_size)
{
	if (requested_size > EXTRA_HEAP_SZ)
		return 0;

	*base = (void *) extra_heap;
	return sizeof(extra_heap);
}
#endif
