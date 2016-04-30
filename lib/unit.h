#ifndef TESTS_LIB_UNIT_H
#define TESTS_LIB_UNIT_H

#include "kernel.h"

#ifdef CONFIG_SEMIHOSTING

void semih_exit(int);

#define TEST_EXIT(ret_val) ({				  \
	if (ret_val)					  \
		printk("test failed: %d\n", (ret_val));	  \
	else						  \
		printk("test passed\n");		  \
	semih_exit(ret_val); })

#else

#define TEST_EXIT(ret_val) ({				  \
	if (ret_val)					  \
		printk("test failed: %d\n", (ret_val));	  \
	else						  \
		printk("test passed\n");		  \
	return (ret_val); })
#endif

#endif /* !TESTS_LIB_UNIT_H */
