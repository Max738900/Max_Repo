//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of scheduler trace log internal function.
//=====================================================================================

#if !defined(SCO_TRACE_LOG_I_H)
#define SCO_TRACE_LOG_I_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
// define trace mode
#if TC_LOG_RECORD_NUM != 0
#if TC_LOG_SCORPIUS_LEVEL == TRACE_LOG_SCORPIUS_FULL
#define SET_TC_LOG(msg_type, a1, a2, a3)       i_set_trace_log(msg_type, a1, a2, a3)
#define SET_TC_LOG_SVC(msg_type, a1, a2, a3)   i_set_trace_log(msg_type, a1, a2, a3)
#elif TC_LOG_SCORPIUS_LEVEL == TRACE_LOG_SCORPIUS_MINIMUM
#define SET_TC_LOG(msg_type, a1, a2, a3)       i_set_trace_log(msg_type, a1, a2, a3)
#define SET_TC_LOG_SVC(msg_type, a1, a2, a3)
#else // TC_LOG_SCORPIUS_LEVEL == TRACE_LOG_SCORPIUS_NON
#define SET_TC_LOG(msg_type, a1, a2, a3)
#define SET_TC_LOG_SVC(msg_type, a1, a2, a3)
#endif // TC_LOG_SCORPIUS_LEVEL == TRACE_LOG_SCORPIUS_FULL
#else
#define SET_TC_LOG(msg_type, a1, a2, a3)
#define SET_TC_LOG_SVC(msg_type, a1, a2, a3)
#endif // TC_LOG_RECORD_NUM != 0

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
void              initial_trace_log(void);
void              i_start_trace_log(trace_mode_t mode);
void              i_stop_trace_log(void);
trace_log_errno_t i_set_trace_log(u16 msg_type, s16 msg_arg1, s16 msg_arg2, s16 msg_arg3);
trace_log_errno_t i_get_trace_log(u32 *p_msg_count, u32 *p_rest_count, void *p_msg_buf);
void              i_set_trace_log_dispatch(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_TRACE_LOG_I_H)
