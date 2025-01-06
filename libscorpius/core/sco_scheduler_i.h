//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of scheduler core internal function.
//=====================================================================================

#if !defined(SCO_SCHEDULER_I_H)
#define SCO_SCHEDULER_I_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
#pragma pack(1)
//! task context
typedef struct task {
	cpu_regs_t    context;      //!< task context (cpu register)
	u8            status;       //!< task status(sleep or executable)
	u8            priority;     //!< higher value is higher priority
	u8            wakeup_queue; //!< set by wakeup_task(), clear by sleep_task()
	u8            wait_type;    //!< task wait type in sleep state
	struct task * p_next;       //!< pointer to next priority executable task
} task_t, *p_task_t;
#pragma pack()

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------
extern p_task_t gp_current_task;
extern p_task_t gp_top_priority_task;
extern task_t   g_task[];

//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
void update_task_chain(p_task_t p_update_task, task_status_t status, task_wait_type_t type);
void add_task_to_chain(p_task_t p_add_task, p_task_t p_chain_target, void **pp_update);
int  get_task_id(p_task_t p_task_ctx);
void _dispatch(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_SCHEDULER_I_H)
