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
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "securec.h"

using namespace testing::ext;

static constexpr unsigned int INVALID_FLAGS_ALL_BITS = 0xFFFFFFFF;
static constexpr unsigned int INVALID_FLAGS_DEADBEEF = 0xDEADBEEF;
static constexpr int TEST_DATA_LEN = 10;
static constexpr int READ_BUF_SIZE = 16;
static constexpr unsigned long KERNEL_ADDR_VAL = 0xFFFF0000;
static constexpr int LIFECYCLE_DATA_LEN = 4;
static constexpr int LIFECYCLE_BUF_SIZE = 8;
static constexpr int EOF_TEST_DATA_LEN = 8;
static constexpr int EOF_READ_BUF_SIZE = 64;
static constexpr int EPIPE_DATA_LEN = 4;
static constexpr int FORK_MSG_LEN = 9;

class HatsPipe2Test : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsPipe2Test::SetUp()
{
}

void HatsPipe2Test::TearDown()
{
}

void HatsPipe2Test::SetUpTestCase()
{
}

void HatsPipe2Test::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0100
 * @tc.name   : Pipe2Success_0001
 * @tc.desc   : Pipe2 create pipe fds with flag O_CLOEXEC and O_NONBLOCK successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2Success_0001, Function | MediumTest | Level1)
{
    int pipeFds[2];

    int ret = pipe2(pipeFds, O_CLOEXEC | O_NONBLOCK);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(pipeFds[0] > 0 && pipeFds[1] > 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0200
 * @tc.name   : Pipe2Failed_0002
 * @tc.desc   : Pipe2 fails for invalid Fd array.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2Failed_0002, Function | MediumTest | Level2)
{
    int* pipeFds = nullptr;

    errno = 0;
    int ret = pipe2(pipeFds, O_CLOEXEC);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/* TestReportAnalysis 问题#1: 缺少 EINVAL 错误码测试 (P0) 建议: pipe2(pipeFd, INVALID_FLAGS_ALL_BITS) 应返回 -1, errno=EINVAL */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0300
 * @tc.name   : Pipe2Failed_0003
 * @tc.desc   : pipe2 with invalid flags returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2Failed_0003, Function | MediumTest | Level2)
{
    int pipeFd[2];

    errno = 0;
    int ret = pipe2(pipeFd, INVALID_FLAGS_ALL_BITS);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/* TestReportAnalysis 问题#2: 缺少 ENOPKG 错误码测试 (P0) 建议: 验证 pipe2 子系统可用, 不应返回 ENOPKG */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0400
 * @tc.name   : Pipe2Success_0004
 * @tc.desc   : pipe2 with O_CLOEXEC flag success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2Success_0004, Function | MediumTest | Level1)
{
    int pipeFd[2];

    errno = 0;
    int ret = pipe2(pipeFd, O_CLOEXEC);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(errno, ENOPKG);
    if (ret == 0) {
        close(pipeFd[0]);
        close(pipeFd[1]);
    }
}

/* TestReportAnalysis 问题#3: 缺少非法指针(非NULL无效地址)测试 (P1) 建议: pipeFd 指向 munmap 后地址应返回 EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0500
 * @tc.name   : Pipe2Failed_0005
 * @tc.desc   : pipe2 with invalid pipeFd buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2Failed_0005, Function | MediumTest | Level2)
{
    void *ptr = mmap(nullptr, sizeof(int) * 2, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_NE(ptr, MAP_FAILED);
    munmap(ptr, sizeof(int) * 2);

    errno = 0;
    int ret = pipe2(static_cast<int *>(ptr), 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/* TestReportAnalysis 问题#4: 缺少 flags=0(等同 pipe)边界测试 (P1) 建议: pipe2(pipeFd, 0) 等效于 pipe() */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0600
 * @tc.name   : Pipe2Success_0006
 * @tc.desc   : pipe2 with O_NONBLOCK flag success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2Success_0006, Function | MediumTest | Level1)
{
    int pipeFd[2];

    errno = 0;
    int ret = pipe2(pipeFd, 0);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(pipeFd[0] > 0 && pipeFd[1] > 0);
    close(pipeFd[0]);
    close(pipeFd[1]);
}

