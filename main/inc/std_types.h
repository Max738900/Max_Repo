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
//! @brief  Definitions of standard types.
//=====================================================================================

#if !defined(__STD_TYPES_H__)
#define __STD_TYPES_H__

#include <stddef.h>
//#include <stdint.h>
#include <stdbool.h>

#define __LITTLE_ENDIAN

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//#if !defined(bool) && !defined(true) && !defined(false)
////! boolean type for windows system
//typedef enum bool {
//	false = (0 == 1), //!< : "false" value
//	true  = (1 == 1)  //!< : "true" value
//} bool;
//#endif // !defined(bool) && !defined(true) && !defined(false)

// unsigned bit width type
typedef unsigned char      uint8_t;  //!< unsigned 8-bit type
typedef unsigned short     uint16_t; //!< unsigned 16-bit type
typedef unsigned int       uint32_t; //!< unsigned 32-bit type
typedef unsigned long long uint64_t; //!< unsigned 64-bit type

// singned bit width type
typedef signed char      int8_t;  //!< signed 8-bit type
typedef signed short     int16_t; //!< signed 16-bit type
typedef signed int       int32_t; //!< signed 32-bit type
typedef signed long long int64_t; //!< signed 64-bit type

#if defined(OFF) || defined(ON)
#error symbol "OFF" or "ON" is already defined!!
#endif // defined(OFF) || defined(ON)

// boolean value ON/OFF
#define OFF false //!< OFF
#define ON  true  //!< ON

// byte order functions
#if defined(__LITTLE_ENDIAN)
//! convert 16-bit CPU endian value to little endian
#define __cpu_to_le16(x) ((uint16_t) (x))
//! convert 32-bit CPU endian value to little endian
#define __cpu_to_le32(x) ((uint32_t) (x))
//! convert 64-bit CPU endian value to little endian
#define __cpu_to_le64(x) ((uint64_t) (x))
//! convert 16-bit CPU endian value to big endian
#define __cpu_to_be16(x) (((((uint16_t) (x)) & 0xFF) << 8) | ((((uint16_t) (x)) & 0xFF00) >> 8))
//! convert 32-bit CPU endian value to big endian
#define __cpu_to_be32(x) (((((uint32_t) (x)) & 0x000000FF) << 24) | \
                          ((((uint32_t) (x)) & 0x0000FF00) <<  8) | \
                          ((((uint32_t) (x)) & 0x00FF0000) >>  8) | \
                          ((((uint32_t) (x)) & 0xFF000000) >> 24))
//! convert 64-bit CPU endian value to big endian
#define __cpu_to_be64(x) (((((uint64_t) (x)) & 0x00000000000000FFULL) << 56) | \
                          ((((uint64_t) (x)) & 0x000000000000FF00ULL) << 40) | \
                          ((((uint64_t) (x)) & 0x0000000000FF0000ULL) << 24) | \
                          ((((uint64_t) (x)) & 0x00000000FF000000ULL) <<  8) | \
                          ((((uint64_t) (x)) & 0x000000FF00000000ULL) >>  8) | \
                          ((((uint64_t) (x)) & 0x0000FF0000000000ULL) >> 24) | \
                          ((((uint64_t) (x)) & 0x00FF000000000000ULL) >> 40) | \
                          ((((uint64_t) (x)) & 0xFF00000000000000ULL) >> 56))
#elif defined(__BIG_ENDIAN)
//! convert 16-bit CPU endian value to little endian
#define __cpu_to_le16(x) (((((uint16_t) (x)) & 0xFF) << 8) | ((((uint16_t) (x)) & 0xFF00) >> 8))
//! convert 32-bit CPU endian value to little endian
#define __cpu_to_le32(x) (((((uint32_t) (x)) & 0x000000FF) << 24) | \
                          ((((uint32_t) (x)) & 0x0000FF00) <<  8) | \
                          ((((uint32_t) (x)) & 0x00FF0000) >>  8) | \
                          ((((uint32_t) (x)) & 0xFF000000) >> 24))
//! convert 64-bit CPU endian value to little endian
#define __cpu_to_le64(x) (((((uint64_t) (x)) & 0x00000000000000FFULL) << 56) | \
                          ((((uint64_t) (x)) & 0x000000000000FF00ULL) << 40) | \
                          ((((uint64_t) (x)) & 0x0000000000FF0000ULL) << 24) | \
                          ((((uint64_t) (x)) & 0x00000000FF000000ULL) <<  8) | \
                          ((((uint64_t) (x)) & 0x000000FF00000000ULL) >>  8) | \
                          ((((uint64_t) (x)) & 0x0000FF0000000000ULL) >> 24) | \
                          ((((uint64_t) (x)) & 0x00FF000000000000ULL) >> 40) | \
                          ((((uint64_t) (x)) & 0xFF00000000000000ULL) >> 56))
//! convert 16-bit CPU endian value to big endian
#define __cpu_to_be16(x) ((uint16_t) (x))
//! convert 32-bit CPU endian value to big endian
#define __cpu_to_be32(x) ((uint32_t) (x))
//! convert 64-bit CPU endian value to big endian
#define __cpu_to_be64(x) ((uint64_t) (x))
#else
#error unknown endian! define "__LITTLE_ENDIAN" or "__BIG_ENDIAN".
#endif // defined(__xxx_ENDIAN)
#define __le16_to_cpu(x) __cpu_to_le16(x) //!< convert 16-bit little endian value to CPU endian
#define __le32_to_cpu(x) __cpu_to_le32(x) //!< convert 32-bit little endian value to CPU endian
#define __le64_to_cpu(x) __cpu_to_le64(x) //!< convert 64-bit little endian value to CPU endian
#define __be16_to_cpu(x) __cpu_to_be16(x) //!< convert 16-bit big endian value to CPU endian
#define __be32_to_cpu(x) __cpu_to_be32(x) //!< convert 32-bit big endian value to CPU endian
#define __be64_to_cpu(x) __cpu_to_be64(x) //!< convert 64-bit big endian value to CPU endian

//! function type of no return value
typedef void (*f_void_t)(void);
//! function type of int return value
typedef int (*f_int_t)(void);
//! function type of boolean return value
typedef bool (*f_bool_t)(void);


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// inline functions
//------------------------------------------------------------------------------


#endif // !defined(__STD_TYPES_H__)
