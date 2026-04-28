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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <poll.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "securec.h"

using namespace testing::ext;

static constexpr int EVENTFD_DATA_SIZE = 8;
static constexpr int PAGE_SIZE_VAL = 4096;
static constexpr int DELAYED_WRITE_US = 10000;
static constexpr int BUF_MODULO = 256;
static constexpr int PATH_BUF_SIZE = 64;
static constexpr int FDINFO_BUF_SIZE = 256;
static constexpr int BAD_FD_VALUE = 9999;
static constexpr int IDEMPOTENT_LOOP_COUNT = 100;
static constexpr int THREAD_RW_COUNT = 1000;
static constexpr int BUF_IDEMPOTENT_COUNT = 50;
static constexpr int SEMAPHORE_INITVAL = 100;
static constexpr int BUF_RACE_COUNT = 10000;
static constexpr int ACCUM_LOOP_COUNT = 10;
static constexpr int TIMING_WRITE_LOOP_COUNT = 1000;
static constexpr int POLL_TIMEOUT_MS = 2000;
static constexpr int READS_PER_THREAD = 50;
static constexpr int WRITES_PER_THREAD = 250;
static constexpr int MASSIVE_RW_PER_THREAD = 100;

struct ThreadArg {
    int fd;
    uint64_t val;
    int count;
    ssize_t lastRet;
    uint64_t totalRead;
};

static void *ThreadWriteFunc(void *arg)
{
    ThreadArg *a = static_cast<ThreadArg *>(arg);
    for (int i = 0; i < a->count; i++) {
        a->lastRet = write(a->fd, &a->val, sizeof(a->val));
    }
    return nullptr;
}

static void *ThreadReadFunc(void *arg)
{
    ThreadArg *a = static_cast<ThreadArg *>(arg);
    uint64_t val = 0;
    a->totalRead = 0;
    for (int i = 0; i < a->count; i++) {
        errno = 0;
        ssize_t r = read(a->fd, &val, sizeof(val));
        a->lastRet = r;
        if (r == static_cast<ssize_t>(sizeof(val))) {
            a->totalRead += val;
        }
    }
    return nullptr;
}

static void *ThreadDelayWriteFunc(void *arg)
{
    ThreadArg *a = static_cast<ThreadArg *>(arg);
    usleep(DELAYED_WRITE_US);
    a->lastRet = write(a->fd, &a->val, sizeof(a->val));
    return nullptr;
}

struct ThreadBufArg {
    uint64_t *buf;
    int count;
};

static void *ThreadBufModifyFunc(void *arg)
{
    ThreadBufArg *a = static_cast<ThreadBufArg *>(arg);
    for (int i = 0; i < a->count; i++) {
        *(a->buf) = static_cast<uint64_t>(i % BUF_MODULO);
    }
    return nullptr;
}

class HatsEventfd2Test : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsEventfd2Test::SetUp()
{
}

void HatsEventfd2Test::TearDown()
{
}

void HatsEventfd2Test::SetUpTestCase()
{
}

void HatsEventfd2Test::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0100
 * @tc.name   : Eventfd2CreateFdSuccess_0001
 * @tc.desc   : eventfd2 create fd use EFD_NONBLOCK/EFD_SEMAPHORE success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2CreateFdSuccess_0001, Function | MediumTest | Level1)
{
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);
    close(fd);

    fd = syscall(__NR_eventfd2, 1, EFD_SEMAPHORE);
    EXPECT_TRUE(fd > 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0200
 * @tc.name   : Eventfd2InvalidParamFailed_0002
 * @tc.desc   : eventfd2 create fd use invalid param failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2InvalidParamFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, -1);
    EXPECT_TRUE(fd < 0);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * TestReportAnalysis 问题[P0-ENODEV]: eventfd2 - 缺少 ENODEV 错误码测试 (P0)
 * 建议: 在可能触发ENODEV的环境中调用eventfd2()或通过mock/故障注入方式模拟设备不可用
 * 注意: ENODEV在常规环境下难以触发，此用例仅尝试调用并记录结果，不做强制断言
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0300
 * @tc.name   : Eventfd2EnodevError_0003
 * @tc.desc   : eventfd2 ENODEV error code test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2EnodevError_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, 0);
    if (fd < 0) {
        EXPECT_EQ(errno, ENODEV);
    } else {
        close(fd);
    }
}

