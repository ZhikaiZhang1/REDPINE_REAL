#ifndef ANTSYS_TYPES_H
#define ANTSYS_TYPES_H

#include <stdbool.h>
#include <stdint.h>

#define ANTSYS_API_VERSION  1

/******************************************************************************/

/**
 * Used to represent time points and durations in the API. Given two points in
 * time, A and B, A is considered to come before B if:
 *      ((B - A) % (1 << 32)) < (1 << 31)
 *
 * Time units are us, and the API is expected to cover the full 32-bit range of
 * values.
 *
 * (TODO: Should we provide example source/library for handling time remapping?)
 */
typedef uint32_t antsys_time_t;

/**
 * Identifies ANT logical links for the purpose of coex management.
 */
typedef uint8_t antsys_link_id_t;

// TODO: Define this in terms of max channels if possible.
/** Maximum value that will be used for a link id. */
#define ANTSYS_NUM_LINK_IDS     ((antsys_link_id_t)16)

/******************************************************************************/

/**
 * Error type. Most API calls return a value of this type. The values are split
 * into 2 ranges:
 *      0x000 - 0xfff Are generic error codes that can be returned from any API.
 *      0x1000 - 0x1fff Are error codes for specific API calls that can be used
 *                      for cases not covered by the generic error codes.
 */
typedef uint32_t antsys_err_t;
typedef uint32_t antsys_result_t;

/* Generic errors */

/** Indicates success */
#define ANTSYS_ERR_NONE             ((antsys_err_t)0x00)
/**
 * Indicates that an API call failed because the parameters did not all
 * contain valid values.
 */
#define ANTSYS_ERR_BAD_PARAMS       ((antsys_err_t)0x01)
/**
 * Indicates that an API call failed because the system was not in a state where
 * it could be handled.
 */
#define ANTSYS_ERR_BAD_STATE        ((antsys_err_t)0x02)
/**
 * Indicates that an API call failed for a reason that doesn't fit in the other
 * error codes.
 * 
 * This is not expected to occur in normal operation. In debug loads this should
 * cause some sort of error report/logging to determine the error cause.
 */
#define ANTSYS_ERR_OTHER            ((antsys_err_t)0xfff)

/* API specific errors */

/**
 * Used by a few apis that indicate the request was acknowledged, but a
 * callback will be used later.
 */
#define ANTSYS_ERR_PENDING          ((antsys_err_t)0x1000)
/**
 * For apis that request things to happen at a specific time, this error code
 * indicates that the request is being made too late to satisfy the requested
 * time.
 */
#define ANTSYS_ERR_LATE             ((antsys_err_t)0x1001)
/**
 * This can be returned by request_activity to indicate that the activity could
 * not be scheduled due to conflicts with other activities.
 */
#define ANTSYS_ERR_SCHED_BLOCKED    ((antsys_err_t)0x1002)

/******************************************************************************/

typedef uint8_t antsys_context_id_t;

/**
 * Context for all API calls from application code to the stack.
 */
#define ANTSYS_CONTEXT_ID_API       ((antsys_context_id_t)0x00)
/**
 * Context where the stack runs the majority of its internal logic.
 */
#define ANTSYS_CONTEXT_ID_STACK     ((antsys_context_id_t)0x01)
/**
 * Context for latency sensitive code related to radio operation.
 */
#define ANTSYS_CONTEXT_ID_RADIO     ((antsys_context_id_t)0x02)

/******************************************************************************/

/**
 * Identifies the different timer types that ANT may use.
 */
typedef uint32_t antsys_timer_id_t;

/**
 * Timer used for triggering a radio configuration call when the scheduler API
 * is unused.
 */
#define ANTSYS_TIMER_ID_PREPARE     ((antsys_timer_id_t)0)

/**
 * Timer used by the stack during scheduler activities and radio operations.
 * When the scheduler API is used this only needs to be accessible when an
 * activity is active. If the scheduler API is unused this must always be
 * supported.
 */
