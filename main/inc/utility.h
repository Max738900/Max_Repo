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
//! @brief  Common utility functions.
//=====================================================================================

#if !defined(__UTILITY_H__)
#define __UTILITY_H__

#include "std_types.h"

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
#if !defined(ARRAY_SIZE)
//! calculate specified array size
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif // !defined(ARRAY_SIZE)

//! get KB size from bytes
#define BYTES2KB(x)   ((x) / 1024)
//! get MB size from bytes
#define BYTES2MB(x)   (((x) / 1024) / 1024)
//! get GB size from bytes
#define BYTES2GB(x)   ((((x) / 1024) / 1024) / 1024)
//! get byte size from KB
#define KB(x)         ((x) * 1024)
//! get byte size from MB
#define MB(x)         ((x) * 1024 * 1024)
//! get byte size from GB
#define GB(x)         (((unsigned long long) x) * 1024 * 1024 * 1024)

//! get masked value of the specified bit in the bitmap
#define BIT_MSK(bitmap, bit)    ((bitmap) & ((typeof(bitmap)) (bit)))
//! get cleared value of the specified bit in the bitmap
#define BIT_CLR(bitmap, bit)    ((bitmap) & ((typeof(bitmap)) ~(bit)))
//! get toggleed value of the specified bit in the bitmap
#define BIT_TGL(bitmap, bit)    ((bitmap) ^ ((typeof(bitmap)) (bit)))
//! set specified bit in the bitmap value
#define SET_BIT(bitmap, bit)    do {                                  \
                                  (bitmap) |= (typeof(bitmap)) (bit); \
                                } while (false)
//! clear specified bit in the bitmap value
#define CLR_BIT(bitmap, bit)    do {                                     \
                                  (bitmap) &= ~((typeof(bitmap)) (bit)); \
                                } while (false)
//! toggle specified bit in the bitmap value
#define TGL_BIT(bitmap, bit)    do {                                  \
                                  (bitmap) ^= (typeof(bitmap)) (bit); \
                                } while (false)
//! check whether specified bit in the bitmap is set or not
#define BIT_IS_SET(bitmap, bit) (((bitmap) & ((typeof(bitmap)) (bit))) != 0)

#ifdef __LITTLE_ENDIAN
//! get value form unaligned address
#define get_unaligned(p_addr) ((typeof(*(p_addr)))                                                        \
                               ({                                                                         \
                                 (sizeof(*(p_addr)) == 1) ? (*(p_addr)) : (                               \
                                 (sizeof(*(p_addr)) == 2) ? _get_unaligned_le16((uint8_t *) (p_addr)) : ( \
                                 (sizeof(*(p_addr)) == 4) ? _get_unaligned_le32((uint8_t *) (p_addr)) : ( \
                                 (sizeof(*(p_addr)) == 8) ? _get_unaligned_le64((uint8_t *) (p_addr)) : ( \
                                 *(p_addr)                                                                \
                                 ))));                                                                    \
                                }))
//! put value to unaligned address
#define put_unaligned(val, p_addr) ({                                                                            \
                                     switch (sizeof(*(p_addr))) {                                                \
                                     case 1: *((uint8_t *) (p_addr)) = (uint8_t) (val);                   break; \
                                     case 2: _put_unaligned_le16((uint16_t) (val), (uint8_t *) (p_addr)); break; \
                                     case 4: _put_unaligned_le32((uint32_t) (val), (uint8_t *) (p_addr)); break; \
                                     case 8: _put_unaligned_le64((uint64_t) (val), (uint8_t *) (p_addr)); break; \
                                     default:                                                             break; \
                                     }                                                                           \
                                     (void) 0;                                                                   \
                                    })
#else
//! get value form unaligned address
#define get_unaligned(p_addr) ((typeof(*(p_addr)))                                                        \
                               ({                                                                         \
                                 (sizeof(*(p_addr)) == 1) ? (*(p_addr)) : (                               \
                                 (sizeof(*(p_addr)) == 2) ? _get_unaligned_be16((uint8_t *) (p_addr)) : ( \
                                 (sizeof(*(p_addr)) == 4) ? _get_unaligned_be32((uint8_t *) (p_addr)) : ( \
                                 (sizeof(*(p_addr)) == 8) ? _get_unaligned_be64((uint8_t *) (p_addr)) : ( \
                                 *(p_addr)                                                                \
                                 ))));                                                                    \
                                }))
//! put value to unaligned address
#define put_unaligned(val, p_addr) ({                                                                            \
                                     switch (sizeof(*(p_addr))) {                                                \
                                     case 1: *((uint8_t *) (p_addr)) = (uint8_t) (val);                   break; \
                                     case 2: _put_unaligned_be16((uint16_t) (val), (uint8_t *) (p_addr)); break; \
                                     case 4: _put_unaligned_be32((uint32_t) (val), (uint8_t *) (p_addr)); break; \
                                     case 8: _put_unaligned_be64((uint64_t) (val), (uint8_t *) (p_addr)); break; \
                                     default:                                                             break; \
                                     }                                                                           \
                                     (void) 0;                                                                   \
                                    })
