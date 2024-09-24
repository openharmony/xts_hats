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
 * @tc.number SUB_KERNEL_SYSCALL_MmapSyscall_0100
 * @tc.name MmapSyscallReadSuccess_0001
 * @tc.desc Mmap sets prot PROT_READ successfully.
*/
HWTEST_F(HatsMmapSyscallTest, MmapSyscallReadSuccess_0001, Function | MediumTest | Level1)
{
    size_t size = 2048;
    void *va = mmap(nullptr, size, PROT_READ, MAP_ANONYMOUS  | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MmapSyscall_0200
 * @tc.name MmapSyscallWriteSuccess_0002
 * @tc.desc Mmap sets prot PROT_WRITE successfully.
*/
HWTEST_F(HatsMmapSyscallTest, MmapSyscallWriteSuccess_0002, Function | MediumTest | Level1)
{
    size_t size = 1024;
    void *va = mmap(nullptr, size, PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MmapSyscall_0300
 * @tc.name MmapSyscallMlockedSuccess_0003
 * @tc.desc Mmap sets flag MAP_LOCKED、MAP_ANONYMOUS、MAP_PRIVATE successfully.
*/
HWTEST_F(HatsMmapSyscallTest, MmapSyscallMlockedSuccess_0003, Function | MediumTest | Level1)
{
    size_t size = 1024;
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_LOCKED | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MmapSyscall_0400
 * @tc.name MmapSyscallNoReserveSuccess_0004
 * @tc.desc Mmap sets flag MAP_NORESERVE、MAP_ANONYMOUS、MAP_PRIVATE successfully.
*/
HWTEST_F(HatsMmapSyscallTest, MmapSyscallNoReserveSuccess_0004, Function | MediumTest | Level1)
{
    size_t size = 1024;
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_NORESERVE | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MmapSyscall_0500
 * @tc.name MmapSyscallPrivateSuccess_0005
 * @tc.desc Mmap sets flag MAP_ANONYMOUS and MAP_PRIVATE successfully.
*/
HWTEST_F(HatsMmapSyscallTest, MmapSyscallPrivateSuccess_0005, Function | MediumTest | Level1)
{
    size_t size = 1024;
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(va, MAP_FAILED);

    int ret = munmap(va, size);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MmapSyscall_0600
 * @tc.name MmapSyscallFailed_0006
 * @tc.desc Mmap map addr failed for size 0.
*/
HWTEST_F(HatsMmapSyscallTest, MmapSyscallFailed_0006, Function | MediumTest | Level2)
{
    size_t size = 0;
    void *va = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_EQ(va, MAP_FAILED);

    errno = 0;
    int ret = munmap(va, size);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MmapSyscall_0700
 * @tc.name MremapSyscallToLargeSuccess_0007
 * @tc.desc Mremap maps with flag 0 from small memory to large successfully.
*/
HWTEST_F(HatsMmapSyscallTest, MremapSyscallToLargeSuccess_0007, Function | MediumTest | Level2)
{
    size_t size = 1024;
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
 * @tc.number SUB_KERNEL_SYSCALL_MmapSyscall_0800
 * @tc.name MremapSyscallToSmallSuccess_0008
 * @tc.desc Mremap maps with flag 0 from large memory to small successfully.
*/
HWTEST_F(HatsMmapSyscallTest, MremapSyscallToSmallSuccess_0008, Function | MediumTest | Level2)
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
 * @tc.number SUB_KERNEL_SYSCALL_MmapSyscall_0900
 * @tc.name MremapSyscallMoveSuccess_0009
 * @tc.desc Mremap maps with flag MREMAP_MAYMOVE from small memory  to large successfully.
*/
HWTEST_F(HatsMmapSyscallTest, MremapSyscallMoveSuccess_0009, Function | MediumTest | Level2)
{
    size_t size = 1024;
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