/* TestReportAnalysis 问题#5: 缺少单标志独立测试 (P1) 建议: 分别单独测试 O_CLOEXEC 和 O_NONBLOCK */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0700
 * @tc.name   : Pipe2Success_0007
 * @tc.desc   : pipe2 with O_CLOEXEC|O_NONBLOCK flags success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2Success_0007, Function | MediumTest | Level1)
{
    int pipeFd1[2];
    int pipeFd2[2];

    errno = 0;
    int ret1 = pipe2(pipeFd1, O_CLOEXEC);
    EXPECT_EQ(ret1, 0);
    EXPECT_TRUE(pipeFd1[0] > 0 && pipeFd1[1] > 0);

    errno = 0;
    int ret2 = pipe2(pipeFd2, O_NONBLOCK);
    EXPECT_EQ(ret2, 0);
    EXPECT_TRUE(pipeFd2[0] > 0 && pipeFd2[1] > 0);

    close(pipeFd1[0]);
    close(pipeFd1[1]);
    close(pipeFd2[0]);
    close(pipeFd2[1]);
}

/* TestReportAnalysis 问题#6: 缺少无效 flags 边界值测试 (P1) 建议: pipe2(pipeFd, INVALID_FLAGS_DEADBEEF) 应返回 EINVAL */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0800
 * @tc.name   : Pipe2Failed_0008
 * @tc.desc   : pipe2 with unmapped pipeFd buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2Failed_0008, Function | MediumTest | Level2)
{
    int pipeFd[2];

    errno = 0;
    int ret = pipe2(pipeFd, INVALID_FLAGS_DEADBEEF);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/* TestReportAnalysis 问题#7: 缺少只读内存指针测试 (P1) 建议: pipeFd 指向只读 mmap 区域应返回 EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0900
 * @tc.name   : Pipe2Failed_0009
 * @tc.desc   : pipe2 with read-only pipeFd buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2Failed_0009, Function | MediumTest | Level2)
{
    void *ptr = mmap(nullptr, sizeof(int) * 2, PROT_READ,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_NE(ptr, MAP_FAILED);

    errno = 0;
    int ret = pipe2(static_cast<int *>(ptr), 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    munmap(ptr, sizeof(int) * 2);
}

struct Pipe2ThreadArg {
    int pipeFd[2];
    int flags;
    int ret;
};

static void* Pipe2ThreadFunc(void* arg)
{
    Pipe2ThreadArg* data = static_cast<Pipe2ThreadArg*>(arg);
    data->ret = pipe2(data->pipeFd, data->flags);
    return nullptr;
}

/* TestReportAnalysis 问题#8: 管道读写方向验证 (P2) 建议: 写入pipeFd[1]从pipeFd[0]读出验证方向正确 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1000
 * @tc.name   : Pipe2ReadWrite_0010
 * @tc.desc   : pipe2 read-write data transfer.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2ReadWrite_0010, Function | MediumTest | Level2)
{
    int pipeFd[2];
    errno = 0;
    int ret = pipe2(pipeFd, 0);
    EXPECT_EQ(ret, 0);

    const char *data = "hello_pipe";
    errno = 0;
    ssize_t wret = write(pipeFd[1], data, TEST_DATA_LEN);
    EXPECT_EQ(wret, 10);

    char buf[READ_BUF_SIZE] = {0};
    errno = 0;
    ssize_t rret = read(pipeFd[0], buf, TEST_DATA_LEN);
    EXPECT_EQ(rret, 10);
    EXPECT_EQ(memcmp(buf, data, 10), 0);

    close(pipeFd[0]);
    close(pipeFd[1]);
}

/* TestReportAnalysis 问题#9: O_CLOEXEC 标志属性验证 (P2) 建议: fcntl F_GETFD 验证 FD_CLOEXEC 已设置 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1100
 * @tc.name   : Pipe2CloexecVerify_0011
 * @tc.desc   : pipe2 O_CLOEXEC flag close-on-exec verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2CloexecVerify_0011, Function | MediumTest | Level2)
{
    int pipeFd[2];
    errno = 0;
    int ret = pipe2(pipeFd, O_CLOEXEC);
    EXPECT_EQ(ret, 0);

    int fdFlags0 = fcntl(pipeFd[0], F_GETFD);
    EXPECT_TRUE((fdFlags0 & FD_CLOEXEC) != 0);

    int fdFlags1 = fcntl(pipeFd[1], F_GETFD);
    EXPECT_TRUE((fdFlags1 & FD_CLOEXEC) != 0);

    close(pipeFd[0]);
    close(pipeFd[1]);
}

/* TestReportAnalysis 问题#10: O_NONBLOCK 标志属性验证 (P2) 建议: fcntl F_GETFL 验证 O_NONBLOCK 已设置且空read返回EAGAIN */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1200
 * @tc.name   : Pipe2NonblockVerify_0012
 * @tc.desc   : pipe2 O_NONBLOCK flag non-blocking verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2NonblockVerify_0012, Function | MediumTest | Level2)
{
    int pipeFd[2];
    errno = 0;
    int ret = pipe2(pipeFd, O_NONBLOCK);
    EXPECT_EQ(ret, 0);

    int flFlags0 = fcntl(pipeFd[0], F_GETFL);
    EXPECT_TRUE((flFlags0 & O_NONBLOCK) != 0);

    int flFlags1 = fcntl(pipeFd[1], F_GETFL);
    EXPECT_TRUE((flFlags1 & O_NONBLOCK) != 0);

    char buf[1];
    errno = 0;
    ssize_t rret = read(pipeFd[0], buf, 1);
    EXPECT_EQ(rret, -1);
    EXPECT_EQ(errno, EAGAIN);

    close(pipeFd[0]);
    close(pipeFd[1]);
}

