/* exit_group() can exit process from non-leader thread */

#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall-wrappers.h"
#include "unit.h"

int pause(void);

static void *start0(void *arg)
{
	_Exit(42);
}

int main(void)
{
	int status;
	pid_t pid;
	pthread_t thread0;

	pid = vfork();
	if (pid > 0) {
		if (waitpid(pid, &status, 0) == -1)
			TEST_EXIT(1);
		if (WEXITSTATUS(status) != 42)
			TEST_EXIT(1);
	} else {
		if (pthread_create(&thread0, NULL, start0, NULL))
			TEST_EXIT(1);
		pause();
		_Exit(0);
	}

	TEST_EXIT(0);
}
