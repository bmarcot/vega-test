/* Simple test of message queues */

#include <fcntl.h>    /* For O_* constants */
#include <mqueue.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>

#include "syscall-wrappers.h"
#include "unit.h"

static char const VAL[] = "Hello World!";
static int step;

void *fn(void *mqdes)
{
	char msg_ptr[64];
	int len;

	step = 1;
	len = mq_receive((mqd_t)mqdes, msg_ptr, 64, NULL);
	if (len != strlen(VAL) + 1)
		TEST_EXIT(1);
	if (strcmp(msg_ptr, VAL))
		TEST_EXIT(1);
	step = 2;

	return 0;
}

int main(void)
{
	pthread_t tip;
	mqd_t mqdes;

	mqdes = mq_open("my_queue", O_CREAT, 0, NULL);
	if (mqdes == -1) {
		printk("Cannot create message queue");
		TEST_EXIT(1);
	}

	if (pthread_create(&tip, NULL, fn, (void *)mqdes))
		TEST_EXIT(1);

	while (step != 1)
		sched_yield();
	mq_send(mqdes, VAL, strlen(VAL) + 1, 0);
	while (step != 2)
		sched_yield();

	TEST_EXIT(0);
}
