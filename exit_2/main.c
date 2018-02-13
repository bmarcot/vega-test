/* Test handle_group_exit () handling of a thread leader still alive with its
 * thread child calling exit_group () and proper passing of the process exit
 * code to the process parent of this whole thread group.
 */

/* https://bugzilla.redhat.com/show_bug.cgi?id=247907 */

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include "syscall-wrappers.h"
#include "unit.h"

static inline void nop() { /* nothing */ }

#define assert(expr) (!(expr) ? TEST_EXIT(1) : nop())

int pause(void);

static void *start0 (void *arg)
{
	sleep (1);

	exit (42);

	return 0;
}

static void *start1 (void *arg)
{
	pause ();

	/* NOTREACHED */
	assert (0);

	return 0;
}

int main (void)
{
	pthread_t thread0;
	pthread_t thread1;
	int i;
	pid_t child, got_pid;
	int status;

	sleep (2);

	child = vfork ();
	switch (child)
	{
	case -1:
		abort ();
	case 0:
		i = pthread_create (&thread0, NULL, start0, NULL);
		assert (i == 0);
		i = pthread_create (&thread1, NULL, start1, NULL);
		assert (i == 0);
		pause ();
		/* NOTREACHED */
		assert (0);
		break;
	default:
		got_pid = waitpid (child, &status, 0);
		assert (got_pid == child);
		assert (WIFEXITED (status));
		assert (WEXITSTATUS (status) == 42);
		printk ("OK\n");
		TEST_EXIT (0);
	}
	/* NOTREACHED */
	abort ();
}
