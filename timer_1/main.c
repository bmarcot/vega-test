#include <stddef.h>
#include <time.h>

#include "unit.h"
#include <kernel/time.h>
#include <kernel/signal.h>

#include <uapi/kernel/signal.h>

#define EXPECTED_VALUE  0xabadcafeul

int timer_create(clockid_t clockid, struct sigevent *sevp,
		timer_t *timerid);
int timer_settime(timer_t timerid, int flags,
		const struct itimerspec *new_value,
		struct itimerspec *old_value);

static volatile int received_signal;

static void event(int sig, siginfo_t *siginfo, void *unused)
{
	(void)sig, (void)unused;

	if (siginfo->si_value.sival_int != (int)EXPECTED_VALUE) {
		printk("error: Did not received expected value (%x != %x)\n",
			siginfo->si_value.sival_int, EXPECTED_VALUE);
		TEST_EXIT(1);
	}
	received_signal = 1;
}

int main()
{
	struct sigaction act = { .sa_sigaction = event,
				 .sa_flags = SA_SIGINFO };
	sigaction(SIGUSR1, &act, NULL);

	struct sigevent sevp = { .sigev_notify = SIGEV_SIGNAL,
				 .sigev_signo = SIGUSR1,
				 .sigev_value.sival_int = EXPECTED_VALUE };
	struct itimerspec val = { .it_value = { .tv_sec = 1, .tv_nsec = 0 } };
	timer_t timerid;

	timer_create(1, &sevp, &timerid);
	timer_settime(timerid, 0, &val, NULL);

	while (!received_signal)
		;

	TEST_EXIT(0);
}
