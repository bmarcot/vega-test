#include "unit.h"

extern void msleep(unsigned int);

int main()
{
	for (int i = 1; i < 4; i++) {
		test_printf("%d...\n", i);
		msleep(999);
	}
	TEST_EXIT(0);
}
