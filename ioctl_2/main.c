/* start a periodic timer and block on read */

#include <unistd.h>
#include <sys/ioctl.h>

#include <uapi/kernel/clock.h>
#include <uapi/kernel/time.h>

#include "syscall-wrappers.h"
#include "unit.h"

int main(void)
{
	int fd = open("/dev/timer0", 0);
	if (fd < 0) {
		printk("error: failed to open /dev/timer0\n");
		TEST_EXIT(1);
	}

	/* tick every 1ms */
	struct timespec ts = {.tv_sec = 0, .tv_nsec = 1000000};
	ioctl(fd, CLOCK_SET_TIME, &ts);
	ioctl(fd, CLOCK_PIE_ON);
	ioctl(fd, CLOCK_START);

	int n, ticks;
	for (ticks = 0; ticks < 1000;) {
		read(fd, &n, 4);
		ticks += n;
	}
	ioctl(fd, CLOCK_STOP);

	TEST_EXIT(0);
}
