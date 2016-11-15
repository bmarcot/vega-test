/* simple sigaction test */

#include <stddef.h>
#include <signal.h>

#include "kernel.h"
#include "unit.h"

#include "cmsis/arm/ARMCM4.h"

int val;

void handler(int sig)
{
	printk("In signal handler, received signal %d\n", sig);

	if (sig != SIGUSR2)
		TEST_EXIT(1);
	val = 1;
}

void sigact(int sig, siginfo_t *siginfo, void *unused)
{
	(void)unused;

	printk("In signal sigaction, received signal %d with value 0x%x\n",
		sig, siginfo->si_value.sival_int);

	if (sig != SIGUSR1)
		TEST_EXIT(1);
	if ((unsigned int)siginfo->si_value.sival_int != 0xabadcafe)
		TEST_EXIT(1);

	val = 1;
}

int main(void *arg)
{
	(void) arg;

	int retval;
	unsigned long sp = __get_PSP();

	/* test handler with SA_SIGINFO attached */
	const struct sigaction act = { .sa_sigaction = sigact, .sa_flags = SA_SIGINFO };
	sigaction(SIGUSR1, &act, NULL);
	retval = sigqueue(0, SIGUSR1, (union sigval){ .sival_int = 0xabadcafe });
	if (retval) {
		printk("error: %d: incorrect return value\n", retval);
		TEST_EXIT(1);
	}
	if (sp != __get_PSP()) {
		printk("error: stack not correctly restored\n");
		TEST_EXIT(1);
	}
	if (!val)
		TEST_EXIT(1);

	/* test simple handler */
	val = 0; /* reset marker */
	const struct sigaction _act = { .sa_handler = handler, .sa_flags = 0 };
	sigaction(SIGUSR2, &_act, NULL);
	if (raise(SIGUSR2))
		TEST_EXIT(1);
	if (!val)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
