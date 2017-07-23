/* simple test for interval timers */

#include <stddef.h>
#include <time.h>

#include <kernel/time.h>
#include <kernel/signal.h>

#include "unit.h"

#include <uapi/kernel/signal.h>

int timer_create(clockid_t clockid, struct sigevent *sevp,
		timer_t *timerid);
int timer_settime(timer_t timerid, int flags,
		const struct itimerspec *new_value,
		struct itimerspec *old_value);

static volatile int count;

static void event(int sig, siginfo_t *siginfo, void *unused)
{
	(void)sig, (void)unused, (void)siginfo;

	printk("Counter=%d\n", ++count);
}

int main()
{
	struct sigaction act = { .sa_sigaction = event,
				 .sa_flags = SA_SIGINFO, };
	sigaction(SIGUSR1, &act, NULL);

	timer_t timerid;
	struct sigevent sevp = { .sigev_notify = SIGEV_SIGNAL,
				 .sigev_signo = SIGUSR1, };
	struct itimerspec val = {
		.it_value    = { .tv_sec = 1, .tv_nsec = 0 },
		.it_interval = { .tv_sec = 1, .tv_nsec = 0 },
	};

	timer_create(0, &sevp, &timerid);
	timer_settime(timerid, 0, &val, NULL);
	while (count < 4)
		;

	TEST_EXIT(0);
}
