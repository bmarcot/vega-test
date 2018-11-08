/* Wait and wake-up many times */

#include <pthread.h>
#include <sched.h>
#include <uapi/kernel/futex.h>
#include "unit.h"
#include "../lib/futex.h"

static int lock;

void *fn(void *arg)
{
	for (;;) {
		futex(&lock, FUTEX_WAIT, 0);
		lock = 0;
	}

	return 0;
}

int main()
{
	pthread_t t;

	if (pthread_create(&t, NULL, fn, NULL))
		TEST_EXIT(1);

	for (int i = 0; i < 30000; i++) {
		sched_yield();
		lock = 1;
		futex(&lock, FUTEX_WAKE, 1);
	}

	TEST_EXIT(0);
}
