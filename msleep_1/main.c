#include "unit.h"
#include "kernel.h"

extern void msleep(unsigned int);

int main()
{
	for (int i = 1; i <= 5; i++) {
		printk("%d...\n", i);
		msleep(999);
	}
	printk("\n");

	TEST_EXIT(0);
}
