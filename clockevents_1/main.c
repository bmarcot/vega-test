/* simple clock event test */

#include <kernel/time/clockevents.h>

#include "unit.h"

volatile int passed;

void event_handler(struct clock_event_device *dev)
{
	(void)dev;

	passed = 1;
}

int main()
{
	struct clock_event_device *dev = clockevents_get_device("lm3s-timer0");

	clockevent_set_event_handler(dev, event_handler);
	clockevents_switch_state(dev, CLOCK_EVT_STATE_ONESHOT);
	if (!clockevent_state_oneshot(dev))
		TEST_EXIT(1);

#define NSEC_PER_SEC 1000000000l
	clockevents_program_event(dev, (2ll * NSEC_PER_SEC));

	while (!passed)
		;

	TEST_EXIT(0);
}
