/*simple signal test */

#include <stddef.h>
#include <signal.h>
#include "kernel.h"
#include "unit.h"

int val;

void handler(int n)
{
	printk("In signal handler, received signal %d\n", n);

	val = 1;
}

int main(void *arg)
{
	(void) arg;

	const struct sigaction act = { .sa_handler = handler, .sa_flags = 0 };

	sigaction(SIGUSR1, &act, NULL);
	raise(SIGUSR1);

	if (!val)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
