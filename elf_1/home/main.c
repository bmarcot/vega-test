// arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -pie -Wl,-z,max-page-size=0 -nostdlib -nostartfiles main.c

#define INIT_VAL 0xdeadbeef

unsigned int val = INIT_VAL;

int _start(void)
{
	val++;
	if (val != INIT_VAL + 1) {
		for (;;)
			;
		/* return -1; */
	}

	return 0;
}
