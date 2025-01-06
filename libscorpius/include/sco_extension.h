//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of extention information in scorpius.
//=====================================================================================

#if !defined(SCO_EXTENSION_H)
#define SCO_EXTENSION_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//! Extension configration setting
#define EXTENSION_SEMID_START   3
#define EXTENSION_SEMID_NUM     7

#define EXTENSION_FLAGID_START  3
#define EXTENSION_FLAGID_NUM    2

#define EXTENSION_MAILBOXID_START  0
#define EXTENSION_MAILBOXID_NUM    0

//! Extension error code
#define EXTENSION_ERR_NOSTART_SCHEDULER     (SCHED_LOG_ERR_NUM + 16)
#define EXTENSION_ERR_NOSUPPORT_SERVICE     (EXTENSION_ERR_NOSTART_SCHEDULER + 1)
#define EXTENSION_ERR_FAIL_ALLOCATION       (EXTENSION_ERR_NOSTART_SCHEDULER + 2)


//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------
extern int scorpius_start_flag;

//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
void extension_initial_scheduler( void );
void extension_start_scheduler( void );
int  extension_assign_sem( u8 *p_sem );
int  extension_get_semaphore( u8 sem );
int  extension_put_semaphore( u8 sem );
int  extension_assign_event_flag( u8 *p_group );
int  extension_wait_event_flag( u8 group, u16 wai_ptn, u32 type );
int  extension_set_event_flag( u8 group, u16 flags );
int  extension_assign_mail_box( u8 *p_mailbox );
int  extension_receive_mail_box( u8 mailbox, void **pp_mail );
int  extension_send_mail_box( u8 mailbox, mail_type_t type, void *p_mail );

int  recursively_get_semaphore( u8 id, int *locked_task_id, int *nest );
int  recursively_put_semaphore( u8 id, int *locked_task_id, int *nest );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* SCO_EXTENSION_H */
