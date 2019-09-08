/**
 Copyright 2018 by Garmin Ltd. or its subsidiaries.
 All rights reserved.
 **/

#include "../include/antsys_types.h"
#include "../include/antsys_core.h"
#include "../include/antsys_vnd.h"

extern uint32_t __bss_start;
extern uint32_t _end;

antsys_vnd_ops_t *antsys_vnd_ops_p;
void run_stack_task(void);
void on_radio_op_complete(const antsys_radio_result_t *result);
void on_timer_triggered(antsys_timer_id_t timer);
antsys_result_t ant_tx_pkt_event_handler(uint8_t *data, uint16_t pkt_len);
void run_simple_test(uint8_t flags);
antsys_result_t ant_update_modulation_index(void);
void on_activity_started(const antsys_activity_info_t *info);
antsys_time_t ant_get_next_sleep_duration();
void on_activity_blocked(const antsys_blocked_info_t blocked_info);
void on_activity_aborted(const antsys_blocked_info_t blocked_info);
antsys_result_t request_to_change_ta_freq(void);

bool schedule_simple_broadcast(void);
bool schedule_simple_search(void);
bool schedule_simple_slave(void);
bool schedule_simple_burst(void);

antsys_core_callbacks_t antsys_core_callbacks_g __attribute__((section(".ant_data")))=
{
   .run_stack_task  = run_stack_task,
   .on_timer_triggered = on_timer_triggered,
   .on_radio_op_complete = on_radio_op_complete,
   .ant_tx_pkt_event_handler = ant_tx_pkt_event_handler,
   .ant_update_modulation_index = ant_update_modulation_index,
   .on_activity_started = on_activity_started,
   .on_activity_blocked = on_activity_blocked,
   .on_activity_aborted = on_activity_aborted,
};

//#define TEST_TYPE_BURST
//#define TEST_TYPE_MASTER_CHANNEL
#define TEST_TYPE_SEARCH
//#define TEST_TYPE_COMBINED
//#define ANT_DEBUG_GPIOS

#define REDPINE_RF_MOD_INX_0P32                          0x0F01A
//#define REDPINE_RF_MOD_INX_0P32                          0xA301A
#define REDPINE_RF_MOD_INX_0P30                          0xA201A

#define RSI_VND_STRUCT_ADDR                              (0x20594)
#define START_ATM                                        1
#define ATM_GET_VERSION                                  2
#define ANT_GET_SCHEDULING_STATS                         3
#define ANT_GET_ACTIVITY_STATS                           4
#define ANT_VERSION                                      ANT1.0

#define GPIO_BASE_ADDR                                   0x40200000
#define GPIO_REG(IDX)                                    *(volatile uint16_t *)(GPIO_BASE_ADDR + ((IDX) * 2))
#define GPIO_OUTPUT_HIGH                                 (1 << 4)
#define GPIO_OUTPUT_LOW                                  0

#ifdef ANT_DEBUG_GPIOS
#define GPIO_TX                                          12
#define GPIO_RX                                          49
#define GPIO_TASK_STACK                                  51
#define GPIO_BLOCKED                                     50
#endif

#define TASS_CLK_PWR_CTRL_BASE_ADDR   0x41400000
#define RF_PROG_SPI_IO_BASE_ADDR                         0x41144000
#define INT_RF_WR(_ADDR_,_DATA_)                         (*(volatile uint32_t *)(RF_PROG_SPI_IO_BASE_ADDR + ((_ADDR_) << 2)) = (_DATA_))
#define INTF_RF_WR_1(_ADDR_,_DATA_)                      (*(volatile uint32_t *)(TASS_CLK_PWR_CTRL_BASE_ADDR + (_ADDR_)) |= (_DATA_))

#define ANT_TEST_PREPARE_LONG                            (1500)         // 1.5 ms
#define ANT_TEST_PREPARE_SHORT                           (1000)         // 1 ms
#define ANT_TEST_PACKET_TIMEOUT                          (144 + 50)     // 18-byte OTA + margin
#define ANT_TEST_RADIO_SYNC_WORD                         (0x00005B25)
#define ANT_TEST_RADIO_TX_POWER                          (30)           // 4 dBm
#define ANT_TEST_CRC_POLY                                (0x00001021)
#define ANT_TEST_CRC_INIT                                (0x0000FFFF)
#define ANT_TEST_PAYLOAD_LEN                             (13)
#define ANT_TEST_RF_FREQ                                 (36)
#define ANT_TEST_SYNC_TIMEOUT                            (1200)
#define ANT_TEST_TURNAROUND_DELAY                        (200)

#define ANT_TEST_DEV_ID                                  (33)
#define ANT_TEST_DEV_TYPE                                (1)
#define ANT_TEST_TRANS_TYPE                              (16)

#define ANT_TEST_BUFFER_COMPLETE_IDX                     (ANT_TEST_PAYLOAD_LEN - 1)
#define ANT_TEST_BUFFER_ABORT_IDX                        (ANT_TEST_PAYLOAD_LEN - 2)
#define ANT_TEST_BUFFER_ACT_BLOCKED_IDX                  (ANT_TEST_PAYLOAD_LEN - 3)
#define ANT_TEST_BUFFER_SCH_BLOCKED_IDX                  (ANT_TEST_PAYLOAD_LEN - 4)

