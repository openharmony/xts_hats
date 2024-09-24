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
static const char* TEST_FILE = "/data/local/tmp/test_file.txt";

class HatsMlockTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMlockTest::SetUp()
{
}

void HatsMlockTest::TearDown()
{
}

void HatsMlockTest::SetUpTestCase()
{
}

void HatsMlockTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Mlock_0100
 * @tc.name MlockSuccess_0001
 * @tc.desc Mlock locks address successfully.
*/
HWTEST_F(HatsMlockTest, MlockSuccess_0001, Function | MediumTest | Level1)
{
    int size = 4096;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fd > -1);

    ftruncate(fd, size);

    void *addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_NE(addr, MAP_FAILED);

    int ret = mlock(addr, size);
    EXPECT_EQ(ret, 0);

    ret = munlock(addr, size);
    EXPECT_EQ(ret, 0);

    munmap(addr, size);
    close(fd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Mlock_0200
 * @tc.name MlockallSuccess_0002
 * @tc.desc mlockall sets flag MCL_CURRENT successfully.
*/
HWTEST_F(HatsMlockTest, MlockallSuccess_0002, Function | MediumTest | Level1)
{
    int ret = mlockall(MCL_CURRENT | MCL_FUTURE);
    EXPECT_EQ(ret, 0);

    ret = munlockall();
    EXPECT_EQ(ret, 0);
}
