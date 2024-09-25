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

static const int BUFFER_SIZE = 128;
static const char *TEST_FILE = "/data/local/tmp/fsetxattr.txt";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
static const char *XATTR_NAME = "user.author";
static const char *AUTHOR_NAME = "sun";
static const char *AUTHOR_NAME_NEW = "bin";
size_t g_length = strlen(AUTHOR_NAME) + 1;

class FsetxattrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FsetxattrApiTest::SetUp()
{
}
void FsetxattrApiTest::TearDown()
{
}
void FsetxattrApiTest::SetUpTestCase()
{
}
void FsetxattrApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSETXATTR_0100
 * @tc.name   : FsetxattrValidFdAttrSuccess_0001
 * @tc.desc   : fsetxattr set valid file attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FsetxattrApiTest, FsetxattrValidFdAttrSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t ret = -1;
    char buf[BUFFER_SIZE] = {0};
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    ret = fsetxattr(fd, XATTR_NAME, AUTHOR_NAME, g_length, 0);
    EXPECT_EQ(ret, 0);
    ret = fgetxattr(fd, XATTR_NAME, buf, g_length);
    EXPECT_EQ(ret, g_length);
    EXPECT_STREQ(buf, AUTHOR_NAME);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSETXATTR_0200
 * @tc.name   : FsetxattrInvalidFdAttrFail_0002
 * @tc.desc   : fsetxattr set invalid file attribute fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FsetxattrApiTest, FsetxattrInvalidFdAttrFail_0002, Function | MediumTest | Level2)
{
    ssize_t ret = -1;
    int invalidFd = -1;

    errno = 0;
    ret = fsetxattr(invalidFd, XATTR_NAME, AUTHOR_NAME, g_length, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSETXATTR_0300
 * @tc.name   : FsetxattrSetTwiceAttrFail_0003
 * @tc.desc   : fsetxattr set the file attribute twice fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FsetxattrApiTest, FsetxattrSetTwiceAttrFail_0003, Function | MediumTest | Level2)
{
    ssize_t ret = -1;
    char buf[BUFFER_SIZE] = {0};
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);
    
    ret = fsetxattr(fd, XATTR_NAME, AUTHOR_NAME, g_length, 0);
    EXPECT_EQ(ret, 0);
    errno = 0;
    ret = fsetxattr(fd, XATTR_NAME, AUTHOR_NAME_NEW, g_length, XATTR_CREATE);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EEXIST);
    ret = fgetxattr(fd, XATTR_NAME, buf, g_length);
    EXPECT_EQ(ret, g_length);
    EXPECT_STREQ(buf, AUTHOR_NAME);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSETXATTR_0400
 * @tc.name   : FsetxattrSetShortAttrFail_0004
 * @tc.desc   : fsetxattr set file attribute return success but length is shorter than real and finally unexpected.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FsetxattrApiTest, FsetxattrSetShortAttrFail_0004, Function | MediumTest | Level2)
{
    ssize_t ret = -1;
    char buf[BUFFER_SIZE] = {0};
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    ret = fsetxattr(fd, XATTR_NAME, AUTHOR_NAME, 0, 0);
    EXPECT_EQ(ret, 0);

    ret = fgetxattr(fd, XATTR_NAME, buf, g_length);
    EXPECT_NE(ret, g_length);
    EXPECT_STRNE(buf, AUTHOR_NAME);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSETXATTR_0500
 * @tc.name   : FsetxattrReplaceNonExistAttrFail_0005
 * @tc.desc   : fsetxattr replace file non-exist attribute fail, errno ENODATA.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FsetxattrApiTest, FsetxattrReplaceNonExistAttrFail_0005, Function | MediumTest | Level2)
{
    ssize_t ret = -1;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ret = fsetxattr(fd, XATTR_NAME, AUTHOR_NAME, g_length, XATTR_REPLACE);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENODATA);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSETXATTR_0600
 * @tc.name   : FsetxattrSetUnsupportAttrFail_0006
 * @tc.desc   : fsetxattr set file unsupported attribute fail, errno ENOTSUP.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FsetxattrApiTest, FsetxattrSetUnsupportAttrFail_0006, Function | MediumTest | Level2)
{
    ssize_t ret = -1;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ret = fsetxattr(fd, "invalid_attr", AUTHOR_NAME, g_length, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTSUP);

    close(fd);
    remove(TEST_FILE);
}
