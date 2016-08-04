#include <kernel/bitops.h>
#include <kernel/types.h>

#include "unit.h"

int main()
{
	u32 j;

	/* test first bit set */
	for (int i = 0; i < 32; i++) {
		j = 1 << i;
		if (find_first_bit(&j, 32) != (unsigned int)(31 - i))
			TEST_EXIT(1);
	}

	/* test first zero bit */
	for (int i = 0; i < 32; i++) {
		j = ~(1 << i);
		if (find_first_zero_bit(&j, 32) != (unsigned int)(31 - i))
			TEST_EXIT(1);
	}

	u32 k[2] = { 0, 1 };
	if (find_first_bit(k, 32) != 32UL)
		TEST_EXIT(1);
	if (find_first_bit(k, 64) != 63UL)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
