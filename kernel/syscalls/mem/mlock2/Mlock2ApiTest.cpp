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

class HatsMlock2Test : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMlock2Test::SetUp()
{
}

void HatsMlock2Test::TearDown()
{
}

void HatsMlock2Test::SetUpTestCase()
{
}

void HatsMlock2Test::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Mlock2_0100
 * @tc.name Mlock2Success_0001
 * @tc.desc Mlock2 locks pages that are currently resident successfully.
*/
HWTEST_F(HatsMlock2Test, Mlock2Success_0001, Function | MediumTest | Level1)
{
    int ret;

    const size_t size = 1024;
    void *addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(addr, MAP_FAILED);

    ret = mlock2(addr, size, MLOCK_ONFAULT);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(munmap(addr, size),  -1);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Mlock2_0200
 * @tc.name Mlock2Success_0002
 * @tc.desc When flags is 0, mlock2() behaves exactly the same as mlock().
*/
HWTEST_F(HatsMlock2Test, Mlock2Success_0002, Function | MediumTest | Level1)
{
    int ret;

    const size_t size = 2048;
    void *addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(addr, MAP_FAILED);

    // when flag is 0, the same as mlock.
    ret = mlock2(addr, size, 0);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(munmap(addr, size),  -1);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Mlock2_0300
 * @tc.name Mlock2Success_0003
 * @tc.desc When flag is invalid, mlock2() locks failed, errno is set to EINVAL.
*/
HWTEST_F(HatsMlock2Test, Mlock2Success_0003, Function | MediumTest | Level2)
{
    int ret;

    const size_t size = 2048;
    void *addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(addr, MAP_FAILED);

    errno = 0;

    // flag 2 is invalid
    ret = mlock2(addr, size, 2);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    EXPECT_NE(munmap(addr, size),  -1);
}