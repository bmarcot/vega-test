/* simple vfork() test */

#include <pthread.h>
#include <unistd.h>

#include "syscall-wrappers.h"

#include "unit.h"

int main(void)
{
	pid_t pid;
	static int count = 0;

	pid = vfork();
	printk("In %s process (pid=%d)\n", pid > 0 ? "parent" : "child",
		getpid());

	if (pid > 0) {
		/* child should exit before its parent */
		if (!count)
			TEST_EXIT(1);
		TEST_EXIT(0);
	} else {
		count++;
		_Exit(0);
	}
}