#define ANTSYS_TIMER_ID_ACTIVITY    ((antsys_timer_id_t)1)

/******************************************************************************/

/**
 * Used in radio configuration to indicate which features should be used.
 */
typedef uint32_t antsys_mode_flags_t;

/**
 * When set the radio should be configured to transmit a packet. If clear then 
 * the radio should be configured to receive a packet.
 */
#define ANTSYS_MODE_FLAG_TX                 ((antsys_mode_flags_t)(1u << 0))
/**
 * When this bit is set the values specified for syncword and payload bytes
 * are transmitted/received most significant bit first. When cleared they are
 * transmitted/received least significant bit first. Note that the CRC is
 * always transmitted/received most significant bit first.
 */
#define ANTSYS_MODE_FLAG_MSBIT_FIRST        ((antsys_mode_flags_t)(1u << 1))
#define ANTSYS_MODE_FLAG_VAR_LENGTH         ((antsys_mode_flags_t)(1u << 2))

#define ANTSYS_MODE_FLAG_SYNC_32BIT         ((antsys_mode_flags_t)(1u << 4))

/**
 * When set the radio should be configured to append/check a 16 bit CRC on
 * transmitted/received packets.
 */
#define ANTSYS_MODE_FLAG_CRC_16BIT          ((antsys_mode_flags_t)(1u << 8))
/**
 * When set the radio should be configured to append/check a 24 bit CRC on
 * transmitted/received packets.
 */
#define ANTSYS_MODE_FLAG_CRC_24BIT          ((antsys_mode_flags_t)(1u << 9))

/**
 * When set the radio should be configured to apply whitening/de-whitening to a
 * packet using a 9-bit lfsr.
 */
#define ANTSYS_MODE_FLAG_WHITEN_9BIT        ((antsys_mode_flags_t)(1u << 12))
#define ANTSYS_MODE_FLAG_WHITEN_7BIT        ((antsys_mode_flags_t)(1u << 13))

/**
 * When set the radio should be configured to encrypt/decrypt packets using
 * AES CTR mode, with a full incrementing counter.
 */
#define ANTSYS_MODE_FLAG_ENC_AES_CTR_FULL   ((antsys_mode_flags_t)(1u << 16))
#define ANTSYS_MODE_FLAG_ENC_AES_CTR_16BIT  ((antsys_mode_flags_t)(1u << 17))

/**
 * When set expecting the ANT Stack to update the Modulation index
 *
 */
#define ANTSYS_MODE_FLAG_UPDATE_MOD_INX    ((antsys_mode_flags_t)(1u << 18))

/**
 * When set expecting the ANT Stack to update the protocol traffic over GPIOs.
 *
 */
#define ANTSYS_MODE_FLAG_PROTOCOL_RADIO_SHARE_DEBUG    ((antsys_mode_flags_t)(1u << 19))

/* (1u << 31) must be reserved for indicating extended flags in the case
   That we ever use up the other 31 bits of the flag word. */

/******************************************************************************/

/** Type for indicating the priority of scheduled activities. */
typedef uint8_t antsys_priority_t;

/** Lowest and most common priority used for ant. */
#define ANTSYS_PRIORITY_DEFAULT             ((antsys_priority_t)0x00)
/**
 * Priority used to indicate that an activity should not be blocked in order
 * to maintain a good user experience. It is expected to override data in other
 * protocols that can deal with a small amount of latency (eg. ACL, standard BLE).
 */
#define ANTSYS_PRIORITY_HIGH                ((antsys_priority_t)0x01)
/**
 * Priority used to indicate that blocking the activity may lead to loss of
 * a connection / poor user experience. It is expected that all non-critical
 * traffic from other protocols is blocked in favor of this activity.
 */
#define ANTSYS_PRIORITY_CRITICAL            ((antsys_priority_t)0x02)

/******************************************************************************/

/** Type of traffic being scheduled. Can be used to select coex policy. */
typedef uint8_t antsys_traffic_type_t;

