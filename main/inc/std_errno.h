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
//! @brief  Definitions of standard error numbers.
//=====================================================================================

#if !defined(__STD_ERRNO_H__)
#define __STD_ERRNO_H__


//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
//! error numbers
typedef enum errnos {
	ERRNO_SUCCESS    = 0,    //!< : "Success"
	ERRNO_PERM       = -1,   //!< : "Operation not permitted"
	ERRNO_IO         = -5,   //!< : "I/O error"
	ERRNO_NOMEM      = -12,  //!< : "Out of memory"
	ERRNO_BUSY       = -16,  //!< : "Device or resource busy"
	ERRNO_NODEV      = -19,  //!< : "No such device"
	ERRNO_INVAL      = -22,  //!< : "Invalid argument"
	ERRNO_NOTTY      = -25,  //!< : "Not a typewriter"
	ERRNO_EFBIG      = -27,  //!< : "File too large"
	ERRNO_NOSPC      = -28,  //!< : "No space left on device"
	ERRNO_ROFS       = -30,  //!< : "Read-only file system"
	ERRNO_PIPE       = -32,  //!< : "Broken pipe"
	ERRNO_NOSYS      = -38,  //!< : "Function not implemented"
	ERRNO_BADRQC     = -56,  //!< : "Invalid request code"
	ERRNO_NODATA     = -61,  //!< : "No data available"
	ERRNO_TIME       = -62,  //!< : "Timer expired"
	ERRNO_COMM       = -70,  //!< : "Communication error on send"
	ERRNO_PROTO      = -71,  //!< : "Protocol error"
	ERRNO_TIMEDOUT   = -110, //!< : "Connection timed out"
	ERRNO_NOMEDIUM   = -123, //!< : "No medium found"
	ERRNO_MEDIUMTYPE = -124, //!< : "Wrong medium type"
	ERRNO_CANCELED   = -125  //!< : "Operation Canceled"
} errnos_t, *p_errnos_t;

//! function type of error number value
typedef errnos_t (*f_errnos_t)(void);


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// inline functions
//------------------------------------------------------------------------------


#endif // !defined(__STD_ERRNO_H__)
