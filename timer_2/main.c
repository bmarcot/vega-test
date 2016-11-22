/* simple create  multiple concurrent timers */

#include <signal.h>
#include <stddef.h>

#include "kernel.h"
#include "unit.h"

int vals[] = { 0, 0, 0, 0 };

static void event(union sigval sival)
{
	printk("In event %d.\n", sival.sival_int);
	vals[sival.sival_int] = 1;
}

int main()
{
	struct sigevent sevp = { .sigev_notify_function = event };
	timer_t timerid[4];

	for (int i = 0; i < 4; i++)
	{
		printk("Creating timer %d...\n", i);
		sevp.sigev_value.sival_int = i;
		timer_create(0, &sevp, &timerid[i]);
		timer_settime(timerid[i], 0, 750 * (i + 1) * 1000);
	}
	for (int i = 0; i < 4; i++) {
		while (vals[i] == 0)
			;
	}

	TEST_EXIT(0);
}
