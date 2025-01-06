//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of task function.
//=====================================================================================

#if !defined(SCO_TASK_H)
#define SCO_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//! error number of task function
typedef enum task_errno {
	TASK_ERR_NULL = SCHED_TSK_ERR_NUM,    //!< : null pointer
	TASK_ERR_TASK_ID,                     //!< : illegal task id
	TASK_ERR_ILLEGAL_CONTEXT,             //!< : called from interrupt context (non user mode)
	TASK_ERR_TASK_IN_IDLE_TASK,           //!< : can't sleep (multi executalbe tasks are not chained)
	TASK_ERR_TASK_WAIT_SERVICE,           //!< : task is wait in service
	TASK_ERR_TASK_EXECUTABLE_ALREADY = 1, //!< : task already executable
	TASK_ERR_SUCCESS = 0                  //!< : success
} task_errno_t, *p_task_errno_t;

//! task reference structure
typedef struct task_reference {
	u8 status;                            //!< task status(sleep or executable)
	u8 priority;                          //!< higher value is higher priority
	u8 wakeup_queue;                      //!< set by wakeup_task(), clear by sleep_task()
	u8 wait_type;                         //!< task wait type in sleep state
} task_ref_t, *p_task_ref_t;

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
task_errno_t wakeup_task(u8 id);
task_errno_t sleep_task(void);
task_errno_t refer_task(u8 id, p_task_ref_t p_task_ref);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_TASK_H)
