// arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -pie -Wl,-z,max-page-size=0 -nostdlib -nostartfiles main.c

#define INIT_VAL 0xdeadbeef
#define SYS_EXIT 21

unsigned int val = INIT_VAL;

void __exit(int status)
{
	__asm__ __volatile__ (
		"mov r0, %0 \r\n"
		"mov r1, %1 \r\n"
		"svc #1"
		:: "r" (status), "I" (SYS_EXIT)
		);
}

int _start(void)
{
	val++;
	if (val != INIT_VAL + 1) {
		for (;;)
			;
		/* return -1; */
	}

	__exit(0);
}
