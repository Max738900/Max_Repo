//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of semaphore function.
//=====================================================================================

#if !defined(SCO_SEMAPHORE_H)
#define SCO_SEMAPHORE_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//! error number of semaphore function
typedef enum semaphore_errno {
	SEMAPHORE_ERR_NULL = SCHED_SEM_ERR_NUM, //!< : null pointer
	SEMAPHORE_ERR_ILLEGAL_ID ,              //!< : not support semaphore id
	SEMAPHORE_ERR_ILLEGAL_CONTEXT,          //!< : must be called in task context
	SEMAPHORE_ERR_IN_IDLE_TASK,             //!< : called in idle task
	SEMAPHORE_ERR_EMPTY,                    //!< : semaphore resource is empty
	SEMAPHORE_ERR_OVERFLOW,                 //!< : put over for semaphore max resource count
	SEMAPHORE_ERR_INITIAL,                  //!< : initial fail
	SEMAPHORE_ERR_SUCCESS = 0               //!< : success
} semaphore_errno_t, *p_semaphore_errno_t;

//! semaphore reference structure
typedef struct semaphore_reference {
	u8 sem_count;                           //!< resource count semaphore
	u8 sem_max_count;                       //!< max resource count semaphore
	u8 wait_task_count;                     //!< task count of wait semaphore
	u8 top_task_id;                         //!< task id of wait semaphore top
} semaphore_ref_t, *p_semaphore_ref_t;

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
semaphore_errno_t get_semaphore(u8 id);
semaphore_errno_t poll_semaphore(u8 id);
semaphore_errno_t put_semaphore(u8 id);
semaphore_errno_t refer_semaphore(u8 id, p_semaphore_ref_t p_sem_ref);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_SEMAPHORE_H)
