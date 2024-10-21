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
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace testing::ext;
using namespace std;

class OpenatApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

private:
};

static const char *TEST_DIR = "/data/local/tmp/tmp";
static const char *TEST_FILE = "/data/local/tmp/tmp/test";

void OpenatApiTest::SetUpTestCase()
{
}

void OpenatApiTest::TearDownTestCase()
{
}

void OpenatApiTest::SetUp()
{
    mkdir(TEST_DIR, S_IRWXU | S_IRWXG | S_IRWXO);
}

void OpenatApiTest::TearDown()
{
    rmdir(TEST_DIR);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_0100
 * @tc.name   : OpenatReadOnlySuccess_0001
 * @tc.desc   : openat with O_RDONLY should open a file for reading only.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(OpenatApiTest, OpenatReadOnlySuccess_0001, Function | MediumTest | Level1)
{
    int dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);

    int fd = openat(dirFd, TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd >= 0);
    close(fd);

    fd = openat(dirFd, TEST_FILE, O_RDONLY);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    ssize_t bytesWritten = write(fd, "A", 1);
    EXPECT_EQ(bytesWritten, -1);
    EXPECT_EQ(errno, EBADF);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_0200
 * @tc.name   : OpenatReadWriteSuccess_0002
 * @tc.desc   : openat with O_RDWR should open a file for reading and writing.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(OpenatApiTest, OpenatReadWriteSuccess_0002, Function | MediumTest | Level1)
{
    const char *data = "Hello, World!";
    char readBuf[128];
    int dirFd = -1;
    int fd = -1;
    dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);
    EXPECT_TRUE(dirFd >= 0);

    fd = openat(dirFd, TEST_FILE, O_RDWR | O_CREAT, 0644);
    EXPECT_TRUE(fd >= 0);
    write(fd, data, strlen(data));
    lseek(fd, 0, SEEK_SET);
    read(fd, readBuf, sizeof(readBuf));
    EXPECT_STREQ(readBuf, data);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_0300
 * @tc.name   : OpenatAppendSuccess_0003
 * @tc.desc   : openat with O_APPEND should append to the end of the file.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(OpenatApiTest, OpenatAppendSuccess_0003, Function | MediumTest | Level1)
{
    char readBuf[256] = {0};
    const char *data = "Hello, World!";
    const char *appendData = " More data";
    int dirFd = -1;
    int fd = -1;

    dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);

    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd >= 0);
    ssize_t bytesWritten = write(fd, data, strlen(data));
    EXPECT_EQ(bytesWritten, strlen(data));

    close(fd);

    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_APPEND);
    EXPECT_TRUE(fd >= 0);
    bytesWritten = write(fd, appendData, strlen(appendData));
    EXPECT_EQ(bytesWritten, strlen(appendData));

    close(fd);

    fd = openat(dirFd, TEST_FILE, O_RDONLY);
    EXPECT_TRUE(fd >= 0);
    ssize_t bytesRead = read(fd, readBuf, sizeof(readBuf) - 1);
    EXPECT_TRUE(bytesRead > 0);

    readBuf[bytesRead] = '\0';
    std::string expectedContent(data);
    expectedContent += appendData;
    EXPECT_STREQ(readBuf, expectedContent.c_str());

    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_0400
 * @tc.name   : OpenatNoFollowFailed_0004
 * @tc.desc   : openat with O_NOFOLLOW should fail if the path is a symbolic link.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(OpenatApiTest, OpenatNoFollowFailed_0004, Function | MediumTest | Level2)
{
    int dirFd = -1;
    int fd = -1;
    dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);

    errno = 0;
    symlink("target", TEST_FILE);
    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_CREAT | O_NOFOLLOW);
    EXPECT_EQ(fd, -1);
    EXPECT_EQ(errno, ELOOP);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_0500
 * @tc.name   : OpenatWriteOnlySuccess_0005
 * @tc.desc   : openat with O_WRONLY should open a file for writing only.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(OpenatApiTest, OpenatWriteOnlySuccess_0005, Function | MediumTest | Level1)
{
    int dirFd;
    int fd;
    char buf;
    ssize_t bytesRead;
    ssize_t bytesWritten;
    const char *data;


    dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);
    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd >= 0);

    bytesRead = read(fd, &buf, 1);
    EXPECT_EQ(bytesRead, -1);
    EXPECT_EQ(errno, EBADF);

    data = "Test data";
    bytesWritten = write(fd, data, strlen(data));
    EXPECT_TRUE(bytesWritten == strlen(data));

    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_0600
 * @tc.name   : OpenatCreateTest_0006
 * @tc.desc   : openat with O_CREAT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(OpenatApiTest, OpenatCreateTest_0006, Function | MediumTest | Level2)
{
    int dirFd = -1;
    int fd = -1;
    struct stat buf;
    dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);

    fd = openat(dirFd, TEST_FILE, O_WRONLY, 0644);
    EXPECT_TRUE(fd == -1);
    stat(TEST_FILE, &buf);
    EXPECT_EQ(errno, ENOENT);
    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd >= 0);
    EXPECT_TRUE(stat(TEST_FILE, &buf) == 0);

    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_0700
 * @tc.name   : OpenatDirectoryTest_0007
 * @tc.desc   : openat with O_DIRECTORY.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(OpenatApiTest, OpenatDirectoryTest_0007, Function | MediumTest | Level2)
{
    int dirFd = -1;
    int fd = -1;
    struct stat buf;
    dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);

    fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    close(fd);
    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_DIRECTORY, 0644);
    EXPECT_TRUE(fd == -1);
    stat(TEST_FILE, &buf);
    EXPECT_TRUE(S_ISREG(buf.st_mode));
    unlinkat(dirFd, TEST_FILE, 0);

    mkdir(TEST_FILE, 0644);
    fd = openat(dirFd, TEST_FILE, O_DIRECTORY, 0644);
    EXPECT_TRUE(fd >= 0);
    stat(TEST_FILE, &buf);
    EXPECT_TRUE(S_ISDIR(buf.st_mode));

    close(fd);
    rmdir(TEST_FILE);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_0800
 * @tc.name   : OpenatExclTest_0008
 * @tc.desc   : openat with O_EXCL to prevent overwriting the content of existing files.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(OpenatApiTest, OpenatExclTest_0008, Function | MediumTest | Level2)
{
    int dirFd = -1;
    int fd = -1;
    dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);

    EXPECT_TRUE(access(TEST_FILE, F_OK) == -1);
    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_CREAT | O_EXCL, 0644);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    EXPECT_TRUE(access(TEST_FILE, F_OK) == 0);

    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_CREAT | O_EXCL, 0644);
    EXPECT_TRUE(fd == -1);
    EXPECT_TRUE(access(TEST_FILE, F_OK) == 0);

    unlinkat(dirFd, TEST_FILE, 0);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_0900
 * @tc.name   : OpenatNoatimeSuccess_0009
 * @tc.desc   : openat with O_NOATIME to prevent changing atime but still changing mtime.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(OpenatApiTest, OpenatNoatimeSuccess_0009, Function | MediumTest | Level1)
{
    int dirFd = -1;
    int fd = -1;
    struct stat bufFirst;
    struct stat bufSecond;
    dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);

    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    stat(TEST_FILE, &bufFirst);

    usleep(100);
    fd = openat(dirFd, TEST_FILE, O_WRONLY | O_NOATIME, 0644);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    stat(TEST_FILE, &bufSecond);
    EXPECT_TRUE(bufFirst.st_atime == bufSecond.st_atime);

    unlinkat(dirFd, TEST_FILE, 0);
    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_1000
 * @tc.name   : OpenatFlagsTestSuccess_0010
 * @tc.desc   : openat with some flags.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(OpenatApiTest, OpenatFlagsTestSuccess_0010, Function | MediumTest | Level1)
{
    int dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);

    // O_TRUNC test
    int fd = openat(dirFd, TEST_FILE, O_RDWR | O_CREAT, 0644);
    const char *data = "Test data";
    write(fd, data, strlen(data));
    close(fd);
    fd = openat(dirFd, TEST_FILE, O_RDWR | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);

    // O_ASYNC test
    fd = openat(dirFd, TEST_FILE, O_RDWR | O_CREAT | O_ASYNC, 0644);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);

    // O_DIRECT test
    fd = openat(dirFd, TEST_FILE, O_RDWR | O_CREAT | O_DIRECT, 0644);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);

    // O_DSYNC test
    fd = openat(dirFd, TEST_FILE, O_RDWR | O_CREAT | O_DSYNC, 0644);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);

    // O_SYNC test
    fd = openat(dirFd, TEST_FILE, O_RDWR | O_CREAT | O_SYNC, 0644);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);

    close(dirFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_OPENAT_1100
 * @tc.name   : OpenatFlagsTestSuccess_0011
 * @tc.desc   : openat O_CLOEXEC/O_LARGEFILE/O_NOCTTY/O_NONBLOCK flags test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(OpenatApiTest, OpenatO_CLOEXECFlagSuccess_0011, Function | MediumTest | Level1)
{
    int dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);

    int fd = openat(dirFd, TEST_FILE, O_RDWR | O_CLOEXEC | O_CREAT, 0755);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);

    fd = openat(dirFd, TEST_FILE, O_RDWR | O_LARGEFILE | O_CREAT, 0755);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);

    fd = openat(dirFd, TEST_FILE, O_RDWR | O_NOCTTY | O_CREAT, 0755);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);

    fd = openat(dirFd, TEST_FILE, O_RDWR | O_NONBLOCK | O_CREAT, 0755);
    EXPECT_TRUE(fd >= 0);
    close(fd);
    unlinkat(dirFd, TEST_FILE, 0);

    close(dirFd);
}