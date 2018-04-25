/* create THREAD_COUNT threads, then join those threads */

#include <pthread.h>
#include "unit.h"

#define THREAD_COUNT 24

static int counter;

void *fn(void *arg)
{
	counter++;

	return 0;
}

int main()
{
	pthread_t tips[THREAD_COUNT];

	/* reduce stack size to 256 bytes */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 256);

	for (int i = 0; i < THREAD_COUNT; i++) {
		if (pthread_create(&tips[i], &attr, fn, NULL)) {
			printk("failed: can't create new posix thread.\n");
			TEST_EXIT(1);
		}
	}

	for (int i = 0; i < THREAD_COUNT; i++)
		pthread_join(tips[i], NULL);

	if (counter != THREAD_COUNT)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
