/* test a simple high-resolution timer */

#include <kernel/time/hrtimer.h>

#include "unit.h"

volatile int passed;

static void callback(void *context)
{
	if ((unsigned int)context != 0xbeefcafe)
		TEST_EXIT(1);
	passed = 1;
}

int main()
{
	static struct hrtimer timer = {
		.callback = callback,
		.context = (void *)0xbeefcafe,
	};

	hrtimer_init(&timer);

#define NSEC_PER_SEC 1000000000l
	hrtimer_set_expires(&timer, 1ll * NSEC_PER_SEC);

	while (!passed)
		;

	TEST_EXIT(0);
}