#define MAX_TIME                                         (0xFFFFFFFF)
#define HALF_MAX_TIME                                    (MAX_TIME / 2)

#define ANT_CHANNEL_SPACING                              (10000)
#define  RSI_ANT_SCHED_STATS                             0x1530
#define  RSI_ANT_ACTIVITY_STATS                          0x1531

#if defined(TEST_TYPE_BURST)
   #undef ANT_TEST_SYNC_TIMEOUT
   #undef ANT_TEST_PREPARE_LONG
   #undef ANT_TEST_PREPARE_SHORT
   #define ANT_TEST_SYNC_TIMEOUT                         (800)
   #define ANT_TEST_PREPARE_LONG                         (300)
   #define ANT_TEST_PREPARE_SHORT                        (150)

   #define ANT_TEST_INITIAL_TX_TO_RX                     (2100)
   #define ANT_TEST_MID_RX_TO_TX                         (1680)
   #define ANT_TEST_MID_TX_TO_RX                         (1420)
   #define ANT_TEST_END_RX_TO_RX                         (3820)

   #define TRANSFER_MODE                                 (0x80)
   #define TRANSFER_REQUEST                              (0x40)
   #define TRANSFER_COMPLETE                             (0x20)
   #define TRANSFER_SEQUENCE                             (0x10)
   #define BURST_20K                                     (0x02)

   #define ANT_TEST_MAX_BURST_PACKETS                    (4)

   #define ANT_TEST_NUM_CHANNELS                         (1)                    

   uint8_t test_burst_interval_idx                    =  0;
   uint8_t test_burst_packet_idx                      =  0;
   uint8_t test_burst_idx                             =  0;
   uint32_t channel_period[ANT_TEST_NUM_CHANNELS]     =  {
                                                         250000,  // 4 Hz
                                                         };
#elif defined(TEST_TYPE_MASTER_CHANNEL)
   #undef ANT_TEST_SYNC_TIMEOUT
   #undef ANT_TEST_PREPARE_LONG
   #undef ANT_TEST_PREPARE_SHORT
   #define ANT_TEST_SYNC_TIMEOUT                         (800)
   #define ANT_TEST_PREPARE_LONG                         (300)
   #define ANT_TEST_PREPARE_SHORT                        (150)
   #define ANT_TEST_TX_TO_RX                             (2100)
   #define ANT_TEST_NUM_CHANNELS                         (1)
   uint32_t channel_period[ANT_TEST_NUM_CHANNELS]     =  {
                                                         250000, //10000, //25000,  // 4 Hz
                                                         };
#elif defined(TEST_TYPE_SEARCH)
   #undef ANT_TEST_SYNC_TIMEOUT
   #undef ANT_TEST_PREPARE_LONG
   #undef ANT_TEST_PREPARE_SHORT
   #define ANT_TEST_SYNC_TIMEOUT                         (1200)
   #define ANT_TEST_PREPARE_LONG                         (300)
   #define ANT_TEST_PREPARE_SHORT                        (150)
   #define ANT_TEST_NUM_CHANNELS                         (1)
   uint32_t channel_period[ANT_TEST_NUM_CHANNELS]     =  {
                                                         2960, //10000, //2960,//37500, //2960,    // 97/32768
							 //20000,
                                                         };
#elif defined(TEST_TYPE_COMBINED)
   #define ANT_TEST_NUM_CHANNELS                         (6)

   uint32_t channel_period[ANT_TEST_NUM_CHANNELS]     =  {
                                                         249694,  // 8182
                                                         247253,  // 8102
                                                         247742,  // 8118
                                                         124634,  // 4084
                                                         124634,  // 4084
                                                         250000,  // 8192
                                                         };
#else
   #error "Test undefined"
#endif

antsys_radio_config_t   radio_config;
antsys_sched_params_t   sched_params;
antsys_coex_params_t    coex_params;
antsys_blocked_info_t   blocked_info;
antsys_time_t           active_channel_time;
antsys_coex_event_t     coex_evt;

uint8_t test_protocol_payload[ANT_TEST_PAYLOAD_LEN]   = {0};

bool activity_is_tx                                   = true;
uint32_t channel_time_segment_offset                  = 0;
uint32_t channel_time[ANT_TEST_NUM_CHANNELS]          = {0};

bool run_find_next                                    = true;

uint8_t active_channel                                = 0;
uint32_t active_channel_period                        = 0;
uint32_t active_channel_interval                      = 0;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Scheduling stats counters
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint16_t activities_blocked                           = 0;
uint16_t activities_aborted                           = 0;
uint16_t schedules_blocked                            = 0;
uint16_t radio_ops_complete                           = 0;
uint16_t schedules_attempted                          = 0;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Radio activity stats counters
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint16_t activity_status_ok                           = 0;
uint16_t activity_status_aborted                      = 0;
uint16_t activity_status_too_late                     = 0;
uint16_t activity_status_no_sync                      = 0;
uint16_t activity_status_incomplete                   = 0;
uint16_t activity_status_bad_crc                      = 0;
uint16_t activity_status_vnd_error_start              = 0;
uint16_t activity_status_skipped                      = 0;

uint8_t current_priority = ANTSYS_PRIORITY_DEFAULT;

