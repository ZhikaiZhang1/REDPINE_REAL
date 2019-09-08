/**
 * @file       CMSIS_CAN_Example.c
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
 * @brief This files contains CMSIS CAN example code of all end to end functionality
 *
 * @section Description
 * This files contains example code to demonstrate the CAN peripheral functionality.
 *
 *
 */
/*Includes*/
#include "rsi_chip.h"
#include "Driver_CAN.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define  CAN_CONTROLLER          1       // CAN Controller number
#define _CAN_Driver_(n)          Driver_CAN##n
#define  CAN_Driver_(n)         _CAN_Driver_(n)
extern   ARM_DRIVER_CAN          CAN_Driver_(CAN_CONTROLLER);
#define  ptrCAN                 (&CAN_Driver_(CAN_CONTROLLER))

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint32_t                        rx_obj_idx  = 0xFFFFFFFFU;
uint8_t                         rx_data[8];
ARM_CAN_MSG_INFO                rx_msg_info;
uint32_t                        tx_obj_idx  = 0xFFFFFFFFU;
uint8_t                         tx_data[8];
ARM_CAN_MSG_INFO                tx_msg_info;
/* Private function prototypes -----------------------------------------------*/
static void Error_Handler (void);
static void CAN_SignalUnitEvent   (uint32_t event);
static void CAN_SignalObjectEvent (uint32_t obj_idx, uint32_t event);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief	Handles the error in driver
 * @return	Nothing
 */
static void Error_Handler (void) { while (0); }

/**
 * @brief	Call back function for CAN uint events
 * @return	Nothing
 */
static void CAN_SignalUnitEvent (uint32_t event) {

	if (event == ARM_CAN_EVENT_UNIT_ACTIVE) {

	}if (event == ARM_CAN_EVENT_UNIT_WARNING) {

	}if (event == ARM_CAN_EVENT_UNIT_PASSIVE) {

	}if (event == ARM_CAN_EVENT_UNIT_BUS_OFF) {
	}
}

/**
 * @brief	Call back function for CAN object events
 * @return	Nothing
 */
static void CAN_SignalObjectEvent (uint32_t obj_idx , uint32_t event) {

	if (obj_idx == rx_obj_idx) {                  // If receive object event
		if (event == ARM_CAN_EVENT_RECEIVE) {       // If message was received successfully
			if (ptrCAN->MessageRead(rx_obj_idx, &rx_msg_info, rx_data, 8U) > 0U) {
				// Read received message
				// process received message ...
				memset(rx_data , 0x00 , 8U);
			}
		}
	}
	if (obj_idx == tx_obj_idx) {                  // If transmit object event
		if (event == ARM_CAN_EVENT_SEND_COMPLETE) { // If message was sent successfully
			// acknowledge sent message ...
			ptrCAN->MessageSend(tx_obj_idx, &tx_msg_info, tx_data, 8U);        // Send data message with 1 data byte
		}
	}
}

/**
 * @brief	Application entry function for CAN example
 * @return	Nothing
 */
int main (void) {
	ARM_CAN_CAPABILITIES     can_cap;
	ARM_CAN_OBJ_CAPABILITIES can_obj_cap;
	int32_t                  status;
	uint32_t                 i, num_objects;
  	/*Configures the system default clock and power configurations*/
 	SystemCoreClockUpdate();

	/*Default keep M4 in reference clock*/
	RSI_CLK_SetSocPllFreq(M4CLK , 30000000 , 40000000);
	/*Switch M4 clock to PLL clock for speed operations*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_SOCPLLCLK,0);
	/*CAN Clock configuration */
	RSI_CLK_CanClkConfig(M4CLK,0,ENABLE_STATIC_CLK);
	can_cap = ptrCAN->GetCapabilities (); // Get CAN driver capabilities
	num_objects = can_cap.num_objects;    // Number of receive/transmit objects

	status = ptrCAN->Initialize    (CAN_SignalUnitEvent, CAN_SignalObjectEvent); // Initialize CAN driver
	if (status != ARM_DRIVER_OK ) { Error_Handler(); }

	status = ptrCAN->PowerControl  (ARM_POWER_FULL);                             // Power-up CAN controller
	if (status != ARM_DRIVER_OK ) { Error_Handler(); }

	status = ptrCAN->SetMode       (ARM_CAN_MODE_INITIALIZATION);                // Activate initialization mode
	if (status != ARM_DRIVER_OK ) { Error_Handler(); }

	status = ptrCAN->SetBitrate    (ARM_CAN_BITRATE_NOMINAL,                     // Set nominal bitrate
			82920U,                               // Set bitrate to 82.92 kbit/s
			ARM_CAN_BIT_PROP_SEG(5U)   |          // Set propagation segment to 5 time quanta
			ARM_CAN_BIT_PHASE_SEG1(4U) |          // Set phase segment 1 to 1 time quantum (sample point at 87.5% of bit time)
			ARM_CAN_BIT_PHASE_SEG2(2U) |          // Set phase segment 2 to 1 time quantum (total bit is 8 time quanta long)
			ARM_CAN_BIT_SJW(4U));                 // Resynchronization jump width is same as phase segment 2
	if (status != ARM_DRIVER_OK ) { Error_Handler(); }

	for (i = 0U; i < num_objects; i++) {                                         // Find first available object for receive and transmit
		can_obj_cap = ptrCAN->ObjectGetCapabilities (i);                         // Get object capabilities
		if      ((rx_obj_idx == 0xFFFFFFFFU) && (can_obj_cap.rx == 1U)) { rx_obj_idx = i; }
		else if ((tx_obj_idx == 0xFFFFFFFFU) && (can_obj_cap.tx == 1U)) { tx_obj_idx = i; break; }
	}

	if ((rx_obj_idx == 0xFFFFFFFFU) || (tx_obj_idx == 0xFFFFFFFFU)) { Error_Handler(); }

	// Set filter to receive messages with extended ID 0x456 to receive object
	status = ptrCAN->ObjectSetFilter(rx_obj_idx, ARM_CAN_FILTER_ID_EXACT_ADD, ARM_CAN_STANDARD_ID(0x456U), 0U);
	if (status != ARM_DRIVER_OK ) { Error_Handler(); }

	status = ptrCAN->ObjectConfigure(tx_obj_idx, ARM_CAN_OBJ_TX);                // Configure transmit object
	if (status != ARM_DRIVER_OK ) { Error_Handler(); }

	status = ptrCAN->ObjectConfigure(rx_obj_idx, ARM_CAN_OBJ_RX);                // Configure receive object
	if (status != ARM_DRIVER_OK ) { Error_Handler(); }

	memset(&tx_msg_info, 0U, sizeof(ARM_CAN_MSG_INFO));                          // Clear message info structure

	/*CAN Tx data */
	tx_data[0] = 0x11;
	tx_data[1] = 0x22;
	tx_data[2] = 0x33;
	tx_data[3] = 0x44;
	tx_data[4] = 0x55;
	tx_data[5] = 0x66;
	tx_data[6] = 0x77;
	tx_data[7] = 0x88;
	tx_msg_info.dlc = 8U;

	while (1U) {
		/*CAN standard ID : 0x123*/
		tx_msg_info.id = ARM_CAN_STANDARD_ID(0x123U);                            // Set extended ID for transmit message
		status = ptrCAN->MessageSend(tx_obj_idx, &tx_msg_info, tx_data, 8U);     // Send data message with 1 data byte
		if (status != 8U) { Error_Handler(); }
		for (i = 0U; i < 1000000U; i++) { __ASM("nop"); }                        // Wait a little while
	}
	/*Application code will never reach here*/
}
