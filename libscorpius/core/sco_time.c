//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  This file provides the following functions of system time.
//!          - initial_system_time_func()
//!          - update_systime_func()
//!          - get_systime_upper()
//!          - get_systime_lower()
//=====================================================================================

#include <sco_common.h>
#include <sco_config.h>
#include <sco_time.h>

#if (TC_LOG_RECORD_NUM != 0) && defined(TC_LOG_SUPPORT_TIME)

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// internal function prototype
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------
extern u32 get_systime_us(void);

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
#if !defined(SCORPIUS_USE_HAL)
static u32 g_systime_tick;
#else
extern u32 _alt_nticks;
#endif // !defined(SCORPIUS_USE_HAL)

//------------------------------------------------------------------------------
//! @brief Initial system time.
//
//! @return None
//------------------------------------------------------------------------------
#if !defined(SCORPIUS_USE_HAL)
void initial_system_time_func(void)
{
	g_systime_tick = 0;
}
#endif // !defined(SCORPIUS_USE_HAL)

//------------------------------------------------------------------------------
//! @brief Update system time.
//
//! @return None
//------------------------------------------------------------------------------
#if !defined(SCORPIUS_USE_HAL)
void update_systime_func(void)
{
	g_systime_tick++;
}
#endif // !defined(SCORPIUS_USE_HAL)

//------------------------------------------------------------------------------
//! @brief Get upper of system time.
//
//! @return "g_systime_tick" system time tick
//------------------------------------------------------------------------------
u32 get_systime_upper(void)
{
#if !defined(SCORPIUS_USE_HAL)
	return g_systime_tick;
#else
	return _alt_nticks;
#endif // !defined(SCORPIUS_USE_HAL)
}

//------------------------------------------------------------------------------
//! @brief Get lower of system time.
//
//! @return system time (us)
//------------------------------------------------------------------------------
u32 get_systime_lower(void)
{
	return get_systime_us();
}
#endif // (TC_LOG_RECORD_NUM != 0) && defined(TC_LOG_SUPPORT_TIME)
