/* test simple romFS */

#include <string.h>

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
	if (mtdram_init_device(&mtd1, &_binary_sda1_start, 1024 * 10,
				dentry.d_name))
		printk("error: mtdram init device failed\n");
	vfs_link(NULL, dev_inode(), &dentry);
}

int atoi(const char *nptr)
{
	int res = 0;
	int i = 1;
	const char *nptr_end = nptr;

	while (*(nptr_end + 1) != '\0')
		nptr_end++;
	while (nptr_end >= nptr) {
		res += (*nptr_end - '0') * i;
		i *= 10;
		nptr_end--;
	}

	return res;
}

int readnum(int fd)
{
	int i = 0;
	char buf[12];

	for (; i < 11; i++) {
		read(fd, &buf[i], 1);
		if ((buf[i] < '0') || (buf[i] > '9'))
			break;
	}
	buf[i] = '\0';

	return atoi(buf);
}

// yellow toolkit

// struct ytk_framebuffer
struct ytk_pixbuf {
	unsigned short height;
	unsigned short width;
	void           *mem;
};

struct ytk_image {
	unsigned short height;
	unsigned short width;
	unsigned short anchor_top;
	unsigned short anchor_left;
	int fd;
};

/* http://netpbm.sourceforge.net/doc/pbm.html */
struct ytk_image yetk_netpbm_open(const char *path, struct ytk_image *image)
{
	if (image == NULL) {
		image = malloc(sizeof(struct ytk_image));
		if (image == NULL)
			return NULL;
	}
}

char aurora_g2_27_buf[264 * 176 / 8];
/* char aa[33 * 176]; */

int draw_pbm_image(struct ytk_pixbuf *pixbuf, const char *filename,
		int x, int y, int transparency)
{
	int fd = open(filename, 0);
	if (fd < 0) {
		printk("error: failed to open %s\n");
		TEST_EXIT(1);
	}

	char magic_number[3];
	if (read(fd, magic_number, 3) != 3)
		return -1;
	if (strcmp("P4\n", magic_number)) {
		printk("error: unrecognized filetype\n");
		return -1;
	}

	int image_width = readnum(fd);
	int image_height = readnum(fd);
	printk("with=%d height=%d (%d)\n", image_width, image_height, image_width / 8);

	int byte_width = align_next(image_width, 8) / 8 ;
	for (int j = y; j < image_height + y; j++) {
		// 8-bit aligned
		/* char *pos = (char *)pixbuf->mem + (((y + j) * pixbuf->width / 8) + x / 8); */
		/* if (!transparency) */
		/* 	read(fd, pos, image_width / 8); */

		read(fd, &aurora_g2_27_buf[j * 33 + x / 8], byte_width);
		/* for (int i = 0; i < image_width / 8; i++) */
		/* 	aurora_g2_27_buf[j * image_width / 8 + i] = ~aurora_g2_27_buf[j * image_width / 8 + i]; */
		//read(fd, &aa[i * 33], 33);
		/* for (int i = 0; i < 33; i++) { */
		/* 	//printk("%02x", aurora_g2_27_buf[i + j*176/8]); */
		/* 	printk("%02x", aa[i * 33]); */
		/* } */
		/* printk("\n"); */
	}

	close(fd);

	return 0;
}

int draw_pbm_image_unaligned(struct ytk_pixbuf *pixbuf, const char *filename,
			int x, int y, int transparency)
{
	int fd = open(filename, 0);
	if (fd < 0) {
		printk("error: failed to open %s\n");
		TEST_EXIT(1);
	}

	char magic_number[3];
	if (read(fd, magic_number, 3) != 3)
		return -1;
	if (strcmp("P4\n", magic_number)) {
		printk("error: unrecognized filetype\n");
		return -1;
	}

	int image_width = readnum(fd);
	int image_height = readnum(fd);
	printk("with=%d height=%d (%d)\n", image_width, image_height, image_width / 8);

	int byte_width = align_next(image_width, 8) / 8 ;
	char buf[400/8];
	int *intptr = (char *)aurora_g2_27_buf + 0x2000000;
	for (int j = y; j < image_height + y; j++) {
		read(fd, buf, image_width / 8);
		for (int i = 0; i < image_width / 8; i++) {
			for (int k = 0; (k < 8) && (k + i * 8 < image_width); k++) {
				intptr[k + i * 8] = (buf[i] >> k) & 1;
			}
		}
	}

	close(fd);

	return 0;
}

int draw_trapezoid(struct ytk_pixbuf *pixbuf, const char *filename,
		int x, int y, int transparency)
{
	return 0;
}

struct ytk_pixbuf aurora_g2_27 = {
	.width  = 264,
	.height = 176,
	.mem    = aurora_g2_27_buf,
};

#include "conf_EPD.h"

int main()
{
	int fd;
	char buffer[128];

	printk("filesystem at %p\n", &_binary_sda1_start);
	dump_romfs_info(&_binary_sda1_start);

	init_tmpfs_inode(&inode);
	flash_init();
	mount("/dev/mtd1", "/dev/flash", "romfs", 0, 0);

	memset(aurora_g2_27_buf, 0xff, 264 * 176 / 8);
	/* draw_pbm_image(&aurora_g2_27, "/dev/flash/viewport_264.pbm", 0, 0, 0); */
	draw_pbm_image(&aurora_g2_27, "/dev/flash/hourtime.pbm", 16, 16, 0);
	draw_pbm_image(&aurora_g2_27, "/dev/flash/power5s.pbm", 16, 64, 0);
	draw_pbm_image(&aurora_g2_27, "/dev/flash/697w.pbm", 16, 96, 0);
	draw_pbm_image(&aurora_g2_27, "/dev/flash/numbers.pbm", 8, 128, 0);
	draw_pbm_image(&aurora_g2_27, "/dev/flash/cadence.pbm", 128, 64, 0);
	draw_pbm_image(&aurora_g2_27, "/dev/flash/cadence_s.pbm", 128, 48, 0);

	EPD_display_hardware_init();
	EPD_display_from_pointer(EPD_270, (uint8_t *)&aurora_g2_27_buf,
				(uint8_t *)&aurora_g2_27_buf);

	TEST_EXIT(0);
}
