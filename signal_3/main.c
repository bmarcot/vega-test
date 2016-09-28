/* test signal error return code */

#include <signal.h>
#include <kernel/signal.h>
#include "kernel.h"
#include "unit.h"
#include "cmsis/arm/ARMCM4.h"

int main(void *arg)
{
	(void) arg;

	union sigval value = { .sival_int = 0 };
	int retval;
	unsigned long sp = __get_PSP();

	/* test ERR_SIGNAL_UNSUPPORTED */
	retval = sigqueue(0, SIGMAX + 1, value);
	if (retval != ERR_SIGNAL_UNSUPPORTED) {
		printk("error: expected UNSUPPORTED_SIGNAL retcode\n");
		TEST_EXIT(1);
	}

	/* test ERR_SIGNAL_UNHANDLED */
	retval = sigqueue(0, SIGUSR1, value);
	if (retval != ERR_SIGNAL_UNHANDLED) {
		printk("error: expected UNHANDLED_SIGNAL retcode\n");
		TEST_EXIT(1);
	}

	if (sp != __get_PSP()) {
		printk("error: stack not correctly restored\n");
		TEST_EXIT(1);
	}

	TEST_EXIT(0);
}
