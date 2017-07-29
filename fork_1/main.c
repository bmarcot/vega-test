/* a simple fork() test */

#include <pthread.h>
#include <unistd.h>

#include "unit.h"

int count;

static void _Exit(int status)
{
	pthread_exit((void *)status);
}

int main(void)
{
	pid_t pid;

	pid = fork();
	printk("In %s process (pid=%d)\n", pid > 0 ? "parent" : "child",
		getpid());

	count++;
	while (count < 2)
		sched_yield();

	if (pid > 0)
		TEST_EXIT(0);
	else
		_Exit(0);
}
