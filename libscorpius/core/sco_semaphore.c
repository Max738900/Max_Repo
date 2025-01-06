//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  This file provides the following functions of semaphore.
//!          - initial_semaphore()
//!          - get_semaphore()
//!          - poll_semaphore()
//!          - put_semaphore()
//!          - refer_semaphore()
//=====================================================================================

#include <sco_common.h>
#include <sco_config.h>
#include <sco_cpu.h>
#include <sco_scheduler.h>
#include <sco_scheduler_i.h>
#include <sco_semaphore.h>
#include <sco_trace_log.h>
#include <sco_trace_log_i.h>
#include <sco_trace_log_msg.h>

#if (SCHED_TOTAL_SEMAPHORE != 0)

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//! semaphore control structure
typedef struct semaphore {
	u8       sem_count;         //!< resource count of semaphore
	u8       sem_max_count;     //!< max resource count of semaphore
	u8       reserve[2];        //!< reserve
	p_task_t p_wait_task;       //!< task context address of wait semaphore top
} semaphore_t, *p_semaphore_t;

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------
extern const table_semaphore_entry_t g_table_semaphore_entry[];

//------------------------------------------------------------------------------
// internal function prototype
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
static semaphore_t g_semaphore[SCHED_TOTAL_SEMAPHORE];

//------------------------------------------------------------------------------
//! @brief Initial semaphore.
//
//! @retval SEMAPHORE_ERR_SUCCESS : success
//! @retval SEMAPHORE_ERR_INITIAL : initial fail
//------------------------------------------------------------------------------
semaphore_errno_t initial_semaphore(void)
{
	int i;

	// setup initial state of semaphore function
	for (i = 0; i < SCHED_TOTAL_SEMAPHORE; i++) {
		g_semaphore[i].sem_count     = g_table_semaphore_entry[i].initial_resource;
		g_semaphore[i].sem_max_count = g_table_semaphore_entry[i].max_resource;
		g_semaphore[i].p_wait_task   = NULL;

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
		if ((g_semaphore[i].sem_max_count == 0)                      ||
		    (g_semaphore[i].sem_count > g_semaphore[i].sem_max_count)) {
			return SEMAPHORE_ERR_INITIAL;
		}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)
	}

	return SEMAPHORE_ERR_SUCCESS;
}

