from time import strftime
import subprocess
from datetime import datetime
from re import search
import os

testsuite_v7m = [
    "test_1",
    "thread_1",
    "thread_2",
    "msleep_1",
    "mm_1",
    "mm_2",
    "timer_1",
    "timer_2",
    "mutex_1",
    "mutex_2",
    "mutex_3",
    "mutex_4",
    "syscall_1",
    "irqaction_1",
    "sysconf_1",
    "itoa_1",
    "sprintf_1",
    "ucontext_1",
    "malloc_1"
]

testsuite_v6m = [
    "test_1",
    "thread_1",
    "thread_2",
    "msleep_1",
    "mm_1",
    "mm_2",
    "timer_1",
    "timer_2",
    "sysconf_1",
    "itoa_1",
    "sprintf_1",
    "ucontext_1",
    "malloc_1"
]

def print_header(testname, arch):
    print("--------------------------------------------")
    print("running test:  \033[1;37;40m%s\033[0m" % testname)
    print("arch        :  %s" % arch)
    print("time        :  %s\n" % strftime("%c"))

def run_test(testname, verbose, platform):
    # platform = os.getenv('PLATFORM', 'qemu')
    cmd = [ "make", "PLATFORM=%s" % platform, "TEST=%s" % testname, "SEMIHOSTING=1",
            "--file", "vega-test/Makefile", "clean", "all", "run-with-semihosting" ]
    res = subprocess.run(cmd, universal_newlines=True, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    if (verbose == True):
        print(' '.join(cmd))
        print(res.stdout)
    # Ubuntu Trusty run an old QEMU, semihosting does not return a code
    # return res.returncode
    if search("test passed", res.stdout) is None:
        return 1
    return 0

def main():
    failed_count = 0
    t0 = datetime.now()
    for testcase in testsuite_v7m:
        print_header(testcase, 'v7m')
        failed_count += run_test(testcase, True, 'qemu')
    for testcase in testsuite_v6m:
        print_header(testcase, 'v6m')
        failed_count += run_test(testcase, True, 'microbit')
    t =  datetime.now()
    print("Ran %d tests in %d.%ds" % (len(testsuite_v7m) + len(testsuite_v6m),
                                      (t - t0).seconds, (t - t0).microseconds / 1000))
    if (failed_count):
        exit(1)

if __name__ == "__main__":
    main()
