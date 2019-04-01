/* start a one-shot timer and block on read */

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

	struct timespec ts = {.tv_sec = 3, .tv_nsec = 0};
	ioctl(fd, CLOCK_SET_TIME, &ts);
	ioctl(fd, CLOCK_START);

	int bytes, n;
	bytes = read(fd, &n, 4);
	if (n != 1)
		TEST_EXIT(1);
	if (bytes != 4)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
