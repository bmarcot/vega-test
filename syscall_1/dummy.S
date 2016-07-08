
	.syntax unified
	.thumb

#include "linux/linkage.h"

#define SYS_PTHREAD_CREATE  3

	/* dummy syscalls for testing-purpose */

ENTRY(dummy_0_arg)
	svc	#SYS_PTHREAD_CREATE + 1
	bx	lr
ENDPROC(dummy_0_arg)

ENTRY(dummy_1_arg)
	svc	#SYS_PTHREAD_CREATE + 2
	bx	lr
ENDPROC(dummy_1_arg)

ENTRY(dummy_2_args)
	svc	#SYS_PTHREAD_CREATE + 3
	bx	lr
ENDPROC(dummy_2_args)

ENTRY(dummy_3_args)
	svc	#SYS_PTHREAD_CREATE + 4
	bx	lr
ENDPROC(dummy_3_args)

ENTRY(dummy_4_args)
	svc	#SYS_PTHREAD_CREATE + 5
	bx	lr
ENDPROC(dummy_4_args)
