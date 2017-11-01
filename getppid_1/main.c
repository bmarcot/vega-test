/* test task's parent PID */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall-wrappers.h"
#include "unit.h"

pid_t getppid(void);

pid_t parent_pid;
pid_t child_pid;

int main(void)
{
	pid_t pid;

	parent_pid = getpid();
	pid = vfork();
	if (pid > 0) {
		if (pid != child_pid)
			TEST_EXIT(1);
		waitpid(pid, NULL, 0);
	} else {
		child_pid = getpid();
		if (getppid() != parent_pid)
			TEST_EXIT(1);
		_Exit(0);
	}

	TEST_EXIT(0);
}