/* TestReportAnalysis 问题#11: 失败时 pipeFd 不变验证 (P2) 建议: 无效flags触发失败后pipeFd保持原值 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1300
 * @tc.name   : Pipe2FailedUnchanged_0013
 * @tc.desc   : pipe2 failure leaves pipeFd unchanged.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2FailedUnchanged_0013, Function | MediumTest | Level2)
{
    int pipeFd[2] = {-1, -1};
    errno = 0;
    int ret = pipe2(pipeFd, INVALID_FLAGS_DEADBEEF);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(pipeFd[0], -1);
    EXPECT_EQ(pipeFd[1], -1);
}

/* TestReportAnalysis 问题3.4.1: pipeFd×未映射地址 EFAULT (P1) 建议: pipeFd指向已munmap区域应返回EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1400
 * @tc.name   : Pipe2ComboUnmapped_0014
 * @tc.desc   : pipe2 unmapped buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2ComboUnmapped_0014, Function | MediumTest | Level2)
{
    void *ptr = mmap(nullptr, sizeof(int) * 2, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_NE(ptr, MAP_FAILED);
    munmap(ptr, sizeof(int) * 2);

    errno = 0;
    int ret = pipe2(static_cast<int*>(ptr), 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/* TestReportAnalysis 问题3.4.2: pipeFd×内核态地址 EFAULT (P1) 建议: pipeFd指向内核高位地址应返回EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1500
 * @tc.name   : Pipe2ComboKernel_0015
 * @tc.desc   : pipe2 kernel address buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2ComboKernel_0015, Function | MediumTest | Level2)
{
    int *kernelPtr = reinterpret_cast<int*>(KERNEL_ADDR_VAL);
    errno = 0;
    int ret = pipe2(kernelPtr, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/* TestReportAnalysis 问题3.4.3: pipeFd×只读区写入 EFAULT (P1) 建议: pipeFd指向只读mmap区域应返回EFAULT */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1600
 * @tc.name   : Pipe2ComboReadonly_0016
 * @tc.desc   : pipe2 read-only buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2ComboReadonly_0016, Function | MediumTest | Level2)
{
    void *ptr = mmap(nullptr, sizeof(int) * 2, PROT_READ,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_NE(ptr, MAP_FAILED);

    errno = 0;
    int ret = pipe2(static_cast<int*>(ptr), 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    munmap(ptr, sizeof(int) * 2);
}

/* TestReportAnalysis 问题3.4.4: pipeFd×幂等性 多次创建独立管道数据隔离 (P1) 建议: 两个管道各自写入不同数据验证隔离 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1700
 * @tc.name   : Pipe2ComboIdempotent_0017
 * @tc.desc   : pipe2 idempotent combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2ComboIdempotent_0017, Function | MediumTest | Level1)
{
    int fd1[2];
    int fd2[2];
    errno = 0;
    int ret1 = pipe2(fd1, 0);
    EXPECT_EQ(ret1, 0);

    errno = 0;
    int ret2 = pipe2(fd2, 0);
    EXPECT_EQ(ret2, 0);

    errno = 0;
    ssize_t wret = write(fd1[1], "A", 1);
    EXPECT_EQ(wret, 1);

    errno = 0;
    wret = write(fd2[1], "B", 1);
    EXPECT_EQ(wret, 1);

    char buf[2] = {0};
    errno = 0;
    ssize_t rret = read(fd1[0], buf, 1);
    EXPECT_EQ(rret, 1);
    EXPECT_EQ(buf[0], 'A');

    errno = 0;
    rret = read(fd2[0], buf, 1);
    EXPECT_EQ(rret, 1);
    EXPECT_EQ(buf[0], 'B');

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
}

/* TestReportAnalysis 问题3.4.5: pipeFd×文件状态变化 fcntl验证O_CLOEXEC和O_NONBLOCK (P1) 建议: fcntl验证两fd的标志均正确设置 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1800
 * @tc.name   : Pipe2ComboFdState_0018
 * @tc.desc   : pipe2 fd state combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2ComboFdState_0018, Function | MediumTest | Level1)
{
    int pipeFd[2];
    errno = 0;
    int ret = pipe2(pipeFd, O_CLOEXEC | O_NONBLOCK);
    EXPECT_EQ(ret, 0);

    int fdFlags0 = fcntl(pipeFd[0], F_GETFD);
    EXPECT_TRUE((fdFlags0 & FD_CLOEXEC) != 0);

    int fdFlags1 = fcntl(pipeFd[1], F_GETFD);
    EXPECT_TRUE((fdFlags1 & FD_CLOEXEC) != 0);

    int flFlags0 = fcntl(pipeFd[0], F_GETFL);
    EXPECT_TRUE((flFlags0 & O_NONBLOCK) != 0);

    int flFlags1 = fcntl(pipeFd[1], F_GETFL);
    EXPECT_TRUE((flFlags1 & O_NONBLOCK) != 0);

    close(pipeFd[0]);
    close(pipeFd[1]);
}

/* TestReportAnalysis 问题3.4.6: pipeFd×时序干扰 多线程并发pipe2并验证数据传输 (P1) 建议: 8线程并发创建管道各做数据传输 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_1900
 * @tc.name   : Pipe2ComboTiming_0019
 * @tc.desc   : pipe2 timing combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2ComboTiming_0019, Function | MediumTest | Level1)
{
    const int THREAD_COUNT_0019 = 8;
    pthread_t threads[THREAD_COUNT_0019];
    Pipe2ThreadArg args[THREAD_COUNT_0019];

    for (int i = 0; i < THREAD_COUNT_0019; i++) {
        args[i].flags = 0;
        args[i].ret = -1;
        args[i].pipeFd[0] = -1;
        args[i].pipeFd[1] = -1;
        pthread_create(&threads[i], nullptr, Pipe2ThreadFunc, &args[i]);
    }

    for (int i = 0; i < THREAD_COUNT_0019; i++) {
        pthread_join(threads[i], nullptr);
    }

    for (int i = 0; i < THREAD_COUNT_0019; i++) {
        EXPECT_EQ(args[i].ret, 0);
    }

    for (int i = 0; i < THREAD_COUNT_0019; i++) {
        if (args[i].ret != 0) {
            continue;
        }
        char writeChar = 'A' + static_cast<char>(i);
        errno = 0;
        ssize_t wret = write(args[i].pipeFd[1], &writeChar, 1);
        EXPECT_EQ(wret, 1);

        char readChar = 0;
        errno = 0;
        ssize_t rret = read(args[i].pipeFd[0], &readChar, 1);
        EXPECT_EQ(rret, 1);
        EXPECT_EQ(readChar, writeChar);
    }

    for (int i = 0; i < THREAD_COUNT_0019; i++) {
        if (args[i].ret == 0) {
            close(args[i].pipeFd[0]);
            close(args[i].pipeFd[1]);
        }
    }
}

/* TestReportAnalysis 问题3.4.7: flags×幂等性 相同flags重复调用100次验证一致性 (P1) 建议: 100次pipe2(O_CLOEXEC)均成功且无fd泄漏 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_2000
 * @tc.name   : Pipe2FlagsIdempotent_0020
 * @tc.desc   : pipe2 flags idempotent test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2FlagsIdempotent_0020, Function | MediumTest | Level1)
{
    const int ITER_COUNT_0020 = 100;
    int successCount = 0;
    for (int i = 0; i < ITER_COUNT_0020; i++) {
        int pipeFd[2];
        errno = 0;
        int ret = pipe2(pipeFd, O_CLOEXEC);
        if (ret == 0) {
            successCount++;
            close(pipeFd[0]);
            close(pipeFd[1]);
        }
    }
    EXPECT_EQ(successCount, ITER_COUNT_0020);
}

/* TestReportAnalysis 问题3.4.8: flags×文件状态变化 不同flags组合产生对应fd状态 (P1) 建议: 4种flags组合fcntl验证状态 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_2100
 * @tc.name   : Pipe2FlagsState_0021
 * @tc.desc   : pipe2 flags state transition test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2FlagsState_0021, Function | MediumTest | Level1)
{
    int fd0[2];
    errno = 0;
    int ret = pipe2(fd0, 0);
    EXPECT_EQ(ret, 0);
    if (ret == 0) {
        int fl = fcntl(fd0[0], F_GETFL);
        int fd = fcntl(fd0[0], F_GETFD);
        EXPECT_EQ(fl & O_NONBLOCK, 0);
        EXPECT_EQ(fd & FD_CLOEXEC, 0);
        close(fd0[0]);
        close(fd0[1]);
    }

    int fd1[2];
    errno = 0;
    ret = pipe2(fd1, O_CLOEXEC);
    EXPECT_EQ(ret, 0);
    if (ret == 0) {
        int fd = fcntl(fd1[0], F_GETFD);
        int fl = fcntl(fd1[0], F_GETFL);
        EXPECT_TRUE((fd & FD_CLOEXEC) != 0);
        EXPECT_EQ(fl & O_NONBLOCK, 0);
        close(fd1[0]);
        close(fd1[1]);
    }

    int fd2[2];
    errno = 0;
    ret = pipe2(fd2, O_NONBLOCK);
    EXPECT_EQ(ret, 0);
    if (ret == 0) {
        int fl = fcntl(fd2[0], F_GETFL);
        int fd = fcntl(fd2[0], F_GETFD);
        EXPECT_TRUE((fl & O_NONBLOCK) != 0);
        EXPECT_EQ(fd & FD_CLOEXEC, 0);
        close(fd2[0]);
        close(fd2[1]);
    }

    int fd3[2];
    errno = 0;
    ret = pipe2(fd3, O_CLOEXEC | O_NONBLOCK);
    EXPECT_EQ(ret, 0);
    if (ret == 0) {
        int fd = fcntl(fd3[0], F_GETFD);
        int fl = fcntl(fd3[0], F_GETFL);
        EXPECT_TRUE((fd & FD_CLOEXEC) != 0);
        EXPECT_TRUE((fl & O_NONBLOCK) != 0);
        close(fd3[0]);
        close(fd3[1]);
    }
}

/* TestReportAnalysis 问题3.4.9: flags×时序干扰 不同线程使用不同flags并发调用 (P1) 建议: 3线程分别用O_CLOEXEC/O_NONBLOCK/0并发pipe2并验证fd状态 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_2200
 * @tc.name   : Pipe2FlagsTiming_0022
 * @tc.desc   : pipe2 flags timing test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2FlagsTiming_0022, Function | MediumTest | Level1)
{
    const int THREAD_COUNT_0022 = 3;
    pthread_t threads[THREAD_COUNT_0022];
    Pipe2ThreadArg args[THREAD_COUNT_0022];
    int flagValues[THREAD_COUNT_0022] = {O_CLOEXEC, O_NONBLOCK, 0};

    for (int i = 0; i < THREAD_COUNT_0022; i++) {
        args[i].flags = flagValues[i];
        args[i].ret = -1;
        args[i].pipeFd[0] = -1;
        args[i].pipeFd[1] = -1;
        pthread_create(&threads[i], nullptr, Pipe2ThreadFunc, &args[i]);
    }

    for (int i = 0; i < THREAD_COUNT_0022; i++) {
        pthread_join(threads[i], nullptr);
    }

    EXPECT_EQ(args[0].ret, 0);
    if (args[0].ret == 0) {
        int fd = fcntl(args[0].pipeFd[0], F_GETFD);
        EXPECT_TRUE((fd & FD_CLOEXEC) != 0);
        close(args[0].pipeFd[0]);
        close(args[0].pipeFd[1]);
    }

    EXPECT_EQ(args[1].ret, 0);
    if (args[1].ret == 0) {
        int fl = fcntl(args[1].pipeFd[0], F_GETFL);
        EXPECT_TRUE((fl & O_NONBLOCK) != 0);
        close(args[1].pipeFd[0]);
        close(args[1].pipeFd[1]);
    }

    EXPECT_EQ(args[2].ret, 0);
    if (args[2].ret == 0) {
        int fd = fcntl(args[2].pipeFd[0], F_GETFD);
        int fl = fcntl(args[2].pipeFd[0], F_GETFL);
        EXPECT_EQ(fd & FD_CLOEXEC, 0);
        EXPECT_EQ(fl & O_NONBLOCK, 0);
        close(args[2].pipeFd[0]);
        close(args[2].pipeFd[1]);
    }
}

/* TestReportAnalysis 问题#13: 管道生命周期完整测试 (P1) 建议: 创建→写入→读取→关闭写端读EOF→关闭读端→操作返回EBADF */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_2300
 * @tc.name   : Pipe2Lifecycle_0023
 * @tc.desc   : pipe2 fd lifecycle test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2Lifecycle_0023, Function | MediumTest | Level1)
{
    int pipeFd[2];
    errno = 0;
    int ret = pipe2(pipeFd, 0);
    EXPECT_EQ(ret, 0);

    errno = 0;
    ssize_t wret = write(pipeFd[1], "life", LIFECYCLE_DATA_LEN);
    EXPECT_EQ(wret, 4);

    char buf[LIFECYCLE_BUF_SIZE] = {0};
    errno = 0;
    ssize_t rret = read(pipeFd[0], buf, LIFECYCLE_DATA_LEN);
    EXPECT_EQ(rret, 4);
    EXPECT_EQ(memcmp(buf, "life", 4), 0);

    close(pipeFd[1]);
    errno = 0;
    rret = read(pipeFd[0], buf, sizeof(buf));
    EXPECT_EQ(rret, 0);

    close(pipeFd[0]);
    errno = 0;
    rret = read(pipeFd[0], buf, sizeof(buf));
    EXPECT_EQ(rret, -1);
    EXPECT_EQ(errno, EBADF);
}

/* TestReportAnalysis 问题#14: 关闭写端后读端EOF (P1) 建议: close写端后read返回0表示EOF */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_2400
 * @tc.name   : Pipe2ReadEof_0024
 * @tc.desc   : pipe2 read returns EOF when write end closed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2ReadEof_0024, Function | MediumTest | Level1)
{
    int pipeFd[2];
    errno = 0;
    int ret = pipe2(pipeFd, 0);
    EXPECT_EQ(ret, 0);

    errno = 0;
    ssize_t wret = write(pipeFd[1], "eof_test", EOF_TEST_DATA_LEN);
    EXPECT_EQ(wret, 8);

    close(pipeFd[1]);

    char buf[EOF_READ_BUF_SIZE] = {0};
    errno = 0;
    ssize_t rret = read(pipeFd[0], buf, sizeof(buf));
    EXPECT_EQ(rret, 8);

    errno = 0;
    rret = read(pipeFd[0], buf, sizeof(buf));
    EXPECT_EQ(rret, 0);

    close(pipeFd[0]);
}

