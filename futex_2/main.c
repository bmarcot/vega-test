/* Not setting the lock does not wake-up a waiter */

#include <pthread.h>
#include <sched.h>
#include <uapi/kernel/futex.h>
#include "unit.h"
#include "../lib/futex.h"

static int counter, lock;

void *fn(void *arg)
{
	futex(&lock, FUTEX_WAIT, 0);
	counter = 1;

	return 0;
}

int main()
{
	pthread_t t;

	if (pthread_create(&t, NULL, fn, NULL))
		TEST_EXIT(1);
	sched_yield();

	/* Shall not wake-up */
	futex(&lock, FUTEX_WAKE, 4);
	sched_yield();
	if (counter)
		TEST_EXIT(1);

	/* Wake-up the waiter */
	lock = 1;
	futex(&lock, FUTEX_WAKE, 4);

	pthread_join(t, NULL);

	TEST_EXIT(0);
}
