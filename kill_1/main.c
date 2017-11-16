/* one process can send signal (kill) to another process */

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall-wrappers.h"
#include "unit.h"

pid_t getppid(void);
int kill(pid_t pid, int sig);

static int val;
static pid_t ppid;

static void handler(int sig)
{
	if (ppid != getpid()) {
		printk("Signal was sent to parent\n");
		TEST_EXIT(1);
	}
	val++;
}

int main(void)
{
	pid_t pid;
	struct sigaction act;

	act.sa_handler = handler;
	act.sa_flags = 0;
	sigaction(SIGUSR1, &act, NULL);

	ppid = getpid();
	pid = vfork();
	if (!pid) {
		kill(getppid(), SIGUSR1);
		_Exit(0);
	} else {
		waitpid(pid, NULL, 0);
		if (!val) {
			printk("Signal not received\n");
			TEST_EXIT(1);
		}
	}

	TEST_EXIT(0);
}
