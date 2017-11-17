#include <errno.h>
#include <unistd.h>

#include <stddef.h>
#include <time.h>
#include <signal.h>

#include "unit.h"
#include <kernel/time.h>
#include <kernel/signal.h>

#include <uapi/kernel/signal.h>

int timer_create(clockid_t clockid, struct sigevent *sevp,
		timer_t *timerid);
int timer_settime(timer_t timerid, int flags,
		const struct itimerspec *new_value,
		struct itimerspec *old_value);
int pause(void);

static int i;

static void event()
{
	i++;
}

int main(void)
{
	struct sigaction act;
	struct sigevent sevp;
	struct itimerspec val;
	timer_t timerid = {0};

	act.sa_sigaction = event;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);

	sevp.sigev_notify = SIGEV_SIGNAL;
	sevp.sigev_signo = SIGUSR1;

	val.it_value.tv_sec = 1;
	timer_create(1, &sevp, &timerid);
	timer_settime(timerid, 0, &val, NULL);

	if (pause() != -EINTR)
		TEST_EXIT(1);
	if (!i)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
