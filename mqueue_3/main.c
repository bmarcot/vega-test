/* Receive in an empty nonblocking message queue returns immediately */
/* Send to a full nonblocking message queue returns immediately */

#include <errno.h>
#include <fcntl.h>    /* For O_* constants */
#include <mqueue.h>

#include "syscall-wrappers.h"
#include "unit.h"

int main(void)
{
	char msg_ptr[1];

	mqd_t mqdes = mq_open("my_queue", O_CREAT | O_NONBLOCK, 0, NULL);
	if (mqdes == -1) {
		printk("Cannot create message queue");
		TEST_EXIT(1);
	}

	/* Receive from empty message queue */
	int len = mq_receive(mqdes, msg_ptr, 1, NULL);
	if (len != -1)
		TEST_EXIT(1);
	if (errno != EAGAIN)
		TEST_EXIT(1);

	/* Send to full message queue */
	int ret;
	do {
		ret = mq_send(mqdes, 0, 1, 0);
	} while (ret != -1);
	if (errno != EAGAIN)
		TEST_EXIT(1);

	TEST_EXIT(0);
}
