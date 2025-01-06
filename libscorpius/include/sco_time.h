//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of system time function.
//=====================================================================================

#if !defined(SCO_TIME_H)
#define SCO_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
#if (TC_LOG_RECORD_NUM != 0) && defined(TC_LOG_SUPPORT_TIME) && !defined(SCORPIUS_USE_HAL)
#define update_systime()     update_systime_func()
#else
#define update_systime()
#endif // (TC_LOG_RECORD_NUM != 0) && defined(TC_LOG_SUPPORT_TIME) && !defined(SCORPIUS_USE_HAL)

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
void update_systime_func(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_TIME_H)
