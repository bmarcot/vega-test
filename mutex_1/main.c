/* simple mutex lock/unlock */

#include <stddef.h>
#include "kernel.h"
#include "pthread.h"
#include "unit.h"

int main(void)
{
	pthread_mutex_t lock;

	pthread_mutex_init(&lock, NULL);
	if (pthread_mutex_lock(&lock))
		TEST_EXIT(1);
	if (pthread_mutex_unlock(&lock))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
