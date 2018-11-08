/* Futex WAIT operation shall wake-up a list of waiters */

#include <pthread.h>
#include <sched.h>
#include <uapi/kernel/futex.h>
#include "unit.h"
#include "../lib/futex.h"

#define THREAD_COUNT 4

static int counter, lock;

void *fn(void *arg)
{
	counter++;
	futex(&lock, FUTEX_WAIT, 0);

	return 0;
}

int main()
{
	pthread_t tips[THREAD_COUNT];

	for (int i = 0; i < THREAD_COUNT; i++) {
		if (pthread_create(&tips[i], NULL, fn, NULL))
			TEST_EXIT(1);
	}

	while (counter != 4)
		sched_yield();

	lock = 1;
	int r = futex(&lock, FUTEX_WAKE, THREAD_COUNT);
	if (r != THREAD_COUNT)
		TEST_EXIT(1);

	for (int i = 0; i < THREAD_COUNT; i++)
		pthread_join(tips[i], NULL);

	TEST_EXIT(0);
}
