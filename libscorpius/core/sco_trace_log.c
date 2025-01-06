//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  This file provides the following functions of trace log.
//!          - initial_trace_log()
//!          - i_start_trace_log()
//!          - i_stop_trace_log()
//!          - i_set_trace_log()
//!          - i_get_trace_log()
//!          - i_set_trace_log_dispatch()
//!          - set_trace_log_dummy()
//!          - get_trace_log_dummy()
//=====================================================================================

#include <sco_common.h>
#include <sco_config.h>
#include <sco_cpu.h>
#include <sco_scheduler.h>
#include <sco_scheduler_i.h>
#include <sco_trace_log.h>
#include <sco_trace_log_i.h>
#include <sco_trace_log_msg.h>
#include <sco_time.h>
#include <sco_time_i.h>

#if (TC_LOG_RECORD_NUM != 0)

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// internal function prototype
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
static trace_log_t  g_trace_log_buffer[TC_LOG_RECORD_NUM];
static int          g_trace_mode;
static u32          g_trace_log_head;
static u32          g_trace_log_tail;
static u32          g_trace_log_cnt;
#if !defined(TC_LOG_SUPPORT_TIME)
static u32          g_trace_log_total_cnt;
#endif // defined(TC_LOG_SUPPORT_TIME)

//------------------------------------------------------------------------------
//! @brief Initial trace log.
//
//! @return None
//------------------------------------------------------------------------------
void initial_trace_log(void)
{
#if defined(TC_LOG_SUPPORT_TIME)
	initial_system_time();
#else
	g_trace_log_total_cnt = 0;
#endif // defined(TC_LOG_SUPPORT_TIME)

	i_start_trace_log(TC_LOG_AUTO_STOP);
}

//------------------------------------------------------------------------------
//! @brief Start trace log.
//!        - select trace mode
//
//! @param[in] mode : trace mode
//
//! @return None
//------------------------------------------------------------------------------
void i_start_trace_log(trace_mode_t mode)
{
	u16 msg_type;
	u32 irq_sts;

	if (mode == TC_LOG_ENDLESS) {
		msg_type = TC_LOG_MSG_START_ENDLESS;
	}
	else {
		msg_type = TC_LOG_MSG_START_AUTO_STOP;
	}

	disable_interrupt(&irq_sts);

	// reset trace log setting
	g_trace_log_cnt  = 0;
	g_trace_log_head = 0;
	g_trace_log_tail = 0;

	// set to start trace log and set start trace log initial message
	g_trace_mode = mode;
	SET_TC_LOG(msg_type, 0, 0, 0);

	enable_interrupt(irq_sts);
}

//------------------------------------------------------------------------------
//! @brief Stop trace log.
//
//! @return None
//------------------------------------------------------------------------------
void i_stop_trace_log(void)
{
	// set stop trace log
	g_trace_mode = 0;
}

//------------------------------------------------------------------------------
//! @brief Set trace log.
//
//! @param[in] msg_type : logging message type
//! @param[in] msg_arg1 : message argument 1
//! @param[in] msg_arg2 : message argument 2
//! @param[in] msg_arg3 : message argument 3
//
//! @retval TC_LOG_ERR_SUCCESS         : success
//! @retval TC_LOG_ERR_TRACE_STOP      : trace log function not running
//! @retval TC_LOG_ERR_OVER_FLOW       : logging buffer over flow
//! @retval TC_LOG_ERR_TRACE_AUTO_STOP : auto stop to trace log
//------------------------------------------------------------------------------
trace_log_errno_t i_set_trace_log(u16 msg_type, s16 msg_arg1, s16 msg_arg2, s16 msg_arg3)
{
	trace_log_errno_t ret = TC_LOG_ERR_SUCCESS;
	u32 irq_sts;

	disable_interrupt(&irq_sts);

	if (g_trace_mode == 0) {
		ret = TC_LOG_ERR_TRACE_STOP;
		goto EXIT;
	}

#if defined(TC_LOG_SUPPORT_TIME)
	// set log trace time
	g_trace_log_buffer[g_trace_log_tail].upper_time = get_systime_upper();
	g_trace_log_buffer[g_trace_log_tail].lower_time = get_systime_lower();
#else
	// set log trace count(total from system start)
	g_trace_log_total_cnt++;
	g_trace_log_buffer[g_trace_log_tail].log_count = g_trace_log_total_cnt;
#endif // defined(TC_LOG_SUPPORT_TIME)

	// set log message to log buffer
	g_trace_log_buffer[g_trace_log_tail].msg_typ = msg_type;
	g_trace_log_buffer[g_trace_log_tail].arg1    = msg_arg1;
	g_trace_log_buffer[g_trace_log_tail].arg2    = msg_arg2;
	g_trace_log_buffer[g_trace_log_tail].arg3    = msg_arg3;
	g_trace_log_cnt++;
	g_trace_log_tail++;

	// set next log message loggin index
	if (g_trace_log_tail >= TC_LOG_RECORD_NUM) g_trace_log_tail = 0;

	if (g_trace_mode == TC_LOG_ENDLESS) {
		// trace log ring buffer mode
		if (g_trace_log_cnt > TC_LOG_RECORD_NUM) {
			// trace log buffer overflow
			g_trace_log_cnt = TC_LOG_RECORD_NUM;
			ret = TC_LOG_ERR_OVER_FLOW;

			// set next log message index
			g_trace_log_head++;
			if (g_trace_log_head >= TC_LOG_RECORD_NUM) g_trace_log_head = 0;
		}
	}
	else {
		// trace log auto stop mode
		if (g_trace_log_head == g_trace_log_tail) {
			// auto set mode to stop when trace log buffer full
			g_trace_mode = 0;
			ret = TC_LOG_ERR_TRACE_AUTO_STOP;
		}
	}

EXIT:
	enable_interrupt(irq_sts);

	return ret;
}

