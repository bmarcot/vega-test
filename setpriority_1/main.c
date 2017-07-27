/* simple test that sets task priority */

#include <sys/resource.h>
#include <unistd.h>

#include "unit.h"

#define PRIO_PROCESS 0

int main()
{
	int pid = getpid();

	if (setpriority(PRIO_PROCESS, pid, 16))
		TEST_EXIT(1);
	if (getpriority(PRIO_PROCESS, pid) != 16)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
