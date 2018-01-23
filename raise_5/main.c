/* sending SIGKILL to self exit the task */

#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall-wrappers.h"
#include "unit.h"

int main(void)
{
	pid_t pid;

	pid = vfork();
	if (pid > 0) {
		if (pid != waitpid(pid, NULL, 0))
			TEST_EXIT(1);
	} else {
		if (raise(SIGKILL))
			TEST_EXIT(1);
		TEST_EXIT(1); /* never reached */
	}

	TEST_EXIT(0);
}
