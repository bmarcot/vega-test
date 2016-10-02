// simple thread create and thread yield

#include "kernel.h"
#include "pthread.h"
#include "unit.h"
#include "linux/stddef.h"

static int val;

static void *fn(void *arg)
{
	(void)arg;

	msleep(600);
	val++;

	return 0;
}

int main()
{
	pthread_t tip;

	for (int i = 0; i < 3; i++) {
		if (pthread_create(&tip, NULL, fn, NULL)) {
			printk("failed: can't create new posix thread.\n");
			TEST_EXIT(1);
		}
	}

	msleep(600);  /* 4 threads are sitting in the sleepqueue */

	while (val != 3)
		;

	TEST_EXIT(0);
}
