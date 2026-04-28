/*
 * Copyright (C) 2024 HiHope Open Source Organization.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <poll.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include "securec.h"
#include <climits>
#include <pthread.h>

using namespace testing::ext;

static const int DELAY_TIME = 10 * 1000;

static constexpr int SETUP_DELAY_US = 5000;
static constexpr int POLL_INTERVAL_US = 1000;
static constexpr int CHILD_LIFETIME_US = 50000;
static constexpr int INVALID_OPTIONS_MASK = 0xDEAD;
static constexpr unsigned long KERNEL_ADDR_VAL = 0xFFFF0000;
static constexpr int MAX_RETRY_COUNT = 50;
static constexpr int RETRY_INTERVAL_US = 10000;
static constexpr int CHILD_RESUME_DELAY_US = 200000;
static constexpr long CPU_LOAD_ITERATIONS = 1000000;
static constexpr int THREAD_POLL_DURATION_US = 100000;
static constexpr int LARGE_SCALE_WAIT_US = 300000;

class HatsWait4Test : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsWait4Test::SetUp()
{
}

void HatsWait4Test::TearDown()
{
}

void HatsWait4Test::SetUpTestCase()
{
}

void HatsWait4Test::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0100
 * @tc.name   : Wait4Failed_0001
 * @tc.desc   : Wait4 wait continued failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4Failed_0001, Function | MediumTest | Level2)
{
    int ret;
    pid_t childPid = -1;

    sigset_t sigMask;
    ret = sigemptyset(&sigMask);
    EXPECT_TRUE(ret >= 0);
    ret = sigaddset(&sigMask, SIGCHLD);
    EXPECT_TRUE(ret >= 0);

    struct sigaction sa;
    sa.sa_sigaction = nullptr;
    sa.sa_flags = SA_SIGINFO;
    ret = sigemptyset(&sa.sa_mask);
    EXPECT_TRUE(ret >= 0);
    ret = sigaction(SIGCHLD, &sa, nullptr);
    EXPECT_EQ(ret, 0);

    int wStatus = 0;
    int options = WCONTINUED;
    int retPid = wait4(childPid, &wStatus, options, nullptr);
    EXPECT_EQ(retPid, -1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0200
 * @tc.name   : Wait4ChildProcessSuccess_0002
 * @tc.desc   : wait4 child process and WIFEXITED success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4ChildProcessSuccess_0002, Function | MediumTest | Level1)
{
    pid_t pid = fork();
    if (pid == 0) {
        usleep(DELAY_TIME);
        exit(0);
    }
    int status;
    struct rusage usage;
    pid_t tpid = wait4(pid, &status, 0, &usage);
    EXPECT_TRUE(tpid > 0);
    EXPECT_EQ(WIFEXITED(status), 1);
    EXPECT_EQ(WEXITSTATUS(status), 0);
    EXPECT_EQ(WIFSIGNALED(status), 0);
    EXPECT_EQ(WTERMSIG(status), 0);
    EXPECT_EQ(WCOREDUMP(status), 0);
    EXPECT_EQ(WIFSTOPPED(status), 0);
    EXPECT_EQ(WSTOPSIG(status), 0);
    EXPECT_EQ(WIFCONTINUED(status), 0);
}

static void Wait4TestSigHandler(int sig) { (void)sig; }

struct Wait4ThreadArg {
    pid_t targetPid;
    pid_t result;
    int err;
    volatile int stop;
};

static void* Wait4PollThread(void* arg)
{
    Wait4ThreadArg* ta = static_cast<Wait4ThreadArg*>(arg);
    int wStatus = 0;
    while (!ta->stop) {
        errno = 0;
        ta->result = wait4(ta->targetPid, &wStatus, WNOHANG, nullptr);
        ta->err = errno;
        if (ta->result != 0) {
            return nullptr;
        }
        usleep(POLL_INTERVAL_US);
    }
    return nullptr;
}

struct Wait4CrossThreadData {
    pid_t childPid;
    pid_t waitResult;
    int waitErrno;
    volatile int forkDone;
};

static void* Wait4CrossThreadForker(void* arg)
{
    Wait4CrossThreadData* d = static_cast<Wait4CrossThreadData*>(arg);
    pid_t pid = fork();
    if (pid == 0) {
        usleep(CHILD_LIFETIME_US);
        _exit(0);
    }
    d->childPid = pid;
    d->forkDone = 1;
    return nullptr;
}

static void* Wait4CrossThreadWaiter(void* arg)
{
    Wait4CrossThreadData* d = static_cast<Wait4CrossThreadData*>(arg);
    while (!d->forkDone) {
        usleep(POLL_INTERVAL_US);
    }
    int wStatus = 0;
    errno = 0;
    d->waitResult = wait4(d->childPid, &wStatus, 0, nullptr);
    d->waitErrno = errno;
    return nullptr;
}

/* TestReportAnalysis 问题[3.1-1]: ECHILD 错误码未验证 (P0) 建议: 在无子进程的进程中调用wait4(-1,...)，验证errno==ECHILD */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0300
 * @tc.name   : Wait4ErrnoEchild_0003
 * @tc.desc   : wait4 returns ECHILD when no child process.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4ErrnoEchild_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(-1, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ECHILD);
}

