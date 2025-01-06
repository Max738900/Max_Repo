//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of mail box function.
//=====================================================================================

#if !defined(SCO_MAIL_BOX_H)
#define SCO_MAIL_BOX_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
// define mail box function
#define MAIL_BOX_HEADER_SIZE       sizeof(mail_t)

//! error number of mail box function
typedef enum mail_box_errno {
	MAIL_BOX_ERR_NULL = SCHED_MBX_ERR_NUM, //!< : null pointer
	MAIL_BOX_ERR_ILLEGAL_ID,               //!< : not support mail box id
	MAIL_BOX_ERR_ILLEGAL_CONTEXT,          //!< : must be called in task context
	MAIL_BOX_ERR_IN_IDLE_TASK,             //!< : called in idle task
	MAIL_BOX_ERR_ALREADY_WAIT_TASK,        //!< : other task already wait receive mail
	MAIL_BOX_ERR_EMPTY,                    //!< : no mail stock in mail box
	MAIL_BOX_ERR_SUCCESS = 0               //!< : success
} mail_box_errno_t, *p_mail_box_errno_t;

//! mail type
typedef enum mail_type {
	SEND_MAIL_TYPE_NORMAL = 0,             //!< : send normal mail
	SEND_MAIL_TYPE_EMERGENCY               //!< : send emergency mail
} mail_type_t, *p_mail_type_t;

//! mail structure
typedef struct mail {
	struct mail *p_next_queue_mail;        //!< mail address of next queueing mail
} mail_t, *p_mail_t;

//! mail box reference structure
typedef struct mail_box_reference {
	u16    stock_mail_count;               //!< queueing mail count
	u8     wait_status;                    //!< wait status of task
	u8     wait_task_id;                   //!< task id of wait receive mail
	void * p_mail;                         //!< mail address of queueing top
} mail_box_ref_t, *p_mail_box_ref_t;

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
mail_box_errno_t receive_mail_box(u8 id, void **pp_rcv_mail);
mail_box_errno_t poll_mail_box(u8 id, void **pp_rcv_mail);
mail_box_errno_t send_mail_box(u8 id, mail_type_t type, void *p_snd_mail);
mail_box_errno_t refer_mail_box(u8 id, p_mail_box_ref_t p_mail_ref);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_MAIL_BOX_H)
