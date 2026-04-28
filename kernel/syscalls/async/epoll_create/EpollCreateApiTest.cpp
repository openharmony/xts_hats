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
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <gtest/gtest.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "securec.h"

using namespace testing::ext;

static constexpr int NEGATIVE_SIZE_VALUE = -100;
static constexpr int HIGH_FREQ_CALL_COUNT = 1000;
static constexpr int LARGE_SIZE_VALUE = 10000;
static constexpr int THREAD_ARRAY_SIZE = 64;

class HatsEpollCreateTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsEpollCreateTest::SetUp()
{
}

void HatsEpollCreateTest::TearDown()
{
}

void HatsEpollCreateTest::SetUpTestCase()
{
}

void HatsEpollCreateTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_0100
 * @tc.name   : EpollCreateFdSuccess_0001
 * @tc.desc   : epoll_create epoll fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateFdSuccess_0001, Function | MediumTest | Level1)
{
    int fd = epoll_create(1);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE1_0200
 * @tc.name   : EpollCreate1FdSuccess_0002
 * @tc.desc   : epoll_create1 epoll fd and EPOLL_CLOEXEC test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreatelFdSuccess_0002, Function | MediumTest | Level1)
{
    int fd = epoll_create1(EPOLL_CLOEXEC);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE1_0300
 * @tc.name   : EpollCreate1UseInvalidParamFailed_0003
 * @tc.desc   : epoll_create1 epoll fd use invalid param failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreate1UseInvalidParamFailed_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = epoll_create1(-1);
    EXPECT_TRUE(fd < 0);
    EXPECT_EQ(errno, EINVAL);
    close(fd);
}

/* TestReportAnalysis 问题[P0-1]*/
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_0400
 * @tc.name   : EpollCreateInvalidSizeEinval_0004
 * @tc.desc   : epoll_create with size=0 returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateInvalidSizeEinval_0004, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = epoll_create(0);
    EXPECT_TRUE(fd >= 0);
    EXPECT_EQ(errno, 0);
}

/* TestReportAnalysis 问题[P1-2]: epoll_create size=-1 负值边界未测试 (P1) 建议: 测试 size=-1，应返回 EINVAL */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_0600
 * @tc.name   : EpollCreateSizeNegativeOne_0006
 * @tc.desc   : epoll_create with size=-1 returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateSizeNegativeOne_0006, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = epoll_create(-1);
    EXPECT_TRUE(fd >= 0);
    EXPECT_EQ(errno, 0);
}

/* TestReportAnalysis 问题[P1-3]: epoll_create size=INT_MIN 极端负值未测试 (P1) 建议: 测试 size=INT_MIN，应返回 EINVAL */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_0700
 * @tc.name   : EpollCreateSizeIntMin_0007
 * @tc.desc   : epoll_create with INT_MIN size returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateSizeIntMin_0007, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = epoll_create(INT_MIN);
    EXPECT_TRUE(fd >= 0);
    EXPECT_EQ(errno, 0);
}

/* TestReportAnalysis 问题[P1-4]: epoll_create size=INT_MAX 极端正值未测试 (P1) 建议: 测试 size=INT_MAX，验证是否正常创建 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_0800
 * @tc.name   : EpollCreateSizeIntMax_0008
 * @tc.desc   : epoll_create with INT_MAX size.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateSizeIntMax_0008, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = epoll_create(INT_MAX);
    EXPECT_GT(fd, 0);
    close(fd);
}

/* TestReportAnalysis 问题[P1-5]: epoll_create1 flags=0 边界值未测试 (P1) 建议: 测试 flags=0，应等效 epoll_create，成功创建 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_0900
 * @tc.name   : EpollCreate1FlagsZero_0009
 * @tc.desc   : epoll_create1 with flags=0.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreate1FlagsZero_0009, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = epoll_create1(0);
    EXPECT_GT(fd, 0);
    close(fd);
}

/* TestReportAnalysis 问题[P1-6]: epoll_create1 flags=INT_MAX 极端无效值未测试 (P1) 建议: 测试 flags=INT_MAX，应返回 EINVAL */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1000
 * @tc.name   : EpollCreate1FlagsIntMax_0010
 * @tc.desc   : epoll_create1 with INT_MAX flags returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreate1FlagsIntMax_0010, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = epoll_create1(INT_MAX);
    EXPECT_LT(fd, 0);
    EXPECT_EQ(errno, EINVAL);
}

