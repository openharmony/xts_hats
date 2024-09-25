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

static const char *TEST_FILE = "/data/local/tmp/fremovexattr.txt";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
static const char *XATTR_NAME = "user.author";
static const char *AUTHOR_NAME = "sun";
size_t g_length = strlen(AUTHOR_NAME) + 1;

class FremovexattrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FremovexattrApiTest::SetUp()
{
}
void FremovexattrApiTest::TearDown()
{
}
void FremovexattrApiTest::SetUpTestCase()
{
}
void FremovexattrApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FREMOVEXATTR_0100
 * @tc.name   : FremovexattrValidFdAttrSuccess_0001
 * @tc.desc   : fremovexattr remove valid file attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FremovexattrApiTest, FremovexattrValidFdAttrSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t ret = -1;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);
    ret = setxattr(TEST_FILE, XATTR_NAME, AUTHOR_NAME, g_length, 0);
    EXPECT_EQ(ret, 0);

    ret = fremovexattr(fd, XATTR_NAME);
    EXPECT_EQ(ret, 0);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FREMOVEXATTR_0200
 * @tc.name   : FremovexattrInvalidFdAttrFail_0002
 * @tc.desc   : fremovexattr remove invalid file attribute fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FremovexattrApiTest, FremovexattrInvalidFdAttrFail_0002, Function | MediumTest | Level2)
{
    int invalidFd = -1;
    errno = 0;
    ssize_t ret = fremovexattr(invalidFd, XATTR_NAME);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FREMOVEXATTR_0300
 * @tc.name   : FremovexattrRemoveNonExistAttrFail_0003
 * @tc.desc   : fremovexattr remove file non-exist attribute fail, errno ENODATA.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FremovexattrApiTest, FremovexattrRemoveNonExistAttrFail_0003, Function | MediumTest | Level2)
{
    ssize_t ret = -1;
    int fd = open(TEST_FILE, O_RDONLY | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ret = fremovexattr(fd, XATTR_NAME);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENODATA);

    close(fd);
    remove(TEST_FILE);
}
