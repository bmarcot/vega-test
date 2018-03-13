/* test blocked signal, test unblocking pending signal */

#include <signal.h>
#include <unistd.h>

#include <kernel/signal.h> //XXX: sigaddset()

#include "syscall-wrappers.h"
#include "unit.h"

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

static int val;

static void handler(int sig)
{
	val++;
}

int main(void)
{
	struct sigaction act;

	act.sa_handler = handler;
	act.sa_flags = 0;
	sigaction(SIGUSR1, &act, NULL);

	sigset_t m;
	sigaddset(&m, SIGUSR1);

	// blocked signals are pending
	sigprocmask(SIG_SETMASK, &m, NULL);
	raise(SIGUSR1);
	if (val) {
		printk("SIGUSR1 is blocked\n");
		TEST_EXIT(1);
	}

	// unblocked pending signals are taken
	sigprocmask(SIG_UNBLOCK, &m, NULL);
	if (!val) {
		printk("SIGUSR1 is unblocked\n");
		TEST_EXIT(1);
	}

	TEST_EXIT(0);
}
