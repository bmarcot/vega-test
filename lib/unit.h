#ifndef TEST_LIB_UNIT_H
#define TEST_LIB_UNIT_H

#include "kernel.h"

#include <libsemi/v7m_semi.h>

int test_printf(const char *format, ...);

#define C_GREEN	 "\033[1;32m"
#define C_RED    "\033[1;31m"
#define C_NORMAL "\033[0m"

#define TEST_EXIT(status) ({			\
	if (status)				\
		test_printf("\n"C_RED"test failed: %d\n"C_NORMAL, (status)); \
	else					\
		test_printf("\n"C_GREEN"test passed\n"C_NORMAL); \
	v7m_semi_exit(status); })

#endif /* !TEST_LIB_UNIT_H */
