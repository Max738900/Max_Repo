//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  This file provides the following functions of scheduler.
//!          - start_scheduler()
//!          - update_task_chain()
//!          - add_task_to_chain()
//!          - get_task_id()
//!          - get_task_id_in_entry()
//!          - disable_interrupt()
//=====================================================================================

#include <sco_common.h>
#include <sco_config.h>
#include <sco_config.entry>
#include <sco_cpu.h>
#include <sco_scheduler.h>
#include <sco_scheduler_i.h>
#include <sco_semaphore.h>
#include <sco_semaphore_i.h>
#include <sco_event_flag_i.h>
#include <sco_mail_box_i.h>
#include <sco_trace_log.h>
#include <sco_trace_log_i.h>
#include <sco_trace_log_msg.h>

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------
extern void _start_scheduler(void);
extern const table_task_entry_t g_table_task_entry[];

//------------------------------------------------------------------------------
// internal function prototype
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
p_task_t gp_current_task;           //!< points to running task
p_task_t gp_top_priority_task;      //!< points to switchable task, normally
                                    //!< null and set if higher priority task is executable
task_t   g_task[SCHED_TOTAL_TASK];  //!< task context array
#if defined(SCHED_SUPPORT_INTERRUPT)
int      g_interrupt_count;         //!< nested interrupt count
#endif // defined(SCHED_SUPPORT_INTERRUPT)

//------------------------------------------------------------------------------
//! @brief Start scheduler function.
//!          - initialize configuration for start scheduler
//!          - call _start_scheduler() to start scheduler without return
//
//! @return None (scheduler will be started)
//------------------------------------------------------------------------------
void start_scheduler(void)
{
	int i;

#if (TC_LOG_RECORD_NUM != 0)
	initial_trace_log();
#endif // (TC_LOG_RECORD_NUM != 0)

#if defined(SCHED_SUPPORT_INTERRUPT)
	g_interrupt_count = 0;
#endif // defined(SCHED_SUPPORT_INTERRUPT)

	// initialize task configuration
	gp_current_task      = NULL;
	gp_top_priority_task = NULL;
	for (i = 0; i < SCHED_TOTAL_TASK; i++) {
#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
		if ((g_table_task_entry[i].f_task == NULL) || (g_table_task_entry[i].p_stack == NULL)) {
			goto EXIT;
		}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

		// setup initial state of task context (default status is executable)
		g_task[i].status       = (u8)SCHED_TASK_EXECUTABLE;
		g_task[i].priority     = g_table_task_entry[i].priority;
		g_task[i].wakeup_queue = 0;
		g_task[i].wait_type    = SCHED_TASK_WAIT_NOT_SERVICE;
		g_task[i].p_next       = NULL;
		cpu_context(&(g_task[i].context),
		            g_table_task_entry[i].p_stack,
		            g_table_task_entry[i].f_task);

		// setup initial task chain
		if (gp_current_task == NULL) {
			// first task
			gp_current_task = &g_task[i];
		}
		else {
			// second or later task
			update_task_chain(&g_task[i], SCHED_TASK_EXECUTABLE, SCHED_TASK_WAIT_NOT_SERVICE);

			// gp_currnet_task should point highest task and
			// gp_top_priority_task should point NULL when initial task chain
			if (gp_top_priority_task) {
				gp_current_task      = gp_top_priority_task;
				gp_top_priority_task = NULL;
			}
		}
	}

#if (SCHED_TOTAL_SEMAPHORE != 0)
	// initialize semaphore configuration
	if (initial_semaphore() != SEMAPHORE_ERR_SUCCESS) goto EXIT;
#endif // (SCHED_TOTAL_SEMAPHORE != 0)

#if (SCHED_TOTAL_EVENT_FLAG != 0)
	// initialize semaphore configuration
	initial_event_flag();
#endif // (SCHED_TOTAL_EVENT_FLAG != 0)

#if (SCHED_TOTAL_MAIL_BOX != 0)
	// initialize mail box configuration
	initial_mail_box();
#endif // (SCHED_TOTAL_MAIL_BOX != 0)

	_start_scheduler();

#if defined(SCHED_SUPPORT_DEVELOP_CHECK) || (SCHED_TOTAL_SEMAPHORE != 0)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK) || (SCHED_TOTAL_SEMAPHORE != 0)

	// initialize configuration fail
	return;
}

