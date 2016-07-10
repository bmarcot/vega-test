#include "unit.h"
#include "kernel.h"

int dummy_0_arg();
int dummy_1_arg();
int dummy_2_args();
int dummy_3_args();
int dummy_4_args();

int syscall_register(unsigned ix, void *(*fn)());

void *sys_0_arg(void)
{
	return (void *)0;
}

void *sys_1_arg(int a)
{
	if (a != 0x101) {
		printk("FAIL: kernel: in syscall with 1 arg\n");
		TEST_EXIT(1);
	}

	return (void *)111;
}

void *sys_2_arg(int a, int b)
{
	if ((a != 0x101) || (b != 0x202)) {
		printk("FAIL: kernel: in syscall with 2 args\n");
		TEST_EXIT(1);
	}

	return (void *)222;
}

void *sys_3_arg(int a, int b, int c)
{
	if ((a != 0x101) || (b != 0x202) || (c != 0x303)) {
		printk("FAIL: kernel: in syscall with 3 args\n");
		TEST_EXIT(1);
	}

	return (void *)333;
}

void *sys_4_arg(int a, int b, int c, int d)
{
	if ((a != 0x101) || (b != 0x202) || (c != 0x303) || (d != 0x404)) {
		printk("FAIL: kernel: in syscall with 4 args\n");
		TEST_EXIT(1);
	}

	return (void *)444;
}

int main(void *arg)
{
	(void) arg;

	syscall_register(4, sys_0_arg);
	syscall_register(5, sys_1_arg);
	syscall_register(6, sys_2_arg);
	syscall_register(7, sys_3_arg);
	syscall_register(8, sys_4_arg);

	printk("testing syscall with %d arg(s)...\n", 0);
	if (dummy_0_arg(0) != 0) {
		printk("FAIL: in syscall with 0 arg\n");
		TEST_EXIT(1);
	}

	printk("testing syscall with %d arg(s)...\n", 1);
	if (dummy_1_arg(0x0101) != 111) {
		printk("FAIL: in syscall with 1 arg\n");
		TEST_EXIT(1);
	}

	printk("testing syscall with %d arg(s)...\n", 2);
	if (dummy_2_args(0x0101, 0x0202) != 222) {
		printk("FAIL: in syscall with 2 args\n");
		TEST_EXIT(1);
	}

	printk("testing syscall with %d arg(s)...\n", 3);
	if (dummy_3_args(0x0101, 0x0202, 0x0303) != 333) {
		printk("FAIL: in syscall with 3 args\n");
		TEST_EXIT(1);
	}

	printk("testing syscall with %d arg(s)...\n", 4);
	if (dummy_4_args(0x0101, 0x0202, 0x0303, 0x0404) != 444) {
		printk("FAIL: in syscall with 4 args\n");
		TEST_EXIT(1);
	}

	TEST_EXIT(0);
}
