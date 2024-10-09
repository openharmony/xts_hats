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
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/mman.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *FILE_NAME = "shmem_file";

class DupApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void DupApiTest::SetUp()
{
}
void DupApiTest::TearDown()
{
}
void DupApiTest::SetUpTestCase()
{
}
void DupApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_DUP_0100
 * @tc.name   : DupFileFdSuccess_0001
 * @tc.desc   : dup file fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(DupApiTest, DupFileFdSuccess_0001, Function | MediumTest | Level1)
{
    int fd = -1;
    int newFd = -1;

    fd = memfd_create(FILE_NAME, 0);
    newFd = dup(fd);
    EXPECT_TRUE(newFd >= 0);

    close(newFd);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_DUP_0200
 * @tc.name   : DupInvalidFdFailed_0002
 * @tc.desc   : dup fd -1 failed, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(DupApiTest, DupInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    int fd = -1;
    int newFd = -1;
    errno = 0;
    newFd = dup(fd);
    EXPECT_EQ(newFd, -1);
    EXPECT_EQ(errno, EBADF);
}