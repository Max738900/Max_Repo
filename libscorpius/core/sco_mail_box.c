//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  This file provides the following functions of mail box.
//!          - initial_mail_box()
//!          - receive_mail_box()
//!          - poll_mail_box()
//!          - send_mail_box()
//!          - refer_mail_box()
//=====================================================================================

#include <sco_common.h>
#include <sco_config.h>
#include <sco_cpu.h>
#include <sco_scheduler.h>
#include <sco_scheduler_i.h>
#include <sco_mail_box.h>
#include <sco_trace_log.h>
#include <sco_trace_log_i.h>
#include <sco_trace_log_msg.h>

#if (SCHED_TOTAL_MAIL_BOX != 0)

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//! mail box control structure
typedef struct mail_box {
	p_mail_t p_queue_mail;      //!< mail address of queueing top
	p_task_t p_wait_task;       //!< task context address of wait receive mail
} mail_box_t, *p_mail_box_t;

//------------------------------------------------------------------------------
// internal function prototype
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
static mail_box_t g_mail_box[SCHED_TOTAL_MAIL_BOX];

//------------------------------------------------------------------------------
//! @brief Initial mail box.
//
//! @return None
//------------------------------------------------------------------------------
void initial_mail_box(void)
{
	int i;

	// setup initial state of mail box function
	for (i = 0; i < SCHED_TOTAL_MAIL_BOX; i++) {
		g_mail_box[i].p_queue_mail = NULL;
		g_mail_box[i].p_wait_task  = NULL;
	}
}

