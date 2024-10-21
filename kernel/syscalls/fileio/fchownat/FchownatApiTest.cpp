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
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const int PATH_MAX_LEN = 128;
static const char *TEST_FILE = "/data/local/tmp/fchownat.txt";
static const char *TEST_FILE_PATH = "/data/local/tmp";
static const char *TEST_FILE_NAME = "fchownat.txt";
static const char *SYMBOL_LINK_NAME = "TestFchmodatSymlink";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
mode_t MODE_0755 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

class FchownatApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FchownatApiTest::SetUp()
{
}
void FchownatApiTest::TearDown()
{
}
void FchownatApiTest::SetUpTestCase()
{
}
void FchownatApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHOWNAT_0100
 * @tc.name   : FchownatFileSuccess_0001
 * @tc.desc   : fchownat change specify path file owner id and group id success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FchownatApiTest, FchownatFileSuccess_0001, Function | MediumTest | Level1)
{
    int ret = -1;
    struct stat stat1;
    struct stat stat2;

    int dirfd = open(TEST_FILE_PATH, O_RDONLY | O_DIRECTORY);
    EXPECT_TRUE(dirfd > 0);

    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0755);
    EXPECT_TRUE(fd > 0);
    ret = fstat(fd, &stat1);
    EXPECT_EQ(ret, 0);
    uid_t uid = stat1.st_uid + 1;
    gid_t gid = stat1.st_gid + 1;

    ret = fchownat(dirfd, TEST_FILE_NAME, uid, gid, 0);
    EXPECT_EQ(ret, 0);
    ret = fstat(fd, &stat2);
    EXPECT_EQ(stat2.st_uid, uid);
    EXPECT_EQ(stat2.st_gid, gid);

    close(dirfd);
    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHOWNAT_0200
 * @tc.name   : FchownatAT_FDCWDTestSuccess_0002
 * @tc.desc   : fchownat change current path file owner id and group id by AT_FDCWD success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FchownatApiTest, FchownatAT_FDCWDTestSuccess_0002, Function | MediumTest | Level1)
{
    int ret = -1;
    char path[PATH_MAX_LEN];
    struct stat stat1;
    struct stat stat2;
    int fd = 0;

    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);
    ret = chdir(TEST_FILE_PATH);
    EXPECT_EQ(ret, 0);
    if (access(TEST_FILE_NAME, F_OK) == -1) {
        fd = open(TEST_FILE_NAME, O_RDWR | O_CREAT, MODE_0644);
    }

    ret = fstat(fd, &stat1);
    EXPECT_EQ(ret, 0);
    uid_t uid = stat1.st_uid + 1;
    gid_t gid = stat1.st_gid + 1;

    ret = fchownat(AT_FDCWD, TEST_FILE_NAME, uid, gid, 0);
    EXPECT_EQ(ret, 0);
    ret = fstat(fd, &stat2);
    EXPECT_EQ(stat2.st_uid, uid);
    EXPECT_EQ(stat2.st_gid, gid);

    chdir(path);
    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHOWNAT_0300
 * @tc.name   : FchownatInvalidFdFail_0003
 * @tc.desc   : fchownat change invalid file owner id and group id fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FchownatApiTest, FchownatInvalidFdFail_0003, Function | MediumTest | Level2)
{
    int invalidFd = -1;
    errno = 0;
    int ret = fchownat(invalidFd, TEST_FILE_NAME, 1, 1, 0);
    EXPECT_NE(ret, 0);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHOWNAT_0400
 * @tc.name   : FchownatLinkFileTestSuccess_0004
 * @tc.desc   : fchownat change file owner id and group id by file link success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FchownatApiTest, FchownatLinkFileTestSuccess_0004, Function | MediumTest | Level1)
{
    int ret = -1;
    struct stat stat1;
    struct stat stat2;
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);
    ret = chdir(TEST_FILE_PATH);
    EXPECT_EQ(ret, 0);

    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0755);
    EXPECT_TRUE(fd > 0);
    ret = fstat(fd, &stat1);
    EXPECT_EQ(ret, 0);
    uid_t uid = stat1.st_uid + 1;
    gid_t gid = stat1.st_gid + 1;

    ret = symlink(TEST_FILE_NAME, SYMBOL_LINK_NAME);
    EXPECT_EQ(ret, 0);

    ret = fchownat(AT_FDCWD, SYMBOL_LINK_NAME, uid, gid, 0);
    EXPECT_EQ(ret, 0);
    ret = fstat(fd, &stat2);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(stat2.st_uid, uid);
    EXPECT_EQ(stat2.st_gid, gid);

    unlink(TEST_FILE_NAME);
    close(fd);
    remove(TEST_FILE);
    remove(SYMBOL_LINK_NAME);
    chdir(path);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHOWNAT_0500
 * @tc.name   : FchownatAT_SYMLINK_NOFOLLOWTestSuccess_0005
 * @tc.desc   : fchownat change owner id and group id of file link itself success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FchownatApiTest, FchownatAT_SYMLINK_NOFOLLOWTestSuccess_0005, Function | MediumTest | Level1)
{
    int ret = -1;
    struct stat stat1;
    struct stat stat2;
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);
    ret = chdir(TEST_FILE_PATH);
    EXPECT_EQ(ret, 0);

    int fd = open(TEST_FILE_NAME, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);
    ret = symlink(TEST_FILE_NAME, SYMBOL_LINK_NAME);
    EXPECT_EQ(ret, 0);

    ret = fstat(fd, &stat1);
    EXPECT_EQ(ret, 0);
    uid_t uid = stat1.st_uid + 1;
    gid_t gid = stat1.st_gid + 1;

    ret = fchownat(AT_FDCWD, SYMBOL_LINK_NAME, uid, gid, AT_SYMLINK_NOFOLLOW);
    EXPECT_EQ(ret, 0);
    ret = fstat(fd, &stat2);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(stat2.st_uid, uid);
    EXPECT_NE(stat2.st_gid, gid);

    unlink(TEST_FILE_NAME);
    close(fd);
    remove(TEST_FILE);
    remove(SYMBOL_LINK_NAME);
    chdir(path);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHOWNAT_0600
 * @tc.name   : FchownatAT_EMPTY_PATHTestSuccess_0006
 * @tc.desc   : fchownat change current path file owner id and group id by AT_EMPTY_PATH success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FchownatApiTest, FchownatAT_EMPTY_PATHTestSuccess_0006, Function | MediumTest | Level1)
{
    int ret = -1;
    struct stat stat1;

    int fd = open(TEST_FILE, O_RDWR | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);

    ret = fstat(fd, &stat1);
    EXPECT_EQ(ret, 0);
    uid_t uid = stat1.st_uid + 1;
    gid_t gid = stat1.st_gid + 1;

    ret = fchownat(fd, "", uid, gid, AT_EMPTY_PATH);
    EXPECT_EQ(ret, 0);

    close(fd);
    remove(TEST_FILE);
}
