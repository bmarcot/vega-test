#!/bin/sh

if ! make PLATFORM=qemu SEMIHOSTING=1 TEST=test_1 --file vega-test-master/Makefile clean all run-with-semihosting; then
    exit 1
fi

# https://github.com/bmarcot/vega/issues/6
#if make PLATFORM=qemu SEMIHOSTING=1 TEST=test_2 --file vega-test-master/Makefile clean all run-with-semihosting; then
#    exit 1
#fi

if ! make PLATFORM=qemu SEMIHOSTING=1 TEST=thread_1 --file vega-test-master/Makefile clean all run-with-semihosting; then
    exit 1
fi

if ! make PLATFORM=qemu SEMIHOSTING=1 TEST=thread_2 --file vega-test-master/Makefile clean all run-with-semihosting; then
    exit 1
fi