//------------------------------------------------------------------------------
//! @brief Receive mail from mail box.
//
//! @param[in]  id          : mail box id
//! @param[out] pp_rcv_mail : receive mail address
//
//! @retval MAIL_BOX_ERR_SUCCESS           : success
//! @retval MAIL_BOX_ERR_ILLEGAL_ID        : not support mail box id
//! @retval MAIL_BOX_ERR_ILLEGAL_CONTEXT   : must be called in task context
//! @retval MAIL_BOX_ERR_IN_IDLE_TASK      : called in idle task
//! @retval MAIL_BOX_ERR_NULL              : null pointer
//! @retval MAIL_BOX_ERR_ALREADY_WAIT_TASK : other task already wait receive mail
//------------------------------------------------------------------------------
mail_box_errno_t receive_mail_box(u8 id, void **pp_rcv_mail)
{
	mail_box_errno_t ret = MAIL_BOX_ERR_SUCCESS;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_RCV_MBX, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_MAIL_BOX) {
		ret = MAIL_BOX_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	if (_is_task_ctx_mode() == FALSE) {
		// this function must be called in task context
		ret = MAIL_BOX_ERR_ILLEGAL_CONTEXT;
		goto EXIT;
	}

	if (gp_current_task->p_next == NULL) {
		// this function can't be called in idle task
		ret = MAIL_BOX_ERR_IN_IDLE_TASK;
		goto EXIT;
	}

	if (pp_rcv_mail == NULL) {
		ret = MAIL_BOX_ERR_NULL;
		goto EXIT;
	}

	_cpu_lock();

	// check other task wait receive mail already
	if (g_mail_box[id].p_wait_task != NULL) {
		_cpu_unlock();
		ret = MAIL_BOX_ERR_ALREADY_WAIT_TASK;
		goto EXIT;
	}
#else
	_cpu_lock();
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	// check queueing mail
	if (g_mail_box[id].p_queue_mail == NULL) {
		// set own task state to sleep for wait receive mail
		g_mail_box[id].p_wait_task = gp_current_task;
		update_task_chain(gp_current_task, SCHED_TASK_SLEEP, SCHED_TASK_WAIT_MAIL_BOX);

		// go to sleep state to receive mail
		// return from _dispatch after queueing mail in mail box
		_dispatch();   // cpu is unlocked in _dispatch

		_cpu_lock();
		g_mail_box[id].p_wait_task = NULL;
	}

	// receive mail from mail queue of top
	*pp_rcv_mail = (void *)g_mail_box[id].p_queue_mail;
	g_mail_box[id].p_queue_mail = g_mail_box[id].p_queue_mail->p_next_queue_mail;

	_cpu_unlock();

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_RCV_MBX, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Polling mail.
//
//! @param[in]  id          : mail box id
//! @param[out] pp_rcv_mail : receive mail address
//
//! @retval MAIL_BOX_ERR_SUCCESS           : success
//! @retval MAIL_BOX_ERR_ILLEGAL_ID        : not support mail box id
//! @retval MAIL_BOX_ERR_NULL              : null pointer
//! @retval MAIL_BOX_ERR_ALREADY_WAIT_TASK : other task already wait receive mail
//! @retval MAIL_BOX_ERR_EMPTY             : no mail stock in mail box
//------------------------------------------------------------------------------
mail_box_errno_t poll_mail_box(u8 id, void **pp_rcv_mail)
{
	mail_box_errno_t ret = MAIL_BOX_ERR_SUCCESS;
	u32 irq_sts;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_POL_MBX, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_MAIL_BOX) {
		ret = MAIL_BOX_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	if (pp_rcv_mail == NULL) {
		ret = MAIL_BOX_ERR_NULL;
		goto EXIT;
	}

	disable_interrupt(&irq_sts);

	// check other task wait receive mail already
	if (g_mail_box[id].p_wait_task != NULL) {
		enable_interrupt(irq_sts);
		ret = MAIL_BOX_ERR_ALREADY_WAIT_TASK;
		goto EXIT;
	}
#else
	disable_interrupt(&irq_sts);
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	// check queueing mail
	if (g_mail_box[id].p_queue_mail == NULL) {
		ret = MAIL_BOX_ERR_EMPTY;
	}
	else {
		// receive mail from mail queue of top
		*pp_rcv_mail = (void *)g_mail_box[id].p_queue_mail;
		g_mail_box[id].p_queue_mail = g_mail_box[id].p_queue_mail->p_next_queue_mail;
	}

	enable_interrupt(irq_sts);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_POL_MBX, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Send mail to mail box.
//!          - wakeup wait receive mail task
//!          - added send mail at the mail queue
//
//! @param[in] id         : mail box id
//! @param[in] type       : send mail type
//! @param[in] p_snd_mail : send mail address
//
//! @retval MAIL_BOX_ERR_SUCCESS    : success
//! @retval MAIL_BOX_ERR_ILLEGAL_ID : not support mail box id
//! @retval MAIL_BOX_ERR_NULL       : null pointer
//------------------------------------------------------------------------------
mail_box_errno_t send_mail_box(u8 id, mail_type_t type, void *p_snd_mail)
{
	mail_box_errno_t ret = MAIL_BOX_ERR_SUCCESS;
	p_mail_t p_next_mail;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_SND_MBX, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_MAIL_BOX) {
		ret = MAIL_BOX_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	if (p_snd_mail == NULL) {
		ret = MAIL_BOX_ERR_NULL;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	_cpu_lock();

	// queueing mail to mail box
	p_next_mail = g_mail_box[id].p_queue_mail;
	// check send mail type and stock mail in mail queue
	if ((p_next_mail == NULL) || (type == SEND_MAIL_TYPE_EMERGENCY)) {
		// stock mail at the top of the mail queue
		// when mail type is emergency or not stock mail in mail queue
		g_mail_box[id].p_queue_mail = (p_mail_t)p_snd_mail;
		g_mail_box[id].p_queue_mail->p_next_queue_mail = p_next_mail;
	}
	else {
		// stock mail at the last of the mail queue
		while (TRUE) {
			if (p_next_mail->p_next_queue_mail == NULL) {
				p_next_mail->p_next_queue_mail = (p_mail_t)p_snd_mail;
				p_next_mail->p_next_queue_mail->p_next_queue_mail = NULL;
				break;
			}

			p_next_mail = p_next_mail->p_next_queue_mail;
		}
	}

	// check wait receive mail task
	if ((g_mail_box[id].p_wait_task != NULL)                   &&
	    (g_mail_box[id].p_queue_mail->p_next_queue_mail == NULL)) {
		// release wait receive mail task state to executable (from sleep)
		update_task_chain(g_mail_box[id].p_wait_task, SCHED_TASK_EXECUTABLE, SCHED_TASK_WAIT_MAIL_BOX);

		// call _dispatch function when in task context mode
		if (_is_task_ctx_mode() == TRUE) {
			_dispatch();
		}
	}

	_cpu_unlock();

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_SND_MBX, ret, 0, 0);
	return ret;
}

//------------------------------------------------------------------------------
//! @brief Reference mail box state.
//
//! @param[in]  id         : mail box id
//! @param[out] p_mail_ref : mail box state
//
//! @retval MAIL_BOX_ERR_SUCCESS    : success
//! @retval MAIL_BOX_ERR_ILLEGAL_ID : not support mail box id
//! @retval MAIL_BOX_ERR_NULL       : null pointer
//------------------------------------------------------------------------------
mail_box_errno_t refer_mail_box(u8 id, p_mail_box_ref_t p_mail_ref)
{
	mail_box_errno_t ret = MAIL_BOX_ERR_SUCCESS;
	u32 i, irq_sts;
	p_mail_t p_next_mail;

	SET_TC_LOG_SVC(TC_LOG_MSG_ENT_REF_MBX, id, 0, 0);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
	if (id >= SCHED_TOTAL_MAIL_BOX) {
		ret = MAIL_BOX_ERR_ILLEGAL_ID;
		goto EXIT;
	}

	if (p_mail_ref == NULL) {
		ret = MAIL_BOX_ERR_NULL;
		goto EXIT;
	}
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	disable_interrupt(&irq_sts);

	// get mail box state
	if (g_mail_box[id].p_wait_task != NULL) {
		p_mail_ref->wait_status  = 1;
		p_mail_ref->wait_task_id = get_task_id(g_mail_box[id].p_wait_task);
	}
	else {
		// not task wait receive mail
		p_mail_ref->wait_status = 0;
	}

	// check queueing mail
	p_next_mail = g_mail_box[id].p_queue_mail;
	if (p_next_mail != NULL) {
		p_mail_ref->p_mail = p_next_mail;
		// get queueing mail count
		for (i = 1; ; i++) {
			if (p_next_mail->p_next_queue_mail == NULL) {
				p_mail_ref->stock_mail_count = i;
				break;
			}

			p_next_mail = p_next_mail->p_next_queue_mail;
		}
	}
	else {
		// not stock mail in mail box
		p_mail_ref->stock_mail_count = 0;
	}

	enable_interrupt(irq_sts);

#if defined(SCHED_SUPPORT_DEVELOP_CHECK)
EXIT:
#endif // defined(SCHED_SUPPORT_DEVELOP_CHECK)

	SET_TC_LOG_SVC(TC_LOG_MSG_EXT_REF_MBX, ret, 0, 0);
	return ret;
}
#endif // (SCHED_TOTAL_MAIL_BOX != 0)
