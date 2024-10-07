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

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/mman.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *SHMEM_NAME = "shmem_test";

class MenFdCreateApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MenFdCreateApiTest::SetUp()
{
}
void MenFdCreateApiTest::TearDown()
{
}
void MenFdCreateApiTest::SetUpTestCase()
{
}
void MenFdCreateApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MEMFDCREATE_0100
 * @tc.name   : MemFdCreateSuccess_0001
 * @tc.desc   : memfd_create create fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MenFdCreateApiTest, MenFdCreateSuccess_0001, Function | MediumTest | Level1)
{
    int fd = memfd_create(SHMEM_NAME, 0);
    EXPECT_TRUE(fd > 0);
    close(fd);

    fd = memfd_create(SHMEM_NAME, MFD_ALLOW_SEALING);
    EXPECT_TRUE(fd > 0);
    close(fd);

    fd = memfd_create(SHMEM_NAME, MFD_CLOEXEC);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MEMFDCREATE_0200
 * @tc.name   : MemFdCreateNullptrPathFailed_0002
 * @tc.desc   : memfd_create with nullptr Failed return EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MenFdCreateApiTest, MemFdCreateNullptrPathFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = memfd_create(nullptr, 0);
    EXPECT_EQ(fd, -1);
    EXPECT_EQ(errno, EFAULT);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MEMFDCREATE_0300
 * @tc.name   : MemFdCreateInvalidFlagsFailed_0003
 * @tc.desc   : memfd_create with invalid flags Failed return EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MenFdCreateApiTest, MemFdCreateInvalidFlagsFailed_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = memfd_create(SHMEM_NAME, -1);
    EXPECT_EQ(fd, -1);
    EXPECT_EQ(errno, EINVAL);
    close(fd);
}