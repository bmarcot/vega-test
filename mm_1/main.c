#include <kernel/mm.h>
#include "unit.h"

int main()
{
	void *p, *q;

	if ((p = page_alloc(256)) == NULL)
		TEST_EXIT(1);
	page_free(p);

	/* same page should be reallocated */
	if ((q = page_alloc(256)) != p)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