void memset(uint8_t *dst, uint8_t c, uint32_t n)
{
   if (n)
   {
      uint8_t *d = dst;
      do
      {
         *d++ = c;
      } while (--n);
   }
}

uint8_t find_next_channel(void)
{
   uint8_t i_channel = 0;
   uint32_t earliest_time = antsys_vnd_ops_p->get_sys_time() + ANT_TEST_PREPARE_LONG + ANT_TEST_TURNAROUND_DELAY;

   // Catch up first
   for (i_channel = 0; i_channel < ANT_TEST_NUM_CHANNELS; i_channel++)
   {
      while (
            (channel_time[i_channel] <= earliest_time) &&
            ((earliest_time - channel_time[i_channel]) < HALF_MAX_TIME)
            )
      {
         channel_time[i_channel] += channel_period[i_channel];
         activity_status_skipped++;
      }
   }

   // Find next channel to run
   uint8_t next_channel = 0;
   for (i_channel = 1; i_channel < ANT_TEST_NUM_CHANNELS; i_channel++)
   {
      if ((channel_time[i_channel] - earliest_time) < (channel_time[next_channel] - earliest_time))
      {
         // This channel is happening sooner
         next_channel = i_channel;
      }
   }

   return next_channel;
}

void on_timer_triggered(antsys_timer_id_t timer)
{
  uint8_t scheduling_results[12] = {0};
  uint8_t activity_results[18] = {0};
  if(timer == ANTSYS_TIMER_ID_PREPARE)
  {
    // This simulates turn around delays
#ifdef ANT_DEBUG_GPIOS
    GPIO_REG(GPIO_TASK_STACK) = GPIO_OUTPUT_HIGH;
    GPIO_REG(GPIO_TASK_STACK) = GPIO_OUTPUT_LOW;
#endif		
    antsys_result_t request_result = antsys_vnd_ops_p->request_activity(&sched_params, &blocked_info);

    // We want to check if we were blocked and retry if so
    if (request_result == ANTSYS_ERR_SCHED_BLOCKED)
    {
      schedules_blocked++;

      // Pend the task, it will try to reschedule
      antsys_vnd_ops_p->pend_stack_task();
    }
    antsys_vnd_ops_p->complete_activity();
  }
  else
  {
    // Build up a packet
    *(uint16_t *)&scheduling_results[0] = RSI_ANT_SCHED_STATS;
    scheduling_results[2] = activities_blocked & 0xFF;
    scheduling_results[3] = (activities_blocked >> 8) & 0xFF;
    scheduling_results[4] = activities_aborted & 0xFF;
    scheduling_results[5] = (activities_aborted >> 8) & 0xFF;
    scheduling_results[6] = schedules_blocked & 0xFF;
    scheduling_results[7] = (schedules_blocked >> 8) & 0xFF;
    scheduling_results[8] = radio_ops_complete & 0xFF;
    scheduling_results[9] = (radio_ops_complete >> 8) & 0xFF;
    scheduling_results[10] = schedules_attempted & 0xFF;
    scheduling_results[11] = (schedules_attempted >> 8) & 0xFF;
    antsys_vnd_ops_p->send_ant_rx_pkt_to_host(scheduling_results, sizeof(scheduling_results));

    // Build up a packet
    *(uint16_t *)&activity_results[0] = RSI_ANT_ACTIVITY_STATS;
    activity_results[2]  = activity_status_ok & 0xFF;
    activity_results[3]  = (activity_status_ok >> 8) & 0xFF;
    activity_results[4]  = activity_status_aborted & 0xFF;
    activity_results[5]  = (activity_status_aborted >> 8) & 0xFF;
    activity_results[6]  = activity_status_too_late & 0xFF;
    activity_results[7]  = (activity_status_too_late >> 8) & 0xFF;
    activity_results[8]  = activity_status_no_sync & 0xFF;
    activity_results[9]  = (activity_status_no_sync >> 8) & 0xFF;
    activity_results[10]  = activity_status_incomplete & 0xFF;
    activity_results[11]  = (activity_status_incomplete >> 8) & 0xFF;
    activity_results[12] = activity_status_bad_crc & 0xFF;
    activity_results[13] = (activity_status_bad_crc >> 8) & 0xFF;
    activity_results[14] = activity_status_vnd_error_start & 0xFF;
    activity_results[15] = (activity_status_vnd_error_start >> 8) & 0xFF;
    activity_results[16] = activity_status_skipped & 0xFF;
    activity_results[17] = (activity_status_skipped >> 8) & 0xFF;
    antsys_vnd_ops_p->send_ant_rx_pkt_to_host(activity_results, sizeof(activity_results));
    antsys_vnd_ops_p->arm_timer(ANTSYS_TIMER_ID_ACTIVITY, antsys_vnd_ops_p->get_sys_time() + (1000 * 1000));

  }
}

antsys_result_t ant_update_modulation_index(void)
{
   INT_RF_WR(0xA9, REDPINE_RF_MOD_INX_0P32);
   return ANTSYS_ERR_NONE;
}