/** Activity used for finding another device. Short period between requests. */
#define ANTSYS_TRAFFIC_TYPE_SEARCH          ((antsys_traffic_type_t)0x00)
/** Activity for standard transfers. Typically occurs on a longer period. */
#define ANTSYS_TRAFFIC_TYPE_BROADCAST       ((antsys_traffic_type_t)0x01)
/** Activity used for high throughput data transfer. */
#define ANTSYS_TRAFFIC_TYPE_BURST           ((antsys_traffic_type_t)0x02)

/******************************************************************************/

/**
 * Indicates the result of a single radio operation.
 */
typedef uint32_t antsys_radio_status_t;

/** The radio operation completed successfully. */
#define ANTSYS_RADIO_STATUS_OK              ((antsys_radio_status_t)0x00)

/** The radio operation was aborted. */
#define ANTSYS_RADIO_STATUS_ABORTED         ((antsys_radio_status_t)0x01)

/**
 * The requested start time was too close / in the past so the radio operation
 * did not start.
 */
#define ANTSYS_RADIO_STATUS_TOO_LATE        ((antsys_radio_status_t)0x02)

/**
 * For RX operations this indicates that the sync word correlator never
 * triggered.
 */
#define ANTSYS_RADIO_STATUS_NO_SYNC         ((antsys_radio_status_t)0x03)

/**
 * Packet reception started but did not complete because the radio was disabled
 * for some reason.
 */
#define ANTSYS_RADIO_STATUS_INCOMPLETE      ((antsys_radio_status_t)0x04)

/**
 * For RX operations this indicates that a packet was received, but the
 * calculated CRC did not match the received CRC.
 */
#define ANTSYS_RADIO_STATUS_BAD_CRC         ((antsys_radio_status_t)0x05)

//#define ANTSYS_RADIO_STATUS_BAD_LEN         ((antsys_radio_status_t)0x05)

/**
 * This is a base value for vendor-specific errors. This can be used to indicate
 * hw reported errors (eg. some PLL failed to stabilize.)
 */
#define ANTSYS_RADIO_STATUS_VND_ERR_START   ((antsys_radio_status_t)0x80000000)

/******************************************************************************/

/**
 * Used to identify why an activity may have been blocked.
 */
typedef uint8_t antsys_blocked_reason_t;

/** Default value. Can be used for all blocked indications. */
#define ANTSYS_BLOCKED_REASON_UNKNOWN       ((antsys_blocked_reason_t)0x00)
/** Blocked by another activity with a higher coexistence priority */
#define ANTSYS_BLOCKED_REASON_PRIORITY      ((antsys_blocked_reason_t)0x01)
/** Blocked because the stack requested the activity to be cancelled. */
#define ANTSYS_BLOCKED_REASON_CANCELLED     ((antsys_blocked_reason_t)0x02)
/** Blocked because the request was made too late. */
#define ANTSYS_BLOCKED_REASON_LATE          ((antsys_blocked_reason_t)0x03)
/** Forcibly aborted because activity deadline was exceeded. */
#define ANTSYS_BLOCKED_REASON_DEADLINE      ((antsys_blocked_reason_t)0x04)

/******************************************************************************/

/**
 * Discriminator for the various coex-related events.
 */
typedef uint8_t antsys_coex_event_type_t;

#define ANTSYS_COEX_EVENT_CHANNEL_OPEN              ((antsys_coex_event_type_t)0x00)
#define ANTSYS_COEX_EVENT_CHANNEL_CLOSED            ((antsys_coex_event_type_t)0x01)
#define ANTSYS_COEX_EVENT_SEARCH_START              ((antsys_coex_event_type_t)0x02)
#define ANTSYS_COEX_EVENT_SEARCH_STOP               ((antsys_coex_event_type_t)0x03)
#define ANTSYS_COEX_EVENT_BURST_START               ((antsys_coex_event_type_t)0x04)
#define ANTSYS_COEX_EVENT_BURST_STOP                ((antsys_coex_event_type_t)0x05)
#define ANTSYS_COEX_EVENT_CRITICAL_SEARCH_START     ((antsys_coex_event_type_t)0x06)
#define ANTSYS_COEX_EVENT_CRITICAL_SEARCH_STOP      ((antsys_coex_event_type_t)0x07)
#define ANTSYS_COEX_EVENT_CHANNEL_UPDATE            ((antsys_coex_event_type_t)0x08)

