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

#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *OPEN_API_TEST_PATH = "/data/local/tmp";
static const char *TEST_PATH = "/data/local/tmp/test_path";
static const char *TEST_PATH_REG = "/data/local/tmp/test_reg";
static const char *TEST_PATH_CHR = "/data/local/tmp/test_chr";
static const char *TEST_PATH_BLK = "/data/local/tmp/test_blk";
static const char *TEST_PATH_SOCK = "/data/local/tmp/test_sock";

class MknodatApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MknodatApiTest::SetUp()
{
}
void MknodatApiTest::TearDown()
{
}
void MknodatApiTest::SetUpTestCase()
{
}
void MknodatApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKNODAT_0100
 * @tc.name   : MknodatCreateDirSuccess_0001
 * @tc.desc   : mknodat create a directory by fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MknodatApiTest, MknodatCreateDirSuccess_0001, Function | MediumTest | Level1)
{
    const char *testPath = "test_path";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int dirFd = open(OPEN_API_TEST_PATH, O_RDONLY);
    EXPECT_TRUE(dirFd > 0);

    int ret = mknodat(dirFd, testPath, S_IFIFO | mode, 0);
    EXPECT_TRUE(ret == 0);
    close(dirFd);
    unlink(TEST_PATH);
}
/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKNODAT_0200
 * @tc.name   : MknodatCreateExistDirFailed_0002
 * @tc.desc   : mknodat create the exist directory failed, errno EEXIST.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MknodatApiTest, MknodatCreateExistDirFailed_0002, Function | MediumTest | Level2)
{
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    errno = 0;
    int ret = mknodat(AT_FDCWD, OPEN_API_TEST_PATH, mode, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EEXIST);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKNODAT_0300
 * @tc.name   : MknodatCreatePathInNotExistDirFailed_0003
 * @tc.desc   : mknodat path in not exist directory failed, errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MknodatApiTest, MknodatCreatePathInNotExistDirFailed_0003, Function | MediumTest | Level2)
{
    const char *path = "/data/local/tmp/abcd/abcd";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    errno = 0;
    int ret = mknodat(AT_FDCWD, path, mode, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKNODAT_0400
 * @tc.name   : MknodatUseInvalidFdFailed_0004
 * @tc.desc   : mknodat use invalid fd failed, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MknodatApiTest, MknodatUseInvalidFdFailed_0004, Function | MediumTest | Level2)
{
    const char *testPath = "test_path";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    errno = 0;
    int ret = mknodat(-1, testPath, mode, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKNODAT_0500
 * @tc.name   : MknodatCreateRegularFileSuccess_0005
 * @tc.desc   : mknodat create a regular file by fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MknodatApiTest, MknodatCreateRegularFileSuccess_0005, Function | MediumTest | Level1)
{
    const char *testPath = TEST_PATH_REG;
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int dirFd = open(OPEN_API_TEST_PATH, O_RDONLY);
    EXPECT_TRUE(dirFd > 0);

    int ret = mknodat(dirFd, testPath, S_IFREG | mode, 0);
    EXPECT_TRUE(ret == 0);
    close(dirFd);
    unlink(testPath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKNODAT_0600
 * @tc.name   : MknodatCreateCharacterDeviceSuccess_0006
 * @tc.desc   : mknodat create a character device by fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MknodatApiTest, MknodatCreateCharacterDeviceSuccess_0006, Function | MediumTest | Level1)
{
    const char *testPath = TEST_PATH_CHR;
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int dirFd = open(OPEN_API_TEST_PATH, O_RDONLY);
    EXPECT_TRUE(dirFd > 0);

    int ret = mknodat(dirFd, testPath, S_IFCHR | mode, makedev(1, 3));
    EXPECT_TRUE(ret == 0);
    close(dirFd);
    unlink(testPath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKNODAT_0700
 * @tc.name   : MknodatCreateBlockDeviceSuccess_0007
 * @tc.desc   : mknodat create a block device by fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MknodatApiTest, MknodatCreateBlockDeviceSuccess_0007, Function | MediumTest | Level1)
{
    const char *testPath = TEST_PATH_BLK;
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int dirFd = open(OPEN_API_TEST_PATH, O_RDONLY);
    EXPECT_TRUE(dirFd > 0);

    int ret = mknodat(dirFd, testPath, S_IFBLK | mode, makedev(1, 4));
    EXPECT_TRUE(ret == 0);
    close(dirFd);
    unlink(testPath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKNODAT_0800
 * @tc.name   : MknodatCreateSocketSuccess_0008
 * @tc.desc   : mknodat create a socket by fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MknodatApiTest, MknodatCreateSocketSuccess_0008, Function | MediumTest | Level1)
{
    const char *testPath = TEST_PATH_SOCK;
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int dirFd = open(OPEN_API_TEST_PATH, O_RDONLY);
    EXPECT_TRUE(dirFd > 0);

    int ret = mknodat(dirFd, testPath, S_IFSOCK | mode, 0);
    EXPECT_TRUE(ret == 0);
    close(dirFd);
    unlink(testPath);
}