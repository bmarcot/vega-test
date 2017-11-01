/* test task's TID, PID */

#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <kernel/kernel.h>
#include "unit.h"

pid_t gettid(void);

static void *fn(void *arg)
{
	if (gettid() == getpid()) {
		printk("failed: clone thread TID is not PID\n");
		TEST_EXIT(1);
	}

	return 0;
}

int main()
{
	pthread_t tip;

	if (gettid() != getpid()) {
		printk("failed: leader thread TID must equal PID\n");
		TEST_EXIT(1);
	}

	if (pthread_create(&tip, NULL, fn, NULL)) {
		printk("failed: can't create new posix thread.\n");
		TEST_EXIT(1);
	}

	if (sched_yield()) {
		printk("failed: can't yield cpu to another thread.\n");
		TEST_EXIT(1);
	}

	TEST_EXIT(0);
}