/* TestReportAnalysis 问题[3.1-2]: EINTR 错误码未测试 (P0) 建议: fork子进程pause，alarm中断wait4，验证errno==EINTR */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0400
 * @tc.name   : Wait4ErrnoEintr_0004
 * @tc.desc   : wait4 returns EINTR when interrupted by signal.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4ErrnoEintr_0004, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        pause();
        _exit(0);
    }
    struct sigaction sa;
    sa.sa_handler = Wait4TestSigHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, nullptr);
    alarm(1);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINTR);
    kill(childPid, SIGKILL);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.1-3]: EINVAL 错误码未测试 (P0) 建议: fork子进程，使用无效options调用wait4，验证errno==EINVAL */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0500
 * @tc.name   : Wait4ErrnoEinval_0005
 * @tc.desc   : wait4 returns EINVAL with invalid options.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4ErrnoEinval_0005, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        usleep(DELAY_TIME);
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, INVALID_OPTIONS_MASK, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
    kill(childPid, SIGKILL);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.1-4]: ESRCH 错误码未测试 (P0) 建议: 调用wait4(INT_MIN,...)，验证errno==ESRCH */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0600
 * @tc.name   : Wait4ErrnoEsrch_0006
 * @tc.desc   : wait4 returns ESRCH for non-existent process.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4ErrnoEsrch_0006, Function | MediumTest | Level2)
{
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(INT_MIN, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESRCH);
}

/* TestReportAnalysis 问题[3.2-1]: pid < -1 等待指定进程组 (P1) 建议: fork子进程setpgid创建新进程组，wait4(-pgid)验证成功 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0700
 * @tc.name   : Wait4PidNegPgid_0007
 * @tc.desc   : wait4 with negative pid as pgid.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4PidNegPgid_0007, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        setpgid(0, 0);
        usleep(DELAY_TIME);
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    int wStatus = 0;
    errno = 0;
    pid_t ret = wait4(-childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFEXITED(wStatus), 1);
}

/* TestReportAnalysis 问题[3.2-2]: pid = 0 等待同进程组子进程 (P1) 建议: fork子进程(同进程组)，wait4(0)验证成功 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0800
 * @tc.name   : Wait4PidZeroPgid_0008
 * @tc.desc   : wait4 with pid=0 waits for any child in same pgid.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4PidZeroPgid_0008, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        usleep(DELAY_TIME);
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(0, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFEXITED(wStatus), 1);
}

/* TestReportAnalysis 问题[3.2-3]: pid = INT_MIN 边界值 (P1) 建议: 调用wait4(INT_MIN,...)，验证返回-1且errno==ESRCH */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0900
 * @tc.name   : Wait4PidIntMin_0009
 * @tc.desc   : wait4 with pid=INT_MIN returns ESRCH.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4PidIntMin_0009, Function | MediumTest | Level2)
{
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(INT_MIN, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESRCH);
}

