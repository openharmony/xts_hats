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

class HatsMadvisecallTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMadvisecallTest::SetUp()
{
}

void HatsMadvisecallTest::TearDown()
{
}

void HatsMadvisecallTest::SetUpTestCase()
{
}

void HatsMadvisecallTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Madvisecall_0100
 * @tc.name MadvisecallForkSuccess_0001
 * @tc.desc Madvise sets advise MADV_DOFORK successfully.
*/
HWTEST_F(HatsMadvisecallTest, MadvisecallForkSuccess_0001, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 1024;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_DOFORK);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Madvisecall_0200
 * @tc.name MadvisecallNormalSuccess_0002
 * @tc.desc Madvise sets advise MADV_NORMAL successfully.
*/
HWTEST_F(HatsMadvisecallTest, MadvisecallNormalSuccess_0002, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 2048;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_NORMAL);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Madvisecall_0300
 * @tc.name MadvisecallWillneedSuccess_0003
 * @tc.desc Madvise sets advise MADV_WILLNEED successfully.
*/
HWTEST_F(HatsMadvisecallTest, MadvisecallWillneedSuccess_0003, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 4096;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_WILLNEED);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}