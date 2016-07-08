#include "kernel.h"
#include "unit.h"
#include <kernel/irqaction.h>

int sysvect_register(unsigned ix, void *(*fn)());
int sys_stage_irqaction(void);

#define SYS_TIMER_CREATE 6
#define SYS_STAGE_IRQACTION (SYS_TIMER_CREATE + 1)

static int counter = 0;

void user_action(void *arg)
{
	printk("In userland irqaction... OK!\n");
	if ((unsigned int) arg != 0xabadcafe) {
		printk("failed: invalid arg (%p != 0xabadcafe).\n", arg);
		return;
	}
	counter++;
}

void *sys_stage_irqaction_1()
{
	struct irqaction action = { .ia_irqaction = user_action };

	stage_irqaction(&action, (void *)0xabadcafe);

	return (void *)0xabadcafe;
}

int main(void *arg)
{
	(void) arg;

	/* We use a syscall to emulate an irq for now, investigate SW Interrupt
	 * feature in the NVIC or target CPU instead.    */
	sysvect_register(SYS_STAGE_IRQACTION, sys_stage_irqaction_1);

	sys_stage_irqaction();
	if (!counter) {
		printk("FAIL: did not enter the user-action\n");
		TEST_EXIT(1);
	}

	TEST_EXIT(0);
}
