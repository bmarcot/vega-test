#include "unit.h"
#include "utils.h"
#include <string.h>
#include "kernel.h"

int _itoa_00(void)
{
	char buf[64];
	if (strcmp("0", _itoa(0, buf, "0")))
		return -1;
	return 0;
}

int _itoa_01(void)
{
	char buf[64];
	if (strcmp("0", _itoa(0, buf, "01234567")))
		return -1;
	return 0;
}

int _itoa_02(void)
{
	char buf[64];
	if (strcmp("1234567890", _itoa(1234567890, buf, "0123456789")))
		return -1;
	return 0;
}

int _itoa_03(void)
{
	char buf[64];
	if (strcmp("20", _itoa(32, buf, "0123456789abcdef")))
		return -1;
	return 0;
}

int _itoa_04(void)
{
	char buf[64];
	if (strcmp("21", _itoa(33, buf, "0123456789abcdef")))
		return -1;
	return 0;
}

int _itoa_05(void)
{
	char buf[64];
	if (_itoa(458, buf, ""))
		return -1;
	return 0;
}

int _itoa_06(void)
{
	char buf[64];
	if (strcmp("ffffffff", _itoa(0xffffffff, buf, "0123456789abcdef")))
		return -1;
	return 0;
}

int _itoa_07(void)
{
	char buf[64];
	if (strcmp("4294967295", _itoa(0xffffffff, buf, "0123456789")))
		return -1;
	return 0;
}

int _itoa_08(void)
{
	char buf[64];
	if (strcmp("1000", _itoa(0x1000, buf, "0123456789abcdef")))
		return -1;
	return 0;
}

int main()
{
	int status = 0;
	int (*test[])(void) = { _itoa_00, _itoa_01, _itoa_02,
				_itoa_03, _itoa_04, _itoa_05,
				_itoa_06, _itoa_07, _itoa_08 };

	for (int i = 0; i <= 8; i++) {
		printk("itoa test #%d\n", i);
		status += test[i]();
	}
	if (status)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
