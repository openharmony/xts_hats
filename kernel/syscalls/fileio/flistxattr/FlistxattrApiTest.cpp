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

static const int LIST_LEN = 128;
static const char *TEST_FILE = "/data/local/tmp/flistxattr.txt";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
static const char *XATTR_NAME = "user.author";
static const char *AUTHOR_NAME = "sun";
size_t g_length = strlen(AUTHOR_NAME) + 1;

class FlistxattrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FlistxattrApiTest::SetUp()
{
}
void FlistxattrApiTest::TearDown()
{
}
void FlistxattrApiTest::SetUpTestCase()
{
}
void FlistxattrApiTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FLISTXATTR_0100
 * @tc.name FlistxattrValidFdAttrSuccess_0001
 * @tc.desc flistxattr list valid test file attribute success.
*/
HWTEST_F(FlistxattrApiTest, FlistxattrValidFdAttrSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t ret = -1;
    char list[LIST_LEN];
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);
    ret = flistxattr(fd, list, 1024);
    EXPECT_NE(ret, -1);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FLISTXATTR_0200
 * @tc.name FlistxattrInvalidFdAttrFail_0002
 * @tc.desc flistxattr list invalid fd attribute fail, errno EBADF.
*/
HWTEST_F(FlistxattrApiTest, FlistxattrInvalidFdAttrFail_0002, Function | MediumTest | Level2)
{
    char list[LIST_LEN];
    int invalidFd = -1;
    errno = 0;
    ssize_t ret = flistxattr(invalidFd, list, 1024);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FLISTXATTR_0300
 * @tc.name FlistxattrListAttrInNullFail_0003
 * @tc.desc flistxattr can't list file attribute in nullptr, errno EFAULT.
*/
HWTEST_F(FlistxattrApiTest, FlistxattrListAttrInNullFail_0003, Function | MediumTest | Level2)
{
    ssize_t ret = -1;
    char *list = nullptr;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);
    errno = 0;
    ret = flistxattr(fd, list, 1024);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(list, nullptr);
    EXPECT_EQ(errno, EFAULT);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FLISTXATTR_0400
 * @tc.name FlistxattrListShorterAttrFail_0004
 * @tc.desc flistxattr can't list file attribute with size 1 shorter than expected, errno ERANGE.
*/
HWTEST_F(FlistxattrApiTest, FlistxattrListShorterAttrFail_0004, Function | MediumTest | Level2)
{
    ssize_t ret = -1;
    char list[LIST_LEN];
    memset_s(list, LIST_LEN, 0, sizeof(list));
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);
    ret = setxattr(TEST_FILE, XATTR_NAME, AUTHOR_NAME, g_length, 0);
    EXPECT_EQ(ret, 0);

    errno = 0;
    ret = flistxattr(fd, list, 1);
    EXPECT_EQ(errno, ERANGE);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(list[0], '\0');

    close(fd);
    remove(TEST_FILE);
}