/******************************************************************************/

/**
 * Different sub-modes for test mode.
 */
typedef uint8_t antsys_atm_mode_t;

/** Transmit an un-modulated carrier wave. */
#define ANTSYS_ATM_MODE_CARRIER         ((antsys_atm_mode_t)0x00)
/**
 * Transmit a modulated carrier. Stack will specify data and sync word
 * to modulate with.
 */
#define ANTSYS_ATM_MODE_MODULATED       ((antsys_atm_mode_t)0x01)
/** Transmit a modulated carrier using a prng source for the data. */
#define ANTSYS_ATM_MODE_PRNG            ((antsys_atm_mode_t)0x02)
/** Keep radio in a continuous receive mode. */
#define ANTSYS_ATM_MODE_RX              ((antsys_atm_mode_t)0x03)

/******************************************************************************/

/**
 * Describe the requested configuration of the radio for a single operation.
 */
typedef struct antsys_radio_config_t {
    /** Flags for turning specific hardware features on/off. */
    antsys_mode_flags_t mode_flags;

    /**
     * When the radio operation should start. For TX operations this is when
     * the first bit of the sync word should be transmitted at the antenna, and
     * for RX operations it is the earliest time that the first bit of the sync
     * word is expected to be received. The vendor implementation should take
     * into account any lead time needed to start up the radio and send/receive
     * pre-amble.
     */
    antsys_time_t start_time;
    /**
     * The latest point in time where the sync word correlation could be triggered.
     * If possible the radio should be automatically disabled at this point for
     * RX operations if no sync has occurred.
     */
    antsys_time_t sync_timeout;
    /**
     * The latest point in time that a bit should be transferred. If possible the
     * radio should be set to automatically disable at this point in time.
     */
    antsys_time_t timeout;

    /**
     * The sync word to send over the air or correlate with. The endianess is
     * specified using the MSBIT_FIRST mode flag. For a 16 bit sync word the
     * least significant 16 bits are used as the sync word (eg. 0x00001234).
     */
    uint32_t sync_word;
    /**
     * Pointer to a the data buffer. For TX operations this is where the data
     * to be sent is sourced from. For RX operations this is where the data
     * received should be written to. The endianess of the data here is
     * specified using the MSBIT_FIRST mode flag.
     */
    uint8_t* data;
    /**
     * Length of the data buffer. For TX operations this indicates the number of
     * bytes to transmit. For RX operations it is the number of bytes to receive.
     * This will not include the length of the preamble, syncword, or CRC. (And
     * those values should not be in tbe buffer).
     */
    uint16_t data_len;
    /**
     * RF channel that the radio should be tuned to. This represents an offset in
     * MHz from 2400 MHz.
     */
    uint8_t rf_channel;
    /**
     * This is the transmit power to use for tx operations. It is signed value
     * in units of dBm. The following tx_power values must all result in discrete
     * effective tx power levels: +4, 0, -5, -10, -20.
     */
    int8_t tx_power;

    /**
     * When the CRC hardware is used this is the polynomial for the CRC.
     *
     * The polynomial will be specified in the least significant bits, and the
     * top bit of the polynomial is implicitly 1.
     *
     * For example to specify a 16-bit crc-ccitt using the polynomial
     *  x^16 + x^12 + x^5 + 1
     * the value supplied will be 0x00001021
     */
    uint32_t crc_poly;
    /**
     * The initial value of the CRC LFSR state (galios form).
     */
    uint32_t crc_init;

    /**
     * When the whitening hardware is used this is the polynomial to be used.
     * This follows the same form as for the crc_poly field.
     */
    uint32_t whiten_poly;
    /**
     * When the whitening hardware is used this is the initial value to use for
     * the lfsr internal state. This is targeted at a galios form lfsr, and will
     * need some transformation for a fibonacci form lfsr.
     */
    uint32_t whiten_init;

    /**
     * When the encryption hardware is used this is the AES 128-bit key
     * to use. The MSByte is enc_key[0].
     */
    const uint8_t* enc_key;
    /**
     * When the encryption hardware is used this is the 128-bit initial value
     * to use for the counter. The MSByte is enc_ctr[0] (ie. increment starts
     * at enc_ctr[15]).
     */
    const uint8_t* enc_ctr;
    /**
     * The offset in bytes into the payload to start the application of encryption.
     */
    uint16_t enc_offset;
} antsys_radio_config_t;