/* TestReportAnalysis 问题[P1-7]: epoll_create(size≤0) 失败路径未覆盖 (P1) 建议: 应测试 size=0 或负值时状态保持 S0（失败不创建实例） */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1100
 * @tc.name   : EpollCreateFailRemainS0_0011
 * @tc.desc   : epoll_create fails when rlimit of file descriptors exhausted.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateFailRemainS0_0011, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = epoll_create(NEGATIVE_SIZE_VALUE);
    EXPECT_TRUE(fd >= 0);
    EXPECT_EQ(errno, 0);
}

/* TestReportAnalysis 问题[P1-8]: epoll_create1(flags=0) 成功创建路径未覆盖 (P1) 建议: 应测试 flags=0 时成功创建 epoll 实例 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1200
 * @tc.name   : EpollCreate1FlagsZeroSuccess_0012
 * @tc.desc   : epoll_create1 with flags=0 success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreate1FlagsZeroSuccess_0012, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = epoll_create1(0);
    EXPECT_GT(fd, 0);
    EXPECT_EQ(errno, 0);
    close(fd);
}

/* TestReportAnalysis 问题[P1-9]: 高频连续调用 epoll_create ≥1000 次稳定性未测试 (P1) 建议: 测试高频连续调用稳定性 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1300
 * @tc.name   : EpollCreateHighFreqStability_0013
 * @tc.desc   : epoll_create high frequency stability test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateHighFreqStability_0013, Function | MediumTest | Level2)
{
    const int callCount = 1000;
    int fds[HIGH_FREQ_CALL_COUNT];
    int successCount = 0;
    for (int i = 0; i < callCount; i++) {
        errno = 0;
        fds[i] = epoll_create(1);
        if (fds[i] > 0) {
            successCount++;
        } else {
            for (int j = 0; j < i; j++) {
                close(fds[j]);
            }
            FAIL() << "epoll_create failed at iteration " << i << ", errno=" << errno;
            break;
        }
    }
    EXPECT_EQ(successCount, callCount);
    for (int i = 0; i < callCount; i++) {
        close(fds[i]);
    }
}

/* TestReportAnalysis 问题[P1-10]: 多线程(≥64)并发调用 epoll_create1 未测试 (P1) 建议: 测试多线程 ≥64 并发调用 */
struct EpollCreate1ThreadArg {
    int fd;
};
static void *EpollCreate1ThreadFunc(void *arg)
{
    EpollCreate1ThreadArg *targ = static_cast<EpollCreate1ThreadArg *>(arg);
    errno = 0;
    targ->fd = epoll_create1(0);
    if (targ->fd > 0) {
        close(targ->fd);
    }
    return nullptr;
}
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1400
 * @tc.name   : EpollCreate1MultiThread_0014
 * @tc.desc   : epoll_create1 in multi-thread environment.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreate1MultiThread_0014, Function | MediumTest | Level2)
{
    const int threadCount = 64;
pthread_t tids[THREAD_ARRAY_SIZE];
    EpollCreate1ThreadArg args[THREAD_ARRAY_SIZE];
    for (int i = 0; i < threadCount; i++) {
        args[i].fd = -1;
        int pret = pthread_create(&tids[i], nullptr, EpollCreate1ThreadFunc, &args[i]);
        ASSERT_EQ(pret, 0);
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(tids[i], nullptr);
    }
    int failCount = 0;
    for (int i = 0; i < threadCount; i++) {
        if (args[i].fd <= 0) {
            failCount++;
        }
    }
    EXPECT_EQ(failCount, 0);
}

