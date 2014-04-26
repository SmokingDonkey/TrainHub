/*
 * FLASHTest_POST_reset.asm
 *
 *  Created on: 30.08.2011
 *      Author: nxp28536
 */

.text
.syntax unified

/* declare the exported routine name */
/* gets plugged into the reset vector */
.global _FLASHTest_POST_reset

/* reference the two routines which need to be called */
.extern  ResetISR
.extern _FLASHTestPOST

.thumb
.thumb_func

/* defines a 'trampoline' assembly function for executing the FLASH test */
_FLASHTest_POST_reset:
	ldr   r0,=_FLASHTestPOST	/* execute the flash post test */
	blx	r0
	ldr   r0,=ResetISR			/* now jump to the application Reset ISR */
	bx r0

.align
.end
