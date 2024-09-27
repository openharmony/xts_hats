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
#include <cerrno>
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

class FchdirApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FchdirApiTest::SetUp()
{
}
void FchdirApiTest::TearDown()
{
}
void FchdirApiTest::SetUpTestCase()
{
}
void FchdirApiTest::TearDownTestCase()
{
}

static const int PATH_MAX_LEN = 128;
static const char *TEST_FILE = "/data/local/tmp/fchdir.txt";
static const char *TEST_DIR = "/data/local/tmp/fchdirTest";
static const mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHDIR_0100
 * @tc.name   : FchdirChangeDirectorySuccess_0001
 * @tc.desc   : fchdir change the current working directory and enter valid directory success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FchdirApiTest, FchdirChangeDirectorySuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    char path[PATH_MAX_LEN] = { 0 };
    if (access(TEST_DIR, F_OK) != 0) {
        ret = mkdir(TEST_DIR, 0755);
        EXPECT_EQ(ret, 0);
    }

    int dirfd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);
    EXPECT_TRUE(dirfd > 0);
    ret = fchdir(dirfd);
    EXPECT_EQ(ret, 0);
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);
    EXPECT_STREQ(dir, TEST_DIR);

    close(dirfd);
    remove(TEST_DIR);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHDIR_0200
 * @tc.name   : FchdirChangeDirectoryFail_0002
 * @tc.desc   : fchdir change the current working directory and enter invalid directory fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FchdirApiTest, FchdirChangeDirectoryFail_0002, Function | MediumTest | Level2)
{
    int ret;
    int fd = -1;
    errno = 0;
    ret = fchdir(fd);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHDIR_0300
 * @tc.name   : FchdirChangeFileDescriptorFail_0003
 * @tc.desc   : fchdir change into a file fail, errno ENOTDIR.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FchdirApiTest, FchdirChangeFileDescriptorFail_0003, Function | MediumTest | Level2)
{
    int ret;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ret = fchdir(fd);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTDIR);

    close(fd);
    remove(TEST_FILE);
}
