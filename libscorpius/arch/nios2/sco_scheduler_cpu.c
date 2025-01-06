//=====================================================================================
// Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
//
// This program is proprietary and confidential. By using this program
// you agree to the terms of the associated Macnica Software License Agreement.
//------------------------------------------------------------------------------------
//! @file
//! @brief  This file provides the following functions of cpu dependent code
//!          - cpu_context()
//=====================================================================================

#include <sco_common.h>
#include <sco_config.h>
#include <sco_cpu.h>

//------------------------------------------------------------------------------
// define and structures
//------------------------------------------------------------------------------
#define PSR_REG_STATUS_PIE_ENA        0x00000001
#define PSR_REG_STATUS_PIE_DIS        0x00000000
#define PSR_REG_STATUS_U_USER         0x00000010
#define PSR_REG_STATUS_U_SVS          0x00000000

//------------------------------------------------------------------------------
// internal function prototype
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//! @brief Register interrupt handler for scheduler.
//
//! @return None
//------------------------------------------------------------------------------
void cpu_context(p_cpu_regs_t p_regs, void *p_stack, f_task_t f_task)
{
	p_regs->sp = (u32)p_stack;
	p_regs->fp = (u32)p_stack;
	p_regs->ra = (u32)f_task;
	p_regs->pc = (u32)f_task;
	p_regs->status = PSR_REG_STATUS_PIE_ENA | PSR_REG_STATUS_U_SVS;
}