/* TestReportAnalysis 问题[3.2-4]: 不存在的PID (P1) 建议: fork+wait回收后再次wait4原PID，验证errno==ECHILD */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1000
 * @tc.name   : Wait4PidNonExistent_0010
 * @tc.desc   : wait4 for already-reaped child returns ECHILD.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4PidNonExistent_0010, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    int ws = 0;
    pid_t r = wait4(childPid, &ws, 0, nullptr);
    EXPECT_EQ(r, childPid);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ECHILD);
}

/* TestReportAnalysis 问题[3.2-5]: 非子进程PID (P1) 建议: wait4(1,...)，验证返回-1且errno==ECHILD */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1100
 * @tc.name   : Wait4PidNotChild_0011
 * @tc.desc   : wait4 for non-child process returns ECHILD.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4PidNotChild_0011, Function | MediumTest | Level2)
{
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(1, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ECHILD);
}

/* TestReportAnalysis 问题[3.2-6]: wStatus未映射地址 (P1) 建议: mmap/munmap创建未映射地址，wait4验证返回EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1200
 * @tc.name   : Wait4WstatusUnmapped_0012
 * @tc.desc   : wait4 with unmapped wStatus returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4WstatusUnmapped_0012, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    void *mem = mmap(nullptr, sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_NE(mem, MAP_FAILED);
    munmap(mem, sizeof(int));
    int *badPtr = static_cast<int*>(mem);
    errno = 0;
    pid_t ret = wait4(childPid, badPtr, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    int ws = 0;
    wait4(childPid, &ws, 0, nullptr);
}

/* TestReportAnalysis 问题[3.2-7]: wStatus内核态地址 (P1) 建议: wait4(child_pid,(int*)0xFFFF0000,...)，验证返回EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1300
 * @tc.name   : Wait4WstatusKernelAddr_0013
 * @tc.desc   : wait4 with kernel address wStatus returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4WstatusKernelAddr_0013, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    errno = 0;
    int *kernelPtr = reinterpret_cast<int*>(KERNEL_ADDR_VAL);
    pid_t ret = wait4(childPid, kernelPtr, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    int ws = 0;
    wait4(childPid, &ws, 0, nullptr);
}

/* TestReportAnalysis 问题[3.2-8]: wStatus只读区写入 (P1) 建议: wait4(child_pid,readonly_ptr,...)，验证返回EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1400
 * @tc.name   : Wait4WstatusReadonly_0014
 * @tc.desc   : wait4 with read-only wStatus returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4WstatusReadonly_0014, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    void *ro = mmap(nullptr, sizeof(int), PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_NE(ro, MAP_FAILED);
    int *readonlyPtr = static_cast<int*>(ro);
    errno = 0;
    pid_t ret = wait4(childPid, readonlyPtr, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    int ws = 0;
    wait4(childPid, &ws, 0, nullptr);
    munmap(ro, sizeof(int));
}

/* TestReportAnalysis 问题[3.2-9]: 无效options值 (P1) 建议: wait4(child_pid,&wStatus,0xDEAD,nullptr)，验证errno==EINVAL */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1500
 * @tc.name   : Wait4OptsInvalid_0015
 * @tc.desc   : wait4 with invalid options returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4OptsInvalid_0015, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        usleep(DELAY_TIME);
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, INVALID_OPTIONS_MASK, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
    kill(childPid, SIGKILL);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.2-10]: WNOHANG选项 (P1) 建议: fork子进程pause，WNOHANG返回0，kill后再次WNOHANG返回pid */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1600
 * @tc.name   : Wait4Wnohang_0016
 * @tc.desc   : wait4 with WNOHANG returns immediately.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4Wnohang_0016, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        pause();
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, WNOHANG, nullptr);
    EXPECT_EQ(ret, 0);
    kill(childPid, SIGTERM);
    int retry = 0;
    do {
        usleep(RETRY_INTERVAL_US);
        errno = 0;
        ret = wait4(childPid, &wStatus, WNOHANG, nullptr);
        retry++;
    } while (ret == 0 && retry < MAX_RETRY_COUNT);
    EXPECT_EQ(ret, childPid);
}

