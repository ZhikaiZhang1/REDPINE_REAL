#ifndef ANTSYS_CORE_H
#define ANTSYS_CORE_H

#include "antsys_types.h"

/**
 * This type contains all of the callbacks that may be made from the vendor implementation
 * into the core ant stack.
 */
typedef struct antsys_core_callbacks_t {

    /**
     * Run the main task for the stack.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_STACK
     */
    void (*run_stack_task)(void);

    /**
     * Notify the stack that a request for random data is complete.
     * This must only be called after ANTSYS_ERR_PENDING has been returned from a
     * call to get_rand.
     *
     * Parameters:
     *      buf - The data parameter that was passed to get_rand.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_STACK or ANTSYS_CONTEXT_ID_RADIO
     */
    void (*on_rng_complete)(uint8_t *data);

    /**
     * Callback for an armed timer.
     *
     * Parameters:
     *      timer - The ID of the timer that triggered this callback.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_RADIO
     */
    void (*on_timer_triggered)(antsys_timer_id_t timer);

    /**
     * Notify the stack about the result of the current radio operation.
     *
     * Parameters:
     *      result - Details about the completion of the radio operation.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_RADIO
     */
    void (*on_radio_op_complete)(const antsys_radio_result_t *result);

    /**
     * Notify the stack that the pending activity has been blocked by another.
     *
     * Parameters:
     *      blocked_info - Information about what caused the block of this activity.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_STACK
     */
    void (*on_activity_blocked)(antsys_blocked_info_t blocked_info);

    /**
     * Notify the stack that the scheduled activity has started. The stack may
     * now access radio hardware until complete_activity or on_activity_aborted
     * is called.
     *
     * Parameters:
     *      info - Information about the activity that is now current.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_RADIO
     */
    void (*on_activity_started)(const antsys_activity_info_t *info);

    /**
     * Notify the stack that the current activity has been aborted.
     * Causes may include:
     *      - A higher priority activity has interrupted the ANT activity.
     *      - The deadline for the activity has expired without complete_activity being called.
     *
     * Parameters:
     *      blocked_info - Information about what caused the block of this activity.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_RADIO
     */
    void (*on_activity_aborted)(antsys_blocked_info_t blocked_info);
    /**
     * Process the ANT Tx Packets
     *
     * Parameters:
     *      data- Address of the host Tx Packet
     *      data_len Length of the host Tx Packet Received.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     */
    antsys_result_t (*ant_tx_pkt_event_handler)(uint8_t *data, uint16_t pkt_len);
    
    /**
     * Updates the modulation index in the Chip
     *
     * Parameters:
     *     None 
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     */
    antsys_result_t (*ant_update_modulation_index)(void);
} antsys_core_callbacks_t;

/**
 * This symbol is exported by the stack library. Since the stack's api version is >= the api
 * version of the vendor implementation it is safe for vendor code to access all known callbacks.
 */
extern antsys_core_callbacks_t antsys_core_callbacks;

#endif
