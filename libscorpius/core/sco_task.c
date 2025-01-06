//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  This file provides the following functions of task.
//!          - wakeup_task()
//!          - sleep_task()
//!          - refer_task()
//=====================================================================================

#include <sco_common.h>
#include <sco_config.h>
#include <sco_cpu.h>
#include <sco_scheduler.h>
#include <sco_scheduler_i.h>
#include <sco_task.h>
#include <sco_trace_log.h>
#include <sco_trace_log_i.h>
#include <sco_trace_log_msg.h>

#if defined(SCHED_SUPPORT_TASK)

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// internal function prototype
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//! @brief Wakeup task specified by id.
//
//! @param[in] id : task id
//
//! @retval TASK_ERR_SUCCESS                 : success
//! @retval TASK_ERR_TASK_ID                 : illegal task id
//! @retval TASK_ERR_TASK_EXECUTABLE_ALREADY : task already executable
//! @retval TASK_ERR_TASK_WAIT_SERVICE       : task is wait in service
//------------------------------------------------------------------------------
task_errno_t wakeup_task(u8 id)
{
	task_errno_t ret = TASK_ERR_SUCCESS;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_WAK_TSK, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_TASK) {
		ret = TASK_ERR_TASK_ID;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	_cpu_lock();

	if (g_task[id].status == SCHED_TASK_EXECUTABLE) {
		g_task[id].wakeup_queue = 1;
		ret = TASK_ERR_TASK_EXECUTABLE_ALREADY;
	}
	else if (g_task[id].wait_type != SCHED_TASK_WAIT_NOT_SERVICE) {
		g_task[id].wakeup_queue = 1;
		ret = TASK_ERR_TASK_WAIT_SERVICE;
	}
	else {
		update_task_chain(&g_task[id], SCHED_TASK_EXECUTABLE, SCHED_TASK_WAIT_NOT_SERVICE);

		// call _dispatch function when in task context mode
		if (_is_task_ctx_mode() == TRUE) {
			_dispatch();
		}
	}

	_cpu_unlock();

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_WAK_TSK, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Sleep own task.
//
//! @retval TASK_ERR_SUCCESS                 : success (sleeped, and then woken up)
//! @retval TASK_ERR_ILLEGAL_CONTEXT         : must be called in task context
//! @retval TASK_ERR_TASK_IN_IDLE_TASK       : can't sleep (multi executalbe tasks are not chained)
//! @retval TASK_ERR_TASK_EXECUTABLE_ALREADY : task already executable
//------------------------------------------------------------------------------
task_errno_t sleep_task(void)
{
	task_errno_t ret = TASK_ERR_SUCCESS;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_SLP_TSK, 0, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (_is_task_ctx_mode() == FALSE) {
		// this function must be called in task context
		ret = TASK_ERR_ILLEGAL_CONTEXT;
		goto EXIT;
	}

	// check if multi executalbe tasks are chained, if not, return error condition
	if (gp_current_task->p_next == NULL) {
		ret = TASK_ERR_TASK_IN_IDLE_TASK;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	_cpu_lock();

	if (gp_current_task->wakeup_queue != 0) {
		// wakeup event is queued, do not sleep and just return
		gp_current_task->wakeup_queue = 0;
		ret = TASK_ERR_TASK_EXECUTABLE_ALREADY;
	}
	else {
		update_task_chain(gp_current_task, SCHED_TASK_SLEEP, SCHED_TASK_WAIT_NOT_SERVICE);
		_dispatch();
	}

	_cpu_unlock();

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_SLP_TSK, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Reference task state.
//
//! @param[in]  id         : task id
//! @param[out] p_task_ref : task state
//
//! @retval TASK_ERR_SUCCESS : success
//! @retval TASK_ERR_TASK_ID : not support task id
//! @retval TASK_ERR_NULL    : null pointer
//------------------------------------------------------------------------------
task_errno_t refer_task(u8 id, p_task_ref_t p_task_ref)
{
	task_errno_t ret = TASK_ERR_SUCCESS;
	u32 irq_sts;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_REF_TSK, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_TASK) {
		ret = TASK_ERR_TASK_ID;
		goto EXIT;
	}

	if (p_task_ref == NULL) {
		ret = TASK_ERR_NULL;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	disable_interrupt(&irq_sts);

	// get task state
	p_task_ref->status       = g_task[id].status;
	p_task_ref->priority     = g_task[id].priority;
	p_task_ref->wakeup_queue = g_task[id].wakeup_queue;
	p_task_ref->wait_type    = g_task[id].wait_type;

	enable_interrupt(irq_sts);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_REF_TSK, ret, 0, 0);
	return ret;
}
#endif // defined(SCHED_SUPPORT_TASK)
