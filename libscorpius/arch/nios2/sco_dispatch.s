/* =====================================================================================
 * Copyright (C) 2011-2021 Macnica Inc. All Rights Reserved.
 *
 *  This program is proprietary and confidential. By using this program
 *  you agree to the terms of the associated Macnica Software License Agreement.
 * ------------------------------------------------------------------------------------
 * ! @file
 * ! @brief  This file provides following Nios2 assembler code.
 * !          - start scheduler
 * !          - entrance and exit of interrupt handler
 * !          - task dispatch
 * ===================================================================================== */

/*	.include "../config/arch/nios2/sco_config.inc"
	.include "../arch/nios2/sco_cpu.inc"  */
	.include "./config/arch/nios2/sco_config.inc"
	.include "./arch/nios2/sco_cpu.inc"
	.set noat

/* ------------------------------------------------------------------------------
 * scheduler start
 * ------------------------------------------------------------------------------*/
	.global _start_scheduler
_start_scheduler:
	/* check gp_current_task is valid */
	ldw     et,   %gprel(gp_current_task)(gp)       /* if gp_current_task is NULL */
	bne     zero, et,   start_schedule_task         /* return to main() function if NULL pointer */
	ret

start_schedule_task:
.ifdef SCHED_SUPPORT_TRACE_LOG
	/* set trace log for first task dispatch */
	stw     et,   %gprel(gp_top_priority_task)(gp)  /* copy to gp_top_priority_task from gp_current_task */
	call    i_set_trace_log_dispatch
	stw     zero, %gprel(gp_top_priority_task)(gp)  /* reset gp_top_priority_task to NULL */
.endif                                              /* if defined "SCHED_SUPPORT_TRACE_LOG" */

	/* go to first task */
	ldw     sp,   92(et)                            /* sp = gp_current_task->context.sp */
	ldw     fp,   96(et)                            /* fp = gp_current_task->context.fp */
	ldw     ra,   100(et)                           /* ra = gp_current_task->context.ra */
	ldw     ea,   104(et)                           /* ea = gp_current_task->context.pc */
	ldw     et,   108(et)
	wrctl   estatus, et                             /* estatus = gp_current_task->context.status */
	eret

.ifdef SCHED_SUPPORT_INTERRUPT
/* ------------------------------------------------------------------------------
 * enter to interrupt handler
 * ------------------------------------------------------------------------------*/
	.global _enter_interrupt
_enter_interrupt:
	ldw     et,   %gprel(g_interrupt_count)(gp)     /* set interrupt count */
	addi    et,   et,   1
	stw     et,   %gprel(g_interrupt_count)(gp)     /* g_interrupt_count++ */
	ret

/* ------------------------------------------------------------------------------
 * exit from interrupt handler
 * ------------------------------------------------------------------------------*/
	.global _exit_interrupt
_exit_interrupt:
	/* call dispatcher function */
	call    _dispatch

	/* clear interrupt count */
	ldw     et,   %gprel(g_interrupt_count)(gp)
	subi    et,   et,   1
	stw     et,   %gprel(g_interrupt_count)(gp)     /* g_interrupt_count-- */

	/* restore register file */
	ldw     ra,   8(fp)                             /* restore user mode ra */
	ldw     at,   16(fp)
	ldw     r2,   20(fp)
	ldw     r3,   24(fp)
	ldw     r4,   28(fp)
	ldw     r5,   32(fp)
	ldw     r6,   36(fp)
	ldw     r7,   40(fp)
	ldw     r8,   44(fp)
	ldw     r9,   48(fp)
	ldw     r10,  52(fp)
	ldw     r11,  56(fp)
	ldw     r12,  60(fp)
	ldw     r13,  64(fp)
	ldw     r14,  68(fp)
	ldw     r15,  72(fp)
	ldw     et,   76(fp)
	wrctl   estatus, et                             /* restore user mode status */
	ldw     ea,   80(fp)                            /* restore return address from interrupt */
	ldw     sp,   84(fp)                            /* restore user mode sp */
	ldw     fp,   0(fp)                             /* restore user mode fp */
	eret
.endif                                              /* if defined "SCHED_SUPPORT_INTERRUPT" */

/* ------------------------------------------------------------------------------
 * dispatcher
 * ------------------------------------------------------------------------------*/
	.global _dispatch
_dispatch:
	/* check dispatch condition */
	ldw     at,   %gprel(gp_top_priority_task)(gp)  /* do not dispatch when */
	bne     zero, at,   dispatch1                   /* gp_top_priority_task == NULL */
	ret
dispatch1:
	ldw     et,   %gprel(gp_current_task)(gp)       /* do not dispatch when */
	bne     et,   at,   task_dispatch               /* gp_top_priority_task == gp_current_task */
	stw     zero, %gprel(gp_top_priority_task)(gp)  /* reset gp_top_priority_task to NULL */
	ret

task_dispatch:
.ifdef SCHED_SUPPORT_TRACE_LOG
	/* set trace log for task dispatch */
	subi    sp,   sp,   4
	stw     ra,   0(sp)
	call    i_set_trace_log_dispatch
	ldw     ra,   0(sp)
	addi    sp,   sp,   4
