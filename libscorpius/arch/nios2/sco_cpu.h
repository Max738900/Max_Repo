//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  Definitions of cpu dependent information.
//=====================================================================================

#if !defined(SCO_CPU_H)
#define SCO_CPU_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
#pragma pack(1)
//! Nios2 CPU register structure
typedef struct cpu_regs {
	u32 regs[23];             //!< general registers
	u32 sp;                   //!< stack pointer
	u32 fp;                   //!< frame pointer
	u32 ra;                   //!< return address
	u32 pc;                   //!< program counter
	u32 status;               //!< status register
} cpu_regs_t, *p_cpu_regs_t;
#pragma pack()

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// function prototype
//------------------------------------------------------------------------------
void cpu_context(p_cpu_regs_t p_regs, void *p_stack, f_task_t f_task);
u32  _is_task_ctx_mode(void);
u32  _cpu_lock(void);
void _cpu_unlock(void);
void _enable_interrupt(u32 irq);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !defined(SCO_CPU_H)
