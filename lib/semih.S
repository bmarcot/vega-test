
	.syntax unified
	.thumb

#include "linux/linkage.h"

	/* http://git.qemu.org/?p=qemu.git;a=blob_plain;f=target-arm/arm-semi.c;hb=HEAD */
	.set SEMIH_EXIT, 0x18

ENTRY(semih_exit)
	mov	r1, r0
	ldr	r0, =SEMIH_EXIT
	bkpt	0xab
0:	b	0b
ENDPROC(semih_exit)
