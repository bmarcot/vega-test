/* simple create  multiple concurrent timers */

#include <signal.h>
#include <stddef.h>
#include <time.h>

#include "kernel.h"
#include "unit.h"

#include <uapi/kernel/signal.h>

int timer_create(clockid_t clockid, struct sigevent *sevp,
		timer_t *timerid);
int timer_settime(timer_t timerid, int flags,
		const struct itimerspec *new_value,
		struct itimerspec *old_value);

static volatile int vals[] = { 0, 0, 0, 0 };

static void event(int sig, siginfo_t *siginfo, void *unused)
{
	(void)sig, (void)unused;

	printk("In event %d.\n", siginfo->si_value.sival_int);
	vals[siginfo->si_value.sival_int] = 1;
}

int main(void)
{
	struct sigaction act = { .sa_sigaction = event,
				 .sa_flags = SA_SIGINFO, };
	sigaction(SIGUSR1, &act, NULL);

	struct sigevent sevp = { .sigev_notify = SIGEV_SIGNAL,
				 .sigev_signo = SIGUSR1, };
	timer_t timerid[4];

	for (int i = 0; i < 4; i++)
	{
		printk("Creating timer %d...\n", i);
		sevp.sigev_value.sival_int = i;
		timer_create(0, &sevp, &timerid[i]);

		unsigned long val_in_nsecs = 750 * (i + 1) * 1000000;
		struct itimerspec val = {
			.it_value = { .tv_sec = val_in_nsecs / 1000000000,
				      .tv_nsec = val_in_nsecs % 1000000000 } };
		timer_settime(timerid[i], 0, &val, NULL);
	}
	printk("All timers armed.\n");
	for (int i = 0; i < 4; i++) {
		while (vals[i] == 0)
			;
	}

	TEST_EXIT(0);
}