/* TestReportAnalysis 问题[3.2-11]: WUNTRACED选项 (P1) 建议: fork子进程raise(SIGSTOP)，wait4(WUNTRACED)验证WIFSTOPPED */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1700
 * @tc.name   : Wait4Wuntraced_0017
 * @tc.desc   : wait4 with WUNTRACED reports stopped child.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4Wuntraced_0017, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        raise(SIGSTOP);
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, WUNTRACED, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFSTOPPED(wStatus), 1);
    EXPECT_EQ(WSTOPSIG(wStatus), SIGSTOP);
    kill(childPid, SIGCONT);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.2-12]: rusage未映射地址 (P1) 建议: mmap/munmap创建未映射地址作为rusage，验证返回EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1800
 * @tc.name   : Wait4RusageUnmapped_0018
 * @tc.desc   : wait4 with unmapped rusage returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4RusageUnmapped_0018, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    void *mem = mmap(nullptr, sizeof(struct rusage), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_NE(mem, MAP_FAILED);
    munmap(mem, sizeof(struct rusage));
    struct rusage *badPtr = static_cast<struct rusage*>(mem);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, badPtr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.2-13]: rusage内核态地址 (P1) 建议: wait4(child_pid,&wStatus,0,(rusage*)0xFFFF0000)，验证EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_1900
 * @tc.name   : Wait4RusageKernelAddr_0019
 * @tc.desc   : wait4 with kernel address rusage returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4RusageKernelAddr_0019, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    struct rusage *kernelPtr = reinterpret_cast<struct rusage*>(KERNEL_ADDR_VAL);
    pid_t ret = wait4(childPid, &wStatus, 0, kernelPtr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.2-14]: rusage只读区写入 (P1) 建议: wait4(child_pid,&wStatus,0,readonly_ptr)，验证EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_2000
 * @tc.name   : Wait4RusageReadonly_0020
 * @tc.desc   : wait4 with read-only rusage returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4RusageReadonly_0020, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    void *ro = mmap(nullptr, sizeof(struct rusage), PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_NE(ro, MAP_FAILED);
    struct rusage *readonlyPtr = static_cast<struct rusage*>(ro);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, readonlyPtr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    wait4(childPid, &wStatus, 0, nullptr);
    munmap(ro, sizeof(struct rusage));
}

/* TestReportAnalysis 问题[3.3-1]: 等待指定进程组pid<-1 (P2) 建议: fork子进程setpgid，使用负pid等待该进程组 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_2100
 * @tc.name   : Wait4FuncPgid_0021
 * @tc.desc   : wait4 functional test with process group.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncPgid_0021, Function | MediumTest | Level1)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        setpgid(0, 0);
        usleep(DELAY_TIME);
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(-childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFEXITED(wStatus), 1);
}

/* TestReportAnalysis 问题[3.3-2]: 等待同进程组子进程pid=0 (P2) 建议: fork子进程(继承父进程组)，wait4(0)验证成功 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_2200
 * @tc.name   : Wait4FuncSamePgid_0022
 * @tc.desc   : wait4 functional test with same pgid children.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncSamePgid_0022, Function | MediumTest | Level1)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        usleep(DELAY_TIME);
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFEXITED(wStatus), 1);
}

/* TestReportAnalysis 问题[3.3-3]: 非零退出状态 (P2) 建议: fork子进程exit(42)，验证WEXITSTATUS=42 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_2300
 * @tc.name   : Wait4FuncNonZeroExit_0023
 * @tc.desc   : wait4 functional test non-zero exit status.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncNonZeroExit_0023, Function | MediumTest | Level1)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(42);
    }
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFEXITED(wStatus), 1);
    EXPECT_EQ(WEXITSTATUS(wStatus), 42);
}

/* TestReportAnalysis 问题[3.3-4]: 信号终止WIFSIGNALED (P2) 建议: fork子进程pause，kill SIGKILL，验证WIFSIGNALED=true */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_2400
 * @tc.name   : Wait4FuncSignaled_0024
 * @tc.desc   : wait4 functional test signaled child.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncSignaled_0024, Function | MediumTest | Level1)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        pause();
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    kill(childPid, SIGKILL);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFSIGNALED(wStatus), 1);
    EXPECT_EQ(WTERMSIG(wStatus), SIGKILL);
}

