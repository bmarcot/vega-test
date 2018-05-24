/*
 * Test functions:
 *   timer_delete
 *
 * Description:
 *   - Block SIGUSR1
 *   - Do loop:
 *       - Create a timer raising SIGUSR1
 *       - Set timer and wait for expire (signal becomes pending)
 *       - Delete timer
 */

#include <stddef.h>
#include <time.h>
#include <signal.h>

#include <kernel/signal.h> //XXX: sigaddset(), etc
#include "missing_syscalls.h"
#include "unit.h"

static void handler(int sig)
{
	test_printf("Signal should not be delivered!");
	TEST_EXIT(1);
}

int main()
{
	struct sigaction sa = {.sa_handler = handler};
	sigset_t m;
	timer_t timerid;
	struct sigevent sevp = {.sigev_notify = SIGEV_SIGNAL,
				.sigev_signo = SIGUSR1};
	struct itimerspec val = {0};

	sigaction(SIGUSR1, &sa, NULL);

	sigemptyset(&m);
	sigaddset(&m, SIGUSR1);
	sigprocmask(SIG_SETMASK, &m, NULL);

	val.it_value.tv_nsec = 250000;

	for (int i = 0; i < 2000; i++) {
		timer_create(1, &sevp, &timerid);
		timer_settime(timerid, 0, &val, NULL);
		do {
			sigpending(&m);
		} while (!sigismember(&m, SIGUSR1));
		timer_delete(timerid);

		/*
		 * Signal was not delivered, and is not pending
		 * after deletion of the timer
		 */
		sigpending(&m);
		if (sigismember(&m, SIGUSR1))
			TEST_EXIT(1);
	}

	TEST_EXIT(0);
}
