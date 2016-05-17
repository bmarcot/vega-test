#ifndef TEST_LIB_UNIT_H
#define TEST_LIB_UNIT_H

#include "kernel.h"

#define C_GREEN	 "\033[1;32;40m"
#define C_RED    "\033[1;31;40m"
#define C_NORMAL "\033[0m"

#define printk_green(format, ...)			\
	printk(C_GREEN format C_NORMAL, ##__VA_ARGS__);

#define printk_red(format, ...)				\
	printk(C_RED format C_NORMAL, ##__VA_ARGS__);


#ifdef CONFIG_SEMIHOSTING

void semih_exit(int);

#define TEST_EXIT(ret_val) ({			       \
	if (ret_val) {				       \
		printk_red("test failed: %d\n", (ret_val));	\
	} else {				       \
		printk_green("test passed\n"); }       \
	semih_exit(ret_val); })

#else

#define TEST_EXIT(ret_val) ({			       \
	if (ret_val) {				       \
		printk_red("test failed: %d\n", (ret_val));	\
	} else {				       \
		printk_green("test passed\n"); }       \
	return (ret_val); })
#endif

#endif /* !TEST_LIB_UNIT_H */
