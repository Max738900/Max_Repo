/* =====================================================================================
 * Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
 *
 *  This program is proprietary and confidential. By using this program
 *  you agree to the terms of the associated Macnica Software License Agreement.
 * ------------------------------------------------------------------------------------
 * ! @file
 * ! @brief  This file provides following Nios2 assembler code.
 * !          - _is_task_ctx_mode()
 * !          - _cpu_lock()
 * !          - _cpu_unlock()
 * !          - _enable_interrupt()
 * ===================================================================================== */

/*	.include "../arch/nios2/sco_cpu.inc" */
	.include "./arch/nios2/sco_cpu.inc"
	.set noat

/* ------------------------------------------------------------------------------
 * _is_task_ctx_mode
 * ------------------------------------------------------------------------------*/
	.global _is_task_ctx_mode
_is_task_ctx_mode:
	ldw     at,   %gprel(g_interrupt_count)(gp)
	beq     zero, at,   cpu_in_task_ctx_mode
	mov     r2,   zero                              /* set return parameter by not in task context mode */
	ret

cpu_in_task_ctx_mode:
	movi    r2,   1                                 /* set return parameter by in task context mode */
	ret

/* ------------------------------------------------------------------------------
 * _is_task_ctx_mode
 * ------------------------------------------------------------------------------*/
	.global _cpu_lock
_cpu_lock:
	rdctl   at,   status
	andi    r2,   at,   PSR_REG_STATUS_PIE          /* get return parameter by before interrupt state */
	beq     zero, r2,   cpu_lock_end
	xor     at,   at,   r2
	wrctl   status, at                              /* disable cpu hardware interrupt */
cpu_lock_end:
	ret

/* ------------------------------------------------------------------------------
 * _is_task_ctx_mode
 * ------------------------------------------------------------------------------*/
	.global _cpu_unlock
_cpu_unlock:
	ldw     at,   %gprel(g_interrupt_count)(gp)
	bne     zero, at,   cpu_unlock_end
	rdctl   at,   status
	ori     at,   at,   PSR_REG_STATUS_PIE
	wrctl   status, at                              /* enable cpu hardware interrupt */
cpu_unlock_end:
	ret

/* ------------------------------------------------------------------------------
 * _is_task_ctx_mode
 * ------------------------------------------------------------------------------*/
	.global _enable_interrupt
_enable_interrupt:
	rdctl   at,   status
	andi    r2,   at,   PSR_REG_STATUS_PIE
	bne     zero, r2,   enable_interrupt_end
	andi    r4,   r4,   PSR_REG_STATUS_PIE
	or      at,   at,   r4                          /* get before interrupt state */
	wrctl   status, at                              /* restore before interrupt state */
enable_interrupt_end:
	ret
