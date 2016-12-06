#include <kernel/linkage.h>

	.syntax unified
	.thumb

ENTRY(test_svcall0)
	ldr	r0, =42
	svc	#0
	bx	lr
ENDPROC(test_svcall0)

ENTRY(test_svcall1)
	ldr	r0, =0xbabe
	ldr	r1, =43
	svc	#1
	bx	lr
ENDPROC(test_svcall1)

ENTRY(test_svcall2)
	ldr	r0, =0xbabe
	ldr	r1, =0xbeef
	ldr	r2, =44
	svc	#2
	bx	lr
ENDPROC(test_svcall2)

ENTRY(test_svcall3)
	ldr	r0, =0xbabe
	ldr	r1, =0xbeef
	ldr	r2, =0x123
	ldr	r3, =45
	svc	#3
	bx	lr
ENDPROC(test_svcall3)

ENTRY(test_svcall4)
	ldr	r3, =46
	push	{r3}
	ldr	r0, =0xbabe
	ldr	r1, =0xbeef
	ldr	r2, =0x123
	ldr	r3, =0x456
	svc	#4
	add	sp, #4
	bx	lr
ENDPROC(test_svcall4)

ENTRY(test_svcall5)
	ldr	r2, =0xcafe
	ldr	r3, =47
	push	{r2, r3}
	ldr	r0, =0xbabe
	ldr	r1, =0xbeef
	ldr	r2, =0x123
	ldr	r3, =0x456
	svc	#5
	add	sp, #8
	bx	lr
ENDPROC(test_svcall5)