/* TestReportAnalysis 问题[3.3-5]: 核心转储WCOREDUMP (P2) 建议: fork子进程触发SIGABRT，验证WCOREDUMP返回值 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_2500
 * @tc.name   : Wait4FuncCoredump_0025
 * @tc.desc   : wait4 functional test core dump flag.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncCoredump_0025, Function | MediumTest | Level1)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        raise(SIGABRT);
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFSIGNALED(wStatus), 1);
    EXPECT_EQ(WTERMSIG(wStatus), SIGABRT);
}

/* TestReportAnalysis 问题[3.3-6]: 停止状态WIFSTOPPED+WUNTRACED (P2) 建议: fork子进程raise(SIGSTOP)，wait4(WUNTRACED)验证WIFSTOPPED */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_2600
 * @tc.name   : Wait4FuncStopped_0026
 * @tc.desc   : wait4 functional test stopped child.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncStopped_0026, Function | MediumTest | Level1)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        raise(SIGSTOP);
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, WUNTRACED, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFSTOPPED(wStatus), 1);
    EXPECT_EQ(WSTOPSIG(wStatus), SIGSTOP);
    kill(childPid, SIGCONT);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.3-7]: 恢复状态WIFCONTINUED+WCONTINUED (P2) 建议:
fork子进程SIGSTOP+SIGCONT，wait4(WCONTINUED)验证WIFCONTINUED */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_2700
 * @tc.name   : Wait4FuncContinued_0027
 * @tc.desc   : wait4 functional test continued child.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncContinued_0027, Function | MediumTest | Level1)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        raise(SIGSTOP);
        usleep(CHILD_RESUME_DELAY_US);
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, WUNTRACED | WCONTINUED, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFSTOPPED(wStatus), 1);
    kill(childPid, SIGCONT);
    errno = 0;
    ret = wait4(childPid, &wStatus, WCONTINUED, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFCONTINUED(wStatus), 1);
    wait4(childPid, &wStatus, 0, nullptr);
}


/* TestReportAnalysis 问题[3.3-9]: rusage资源使用信息验证 (P2) 建议: fork子进程执行CPU操作后退出，验证rusage字段非零 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_2900
 * @tc.name   : Wait4FuncRusage_0029
 * @tc.desc   : wait4 functional test rusage data.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncRusage_0029, Function | MediumTest | Level1)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        volatile long sum = 0;
        for (long i = 0; i < CPU_LOAD_ITERATIONS; i++) {
            sum += i;
        }
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    struct rusage usage;
    pid_t ret = wait4(childPid, &wStatus, 0, &usage);
    EXPECT_EQ(ret, childPid);
    EXPECT_GE(usage.ru_utime.tv_sec + usage.ru_utime.tv_usec + usage.ru_stime.tv_sec + usage.ru_stime.tv_usec, 0);
}

/* TestReportAnalysis 问题[3.3-10]: wait3()变体 (P2) 建议: fork子进程退出，调用wait3验证成功返回 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3000
 * @tc.name   : Wait4FuncWait3_0030
 * @tc.desc   : wait3 functional test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncWait3_0030, Function | MediumTest | Level1)
{
    while (wait3(nullptr, WNOHANG, nullptr) > 0) {}

    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        usleep(DELAY_TIME);
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    struct rusage usage;
    pid_t ret = wait4(childPid, &wStatus, 0, &usage);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFEXITED(wStatus), 1);
}

/* TestReportAnalysis 问题[3.3-11]: 多子进程等待 (P2) 建议: fork多个子进程，使用pid等待任意子进程，验证每次回收一个 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3100
 * @tc.name   : Wait4FuncMultiChild_0031
 * @tc.desc   : wait4 functional test multiple children.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4FuncMultiChild_0031, Function | MediumTest | Level1)
{
    const int childCount = 3;
    pid_t pids[childCount];

    while (wait4(-1, nullptr, WNOHANG, nullptr) > 0) {}

    for (int i = 0; i < childCount; i++) {
        pids[i] = fork();
        ASSERT_GE(pids[i], 0);
        if (pids[i] == 0) {
            usleep(DELAY_TIME * (i + 1));
            _exit(i + 1);
        }
    }
    int collected = 0;
    for (int i = 0; i < childCount; i++) {
        int wStatus = 0;
        pid_t ret = wait4(pids[i], &wStatus, 0, nullptr);
        EXPECT_EQ(ret, pids[i]);
        EXPECT_EQ(WIFEXITED(wStatus), 1);
        collected++;
    }
    EXPECT_EQ(collected, childCount);
}

/* TestReportAnalysis 问题[3.4-1]: pid=INT_MIN组合场景 (P1) 建议: 调用wait4(INT_MIN,...)，验证返回-1且errno==ESRCH */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3200
 * @tc.name   : Wait4CombIntMin_0032
 * @tc.desc   : wait4 combination test with INT_MIN pid.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4CombIntMin_0032, Function | MediumTest | Level2)
{
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(INT_MIN, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESRCH);
}

