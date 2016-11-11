/* test simple romFS */

#include "kernel.h"
#include "unit.h"

#include <string.h>

#include <kernel/fs/fs.h>

int main()
{
	char str[] = "Hello World!";
	char buf[32];
	size_t len = strlen(str);
	size_t retlen;
	int fd;

	printk("Opening /dev/mtd0...\n");
	fd = open("/dev/mtd0", 0);
	if (write(fd, str, len) != len)
		TEST_EXIT(1);
	seek(fd, 0, SEEK_SET);
	if (read(fd, buf, len) != len)
		TEST_EXIT(1);
	printk("Read /dev/mtd0: %s\n", buf);
	if (strcmp(str, buf))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