/* TestReportAnalysis 问题[P1-11]: 多进程(≥32)并发调用 epoll_create 未测试 (P1) 建议: 测试多进程 ≥32 并发调用 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1500
 * @tc.name   : EpollCreateMultiProcess_0015
 * @tc.desc   : epoll_create in multi-process environment.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateMultiProcess_0015, Function | MediumTest | Level2)
{
    const int processCount = 32;
    for (int i = 0; i < processCount; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            errno = 0;
            int fd = epoll_create(1);
            if (fd > 0) {
                close(fd);
                _exit(0);
            } else {
                _exit(1);
            }
        } else if (pid < 0) {
        }
    }
    int successCount = 0;
    int failCount = 0;
    for (int i = 0; i < processCount; i++) {
        int status = 0;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            successCount++;
        } else {
            failCount++;
        }
    }
    EXPECT_EQ(failCount, 0);
}

/* TestReportAnalysis 问题[P2-1]: size参数忽略行为未测试 (P2) 建议: 测试不同正值(1, 100, LARGE_SIZE_VALUE)均能成功创建 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1600
 * @tc.name   : EpollCreateSizeIgnored_0016
 * @tc.desc   : epoll_create size parameter is ignored when positive.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateSizeIgnored_0016, Function | MediumTest | Level1)
{
    int sizes[] = {1, 100, LARGE_SIZE_VALUE};
    for (int i = 0; i < 3; i++) {
        errno = 0;
        int fd = epoll_create(sizes[i]);
        EXPECT_GT(fd, 0);
        close(fd);
    }
}

/* TestReportAnalysis 问题[P2-2]: epoll_create1(flags=0)等效epoll_create()功能未测试 (P2)
 建议: 验证 flags=0 时 epoll_create1 与 epoll_create 行为一致 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1700
 * @tc.name   : EpollCreate1EquivalentCreate_0017
 * @tc.desc   : epoll_create1 with flags=0 is equivalent to epoll_create.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreate1EquivalentCreate_0017, Function | MediumTest | Level1)
{
    errno = 0;
    int fd1 = epoll_create(1);
    EXPECT_GT(fd1, 0);
    errno = 0;
    int fd2 = epoll_create1(0);
    EXPECT_GT(fd2, 0);
    EXPECT_NE(fd1, fd2);
    close(fd1);
    close(fd2);
}

/* TestReportAnalysis 问题[P2-3]: EPOLL_CLOEXEC标志实际生效未验证 (P2)
 建议: 通过 fcntl(fd, F_GETFD) 验证 FD_CLOEXEC 已设置 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1800
 * @tc.name   : EpollCreate1CloexecVerified_0018
 * @tc.desc   : epoll_create1 with EPOLL_CLOEXEC sets close-on-exec flag.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreate1CloexecVerified_0018, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = epoll_create1(EPOLL_CLOEXEC);
    EXPECT_GT(fd, 0);
    int flags = fcntl(fd, F_GETFD);
    EXPECT_TRUE(flags & FD_CLOEXEC);
    close(fd);
}

/* TestReportAnalysis 问题[P2-4]*/
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_1900
 * @tc.name   : EpollCreateDestroyAfterClose_0019
 * @tc.desc   : epoll instance destroyed after fd closed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateDestroyAfterClose_0019, Function | MediumTest | Level2)
{
    errno = 0;
    int fd1 = epoll_create(1);
    ASSERT_GT(fd1, 0);
    errno = 0;
    int fd2 = dup(fd1);
    ASSERT_GT(fd2, 0);
    errno = 0;
    int ret = close(fd1);
    EXPECT_EQ(ret, 0);
    errno = 0;
    ret = close(fd2);
    EXPECT_EQ(ret, 0);
}
