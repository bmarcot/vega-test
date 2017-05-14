// arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -pie -Wl,-z,max-page-size=0 main.c

void _exit(int status)
{
	for (;;)
		;
}

static int a;

static int inc(void)
{
	a += 1;

	return a;
}

int main(int argc, char *argv[])
{
	a = 12;

	return 0;
}