antsys_result_t request_to_change_ta_freq(void)
{
  INTF_RF_WR_1(0xC, 0x800);
  return ANTSYS_ERR_NONE;
}
void on_activity_blocked(const antsys_blocked_info_t blocked_info)
{
#ifdef ANT_DEBUG_GPIOS
   GPIO_REG(GPIO_BLOCKED) = GPIO_OUTPUT_HIGH;
#endif
   // In this case, the activity is blocked before it runs, so we never start the activity.
   // We will reschedule the activity, but before doing so, increment the blocked counter.
   activities_blocked++;
   activity_status_skipped++;

#if defined TEST_TYPE_BURST
   coex_evt.burst_passed      = false;
   coex_evt.is_burst_master   = false;
#endif

   run_find_next = true;

   // We then pend the stack task to schedule next activity
   antsys_vnd_ops_p->pend_stack_task();
#ifdef ANT_DEBUG_GPIOS
   GPIO_REG(GPIO_BLOCKED) = GPIO_OUTPUT_LOW;
#endif
}

void on_activity_aborted(const antsys_blocked_info_t blocked_info)
{
   // In this case, the activity is being interrupted mid-way, so we will not get the on_activity_complete
   // callback.
   // In this example, the on_activity_complete callback is incrementing one of the bytes in the Tx buffer.
   // In reality, ANT stack might be doing some packet-specific activities

   // In addition to incrementing the last Tx buffer byte, we also increment another byte to indicate that
   // the activity was aborted. This simulates the ANT stack performing some other activity to deal with an
   // aborted activity.

#ifdef ANT_DEBUG_GPIOS
   GPIO_REG(GPIO_RX) = GPIO_OUTPUT_LOW;
   //!GPIO_REG(GPIO_TX) = GPIO_OUTPUT_LOW;
#endif

   activities_aborted++;

#if defined TEST_TYPE_BURST
   coex_evt.burst_passed      = false;
   coex_evt.is_burst_master   = false;
#endif

   run_find_next = true;

   // We then pend the stack task to schedule next activity
   antsys_vnd_ops_p->pend_stack_task();
}

/* This Function was written for the Example and testing purpose
 * Based on the Requirement can change it
 */
antsys_result_t ant_tx_pkt_event_handler(uint8_t *data, uint16_t pkt_len)
{
   uint8_t opcode = data[0];
   uint8_t status = ANTSYS_ERR_NONE;
   uint8_t ant_version[6] = {0x41, 0x4E, 0x54, 0x31, 0x2E, 0x30};
   uint8_t scheduling_results[10] = {0};
   uint8_t activity_results[14] = {0};

   antsys_vnd_ops_p = RSI_VND_STRUCT_ADDR;

   switch(opcode)
   {
      case START_ATM:
         run_simple_test(data[1]);
         antsys_vnd_ops_p->send_ant_rx_pkt_to_host(&status, 1);
         break;
      case ATM_GET_VERSION:
         antsys_vnd_ops_p->send_ant_rx_pkt_to_host(ant_version, sizeof(ant_version));
         break;
      case ANT_GET_SCHEDULING_STATS:
         // Build up a packet
         scheduling_results[0] = activities_blocked & 0xFF;
         scheduling_results[1] = (activities_blocked >> 8) & 0xFF;
         scheduling_results[2] = activities_aborted & 0xFF;
         scheduling_results[3] = (activities_aborted >> 8) & 0xFF;
         scheduling_results[4] = schedules_blocked & 0xFF;
         scheduling_results[5] = (schedules_blocked >> 8) & 0xFF;
         scheduling_results[6] = radio_ops_complete & 0xFF;
         scheduling_results[7] = (radio_ops_complete >> 8) & 0xFF;
         scheduling_results[8] = schedules_attempted & 0xFF;
         scheduling_results[9] = (schedules_attempted >> 8) & 0xFF;
         antsys_vnd_ops_p->send_ant_rx_pkt_to_host(scheduling_results, sizeof(scheduling_results));
         break;
      case ANT_GET_ACTIVITY_STATS:
         // Build up a packet
         activity_results[0]  = activity_status_ok & 0xFF;
         activity_results[1]  = (activity_status_ok >> 8) & 0xFF;
         activity_results[2]  = activity_status_aborted & 0xFF;
         activity_results[3]  = (activity_status_aborted >> 8) & 0xFF;
         activity_results[4]  = activity_status_too_late & 0xFF;
         activity_results[5]  = (activity_status_too_late >> 8) & 0xFF;
         activity_results[6]  = activity_status_no_sync & 0xFF;
         activity_results[7]  = (activity_status_no_sync >> 8) & 0xFF;
         activity_results[8]  = activity_status_incomplete & 0xFF;
         activity_results[9]  = (activity_status_incomplete >> 8) & 0xFF;
         activity_results[10] = activity_status_bad_crc & 0xFF;
         activity_results[11] = (activity_status_bad_crc >> 8) & 0xFF;
         activity_results[12] = activity_status_vnd_error_start & 0xFF;
         activity_results[13] = (activity_status_vnd_error_start >> 8) & 0xFF;
         antsys_vnd_ops_p->send_ant_rx_pkt_to_host(activity_results, sizeof(activity_results));
         break;
   }
   return ANTSYS_ERR_NONE;
}

