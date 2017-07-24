/* simple test for stat() */

#include <sys/stat.h>
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
	flash_init();
	mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

	struct stat st;
	if (stat("/dev/flash/lorem.txt", &st)) {
		printk("error: Wrong pathname\n");
		TEST_EXIT(1);
	}
	printk("Opened file with inode number %d\n", st.st_ino);
	if (!S_ISREG(st.st_mode)) {
		printk("error: File is not a regular file\n");
		TEST_EXIT(1);
	}
	if (st.st_size != 455) {
		printk("error: Wrong file size (%d != 455)\n", st.st_size);
		TEST_EXIT(1);
	}

	TEST_EXIT(0);
}
