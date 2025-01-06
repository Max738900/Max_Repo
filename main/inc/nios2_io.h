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
//! @brief  I/O memory access functions for Nios II processor.
//=====================================================================================

#if !defined(__NIOS2_IO_H__)
#define __NIOS2_IO_H__

#include <io.h>
#include "utility.h"

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
// ------ I/O memory read (field get) functions ------
//! I/O memory read (field get) function (for 8-bit access)
#define iofldget8(fld, p_addr)         iobitget8(fld, p_addr)
//! I/O memory read (field get) function (for 16-bit access)
#define iofldget16(fld, p_addr)        iobitget16(fld, p_addr)
//! I/O memory read (field get) function (for 32-bit access)
#define iofldget32(fld, p_addr)        iobitget32(fld, p_addr)

// ------ Nios II I/O memory access functions ------
//! Nios II I/O memory access functions (SFR read)
#define nios2_read(p_addr)             ioread32((void *) (p_addr))
//! Nios II I/O memory access functions (SFR write)
#define nios2_write(value, p_addr)     iowrite32(value, (void *) (p_addr))
//! Nios II I/O memory access functions (SFR read (bit get))
#define nios2_bitget(bit, p_addr)      iobitget32(bit, (void *) (p_addr))
//! Nios II I/O memory access functions (SFR write (bit set))
#define nios2_bitset(bit, p_addr)      iobitset32(bit, (void *) (p_addr))
//! Nios II I/O memory access functions (SFR write (bit clear))
#define nios2_bitclr(bit, p_addr)      iobitclr32(bit, (void *) (p_addr))
//! Nios II I/O memory access functions (SFR write (bit toggle))
#define nios2_bittgl(bit, p_addr)      iobittgl32(bit, (void *) (p_addr))
//! Nios II I/O memory access functions (SFR write (field get))
#define nios2_fldget(fld, p_addr)      iofldget32(fld, (void *) (p_addr))
//! Nios II I/O memory access functions (SFR write (field set))
#define nios2_fldset(fld, val, p_addr) iofldset32(fld, val, (void *) (p_addr))


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
//! @brief I/O memory read (for 8-bit access).
//
//! @param[in] p_addr : Pointer to the I/O memory
//
//! @return Read value
//------------------------------------------------------------------------------
static inline unsigned int ioread8(const void *p_addr)
{
	return (unsigned int) __builtin_ldbuio((void *) p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory read (for 16-bit access).
//
//! @param[in] p_addr : Pointer to the I/O memory
//
//! @return Read value
//------------------------------------------------------------------------------
static inline unsigned int ioread16(const void *p_addr)
{
	return (unsigned int) __builtin_ldhuio((void *) p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory read (for 32-bit access).
//
//! @param[in] p_addr : Pointer to the I/O memory
//
//! @return Read value
//------------------------------------------------------------------------------
static inline unsigned int ioread32(const void *p_addr)
{
	return (unsigned int) __builtin_ldwio((void *) p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (for 8-bit access).
//
//! @param[in]  value  : Write value
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iowrite8(uint8_t value, void *p_addr)
{
	__builtin_stbio(p_addr, value);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (for 16-bit access).
//
//! @param[in]  value  : Write value
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iowrite16(uint16_t value, void *p_addr)
{
	__builtin_sthio(p_addr, value);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (for 32-bit access).
//
//! @param[in]  value  : Write value
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iowrite32(uint32_t value, void *p_addr)
{
	__builtin_stwio(p_addr, value);
}

//------------------------------------------------------------------------------
//! @brief I/O memory read (bit get) (for 8-bit access).
//
//! @param[in] bit    : Access bit
//! @param[in] p_addr : Pointer to the I/O memory
//
//! @return Read value
//------------------------------------------------------------------------------
static inline unsigned int iobitget8(uint8_t bit, const void *p_addr)
{
	return BIT_MSK(ioread8(p_addr), bit);
}

//------------------------------------------------------------------------------
//! @brief I/O memory read (bit get) (for 16-bit access).
//
//! @param[in] bit    : Access bit
//! @param[in] p_addr : Pointer to the I/O memory
//
//! @return Read value
//------------------------------------------------------------------------------
static inline unsigned int iobitget16(uint16_t bit, const void *p_addr)
{
	return BIT_MSK(ioread16(p_addr), bit);
}

//------------------------------------------------------------------------------
//! @brief I/O memory read (bit get) (for 32-bit access).
//
//! @param[in] bit    : Access bit
//! @param[in] p_addr : Pointer to the I/O memory
//
//! @return Read value
//------------------------------------------------------------------------------
static inline unsigned int iobitget32(uint32_t bit, const void *p_addr)
{
	return BIT_MSK(ioread32(p_addr), bit);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (bit set) (for 8-bit access).
//
//! @param[in] bit     : Access bit
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iobitset8(uint8_t bit, void *p_addr)
{
	iowrite8(ioread8(p_addr) | bit, p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (bit set) (for 16-bit access).
//
//! @param[in]  bit    : Access bit
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iobitset16(uint16_t bit, void *p_addr)
{
	iowrite16(ioread16(p_addr) | bit, p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (bit set) (for 32-bit access).
//
//! @param[in]  bit    : Access bit
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iobitset32(uint32_t bit, void *p_addr)
{
	iowrite32(ioread32(p_addr) | bit, p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (bit clear) (for 8-bit access).
//
//! @param[in]  bit    : Access bit
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iobitclr8(uint8_t bit, void *p_addr)
{
	iowrite8(BIT_MSK(ioread8(p_addr), ~bit), p_addr);
}

//-----------------------------------------------------------------------
//! @brief  I/O memory write (bit clear) (for 16-bit access).
//
//! @param[in]  bit    : Access bit
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//-----------------------------------------------------------------------
static inline void iobitclr16(uint16_t bit, void *p_addr)
{
	iowrite16(BIT_MSK(ioread16(p_addr), ~bit), p_addr);
}

//-----------------------------------------------------------------------
//! @brief  I/O memory write (bit clear) (for 32-bit access).
//
//! @param[in]  bit    : Access bit
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//-----------------------------------------------------------------------
static inline void iobitclr32(uint32_t bit, void *p_addr)
{
	iowrite32(BIT_MSK(ioread32(p_addr), ~bit), p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (bit toggle) (for 8-bit access).
//
//! @param[in]  bit    : Access bit
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iobittgl8(uint8_t bit, void *p_addr)
{
	iowrite8(BIT_TGL(ioread8(p_addr), bit), p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (bit toggle) (for 16-bit access).
//
//! @param[in]  bit    : Access bit
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iobittgl16(uint16_t bit, void *p_addr)
{
	iowrite16(BIT_TGL(ioread16(p_addr), bit), p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (bit toggle) (for 32-bit access).
//
//! @param[in]  bit    : Access bit
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iobittgl32(uint32_t bit, void *p_addr)
{
	iowrite32(BIT_TGL(ioread32(p_addr), bit), p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (field set) (for 8-bit access).
//
//! @param[in]  fld    : Access bit field
//! @param[in]  val    : Write value
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iofldset8(uint8_t fld, uint8_t val, void *p_addr)
{
	const uint8_t save = iobitget8(~fld, p_addr);

	iowrite8(save | BIT_MSK(val, fld), p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (field set) (for 16-bit access).
//
//! @param[in]  fld    : Access bit field
//! @param[in]  val    : Write value
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iofldset16(uint16_t fld, uint16_t val, void *p_addr)
{
	const uint16_t save = iobitget16(~fld, p_addr);

	iowrite16(save | BIT_MSK(val, fld), p_addr);
}

//------------------------------------------------------------------------------
//! @brief I/O memory write (field set) (for 32-bit access).
//
//! @param[in]  fld    : Access bit field
//! @param[in]  val    : Write value
//! @param[out] p_addr : Pointer to the I/O memory
//
//! @return None
//------------------------------------------------------------------------------
static inline void iofldset32(uint32_t fld, uint32_t val, void *p_addr)
{
	const uint32_t save = iobitget32(~fld, p_addr);

	iowrite32(save | BIT_MSK(val, fld), p_addr);
}


#endif // !defined(__NIOS2_IO_H__)
