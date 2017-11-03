/* raise a signal that has no handler is OK */

#include <signal.h>

#include "unit.h"

int main()
{
	if (raise(SIGUSR1))
		TEST_EXIT(1);

	TEST_EXIT(0);
}
