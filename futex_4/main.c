/* Interrupt the futex() syscall */

#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <uapi/kernel/futex.h>
#include "../lib/futex.h"
#include "unit.h"

pid_t getpid(void);
pid_t gettid(void);
int tgkill(int tgid, int tid, int sig);

static pid_t other_tid = -1;
static int signal_serviced;

static void handler(int sig)
{
	signal_serviced = 1;
}

static void *fn(void *arg)
{
	int lock;

	other_tid = gettid();

	// The futex() syscall will be interrupted by the main thread
	if (futex(&lock, FUTEX_WAIT, 0) != -1)
		TEST_EXIT(1);
	if (errno != EINTR)
		TEST_EXIT(1);

	// The handler for the signal must have run after the futex()
	// syscall was interrupted
	if (!signal_serviced)
		TEST_EXIT(1);

	return 0;
}

int main(void)
{
	struct sigaction act = {
		.sa_handler = handler,
		.sa_flags = 0
	};
	if (sigaction(SIGUSR1, &act, NULL))
		TEST_EXIT(1);

	pthread_t thread;
	if (pthread_create(&thread, NULL, fn, NULL))
		TEST_EXIT(1);

	// The other thread startup might be delayed depending on the
	// scheduler's prolicy
	while (other_tid == -1)
		sched_yield();

	// Interrupt the other thread which is blocking in a FUTEX_WAIT
	// operation
	if (tgkill(getpid(), other_tid, SIGUSR1))
		TEST_EXIT(1);

	if (pthread_join(thread, NULL))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