void run_simple_test(uint8_t flags)
{
   uint8_t i_channel = 0;
   memset(&__bss_start, 0, (&_end - &__bss_start));

#ifdef ANT_DEBUG_GPIOS
   GPIO_REG(GPIO_RX) = GPIO_OUTPUT_LOW;
  //! GPIO_REG(GPIO_TX) = GPIO_OUTPUT_LOW;
#endif

   antsys_vnd_ops_p = RSI_VND_STRUCT_ADDR;
   antsys_vnd_ops_p->init_interface();

   run_find_next                    = true;

   uint32_t i_channel_time_offset   = ANT_CHANNEL_SPACING;
   uint32_t current_time             = antsys_vnd_ops_p->get_sys_time() + ANT_TEST_PREPARE_LONG;

   for (i_channel = 0; i_channel < ANT_TEST_NUM_CHANNELS; i_channel++)
   {
      channel_time[i_channel] = current_time + i_channel_time_offset;
      i_channel_time_offset  += ANT_CHANNEL_SPACING;
   }

#if defined(TEST_TYPE_BURST)
   activity_is_tx             = true;
#elif defined(TEST_TYPE_MASTER_CHANNEL)
   activity_is_tx             = true;
#elif defined(TEST_TYPE_SEARCH)
   activity_is_tx             = false;
#elif defined(TEST_TYPE_COMBINED)
   activity_is_tx             = false;
#endif

   // Reset all counters
   activities_blocked                           = 0;
   activities_aborted                           = 0;
   schedules_blocked                            = 0;
   radio_ops_complete                           = 0;
   schedules_attempted                          = 0;
   activity_status_ok                           = 0;
   activity_status_aborted                      = 0;
   activity_status_too_late                     = 0;
   activity_status_no_sync                      = 0;
   activity_status_incomplete                   = 0;
   activity_status_bad_crc                      = 0;
   activity_status_vnd_error_start              = 0;
   activity_status_skipped                      = 0;

   antsys_vnd_ops_p->pend_stack_task();
   antsys_vnd_ops_p->arm_timer(ANTSYS_TIMER_ID_ACTIVITY, antsys_vnd_ops_p->get_sys_time() + (1000 * 1000));
}

void run_stack_task(void)
{
   static antsys_time_t ch_time = 0;

   if (ch_time == 0)
   {
      ch_time = antsys_vnd_ops_p->get_sys_time();
   }

#ifdef ANT_DEBUG_GPIOS
   //GPIO_REG(GPIO_RX) = GPIO_OUTPUT_LOW;
   //!GPIO_REG(GPIO_TX) = GPIO_OUTPUT_LOW;
#endif

   if (run_find_next)
   {
      active_channel          = find_next_channel();
      active_channel_time     = channel_time[active_channel];
      active_channel_interval = channel_period[active_channel];
      run_find_next           = false;
   }

   coex_evt.link_id           = active_channel;
   coex_evt.period            = active_channel_period;
   coex_evt.start_time        = active_channel_time;
   //coex_evt.evt_type          = ANTSYS_COEX_EVENT_CHANNEL_OPEN;
#if defined TEST_TYPE_BURST
   coex_evt.burst_passed      = true;
   coex_evt.is_burst_master   = true;
#elif defined(TEST_TYPE_MASTER_CHANNEL)
   coex_evt.burst_passed      = false;
   coex_evt.is_burst_master   = false;
#elif defined(TEST_TYPE_SEARCH)
   coex_evt.burst_passed      = false;
   coex_evt.is_burst_master   = false;
   activity_is_tx             = false;
#elif defined(TEST_TYPE_COMBINED)
   coex_evt.burst_passed      = false;
   coex_evt.is_burst_master   = false;
   activity_is_tx             = false;
#else
   coex_evt.burst_passed      = false;
   coex_evt.is_burst_master   = false;
#endif
   antsys_vnd_ops_p->on_coex_event(&coex_evt);
   current_priority = ANTSYS_PRIORITY_DEFAULT;
   //current_priority = ANTSYS_PRIORITY_CRITICAL;
   //current_priority = ANTSYS_PRIORITY_HIGH;

#if 0
   if ((antsys_vnd_ops_p->get_sys_time() - ch_time) > 5000000)
   {
      ch_time = antsys_vnd_ops_p->get_sys_time();
      
#if 0
      if (channel_period[0] == 2960)
      {
         //channel_period[0] = 30000;
         //channel_period[0] = 37500;
         channel_period[0] = 120000;
      }
      else
#endif
      {
         channel_period[0] = 2960;
      }
      current_priority++;
      if (current_priority > ANTSYS_PRIORITY_CRITICAL)
      {
         current_priority = ANTSYS_PRIORITY_DEFAULT;
      }
   }
#endif
   coex_params.link_id        = 0;
   coex_params.priority       = current_priority;
#if defined(TEST_TYPE_BURST)
   coex_params.traffic_type   = ANTSYS_TRAFFIC_TYPE_BURST;
#elif defined(TEST_TYPE_MASTER_CHANNEL)
   coex_params.traffic_type   = ANTSYS_TRAFFIC_TYPE_BROADCAST;
#elif defined(TEST_TYPE_SEARCH)
   coex_params.traffic_type   = ANTSYS_TRAFFIC_TYPE_SEARCH;
#elif defined(TEST_TYPE_COMBINED)
   coex_params.traffic_type   = ANTSYS_TRAFFIC_TYPE_BROADCAST;
#else
   #error "Cannot define traffic type - test type unknown!"
#endif

   sched_params.coex          = &coex_params;

   bool ret = false;

#if defined(TEST_TYPE_BURST)
   ret = schedule_simple_burst();
#elif defined(TEST_TYPE_MASTER_CHANNEL)
   ret = schedule_simple_broadcast();
#elif defined(TEST_TYPE_SEARCH)
   ret = schedule_simple_search();
#elif defined(TEST_TYPE_COMBINED)
   ret = schedule_simple_slave();
#else
   #error "Cannot schedule - test type unknown!"
#endif

   schedules_attempted++;
   if (!ret)
   {
      // Try again
      antsys_vnd_ops_p->pend_stack_task();
      return;
   }

   antsys_vnd_ops_p->arm_timer(ANTSYS_TIMER_ID_PREPARE, antsys_vnd_ops_p->get_sys_time() + ANT_TEST_TURNAROUND_DELAY);
}