.endif                                              /* if defined "SCHED_SUPPORT_TRACE_LOG" */

.ifdef SCHED_SUPPORT_INTERRUPT
	/* cehck context mode */
	ldw     at,   %gprel(g_interrupt_count)(gp)     /* dispatch in user mode when */
	beq     zero, at,   dispatch_user_mode          /* g_interrupt_count == 0 */
	subi    at,   at,   1
	stw     at,   %gprel(g_interrupt_count)(gp)     /* g_interrupt_count-- */

dispatch_irq_mode:
	/* save current task context */
	ldw     at,   8(fp)
	stw     at,   100(et)                           /* restore and save user mode ra to task context */
	ldw     at,   16(fp)
	stw     at,   0(et)                             /* restore and save user mode r1-r15 to task context */
	ldw     at,   20(fp)
	stw     at,   4(et)
	ldw     at,   24(fp)
	stw     at,   8(et)
	ldw     at,   28(fp)
	stw     at,   12(et)
	ldw     at,   32(fp)
	stw     at,   16(et)
	ldw     at,   36(fp)
	stw     at,   20(et)
	ldw     at,   40(fp)
	stw     at,   24(et)
	ldw     at,   44(fp)
	stw     at,   28(et)
	ldw     at,   48(fp)
	stw     at,   32(et)
	ldw     at,   52(fp)
	stw     at,   36(et)
	ldw     at,   56(fp)
	stw     at,   40(et)
	ldw     at,   60(fp)
	stw     at,   44(et)
	ldw     at,   64(fp)
	stw     at,   48(et)
	ldw     at,   68(fp)
	stw     at,   52(et)
	ldw     at,   72(fp)
	stw     at,   56(et)
	ldw     at,   76(fp)
	stw     at,   108(et)                           /* restore and save user mode status to task context */
	ldw     at,   80(fp)
	stw     at,   104(et)                           /* restore and save user mode pc to task context */
	ldw     at,   84(fp)
	stw     at,   92(et)                            /* restore and save user mode sp to task context */
	ldw     at,   0(fp)
	stw     at,   96(et)                            /* restore and save user mode fp to task context */
	stw     r16,  60(et)                            /* save user mode r16-r23 to task context */
	stw     r17,  64(et)
	stw     r18,  68(et)
	stw     r19,  72(et)
	stw     r20,  76(et)
	stw     r21,  80(et)
	stw     r22,  84(et)
	stw     r23,  88(et)

	br      dispatch_common
.endif                                              /* if defined "SCHED_SUPPORT_INTERRUPT" */

dispatch_user_mode:
	/* save current task context */
	stw     r2,   4(et)
	stw     r3,   8(et)
	stw     r4,   12(et)
	stw     r5,   16(et)
	stw     r6,   20(et)
	stw     r7,   24(et)
	stw     r8,   28(et)
	stw     r9,   32(et)
	stw     r10,  36(et)
	stw     r11,  40(et)
	stw     r12,  44(et)
	stw     r13,  48(et)
	stw     r14,  52(et)
	stw     r15,  56(et)
	stw     r16,  60(et)
	stw     r17,  64(et)
	stw     r18,  68(et)
	stw     r19,  72(et)
	stw     r20,  76(et)
	stw     r21,  80(et)
	stw     r22,  84(et)
	stw     r23,  88(et)
	stw     sp,   92(et)
	stw     fp,   96(et)
	stw     ra,   100(et)
	stw     ra,   104(et)
	rdctl   at,   status
	ori     at,   at,   PSR_REG_STATUS_PIE
	stw     at,   108(et)

dispatch_common:
	/* swap gp_current_task and gp_top_priority_task */
	ldw     et,   %gprel(gp_top_priority_task)(gp)
	stw     et,   %gprel(gp_current_task)(gp)       /* copy to gp_current_task from gp_top_priority_task */
	stw     zero, %gprel(gp_top_priority_task)(gp)  /* reset gp_top_priority_task to NULL */

	/* restore next task context */
	ldw     at,   0(et)
	ldw     r2,   4(et)
	ldw     r3,   8(et)
	ldw     r4,   12(et)
	ldw     r5,   16(et)
	ldw     r6,   20(et)
	ldw     r7,   24(et)
	ldw     r8,   28(et)
	ldw     r9,   32(et)
	ldw     r10,  36(et)
	ldw     r11,  40(et)
	ldw     r12,  44(et)
	ldw     r13,  48(et)
	ldw     r14,  52(et)
	ldw     r15,  56(et)
	ldw     r16,  60(et)
	ldw     r17,  64(et)
	ldw     r18,  68(et)
	ldw     r19,  72(et)
	ldw     r20,  76(et)
	ldw     r21,  80(et)
	ldw     r22,  84(et)
	ldw     r23,  88(et)
	ldw     sp,   92(et)
	ldw     fp,   96(et)
	ldw     ra,   100(et)
	ldw     ea,   104(et)
	ldw     et,   108(et)
	wrctl   estatus, et
	eret                                             /* dispatch to next task context */
