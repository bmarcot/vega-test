/*simple signal test */

#include <stddef.h>
#include <signal.h>
#include "kernel.h"
#include "unit.h"

int val;
const int SIGNO = SIGUSR1;

void handler(int n)
{
	printk("In signal handler, received signal %d\n", n);

	if (n != SIGNO)
		TEST_EXIT(1);
	val = 1;
}

int main(void *arg)
{
	(void) arg;

	const struct sigaction act = { .sa_handler = handler, .sa_flags = 0 };

	sigaction(SIGNO, &act, NULL);
	if (raise(SIGNO))
		TEST_EXIT(1);
	if (!val)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
