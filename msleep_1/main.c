#include "unit.h"
#include "kernel.h"

extern void msleep(unsigned int);

int main()
{
	for (int i = 0; i <= 13; i++) {
		printk("%d...\n", i);
		msleep(700);
	}
	printk("\n");

	TEST_EXIT(0);
}
