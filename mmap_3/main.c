/* memory allocated with mmap() is released after process has exited */

#include <stddef.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall-wrappers.h"
#include "unit.h"

int main(void)
{
	static int i = 0;

	while (i < 10000) {
		pid_t pid = vfork();
		if (!pid) {
			if (mmap(0, 256, 0, MAP_ANONYMOUS, 0, 0) == MAP_FAILED)
				TEST_EXIT(1);
			_Exit(0);
		} else {
			if (pid != waitpid(pid, NULL, 0))
				TEST_EXIT(1);
			i++;
		}
	}

	TEST_EXIT(0);
}
