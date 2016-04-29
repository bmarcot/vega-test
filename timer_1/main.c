#include "unit.h"
#include "timer.h"

int main()
{
	printk("Create a 2s timer...\n");
	timer_create(2000);
	printk("OK, timer expired!\n");

	TEST_EXIT(0);
}
