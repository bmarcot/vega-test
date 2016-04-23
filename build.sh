#!/bin/bash

function run_test {

    echo "--------------------------------------------"
    echo -e "running test:  $1"
    echo -e "time        :  $(date)\n"

    if ! make PLATFORM=qemu SEMIHOSTING=1 TEST=$1 --file vega-test-master/Makefile clean all run-with-semihosting \
	| tee /dev/tty | grep --quiet 'test passed'; then
	exit 1
    fi

    return 0
}

# https://github.com/bmarcot/vega/issues/6

for testname in \
    test_1 \
    thread_1 \
    thread_2 \
    msleep_1
do
    run_test $testname
done

exit 0