/**
 * This type contains all of the information about the outcome of a single
 * radio operation.
 */
typedef struct antsys_radio_result_t {
    /**
     * Status indicating whether the radio operation fully completed or not.
     */
    antsys_radio_status_t status;
    /**
     * For RX activities this is the exact time the correlator triggered.
     */
    antsys_time_t sync_time;
    /**
     * For RX activities this is the signal strength measured while receiving
     * the packet. It is in (signed) units of dBm.
     */
    int8_t rssi;
} antsys_radio_result_t;

/**
 * This type contains all of the parameters while scheduling that are related
 * to co-existance with other protocols.
 */
typedef struct antsys_coex_params_t {
    /**
     * The logical link to associate the schedule request with.
     */
    antsys_link_id_t link_id;
    /**
     * The priority of the request.
     */
    antsys_priority_t priority;
    /**
     * The type of traffic the activity will be used for.
     */
    antsys_traffic_type_t traffic_type;
} antsys_coex_params_t;

/**
 * Type that contains all of the parameters used for scheduling.
 */
typedef struct antsys_sched_params_t {
    /**
     * The earliest time the activity can start.
     */
    antsys_time_t min_start;
    /**
     * The latest time the activity can start.
     */
    antsys_time_t max_start;
    /**
     * The minimum amount of time that must be allocated to the activity.
     */
    antsys_time_t min_duration;
    /**
     * The maximum amount of time that the activity is requesting.
     */
    antsys_time_t max_duration;
    /**
     * Regardless of the start time and duration, the activity must end before
     * this point in time.
     */
    antsys_time_t deadline;

    /**
     * All of the parameters related to co-existance.
     */
    const antsys_coex_params_t *coex;
} antsys_sched_params_t;

/**
 * Information about the activity once it's started.
 */
typedef struct antsys_activity_info_t {
    /**
     * The earliest value for start_time that can be used for the first radio
     * configuration in this activity.
     */
    antsys_time_t earliest_radio;
    /**
     * The scheduled end time of this activity. The ANT stack may end the
     * activity before this point, but must not end it after without first
     * extending through an update_activity call.
     */
    antsys_time_t deadline;
} antsys_activity_info_t;

/**
 * Information that can be provided as feedback to the ANT stack about a blocked
 * activity. The stack may use this information to optimize its co-existance
 * behaviour.
 */
typedef struct antsys_blocked_info_t {
    /**
     * The reason that the activity was blocked. If the vendor implementation
     * is unable to determine the cause it may be set to ANTSYS_BLOCKED_REASON_UNKNOWN.
     * This is only used for debugging coex issues.
     */
    antsys_blocked_reason_t reason;
    /**
     * The start time of the activity that blocked ANT.
     * If the value cannot be determined it should be set to the END time
     * of the ANT activity that was blocked.
     */
    antsys_time_t block_start;
    /**
     * The end time of the activity that blocked ANT.
     * If this cannot be determined is should be set to the START time of the
     * ANT activity that was blocked.
     */
    antsys_time_t block_end;
} antsys_blocked_info_t;

/**
 * Parameters that the stack may request to adjust with an update_activity call.
 * All fields have the same meaning as they do in antsys_sched_params_t.
 */
typedef struct antsys_activity_update_params_t {
    antsys_priority_t priority;
    antsys_traffic_type_t traffic_type;
    antsys_time_t deadline;
} antsys_activity_update_params_t;

