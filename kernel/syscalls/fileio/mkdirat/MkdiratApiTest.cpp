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
#include <string>
#include <unistd.h>
#include <vector>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *OPEN_API_TEST_PATH = "/data/local/tmp";
static const char *TEST_PATH = "/data/local/tmp/test_path";
const int MAX_LEN = 128;

class MkdiratApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MkdiratApiTest::SetUp()
{
}
void MkdiratApiTest::TearDown()
{
}
void MkdiratApiTest::SetUpTestCase()
{
}
void MkdiratApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKDIRAT_0100
 * @tc.name   : MkdiratCreateDirSuccess_0001
 * @tc.desc   : mkdirat create a directory by fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MkdiratApiTest, MkdiratCreateDirSuccess_0001, Function | MediumTest | Level1)
{
    const char *testPath = "test_path";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int dirFd = open(OPEN_API_TEST_PATH, O_RDONLY);
    EXPECT_TRUE(dirFd > 0);

    int ret = mkdirat(dirFd, testPath, mode);
    EXPECT_TRUE(ret == 0);
    close(dirFd);

    char path[MAX_LEN] = { 0 };
    int num = sprintf_s(path, MAX_LEN, "%s/%s", OPEN_API_TEST_PATH, testPath);
    EXPECT_TRUE(num > 0);
    ret = rmdir(path);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKDIRAT_0200
 * @tc.name   : MkdiratAT_FDCWDCreateDirSuccess_0002
 * @tc.desc   : current the path to create a directory success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MkdiratApiTest, MkdiratAT_FDCWDCreateDirSuccess_0002, Function | MediumTest | Level1)
{
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

    int ret = mkdirat(AT_FDCWD, TEST_PATH, mode);
    EXPECT_EQ(ret, 0);

    ret = rmdir(TEST_PATH);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKDIRAT_0300
 * @tc.name   : MkdiratCreateExistDirFailed_0003
 * @tc.desc   : mkdirat create the exist directory failed, errno EEXIST.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MkdiratApiTest, MkdiratCreateExistDirFailed_0003, Function | MediumTest | Level2)
{
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    errno = 0;
    int ret = mkdirat(AT_FDCWD, OPEN_API_TEST_PATH, mode);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EEXIST);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKDIRAT_0400
 * @tc.name   : MkdiratCreatePathInNotExistDirFailed_0004
 * @tc.desc   : mkdirat path in not exist directory failed, errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MkdiratApiTest, MkdiratCreatePathInNotExistDirFailed_0004, Function | MediumTest | Level2)
{
    const char *path = "/data/local/tmp/abcd/abcd";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    errno = 0;
    int ret = mkdirat(AT_FDCWD, path, mode);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MKDIRAT_0500
 * @tc.name   : MkdiratUseInvalidFdFailed_0005
 * @tc.desc   : mkdirat use invalid fd failed, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MkdiratApiTest, MkdiratUseInvalidFdFailed_0005, Function | MediumTest | Level2)
{
    const char *testPath = "test_path";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    errno = 0;
    int ret = mkdirat(-1, testPath, mode);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}
