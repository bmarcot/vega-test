/* test the monotonic clock */

#include <time.h>
#include <kernel/ktime.h>
#include <kernel/time.h>
#include "unit.h"

int main(void)
{
	struct timespec tp;
	ktime_t kt, kt_old = 0;

	for (int i = 0; i < 1000; i++) {
		msleep(2);
		clock_gettime(CLOCK_MONOTONIC, &tp);
		kt = timespec_to_ktime(tp);
		if (kt <= kt_old)
			TEST_EXIT(1);
		kt_old = kt;
	}

	TEST_EXIT(0);
}
