/* test the nanosleep() syscall */

#include <time.h>
#include "unit.h"

int nanosleep(const struct timespec *req, struct timespec *rem);

int main(void)
{
	struct timespec req, rem;

	req.tv_sec = 1;
	req.tv_nsec = 0;

	if (nanosleep(&req, &rem))
		TEST_EXIT(1);
	if (rem.tv_sec || rem.tv_nsec)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
