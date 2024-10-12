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
#include <sys/xattr.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *TEST_DIR = "/data/local/tmp";
static const char *OLD_FILE_NAME = "/data/local/tmp/old_file";
static const char *NEW_FILE_NAME = "/data/local/tmp/new_link";
static const char *INVALID_FILE_NAME = "/data/local/tmp/invalid";

class LinkatApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void LinkatApiTest::SetUp()
{
}
void LinkatApiTest::TearDown()
{
}
void LinkatApiTest::SetUpTestCase()
{
}
void LinkatApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LINKAT_0100
 * @tc.name   : LinkatCreateHardLinkSuccess_0001
 * @tc.desc   : linkat create hard link success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LinkatApiTest, LinkatCreateHardLinkSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int dirFd;
    dirFd = open(TEST_DIR, O_DIRECTORY);
    EXPECT_GE(dirFd, 0);
    int fd = open(OLD_FILE_NAME, O_CREAT | O_RDWR, 0644);
    close(fd);

    ret = linkat(dirFd, OLD_FILE_NAME, dirFd, NEW_FILE_NAME, 0);
    EXPECT_GE(ret, 0);

    close(dirFd);
    unlink(NEW_FILE_NAME);
    unlink(OLD_FILE_NAME);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LINKAT_0200
 * @tc.name   : LinkatInvalidFlagFailed_0002
 * @tc.desc   : linkat invalid flag failed errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LinkatApiTest, LinkatInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    int ret;
    int fd = open(OLD_FILE_NAME, O_CREAT | O_RDWR, 0644);
    close(fd);

    errno = 0;
    ret = linkat(0, OLD_FILE_NAME, 0, NEW_FILE_NAME, -1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    unlink(NEW_FILE_NAME);
    unlink(OLD_FILE_NAME);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LINKAT_0300
 * @tc.name   : LinkatInvalidPathFailed_0003
 * @tc.desc   : linkat invalid path failed errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LinkatApiTest, LinkatInvalidPathFailed_0003, Function | MediumTest | Level2)
{
    int ret;
    int dirFd;
    dirFd = open(TEST_DIR, O_DIRECTORY);
    EXPECT_GE(dirFd, 0);
    int fd = open(OLD_FILE_NAME, O_CREAT | O_RDWR, 0644);
    close(fd);

    ret = linkat(dirFd, INVALID_FILE_NAME, dirFd, NEW_FILE_NAME, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);

    close(dirFd);
    unlink(NEW_FILE_NAME);
    unlink(OLD_FILE_NAME);
}