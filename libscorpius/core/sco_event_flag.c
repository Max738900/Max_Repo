//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  This file provides the following functions of event flag.
//!          - initial_event_flag()
//!          - wait_event_flag()
//!          - poll_event_flag()
//!          - set_event_flag()
//!          - clear_event_flag()
//!          - refer_event_flag()
//=====================================================================================

#include <sco_common.h>
#include <sco_config.h>
#include <sco_cpu.h>
#include <sco_scheduler.h>
#include <sco_scheduler_i.h>
#include <sco_event_flag.h>
#include <sco_trace_log.h>
#include <sco_trace_log_i.h>
#include <sco_trace_log_msg.h>

#if (SCHED_TOTAL_EVENT_FLAG != 0)

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//! event flag control structure
typedef struct event_flag {
	u16               event_flag;   //!< event flag pattern
	u16               wait_flag;    //!< wait flag pattern
	event_wait_mode_t wait_mode;    //!< wait mode
	p_task_t          p_wait_task;  //!< task context address of wait event flag
} event_flag_t, *p_event_flag_t;

//------------------------------------------------------------------------------
// internal function prototype
//------------------------------------------------------------------------------
static bool is_agree_event_flag(event_wait_mode_t wait_mode, u16 wait_flag, u16 event_flag);

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
static event_flag_t g_event_flag[SCHED_TOTAL_EVENT_FLAG];

//------------------------------------------------------------------------------
//! @brief Initial event flag.
//
//! @return None
//------------------------------------------------------------------------------
void initial_event_flag(void)
{
	int i;

	// setup initial state of event flag function
	for (i = 0; i < SCHED_TOTAL_EVENT_FLAG; i++) {
		g_event_flag[i].event_flag  = 0;
		g_event_flag[i].p_wait_task = NULL;
	}
}

