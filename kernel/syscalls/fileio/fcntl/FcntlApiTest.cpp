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
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *TEST_FILE = "/data/local/tmp/fcntl.txt";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

class FcntlApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FcntlApiTest::SetUp()
{
}
void FcntlApiTest::TearDown()
{
}
void FcntlApiTest::SetUpTestCase()
{
}
void FcntlApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCNTL_0100
 * @tc.name   : FcntlManipulateFdFlagSuccess_0001
 * @tc.desc   : fcntl get fd flag and set fd flag success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FcntlApiTest, FcntlFdFlagSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int flags = 0;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0644);
    EXPECT_TRUE(fd > 0);

    ret = fcntl(fd, F_SETFD, FD_CLOEXEC);
    EXPECT_EQ(ret, 0);

    flags = fcntl(fd, F_GETFD);
    EXPECT_EQ(flags, FD_CLOEXEC);

    ret = fcntl(fd, F_SETFL, O_NONBLOCK);
    EXPECT_EQ(ret, 0);

    flags = fcntl(fd, F_GETFL);
    EXPECT_EQ(flags & O_NONBLOCK, O_NONBLOCK);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCNTL_0300
 * @tc.name   : FcntlManipulateFileLockSuccess_0003
 * @tc.desc   : fcntl set file lock success and return unlock by get lock check.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FcntlApiTest, FcntlFileStatusSuccess_0003, Function | MediumTest | Level1)
{
    int ret;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    struct flock lockCheck = { 0 };
    struct flock lock = {
        .l_type = F_RDLCK,
        .l_whence = SEEK_SET,
        .l_start = 0,
        .l_len = 0,
    };

    ret = fcntl(fd, F_SETLK, &lock);
    EXPECT_EQ(ret, 0);

    ret = fcntl(fd, F_GETLK, &lockCheck);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(lockCheck.l_type, F_UNLCK);
    EXPECT_EQ(lockCheck.l_whence, lock.l_whence);
    EXPECT_EQ(lockCheck.l_start, lock.l_start);
    EXPECT_EQ(lockCheck.l_len, lock.l_len);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCNTL_0400
 * @tc.name   : FcntlManipulateInvalidFdFail_0004
 * @tc.desc   : fcntl manipulate invalid fd fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FcntlApiTest, FcntlManipulateInvalidFdFail_0004, Function | MediumTest | Level2)
{
    int ret;
    int invalidFd = -1;
    errno = 0;
    ret = fcntl(invalidFd, F_GETFL);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FCNTL_0500
 * @tc.name FcntlSetInvalidLockFail_0005
 * @tc.desc fcntl set invalid nullptr lock fail, errno EFAULT.
*/
HWTEST_F(FcntlApiTest, FcntlSetInvalidLockFail_0005, Function | MediumTest | Level2)
{
    int ret;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ret = fcntl(fd, F_SETLKW, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);

    close(fd);
    remove(TEST_FILE);
}
