/* test /dev/zero */

#include "kernel.h"
#include "unit.h"

#include <string.h>

#include <kernel/fs/fs.h>

char buf[128];

int main()
{
	fs_init();

	int fd = open_1("/dev/zero", 0);
	if (fd < 0) {
		printk("error: failed to open /dev/zero\n");
		TEST_EXIT(1);
	}

	memset(buf, 0xff, 128);
	read_1(fd, &buf, 128);
	for (int j = 0; j < 128; j++) {
		if (buf[j]) {
			printk("error: got a non-zero value\n");
			TEST_EXIT(1);
		}
	}

	TEST_EXIT(0);
}
