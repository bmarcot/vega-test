/* child process send SIGCHLD to parent on exit */

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall-wrappers.h"
#include "unit.h"

#define EXIT_CODE 123

static int val;
static pid_t childpid;

static void sigchld_handler(int sig, siginfo_t *info, void *unused)
{
	if (sig != SIGCHLD) {
		printk("Expect SIGCHLD\n\n");
		TEST_EXIT(1);
	}
	if (info->si_signo != SIGCHLD) {
		printk("Wrong signal number (%d)\n", info->si_signo);
		TEST_EXIT(1);
	}
	if (info->si_code != CLD_EXITED) {
		printk("Wrong signal code (%d)\n", info->si_code);
		TEST_EXIT(1);
	}
	if (info->_sigchld.si_status != EXIT_CODE) {
		printk("Wrong status value (%d)\n", info->_sigchld.si_status);
		TEST_EXIT(1);
	}
	childpid = info->_sigchld.si_pid;
	val++;
}

int main(void)
{
	const struct sigaction act = { .sa_sigaction = sigchld_handler,
				       .sa_flags = SA_SIGINFO };
	sigaction(SIGCHLD, &act, NULL);

	pid_t pid = vfork();
	if (!pid) {
		_Exit(EXIT_CODE);
	} else {
		waitpid(pid, NULL, 0);
		if (pid != childpid) {
			printk("Wrong child PID (%d != %d)\n", pid, childpid);
			TEST_EXIT(1);
		}
		if (!val) {
			printk("Signal not received\n");
			TEST_EXIT(1);
		}
	}

	TEST_EXIT(0);
}
