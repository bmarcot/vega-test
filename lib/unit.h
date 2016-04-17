#ifndef TESTS_LIB_UNIT_H
#define TESTS_LIB_UNIT_H

#include "kernel.h"

#ifdef CONFIG_SEMIHOSTING

void semih_exit(int);

#define TEST_EXIT(ret_val)			      \
	printk("exit: %d\n", (ret_val));	      \
	semih_exit(ret_val);

#else

#define TEST_EXIT(ret_val)			      \
	printk("exit: %d\n", (ret_val));	      \
	return (ret_val);
#endif

#endif /* !TESTS_LIB_UNIT_H */
