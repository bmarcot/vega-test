/* test /dev/random */

#include "kernel.h"
#include "unit.h"

#include <kernel/fs/fs.h>

int main()
{
	unsigned int n, p;

	fs_init();

	int fd = open_1("/dev/random", 0);
	if (fd < 0) {
		printk("error: failed to open /dev/random\n");
		TEST_EXIT(1);
	}

	for (int j = 0; j < 1000; j++) {
		read_1(fd, &n, 4);
		if (n == p) {
			printk("error: got same random number %d and %d\n",
				n, p);
			TEST_EXIT(1);
		}
		p = n;
	}

	TEST_EXIT(0);
}
