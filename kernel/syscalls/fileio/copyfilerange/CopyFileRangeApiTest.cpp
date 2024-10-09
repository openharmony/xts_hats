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

class CopyFileRangeApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void CopyFileRangeApiTest::SetUp()
{
}
void CopyFileRangeApiTest::TearDown()
{
}
void CopyFileRangeApiTest::SetUpTestCase()
{
}
void CopyFileRangeApiTest::TearDownTestCase()
{
}

static const int MAX_LEN = 128;
static const char *TEST_FILE = "/data/local/tmp/CopyFileRange.txt";
static const char *TEST_SRC_FILE = "/data/local/tmp/CopyFileRangeSource.txt";
static const char *TEST_DES_FILE = "/data/local/tmp/CopyFileRangeDestination.txt";
static const char *TEST_SRC_DIR = "/data/local/tmp/CopyFileRangeSource";
static const char *TEST_DES_DIR = "/data/local/tmp/CopyFileRangeDestination";
static const char *TEST_DATA = "Hello, world!";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
mode_t MODE_0755 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_COPYFILERANGE_0100
 * @tc.name   : CopyFileRangeCopyValidFdContentSuccess_0001
 * @tc.desc   : copy_file_range copy TEST_DATA  in fdOut with specify length from fdIn success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(CopyFileRangeApiTest, CopyFileRangeCopyValidFdContentSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t size;
    loff_t offIn = 0;
    loff_t offOut = 0;
    int ret = 0;
    char buffer[MAX_LEN];
    int fdIn = open(TEST_SRC_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0644);
    EXPECT_TRUE(fdIn > 0);
    ret = write(fdIn, TEST_DATA, strlen(TEST_DATA));
    EXPECT_EQ(ret, strlen(TEST_DATA));
    int fdOut = open(TEST_DES_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0644);
    EXPECT_TRUE(fdOut > 0);

    size = copy_file_range(fdIn, &offIn, fdOut, &offOut, strlen(TEST_DATA), 0);
    EXPECT_EQ(size, strlen(TEST_DATA));
    ret = read(fdOut, buffer, strlen(TEST_DATA));
    EXPECT_EQ(size, strlen(TEST_DATA));
    EXPECT_STREQ(buffer, TEST_DATA);

    close(fdIn);
    close(fdOut);
    remove(TEST_SRC_FILE);
    remove(TEST_DES_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_COPYFILERANGE_0200
 * @tc.name   : CopyFileRangeCopyInvalidFdContentFail_0002
 * @tc.desc   : copy_file_range copy from invalid fd or copy into invalid fd fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CopyFileRangeApiTest, CopyFileRangeCopyInvalidFdContentFail_0002, Function | MediumTest | Level2)
{
    ssize_t size;
    loff_t offIn = 0;
    loff_t offOut = 0;
    int invalidFd = -1;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0644);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    size = copy_file_range(invalidFd, &offIn, fd, &offOut, strlen(TEST_DATA), 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    size = copy_file_range(fd, &offIn, invalidFd, &offOut, strlen(TEST_DATA), 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_COPYFILERANGE_0300
 * @tc.name   : CopyFileRangeCopyInvalidFlagFail_0003
 * @tc.desc   : copy_file_range copy with invalid flag fail, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CopyFileRangeApiTest, CopyFileRangeCopyInvalidFlagFail_0003, Function | MediumTest | Level2)
{
    ssize_t size;
    loff_t offIn = 0;
    loff_t offOut = 0;
    int fdIn = open(TEST_SRC_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0644);
    EXPECT_TRUE(fdIn > 0);
    int fdOut = open(TEST_DES_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0644);
    EXPECT_TRUE(fdOut > 0);

    errno = 0;
    size = copy_file_range(fdIn, &offIn, fdOut, &offOut, strlen(TEST_DATA), 1);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fdIn);
    close(fdOut);
    remove(TEST_SRC_FILE);
    remove(TEST_DES_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_COPYFILERANGE_0400
 * @tc.name   : CopyFileRangeCopyFdNoPermissionFail_0004
 * @tc.desc   : copy_file_range copy TEST_DATA without correct permission in fdOut or fdIn fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CopyFileRangeApiTest, CopyFileRangeCopyFdNoPermissionFail_0004, Function | MediumTest | Level2)
{
    ssize_t size;
    loff_t offIn = 0;
    loff_t offOut = 0;
    int result = 0;
    int fdIn = open(TEST_SRC_FILE, O_WRONLY | O_CREAT, MODE_0644);
    EXPECT_TRUE(fdIn > 0);
    result = write(fdIn, TEST_DATA, strlen(TEST_DATA));
    EXPECT_EQ(result, strlen(TEST_DATA));
    int fdOut = open(TEST_DES_FILE, O_RDONLY | O_CREAT, MODE_0644);
    EXPECT_TRUE(fdOut > 0);
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    size = copy_file_range(fdIn, &offIn, fd, &offOut, strlen(TEST_DATA), 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    size = copy_file_range(fd, &offIn, fdOut, &offOut, strlen(TEST_DATA), 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);

    close(fd);
    close(fdIn);
    close(fdOut);
    remove(TEST_FILE);
    remove(TEST_SRC_FILE);
    remove(TEST_DES_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_COPYFILERANGE_0500
 * @tc.name   : CopyFileRangeCopyOverOffsetContentFail_0005
 * @tc.desc   : copy_file_range copy TEST_DATA  in fdOut with over offset length from fdIn .
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CopyFileRangeApiTest, CopyFileRangeCopyOverOffsetContentFail_0005, Function | MediumTest | Level2)
{
    int ret;
    ssize_t size;
    loff_t offIn = 0;
    loff_t offOut = 0;
    if (access(TEST_SRC_DIR, F_OK) != 0) {
        ret = mkdir(TEST_SRC_DIR, MODE_0755);
        EXPECT_EQ(ret, 0);
    }
    int fdIn = open(TEST_SRC_DIR, O_RDONLY | O_DIRECTORY);
    EXPECT_NE(fdIn, -1);
    if (access(TEST_DES_DIR, F_OK) != 0) {
        ret = mkdir(TEST_DES_DIR, MODE_0755);
        EXPECT_EQ(ret, 0);
    }
    int fdOut = open(TEST_DES_DIR, O_RDONLY | O_DIRECTORY);
    EXPECT_NE(fdOut, -1);

    errno = 0;
    size = copy_file_range(fdIn, &offIn, fdOut, &offOut, strlen(TEST_DATA), 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EISDIR);

    close(fdIn);
    close(fdOut);
    remove(TEST_SRC_DIR);
    remove(TEST_DES_DIR);
}