//------------------------------------------------------------------------------
//! @brief Update task chain.
//!           - remove sleep task from chain
//!           - connect executable status task to chain
//!           - function must be call with disable interrupt
//
//! @param[in,out] p_update_task : task context address
//! @param[in]     status        : task status
//! @param[in]     type          : task wait type
//
//! @return None
//------------------------------------------------------------------------------
void update_task_chain(p_task_t p_update_task, task_status_t status, task_wait_type_t type)
{
	p_update_task->status = status;

	if (status == SCHED_TASK_SLEEP) { // switch to sleep state
		p_update_task->wait_type = type;
		gp_top_priority_task     = p_update_task->p_next;
		p_update_task->p_next    = NULL;

		SET_TC_LOG(TC_LOG_MSG_SLP_TSK, get_task_id(p_update_task), 0, 0);
	}
	else {                            // switch to executable state
		p_update_task->wait_type = SCHED_TASK_WAIT_NOT_SERVICE;

		// get task context at the top of the chain and added update task to chain
		add_task_to_chain(p_update_task,
		                  gp_top_priority_task != NULL ? gp_top_priority_task:
		                                                 gp_current_task,
		                  (void **)&gp_top_priority_task);

		SET_TC_LOG(TC_LOG_MSG_EXC_TSK, get_task_id(p_update_task), 0, 0);
	}
}

//------------------------------------------------------------------------------
//! @brief Added task to chain.
//!           - function must be call with disable interrupt
//
//! @param[in,out] p_add_task     : add task context address
//! @param[in,out] p_chain_target : chain top task context address
//! @param[out]    pp_update      : update contents address
//
//! @return None
//------------------------------------------------------------------------------
void add_task_to_chain(p_task_t p_add_task, p_task_t p_chain_target, void **pp_update)
{
	p_task_t p_chain_prev;

	if (p_chain_target->priority < p_add_task->priority) { // subject task is added at the top of the chain
		p_add_task->p_next = p_chain_target;
		*pp_update         = p_add_task;
	}
	else {                                                 // subject task is added after the second element of the chain
		while (TRUE) {
			p_chain_prev   = p_chain_target;
			p_chain_target = p_chain_target->p_next;

			if ((p_chain_target == NULL)                         ||    // added at the last of the chain
			    (p_chain_target->priority < p_add_task->priority)  ) { // added in the middle of the chain
				// subject task is added at the chain
				p_add_task->p_next   = p_chain_target;
				p_chain_prev->p_next = p_add_task;
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
//! @brief Get task id. (get id from task context address)
//
//! @param[in] p_task_ctx : task context address
//
//! @retval 0<=Values              : task id
//! @retval SCHED_ERR_ILLEGAL_TASK : illegal task context
//------------------------------------------------------------------------------
int get_task_id(p_task_t p_task_ctx)
{
	int i;

	// search task
	for (i = 0; i < SCHED_TOTAL_TASK; i++) {
		if (p_task_ctx == &g_task[i]) return i;   // get task id
	}

	return SCHED_ERR_ILLEGAL_TASK;
}

//------------------------------------------------------------------------------
//! @brief Get task id. (get id from task entry point)
//
//! @param[in]  p_task_entry : task entry point
//! @param[out] p_id         : task id
//
//! @retval SCHED_ERR_SUCCESS      : success
//! @retval SCHED_ERR_ILLEGAL_TASK : illegal task entry point
//------------------------------------------------------------------------------
sched_errno_t get_task_id_in_entry(void *p_task_entry, u8 *p_id)
{
	u8 i;

	if (p_task_entry == NULL) {
		*p_id = (u8)get_task_id(gp_current_task);
		return SCHED_ERR_SUCCESS;
	}

	// search task
	for (i = 0; i < SCHED_TOTAL_TASK; i++) {
		if (p_task_entry == g_table_task_entry[i].f_task) {
			*p_id = i;
			return SCHED_ERR_SUCCESS;
		}
	}

	return SCHED_ERR_ILLEGAL_TASK;
}

//------------------------------------------------------------------------------
//! @brief Disable interrupt.
//
//! @param[out] p_irq : interrupt status
//
//! @retval SCHED_ERR_SUCCESS : success
//! @retval SCHED_ERR_NULL    : null pointer
//------------------------------------------------------------------------------
sched_errno_t disable_interrupt(u32 *p_irq)
{
#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (p_irq == NULL) return SCHED_ERR_NULL;
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	*p_irq = _cpu_lock();

	return SCHED_ERR_SUCCESS;
}