//------------------------------------------------------------------------------
//! @brief Get trace log.
//! @brief Note:
//!        Need to malloc message buffer, buffer size must over to
//!        request message count multiple TC_LOG_RECORD_SIZE.
//
//! @param[in,out] p_msg_count  : request or get message count
//! @param[out]    p_rest_count : rest message count in the log buffer
//! @param[out]    p_msg_buf    : user message buffer
//
//! @retval TC_LOG_ERR_SUCCESS : success
//! @retval TC_LOG_ERR_NULL    : null pointer
//------------------------------------------------------------------------------
trace_log_errno_t i_get_trace_log(u32 *p_msg_count, u32 *p_rest_count, void *p_msg_buf)
{
	trace_log_errno_t ret = TC_LOG_ERR_SUCCESS;
	u32 i, j, req_cnt, irq_sts;
	s8 *p_log_buf, *p_msg_buf_tmp;

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if ((p_msg_count == NULL) || (p_rest_count == NULL) || (p_msg_buf == NULL)) {
		ret = TC_LOG_ERR_NULL;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	p_msg_buf_tmp = p_msg_buf;

	// restore request message count
	req_cnt = *p_msg_count;

	disable_interrupt(&irq_sts);

	for (i = 0; i < req_cnt; i++) {
		// check log message count
		if (g_trace_log_cnt == 0) break;

		// get log message
		p_log_buf = (s8 *)&g_trace_log_buffer[g_trace_log_head];
		for (j = 0; j < TC_LOG_RECORD_SIZE; j++) {
			*p_msg_buf_tmp++ = *p_log_buf++;
		}
		g_trace_log_cnt--;
		g_trace_log_head++;

		if (g_trace_log_head >= TC_LOG_RECORD_NUM) g_trace_log_head = 0;
	}

	// set get message count and rest message count in log buffer
	*p_msg_count  = i;
	*p_rest_count = g_trace_log_cnt;

	enable_interrupt(irq_sts);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	return ret;
}

//------------------------------------------------------------------------------
//! @brief Set trace log.
//!        - for calling in dispatch.s
//
//! @return None
//------------------------------------------------------------------------------
void i_set_trace_log_dispatch(void)
{
	// get task id of leave from and set trace log
	SET_TC_LOG(TC_LOG_MSG_DSP_EXT_TSK, get_task_id(gp_current_task), 0, 0);

	// get task id of dispatch to and set trace log
	SET_TC_LOG(TC_LOG_MSG_DSP_ENT_TSK, get_task_id(gp_top_priority_task), 0, 0);
}
#else
//------------------------------------------------------------------------------
//! @brief Dummy function for get trace log.
//
//! @retval TC_LOG_ERR_SUCCESS : success
//------------------------------------------------------------------------------
trace_log_errno_t get_trace_log_dummy(void){return TC_LOG_ERR_SUCCESS;}
#endif // (TC_LOG_RECORD_NUM != 0)

#if (TC_LOG_RECORD_NUM == 0) || (TC_LOG_APPLICATION_LEVEL != TRACE_LOG_APPLICATION_ENABLE)
//------------------------------------------------------------------------------
//! @brief Dummy function for set trace log.
//
//! @retval TC_LOG_ERR_SUCCESS : success
//------------------------------------------------------------------------------
trace_log_errno_t set_trace_log_dummy(void){return TC_LOG_ERR_SUCCESS;}
#endif // (TC_LOG_RECORD_NUM == 0) || (TC_LOG_APPLICATION_LEVEL != TRACE_LOG_APPLICATION_ENABLE)
