/******************************************************************************
* WindowCoveringController.h
******************************************************************************
* Program Description:
* This file contains the WindowCovering Controller
******************************************************************************/

#ifndef _WINDOW_COVERING_CONTROLLER_H_
#define _WINDOW_COVERING_CONTROLLER_H_

#include "ZCL_Foundation.h"

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

void Handle_ZCL_Message( uint8_t buffer_index, APSDE_Data_Indication_t *p_buffer );


#endif 													/* _WINDOW_COVERING_CONTROLLER_H_*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
