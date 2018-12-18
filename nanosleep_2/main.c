/* Interrupt the nanosleep() syscall */

#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <time.h>
#include "unit.h"

pid_t getpid(void);
pid_t gettid(void);
int nanosleep(const struct timespec *req, struct timespec *rem);
int tgkill(int tgid, int tid, int sig);

static pid_t other_tid = -1;
static int signal_serviced;

static void handler(int sig)
{
	signal_serviced = 1;
}

static void *fn(void *arg)
{
	struct timespec rem, req = {
		.tv_sec = 1,
		.tv_nsec = 0
	};

	other_tid = gettid();

	// The syscall will be interrupted by the main thread
	if (nanosleep(&req, &rem) != -1)
		TEST_EXIT(1);
	if (errno != EINTR)
		TEST_EXIT(1);

	// The handler for the signal must have run after the nanosleep()
	// syscall was interrupted
	if (!signal_serviced)
		TEST_EXIT(1);

	// Test is interrupted before timer has expired, there must
	// be some time remaining
	if (!rem.tv_sec && !rem.tv_nsec)
		TEST_EXIT(1);

	return 0;
}

int main(void)
{
	struct sigaction act = {
		.sa_handler = handler,
		.sa_flags = 0,
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

	// Interrupt the other thread which is blocking in a sleep operation
	if (tgkill(getpid(), other_tid, SIGUSR1))
		TEST_EXIT(1);

	// Join to make sure there other thread was interrupted with
	// some time remaining
	if (pthread_join(thread, NULL))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
