/* test /dev/random */

#include "kernel.h"
#include "unit.h"

#include <kernel/fs/fs.h>

int main()
{
	unsigned int n, p;

	fs_init();

	int fd = open("/dev/random", 0);
	if (fd < 0) {
		printk("error: failed to open /dev/random\n");
		TEST_EXIT(1);
	}

	for (int j = 0; j < 1000; j++) {
		read(fd, &n, 4);
		if (j < 40)
			printk("%08x\n", n);
		if (n == p) {
			printk("error: got same random number %d twice in a row\n", n);
			TEST_EXIT(1);
		}
		p = n;
	}

	TEST_EXIT(0);
}