/**
 * Information that is generated by the ant stack as part of a coex event.
 */
typedef struct antsys_coex_event_t {
    /**
     * The type of event this represents.
     */
    //antsys_coex_evt_type_t evt_type;
    /**
     * The logical ANT link this event is associated with.
     */
    antsys_link_id_t link_id;
    /**
     * The current period of the specified channel. Valid for the following events.
     * 
     * ANTSYS_COEX_EVENT_CHANNEL_OPEN
     * ANTSYS_COEX_EVENT_CHANNEL_UPDATE
     */
    antsys_time_t period;
    /**
     * Time of the specified event. Valid for the following events.
     * 
     * ANTSYS_COEX_EVENT_CHANNEL_OPEN
     * ANTSYS_COEX_EVENT_SEARCH_START
     * ANTSYS_COEX_EVENT_BURST_START
     * ANTSYS_COEX_EVENT_CRITICAL_SEARCH_START
     * ANTSYS_COEX_EVENT_CHANNEL_UPDATE
     * 
     * For ANTSYS_COEX_EVENT_CHANNEL_UPDATE it is the exact anchor time for the
     * event.
     */
    antsys_time_t start_time;
    /**
     * Indicates whether the burst succeeded or failed. This is only valid for
     * the ANTSYS_COEX_EVENT_BURST_STOP event.
     */
    bool burst_passed;
    /**
     * Indicates that burst mode is being initiated by this device and not the
     * remote device. Is only valid for the ANTSYS_COEX_EVENT_BURST_START event.
     */
    bool is_burst_master;
} antsys_coex_event_t;

/**
 * Configuration parameters for direct test mode.
 */
typedef struct antsys_atm_config_t {
    /**
     * Specifies the specific test mode to use. The remaining parameters may
     * only be used in certain test modes.
     */
    antsys_atm_mode_t dtm_mode;
   
    /**
     * Used to specify sync word length and endianess of sync word + data.
     */
    antsys_mode_flags_t mode_flags;
    /**
     * RF frequency to use. Matches the definition in the radio config struct.
     */
    uint8_t rf_channel;
    /**
     * Transmit power level to use. Matches definition in the radio config struct.
     */
    int8_t tx_power;
    /**
     * Sync word to use. Matches definition in the radio config struct.
     */
    uint32_t sync_word;
    /**
     * Source data to use for test modes that require it.
     */
    const uint8_t *data;
    /**
     * Length of source data for test modes that require it.
     */
    uint16_t data_len;
} antsys_atm_config_t;

/**
 * Extra information about the vendor implementation used by the ant stack.
 */
typedef struct antsys_vnd_capabilities_t {
    /**
     * Integer version number of the API the vendor implementation was built
     * against. Should be set using the ANTSYS_API_VERSION define.
     */
    uint32_t api_version;
    /**
     * Version string for the vendor implementation.
     * TODO: add this as one of the version strings that can be accessed
     *       through the ANT api?
     */
    const char *version_str;

    // TODO: work out exactly what timing parameters are needed.
    antsys_time_t scheduler_lead_time;
    antsys_time_t radio_config_time;
    antsys_time_t radio_turnaround_time;
    antsys_time_t scheduler_tail_time;
    antsys_time_t scheduler_turnaround_time;

    /**
     * Indicates which hw features are supported by the implementation.
     */
    antsys_mode_flags_t supported_hw;

    // TODO: combine these into flags?
    /**
     * If set the stack will use coarser grained activities. Otherwise a
     * separate activity will be requested for each radio op.
     */
    bool multiple_ops_per_activity;
    /**
     * Indicates whether the vendor implementation supports scheduling with
     * flexible start times/durations. If not set the the following will
     * always be true in schedule requests.
     * 
     *      min_start = max_start
     *      min_duration = max_duration
     *      min_start + min_duration = deadline
     */
    bool flexible_activities;
} antsys_vnd_capabilities_t;

#endif