//------------------------------------------------------------------------------
//! @brief Get semaphore.
//!          - get semaphore
//!          - added wait task in the wait semaphore chain
//
//! @param[in] id : semaphore id
//
//! @retval SEMAPHORE_ERR_SUCCESS         : success
//! @retval SEMAPHORE_ERR_ILLEGAL_ID      : not support semaphore id
//! @retval SEMAPHORE_ERR_ILLEGAL_CONTEXT : must be called in task context
//! @retval SEMAPHORE_ERR_IN_IDLE_TASK    : called in idle task
//------------------------------------------------------------------------------
semaphore_errno_t get_semaphore(u8 id)
{
	semaphore_errno_t ret = SEMAPHORE_ERR_SUCCESS;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_GET_SEM, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_SEMAPHORE) {
		ret = SEMAPHORE_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	if (_is_task_ctx_mode() == FALSE) {
		// this function must be called in task context
		ret = SEMAPHORE_ERR_ILLEGAL_CONTEXT;
		goto EXIT;
	}

	if (gp_current_task->p_next == NULL) {
		// this function can't be called in idle task
		ret = SEMAPHORE_ERR_IN_IDLE_TASK;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	_cpu_lock();

	// check semaphore resource
	if (g_semaphore[id].sem_count > 0) {
		// get semaphore
		g_semaphore[id].sem_count--;
		_cpu_unlock();
	}
	else {
		// set own task state to sleep for wait get semaphore
		update_task_chain(gp_current_task, SCHED_TASK_SLEEP, SCHED_TASK_WAIT_SEMAPHORE);

		// add wait task at the wait semaphore chain
		if (g_semaphore[id].p_wait_task == NULL) {
			// wait task is added at the top of the wait semaphore chain
			g_semaphore[id].p_wait_task = gp_current_task;
		}
		else {
			// added task to wait semaphore chain
			add_task_to_chain(gp_current_task, g_semaphore[id].p_wait_task, (void **)&g_semaphore[id].p_wait_task);
		}

		// go to sleep state to get semaphore
		// return from _dispatch after getting semaphore
		_dispatch();   // cpu is unlocked in _dispatch
	}

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_GET_SEM, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Polling semaphore.
//!          - get semaphore without wait state
//
//! @param[in] id : semaphore id
//
//! @retval SEMAPHORE_ERR_SUCCESS    : success
//! @retval SEMAPHORE_ERR_ILLEGAL_ID : not support semaphore id
//! @retval SEMAPHORE_ERR_EMPTY      : semaphore resource is empty
//------------------------------------------------------------------------------
semaphore_errno_t poll_semaphore(u8 id)
{
	semaphore_errno_t ret = SEMAPHORE_ERR_SUCCESS;
	u32 irq_sts;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_POL_SEM, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_SEMAPHORE) {
		ret = SEMAPHORE_ERR_ILLEGAL_ID;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	disable_interrupt(&irq_sts);

	// check semaphore resource
	if (g_semaphore[id].sem_count > 0) {
		// get semaphore
		g_semaphore[id].sem_count--;
	}
	else {
		ret = SEMAPHORE_ERR_EMPTY;
	}

	enable_interrupt(irq_sts);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_POL_SEM, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Put semaphore.
//!          - put semaphore
//!          - wakeup wait task in the wait semaphore chain
//
//! @param[in] id : semaphore id
//
//! @retval SEMAPHORE_ERR_SUCCESS    : success
//! @retval SEMAPHORE_ERR_ILLEGAL_ID : not support semaphore id
//! @retval SEMAPHORE_ERR_OVERFLOW   : put over for semaphore max resource count
//------------------------------------------------------------------------------
semaphore_errno_t put_semaphore(u8 id)
{
	semaphore_errno_t ret = SEMAPHORE_ERR_SUCCESS;
	p_task_t p_next_task;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_PUT_SEM, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_SEMAPHORE) {
		ret = SEMAPHORE_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	_cpu_lock();

	// check semaphore resource
	if (g_semaphore[id].sem_count >= g_semaphore[id].sem_max_count) {
		_cpu_unlock();
		ret = SEMAPHORE_ERR_OVERFLOW;
		goto EXIT;
	}
#else
	_cpu_lock();
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	// check wait semaphore task
	if (g_semaphore[id].p_wait_task != NULL) {
		// delete wait task in the top of wait semaphore chain
		p_next_task                 = g_semaphore[id].p_wait_task;
		g_semaphore[id].p_wait_task = p_next_task->p_next;
		p_next_task->p_next         = NULL;

		// release wait semaphore task state to executable (from sleep)
		update_task_chain(p_next_task, SCHED_TASK_EXECUTABLE, SCHED_TASK_WAIT_SEMAPHORE);

		// call _dispatch function when in task context mode
		if (_is_task_ctx_mode() == TRUE) {
			_dispatch();
		}
	}
	else {
		// no task wait semaphore
		g_semaphore[id].sem_count++;
	}

	_cpu_unlock();

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_PUT_SEM, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Reference semaphore state.
//
//! @param[in]  id        : semaphore id
//! @param[out] p_sem_ref : semaphore state
//
//! @retval SEMAPHORE_ERR_SUCCESS    : success
//! @retval SEMAPHORE_ERR_ILLEGAL_ID : not support semaphore id
//! @retval SEMAPHORE_ERR_NULL       : null pointer
//------------------------------------------------------------------------------
semaphore_errno_t refer_semaphore(u8 id, p_semaphore_ref_t p_sem_ref)
{
	semaphore_errno_t ret = SEMAPHORE_ERR_SUCCESS;
	u32 i, irq_sts;
	p_task_t p_next_task;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_REF_SEM, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_SEMAPHORE) {
		ret = SEMAPHORE_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	if (p_sem_ref == NULL) {
		ret = SEMAPHORE_ERR_NULL;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	disable_interrupt(&irq_sts);

	// get semaphore state
	p_sem_ref->sem_count     = g_semaphore[id].sem_count;
	p_sem_ref->sem_max_count = g_semaphore[id].sem_max_count;
	// check wait semaphore task
	p_next_task = g_semaphore[id].p_wait_task;
	if (p_next_task != NULL) {
		p_sem_ref->top_task_id = get_task_id(p_next_task);
		// get task count of wait semaphore
		for (i = 1; i <= SCHED_TOTAL_TASK; i++) {
			if (p_next_task->p_next == NULL) {
				p_sem_ref->wait_task_count = i;
				break;
			}

			p_next_task = p_next_task->p_next;
		}
	}
	else {
		// not task wait semaphore
		p_sem_ref->wait_task_count = 0;
	}

	enable_interrupt(irq_sts);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_REF_SEM, ret, 0, 0);
	return ret;
}
#endif // (SCHED_TOTAL_SEMAPHORE != 0)
