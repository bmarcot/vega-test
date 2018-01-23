/* calling vfork() multiple times won't deplete the memory */

#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall-wrappers.h"
#include "unit.h"

int main(void)
{
	pid_t pid;
	static int i = 0;

	while (i < 200000) {
		pid = vfork();
		if (!pid) {
			_Exit(0);
		} else {
			if (pid != waitpid(pid, NULL, 0))
				TEST_EXIT(1);
			i++;
		}
	}

	TEST_EXIT(0);
}
