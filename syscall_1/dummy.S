
	.syntax unified
	.thumb

#include "linux/linkage.h"

#if __ARM_ARCH == 6 /* __ARM_ARCH_6M__ */
	@ sys	argc, id
#define DEFINE_SYS_CALL_SITE()			\
	.macro sys argc, id		;	\
	.if	\argc == 0		;	\
	movs	r0, #\id		;	\
	.elseif \argc == 1		;	\
	movs	r1, #\id		;	\
	.elseif \argc == 2		;	\
	movs	r2, #\id		;	\
	.elseif \argc == 3		;	\
	movs	r3, #\id		;	\
	.elseif \argc == 4		;	\
	push	{r0}			;	\
	movs	r0, #\id		;	\
	mov	r12, r0			;	\
	pop	{r0}			;	\
	.endif				;	\
	svc	#\argc			;	\
	.endm
#elif __ARM_ARCH == 7 /* __ARM_ARCH_7M__ || __ARM_ARCH_7EM__ */
	@ sys{cond} argc, id
#define DEFINE_SYS_CALL_SITE(cc)		\
	.macro sys##cc argc, id		;	\
	.if	\argc == 0		;	\
	mov##cc r0, #\id		;	\
	.elseif \argc == 1		;	\
	mov##cc r1, #\id		;	\
	.elseif \argc == 2		;	\
	mov##cc r2, #\id		;	\
	.elseif \argc == 3		;	\
	mov##cc r3, #\id		;	\
	.elseif \argc == 4		;	\
	mov##cc r12, #\id		;	\
	.endif				;	\
	svc##cc #\argc			;	\
	.endm
#endif

DEFINE_SYS_CALL_SITE()

#define SYS_PTHREAD_CREATE  3

	/* dummy syscalls for testing-purpose */

ENTRY(dummy_0_arg)
	sys	0, SYS_PTHREAD_CREATE + 1
	bx	lr
ENDPROC(dummy_0_arg)

ENTRY(dummy_1_arg)
	sys	1, SYS_PTHREAD_CREATE + 2
	bx	lr
ENDPROC(dummy_1_arg)

ENTRY(dummy_2_args)
	sys	2, SYS_PTHREAD_CREATE + 3
	bx	lr
ENDPROC(dummy_2_args)

ENTRY(dummy_3_args)
	sys	3, SYS_PTHREAD_CREATE + 4
	bx	lr
ENDPROC(dummy_3_args)

ENTRY(dummy_4_args)
	sys	4, SYS_PTHREAD_CREATE + 5
	bx	lr
ENDPROC(dummy_4_args)
