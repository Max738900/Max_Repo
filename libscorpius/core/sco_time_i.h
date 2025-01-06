//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of system time function.
//=====================================================================================

#if !defined(SCO_TIME_I_H)
#define SCO_TIME_I_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
#if (TC_LOG_RECORD_NUM != 0) && defined(TC_LOG_SUPPORT_TIME) && !defined(SCORPIUS_USE_HAL)
#define initial_system_time()     initial_system_time_func()
#else
#define initial_system_time()
#endif // (TC_LOG_RECORD_NUM != 0) && defined(TC_LOG_SUPPORT_TIME) && !defined(SCORPIUS_USE_HAL)


//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
void initial_system_time_func(void);
void update_systime_func(void);
u32  get_systime_upper(void);
u32  get_systime_lower(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_TIME_I_H)
