/* test a simple high-resolution timer */

#include <kernel/hrtimer.h>

#include "unit.h"

volatile int passed;

enum hrtimer_restart function(struct hrtimer *timer)
{
	passed = 1;

	return HRTIMER_NORESTART;
}

int main()
{
	static struct hrtimer timer = {.function = function};

	hrtimer_init(&timer);

#define NSEC_PER_SEC 1000000000l
	hrtimer_start(&timer, 1ll * NSEC_PER_SEC);

	while (!passed)
		;

	TEST_EXIT(0);
}
