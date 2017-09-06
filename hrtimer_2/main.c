/* test concurrent high-resolution timers */

#include <kernel/time/hrtimer.h>

#include "unit.h"

volatile int counter;

static void callback(void *context)
{
	static int expected = 0;

	if ((int)context != expected++)
		TEST_EXIT(1);
	counter++;
}

int main()
{
	struct hrtimer *timers[5];

	for (int i = 0; i < 5; i++) {
		timers[i] = hrtimer_alloc();
		if (!timers[i])
			TEST_EXIT(1);
		timers[i]->callback = callback;
		timers[i]->context = (void *)i;
	}

#define NSEC_PER_MSEC	1000000l
	hrtimer_set_expires(timers[3],  900ll * NSEC_PER_MSEC); //  900 msecs
	hrtimer_set_expires(timers[1],  500ll * NSEC_PER_MSEC); //  500 msecs
	hrtimer_set_expires(timers[0],  250ll * NSEC_PER_MSEC); //  250 msecs
	hrtimer_set_expires(timers[2],  700ll * NSEC_PER_MSEC); //  700 msecs
	hrtimer_set_expires(timers[4], 1000ll * NSEC_PER_MSEC); // 1000 msecs

	while (counter != 5)
		;

	TEST_EXIT(0);
}
