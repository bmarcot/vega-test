#pragma once

#include <signal.h>
#include <time.h>

/*
 * <time.h>
 */

int timer_create(clockid_t clockid, struct sigevent *sevp,
		timer_t *timerid);
int timer_settime(timer_t timerid, int flags,
		const struct itimerspec *new_value,
		struct itimerspec *old_value);
int timer_delete(timer_t timerid);

/*
 * <signal.h>
 */

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigpending(sigset_t *set);
