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

class FchmodatApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};

void FchmodatApiTest::SetUp()
{
}
void FchmodatApiTest::TearDown()
{
}
void FchmodatApiTest::SetUpTestCase()
{
}
void FchmodatApiTest::TearDownTestCase()
{
}

static const int PATH_MAX_LEN = 128;
static const char *TEST_FILE = "/data/local/tmp/test.txt";
static const char *TEST_FILE_PATH = "/data/local/tmp";
static const char *TEST_FILE_NAME = "test.txt";
static const char *SYMBOL_LINK_NAME = "TestSymlink";
mode_t MODE_0600 = S_IRUSR | S_IWUSR;
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
mode_t MODE_0755 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
struct stat g_statbuf;

/*
 * @tc.number SUB_KERNEL_SYSCALL_FCHMODAT_0100
 * @tc.name FchmodatFileModeSuccess_0001
 * @tc.desc fchmodat valid file mode bits success.
*/
HWTEST_F(FchmodatApiTest, FchmodatFileSuccess_0001, Function | MediumTest | Level1)
{
    int ret = -1;
    if (access(TEST_FILE_PATH, F_OK) != 0) {
        ret = mkdir(TEST_FILE_PATH, MODE_0644);
        EXPECT_EQ(ret, 0);
    }
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);
    ret = chdir(TEST_FILE_PATH);
    EXPECT_EQ(ret, 0);
    if (access(TEST_FILE_NAME, F_OK) == -1) {
        FILE *fp = fopen(TEST_FILE_NAME, "w");
        EXPECT_NE(fp, nullptr);
        fclose(fp);
    }
    int dirfd = open(TEST_FILE_PATH, O_RDONLY | O_DIRECTORY);
    EXPECT_TRUE(dirfd > 0);
    ret = fchmodat(dirfd, TEST_FILE_NAME, MODE_0755, 0);
    EXPECT_EQ(ret, 0);

    remove(TEST_FILE_NAME);
    chdir(path);
    close(dirfd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FCHMODAT_0200
 * @tc.name FchmodatInvalidFdModeFail_0002
 * @tc.desc fchmodat invalid file mode bits fail, errno EBADF.
*/
HWTEST_F(FchmodatApiTest, FchmodatInvalidFdModeFail_0002, Function | MediumTest | Level2)
{
    int ret = -1;
    int invalidFd = -1;
    errno = 0;
    ret = fchmodat(invalidFd, TEST_FILE_NAME, MODE_0755, 0);

    EXPECT_NE(ret, 0);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FCHMODAT_0300
 * @tc.name FchmodatAT_FDCWDModeSuccess_0003
 * @tc.desc fchmodat AT_FDCWD test success.
*/
HWTEST_F(FchmodatApiTest, FchmodatAT_FDCWDModeSuccess_0003, Function | MediumTest | Level1)
{
    int ret = -1;
    int fd = open(TEST_FILE, O_CREAT | O_RDWR, MODE_0644);
    EXPECT_TRUE(fd > 0);
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);
    ret = chdir(TEST_FILE_PATH);
    EXPECT_EQ(ret, 0);
    ret = fchmodat(AT_FDCWD, TEST_FILE_NAME, MODE_0755, 0);
    EXPECT_EQ(ret, 0);
    ret = fstat(fd, &g_statbuf);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ((g_statbuf.st_mode & S_IXUSR), S_IXUSR);
    EXPECT_EQ((g_statbuf.st_mode & S_IXGRP), S_IXGRP);
    EXPECT_EQ((g_statbuf.st_mode & S_IXOTH), S_IXOTH);

    close(fd);
    remove(TEST_FILE_NAME);
    chdir(path);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FCHMODAT_0400
 * @tc.name FchmodatLinkFileModeFail_0004
 * @tc.desc fchmodat change link file mode bits when symbol link not follow fail, errno EOPNOTSUPP.
*/
HWTEST_F(FchmodatApiTest, FchmodatLinkFileModeFail_0004, Function | MediumTest | Level2)
{
    int ret = -1;
    int fd = open(TEST_FILE, O_CREAT | O_RDWR, MODE_0644);
    EXPECT_TRUE(fd > 0);
    char path[PATH_MAX_LEN];
    char *dir = getcwd(path, sizeof(path));
    EXPECT_NE(dir, nullptr);
    ret = chdir(TEST_FILE_PATH);
    EXPECT_EQ(ret, 0);
    ret = symlink(TEST_FILE_NAME, SYMBOL_LINK_NAME);
    EXPECT_EQ(ret, 0);

    errno = 0;
    ret = fchmodat(AT_FDCWD, SYMBOL_LINK_NAME, MODE_0600, AT_SYMLINK_NOFOLLOW);
    EXPECT_NE(ret, 0);
    EXPECT_EQ(errno, EOPNOTSUPP);

    unlink(TEST_FILE_NAME);
    remove(SYMBOL_LINK_NAME);
    remove(TEST_FILE_NAME);
    chdir(path);
}
