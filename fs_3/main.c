/* test simple romFS */

#include "kernel.h"
#include "unit.h"

#include <string.h>

#include <kernel/fs/fs.h>

#include "cmsis/arm/ARMCM4.h"

extern char _binary_sda1_start;

int main()
{
	int fd;
	char buffer[128];

	romfs_init();
	mount("/dev/sda1", "home", "romfs", 0, &_binary_sda1_start);

	/* Lorem ipsum dolor sit amet, consectetur adipiscing elit. */

	fd = open("/home/lorem.txt", 0);
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
	seek(fd, 0, SEEK_SET);
	memset(buffer, 0, 128);
	read(fd, buffer, 11);
	printk("read(): %s\n", buffer);
	if (strcmp(buffer, "Lorem ipsum"))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
