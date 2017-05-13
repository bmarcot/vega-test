/* open files in romFS multiple times */

#include <fcntl.h>

#include <vega/sys/mount.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>

#include <drivers/mtd/mtd.h>

#include "unit.h"

extern char _binary_sda1_start;
struct mtd_info mtd1;

void flash_init(void)
{
	struct dentry dentry = { .d_name  = "mtd1" };

	printk("Creating MTD device %s\n", dentry.d_name);
	if (mtdram_init_device(&mtd1, &_binary_sda1_start, 1024,
				dentry.d_name))
		printk("error: mtdram init device failed\n");
	add_mtd_device(&mtd1, dentry.d_name);
}

int main()
{
	int fd1, fd2;

	flash_init();
	mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

	for (int i = 0; i < 1000; i++) {
		fd1 = open("/dev/flash/lorem.txt", 0);
		fd2 = open("/dev/flash/quadrant.xml", 0);
		if (fd1 < 0 || fd2 < 0) {
			printk("error: failed to open file in flash\n");
			TEST_EXIT(1);
		}
		close(fd1);
		close(fd2);
	}

	TEST_EXIT(0);
}
