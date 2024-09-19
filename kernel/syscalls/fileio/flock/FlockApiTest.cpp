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

static const char *TEST_FILE = "/data/local/tmp/flock.txt";
mode_t MODE_0600 = S_IRUSR | S_IWUSR;
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

class FlockApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FlockApiTest::SetUp()
{
}
void FlockApiTest::TearDown()
{
}
void FlockApiTest::SetUpTestCase()
{
}
void FlockApiTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FLOCK_0100
 * @tc.name FlockValidFdSuccess_0001
 * @tc.desc flock lock valid file with share lock and exclusive lock success.
*/
HWTEST_F(FlockApiTest, FlockValidFdSuccess_0001, Function | MediumTest | Level1)
{
    int ret = -1;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0644);
    EXPECT_TRUE(fd > 0);
    
    ret = flock(fd, LOCK_SH);
    EXPECT_EQ(ret, 0);
    ret = flock(fd, LOCK_UN);
    EXPECT_EQ(ret, 0);

    ret = flock(fd, LOCK_EX);
    EXPECT_EQ(ret, 0);
    ret = flock(fd, LOCK_UN);
    EXPECT_EQ(ret, 0);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FLOCK_0200
 * @tc.name FlockInvalidFdFail_0002
 * @tc.desc flock lock invalid file with share lock fail, errno EBADF.
*/
HWTEST_F(FlockApiTest, FlockInvalidFdFail_0002, Function | MediumTest | Level2)
{
    int ret = -1;
    int invalidFd = -1;

    errno = 0;
    ret = flock(invalidFd, LOCK_SH);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FLOCK_0300
 * @tc.name FlockLockTwiceFail_0003
 * @tc.desc flock try to acquire an exclusive lock on the second descriptor fail, errno EWOULDBLOCK.
*/
HWTEST_F(FlockApiTest, FlockLockTwiceFail_0003, Function | MediumTest | Level2)
{
    int ret = -1;
    int fd1 = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0600);
    EXPECT_TRUE(fd1 > 0);
    int fd2 = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0600);
    EXPECT_TRUE(fd2 > 0);

    ret = flock(fd1, LOCK_EX);
    EXPECT_EQ(ret, 0);

    errno = 0;
    ret = flock(fd2, LOCK_EX | LOCK_NB);
    EXPECT_NE(ret, 0);
    EXPECT_EQ(errno, EWOULDBLOCK);

    flock(fd1, LOCK_UN);
    close(fd1);
    close(fd2);
    remove(TEST_FILE);
}