#if defined TEST_TYPE_BURST
bool schedule_simple_burst(void)
{
   uint32_t sync_timeout = 0;

   // Determine where we are in the burst
   if ((test_burst_interval_idx == (ANT_TEST_MAX_BURST_PACKETS * 2 + 1)) || (test_burst_interval_idx == 0))
   {
      // Finished burst, start again
      test_burst_interval_idx = 0;
      test_burst_packet_idx = 0;
      activity_is_tx = true;
      channel_time[active_channel] += channel_period[active_channel];
      channel_time_segment_offset = 0;
   }
   else if (test_burst_interval_idx == 1)
   {
      // First Rx window
      channel_time_segment_offset += ANT_TEST_INITIAL_TX_TO_RX;
      activity_is_tx = false;
   }
   else if (test_burst_interval_idx == (ANT_TEST_MAX_BURST_PACKETS * 2))
   {
      // Done burst, need to open last Rx window
      channel_time_segment_offset += ANT_TEST_END_RX_TO_RX;
      activity_is_tx = false;
   }
   else
   {
      // Mid burst
      if ((test_burst_interval_idx & 0x1) == 0)
      {
         // Tx windows occur at even indexes
         activity_is_tx = true;
         channel_time_segment_offset += ANT_TEST_MID_RX_TO_TX;
      }
      else
      {
         // Rx windows otherwise
         activity_is_tx = false;
         channel_time_segment_offset += ANT_TEST_MID_TX_TO_RX;
      }
   }

   sched_params.min_start     = active_channel_time + channel_time_segment_offset - ANT_TEST_PREPARE_LONG;
   if (
      (sched_params.min_start < antsys_vnd_ops_p->get_sys_time()) &&
      ((sched_params.min_start - antsys_vnd_ops_p->get_sys_time()) > HALF_MAX_TIME)
      )
   {
      // Too late, something went wrong? Weird... Abandon ship and reschedule.
      return false;
   }

   sched_params.max_start     = active_channel_time + channel_time_segment_offset - ANT_TEST_PREPARE_SHORT;

   if (!activity_is_tx)
   {
      // For Rx windows, the activity duration is increased by the potential sync timeout
      sync_timeout            = ANT_TEST_SYNC_TIMEOUT;
   }

   sched_params.min_duration  = ANT_TEST_PACKET_TIMEOUT + sync_timeout + ANT_TEST_PREPARE_LONG;
   sched_params.max_duration  = ANT_TEST_PACKET_TIMEOUT + sync_timeout + ANT_TEST_PREPARE_LONG;
   sched_params.deadline      = ANT_TEST_PACKET_TIMEOUT + sync_timeout + ANT_TEST_PREPARE_LONG;

   return true;
}
#endif

#if defined(TEST_TYPE_MASTER_CHANNEL)
bool schedule_simple_broadcast(void)
{
   uint32_t sync_timeout = 0;

   if (activity_is_tx)
   {
      channel_time[active_channel] += channel_period[active_channel];
      channel_time_segment_offset   = 0;
   }
   else
   {
      // For Rx windows, the activity duration is increased by the potential sync timeout
      sync_timeout                  = ANT_TEST_SYNC_TIMEOUT;
      channel_time_segment_offset   = ANT_TEST_TX_TO_RX;
   }

   sched_params.min_start     = active_channel_time + channel_time_segment_offset - ANT_TEST_PREPARE_LONG;
   if (
      (sched_params.min_start < antsys_vnd_ops_p->get_sys_time()) &&
      ((sched_params.min_start - antsys_vnd_ops_p->get_sys_time()) > HALF_MAX_TIME)
      )
   {
      run_find_next  = true;
      // Too late, something went wrong? Weird... Abandon ship and reschedule.
      return false;
   }

   sched_params.max_start     = active_channel_time + channel_time_segment_offset - ANT_TEST_PREPARE_SHORT;

   sched_params.min_duration  = ANT_TEST_PACKET_TIMEOUT + sync_timeout + ANT_TEST_PREPARE_LONG;
   sched_params.max_duration  = ANT_TEST_PACKET_TIMEOUT + sync_timeout + ANT_TEST_PREPARE_LONG;
   sched_params.deadline      = ANT_TEST_PACKET_TIMEOUT + sync_timeout + ANT_TEST_PREPARE_LONG;

   return true;
}
#endif

