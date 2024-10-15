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
#include <fcntl.h>
#include <unistd.h>
#include <vector>
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
static const char *TEST_RENAME_PATH = "/data/local/tmp/renameat_test_dir";
static const char *TEST_RENAME_FILE = "/data/local/tmp/test_dir/renameat_test.txt";
static const char *TEST_RENAME_FILE_TMP = "/data/local/tmp/renameat_test_dir/renameat_test.txt";
static const char *TEST_INVALID_FILE = "/data/local/abcd/abcd.txt";

class HatsRenameatTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsRenameatTest::SetUp()
{
    int fd = -1;
    if (access(TEST_VALID_PATH, F_OK) == 0) {
        (void)remove(TEST_VALID_PATH);
    }
    (void)mkdir(TEST_VALID_PATH, S_IWUSR | S_IRUSR | S_IXUSR);
    fd = open(TEST_VALID_FILE, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    close(fd);
}
void HatsRenameatTest::TearDown()
{
    (void)remove(TEST_VALID_FILE);
    (void)remove(TEST_RENAME_FILE);
    (void)remove(TEST_RENAME_FILE_TMP);
    (void)remove(TEST_VALID_PATH);
    (void)remove(TEST_RENAME_PATH);
}
void HatsRenameatTest::SetUpTestCase()
{
}
void HatsRenameatTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_RENAMEAT_0100
 * @tc.name   : RenameatFileAndPathSuccess_0001
 * @tc.desc   : renameat an existed file or path success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsRenameatTest, RenameatFileAndPathSuccess_0001, Function | MediumTest | Level1)
{
    // renameat exist file success
    int ret = renameat(AT_FDCWD, TEST_VALID_FILE, AT_FDCWD, TEST_RENAME_FILE);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(access(TEST_VALID_FILE, F_OK), -1);
    EXPECT_EQ(access(TEST_RENAME_FILE, F_OK), 0);

    // renameat exist path success
    ret = renameat(AT_FDCWD, TEST_VALID_PATH, AT_FDCWD, TEST_RENAME_PATH);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(access(TEST_VALID_PATH, F_OK), -1);
    EXPECT_EQ(access(TEST_RENAME_PATH, F_OK), 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_RENAMEAT_0200
 * @tc.name   : RenameatNotExistFileFailed_0002
 * @tc.desc   : renameat a file that does not exist failed, errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsRenameatTest, RenameatNotExistFileFailed_0002, Function | MediumTest | Level2)
{
    if (access(TEST_INVALID_FILE, F_OK) == 0) {
        (void)remove(TEST_INVALID_FILE);
    }

    errno = 0;
    int ret = renameat(AT_FDCWD, TEST_INVALID_FILE, AT_FDCWD, TEST_RENAME_FILE);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_RENAMEAT_0300
 * @tc.name   : Renameat2FileAndPathSuccess_0003
 * @tc.desc   : renameat2 an existed file or path success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsRenameatTest, Renameat2FileAndPathSuccess_0003, Function | MediumTest | Level1)
{
    // renameat exist file success
    int ret = renameat2(AT_FDCWD, TEST_VALID_FILE, AT_FDCWD, TEST_RENAME_FILE, 0);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(access(TEST_VALID_FILE, F_OK), -1);
    EXPECT_EQ(access(TEST_RENAME_FILE, F_OK), 0);

    // renameat exist path success
    ret = renameat2(AT_FDCWD, TEST_VALID_PATH, AT_FDCWD, TEST_RENAME_PATH, 0);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(access(TEST_VALID_PATH, F_OK), -1);
    EXPECT_EQ(access(TEST_RENAME_PATH, F_OK), 0);
}