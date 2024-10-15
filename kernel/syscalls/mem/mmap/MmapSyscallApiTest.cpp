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
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

static const int SIZE_1M = 1024 * 1024;
static const int SIZE_1G = 1024 * 1024 * 1024;
static const char *TEST_FILE = "/data/local/tmp/mmap_test";

class HatsMmapSyscallTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMmapSyscallTest::SetUp()
{
}

void HatsMmapSyscallTest::TearDown()
{
}

void HatsMmapSyscallTest::SetUpTestCase()
{
}

void HatsMmapSyscallTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MMAPSYSCALL_0100
 * @tc.name   : MmapSyscallPortTestSuccess_0001
 * @tc.desc   : Mmap sets prot PROT_READ/PROT_WRITE/PROT_EXEC/PROT_NONE test successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMmapSyscallTest, MmapSyscallPortTestSuccess_0001, Function | MediumTest | Level1)
{
    size_t size = SIZE_1M;
    void *va = mmap(nullptr, size, PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    va = mmap(nullptr, size, PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    va = mmap(nullptr, size, PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    va = mmap(nullptr, size, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MMAPSYSCALL_0200
 * @tc.name   : MmapFlagTestSuccess_0002
 * @tc.desc   : Mmap sets flag MAP_SHARED/MAP_PRIVATE/MAP_FIXED/MAP_ANONYMOUS test successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMmapSyscallTest, MmapFlagTestSuccess_0002, Function | MediumTest | Level1)
{
    size_t size = SIZE_1M;
    int fd = open(TEST_FILE, O_CREAT | O_RDWR, 0664);

    // flag MAP_SHARED and MAP_PRIVATE test
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0);
    EXPECT_NE(va, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
    close(fd);

    // flag MAP_FIXED and MAP_ANONYMOUS test
    void *addr = reinterpret_cast<void *>(0x200000);
    va = mmap(addr, size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MMAPSYSCALL_0300
 * @tc.name   : MmapFlagTestSuccess_0003
 * @tc.desc   : Mmap sets flag MAP_LOCKED/MAP_NORESERVE/MAP_POPULATE/MAP_STACK test successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMmapSyscallTest, MmapFlagTestSuccess_0003, Function | MediumTest | Level1)
{
    size_t size = SIZE_1M;

    // flag MAP_LOCKED test
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_LOCKED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    // flag MAP_NORESERVE test
    va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_NORESERVE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    // flag MAP_POPULATE test
    va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    // flag MAP_STACK test
    va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_STACK | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MMAPSYSCALL_0400
 * @tc.name   : MmapFlagTestSuccess_0004
 * @tc.desc   : Mmap sets flag MAP_FILE/MAP_HUGE_2MB/MAP_HUGE_1GB/MAP_HUGETLB successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMmapSyscallTest, MmapFlagTestSuccess_0004, Function | MediumTest | Level1)
{
    size_t size = SIZE_1M;
    int fd = open("/data/local/tmp/test_file", O_CREAT | O_RDWR, 0664);
    ASSERT_TRUE(fd > 0);

    // flag MAP_FILE test
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
    EXPECT_NE(va, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
    close(fd);

    // flag MAP_HUGE_2MB test
    size = SIZE_1M * 2;
    va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_HUGE_2MB | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    // flag MAP_HUGE_1GB test
    size = SIZE_1G;
    va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_HUGE_1GB | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MMAPSYSCALL_0500
 * @tc.name   : MmapAndMunmapInvalidSizeFailed_0005
 * @tc.desc   : Mmap map addr failed for size 0.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsMmapSyscallTest, MmapAndMunmapInvalidSizeFailed_0005, Function | MediumTest | Level2)
{
    size_t size = 0;
    errno = 0;
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_EQ(va, MAP_FAILED);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    int ret = munmap(va, size);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MMAPSYSCALL_0600
 * @tc.name   : MremapSyscallToLargeSuccess_0006
 * @tc.desc   : Mremap maps with flag 0 from small memory to large successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsMmapSyscallTest, MremapSyscallToLargeSuccess_0006, Function | MediumTest | Level2)
{
    size_t size = SIZE_1M;
    size_t newSize  = 2048;
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    void* vaNew = mremap(va, size, newSize, 0);
    EXPECT_NE(vaNew, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    ret = munmap(vaNew, newSize);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MMAPSYSCALL_0700
 * @tc.name   : MremapSyscallToSmallSuccess_0007
 * @tc.desc   : Mremap maps with flag 0 from large memory to small successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsMmapSyscallTest, MremapSyscallToSmallSuccess_0007, Function | MediumTest | Level2)
{
    size_t size = 4096;
    size_t newSize  = 2048;
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    void* vaNew = mremap(va, size, newSize, 0);
    EXPECT_NE(vaNew, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    ret = munmap(vaNew, newSize);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MMAPSYSCALL_0800
 * @tc.name   : MremapSyscallMoveSuccess_0008
 * @tc.desc   : Mremap maps with flag MREMAP_MAYMOVE from small memory to large successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsMmapSyscallTest, MremapSyscallMoveSuccess_0008, Function | MediumTest | Level2)
{
    size_t size = SIZE_1M;
    size_t newSize  = 2048;
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    void* vaNew = mremap(va, size, newSize, MREMAP_MAYMOVE);
    EXPECT_NE(vaNew, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);

    ret = munmap(vaNew, newSize);
    EXPECT_EQ(ret, 0);
}