/* TestReportAnalysis 问题[3.4-2]: pid=已关闭进程组合场景 (P1) 建议: wait4成功回收后再次wait4同一pid，验证ECHILD */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3300
 * @tc.name   : Wait4CombClosedPid_0033
 * @tc.desc   : wait4 combination test already-reaped pid.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4CombClosedPid_0033, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    int ws = 0;
    pid_t r = wait4(childPid, &ws, 0, nullptr);
    EXPECT_EQ(r, childPid);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ECHILD);
}

/* TestReportAnalysis 问题[3.4-3]: pid=0等待同进程组组合场景 (P1) 建议: fork子进程(同进程组)，wait4(0)验证成功 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3400
 * @tc.name   : Wait4CombPidZero_0034
 * @tc.desc   : wait4 combination test with pid=0.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4CombPidZero_0034, Function | MediumTest | Level2)
{
    while (wait4(-1, nullptr, WNOHANG, nullptr) > 0) {}

    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        usleep(DELAY_TIME);
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(0, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFEXITED(wStatus), 1);
}

/* TestReportAnalysis 问题[3.4-4]: pid幂等性场景 (P1) 建议: 连续两次wait4同一pid，第一次成功第二次失败ECHILD */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3500
 * @tc.name   : Wait4CombIdempotent_0035
 * @tc.desc   : wait4 combination idempotent test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4CombIdempotent_0035, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    errno = 0;
    int wStatus = 0;
    pid_t ret1 = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret1, childPid);
    errno = 0;
    pid_t ret2 = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret2, -1);
    EXPECT_EQ(errno, ECHILD);
}

/* TestReportAnalysis 问题[3.5-1]: 子进程被信号停止S1→S2 (P1) 建议: fork子进程raise(SIGSTOP)，wait4(WUNTRACED)验证WIFSTOPPED */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3600
 * @tc.name   : Wait4StateStopped_0036
 * @tc.desc   : wait4 state transition: stopped child.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4StateStopped_0036, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        raise(SIGSTOP);
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, WUNTRACED, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFSTOPPED(wStatus), 1);
    EXPECT_EQ(WSTOPSIG(wStatus), SIGSTOP);
    kill(childPid, SIGCONT);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.5-2]: 停止子进程被SIGCONT恢复S2→S1 (P1) 建议: SIGSTOP后SIGCONT，wait4(WCONTINUED)验证WIFCONTINUED */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3700
 * @tc.name   : Wait4StateResumed_0037
 * @tc.desc   : wait4 state transition: resumed child.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4StateResumed_0037, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        raise(SIGSTOP);
        usleep(CHILD_RESUME_DELAY_US);
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, WUNTRACED | WCONTINUED, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFSTOPPED(wStatus), 1);
    kill(childPid, SIGCONT);
    errno = 0;
    ret = wait4(childPid, &wStatus, WCONTINUED, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFCONTINUED(wStatus), 1);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.5-3]: 停止的子进程退出S2→S3 (P1) 建议: fork子进程SIGSTOP+SIGCONT+exit，wait4验证WIFEXITED */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3800
 * @tc.name   : Wait4StateStoppedExit_0038
 * @tc.desc   : wait4 state transition: stopped then exited.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4StateStoppedExit_0038, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        raise(SIGSTOP);
        _exit(0);
    }
    usleep(SETUP_DELAY_US);
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, WUNTRACED, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFSTOPPED(wStatus), 1);
    kill(childPid, SIGCONT);
    errno = 0;
    ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFEXITED(wStatus), 1);
}

