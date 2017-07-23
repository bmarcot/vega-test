/* test cancellation of timer */

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

static volatile int canary;
static volatile int count;


static void event_should_not_happen(int sig, siginfo_t *siginfo,
				void *unused)
{
	(void)sig, (void)unused, (void)siginfo;

	canary++;
}

static void event_check(int sig, siginfo_t *siginfo,
				void *unused)
{
	(void)sig, (void)unused, (void)siginfo;

	count++;
}

int main()
{
	struct sigaction act_a = { .sa_sigaction = event_should_not_happen,
				   .sa_flags = SA_SIGINFO, };
	sigaction(SIGUSR1, &act_a, NULL);
	timer_t timerid_a;
	struct sigevent sevp_a = { .sigev_notify = SIGEV_SIGNAL,
				   .sigev_signo = SIGUSR1, };
	timer_create(0, &sevp_a, &timerid_a);

	struct sigaction act_b = { .sa_sigaction = event_check,
				   .sa_flags = SA_SIGINFO, };
	sigaction(SIGUSR2, &act_b, NULL);
	timer_t timerid_b;
	struct sigevent sevp_b = { .sigev_notify = SIGEV_SIGNAL,
				   .sigev_signo = SIGUSR2, };
	timer_create(0, &sevp_b, &timerid_b);

	struct itimerspec val_a = {
		.it_value    = { .tv_sec = 1, .tv_nsec = 0 },
		.it_interval = { .tv_sec = 0, .tv_nsec = 0 },
	};
	struct itimerspec val_b = {
		.it_value    = { .tv_sec = 2, .tv_nsec = 0 },
		.it_interval = { .tv_sec = 0, .tv_nsec = 0 },
	};
	struct itimerspec val_zero = {
		.it_value    = { .tv_sec = 0, .tv_nsec = 0 },
		.it_interval = { .tv_sec = 0, .tv_nsec = 0 },
	};
	timer_settime(timerid_a, 0, &val_a, NULL);
	timer_settime(timerid_b, 0, &val_b, NULL);
	timer_settime(timerid_a, 0, &val_zero, NULL);

	while (!count)
		;
	if (canary)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
