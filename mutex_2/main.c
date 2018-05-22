#include <sched.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/cdefs.h>

#include "kernel.h"
#include "pthread.h"
#include "unit.h"

pthread_mutex_t lock;
int has_waited, fini;

void *fn(__unused void *arg)
{
	test_printf("thread 2: locking mutex...\n");
	if (pthread_mutex_lock(&lock))
		return NULL;
	has_waited = 1;
	test_printf("thread 2: locked mutex\n");
	if (pthread_mutex_unlock(&lock))
		return NULL;
	test_printf("thread 2: released mutex\n");
	fini = 1;

	return NULL;
}

int main(void)
{
	pthread_t tid;

	pthread_mutex_init(&lock, NULL);

	if (pthread_create(&tid, NULL, fn, NULL))
		test_printf("error: Could not create new thread.\n");

	if (pthread_mutex_lock(&lock))
		TEST_EXIT(1);
	test_printf("thread 1: locked mutex, and yielding...\n");
	sched_yield();
	if (has_waited)
		TEST_EXIT(1);
	test_printf("thread 1: releasing mutex...\n");
	if (pthread_mutex_unlock(&lock))
		TEST_EXIT(1);
	test_printf("thread 1: released mutex\n");

	while (!fini)
		sched_yield();

	/* re-acquire the mutex to check the thread released the mutex
	   correctly */
	if (pthread_mutex_lock(&lock))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
