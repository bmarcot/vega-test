#pragma once

#include <asm/syscalls.h>
#include <libvega/syscalls.h>

static inline int futex(int *uaddr, int futex_op, int val)
{
	return syscall(3, uaddr, futex_op, val, SYS_FUTEX);
}
