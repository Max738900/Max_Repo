//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of scheduler core function.
//=====================================================================================

#if !defined(SCO_SCHEDULER_H)
#define SCO_SCHEDULER_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
// define schedule core function
#define SCHEDULER_VERSION        "1.0"
#define enable_interrupt(irq)  _enable_interrupt(irq)
#if defined(SCHED_SUPPORT_INTERRUPT)
#define enter_interrupt()      _enter_interrupt()
#define exit_interrupt()       _exit_interrupt()
#else
#define enter_interrupt()
#define exit_interrupt()
#endif // defined(SCHED_SUPPORT_INTERRUPT)

// error number of scorpius function
#define SCHED_BASE_ERR_NUM  -256                     //!< error number of scorpius base
#define SCHED_ERR_NUM       SCHED_BASE_ERR_NUM       //!< error number of scheduler core function
#define SCHED_TSK_ERR_NUM   (SCHED_ERR_NUM     + 16) //!< error number of task           function
#define SCHED_SEM_ERR_NUM   (SCHED_TSK_ERR_NUM + 16) //!< error number of semaphore      function
#define SCHED_FLG_ERR_NUM   (SCHED_SEM_ERR_NUM + 16) //!< error number of event flag     function
#define SCHED_MBX_ERR_NUM   (SCHED_FLG_ERR_NUM + 16) //!< error number of mail box       function
#define SCHED_LOG_ERR_NUM   (SCHED_MBX_ERR_NUM + 16) //!< error number of trace log      function

//! scheduler core function error number
typedef enum sched_errno {
	SCHED_ERR_NULL = SCHED_ERR_NUM,        //!< : null pointer
	SCHED_ERR_ILLEGAL_TASK,                //!< : illegal task context or entry point (for get task id)
	SCHED_ERR_SUCCESS = 0                  //!< : success
} sched_errno_t, *p_sched_errno_t;

//! task status
typedef enum task_status {
	SCHED_TASK_SLEEP = 0,                 //!< : task is sleep
	SCHED_TASK_EXECUTABLE                 //!< : task is executable or running
} task_status_t, *p_task_status_t;

//! task wait type
typedef enum task_wait_type {
	SCHED_TASK_WAIT_NOT_SERVICE = 0,      //!< : task wait in task             function
	SCHED_TASK_WAIT_SEMAPHORE,            //!< : task wait in get semaphore    function
	SCHED_TASK_WAIT_EVENT_FLAG,           //!< : task wait in wait event flag  function
	SCHED_TASK_WAIT_MAIL_BOX              //!< : task wait in receive mail box function
} task_wait_type_t, *p_task_wait_type_t;

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
void          start_scheduler(void);
sched_errno_t get_task_id_in_entry(void *p_task_entry, u8 *p_id);
sched_errno_t disable_interrupt(u32 *p_irq);
void          _enable_interrupt(u32 irq);
void          _enter_interrupt(void);
void          _exit_interrupt(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_SCHEDULER_H)
