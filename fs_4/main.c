/* test overreading a file */

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/mount.h>

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
	char buffer[128];

	memset(buffer, 0, 128);
	flash_init();
	mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

	/* file contains -Lorem ipsum\n- */
	fd = open("/dev/flash/lorem.txt", 0);
	if (fd < 0) {
		printk("error: failed to open /home/lorem.txt\n");
		TEST_EXIT(1);
	}

	/* overread the file */
	int r = read(fd, buffer, 128);
	if (r != 12) {
		printk("error: read incorrect number of char (%d)\n", r);
		TEST_EXIT(1);
	}
	if (strcmp(buffer, "Lorem ipsum\n"))
		TEST_EXIT(1);

	/* try to read again */
	r = read(fd, buffer, 1);
	if (r != 0) {
		printk("error: read incorrect number of char (%d)\n", r);
		TEST_EXIT(1);
	}

	TEST_EXIT(0);
}