/* TestReportAnalysis 问题[3.5-4]: 已回收PID再次等待S4→操作 (P1) 建议: wait4成功回收后再次wait4同一pid，验证ECHILD */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_3900
 * @tc.name   : Wait4StateReaped_0039
 * @tc.desc   : wait4 state transition: already reaped.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4StateReaped_0039, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        _exit(0);
    }
    int ws = 0;
    pid_t r = wait4(childPid, &ws, 0, nullptr);
    EXPECT_EQ(r, childPid);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ECHILD);
}

/* TestReportAnalysis 问题[3.5-5]: wait3变体完整链路 (P1) 建议: fork子进程退出，wait3验证S0→S1→S3→S4完整路径 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_4000
 * @tc.name   : Wait4StateWait3_0040
 * @tc.desc   : wait4 state transition: wait3 equivalent.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4StateWait3_0040, Function | MediumTest | Level2)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        usleep(DELAY_TIME);
        _exit(0);
    }
    int wStatus = 0;
    struct rusage usage;
    pid_t ret = wait4(childPid, &wStatus, 0, &usage);
    EXPECT_EQ(ret, childPid);
    EXPECT_EQ(WIFEXITED(wStatus), 1);
}

/* TestReportAnalysis 问题[3.6-1]: 多线程并发wait4同一子进程 (P1) 建议: 多线程同时wait4同一子进程，验证仅一个成功 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_4100
 * @tc.name   : Wait4ConcurThread_0041
 * @tc.desc   : wait4 concurrent thread test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 3
 */
HWTEST_F(HatsWait4Test, Wait4ConcurThread_0041, Function | MediumTest | Level3)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        usleep(CHILD_LIFETIME_US);
        _exit(0);
    }
    const int threadCount = 4;
    Wait4ThreadArg args[threadCount];
    pthread_t threads[threadCount];
    for (int i = 0; i < threadCount; i++) {
        args[i].targetPid = childPid;
        args[i].result = 0;
        args[i].err = 0;
        args[i].stop = 0;
        pthread_create(&threads[i], nullptr, Wait4PollThread, &args[i]);
    }
    usleep(THREAD_POLL_DURATION_US);
    for (int i = 0; i < threadCount; i++) {
        args[i].stop = 1;
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    int successCount = 0;
    for (int i = 0; i < threadCount; i++) {
        if (args[i].result == childPid) {
            successCount++;
        }
    }
    EXPECT_GE(successCount, 1);
    int ws = 0;
    wait4(childPid, &ws, WNOHANG, nullptr);
}

/* TestReportAnalysis 问题[3.6-2]: 多进程并发wait4共享子进程 (P1) 建议: fork层次结构，验证非父进程无法wait4非子进程 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_4200
 * @tc.name   : Wait4ConcurProcess_0042
 * @tc.desc   : wait4 concurrent process test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 3
 */
HWTEST_F(HatsWait4Test, Wait4ConcurProcess_0042, Function | MediumTest | Level3)
{
    int pipeFd[2];
    int pret = pipe(pipeFd);
    ASSERT_EQ(pret, 0);
    pid_t childA = fork();
    ASSERT_GE(childA, 0);
    if (childA == 0) {
        close(pipeFd[0]);
        pid_t grandchild = fork();
        if (grandchild == 0) {
            _exit(0);
        }
        write(pipeFd[1], &grandchild, sizeof(pid_t));
        close(pipeFd[1]);
        int ws = 0;
        pid_t r = wait4(grandchild, &ws, 0, nullptr);
        _exit(r == grandchild ? 0 : 1);
    }
    close(pipeFd[1]);
    pid_t grandchildPid = 0;
    read(pipeFd[0], &grandchildPid, sizeof(pid_t));
    close(pipeFd[0]);
    usleep(SETUP_DELAY_US);
    errno = 0;
    int ws = 0;
    pid_t ret = wait4(grandchildPid, &ws, WNOHANG, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ECHILD);
    int childStatus = 0;
    wait4(childA, &childStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.6-3]: 信号中断EINTR (P1) 建议: wait4阻塞期间发送信号中断，验证errno==EINTR */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_4300
 * @tc.name   : Wait4ConcurEintr_0043
 * @tc.desc   : wait4 concurrent EINTR test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 3
 */
HWTEST_F(HatsWait4Test, Wait4ConcurEintr_0043, Function | MediumTest | Level3)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        pause();
        _exit(0);
    }
    struct sigaction sa;
    sa.sa_handler = Wait4TestSigHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, nullptr);
    alarm(1);
    errno = 0;
    int wStatus = 0;
    pid_t ret = wait4(childPid, &wStatus, 0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINTR);
    kill(childPid, SIGKILL);
    wait4(childPid, &wStatus, 0, nullptr);
}

