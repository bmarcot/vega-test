/* calling vfork() multiple times won't deplete the memory */

#include <pthread.h>
#include <unistd.h>

#include "syscall-wrappers.h"

#include "unit.h"

int main(void)
{
	pid_t pid;
	static int i = 0;

	while (i < 100000) {
		pid = vfork();
		if (!pid)
			_exit(0);
		else
			i++;
	}

	TEST_EXIT(0);
}