/* TestReportAnalysis 问题#15: 关闭读端后写端EPIPE (P1) 建议: close读端后write返回-1且errno=EPIPE */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_2500
 * @tc.name   : Pipe2WriteEpipe_0025
 * @tc.desc   : pipe2 write returns EPIPE when read end closed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2WriteEpipe_0025, Function | MediumTest | Level1)
{
    int pipeFd[2];
    errno = 0;
    int ret = pipe2(pipeFd, 0);
    EXPECT_EQ(ret, 0);

    close(pipeFd[0]);
    signal(SIGPIPE, SIG_IGN);

    errno = 0;
    ssize_t wret = write(pipeFd[1], "test", EPIPE_DATA_LEN);
    EXPECT_EQ(wret, -1);
    EXPECT_EQ(errno, EPIPE);

    close(pipeFd[1]);
}

/* TestReportAnalysis 问题#16: fd资源泄漏修复 (P1) 建议: 1000次pipe2+close循环验证无fd泄漏 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_2600
 * @tc.name   : Pipe2FdLeak_0026
 * @tc.desc   : pipe2 fd leak verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2FdLeak_0026, Function | MediumTest | Level1)
{
    const int ITER_COUNT_0026 = 1000;
    int failCount = 0;
    for (int i = 0; i < ITER_COUNT_0026; i++) {
        int pipeFd[2];
        errno = 0;
        int ret = pipe2(pipeFd, 0);
        if (ret != 0) {
            failCount++;
            continue;
        }
        close(pipeFd[0]);
        close(pipeFd[1]);
    }
    EXPECT_EQ(failCount, 0);
}

/* TestReportAnalysis 问题#17: 多线程并发pipe2 (P1) 建议: 10线程并发创建管道验证fd有效且唯一 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_2700
 * @tc.name   : Pipe2Concurrent_0027
 * @tc.desc   : pipe2 concurrent access test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2Concurrent_0027, Function | MediumTest | Level1)
{
    const int THREAD_COUNT_0027 = 10;
    pthread_t threads[THREAD_COUNT_0027];
    Pipe2ThreadArg args[THREAD_COUNT_0027];

    for (int i = 0; i < THREAD_COUNT_0027; i++) {
        args[i].flags = 0;
        args[i].ret = -1;
        args[i].pipeFd[0] = -1;
        args[i].pipeFd[1] = -1;
        pthread_create(&threads[i], nullptr, Pipe2ThreadFunc, &args[i]);
    }

    for (int i = 0; i < THREAD_COUNT_0027; i++) {
        pthread_join(threads[i], nullptr);
    }

    int successCount = 0;
    for (int i = 0; i < THREAD_COUNT_0027; i++) {
        if (args[i].ret == 0) {
            successCount++;
            EXPECT_TRUE(args[i].pipeFd[0] > 0);
            EXPECT_TRUE(args[i].pipeFd[1] > 0);
            EXPECT_NE(args[i].pipeFd[0], args[i].pipeFd[1]);
            close(args[i].pipeFd[0]);
            close(args[i].pipeFd[1]);
        }
    }
    EXPECT_EQ(successCount, THREAD_COUNT_0027);
}

/* TestReportAnalysis 问题#18: 多进程管道通信 (P1) 建议: fork后父子进程通过管道通信验证数据传输 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_2800
 * @tc.name   : Pipe2ForkComm_0028
 * @tc.desc   : pipe2 fork communication test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2ForkComm_0028, Function | MediumTest | Level1)
{
    int pipeFd[2];
    errno = 0;
    int ret = pipe2(pipeFd, 0);
    EXPECT_EQ(ret, 0);

    pid_t pid = fork();
    ASSERT_TRUE(pid >= 0);

    if (pid == 0) {
        close(pipeFd[1]);
        char buf[READ_BUF_SIZE] = {0};
        ssize_t rret = read(pipeFd[0], buf, sizeof(buf) - 1);
        if (rret > 0) {
            write(STDOUT_FILENO, buf, rret);
        }
        close(pipeFd[0]);
        _exit(0);
    } else {
        close(pipeFd[0]);
        const char *msg = "fork_test";
        errno = 0;
        ssize_t wret = write(pipeFd[1], msg, FORK_MSG_LEN);
        EXPECT_EQ(wret, 9);
        close(pipeFd[1]);

        int status = 0;
        waitpid(pid, &status, 0);
        EXPECT_TRUE(WIFEXITED(status));
        EXPECT_EQ(WEXITSTATUS(status), 0);
    }
}
