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
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace testing::ext;

class HatsUnlinkatTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsUnlinkatTest::SetUp()
{
}
void HatsUnlinkatTest::TearDown()
{
}
void HatsUnlinkatTest::SetUpTestCase()
{
}
void HatsUnlinkatTest::TearDownTestCase()
{
}

static const char *UNLINKAT_TEST_FILE = "/data/local/tmp/tryUnlinkat.txt";
static const char *UNLINKAT_TEST_DIR = "/data/local/tmp";
static const char *UNLINKAT_TEST_FILENAME = "tryUnlinkat.txt";

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UNLINKAT_0100
 * @tc.name   : UnlinkatUnlinkFileSuccess_0001
 * @tc.desc   : Unlinkat unlink file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsUnlinkatTest, UnlinkatUnlinkFileSuccess_0001, Function | MediumTest | Level1)
{
    int fileFd;
    int dirFd;
    int ret;

    fileFd = open(UNLINKAT_TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fileFd >= 3);
    close(fileFd);

    dirFd = open(UNLINKAT_TEST_DIR, O_RDONLY);
    EXPECT_TRUE(dirFd >= 3);

    ret = unlinkat(dirFd, UNLINKAT_TEST_FILENAME, 0);
    EXPECT_TRUE(ret == 0);
    close(dirFd);

    fileFd = open(UNLINKAT_TEST_FILE, O_RDWR, 0644);
    EXPECT_TRUE(fileFd == -1);
    close(fileFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UNLINKAT_0200
 * @tc.name   : UnlinkatUnlinkEmptyDirectorySuccess_0002
 * @tc.desc   : Unlinkat unlink an empty directory success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsUnlinkatTest, UnlinkatUnlinkEmptyDirectorySuccess_0002, Function | MediumTest | Level1)
{
    int ret;
    DIR *testDir;
    struct dirent *dirEntry;
    const char *unlinkatNewDir1 = "/data/local/tmp/unlinkatDir1";


    ret = mkdir(unlinkatNewDir1, 0777);
    if (ret == -1) {
        testDir = opendir(unlinkatNewDir1);
        EXPECT_TRUE(testDir != nullptr);

        dirEntry = readdir(testDir);
        EXPECT_TRUE(dirEntry != nullptr);
        
        ret = strcmp(dirEntry->d_name, ".");
        EXPECT_TRUE(ret == 0);
    }

    ret = unlinkat(AT_FDCWD, unlinkatNewDir1, AT_REMOVEDIR);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UNLINKAT_0300
 * @tc.name   : UnlinkatUnlinkNonemptyDirectoryFail_0003
 * @tc.desc   : Unlinkat unlink a non-empty directory fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsUnlinkatTest, UnlinkatUnlinkNonemptyDirectoryFail_0003, Function | MediumTest | Level2)
{
    int fileFd;
    int ret;
    DIR *testDir;
    struct dirent *dirEntry;
    const char *unlinkatNewDir2 = "/data/local/tmp/unlinkatDir2";
    const char *unlinkatNewFile2 = "/data/local/tmp/unlinkatDir2/testFile.txt";

    ret = mkdir(unlinkatNewDir2, 0777);
    if (ret == -1) {
        testDir = opendir(unlinkatNewDir2);
        EXPECT_TRUE(testDir != nullptr);

        dirEntry = readdir(testDir);
        EXPECT_TRUE(dirEntry != nullptr);
        
        ret = strcmp(dirEntry->d_name, ".");
        if (ret == 0) {
            fileFd = open(unlinkatNewFile2, O_RDWR | O_CREAT, 0777);
            EXPECT_TRUE(fileFd >= 3);
            close(fileFd);
        }
    } else {
        fileFd = open(unlinkatNewFile2, O_RDWR | O_CREAT, 0777);
        EXPECT_TRUE(fileFd >= 3);
        close(fileFd);
    }
    errno = 0;
    ret = unlinkat(AT_FDCWD, unlinkatNewDir2, AT_REMOVEDIR);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, ENOTEMPTY);

    remove(unlinkatNewFile2);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UNLINKAT_0400
 * @tc.name   : UnlinkatNonexistFileFail_0004
 * @tc.desc   : Unlinkat a non-exist file fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsUnlinkatTest, UnlinkatNonexistFileFail_0004, Function | MediumTest | Level2)
{
    int fileFd;
    int dirFd;
    int ret;

    remove(UNLINKAT_TEST_FILE);
    fileFd = open(UNLINKAT_TEST_FILE, O_RDWR | O_TRUNC, 0644);
    EXPECT_TRUE(fileFd == -1);
    close(fileFd);

    dirFd = open(UNLINKAT_TEST_DIR, O_RDONLY);
    EXPECT_TRUE(dirFd >= 3);

    errno = 0;
    ret = unlinkat(dirFd, UNLINKAT_TEST_FILENAME, 0);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, ENOENT);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UNLINKAT_0500
 * @tc.name   : UnlinkatDirectoryWhenFlagIsNotRemovedirFail_0005
 * @tc.desc   : Unlinkat unlink a directory when flag is not set to AT_REMOVEDIR fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsUnlinkatTest, UnlinkatDirectoryWhenFlagIsNotRemovedirFail_0005, Function | MediumTest | Level2)
{
    int ret;
    DIR *testDir;
    struct dirent *dirEntry;
    const char *unlinkatNewDir3 = "/data/local/tmp/unlinkatDir3";

    ret = mkdir(unlinkatNewDir3, 0777);
    if (ret == -1) {
        testDir = opendir(unlinkatNewDir3);
        EXPECT_TRUE(testDir != nullptr);

        dirEntry = readdir(testDir);
        EXPECT_TRUE(dirEntry != nullptr);
        
        ret = strcmp(dirEntry->d_name, ".");
        EXPECT_TRUE(ret == 0);
    }
    errno = 0;
    ret = unlinkat(AT_FDCWD, unlinkatNewDir3, 0);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EISDIR);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UNLINKAT_0600
 * @tc.name   : UnlinkatFileWhenFlagIsNot0Fail_0006
 * @tc.desc   : Unlinkat a file when flag is not set 0 fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsUnlinkatTest, UnlinkatFileWhenFlagIsNot0Fail_0006, Function | MediumTest | Level2)
{
    int fileFd;
    int dirFd;
    int ret;

    fileFd = open(UNLINKAT_TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fileFd >= 3);
    close(fileFd);

    dirFd = open(UNLINKAT_TEST_DIR, O_RDONLY);
    EXPECT_TRUE(dirFd >= 3);

    errno = 0;
    ret = unlinkat(dirFd, UNLINKAT_TEST_FILENAME, AT_REMOVEDIR);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, ENOTDIR);
    close(dirFd);
}