#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 256

int test_printf(const char *format, ...)
{
	static char buf[BUF_SIZE];
	va_list ap;

	va_start(ap, format);
	if (vsnprintf(buf, BUF_SIZE, format, ap) < 0)
		return -1;
	puts(buf);
	va_end(ap);

	return 0;
}
