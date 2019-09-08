#ifndef ANTSYS_VND_H
#define ANTSYS_VND_H

#include "antsys_types.h"

typedef struct antsys_vnd_ops_t {

    /**
     * Initialize the system interface. This will be called before any
     * other vendor ops.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     * Contexts:
     *      ANTSYS_CONTEXT_ID_API
     */
    antsys_result_t (*init_interface)(void);

    /**
     * Release any resources used by the system interface.
     * // Tentative: stack cleanup may be sync or async =, not sure yet.
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     * Contexts:
     *      ANTSYS_CONTEXT_ID_STACK
     */
    antsys_result_t (*cleanup_interface)(void);

    /**
     * Set the stack task to pending. The vendor implementation should call
     * run_stack_task as soon as possible.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     * Contexts:
     *      any
     */
    antsys_result_t (*pend_stack_task)(void);

    /**
     * Disables callbacks from being made at a particular context level until
     * the corresponding enable_interrupts() call is made.
     *
     * Callbacks that would have occurred during this period must still be made,
     * just delayed until after the interrupts are enabled.
     *
     * Contexts:
     *      any - The context parameter will be higher priority than the caller
     *              context.
     */
    antsys_result_t (*disable_interrupts)(antsys_context_id_t context);

    /**
     * Re-enable callbacks at a particular context level.
     * 
     * Contexts:
     *      any - The context parameter will be higher priority than the caller
     *              context.
     */
    antsys_result_t (*enable_interrupts)(antsys_context_id_t context);

    /**
     * Request cryptographically secure random number generation.
     *
     * Parameters:
     *      data - Buffer to store generate numbers in.
     *      len - Requested amount of data to retrieve.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE: The full amount of requested data was generated.
     *      ANTSYS_ERR_PENDING: The request is ongoing. A call to
     *              on_rng_complete will be made when it is completed. The
     *              buffer pointed to by data will not be accessed by the stack
     *              until the request is completed.
     *
     * Contexts:
     *      ANTSYS_CONTEXT_ID_STACK
     */
    antsys_result_t (*get_rand)(uint8_t *data, uint32_t len);



    /**
     * Capture the current value of the system timer. The system timer must be
     * a monotonic source.
     *
     * Returns:
     *      The current value of the system timer.
     *
     * Contexts:
     *      any
     */
    antsys_time_t (*get_sys_time)(void);

    /**
     * Arm the timer callback to be made at a specific time.
     * 
     * Parameters:
     *      timer - The timer that should be scheduled.
     *      trigger_time - The system time that the callback should be made at.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE: The timer is set to trigger a callback at the
     *              specified time.
     *      ANTSYS_ERR_LATE: The timer was not armed because the requested time
     *              is too close or is in the past.
     *
     * Contexts:
     *      ANTSYS_CONTEXT_ID_STACK - For ANTSYS_TIMER_ID_PREPARE
     *      ANTSYS_CONTEXT_ID_RADIO - For ANTSYS_TIMER_ID_ACTIVITY
     */
    antsys_result_t (*arm_timer)(antsys_timer_id_t timer, antsys_time_t trigger_time);

    /**
     * Cancel the scheduled timer callback if possible.
     * 
     * Parameters:
     *      timer - The timer to cancel.
     * 
     * Expected Returns:
     *      ANTSYS_ERR_NONE: There was a scheduled event that was successfully
     *              cancelled.
     *      ANTSYS_ERR_LATE: Either there was no event scheduled, or it was too
     *              late to successfully cancel the event.
     *
     * Contexts:
     *      ANTSYS_CONTEXT_ID_STACK
     */
    antsys_result_t (*cancel_timer)(antsys_timer_id_t timer);



    /**
     * Start on operation on the radio hardware. If this call is successfull the
     * stack will not access the data buffer or encryption buffers specified in
     * the configuration until the operation completes or is aborted.
     *
     * Parameters:
     *      config - Configuration to be used for the radio operation.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE: The request was accepted. There will be a call to
     *              on_radio_op_complete when the operation is done.
     *      ANTSYS_ERR_LATE: The request was not accepted because the requested
     *              time is too close or is in the past.
     * 
     * Contexts:
     *      ANTSYS_CONTEXT_ID_RADIO
     */
    antsys_result_t (*config_radio)(const antsys_radio_config_t *config);

    /**
     * Abort an ongoing operation using the radio hardware.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE: The operation is succesfully aborted, there will be
     *              no call to on_radio_op_complete.
     *      ANTSYS_ERR_BAD_STATE: There is no operation to abort. This is
     *              treated identically to ANTSYS_ERR_PENDING by the stack and
     *              is only meant for clarity.
     *      ANTSYS_ERR_PENDING: Indicates that there was no ongoing radio
     *              operation or that the result of the cancel will be indicated
     *              in the on_radio_complete callback. The status may be
     *              ANTSYS_RADIO_STATUS_ABORTED if the abort took effect, or it
     *              may be any of the other codes (abort has no effect in this
     *              case).
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_STACK
     *      ANTSYS_CONTEXT_ID_RADIO
     */
    antsys_result_t (*abort_radio_op)(void);

    /**
     * Attempt to schedule an activity.
     *
     * An activity is a period of time where ANT has access to radio and other
     * hardware that may be shared with other protocol stacks.
     *
     * Parameters:
     *      params - All information related to the activity request.
     *      block_info - If the request is blocked this struct should be filled
     *                   out with information about the reason.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE - The scheduler has started processing the request.
     *              A call will later be made to on_activity_started or
     *              on_activity_blocked.
     *      ANTSYS_ERR_SCHED_BLOCKED - The scheduler has determined that the
     *              activity is blocked from running. There will be no further
     *              callbacks related to this activity. blocked_hint may be set
     *              to the end time of a conflicting activity to give the stack
     *              a hint about when it can schedule another activity of the
     *              same priority. See the ANT System API Specification document
     *              for details.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_STACK
     */
    antsys_result_t (*request_activity)(
            const antsys_sched_params_t *params,
            antsys_blocked_info_t *blocked_info);

    /**
     * Make a modification to the current request for an activity. This will
     * only be used on simple requests where the following holds true:
     *      min_start = max_start
     *      min_duration = max_duration
     *      min_start + min_duration = deadline.
     *
     * The modification will only specify a new deadline, but the min_duration
     * and max_duration parameters of the request should be updated accordingly.
     *
     * Parameters:
     *      params - Overrides for a subset of the values for a new request.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE - The modifications have been made. A call will
     *              still later be made to on_activity_started or
     *              on_activity_blocked.
     *      ANTSYS_ERR_SCHED_BLOCKED - The modifications could not be accepted.
     *              The original parameters for the activity are still in
     *              effect.
     *      ANTSYS_ERR_BAD_STATE - There was no pending activity to modify.
     *              This could be because the activity has already
     *              started/completed.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_STACK
     */
    antsys_result_t (*modify_request)(const antsys_activity_update_params_t *params);

    /**
     * Request a modification of the currently running activity's parameters.
     *
     * Parameters:
     *      params - The new values being requested for the activity. If
     *               ANTSYS_ERR_NONE is returned then the values in this structure
     *               must be updated to what is actually applied (eg. If a
     *               deadline extension of 2ms was requested, but an actual
     *               deadline extension of 1ms was granted the value must be
     *               adjusted to reflect.)
     * 
     *               It is acceptable to return ANTSYS_ERR_NONE and set the values
     *               of params to the old values for the activity if no adjustments
     *               are made.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE: The values in the params struct have been updated to
     *              reflect possibly new values for the activity.
     *      ANTSYS_ERR_SCHED_BLOCKED: The request was rejected, the original values
     *              for the activity are still in effect.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_RADIO
     */
    antsys_result_t (*update_activity)(antsys_activity_update_params_t *params);

    /**
     * Indicate the completion of the currently running activity.
     *
     * After this call completes the stack no longer has exclusive access to the radio
     * hardware.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_RADIO
     */
    antsys_result_t (*complete_activity)(void);

    /**
     * Cancel the currently requested activity.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE: There was an activity request that was succesfully
     *              cancelled. There will be no further callbacks related to the
     *              activity.
     *      ANTSYS_ERR_BADSTATE: There was no activity to cancel. The stack
     *              treats this the same as ANTSYS_ERR_PENDING, it is here only
     *              for API clarity.
     *      ANTSYS_ERR_PENDING: Indicates either that there was no activity to
     *              cancel, or that the result of the cancel will be indicated
     *              with a later callback. A successfull cancel in this case
     *              results in a call to on_activity_blocked, otherwise the
     *              cancel request has no effect on the activity.
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_STACK
     */
    antsys_result_t (*cancel_activity)(void);

    /**
     * Notify the vendor implementation of a coexistence event.
     *
     * Parameters:
     *      event - Structure describing the details of the coex event.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_STACK
     *      ANTSYS_CONTEXT_ID_RADIO - TODO: can we dispatch at stack level?
     */
    antsys_result_t (*on_coex_event)(const antsys_coex_event_t *event);



    /**
     * Begin an ANT-test-mode operation.
     *
     * Parameters:
     *      config - Details about the requested test mode.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_API
     */
    antsys_result_t (*start_atm)(const antsys_atm_config_t *config);

    /**
     * Halt ANT-test-mode operation.
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     * Context:
     *      ANTSYS_CONTEXT_ID_API
     */
    antsys_result_t (*stop_dtm)(void);
    
    
    /**
     * Send the ANT Rx Packets to the Host
     *
     * Parameters:
     *      data- Address of the host Rx Packet
     *      data_len Length of the host Rx Packet to host
     *
     * Expected Returns:
     *      ANTSYS_ERR_NONE
     *
     */
    antsys_result_t (*send_ant_rx_pkt_to_host)(uint8_t *data, uint16_t pkt_len);
    
    /**
     * Manual encryption provision for the ant stack.
     *
     * Parameters:
     *      key - 16 byte encryption ket
     *      ctr_iv - initization vector
     *      buffer - buffer of the packet to be encrypted.
     *      buffer_len - length of the buffer
     *      enc_offset Point from each encryption to be started.
     *
     * Expected Returns:
     *      NONE
     *
     */
    void (*ant_encrypt)(uint8_t *key, uint8_t *ctr_iv, uint8_t *buffer, uint8_t buffer_len, uint16_t enc_offset);

}antsys_vnd_ops_t;

/**
 * Function used by the ant stack to retrieve all API pointers from the
 * vendor implementation.
 * 
 * Parameters:
 *      ops: Pointer to the structure that should be filled out with all
 *           function pointers. Some functions in the structure may be left as
 *           null pointers to indicate they are not used, others are required
 *           to be set. For the details of what is required/optional please
 *           refer to the ANT System API Specification.
 *
 * Context:
 *      ANTSYS_CONTEXT_ID_API
 */
void antsys_vnd_get_ops(antsys_vnd_ops_t *ops);

/**
 * Function used by the ant stack to query the capabilities of the vendor
 * implementation.
 * 
 * Parameters:
 *      caps: Pointer to the structure that should be filled in with all vendor
 *            capabilities.
 *
 * Context:
 *      ANTSYS_CONTEXT_ID_API
 */
void antsys_vnd_get_caps(antsys_vnd_capabilities_t *caps);

#endif
