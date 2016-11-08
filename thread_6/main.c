/* create/delete 1000 threads */

#include <pthread.h>
#include <stddef.h>

#include "kernel.h"
#include "unit.h"

static void *fn(void *arg)
{
	(void)arg;
	printk(".");

	return 0;
}

int main()
{
	pthread_t thread;

	for (int i = 0; i < 1000; i++) {
		if (pthread_create(&thread, NULL, fn, NULL)) {
			printk("failed: can't create new posix thread.\n");
			TEST_EXIT(1);
		}
		pthread_detach(thread);
		pthread_yield();
	}
	printk("\n");
	TEST_EXIT(0);
}
