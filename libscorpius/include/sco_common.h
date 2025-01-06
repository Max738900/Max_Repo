//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of common information in scorpius.
//=====================================================================================

#if !defined(SCO_COMMON_H)
#define SCO_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
// define scorpius common information
#if !defined(TRUE)
#define TRUE  true
#endif
#if !defined(FALSE)
#define FALSE false
#endif
#if !defined(NULL)
#define NULL ((void*)0)
#endif

//! boolean type
#if !defined(__cplusplus)
#include <stdbool.h>
#endif // !defined(__cplusplus)
//	typedef enum bool {
//		false = (0 == 1),         //!< : false
//		true  = (1 == 1)          //!< : true
//	} bool;

//! bit type
typedef unsigned long  u32;   //!< unsigned 32bit
typedef unsigned short u16;   //!< unsigned 16bit
typedef unsigned char  u8;    //!< unsigned  8bit
typedef signed   long  s32;   //!< signed   32bit
typedef signed   short s16;   //!< signed   16bit
typedef signed   char  s8;    //!< signed    8bit

//------------------------------------------------------------------------------
// external variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_COMMON_H)
