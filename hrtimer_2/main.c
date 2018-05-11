/* test concurrent high-resolution timers */

#include <sched.h>

#include <kernel/hrtimer.h>
#include <kernel/kernel.h>

#include "unit.h"

static struct hrtimer timer[5];
static int count;

enum hrtimer_restart function(struct hrtimer *ht)
{
	static unsigned int expect = 0;

	if (ARRAY_INDEX(ht, timer) != expect++)
		TEST_EXIT(1);
	count++;

	return HRTIMER_NORESTART;
}

int main()
{
	for (int i = 0; i < 5; i++) {
		hrtimer_init(&timer[i]);
		timer[i].function = function;
	}

#define NSEC_PER_MSEC	1000000l
	hrtimer_start(&timer[3],  900ll * NSEC_PER_MSEC); //  900 msecs
	hrtimer_start(&timer[1],  500ll * NSEC_PER_MSEC); //  500 msecs
	hrtimer_start(&timer[0],  250ll * NSEC_PER_MSEC); //  250 msecs
	hrtimer_start(&timer[2],  700ll * NSEC_PER_MSEC); //  700 msecs
	hrtimer_start(&timer[4], 1000ll * NSEC_PER_MSEC); // 1000 msecs

	while (count != 5)
		sched_yield();

	TEST_EXIT(0);
}
