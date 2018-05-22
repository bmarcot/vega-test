/* create/delete 1000 threads */

#include <pthread.h>
#include <sched.h>
#include <stddef.h>
#include <kernel/kernel.h>
#include "unit.h"

static void *fn(void *arg)
{
	test_printf(".");

	return 0;
}

int main()
{
	pthread_t thread;

	for (int i = 0; i < 1000; i++) {
		if (pthread_create(&thread, NULL, fn, NULL)) {
			test_printf("failed: can't create new posix thread.\n");
			TEST_EXIT(1);
		}
		pthread_detach(thread);
		sched_yield();
	}
	test_printf("\n");
	TEST_EXIT(0);
}