#endif // __LITTLE_ENDIAN


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// inline functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//! @brief Get powered value. \n
//!        (This function is same as "pow()" in <math.h>, except to using
//!         "int" type)
//
//! @param[in] base : Base value
//! @param[in] n    : Number of power
//
//! @return Powered value
//------------------------------------------------------------------------------
static inline int power(int base, int n)
{
	int i, p;

	p = 1;
	for (i = 0; i < n; i++) p *= base;

	return p;
}

//------------------------------------------------------------------------------
//! @brief Get 16-bit value from unaligned address. (for little endian) \n
//!        (This function should not be called directly)
//
//! @param[in] p_addr : Pointer to the value
//
//! @return Value from unaligned address
//------------------------------------------------------------------------------
static inline uint16_t _get_unaligned_le16(const uint8_t *p_addr)
{
	return ((uint16_t) p_addr[0]) | (((uint16_t) p_addr[1]) << 8);
}

//------------------------------------------------------------------------------
//! @brief Get 32-bit value from unaligned address. (for little endian) \n
//!        (This function should not be called directly)
//
//! @param[in] p_addr : Pointer to the value
//
//! @return Value from unaligned address
//------------------------------------------------------------------------------
static inline uint32_t _get_unaligned_le32(const uint8_t *p_addr)
{
	return ((uint32_t) p_addr[0])        | (((uint32_t) p_addr[1]) <<  8) |
	      (((uint32_t) p_addr[2]) << 16) | (((uint32_t) p_addr[3]) << 24);
}

//------------------------------------------------------------------------------
//! @brief Get 64-bit value from unaligned address. (for little endian) \n
//!        (This function should not be called directly)
//
//! @param[in] p_addr : Pointer to the value
//
//! @return Value from unaligned address
//------------------------------------------------------------------------------
static inline uint64_t _get_unaligned_le64(const uint8_t *p_addr)
{
	return ((uint64_t) p_addr[0])        | (((uint64_t) p_addr[1]) <<  8) |
	      (((uint64_t) p_addr[2]) << 16) | (((uint64_t) p_addr[3]) << 24) |
	      (((uint64_t) p_addr[4]) << 32) | (((uint64_t) p_addr[5]) << 40) |
	      (((uint64_t) p_addr[6]) << 48) | (((uint64_t) p_addr[7]) << 56);
}

//------------------------------------------------------------------------------
//! @brief Get 16-bit value from unaligned address. (for big endian) \n
//!        (This function should not be called directly)
//
//! @param[in] p_addr : Pointer to the value
//
//! @return Value from unaligned address
//------------------------------------------------------------------------------
static inline uint16_t _get_unaligned_be16(const uint8_t *p_addr)
{
	return (((uint16_t) p_addr[0]) << 8) | (uint16_t) p_addr[1];
}

//------------------------------------------------------------------------------
//! @brief Get 32-bit value from unaligned address. (for big endian) \n
//!        (This function should not be called directly)
//
//! @param[in] p_addr : Pointer to the value
//
//! @return Value from unaligned address
//------------------------------------------------------------------------------
static inline uint32_t _get_unaligned_be32(const uint8_t *p_addr)
{
	return (((uint32_t) p_addr[0]) << 24) | (((uint32_t) p_addr[1]) << 16) |
	       (((uint32_t) p_addr[2]) <<  8) |   (uint32_t) p_addr[3];
}

//------------------------------------------------------------------------------
//! @brief Get 64-bit value from unaligned address. (for big endian) \n
//!        (This function should not be called directly)
//
//! @param[in] p_addr : Pointer to the value
//
//! @return Value from unaligned address
//------------------------------------------------------------------------------
static inline uint64_t _get_unaligned_be64(const uint8_t *p_addr)
{
	return (((uint64_t) p_addr[0]) << 56) | (((uint64_t) p_addr[1]) << 48) |
	       (((uint64_t) p_addr[2]) << 40) | (((uint64_t) p_addr[3]) << 32) |
	       (((uint64_t) p_addr[4]) << 24) | (((uint64_t) p_addr[5]) << 15) |
	       (((uint64_t) p_addr[6]) <<  8) |   (uint64_t) p_addr[7];
}

//------------------------------------------------------------------------------
//! @brief Put 16-bit value to unaligned address. (for little endian) \n
//!        (This function should not be called directly)
//
//! @param[in]  val    : Value to put
//! @param[out] p_addr : Pointer to the put address
//
//! @return None
//------------------------------------------------------------------------------
static inline void _put_unaligned_le16(uint16_t val, uint8_t *p_addr)
{
	p_addr[0] = (uint8_t)  BIT_MSK(val, 0x00FF);
	p_addr[1] = (uint8_t) (BIT_MSK(val, 0xFF00) >> 8);
}

