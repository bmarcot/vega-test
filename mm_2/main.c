#include "mm.h"
#include "unit.h"

int main()
{
	void *p[14];
	int sz[] = {256, 1024, 2048, 256, 256, 256, 512, 512, 1024, 512,
		     2048, 256, 512, 1024};

	for (int j = 0; j < 30; j++) {
		printk("Iteration #%d\n", j);
		for (int i = 0; i < 14; i++) {
			if ((p[i] = page_alloc(sz[i])) == NULL)
				TEST_EXIT(1);
		}
		for (int i = 0; i < 14; i++)
			page_free(p[i]);
	}

	TEST_EXIT(0);
}