//------------------------------------------------------------------------------
//! @brief Wait event flag.
//
//! @param[in]  id                   : event flag id
//! @param[in]  wait_mode            : wait mode
//! @param[in]  wait_flag_pattern    : wait flag pattern
//! @param[out] p_event_flag_pattern : event flag pattern
//
//! @retval EVENT_FLAG_ERR_SUCCESS           : success
//! @retval EVENT_FLAG_ERR_ILLEGAL_ID        : not support event flag id
//! @retval EVENT_FLAG_ERR_ILLEGAL_CONTEXT   : must be called in task context
//! @retval EVENT_FLAG_ERR_IN_IDLE_TASK      : called in idle task
//! @retval EVENT_FLAG_ERR_ILLEGAL_WAIT_FLAG : illegal wait flag pattern (0x0000)
//! @retval EVENT_FLAG_ERR_ALREADY_WAIT_TASK : other task already wait event flag
//------------------------------------------------------------------------------
event_flag_errno_t wait_event_flag(u8 id, event_wait_mode_t wait_mode, u16 wait_flag_pattern, u16 *p_event_flag_pattern)
{
	event_flag_errno_t ret = EVENT_FLAG_ERR_SUCCESS;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_WAI_FLG, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_EVENT_FLAG) {
		ret = EVENT_FLAG_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	if (_is_task_ctx_mode() == FALSE) {
		// this function must be called in task context
		ret = EVENT_FLAG_ERR_ILLEGAL_CONTEXT;
		goto EXIT;
	}

	if (gp_current_task->p_next == NULL) {
		// this function can't be called in idle task
		ret = EVENT_FLAG_ERR_IN_IDLE_TASK;
		goto EXIT;
	}

	if (wait_flag_pattern == 0x0000) {
		// not support wait flag pattern 0x0000
		ret = EVENT_FLAG_ERR_ILLEGAL_WAIT_FLAG;
		goto EXIT;
	}

	_cpu_lock();

	// check other task wait event flag already
	if (g_event_flag[id].p_wait_task != NULL) {
		_cpu_unlock();
		ret = EVENT_FLAG_ERR_ALREADY_WAIT_TASK;
		goto EXIT;
	}
#else
	_cpu_lock();
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	// check wait mode and wait flag match event flag already
	if (is_agree_event_flag(wait_mode, wait_flag_pattern, g_event_flag[id].event_flag) == FALSE) {
		// set own task to wait event flag
		g_event_flag[id].wait_mode   = wait_mode;
		g_event_flag[id].wait_flag   = wait_flag_pattern;
		g_event_flag[id].p_wait_task = gp_current_task;

		// set own task state to sleep for wait event flag
		update_task_chain(gp_current_task, SCHED_TASK_SLEEP, SCHED_TASK_WAIT_EVENT_FLAG);

		// go to sleep state to wait event flag
		// return from _dispatch after set event matching event flag
		_dispatch();   // cpu is unlocked in _dispatch
	}

	// get event flag if user set return buffer
	if (p_event_flag_pattern != NULL) {
		*p_event_flag_pattern = g_event_flag[id].event_flag;
	}

	_cpu_unlock();

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_WAI_FLG, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Polling event flag.
//
//! @param[in]  id                   : event flag id
//! @param[in]  wait_mode            : wait mode
//! @param[in]  wait_flag_pattern    : wait flag pattern
//! @param[out] p_event_flag_pattern : event flag pattern
//
//! @retval EVENT_FLAG_ERR_SUCCESS           : success
//! @retval EVENT_FLAG_ERR_ILLEGAL_ID        : not support event flag id
//! @retval EVENT_FLAG_ERR_ILLEGAL_WAIT_FLAG : illegal wait flag pattern (0x0000)
//! @retval EVENT_FLAG_ERR_NOT_MATCH         : not match event flag
//------------------------------------------------------------------------------
event_flag_errno_t poll_event_flag(u8 id, event_wait_mode_t wait_mode, u16 wait_flag_pattern, u16 *p_event_flag_pattern)
{
	event_flag_errno_t ret = EVENT_FLAG_ERR_SUCCESS;
	u32 irq_sts;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_POL_FLG, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_EVENT_FLAG) {
		ret = EVENT_FLAG_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	if (wait_flag_pattern == 0x0000) {
		// not support wait flag pattern 0x0000
		ret = EVENT_FLAG_ERR_ILLEGAL_WAIT_FLAG;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	disable_interrupt(&irq_sts);

	// check wait mode and wait flag match event flag
	if (is_agree_event_flag(wait_mode, wait_flag_pattern, g_event_flag[id].event_flag) == FALSE) {
		ret = EVENT_FLAG_ERR_NOT_MATCH;
	}

	// get event flag if user set return buffer
	if (p_event_flag_pattern != NULL) {
		*p_event_flag_pattern = g_event_flag[id].event_flag;
	}

	enable_interrupt(irq_sts);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_POL_FLG, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Set event flag.
//!          - wakeup wait event flag task
//
//! @param[in] id              : event flag id
//! @param[in] set_bit_pattern : set bit pattern
//
//! @retval EVENT_FLAG_ERR_SUCCESS    : success
//! @retval EVENT_FLAG_ERR_ILLEGAL_ID : not support event flag id
//------------------------------------------------------------------------------
event_flag_errno_t set_event_flag(u8 id, u16 set_bit_pattern)
{
	event_flag_errno_t ret = EVENT_FLAG_ERR_SUCCESS;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_SET_FLG, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_EVENT_FLAG) {
		ret = EVENT_FLAG_ERR_ILLEGAL_ID;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	_cpu_lock();

	// set event flag specific bit
	g_event_flag[id].event_flag |= set_bit_pattern;

	// check wait mode and wait flag match event flag
	if ((g_event_flag[id].p_wait_task != NULL)                   &&
	    (is_agree_event_flag(g_event_flag[id].wait_mode,
	                         g_event_flag[id].wait_flag,
	                         g_event_flag[id].event_flag) == TRUE)) {
		// release wait event flag task state to executable (from sleep)
		update_task_chain(g_event_flag[id].p_wait_task, SCHED_TASK_EXECUTABLE, SCHED_TASK_WAIT_EVENT_FLAG);
		g_event_flag[id].p_wait_task = NULL;

		// call _dispatch function when in task context mode
		if (_is_task_ctx_mode() == TRUE) {
			_dispatch();
		}
	}

	_cpu_unlock();

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_SET_FLG, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Clear event flag.
//
//! @param[in] id                : event flag id
//! @param[in] clear_bit_pattern : clear bit pattern
//
//! @retval EVENT_FLAG_ERR_SUCCESS    : success
//! @retval EVENT_FLAG_ERR_ILLEGAL_ID : not support event flag id
//------------------------------------------------------------------------------
event_flag_errno_t clear_event_flag(u8 id, u16 clear_bit_pattern)
{
	event_flag_errno_t ret = EVENT_FLAG_ERR_SUCCESS;
	u32 irq_sts;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_CLR_FLG, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_EVENT_FLAG) {
		ret = EVENT_FLAG_ERR_ILLEGAL_ID;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	disable_interrupt(&irq_sts);

	// clear event flag specific bit
	g_event_flag[id].event_flag &= ~clear_bit_pattern;

	enable_interrupt(irq_sts);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_CLR_FLG, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Reference event flag state.
//
//! @param[in]  id          : event flag id
//! @param[out] p_event_ref : event flag state
//
//! @retval EVENT_FLAG_ERR_SUCCESS    : success
//! @retval EVENT_FLAG_ERR_ILLEGAL_ID : not support event flag id
//! @retval EVENT_FLAG_ERR_NULL       : null pointer
//------------------------------------------------------------------------------
event_flag_errno_t refer_event_flag(u8 id, p_event_flag_ref_t p_event_ref)
{
	event_flag_errno_t ret = EVENT_FLAG_ERR_SUCCESS;
	u32 irq_sts;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_REF_FLG, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_EVENT_FLAG) {
		ret = EVENT_FLAG_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	if (p_event_ref == NULL) {
		ret = EVENT_FLAG_ERR_NULL;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	disable_interrupt(&irq_sts);

	// get event flag
	p_event_ref->event_flag = g_event_flag[id].event_flag;
	if (g_event_flag[id].p_wait_task != NULL) {
		p_event_ref->wait_status  = 1;
		p_event_ref->wait_task_id = get_task_id(g_event_flag[id].p_wait_task);
	}
	else {
		// not task wait event flag
		p_event_ref->wait_status = 0;
	}

	enable_interrupt(irq_sts);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_REF_FLG, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Check wait state agree event flag.
//
//! @param[in] wait_mode  : wait mode
//! @param[in] wait_flag  : wait flag
//! @param[in] event_flag : event flag
//
//! @retval TRUE  : agree event flag
//! @retval FALSE : disagree event flag
//------------------------------------------------------------------------------
static bool is_agree_event_flag(event_wait_mode_t wait_mode, u16 wait_flag, u16 event_flag)
{
	if (wait_mode == EVENT_FLAG_WAIT_OR) {
		if (wait_flag & event_flag)                return TRUE;
	}
	else { // wait_mode == EVENT_FLAG_WAIT_AND
		if (wait_flag == (wait_flag & event_flag)) return TRUE;
	}

	return FALSE;
}
#endif // (SCHED_TOTAL_EVENT_FLAG != 0)
