/* test simple romFS */

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

static struct inode inode = {
	.i_private = &mtd1,
};

void flash_init(void)
{
	struct dentry dentry = { .d_inode = &inode,
				 .d_name  = "mtd1" };

	printk("Creating MTD device %s\n", dentry.d_name);
	if (mtdram_init_device(&mtd1, &_binary_sda1_start, 1024,
				dentry.d_name))
		printk("error: mtdram init device failed\n");
	vfs_link(NULL, dev_inode(), &dentry);
}

int main()
{
	int fd;
	char buffer[128];

	/* printk("filesystem at %p\n", &_binary_sda1_start); */
	/* dump_romfs_info(&_binary_sda1_start); */

	init_tmpfs_inode(&inode);
	flash_init();
	mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

	/* Lorem ipsum dolor sit amet, consectetur adipiscing elit. */
	fd = open("/dev/flash/lorem.txt", 0);
	if (fd < 0) {
		printk("error: failed to open /home/lorem.txt\n");
		TEST_EXIT(1);
	}

	memset(buffer, 0, 128);
	read(fd, buffer, 11);
	printk("read(): %s\n", buffer);
	if (strcmp(buffer, "Lorem ipsum"))
		TEST_EXIT(1);

	memset(buffer, 0, 128);
	read(fd, buffer, 10);
	printk("read(): %s\n", buffer);
	if (strcmp(buffer, " dolor sit"))
		TEST_EXIT(1);

	memset(buffer, 0, 128);
	read(fd, buffer, 18);
	printk("read(): %s\n", buffer);
	if (strcmp(buffer, " amet, consectetur"))
		TEST_EXIT(1);

	/* rewind the file */
	lseek(fd, 0, SEEK_SET);
	memset(buffer, 0, 128);
	read(fd, buffer, 11);
	printk("read(): %s\n", buffer);
	if (strcmp(buffer, "Lorem ipsum"))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
