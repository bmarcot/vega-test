/* test directory hierarchy in RomFS */

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

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
	int fd;
	const char filename[] = "/dev/flash/foo/bar/lorem";
	char buffer[32];

	/* printk("filesystem at %p\n", &_binary_sda1_start); */
	/* dump_romfs_info(&_binary_sda1_start); */

	flash_init();
	mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

	/* Lorem ipsum dolor sit amet, consectetur adipiscing elit. */
	fd = open(filename, 0);
	if (fd < 0) {
		printk("error: failed to open %s\n", filename);
		TEST_EXIT(1);
	}

	memset(buffer, 0, 32);
	if (read(fd, buffer, 11) != 11)
		TEST_EXIT(1);
	if (strcmp(buffer, "Lorem ipsum"))
		TEST_EXIT(1);

	memset(buffer, 0, 32);
	if (read(fd, buffer, 10) != 10)
		TEST_EXIT(1);
	if (strcmp(buffer, " dolor sit"))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
