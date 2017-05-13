/* test a mtdram device, using the mtdchar interface */

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>
#include <kernel/mm/page.h>

#include <drivers/mtd/mtd.h>
#include <drivers/mtd/mtdram.h>

#include "unit.h"

int main()
{
	const char name[] = "mtd0";
	char str[] = "Hello World!";
	char buf[32];
	ssize_t len = strlen(str);
	int fd;
	struct mtd_info mtdram;
	void *mem;

	/* create a dummy mtdram (mtdchar) device */
	mem = alloc_pages(size_to_page_order(1024));
	if (mem == NULL)
		TEST_EXIT(1);
	mtdram_init_device(&mtdram, mem, 1024, name);
	add_mtd_device(&mtdram, name);

	/* r/w to the new device using the mtdchar interface */
	fd = open("/dev/mtd0", 0);
	if (write(fd, str, len) != len)
		TEST_EXIT(1);
	lseek(fd, 0, SEEK_SET);
	if (read(fd, buf, len) != len)
		TEST_EXIT(1);
	//printk("Read /dev/mtd0: %s\n", buf);
	if (strcmp(str, buf))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