#if defined(TEST_TYPE_SEARCH)
bool schedule_simple_search(void)
{
   uint32_t sync_timeout         = 0;
   channel_time_segment_offset   = 0;

   channel_time[active_channel] += channel_period[active_channel];

   // For Rx windows, the activity duration is increased by the potential sync timeout
   sync_timeout                  = ANT_TEST_SYNC_TIMEOUT;

   sched_params.min_start        = active_channel_time + channel_time_segment_offset - ANT_TEST_PREPARE_LONG;
   if (
      (sched_params.min_start < antsys_vnd_ops_p->get_sys_time()) &&
      ((sched_params.min_start - antsys_vnd_ops_p->get_sys_time()) > HALF_MAX_TIME)
      )
   {
      // Too late, something went wrong? Weird... Abandon ship and reschedule.
      return false;
   }

   sched_params.max_start        = active_channel_time + channel_time_segment_offset - ANT_TEST_PREPARE_SHORT;
   sched_params.min_duration     = ANT_TEST_PACKET_TIMEOUT + ANT_TEST_SYNC_TIMEOUT + ANT_TEST_PREPARE_LONG;
   sched_params.max_duration     = ANT_TEST_PACKET_TIMEOUT + ANT_TEST_SYNC_TIMEOUT + ANT_TEST_PREPARE_LONG;
   sched_params.deadline         = ANT_TEST_PACKET_TIMEOUT + ANT_TEST_SYNC_TIMEOUT + ANT_TEST_PREPARE_LONG;

   return true;
}
#endif

#if defined(TEST_TYPE_COMBINED)
bool schedule_simple_slave(void)
{
   uint32_t sync_timeout         = 0;
   channel_time_segment_offset   = 0;

   channel_time[active_channel] += channel_period[active_channel];

   // For Rx windows, the activity duration is increased by the potential sync timeout
   sync_timeout                  = ANT_TEST_SYNC_TIMEOUT;

   sched_params.min_start        = active_channel_time + channel_time_segment_offset - ANT_TEST_PREPARE_LONG;
   if (
      (sched_params.min_start < antsys_vnd_ops_p->get_sys_time()) &&
      ((sched_params.min_start - antsys_vnd_ops_p->get_sys_time()) > HALF_MAX_TIME)
      )
   {
      // Too late, something went wrong? Weird... Abandon ship and reschedule.
      return false;
   }

   sched_params.max_start        = active_channel_time + channel_time_segment_offset - ANT_TEST_PREPARE_SHORT;
   sched_params.min_duration     = ANT_TEST_PACKET_TIMEOUT + ANT_TEST_SYNC_TIMEOUT + ANT_TEST_PREPARE_LONG;
   sched_params.max_duration     = ANT_TEST_PACKET_TIMEOUT + ANT_TEST_SYNC_TIMEOUT + ANT_TEST_PREPARE_LONG;
   sched_params.deadline         = ANT_TEST_PACKET_TIMEOUT + ANT_TEST_SYNC_TIMEOUT + ANT_TEST_PREPARE_LONG;

   return true;
}
#endif