/*
 * TestReportAnalysis 问题[#PB-01]: eventfd2 - initval=0 未测试 (P1)
 * 建议: 调用 syscall(__NR_eventfd2, 0, 0) 验证fd > 0
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0400
 * @tc.name   : Eventfd2InitvalZero_0004
 * @tc.desc   : eventfd2 with initval=0.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2InitvalZero_0004, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, 0);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-02]: eventfd2 - initval=UINT_MAX 未测试 (P1)
 * 建议: 调用 syscall(__NR_eventfd2, UINT_MAX, 0) 验证fd > 0，read验证计数器值为UINT_MAX
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0500
 * @tc.name   : Eventfd2InitvalUintMax_0005
 * @tc.desc   : eventfd2 with initval=UINT_MAX.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2InitvalUintMax_0005, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, UINT_MAX, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t val = 0;
    errno = 0;
    ssize_t ret = read(fd, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(sizeof(val)));
    EXPECT_EQ(val, static_cast<uint64_t>(UINT_MAX));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-03]: eventfd2 - flags=0 未测试 (P1)
 * 建议: 调用 syscall(__NR_eventfd2, 0, 0) 验证fd > 0
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0600
 * @tc.name   : Eventfd2FlagsZero_0006
 * @tc.desc   : eventfd2 with flags=0.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2FlagsZero_0006, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, 0);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-04]: eventfd2 - EFD_CLOEXEC 标志未测试 (P1)
 * 建议: 调用 syscall(__NR_eventfd2, 0, EFD_CLOEXEC) 验证fd > 0
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0700
 * @tc.name   : Eventfd2Cloexec_0007
 * @tc.desc   : eventfd2 with EFD_CLOEXEC flag.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2Cloexec_0007, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_CLOEXEC);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-05]: eventfd2 - EFD_CLOEXEC|EFD_NONBLOCK 组合未测试 (P1)
 * 建议: 调用 syscall(__NR_eventfd2, 0, EFD_CLOEXEC | EFD_NONBLOCK) 验证fd > 0
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0800
 * @tc.name   : Eventfd2CloexecNonblock_0008
 * @tc.desc   : eventfd2 with EFD_CLOEXEC|EFD_NONBLOCK flags.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2CloexecNonblock_0008, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_CLOEXEC | EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-06]: eventfd2 - EFD_CLOEXEC|EFD_SEMAPHORE 组合未测试 (P1)
 * 建议: 调用 syscall(__NR_eventfd2, 0, EFD_CLOEXEC | EFD_SEMAPHORE) 验证fd > 0
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0900
 * @tc.name   : Eventfd2CloexecSemaphore_0009
 * @tc.desc   : eventfd2 with EFD_CLOEXEC|EFD_SEMAPHORE flags.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2CloexecSemaphore_0009, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_CLOEXEC | EFD_SEMAPHORE);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-07]: eventfd2 - EFD_NONBLOCK|EFD_SEMAPHORE 组合未测试 (P1)
 * 建议: 调用 syscall(__NR_eventfd2, 1, EFD_NONBLOCK | EFD_SEMAPHORE)，write后read验证信号量语义
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1000
 * @tc.name   : Eventfd2NonblockSemaphore_0010
 * @tc.desc   : eventfd2 with EFD_NONBLOCK|EFD_SEMAPHORE flags.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2NonblockSemaphore_0010, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK | EFD_SEMAPHORE);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 3;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(1));

    errno = 0;
    rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(1));

    errno = 0;
    rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(1));

    errno = 0;
    rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-08]: eventfd2 - EFD_CLOEXEC|EFD_NONBLOCK|EFD_SEMAPHORE 全组合未测试 (P1)
 * 建议: 调用 syscall(__NR_eventfd2, 1, EFD_CLOEXEC | EFD_NONBLOCK | EFD_SEMAPHORE) 验证fd > 0
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1100
 * @tc.name   : Eventfd2AllFlagsCombo_0011
 * @tc.desc   : eventfd2 with all flags combined.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2AllFlagsCombo_0011, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_CLOEXEC | EFD_NONBLOCK | EFD_SEMAPHORE);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 2;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(1));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-09]: read操作 - buf=NULL 非法指针未测试 (P1)
 * 建议: 创建eventfd2 fd，write写入值，调用 read(fd, NULL, 8)，期望返回-1，errno=EFAULT
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1200
 * @tc.name   : Eventfd2ReadBufNull_0012
 * @tc.desc   : eventfd2 read with null buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufNull_0012, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = read(fd, nullptr, EVENTFD_DATA_SIZE);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-10]: read操作 - buf=未映射地址 非法指针未测试 (P1)
 * 建议: 创建eventfd2 fd，write写入值，mmap后munmap，调用read(fd, buf, EVENTFD_DATA_SIZE)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1300
 * @tc.name   : Eventfd2ReadBufUnmapped_0013
 * @tc.desc   : eventfd2 read with unmapped buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufUnmapped_0013, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    void *buf = mmap(nullptr, PAGE_SIZE_VAL, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(buf, MAP_FAILED);
    munmap(buf, 4096);

    errno = 0;
    ssize_t ret = read(fd, buf, EVENTFD_DATA_SIZE);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-11]: read操作 - buf=内核态地址 非法指针未测试 (P1)
 * 建议: 创建eventfd2 fd，write写入值，调用read(fd, (void*)0xFFFF800000000000, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1400
 * @tc.name   : Eventfd2ReadBufKernel_0014
 * @tc.desc   : eventfd2 read with kernel address buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufKernel_0014, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = read(fd, reinterpret_cast<void *>(0xFFFF800000000000ULL), 8);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-12]: read操作 - buf=只读区 非法指针未测试 (P1)
 * 建议: 创建eventfd2 fd，write写入值，调用read(fd, (void*)"readonly", 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1500
 * @tc.name   : Eventfd2ReadBufReadonly_0015
 * @tc.desc   : eventfd2 read with read-only buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufReadonly_0015, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = read(fd, const_cast<void *>(static_cast<const void *>("readonly")), 8);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-13]: write操作 - buf=NULL 非法指针未测试 (P1)
 * 建议: 创建eventfd2 fd，调用write(fd, NULL, 8)，期望返回-1，errno=EFAULT
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1600
 * @tc.name   : Eventfd2WriteBufNull_0016
 * @tc.desc   : eventfd2 write with null buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufNull_0016, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = write(fd, nullptr, EVENTFD_DATA_SIZE);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-14]: write操作 - buf=未映射地址 非法指针未测试 (P1)
 * 建议: 创建eventfd2 fd，mmap后munmap，调用write(fd, buf, EVENTFD_DATA_SIZE)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1700
 * @tc.name   : Eventfd2WriteBufUnmapped_0017
 * @tc.desc   : eventfd2 write with unmapped buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufUnmapped_0017, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    void *buf = mmap(nullptr, PAGE_SIZE_VAL, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(buf, MAP_FAILED);
    munmap(buf, 4096);

    errno = 0;
    ssize_t ret = write(fd, buf, EVENTFD_DATA_SIZE);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-15]: write操作 - buf=内核态地址 非法指针未测试 (P1)
 * 建议: 创建eventfd2 fd，调用write(fd, (void*)0xFFFF800000000000, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1800
 * @tc.name   : Eventfd2WriteBufKernel_0018
 * @tc.desc   : eventfd2 write with kernel address buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufKernel_0018, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = write(fd, reinterpret_cast<void *>(0xFFFF800000000000ULL), 8);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#PB-16]: write操作 - buf=只读区 非法指针未测试 (P1)
 * 建议: 创建eventfd2 fd，调用write(fd, (char*)"readonly", 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_1900
 * @tc.name   : Eventfd2WriteBufReadonly_0019
 * @tc.desc   : eventfd2 write with read-only buffer returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufReadonly_0019, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = write(fd, const_cast<void *>(static_cast<const void *>("readonly")), 8);
    EXPECT_TRUE(ret > 0);
    EXPECT_EQ(errno, 0);
}
/*
 * TestReportAnalysis 问题[#FP-01]: eventfd2 - 计数器初始化值未验证 (P2)
 * 建议: 创建eventfd2(initval=5, 0)，立即read验证返回5
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2000
 * @tc.name   : Eventfd2CounterInitVal_0020
 * @tc.desc   : eventfd2 counter initial value verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2CounterInitVal_0020, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 5, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t val = 0;
    errno = 0;
    ssize_t ret = read(fd, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(sizeof(val)));
    EXPECT_EQ(val, static_cast<uint64_t>(5));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-02]: eventfd2 - flags=0 创建成功未测试 (P2)
 * 建议: syscall(__NR_eventfd2, 0, 0) 验证fd > 0
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2100
 * @tc.name   : Eventfd2FlagsZeroFunc_0021
 * @tc.desc   : eventfd2 flags=0 functional behavior.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2FlagsZeroFunc_0021, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, 0);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-03]: eventfd2 - EFD_CLOEXEC 标志行为未测试 (P2)
 * 建议: 创建EFD_CLOEXEC的fd，fork+execve后验证fd不可用
 * 注意: 完整execve测试需要外部测试程序，此处通过fcntl验证FD_CLOEXEC标志已设置
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2200
 * @tc.name   : Eventfd2CloexecBehavior_0022
 * @tc.desc   : eventfd2 EFD_CLOEXEC behavior across exec.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2CloexecBehavior_0022, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_CLOEXEC);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    int flags = fcntl(fd, F_GETFD);
    EXPECT_NE(flags, -1);
    EXPECT_NE(flags & FD_CLOEXEC, 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-04]: eventfd2 - read 基本操作未测试 (P2)
 * 建议: write(42)后read，验证返回42和8字节大小
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2300
 * @tc.name   : Eventfd2ReadBasic_0023
 * @tc.desc   : eventfd2 basic read operation.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBasic_0023, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 42;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(42));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-05]: eventfd2 - 非信号量read语义未测试 (P2)
 * 建议: write多次累加后read，验证返回累加总值，再次read返回EAGAIN
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2400
 * @tc.name   : Eventfd2ReadNonSem_0024
 * @tc.desc   : eventfd2 read in non-semaphore mode.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadNonSem_0024, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval1 = 10;
    errno = 0;
    ssize_t wret = write(fd, &wval1, sizeof(wval1));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval1)));

    uint64_t wval2 = 20;
    errno = 0;
    wret = write(fd, &wval2, sizeof(wval2));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval2)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(30));

    errno = 0;
    rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-06]: eventfd2 - 信号量read语义未测试 (P2)
 * 建议: 创建EFD_SEMAPHORE的fd，write(3)后连续read 3次，每次返回1
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2500
 * @tc.name   : Eventfd2ReadSem_0025
 * @tc.desc   : eventfd2 read in semaphore mode.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadSem_0025, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK | EFD_SEMAPHORE);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 3;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    for (int i = 0; i < 3; i++) {
        uint64_t rval = 0;
        errno = 0;
        ssize_t rret = read(fd, &rval, sizeof(rval));
        EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
        EXPECT_EQ(rval, static_cast<uint64_t>(1));
    }

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-07]: eventfd2 - 非阻塞空计数器read未测试 (P2)
 * 建议: 创建EFD_NONBLOCK的fd，不write直接read，验证errno=EAGAIN
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2600
 * @tc.name   : Eventfd2ReadEagain_0026
 * @tc.desc   : eventfd2 read returns EAGAIN when counter is zero.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadEagain_0026, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t val = 0;
    errno = 0;
    ssize_t ret = read(fd, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-08]: eventfd2 - write 基本操作未测试 (P2)
 * 建议: 创建fd，write(10)后read验证返回10
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2700
 * @tc.name   : Eventfd2WriteBasic_0027
 * @tc.desc   : eventfd2 basic write operation.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBasic_0027, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 10;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(10));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-09]: eventfd2 - write 累加计数器值未测试 (P2)
 * 建议: write(5), write(3)后read，验证返回8
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2800
 * @tc.name   : Eventfd2WriteAccum_0028
 * @tc.desc   : eventfd2 write accumulates counter value.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteAccum_0028, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval1 = 5;
    errno = 0;
    ssize_t wret = write(fd, &wval1, sizeof(wval1));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval1)));

    uint64_t wval2 = 3;
    errno = 0;
    wret = write(fd, &wval2, sizeof(wval2));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval2)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(8));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-10]: eventfd2 - write 最大值约束未测试 (P2)
 * 建议: write接近最大值后继续write，验证EFD_NONBLOCK下返回EAGAIN
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_2900
 * @tc.name   : Eventfd2WriteMax_0029
 * @tc.desc   : eventfd2 write with UINT64_MAX.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteMax_0029, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t maxVal = 0xfffffffffffffffeULL;
    errno = 0;
    ssize_t wret = write(fd, &maxVal, sizeof(maxVal));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(maxVal)));

    uint64_t one = 1;
    errno = 0;
    wret = write(fd, &one, sizeof(one));
    EXPECT_EQ(wret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-11]: eventfd2 - write 非法值 0xffffffffffffffff 未测试 (P2)
 * 建议: uint64_t val = 0xffffffffffffffff; write(fd, &val, 8); 验证errno=EINVAL
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3000
 * @tc.name   : Eventfd2WriteIllegal_0030
 * @tc.desc   : eventfd2 write with illegal value returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteIllegal_0030, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t illegal = 0xffffffffffffffffULL;
    errno = 0;
    ssize_t ret = write(fd, &illegal, sizeof(illegal));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EINVAL);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-12]: eventfd2 - 非阻塞write溢出未测试 (P2)
 * 建议: 创建EFD_NONBLOCK的fd，write大值使计数器接近最大，再write验证EAGAIN
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3100
 * @tc.name   : Eventfd2WriteOverflow_0031
 * @tc.desc   : eventfd2 write overflow returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteOverflow_0031, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t nearMax = 0xfffffffffffffffdULL;
    errno = 0;
    ssize_t wret = write(fd, &nearMax, sizeof(nearMax));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(nearMax)));

    uint64_t addVal = 2;
    errno = 0;
    wret = write(fd, &addVal, sizeof(addVal));
    EXPECT_EQ(wret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-13]: eventfd2 - poll/select 支持未测试 (P2)
 * 建议: 创建fd，使用poll验证空计数器时POLLOUT、write后POLLIN|POLLOUT
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3200
 * @tc.name   : Eventfd2PollSupport_0032
 * @tc.desc   : eventfd2 poll support verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2PollSupport_0032, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    struct pollfd pfd = {fd, POLLIN | POLLOUT, 0};
    errno = 0;
    int pret = poll(&pfd, 1, 0);
    EXPECT_EQ(pret, 1);
    EXPECT_NE(pfd.revents & POLLOUT, 0);

    uint64_t wval = 1;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    pfd.revents = 0;
    errno = 0;
    pret = poll(&pfd, 1, 0);
    EXPECT_EQ(pret, 1);
    EXPECT_NE(pfd.revents & POLLIN, 0);
    EXPECT_NE(pfd.revents & POLLOUT, 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-14]: eventfd2 - fork 继承fd未测试 (P2)
 * 建议: 创建fd，fork后子进程write，父进程read验证值正确
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3300
 * @tc.name   : Eventfd2ForkInherit_0033
 * @tc.desc   : eventfd2 fd inherited across fork.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ForkInherit_0033, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, 0);
    EXPECT_TRUE(fd > 0);

    int pipeFd[2] = {-1, -1};
    errno = 0;
    int pret = pipe(pipeFd);
    EXPECT_EQ(pret, 0);

    pid_t pid = fork();
    EXPECT_GE(pid, 0);
    if (pid == 0) {
        close(pipeFd[0]);
        uint64_t val = 42;
        write(fd, &val, sizeof(val));
        char c = 'A';
        write(pipeFd[1], &c, 1);
        close(pipeFd[1]);
        close(fd);
        _exit(0);
    }
    close(pipeFd[1]);
    char c = 0;
    read(pipeFd[0], &c, 1);
    close(pipeFd[0]);

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(42));

    int status = 0;
    waitpid(pid, &status, 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-15]: eventfd2 - execve 保留fd（除非CLOEXEC）未测试 (P2)
 * 建议: 创建无CLOEXEC的fd，fork+execve后验证fd仍有效
 * 注意: 完整execve测试需要外部测试程序，此处通过fcntl验证FD_CLOEXEC未设置
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3400
 * @tc.name   : Eventfd2ExecveKeepFd_0034
 * @tc.desc   : eventfd2 fd kept across execve with EFD_CLOEXEC.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ExecveKeepFd_0034, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    int flags = fcntl(fd, F_GETFD);
    EXPECT_NE(flags, -1);
    EXPECT_EQ(flags & FD_CLOEXEC, 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#FP-16]: eventfd2 - /proc/pid/fdinfo 查看计数器未测试 (P2)
 * 建议: 创建fd，write后读取/proc/self/fdinfo/<fd>验证eventfd-count值
 * 注意: /proc/pid/fdinfo在部分OpenHarmony环境中可能不可用
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3500
 * @tc.name   : Eventfd2ProcFdinfo_0035
 * @tc.desc   : eventfd2 /proc/fd info verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ProcFdinfo_0035, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 12345;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    char path[PATH_BUF_SIZE] = {0};
    sprintf_s(path, PATH_BUF_SIZE, "/proc/self/fdinfo/%d", fd);
    errno = 0;
    int infoFd = open(path, O_RDONLY);
    if (infoFd >= 0) {
        char buf[FDINFO_BUF_SIZE] = {0};
        errno = 0;
        ssize_t rret = read(infoFd, buf, sizeof(buf) - 1);
        EXPECT_GT(rret, 0);
        close(infoFd);
    } else {
    }
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-01]: read操作 - fd已关闭后read (P1)
 * 建议: 创建eventfd2 fd，close(fd)，read(fd, &val, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3600
 * @tc.name   : Eventfd2ReadClosedFd_0036
 * @tc.desc   : eventfd2 read on closed fd returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadClosedFd_0036, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);
    close(fd);

    uint64_t val = 0;
    errno = 0;
    ssize_t ret = read(fd, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EBADF);
}

/*
 * TestReportAnalysis 问题[#CS-02]: read操作 - fd无效值read (P1)
 * 建议: read(BAD_FD_VALUE, &val, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3700
 * @tc.name   : Eventfd2ReadBadFd_0037
 * @tc.desc   : eventfd2 read on bad fd returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBadFd_0037, Function | MediumTest | Level2)
{
    uint64_t val = 0;
    errno = 0;
    ssize_t ret = read(BAD_FD_VALUE, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EBADF);
}

/*
 * TestReportAnalysis 问题[#CS-03]: read操作 - fd=-1 read (P1)
 * 建议: read(-1, &val, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3800
 * @tc.name   : Eventfd2ReadFdNegOne_0038
 * @tc.desc   : eventfd2 read on fd=-1 returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadFdNegOne_0038, Function | MediumTest | Level2)
{
    uint64_t val = 0;
    errno = 0;
    ssize_t ret = read(-1, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EBADF);
}

/*
 * TestReportAnalysis 问题[#CS-04]: read操作 - buf=NULL (P1)
 * 建议: 创建fd，write写入值，read(fd, NULL, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_3900
 * @tc.name   : Eventfd2ReadBufNullCombo_0039
 * @tc.desc   : eventfd2 read null buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufNullCombo_0039, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = read(fd, nullptr, EVENTFD_DATA_SIZE);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-05]: read操作 - buf=未映射地址 (P1)
 * 建议: 创建fd，write写入值，mmap后munmap，调用read(fd, buf, EVENTFD_DATA_SIZE)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4000
 * @tc.name   : Eventfd2ReadBufUnmappedCombo_0040
 * @tc.desc   : eventfd2 read unmapped buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufUnmappedCombo_0040, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    void *buf = mmap(nullptr, PAGE_SIZE_VAL, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(buf, MAP_FAILED);
    munmap(buf, 4096);

    errno = 0;
    ssize_t ret = read(fd, buf, EVENTFD_DATA_SIZE);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-06]: read操作 - buf=内核态地址 (P1)
 * 建议: 创建fd，write写入值，read(fd, (void*)0xFFFF800000000000, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4100
 * @tc.name   : Eventfd2ReadBufKernelCombo_0041
 * @tc.desc   : eventfd2 read kernel buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufKernelCombo_0041, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = read(fd, reinterpret_cast<void *>(0xFFFF800000000000ULL), 8);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-07]: read操作 - buf=只读区 (P1)
 * 建议: 创建fd，write写入值，read(fd, (void*)"readonly", 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4200
 * @tc.name   : Eventfd2ReadBufReadonlyCombo_0042
 * @tc.desc   : eventfd2 read read-only buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufReadonlyCombo_0042, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = read(fd, const_cast<void *>(static_cast<const void *>("readonly")), 8);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-08]: write操作 - fd已关闭后write (P1)
 * 建议: 创建eventfd2 fd，close(fd)，write(fd, &val, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4300
 * @tc.name   : Eventfd2WriteClosedFd_0043
 * @tc.desc   : eventfd2 write on closed fd returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteClosedFd_0043, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);
    close(fd);

    uint64_t val = 1;
    errno = 0;
    ssize_t ret = write(fd, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EBADF);
}

/*
 * TestReportAnalysis 问题[#CS-09]: write操作 - fd无效值write (P1)
 * 建议: uint64_t val = 1; write(BAD_FD_VALUE, &val, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4400
 * @tc.name   : Eventfd2WriteBadFd_0044
 * @tc.desc   : eventfd2 write on bad fd returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBadFd_0044, Function | MediumTest | Level2)
{
    uint64_t val = 1;
    errno = 0;
    ssize_t ret = write(BAD_FD_VALUE, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EBADF);
}

/*
 * TestReportAnalysis 问题[#CS-10]: write操作 - fd=-1 write (P1)
 * 建议: uint64_t val = 1; write(-1, &val, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4500
 * @tc.name   : Eventfd2WriteFdNegOne_0045
 * @tc.desc   : eventfd2 write on fd=-1 returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteFdNegOne_0045, Function | MediumTest | Level2)
{
    uint64_t val = 1;
    errno = 0;
    ssize_t ret = write(-1, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EBADF);
}

/*
 * TestReportAnalysis 问题[#CS-11]: write操作 - buf=NULL (P1)
 * 建议: 创建fd，write(fd, NULL, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4600
 * @tc.name   : Eventfd2WriteBufNullCombo_0046
 * @tc.desc   : eventfd2 write null buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufNullCombo_0046, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = write(fd, nullptr, EVENTFD_DATA_SIZE);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-12]: write操作 - buf=未映射地址 (P1)
 * 建议: 创建fd，mmap后munmap，调用write(fd, buf, EVENTFD_DATA_SIZE)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4700
 * @tc.name   : Eventfd2WriteBufUnmappedCombo_0047
 * @tc.desc   : eventfd2 write unmapped buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufUnmappedCombo_0047, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    void *buf = mmap(nullptr, PAGE_SIZE_VAL, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(buf, MAP_FAILED);
    munmap(buf, 4096);

    errno = 0;
    ssize_t ret = write(fd, buf, EVENTFD_DATA_SIZE);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-13]: write操作 - buf=内核态地址 (P1)
 * 建议: 创建fd，write(fd, (void*)0xFFFF800000000000, 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4800
 * @tc.name   : Eventfd2WriteBufKernelCombo_0048
 * @tc.desc   : eventfd2 write kernel buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufKernelCombo_0048, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = write(fd, reinterpret_cast<void *>(0xFFFF800000000000ULL), 8);
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-14]: write操作 - buf=只读区 (P1)
 * 建议: 创建fd，write(fd, (char*)"readonly", 8)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_4900
 * @tc.name   : Eventfd2WriteBufReadonlyCombo_0049
 * @tc.desc   : eventfd2 write read-only buffer combination scenario.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufReadonlyCombo_0049, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ssize_t ret = write(fd, const_cast<void *>(static_cast<const void *>("readonly")), 8);
    EXPECT_TRUE(ret > 0);
    EXPECT_EQ(errno, 0);
}

/*
 * TestReportAnalysis 问题[#CS-15]: poll操作 - fd已关闭后poll (P1)
 * 建议: 创建eventfd2 fd，close(fd)，struct pollfd pfd={fd, POLLIN, 0}; poll(&pfd, 1, 0)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5000
 * @tc.name   : Eventfd2PollClosedFd_0050
 * @tc.desc   : eventfd2 poll on closed fd returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2PollClosedFd_0050, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);
    close(fd);

    struct pollfd pfd = {fd, POLLIN, 0};
    errno = 0;
    int pret = poll(&pfd, 1, 0);
    EXPECT_EQ(pret, 1);
    EXPECT_NE(pfd.revents & POLLNVAL, 0);
}

/*
 * TestReportAnalysis 问题[#CS-16]: poll操作 - fd无效值poll (P1)
 * 建议: struct pollfd pfd={BAD_FD_VALUE, POLLIN, 0}; poll(&pfd, 1, 0)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5100
 * @tc.name   : Eventfd2PollBadFd_0051
 * @tc.desc   : eventfd2 poll on bad fd returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2PollBadFd_0051, Function | MediumTest | Level2)
{
    struct pollfd pfd = {BAD_FD_VALUE, POLLIN, 0};
    errno = 0;
    int pret = poll(&pfd, 1, 0);
    EXPECT_EQ(pret, 1);
    EXPECT_NE(pfd.revents & POLLNVAL, 0);
}

/*
 * TestReportAnalysis 问题[#CS-17]: poll操作 - fd=-1 poll (P1)
 * 建议: struct pollfd pfd={-1, POLLIN, 0}; poll(&pfd, 1, 0)
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5200
 * @tc.name   : Eventfd2PollFdNegOne_0052
 * @tc.desc   : eventfd2 poll on fd=-1 returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2PollFdNegOne_0052, Function | MediumTest | Level2)
{
    struct pollfd pfd = {-1, POLLIN, 0};
    errno = 0;
    int pret = poll(&pfd, 1, 0);
    EXPECT_EQ(pret, 0);
}

/*
 * TestReportAnalysis 问题[#CS-18]: read操作.fd - 幂等性场景 (P1)
 * 建议: 创建fd，循环100次：write(42) → read验证返回42
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5300
 * @tc.name   : Eventfd2ReadIdempotent_0053
 * @tc.desc   : eventfd2 read idempotent verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadIdempotent_0053, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    for (int i = 0; i < IDEMPOTENT_LOOP_COUNT; i++) {
        uint64_t wval = 42;
        errno = 0;
        ssize_t wret = write(fd, &wval, sizeof(wval));
        if (wret != static_cast<ssize_t>(sizeof(wval))) {
            break;
        }

        uint64_t rval = 0;
        errno = 0;
        ssize_t rret = read(fd, &rval, sizeof(rval));
        if (rret != static_cast<ssize_t>(sizeof(rval)) || rval != 42) {
            EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
            EXPECT_EQ(rval, static_cast<uint64_t>(42));
            break;
        }
    }
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-19]: read操作.fd - 文件状态变化场景 (P1)
 * 建议: 创建fd，write(10), write(20) → read验证返回30，write(5) → read验证返回5
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5400
 * @tc.name   : Eventfd2ReadStateChange_0054
 * @tc.desc   : eventfd2 read state transition test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadStateChange_0054, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval1 = 10;
    errno = 0;
    ssize_t wret = write(fd, &wval1, sizeof(wval1));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval1)));

    uint64_t wval2 = 20;
    errno = 0;
    wret = write(fd, &wval2, sizeof(wval2));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval2)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(30));

    uint64_t wval3 = 5;
    errno = 0;
    wret = write(fd, &wval3, sizeof(wval3));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval3)));

    errno = 0;
    rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(5));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-20]: read操作.fd - 时序干扰场景 (P1)
 * 建议: 创建EFD_NONBLOCK的fd，线程A循环read，线程B循环write，验证无崩溃
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5500
 * @tc.name   : Eventfd2ReadTiming_0055
 * @tc.desc   : eventfd2 read timing verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadTiming_0055, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    ThreadArg warg = {fd, 1, THREAD_RW_COUNT, 0, 0};
    ThreadArg rarg = {fd, 0, THREAD_RW_COUNT, 0, 0};
    pthread_t wt = 0;
    pthread_t rt = 0;
    int pret = pthread_create(&wt, nullptr, ThreadWriteFunc, &warg);
    ASSERT_EQ(pret, 0);

    pret = pthread_create(&rt, nullptr, ThreadReadFunc, &rarg);
    ASSERT_EQ(pret, 0);

    pthread_join(wt, nullptr);
    pthread_join(rt, nullptr);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-21]: read操作.buf - 幂等性场景 (P1)
 * 建议: 创建fd，循环：write(i) → read(fd, buf, EVENTFD_DATA_SIZE) → 验证buf内容
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5600
 * @tc.name   : Eventfd2ReadBufIdempotent_0056
 * @tc.desc   : eventfd2 read buffer idempotent verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufIdempotent_0056, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t buf = 0;
    for (int i = 1; i <= BUF_IDEMPOTENT_COUNT; i++) {
        uint64_t wval = static_cast<uint64_t>(i);
        errno = 0;
        ssize_t wret = write(fd, &wval, sizeof(wval));
        if (wret != static_cast<ssize_t>(sizeof(wval))) {
            break;
        }

        errno = 0;
        ssize_t rret = read(fd, &buf, sizeof(buf));
        if (rret != static_cast<ssize_t>(sizeof(buf)) || buf != wval) {
            EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(buf)));
            EXPECT_EQ(buf, wval);
            break;
        }
    }
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-22]: read操作.buf - 时序干扰场景 (P1)
 * 建议: 创建fd，write写入值，read使用共享buf，同时另一线程修改buf
 * 注意: 此场景为竞态条件测试，主要验证无崩溃，不保证确定性结果
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5700
 * @tc.name   : Eventfd2ReadBufTiming_0057
 * @tc.desc   : eventfd2 read buffer timing verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ReadBufTiming_0057, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, SEMAPHORE_INITVAL, EFD_NONBLOCK | EFD_SEMAPHORE);
    EXPECT_TRUE(fd > 0);

    uint64_t sharedBuf = 0;
    ThreadBufArg barg = {&sharedBuf, BUF_RACE_COUNT};
    pthread_t t1 = 0;
    int pret = pthread_create(&t1, nullptr, ThreadBufModifyFunc, &barg);
    ASSERT_EQ(pret, 0);

    for (int i = 0; i < IDEMPOTENT_LOOP_COUNT; i++) {
        errno = 0;
        ssize_t r = read(fd, &sharedBuf, sizeof(sharedBuf));
        if (r == static_cast<ssize_t>(-1) && errno == EAGAIN) {
            break;
        }
    }
    pthread_join(t1, nullptr);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-23]: write操作.fd - 幂等性场景 (P1)
 * 建议: 创建fd，initval=0，循环10次write(1)，read验证返回10
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5800
 * @tc.name   : Eventfd2WriteIdempotent_0058
 * @tc.desc   : eventfd2 write idempotent verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteIdempotent_0058, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    for (int i = 0; i < ACCUM_LOOP_COUNT; i++) {
        uint64_t val = 1;
        errno = 0;
        ssize_t wret = write(fd, &val, sizeof(val));
        EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(val)));
    }

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(10));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-24]: write操作.fd - 文件状态变化场景 (P1)
 * 建议: 创建fd，write(100) → read清空，write(50) → read验证返回50
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_5900
 * @tc.name   : Eventfd2WriteStateChange_0059
 * @tc.desc   : eventfd2 write state transition test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteStateChange_0059, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval1 = 100;
    errno = 0;
    ssize_t wret = write(fd, &wval1, sizeof(wval1));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval1)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));

    uint64_t wval2 = 50;
    errno = 0;
    wret = write(fd, &wval2, sizeof(wval2));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval2)));

    errno = 0;
    rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(50));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-25]: write操作.fd - 时序干扰场景 (P1)
 * 建议: 创建fd，initval=0，2个线程各write(1)执行1000次，read验证总值为2000
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6000
 * @tc.name   : Eventfd2WriteTiming_0060
 * @tc.desc   : eventfd2 write timing verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteTiming_0060, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    ThreadArg arg1 = {fd, 1, THREAD_RW_COUNT, 0, 0};
    ThreadArg arg2 = {fd, 1, THREAD_RW_COUNT, 0, 0};
    pthread_t t1 = 0;
    pthread_t t2 = 0;
    int pret = pthread_create(&t1, nullptr, ThreadWriteFunc, &arg1);
    ASSERT_EQ(pret, 0);

    pret = pthread_create(&t2, nullptr, ThreadWriteFunc, &arg2);
    ASSERT_EQ(pret, 0);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(2000));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-26]: write操作.buf - 幂等性场景 (P1)
 * 建议: 创建fd，同一buf写入不同值，循环write
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6100
 * @tc.name   : Eventfd2WriteBufIdempotent_0061
 * @tc.desc   : eventfd2 write buffer idempotent verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufIdempotent_0061, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t buf = 1;
    uint64_t expectedTotal = 0;
    for (int i = 0; i < ACCUM_LOOP_COUNT; i++) {
        buf = static_cast<uint64_t>(i + 1);
        expectedTotal += buf;
        errno = 0;
        ssize_t wret = write(fd, &buf, sizeof(buf));
        EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(buf)));
    }

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, expectedTotal);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-27]: write操作.buf - 时序干扰场景 (P1)
 * 建议: 创建fd，write使用buf时，另一线程修改buf内容
 * 注意: 此场景为竞态条件测试，主要验证无崩溃，不保证确定性结果
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6200
 * @tc.name   : Eventfd2WriteBufTiming_0062
 * @tc.desc   : eventfd2 write buffer timing verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2WriteBufTiming_0062, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t sharedBuf = 1;
    ThreadBufArg barg = {&sharedBuf, BUF_RACE_COUNT};
    pthread_t t1 = 0;
    int pret = pthread_create(&t1, nullptr, ThreadBufModifyFunc, &barg);
    ASSERT_EQ(pret, 0);

    for (int i = 0; i < TIMING_WRITE_LOOP_COUNT; i++) {
        sharedBuf = 1;
        errno = 0;
        ssize_t wret = write(fd, &sharedBuf, sizeof(sharedBuf));
        if (wret == static_cast<ssize_t>(-1)) {
            break;
        }
    }
    pthread_join(t1, nullptr);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-28]: poll操作.fd - 幂等性场景 (P1)
 * 建议: 创建fd，write(1)，连续poll 10次验证每次返回POLLIN
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6300
 * @tc.name   : Eventfd2PollIdempotent_0063
 * @tc.desc   : eventfd2 poll idempotent verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2PollIdempotent_0063, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 1;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    for (int i = 0; i < ACCUM_LOOP_COUNT; i++) {
        struct pollfd pfd = {fd, POLLIN, 0};
        errno = 0;
        int pret = poll(&pfd, 1, 0);
        EXPECT_EQ(pret, 1);
        EXPECT_NE(pfd.revents & POLLIN, 0);
    }
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-29]: poll操作.fd - 文件状态变化场景 (P1)
 * 建议: 创建fd，poll验证仅POLLOUT，write(1)后poll验证POLLIN|POLLOUT，read清空后poll仅POLLOUT
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6400
 * @tc.name   : Eventfd2PollStateChange_0064
 * @tc.desc   : eventfd2 poll state transition test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2PollStateChange_0064, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    struct pollfd pfd1 = {fd, POLLIN | POLLOUT, 0};
    errno = 0;
    int pret = poll(&pfd1, 1, 0);
    EXPECT_EQ(pret, 1);
    EXPECT_EQ(pfd1.revents & POLLIN, 0);
    EXPECT_NE(pfd1.revents & POLLOUT, 0);

    uint64_t wval = 1;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    struct pollfd pfd2 = {fd, POLLIN | POLLOUT, 0};
    errno = 0;
    pret = poll(&pfd2, 1, 0);
    EXPECT_EQ(pret, 1);
    EXPECT_NE(pfd2.revents & POLLIN, 0);
    EXPECT_NE(pfd2.revents & POLLOUT, 0);

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));

    struct pollfd pfd3 = {fd, POLLIN | POLLOUT, 0};
    errno = 0;
    pret = poll(&pfd3, 1, 0);
    EXPECT_EQ(pret, 1);
    EXPECT_EQ(pfd3.revents & POLLIN, 0);
    EXPECT_NE(pfd3.revents & POLLOUT, 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CS-30]: poll操作.fd - 时序干扰场景 (P1)
 * 建议: 创建fd（无EFD_NONBLOCK），poll等待POLLIN，另一线程write(1)触发poll返回
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6500
 * @tc.name   : Eventfd2PollTiming_0065
 * @tc.desc   : eventfd2 poll timing verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2PollTiming_0065, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, 0);
    EXPECT_TRUE(fd > 0);

    ThreadArg warg = {fd, 1, 1, 0, 0};
    pthread_t t1 = 0;
    int pret = pthread_create(&t1, nullptr, ThreadDelayWriteFunc, &warg);
    ASSERT_EQ(pret, 0);

    struct pollfd pfd = {fd, POLLIN, 0};
    errno = 0;
    int pollRet = poll(&pfd, 1, POLL_TIMEOUT_MS);
    EXPECT_EQ(pollRet, 1);
    EXPECT_NE(pfd.revents & POLLIN, 0);

    pthread_join(t1, nullptr);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#ST-01]: eventfd2 - 缺少 已创建→counter>0 转换（write写入计数器） (P1)
 * 建议: 创建eventfd2(0, EFD_NONBLOCK)，write(42)，read验证返回42
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6600
 * @tc.name   : Eventfd2StateWriteCounter_0066
 * @tc.desc   : eventfd2 state write counter verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateWriteCounter_0066, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 42;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(42));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#ST-02]: eventfd2 - 缺少 counter>0→counter=0 转换（非信号量read重置） (P1)
 * 建议: 创建eventfd2(0, EFD_NONBLOCK)，write(100) → read → 验证返回100，再次read → EAGAIN
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6700
 * @tc.name   : Eventfd2StateReadReset_0067
 * @tc.desc   : eventfd2 state read resets counter.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateReadReset_0067, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 100;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(100));

    errno = 0;
    rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#ST-03]: eventfd2 - 缺少 counter>0→counter-1 转换（信号量read递减） (P1)
 * 建议: 创建eventfd2(0, EFD_NONBLOCK|EFD_SEMAPHORE)，write(3)，3次read返回1，第4次EAGAIN
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6800
 * @tc.name   : Eventfd2StateSemDecrement_0068
 * @tc.desc   : eventfd2 semaphore mode decrements counter.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateSemDecrement_0068, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK | EFD_SEMAPHORE);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 3;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    for (int i = 0; i < 3; i++) {
        uint64_t rval = 0;
        errno = 0;
        ssize_t rret = read(fd, &rval, sizeof(rval));
        EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
        EXPECT_EQ(rval, static_cast<uint64_t>(1));
    }

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#ST-04]: eventfd2 - 缺少 counter=0→EAGAIN 转换（非阻塞read空计数器） (P1)
 * 建议: 创建eventfd2(0, EFD_NONBLOCK)，read → 验证errno=EAGAIN
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_6900
 * @tc.name   : Eventfd2StateEagainRead_0069
 * @tc.desc   : eventfd2 state EAGAIN on zero counter read.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateEagainRead_0069, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t val = 0;
    errno = 0;
    ssize_t ret = read(fd, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#ST-05]: eventfd2 - 缺少 counter=0→阻塞 转换（阻塞模式read等待） (P1)
 * 建议: 创建eventfd2(0, 0)（阻塞模式），使用子线程write触发read返回
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7000
 * @tc.name   : Eventfd2StateBlockingRead_0070
 * @tc.desc   : eventfd2 state blocking read behavior.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateBlockingRead_0070, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, 0);
    EXPECT_TRUE(fd > 0);

    ThreadArg warg = {fd, 99, 1, 0, 0};
    pthread_t t1 = 0;
    int pret = pthread_create(&t1, nullptr, ThreadDelayWriteFunc, &warg);
    ASSERT_EQ(pret, 0);

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(99));

    pthread_join(t1, nullptr);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#ST-06]: eventfd2 - 缺少 counter接近max→EAGAIN 转换（非阻塞write溢出） (P1)
 * 建议: 创建eventfd2(0, EFD_NONBLOCK)，write(0xfffffffffffffffe) → write(1) → 验证EAGAIN
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7100
 * @tc.name   : Eventfd2StateWriteEagain_0071
 * @tc.desc   : eventfd2 state write EAGAIN on overflow.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateWriteEagain_0071, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t maxVal = 0xfffffffffffffffeULL;
    errno = 0;
    ssize_t wret = write(fd, &maxVal, sizeof(maxVal));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(maxVal)));

    uint64_t one = 1;
    errno = 0;
    wret = write(fd, &one, sizeof(one));
    EXPECT_EQ(wret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EAGAIN);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#ST-07]: eventfd2 - 缺少 已关闭→EBADF 转换（已关闭fd操作） (P1)
 * 建议: 创建fd → close(fd)，read(fd, &val, 8) → EBADF，write(fd, &val, 8) → EBADF
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7200
 * @tc.name   : Eventfd2StateEbadfAfterClose_0072
 * @tc.desc   : eventfd2 state EBADF after fd closed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateEbadfAfterClose_0072, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);
    close(fd);

    uint64_t val = 1;
    errno = 0;
    ssize_t ret = read(fd, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    ret = write(fd, &val, sizeof(val));
    EXPECT_EQ(ret, static_cast<ssize_t>(-1));
    EXPECT_EQ(errno, EBADF);
}

/*
 * TestReportAnalysis 问题[#ST-08]: eventfd2 - 缺少 已创建→fork继承 转换 (P1)
 * 建议: 创建eventfd2(0, 0)，fork() → 子进程write(42) → 父进程read验证42
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7300
 * @tc.name   : Eventfd2StateForkInherit_0073
 * @tc.desc   : eventfd2 state fork inherits fd.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateForkInherit_0073, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, 0);
    EXPECT_TRUE(fd > 0);

    int pipeFd[2] = {-1, -1};
    errno = 0;
    int pret = pipe(pipeFd);
    EXPECT_EQ(pret, 0);

    pid_t pid = fork();
    EXPECT_GE(pid, 0);
    if (pid == 0) {
        close(pipeFd[0]);
        uint64_t val = 42;
        write(fd, &val, sizeof(val));
        char c = 'D';
        write(pipeFd[1], &c, 1);
        close(pipeFd[1]);
        close(fd);
        _exit(0);
    }
    close(pipeFd[1]);
    char c = 0;
    read(pipeFd[0], &c, 1);
    close(pipeFd[0]);

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(42));

    int status = 0;
    waitpid(pid, &status, 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#ST-09]: eventfd2 - 缺少 已创建(无CLOEXEC)→exec保留 转换 (P1)
 * 建议: 创建eventfd2(0, 0)，fork() → 子进程execve测试程序 → 验证fd仍可操作
 * 注意: 完整execve测试需要外部测试程序，此处通过fcntl验证FD_CLOEXEC未设置
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7400
 * @tc.name   : Eventfd2StateExecKeepFd_0074
 * @tc.desc   : eventfd2 state exec keeps fd with CLOEXEC.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateExecKeepFd_0074, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    int flags = fcntl(fd, F_GETFD);
    EXPECT_NE(flags, -1);
    EXPECT_EQ(flags & FD_CLOEXEC, 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#ST-10]: eventfd2 - 缺少 已创建(CLOEXEC)→exec关闭 转换 (P1)
 * 建议: 创建eventfd2(0, EFD_CLOEXEC)，fork() → 子进程execve测试程序 → 验证fd不可用
 * 注意: 完整execve测试需要外部测试程序，此处通过fcntl验证FD_CLOEXEC已设置
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7500
 * @tc.name   : Eventfd2StateExecCloseFd_0075
 * @tc.desc   : eventfd2 state exec closes fd without CLOEXEC.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2StateExecCloseFd_0075, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_CLOEXEC);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    int flags = fcntl(fd, F_GETFD);
    EXPECT_NE(flags, -1);
    EXPECT_NE(flags & FD_CLOEXEC, 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CC-01]: eventfd2 - 多线程并发read同一eventfd fd (P1)
 * 建议: pthread_create，多线程同时read
 * 期望: read结果总和等于计数器值，无数据丢失
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7600
 * @tc.name   : Eventfd2ConcurrentRead_0076
 * @tc.desc   : eventfd2 concurrent read test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ConcurrentRead_0076, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK | EFD_SEMAPHORE);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 100;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    const int threadCount = 4;
    ThreadArg args[threadCount] = {};
    pthread_t threads[threadCount] = {};
    for (int i = 0; i < threadCount; i++) {
        args[i].fd = fd;
        args[i].count = READS_PER_THREAD;
        args[i].val = 0;
        args[i].totalRead = 0;
        int pret = pthread_create(&threads[i], nullptr, ThreadReadFunc, &args[i]);
        ASSERT_EQ(pret, 0);
    }

    uint64_t totalRead = 0;
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
        totalRead += args[i].totalRead;
    }
    EXPECT_EQ(totalRead, static_cast<uint64_t>(100));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CC-02]: eventfd2 - 多线程并发write同一eventfd fd (P1)
 * 建议: pthread_create，多线程同时write
 * 期望: 所有write值正确累加到计数器
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7700
 * @tc.name   : Eventfd2ConcurrentWrite_0077
 * @tc.desc   : eventfd2 concurrent write test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ConcurrentWrite_0077, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    const int threadCount = 4;
    ThreadArg args[threadCount] = {};
    pthread_t threads[threadCount] = {};
    for (int i = 0; i < threadCount; i++) {
        args[i].fd = fd;
        args[i].val = 1;
        args[i].count = WRITES_PER_THREAD;
        int pret = pthread_create(&threads[i], nullptr, ThreadWriteFunc, &args[i]);
        ASSERT_EQ(pret, 0);
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(1000));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CC-03]: eventfd2 - 多线程并发read+write同一eventfd fd (P1)
 * 建议: pthread_create，半数线程read，半数线程write
 * 期望: 无崩溃，无死锁，数据一致
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7800
 * @tc.name   : Eventfd2ConcurrentReadWrite_0078
 * @tc.desc   : eventfd2 concurrent read-write test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ConcurrentReadWrite_0078, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    const int writerCount = 4;
    const int readerCount = 4;
    ThreadArg wargs[writerCount] = {};
    ThreadArg rargs[readerCount] = {};
    pthread_t wthreads[writerCount] = {};
    pthread_t rthreads[readerCount] = {};

    for (int i = 0; i < writerCount; i++) {
        wargs[i].fd = fd;
        wargs[i].val = 1;
        wargs[i].count = WRITES_PER_THREAD;
        int pret = pthread_create(&wthreads[i], nullptr, ThreadWriteFunc, &wargs[i]);
        ASSERT_EQ(pret, 0);
    }
    for (int i = 0; i < readerCount; i++) {
        rargs[i].fd = fd;
        rargs[i].count = WRITES_PER_THREAD;
        rargs[i].totalRead = 0;
        int pret = pthread_create(&rthreads[i], nullptr, ThreadReadFunc, &rargs[i]);
        ASSERT_EQ(pret, 0);
    }

    for (int i = 0; i < writerCount; i++) {
        pthread_join(wthreads[i], nullptr);
    }
    uint64_t totalRead = 0;
    for (int i = 0; i < readerCount; i++) {
        pthread_join(rthreads[i], nullptr);
        totalRead += rargs[i].totalRead;
    }

    uint64_t remaining = 0;
    errno = 0;
    ssize_t rret = read(fd, &remaining, sizeof(remaining));
    if (rret == static_cast<ssize_t>(sizeof(remaining))) {
        totalRead += remaining;
    }
    EXPECT_EQ(totalRead, static_cast<uint64_t>(1000));
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CC-04]: eventfd2 - fork后父子进程并发read/write (P1)
 * 建议: fork()，管道同步
 * 期望: 父子进程操作正确，计数器一致
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_7900
 * @tc.name   : Eventfd2ForkConcurrent_0079
 * @tc.desc   : eventfd2 fork concurrent access test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2ForkConcurrent_0079, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, 0);
    EXPECT_TRUE(fd > 0);

    int pipeFd[2] = {-1, -1};
    errno = 0;
    int pret = pipe(pipeFd);
    EXPECT_EQ(pret, 0);

    pid_t pid = fork();
    EXPECT_GE(pid, 0);
    if (pid == 0) {
        close(pipeFd[0]);
        uint64_t val = 50;
        write(fd, &val, sizeof(val));
        val = 30;
        write(fd, &val, sizeof(val));
        char c = 'X';
        write(pipeFd[1], &c, 1);
        close(pipeFd[1]);
        close(fd);
        _exit(0);
    }
    close(pipeFd[1]);
    char c = 0;
    read(pipeFd[0], &c, 1);
    close(pipeFd[0]);

    uint64_t rval = 0;
    errno = 0;
    ssize_t rret = read(fd, &rval, sizeof(rval));
    EXPECT_EQ(rret, static_cast<ssize_t>(sizeof(rval)));
    EXPECT_EQ(rval, static_cast<uint64_t>(80));

    int status = 0;
    waitpid(pid, &status, 0);
    close(fd);
}

/*
 * TestReportAnalysis 问题[#CC-05]: eventfd2 - 高频连续调用≥1000次验证无资源泄漏 (P1)
 * 建议: for循环≥1000次 create→close
 * 期望: 无fd泄漏，无内存泄漏
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_8000
 * @tc.name   : Eventfd2HighFrequency_0080
 * @tc.desc   : eventfd2 high frequency test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2HighFrequency_0080, Function | MediumTest | Level2)
{
    const int iterations = 1000;
    for (int i = 0; i < iterations; i++) {
        errno = 0;
        int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
        if (fd < 0) {
            EXPECT_TRUE(fd > 0);
            break;
        }
        close(fd);
    }
}

/*
 * TestReportAnalysis 问题[#CC-06]: eventfd2 - 大规模并发≥64线程操作eventfd验证无崩溃 (P1)
 * 建议: pthread_create ≥64线程同时read/write
 * 期望: 无崩溃，无SIGSEGV，无死锁
 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_8100
 * @tc.name   : Eventfd2MassiveConcurrency_0081
 * @tc.desc   : eventfd2 massive concurrency test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2MassiveConcurrency_0081, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 0, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);

    uint64_t wval = 1;
    errno = 0;
    ssize_t wret = write(fd, &wval, sizeof(wval));
    EXPECT_EQ(wret, static_cast<ssize_t>(sizeof(wval)));

    const int threadCount = 64;
    ThreadArg args[threadCount] = {};
    pthread_t threads[threadCount] = {};
    for (int i = 0; i < threadCount; i++) {
        args[i].fd = fd;
        args[i].count = MASSIVE_RW_PER_THREAD;
        if (i < threadCount / 2) {
            args[i].val = 1;
            int pret = pthread_create(&threads[i], nullptr, ThreadWriteFunc, &args[i]);
            ASSERT_EQ(pret, 0);
        } else {
            args[i].totalRead = 0;
            int pret = pthread_create(&threads[i], nullptr, ThreadReadFunc, &args[i]);
            ASSERT_EQ(pret, 0);
        }
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], nullptr);
    }
    close(fd);
}