//------------------------------------------------------------------------------
//! @brief Put 32-bit value to unaligned address. (for little endian) \n
//!        (This function should not be called directly)
//
//! @param[in]  val    : Value to put
//! @param[out] p_addr : Pointer to the put address
//
//! @return None
//------------------------------------------------------------------------------
static inline void _put_unaligned_le32(uint32_t val, uint8_t *p_addr)
{
	p_addr[0] = (uint8_t)  BIT_MSK(val, 0x000000FF);
	p_addr[1] = (uint8_t) (BIT_MSK(val, 0x0000FF00) >>  8);
	p_addr[2] = (uint8_t) (BIT_MSK(val, 0x00FF0000) >> 16);
	p_addr[3] = (uint8_t) (BIT_MSK(val, 0xFF000000) >> 24);
}

//------------------------------------------------------------------------------
//! @brief Put 64-bit value to unaligned address. (for little endian) \n
//!        (This function should not be called directly)
//
//! @param[in]  val    : Value to put
//! @param[out] p_addr : Pointer to the put address
//
//! @return None
//------------------------------------------------------------------------------
static inline void _put_unaligned_le64(uint64_t val, uint8_t *p_addr)
{
	p_addr[0] = (uint8_t)  BIT_MSK(val, 0x00000000000000FFULL);
	p_addr[1] = (uint8_t) (BIT_MSK(val, 0x000000000000FF00ULL) >>  8);
	p_addr[2] = (uint8_t) (BIT_MSK(val, 0x0000000000FF0000ULL) >> 16);
	p_addr[3] = (uint8_t) (BIT_MSK(val, 0x00000000FF000000ULL) >> 24);
	p_addr[4] = (uint8_t) (BIT_MSK(val, 0x000000FF00000000ULL) >> 32);
	p_addr[5] = (uint8_t) (BIT_MSK(val, 0x0000FF0000000000ULL) >> 40);
	p_addr[6] = (uint8_t) (BIT_MSK(val, 0x00FF000000000000ULL) >> 48);
	p_addr[7] = (uint8_t) (BIT_MSK(val, 0xFF00000000000000ULL) >> 56);
}

//------------------------------------------------------------------------------
//! @brief Put 16-bit value to unaligned address. (for big endian) \n
//!        (This function should not be called directly)
//
//! @param[in]  val    : Value to put
//! @param[out] p_addr : Pointer to the put address
//
//! @return None
//------------------------------------------------------------------------------
static inline void _put_unaligned_be16(uint16_t val, uint8_t *p_addr)
{
	p_addr[0] = (uint8_t) (BIT_MSK(val, 0xFF00) >> 8);
	p_addr[1] = (uint8_t)  BIT_MSK(val, 0x00FF);
}

//------------------------------------------------------------------------------
//! @brief Put 32-bit value to unaligned address. (for big endian) \n
//!        (This function should not be called directly)
//
//! @param[in]  val    : Value to put
//! @param[out] p_addr : Pointer to the put address
//
//! @return None
//------------------------------------------------------------------------------
static inline void _put_unaligned_be32(uint32_t val, uint8_t *p_addr)
{
	p_addr[0] = (uint8_t) (BIT_MSK(val, 0xFF000000) >> 24);
	p_addr[1] = (uint8_t) (BIT_MSK(val, 0x00FF0000) >> 16);
	p_addr[2] = (uint8_t) (BIT_MSK(val, 0x0000FF00) >>  8);
	p_addr[3] = (uint8_t)  BIT_MSK(val, 0x000000FF);
}

//------------------------------------------------------------------------------
//! @brief Put 64-bit value to unaligned address. (for big endian) \n
//!        (This function should not be called directly)
//
//! @param[in]  val    : Value to put
//! @param[out] p_addr : Pointer to the put address
//
//! @return None
//------------------------------------------------------------------------------
static inline void _put_unaligned_be64(uint64_t val, uint8_t *p_addr)
{
	p_addr[0] = (uint8_t) (BIT_MSK(val, 0xFF00000000000000ULL) >> 56);
	p_addr[1] = (uint8_t) (BIT_MSK(val, 0x00FF000000000000ULL) >> 48);
	p_addr[2] = (uint8_t) (BIT_MSK(val, 0x0000FF0000000000ULL) >> 40);
	p_addr[3] = (uint8_t) (BIT_MSK(val, 0x000000FF00000000ULL) >> 32);
	p_addr[4] = (uint8_t) (BIT_MSK(val, 0x00000000FF000000ULL) >> 24);
	p_addr[5] = (uint8_t) (BIT_MSK(val, 0x0000000000FF0000ULL) >> 16);
	p_addr[6] = (uint8_t) (BIT_MSK(val, 0x000000000000FF00ULL) >>  8);
	p_addr[7] = (uint8_t)  BIT_MSK(val, 0x00000000000000FFULL);
}


#endif // !defined(__UTILITY_H__)
