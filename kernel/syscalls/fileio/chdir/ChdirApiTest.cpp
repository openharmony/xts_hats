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

#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gtest/gtest.h>

using namespace testing::ext;
using namespace std;

static const int PATH_MAX_LEN = 128;
static const char *TEST_FILE_PATH = "/data/local/tmp";
static const char *INVALID_FILE_PATH = "/data/invalid";
static const char *TEST_FILE = "/data/local/tmp/chdir_file";
mode_t MODE_0755 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

class ChdirApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ChdirApiTest::SetUp()
{
}
void ChdirApiTest::TearDown()
{
}
void ChdirApiTest::SetUpTestCase()
{
}
void ChdirApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CHDIR_0100
 * @tc.name   : ChdirChangeSuccess_0001
 * @tc.desc   : Chdir change work dir successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ChdirApiTest, ChdirChangeSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);

    ret = chdir(TEST_FILE_PATH);
    EXPECT_EQ(ret, 0);
    chdir(path);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CHDIR_0200
 * @tc.name   : ChdirNullptrPathFailed_0002
 * @tc.desc   : Chdir nullptr failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ChdirApiTest, ChdirNullptrPathFailed_0002, Function | MediumTest | Level2)
{
    int ret;
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);

    errno = 0;
    ret = chdir(nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    chdir(path);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CHDIR_0300
 * @tc.name   : ChdirInvalidPathFailed_0003
 * @tc.desc   : Chdir invalid dir failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ChdirApiTest, ChdirInvalidPathFailed_0003, Function | MediumTest | Level2)
{
    int ret;
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);

    errno = 0;
    ret = chdir(INVALID_FILE_PATH);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
    chdir(path);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CHDIR_0400
 * @tc.name   : ChdirFileFailed_0004
 * @tc.desc   : Chdir file failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ChdirApiTest, ChdirFileFailed_0004, Function | MediumTest | Level2)
{
    int ret;
    int fd;
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);

    fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0755);
    close(fd);

    errno = 0;
    ret = chdir(TEST_FILE);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTDIR);
    chdir(path);
    unlink(TEST_FILE);
}