//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// Use in source and binary forms, with or without modification, are permitted provided
// by agreeing to the following terms and conditions:
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of configuration for scorpius.
//=====================================================================================

#if !defined(SCO_CONFIG_H)
#define SCO_CONFIG_H

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
// configuration service function
#define SCHED_TOTAL_TASK                11     //!< total number of tasks
#define SCHED_TOTAL_SEMAPHORE           (5 + 5)     //!< total number of semaphores
#define SCHED_TOTAL_EVENT_FLAG          (6 + 3)     //!< total number of event flags
#define SCHED_TOTAL_MAIL_BOX            3     //!< total number of mail boxes

// configuration trace log function
#define TC_LOG_RECORD_NUM               0   //!< trace log buffer number
#define TC_LOG_SUPPORT_TIME                   //!< enable this define to support trace time in trace log
#define TRACE_LOG_SCORPIUS_FULL         0     //!< trace all log
#define TRACE_LOG_SCORPIUS_MINIMUM      1     //!< trace minimum log for output to TLV
#define TRACE_LOG_SCORPIUS_NON          2     //!< not trace log in scorpius
#define TRACE_LOG_APPLICATION_ENABLE    0     //!< enable  trace log in application
#define TRACE_LOG_APPLICATION_DISABLE   1     //!< disable trace log in application
#define TC_LOG_SCORPIUS_LEVEL           TRACE_LOG_SCORPIUS_MINIMUM
#define TC_LOG_APPLICATION_LEVEL        TRACE_LOG_APPLICATION_ENABLE

// configuration support service function
#define SCHED_SUPPORT_TASK           //!< enable this define to support scheduler task function
#define SCHED_SUPPORT_INTERRUPT      //!< enable this define to support scheduler int handler
#define SCHED_SUPPORT_DEVELOP_CHECK  //!< enable this define to support developper check

//! task entry table structure
typedef void (* f_task_t)(void);
typedef struct table_task_entry {
	f_task_t f_task;                       //!< task entry point
	void *   p_stack;                      //!< stack base address(descending order)
	u8       priority;                     //!< task priority
} table_task_entry_t, *p_table_task_entry_t;

//! semaphore entry table structure
typedef struct table_semaphore_entry {
	u8 max_resource;                       //!< number of maximum resource
	u8 initial_resource;                   //!< number of initial resource
} table_semaphore_entry_t, *p_table_semaphore_entry_t;

#endif // !defined(SCO_CONFIG_H)
