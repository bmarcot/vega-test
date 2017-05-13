/* test mapping file to memory */

#include <string.h>
#include <fcntl.h>

#include <vega/sys/mman.h>
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
	flash_init();
	mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

	/* Lorem ipsum dolor sit amet, consectetur adipiscing elit. */
	int fd = open("/dev/flash/lorem.txt", 0);
	if (fd < 0) {
		printk("error: failed to open /home/lorem.txt\n");
		TEST_EXIT(1);
	}

	void *p = mmap(NULL, 256, 0, 0, fd, 0);
	if (p == MAP_FAILED)
		TEST_EXIT(1);
	if (strncmp((char *)p, "Lorem ipsum", 11))
		TEST_EXIT(1);

	p = mmap(NULL, 256, 0, 0, fd, 11);
	if (p == MAP_FAILED)
		TEST_EXIT(1);
	if (strncmp((char *)p, " dolor sit", 10))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
