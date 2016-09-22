/* simple sigaction test */

#include <stddef.h>
#include <signal.h>

#include "kernel.h"
#include "unit.h"

int val;

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
	const struct sigaction act = { .sa_sigaction = sigact, .sa_flags = SA_SIGINFO };

	sigaction(SIGUSR1, &act, NULL);
	retval = sigqueue(0, SIGUSR1, (union sigval){ .sival_int = 0xabadcafe });
	if (retval)
		TEST_EXIT(1);
	if (!val)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
