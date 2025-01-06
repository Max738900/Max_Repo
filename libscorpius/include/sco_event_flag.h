//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of event flag function.
//=====================================================================================

#if !defined(SCO_EVENT_FLAG_H)
#define SCO_EVENT_FLAG_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//! error number of event flag function
typedef enum event_flag_errno {
	EVENT_FLAG_ERR_NULL = SCHED_FLG_ERR_NUM, //!< : null pointer
	EVENT_FLAG_ERR_ILLEGAL_ID,               //!< : not support event flag id
	EVENT_FLAG_ERR_ILLEGAL_CONTEXT,          //!< : must be called in task context
	EVENT_FLAG_ERR_IN_IDLE_TASK,             //!< : called in idle task
	EVENT_FLAG_ERR_ALREADY_WAIT_TASK,        //!< : other task already wait event flag
	EVENT_FLAG_ERR_NOT_MATCH,                //!< : not match event flag
	EVENT_FLAG_ERR_ILLEGAL_WAIT_FLAG,        //!< : illegal wait flag pattern (0x0000)
	EVENT_FLAG_ERR_SUCCESS = 0               //!< : success
} event_flag_errno_t, *p_event_flag_errno_t;

//! event flag wait mode
typedef enum event_flag_wait_mode {
	EVENT_FLAG_WAIT_OR = 0,                  //!< : wait bit OR mode
	EVENT_FLAG_WAIT_AND                      //!< : wait bit AND mode
} event_wait_mode_t, *p_event_wait_mode_t;

//! event flag reference structure
typedef struct event_flag_reference {
	u16 event_flag;                          //!< event flag pattern
	u8  wait_status;                         //!< wait status of task
	u8  wait_task_id;                        //!< task id of wait event flag
} event_flag_ref_t, *p_event_flag_ref_t;

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
event_flag_errno_t wait_event_flag(u8 id, event_wait_mode_t wait_mode, u16 wait_flag_pattern, u16 *p_event_flag_pattern);
event_flag_errno_t poll_event_flag(u8 id, event_wait_mode_t wait_mode, u16 wait_flag_pattern, u16 *p_event_flag_pattern);
event_flag_errno_t set_event_flag(u8 id, u16 set_bit_pattern);
event_flag_errno_t clear_event_flag(u8 id, u16 clear_bit_pattern);
event_flag_errno_t refer_event_flag(u8 id, p_event_flag_ref_t p_event_ref);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_EVENT_FLAG_H)
