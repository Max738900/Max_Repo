//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of trace log message by scorpius.
//=====================================================================================

#if !defined(SCO_TRACE_LOG_MSG_H)
#define SCO_TRACE_LOG_MSG_H

// trace log message by scorpius
#define TC_LOG_MSG_START_ENDLESS    1    //!< "start trace log in endless mode."
#define TC_LOG_MSG_START_AUTO_STOP  2    //!< "start trace log in auto stop mode."
#define TC_LOG_MSG_EXC_TSK          3    //!< "executable task %d."
#define TC_LOG_MSG_SLP_TSK          4    //!< "task %d sleep."
#define TC_LOG_MSG_DSP_ENT_TSK      5    //!< "dispatch to task %d."
#define TC_LOG_MSG_DSP_EXT_TSK      6    //!< "leave from task %d."
#define TC_LOG_MSG_ENT_WAK_TSK      64   //!< "enter to wakeup_task taskid=%d."
#define TC_LOG_MSG_EXT_WAK_TSK      65   //!< "leave from wakeup_task %d."
#define TC_LOG_MSG_ENT_SLP_TSK      66   //!< "enter to sleep_task."
#define TC_LOG_MSG_EXT_SLP_TSK      67   //!< "leave from sleep_task %d."
#define TC_LOG_MSG_ENT_REF_TSK      68   //!< "enter to refer_task taskid=%d."
#define TC_LOG_MSG_EXT_REF_TSK      69   //!< "leave from refer_task %d."
#define TC_LOG_MSG_ENT_GET_SEM      80   //!< "enter to get_semaphore semid=%d."
#define TC_LOG_MSG_EXT_GET_SEM      81   //!< "leave from get_semaphore %d."
#define TC_LOG_MSG_ENT_POL_SEM      82   //!< "enter to poll_semaphore semid=%d."
#define TC_LOG_MSG_EXT_POL_SEM      83   //!< "leave from poll_semaphore %d."
#define TC_LOG_MSG_ENT_PUT_SEM      84   //!< "enter to put_semaphore semid=%d."
#define TC_LOG_MSG_EXT_PUT_SEM      85   //!< "leave from put_semaphore %d."
#define TC_LOG_MSG_ENT_REF_SEM      86   //!< "enter to refer_semaphore semid=%d."
#define TC_LOG_MSG_EXT_REF_SEM      87   //!< "leave from refer_semaphore %d."
#define TC_LOG_MSG_ENT_WAI_FLG      96   //!< "enter to wait_event_flag flgid=%d."
#define TC_LOG_MSG_EXT_WAI_FLG      97   //!< "leave from wait_event_flag %d."
#define TC_LOG_MSG_ENT_POL_FLG      98   //!< "enter to poll_event_flag flgid=%d."
#define TC_LOG_MSG_EXT_POL_FLG      99   //!< "leave from poll_event_flag %d."
#define TC_LOG_MSG_ENT_SET_FLG      100  //!< "enter to set_event_flag flgid=%d."
#define TC_LOG_MSG_EXT_SET_FLG      101  //!< "leave from set_event_flag %d."
#define TC_LOG_MSG_ENT_CLR_FLG      102  //!< "enter to clear_event_flag flgid=%d."
#define TC_LOG_MSG_EXT_CLR_FLG      103  //!< "leave from clear_event_flag %d."
#define TC_LOG_MSG_ENT_REF_FLG      104  //!< "enter to refer_event_flag flgid=%d."
#define TC_LOG_MSG_EXT_REF_FLG      105  //!< "leave from refer_event_flag %d."
#define TC_LOG_MSG_ENT_RCV_MBX      112  //!< "enter to receive_mail_box mail_box_id=%d."
#define TC_LOG_MSG_EXT_RCV_MBX      113  //!< "leave from receive_mail_box %d."
#define TC_LOG_MSG_ENT_POL_MBX      114  //!< "enter to poll_mail_box mail_box_id=%d."
#define TC_LOG_MSG_EXT_POL_MBX      115  //!< "leave from poll_mail_box %d."
#define TC_LOG_MSG_ENT_SND_MBX      116  //!< "enter to send_mail_box mail_box_id=%d."
#define TC_LOG_MSG_EXT_SND_MBX      117  //!< "leave from send_mail_box %d."
#define TC_LOG_MSG_ENT_REF_MBX      118  //!< "enter to refer_mail_box mail_box_id=%d."
#define TC_LOG_MSG_EXT_REF_MBX      119  //!< "leave from refer_mail_box %d."
#define TC_LOG_MSG_EXC_ARD_TSK      224  //!< "task is already queued wakeup event."
#define TC_LOG_MSG_WAI_SVC_TSK      225  //!< "task is wait in service type %d."

#endif // !defined(SCO_TRACE_LOG_MSG_H)
