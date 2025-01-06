//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of trace log function.
//=====================================================================================

#if !defined(SCO_TRACE_LOG_H)
#define SCO_TRACE_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
// defines for trace log
#define TC_LOG_RECORD_SIZE                     sizeof(trace_log_t)
#if TC_LOG_RECORD_NUM != 0
#define start_trace_log(mode)                  i_start_trace_log(mode)
#define stop_trace_log()                       i_stop_trace_log()
#define get_trace_log(p_msg, p_rest, p_buf)    i_get_trace_log(p_msg, p_rest, p_buf)
#if TC_LOG_APPLICATION_LEVEL == TRACE_LOG_APPLICATION_ENABLE
#define set_trace_log(msg_type, a1, a2, a3)    i_set_trace_log(msg_type, a1, a2, a3)
#else
#define set_trace_log(msg_type, a1, a2, a3)    set_trace_log_dummy()
#endif // TC_LOG_APPLICATION_LEVEL == TRACE_LOG_APPLICATION_ENABLE
#else
#define start_trace_log(mode)
#define stop_trace_log()
#define set_trace_log(msg_type, a1, a2, a3)    set_trace_log_dummy()
#define get_trace_log(p_msg, p_rest, p_buf)    get_trace_log_dummy()
#endif // TC_LOG_RECORD_NUM != 0

//! error number of trace log function
typedef enum trace_log_errno {
	TC_LOG_ERR_NULL = SCHED_LOG_ERR_NUM, //!< : null pointer
	TC_LOG_ERR_TRACE_STOP,               //!< : trace log function not running
	TC_LOG_ERR_OVER_FLOW = 1,            //!< : logging buffer over flow
	TC_LOG_ERR_TRACE_AUTO_STOP = 2,      //!< : auto stop to trace log
	TC_LOG_ERR_SUCCESS = 0               //!< : success
} trace_log_errno_t, *p_trace_log_errno_t;

//! log trace mode
typedef enum trace_mode {
	TC_LOG_ENDLESS = 1,                  //!< : trace for ring buffer
	TC_LOG_AUTO_STOP                     //!< : trace until buffer full
} trace_mode_t, *p_trace_mode_t;

//! trace log structure
typedef struct trace_log {
	u16 msg_typ;                         //!< logging message type
	s16 arg1;                            //!< message argument 1
	s16 arg2;                            //!< message argument 2
	s16 arg3;                            //!< message argument 3
#if defined(TC_LOG_SUPPORT_TIME)
	u32 upper_time;                      //!< trace time of upper(time tick order)
	u32 lower_time;                      //!< trace time of lower(us order)
#else
	u32 log_count;                       //!< total number from system start
#endif // defined(TC_LOG_SUPPORT_TIME)
} trace_log_t, *p_trace_log_t;

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
void              i_start_trace_log(trace_mode_t mode);
void              i_stop_trace_log(void);
trace_log_errno_t i_set_trace_log(u16 msg_type, s16 msg_arg1, s16 msg_arg2, s16 msg_arg3);
trace_log_errno_t i_get_trace_log(u32 *p_msg_count, u32 *p_rest_count, void *p_msg_buf);
trace_log_errno_t set_trace_log_dummy(void);
trace_log_errno_t get_trace_log_dummy(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_TRACE_LOG_H)
