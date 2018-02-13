from time import strftime
import subprocess
from datetime import datetime
from re import search
import os

testsuite = [
    "test_1",
    "bitops_1",
    "thread_1",
    "thread_2",
    "thread_3",
    "thread_4",
    "thread_5",
    "thread_6",
    "msleep_1",
    "msleep_2",
    "mm_1",
    "mm_2",
    "timer_1",
    "timer_2",
    "timer_3",
    "timer_4",
    "timer_5",
    "mutex_1",
    "mutex_2",
    "mutex_3",
    "mutex_4",
    "mutex_5",
    "syscall_1",
    "raise_1",
    "raise_2",
    "raise_3",
    "raise_4",
    "sigqueue_1",
    "sigqueue_2",
    "sysconf_1",
    "itoa_1",
    "sprintf_1",
    "ucontext_1",
    "malloc_1",
    "cond_1",
    "cond_2",
    "cond_3",
    "fs_1",
    "fs_2",
    "fs_3",
    "fs_4",
    "fs_5",
    "fs_6",
    "fs_7",
    "mtdram_1",
    "readdir_1",
    "stat_1",
    "getpid_1",
    "slab_1",
    "slab_2",
    "mmap_1",
    "mmap_2",
    "page_3",
    "lib_rbtree_1",
    "vfork_1",
    "vfork_2",
    "vfork_3",
    "elf_1",
    "setpriority_1",
    "clockevents_1",
    "hrtimer_1",
    "hrtimer_2",
    "clock_1",
    "gettid_1",
    "getppid_1",
    "raise_5",
    "kill_1",
    "nanosleep_1",
    "nanosleep_2",
    "pause_1",
    "exit_1",
    "exit_2",
]

blacklist = [
    "lib_rbtree_1",
    "thread_4",
    "sigqueue_2",
    "sysconf_1",
    "hrtimer_2",
]

def print_qemu_version():
    cmd = [ "qemu-system-arm", "--version" ]
    res = subprocess.run(cmd, universal_newlines=True, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    print("QEMU version:\n" + res.stdout)

def print_gcc_version():
    cmd = [ "arm-none-eabi-gcc", "--version" ]
    res = subprocess.run(cmd, universal_newlines=True, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    print("GCC version:\n" + res.stdout)

def print_header(testname, arch):
    print("--------------------------------------------")
    print("running test:  \033[1;37m%s\033[0m" % testname)
    print("arch        :  %s" % arch)
    print("time        :  %s\n" % strftime("%c"))

def run_test(testname, verbose, platform):
    # platform = os.getenv('PLATFORM', 'qemu')
    cmd = [ "make", "TEST=%s" % testname,
            "--file", "test/Makefile", "clean_test", "all", "run" ]
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

def is_test_blacklisted(testname):
    for t in blacklist:
        if t == testname:
            return 1
    return 0

def main():
    print_qemu_version()
    print_gcc_version()
    print('Staging %d tests: %s' % (len(testsuite), ', '.join(testsuite)))
    failed_count = 0
    results = dict()
    t0 = datetime.now()
    for testcase in testsuite:
        if is_test_blacklisted(testcase):
            continue
        print_header(testcase, 'v7m')
        status = run_test(testcase, True, 'qemu')
        failed_count += status
        if status:
            results[testcase] = 'failed'
        else:
            results[testcase] = 'ok'
    t =  datetime.now()

    for key in sorted(results.keys()):
        print("% 16s:  %s" % (key, results[key]))

    status = "failed" if failed_count else "ok"
    print("\nRan %d tests in %d.%ds (\033[1;37m%s\033[0m)" % (
        len(testsuite) - len(blacklist),
        (t - t0).seconds, (t - t0).microseconds / 1000,
        status))

    if (failed_count):
        exit(1)

if __name__ == "__main__":
    main()
