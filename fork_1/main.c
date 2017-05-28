/* a simple fork() test */

#include <pthread.h>
#include <unistd.h>

#include "unit.h"

static void _Exit(int status)
{
	pthread_exit((void *)status);
}

int main(void)
{
	pid_t pid;

	pid = fork();
	if (pid > 0)
		sched_yield();
	printk("In %s process (pid=%d)\n", pid > 0 ? "parent" : "child",
		getpid());

	if (pid > 0)
		TEST_EXIT(0);
	else
		_Exit(0);
}
