//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of scorpius include.
//=====================================================================================

#if !defined(SCORPIUS_H)
#define SCORPIUS_H

#include "sco_common.h"
#include "../config/sco_config.h"
#include "sco_scheduler.h"
#include "sco_task.h"
#include "sco_semaphore.h"
#include "sco_event_flag.h"
#include "sco_mail_box.h"
#include "sco_time.h"
#include "sco_trace_log.h"
#include "sco_trace_log_msg.h"

#if defined(SCORPIUS_USE_HAL)
#include "sco_extension.h"
#endif // defined(SCORPIUS_USE_HAL)

#endif // !defined(SCORPIUS_H)
