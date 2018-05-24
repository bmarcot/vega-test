/* test pending signals */

#include <signal.h>

#include <kernel/signal.h> //XXX: sigaddset()
#include "missing_syscalls.h"
#include "unit.h"

static void handler(int sig)
{
	test_printf("Signal should not be delivered!");
	TEST_EXIT(1);
}

int main(void)
{
	struct sigaction sa = {.sa_handler = handler};
	sigset_t m;

	sigaction(SIGUSR1, &sa, NULL);

	sigemptyset(&m);
	sigaddset(&m, SIGUSR1);
	sigprocmask(SIG_SETMASK, &m, NULL);

	raise(SIGUSR1);

	sigpending(&m);
	if (!sigismember(&m, SIGUSR1))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
