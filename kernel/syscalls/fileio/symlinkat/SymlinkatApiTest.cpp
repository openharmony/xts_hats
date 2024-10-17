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
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

static const char *TEST_VALID_PATH = "/data/local/tmp/test_dir";
static const char *TEST_VALID_FILE = "/data/local/tmp/test_dir/test.txt";
static const char *TEST_LINK_PATH = "/data/local/tmp/symlinkat_test_dir";
static const char *TEST_LINK_FILE = "/data/local/tmp/test_dir/symlinkat_test.txt";
static const char *TEST_NOACCESS_PATH = "/data/local/abcd/abcd";

class HatsSymlinkatTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsSymlinkatTest::SetUp()
{
    int fd = -1;
    if (access(TEST_VALID_PATH, F_OK) == 0) {
        (void)remove(TEST_VALID_PATH);
    }
    (void)mkdir(TEST_VALID_PATH, S_IWUSR | S_IRUSR | S_IXUSR);
    fd = open(TEST_VALID_FILE, O_WRONLY | O_CREAT, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    close(fd);
}
void HatsSymlinkatTest::TearDown()
{
    (void)remove(TEST_VALID_FILE);
    (void)remove(TEST_LINK_FILE);
    (void)remove(TEST_VALID_PATH);
    (void)remove(TEST_LINK_PATH);
}
void HatsSymlinkatTest::SetUpTestCase()
{
}
void HatsSymlinkatTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYMLINKAT_0100
 * @tc.name   : SymlinkatFileAndPathSuccess_0001
 * @tc.desc   : symlinkat create file and path symlink success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSymlinkatTest, SymlinkatFileAndPathSuccess_0001, Function | MediumTest | Level1)
{
    // symlinkat create file symlink success
    int ret = symlinkat(TEST_VALID_FILE, AT_FDCWD, TEST_LINK_FILE);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(access(TEST_LINK_FILE, F_OK), 0);

    // symlinkat create path symlink success
    ret = symlinkat(TEST_VALID_PATH, AT_FDCWD, TEST_LINK_PATH);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(access(TEST_LINK_PATH, F_OK), 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYMLINKAT_0200
 * @tc.name   : SymlinkatNoExistLinkPathFailed_0002
 * @tc.desc   : symlinkat create path symlink which is no exist path failed, errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSymlinkatTest, SymlinkatNoPermissionPathFailed_0002, Function | MediumTest | Level1)
{
    errno = 0;
    int ret = symlinkat(TEST_VALID_PATH, AT_FDCWD, TEST_NOACCESS_PATH);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYMLINKAT_0300
 * @tc.name   : SymlinkatExistPathFailed_0003
 * @tc.desc   : symlinkat create path symlink which is already exist failed, errno EEXIST.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSymlinkatTest, SymlinkatExistPathFailed_0003, Function | MediumTest | Level1)
{
    int ret;
    if (access(TEST_LINK_PATH, F_OK) != 0) {
        ret = mkdir(TEST_LINK_PATH, S_IWUSR | S_IRUSR | S_IXUSR);
        EXPECT_EQ(ret, 0);
    }

    errno = 0;
    ret = symlinkat(TEST_VALID_PATH, AT_FDCWD, TEST_LINK_PATH);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EEXIST);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYMLINKAT_0400
 * @tc.name   : SymlinkatLinkPathLongerFailed_0004
 * @tc.desc   : symlinkat create path symlink which the name is too longer failed, errno ENAMETOOLONG.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSymlinkatTest, SymlinkatLinkPathLongerFailed_0004, Function | MediumTest | Level1)
{
    int ret;
    char name[256];
    (void)memset_s(name, sizeof(name), 's', sizeof(name));

    errno = 0;
    ret = symlinkat(TEST_VALID_PATH, AT_FDCWD, name);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENAMETOOLONG);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYMLINKAT_0500
 * @tc.name   : SymlinkatLinkPathNullptrFailed_0005
 * @tc.desc   : symlinkat create path symlink is nullptr failed, errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSymlinkatTest, SymlinkatLinkPathNullptrFailed_0005, Function | MediumTest | Level1)
{
    errno = 0;
    int ret = symlinkat(TEST_VALID_PATH, AT_FDCWD, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYMLINKAT_0600
 * @tc.name   : SymlinkatInvalidFdFailed_0006
 * @tc.desc   : symlinkat used invalid fd failed, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSymlinkatTest, SymlinkatInvalidFdFailed_0006, Function | MediumTest | Level1)
{
    errno = 0;
    int ret = symlinkat("test_path", -1, "symlinkat_test_dir");
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}
