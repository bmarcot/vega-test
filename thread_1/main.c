// simple thread create and thread yield

#include "kernel.h"
#include "pthread.h"
#include "unit.h"

static int val = 123;

static void *fn(void *arg)
{
	val += (int) arg;

	return 0;
}

int main()
{
	pthread_t tip;

	if (pthread_create(&tip, fn, (void *) 321)) {
		printk("failed: can't create new posix thread.\n");
		TEST_EXIT(1);
	}

	if (pthread_yield()) {
		printk("failed: can't yield cpu to another thread.\n");
		TEST_EXIT(1);
	}

	while (val != 444)
		;

	TEST_EXIT(0);
}
