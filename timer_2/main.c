#include "kernel.h"
#include "pthread.h"
#include "unit.h"
#include "libc/stddef.h"

extern void msleep(unsigned int);

int vals[] = {0, 0, 0, 0};

void *fn(void *arg)
{
	msleep(500 + (int) arg * 500);
	vals[(int) arg]++;
	printk("Thread_%d returns.\n", (int) arg);

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
		while (vals[i] == 0)
			pthread_yield();
	}

	TEST_EXIT(0);
}
