/* test stack robustness */

#include <stddef.h>
#include <signal.h>
#include "unit.h"

int count;
int signo = SIGUSR1;

void handler(int sig)
{
	if (sig != signo)
		TEST_EXIT(1);
	count++;
}

int main(void *arg)
{
	(void) arg;

	const struct sigaction act = { .sa_handler = handler,
				       .sa_flags = 0, };
	sigaction(signo, &act, NULL);

	int retval;
	do {
		retval = raise(signo);
		if (retval) {
			printk("Syscall returned %d, expected 0\n", retval);
			TEST_EXIT(1);
		}
	} while (count < 10000);

	TEST_EXIT(0);
}
