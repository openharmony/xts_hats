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
static const char *NEW_FILE_NAME = "new_shmem_file";

class Dup3ApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void Dup3ApiTest::SetUp()
{
}
void Dup3ApiTest::TearDown()
{
}
void Dup3ApiTest::SetUpTestCase()
{
}
void Dup3ApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_DUP3_0100
 * @tc.name   : Dup3FileFdSuccess_0001
 * @tc.desc   : dup3 file fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(Dup3ApiTest, Dup3FileFdSuccess_0001, Function | MediumTest | Level1)
{
    int fd = -1;
    int newFd = -1;
    int oldFd = -1;
    oldFd = memfd_create(FILE_NAME, 0);
    newFd = memfd_create(NEW_FILE_NAME, 0);
    EXPECT_NE(oldFd, -1);
    EXPECT_NE(newFd, -1);

    fd = dup3(oldFd, newFd, 0);
    EXPECT_TRUE(fd >= 0);
    EXPECT_EQ(fd, newFd);
    EXPECT_NE(newFd, oldFd);
    EXPECT_TRUE(oldFd >= 0);

    close(fd);
    close(oldFd);
    close(newFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_DUP3_0200
 * @tc.name   : Dup3SetFlagSuccess_0002
 * @tc.desc   : dup3 force set the close-on-exec flag to the new fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(Dup3ApiTest, Dup3SetFlagSuccess_0002, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    int newFd = -1;
    int oldFd = -1;
    oldFd = memfd_create(FILE_NAME, 0);
    newFd = memfd_create(NEW_FILE_NAME, 0);
    EXPECT_NE(oldFd, -1);
    EXPECT_NE(newFd, -1);

    fd = dup3(oldFd, newFd, O_CLOEXEC);
    EXPECT_TRUE(fd >= 0);
    EXPECT_EQ(fd, newFd);
    EXPECT_NE(newFd, oldFd);

    ret = fcntl(fd, F_GETFD);
    EXPECT_EQ(ret & FD_CLOEXEC, FD_CLOEXEC);

    close(fd);
    close(oldFd);
    close(newFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_DUP3_0300
 * @tc.name   : Dup3FileInvalidFdFailed_0003
 * @tc.desc   : dup3 file invalid fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(Dup3ApiTest, Dup3FileInvalidFdFailed_0003, Function | MediumTest | Level2)
{
    int fd = -1;
    int oldFd = -1;
    oldFd = memfd_create(FILE_NAME, 0);
    EXPECT_NE(oldFd, -1);

    fd = dup3(oldFd, 2, 0);
    EXPECT_EQ(fd, 2);
    errno = 0;
    fd = dup3(oldFd, -3, 0);
    EXPECT_EQ(fd, -1);
    EXPECT_EQ(errno, EBADF);

    close(fd);
    close(oldFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_DUP3_0400
 * @tc.name   : Dup3FileEqualFdFailed_0004
 * @tc.desc   : dup3 file equal oldfd and newfd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(Dup3ApiTest, Dup3FileEqualFdFailed_0004, Function | MediumTest | Level2)
{
    int fd = -1;

    errno = 0;
    fd = dup3(2, 2, 0);
    EXPECT_EQ(fd, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
}
