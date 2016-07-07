#include "kernel.h"
#include "pthread.h"
#include "unit.h"
#include "linux/stddef.h"

int vals[] = {0, 0, 0, 0};

void *fn(void *arg)
{
	for (;;) {
		vals[(int) arg]++;
		pthread_yield();
	}

	return 0;
}

int main()
{
	pthread_t tips[4];

	for (int i = 0; i < 4; i++) {
		if (pthread_create(&tips[i], NULL, fn, (void *) i)) {
			printk("failed: can't create new posix thread.\n");
			TEST_EXIT(1);
		}
	}

	for (int i = 0; i < 4; i++) {
		while (vals[i] < 10)
			pthread_yield();
	}

	TEST_EXIT(0);
}
