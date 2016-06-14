
	.syntax unified
	.thumb

#include "linux/linkage.h"
#include "../pseudo.S"

#define SYS_TIMER_CREATE 6

	@ int sys_stage_irqaction(void)
ENTRY(sys_stage_irqaction)
	sys	0, SYS_TIMER_CREATE+1
	bx	lr
ENDPROC(sys_stage_irqaction)