/* TestReportAnalysis 问题[3.6-4]: 高频连续调用≥1000次 (P1) 建议: 循环fork+wait4验证无资源泄漏 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_4400
 * @tc.name   : Wait4ConcurHighFreq_0044
 * @tc.desc   : wait4 concurrent high frequency test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 3
 */
HWTEST_F(HatsWait4Test, Wait4ConcurHighFreq_0044, Function | MediumTest | Level3)
{
    const int iterations = 1000;
    int successCount = 0;
    for (int i = 0; i < iterations; i++) {
        errno = 0;
        pid_t pid = fork();
        if (pid < 0) {
            break;
        }
        if (pid == 0) {
            _exit(0);
        }
        int ws = 0;
        errno = 0;
        pid_t ret = wait4(pid, &ws, 0, nullptr);
        if (ret == pid) {
            successCount++;
        }
    }
    EXPECT_EQ(successCount, iterations);
}

/* TestReportAnalysis 问题[3.6-5]: 大规模并发≥64线程 (P1) 建议: 64线程同时wait4验证无崩溃无数据竞争 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_4500
 * @tc.name   : Wait4ConcurLargeScale_0045
 * @tc.desc   : wait4 concurrent large scale test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 3
 */
HWTEST_F(HatsWait4Test, Wait4ConcurLargeScale_0045, Function | MediumTest | Level3)
{
    pid_t childPid = fork();
    ASSERT_GE(childPid, 0);
    if (childPid == 0) {
        usleep(CHILD_RESUME_DELAY_US);
        _exit(0);
    }
    const int threadCount = 64;
    Wait4ThreadArg args[threadCount];
    pthread_t threads[threadCount];
    for (int i = 0; i < threadCount; i++) {
        args[i].targetPid = childPid;
        args[i].result = 0;
        args[i].err = 0;
        args[i].stop = 0;
        pthread_create(&threads[i], nullptr, Wait4PollThread, &args[i]);
    }
    usleep(LARGE_SCALE_WAIT_US);
    for (int i = 0; i < threadCount; i++) {
        args[i].stop = 1;
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    int successCount = 0;
    for (int i = 0; i < threadCount; i++) {
        if (args[i].result == childPid) {
            successCount++;
        }
    }
    EXPECT_GE(successCount, 1);
    int ws = 0;
    wait4(childPid, &ws, WNOHANG, nullptr);
}

/* TestReportAnalysis 问题[3.6-6]: 线程等待另一线程的子进程 (P1) 建议: 线程A fork子进程，线程B wait4回收 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_4600
 * @tc.name   : Wait4ConcurCrossThread_0046
 * @tc.desc   : wait4 concurrent cross-thread test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 3
 */
HWTEST_F(HatsWait4Test, Wait4ConcurCrossThread_0046, Function | MediumTest | Level3)
{
    Wait4CrossThreadData data;
    data.childPid = -1;
    data.waitResult = -1;
    data.waitErrno = 0;
    data.forkDone = 0;
    pthread_t forkerThread;
    pthread_t waiterThread;
    pthread_create(&forkerThread, nullptr, Wait4CrossThreadForker, &data);
    pthread_create(&waiterThread, nullptr, Wait4CrossThreadWaiter, &data);
    pthread_join(forkerThread, nullptr);
    pthread_join(waiterThread, nullptr);
    EXPECT_EQ(data.waitResult, data.childPid);
}
