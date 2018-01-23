/* child process send SIGCHLD to parent on exit */

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall-wrappers.h"
#include "unit.h"

static int val;

static void sigchld_handler(int sig)
{
	if (sig != SIGCHLD) {
		printk("Received wrong signal\n");
		TEST_EXIT(1);
	}
	val++;
}

int main(void)
{
	pid_t pid;
	struct sigaction act;

	act.sa_handler = sigchld_handler;
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, NULL);

	pid = vfork();
	if (!pid) {
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
