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
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const int PATH_MAX_LEN = 128;
static const char *TEST_FILE_PATH = "/data/local/tmp";
static const char *TEST_FILE = "/data/local/tmp/test.txt";
static const char *TEST_FILE_NAME = "test.txt";
static const char *TEST_NOT_EXIST_FILE = "/data/local/tmp/NotExist.txt";
static const char *TEST_NOT_EXIST_FILE_NAME = "NotExist.txt";

class FaccessatApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FaccessatApiTest::SetUp()
{
}
void FaccessatApiTest::TearDown()
{
}
void FaccessatApiTest::SetUpTestCase()
{
}
void FaccessatApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FACCESSAT_0100
 * @tc.name   : FaccessatCheckSuccess_0001
 * @tc.desc   : faccessat checks the calling process can access the file in current directory successful.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FaccessatApiTest, FaccessatCheckSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);
    if (access(TEST_FILE_PATH, F_OK) != 0) {
        ret = mkdir(TEST_FILE_PATH, 0755);
        EXPECT_EQ(ret, 0);
    }
    ret = chdir(TEST_FILE_PATH);
    EXPECT_EQ(ret, 0);
    if (access(TEST_FILE_NAME, F_OK) == -1) {
        FILE *fp = fopen(TEST_FILE_NAME, "w");
        EXPECT_NE(fp, nullptr);
        fclose(fp);
    }
    ret = faccessat(AT_FDCWD, TEST_FILE_NAME, F_OK, AT_EACCESS);
    EXPECT_EQ(ret, 0);

    remove(TEST_FILE);
    chdir(path);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FACCESSAT_0200
 * @tc.name   : FaccessatPermissionCheckTest_0002
 * @tc.desc   : faccessat check user's permissions of valid fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FaccessatApiTest, FaccessatPermissionCheckTest_0002, Function | MediumTest | Level1)
{
    int ret;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, 0755);
    EXPECT_TRUE(fd > 0);

    ret = faccessat(fd, TEST_FILE, F_OK, AT_EACCESS);
    EXPECT_TRUE(ret == 0);

    ret = faccessat(fd, TEST_FILE, R_OK, AT_EACCESS);
    EXPECT_TRUE(ret == 0);

    ret = faccessat(fd, TEST_FILE, W_OK, AT_EACCESS);
    EXPECT_TRUE(ret == 0);

    ret = faccessat(fd, TEST_FILE, X_OK, AT_EACCESS);
    EXPECT_TRUE(ret == 0);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FACCESSAT_0300
 * @tc.name   : FaccessatCheckInvalidFdFail_0003
 * @tc.desc   : faccessat check invalid fd permission fail, errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FaccessatApiTest, FaccessatCheckInvalidFdFail_0003, Function | MediumTest | Level2)
{
    int ret;
    int invalidFd = -1;
    errno = 0;
    ret = faccessat(invalidFd, TEST_NOT_EXIST_FILE, F_OK, AT_EACCESS);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FACCESSAT_0400
 * @tc.name   : FaccessatCheckFail_0004
 * @tc.desc   : faccessat check non-exist file permission fail, errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FaccessatApiTest, FaccessatCheckFail_0004, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = faccessat(AT_FDCWD, TEST_NOT_EXIST_FILE_NAME, W_OK, AT_EACCESS);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
}