void on_activity_started(const antsys_activity_info_t * info)
{
   uint8_t i = 0;
   // Once in the activity, we have exclusive access to the radio
   // So we can schedule the radio session now
   // This radio session will start between ANT_TEST_PREPARE_LONG and ANT_TEST_PREPARE_SHORT from now
   memset((uint8_t *)&radio_config, 0, sizeof(antsys_radio_config_t));
   memset(test_protocol_payload, 0, sizeof(test_protocol_payload));

   radio_config.mode_flags    = ANTSYS_MODE_FLAG_MSBIT_FIRST | ANTSYS_MODE_FLAG_CRC_16BIT | ANTSYS_MODE_FLAG_UPDATE_MOD_INX /*| ANTSYS_MODE_FLAG_PROTOCOL_RADIO_SHARE_DEBUG*/;
   radio_config.sync_word     = ANT_TEST_RADIO_SYNC_WORD;
   radio_config.rf_channel    = ANT_TEST_RF_FREQ;
   radio_config.crc_poly      = ANT_TEST_CRC_POLY;
   radio_config.crc_init      = ANT_TEST_CRC_INIT;
   radio_config.data_len      = ANT_TEST_PAYLOAD_LEN;
	 radio_config.tx_power			= ANT_TEST_RADIO_TX_POWER;
   radio_config.data          = test_protocol_payload;
   radio_config.start_time    = active_channel_time + channel_time_segment_offset;

   if (
      (radio_config.start_time < antsys_vnd_ops_p->get_sys_time()) &&
      ((radio_config.start_time - antsys_vnd_ops_p->get_sys_time()) > HALF_MAX_TIME)
      )
   {
      // Too late, abort.
      antsys_vnd_ops_p->pend_stack_task();
      return;
   }

   if (activity_is_tx)
   {
      radio_config.mode_flags   |= ANTSYS_MODE_FLAG_TX;
      radio_config.timeout       = ANT_TEST_PACKET_TIMEOUT;
      test_protocol_payload[0]   = ANT_TEST_DEV_ID & 0xFF;
      test_protocol_payload[1]   = (ANT_TEST_DEV_ID >> 8) & 0xFF;
      test_protocol_payload[2]   = ANT_TEST_DEV_TYPE & 0xFF;
      test_protocol_payload[3]   = ANT_TEST_TRANS_TYPE & 0xFF;
   #if defined (TEST_TYPE_BURST)
      if (test_burst_packet_idx == 0)
      {
         // Start of burst
         test_protocol_payload[4]   = 0x0A | TRANSFER_MODE;
      }
      else if (test_burst_packet_idx < (ANT_TEST_MAX_BURST_PACKETS - 1))
      {
         // Mid burst
         test_protocol_payload[4]   = BURST_20K | TRANSFER_MODE;
         if (test_burst_packet_idx & 0x1)
         {
            test_protocol_payload[4]  |= TRANSFER_SEQUENCE;
         }
      }
      else
      {
         // End of burst
         test_protocol_payload[4]   = BURST_20K | TRANSFER_COMPLETE | TRANSFER_MODE;
         if (test_burst_packet_idx & 0x1)
         {
            test_protocol_payload[4]  |= TRANSFER_SEQUENCE;
         }
      }
      for (i = 0; i < 8; i++)
      {
         // Generate payload
         test_protocol_payload[5 + i] = (test_burst_packet_idx * 0x8 + i) & 0xFF;
      }
      test_burst_packet_idx++;
   #elif defined(TEST_TYPE_MASTER_CHANNEL)
      test_protocol_payload[4] = 0x0A;
      for (i = 0; i < 8; i++)
      {
         // Generate payload
         test_protocol_payload[5 + i] = schedules_attempted;
      }
   #endif
#ifdef ANT_DEBUG_GPIOS
      //!GPIO_REG(GPIO_TX) = GPIO_OUTPUT_HIGH;
#endif
   }
   else
   {
      radio_config.sync_timeout  = radio_config.start_time + ANT_TEST_SYNC_TIMEOUT;
      radio_config.timeout       = radio_config.sync_timeout + ANT_TEST_PACKET_TIMEOUT;
#ifdef ANT_DEBUG_GPIOS
      GPIO_REG(GPIO_RX) = GPIO_OUTPUT_HIGH;
#endif
   }

#if defined (TEST_TYPE_BURST)
   test_burst_interval_idx++;
   if ((test_burst_interval_idx == (ANT_TEST_MAX_BURST_PACKETS * 2 + 1)) || (test_burst_interval_idx == 0))
   {
      // End of burst
      run_find_next  = true;
   }
#elif defined (TEST_TYPE_MASTER_CHANNEL)
   activity_is_tx    = (activity_is_tx ? false : true);
   if (activity_is_tx)
   {
      run_find_next  = true;
   }
#elif defined (TEST_TYPE_SEARCH)
   activity_is_tx    = false;
   run_find_next     = true;
#elif defined (TEST_TYPE_COMBINED)
   activity_is_tx    = false;
   run_find_next     = true;
#endif

   if (antsys_vnd_ops_p->config_radio(&radio_config) != ANTSYS_ERR_NONE)
   {
      // Could not schedule, gotta pend the stack task and try again later
      antsys_vnd_ops_p->pend_stack_task();
   }
}

void on_radio_op_complete(const antsys_radio_result_t *result)
{
#ifdef ANT_DEBUG_GPIOS
   GPIO_REG(GPIO_RX) = GPIO_OUTPUT_LOW;
   //!GPIO_REG(GPIO_TX) = GPIO_OUTPUT_LOW;
#endif

#if defined TEST_TYPE_BURST
   coex_evt.burst_passed      = true;
   coex_evt.is_burst_master   = true;
#endif

   bool curr_activity_is_tx = !activity_is_tx;     // Activity type was changed earlier before radio was configured

   if (curr_activity_is_tx)
   {
      if (result->status != ANTSYS_RADIO_STATUS_OK)
      {
      #if defined (TEST_TYPE_BURST)
         // Abort burst
         test_burst_interval_idx    = 0;
         test_burst_packet_idx      = 0;
         coex_evt.burst_passed      = false;
         coex_evt.is_burst_master   = false;
         run_find_next              = true;
      #endif
      }
   }

   switch (result->status)
   {
      case ANTSYS_RADIO_STATUS_OK:
         activity_status_ok++;
         break;
      case ANTSYS_RADIO_STATUS_ABORTED:
         activity_status_aborted++;
         break;
      case ANTSYS_RADIO_STATUS_TOO_LATE:
         activity_status_too_late++;
         break;
      case ANTSYS_RADIO_STATUS_NO_SYNC:
         activity_status_no_sync++;
         break;
      case ANTSYS_RADIO_STATUS_INCOMPLETE:
         activity_status_incomplete++;
         break;
      case ANTSYS_RADIO_STATUS_BAD_CRC:
         activity_status_bad_crc++;
         break;
      case ANTSYS_RADIO_STATUS_VND_ERR_START:
         activity_status_vnd_error_start++;
         break;
   }

   radio_ops_complete++;

   // The transmission is now complete, we pend stack task to determine what to do next
   antsys_vnd_ops_p->pend_stack_task(); // run next iteration, execute run_stack_task() callback
}
