
	.syntax unified
	.thumb

#include "linux/linkage.h"

	//FIXME: move to asm-pseudo.h
        @ sys{cond}     argc, sys_id
#define DEFINE_SYS_CALL_SITE(cc)                \
        .macro sys##cc argc, id         ;       \
        .ifnb   cc                      ;       \
        itt     cc                      ;       \
        .endif                          ;       \
        .if \argc == 0                  ;       \
        mov##cc r0, #\id                ;       \
        .elseif \argc == 1              ;       \
        mov##cc r1, #\id                ;       \
        .elseif \argc == 2              ;       \
        mov##cc r2, #\id                ;       \
        .elseif \argc == 3              ;       \
        mov##cc r3, #\id                ;       \
        .elseif \argc == 4              ;       \
        mov##cc r12, #\id               ;       \
        .else                           ;       \
        .error "error: Unhandled number of parameters in SysCall." ;    \
        .endif                          ;       \
        svc##cc #\argc                  ;       \
        .endm

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
