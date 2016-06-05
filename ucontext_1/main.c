#include "unit.h"
#include "libc/ucontext.h"
#include "uart.h"

static int test_status;
static ucontext_t main_context, other_context;
static unsigned int ctx_stack[128];

void test(int a1, int a2, int a3, int a4)
{
	uart_putstring("Hello from a new context!\n");

	if ((a1 != 9) || (a2 != 0xcafe) || (a3 != 13) || (a4 != 14)) {
		uart_putstring("failed: incorrect correct arg.\n");
		test_status = 1;
	}
}

int main()
{
	other_context.uc_link = &main_context;
	other_context.uc_stack.ss_sp = &ctx_stack[128];

	/* pass 4 arguments to the new context, and swap */
	makecontext(&other_context, test, 4, 9, 0xcafe, 13, 14);
	swapcontext(&main_context, &other_context);

	uart_putstring("And back to the main context.\n");

	TEST_EXIT(test_status);
}
