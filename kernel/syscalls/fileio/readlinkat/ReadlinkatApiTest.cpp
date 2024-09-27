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

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class ReadlinkatApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

private:
};

static const char *TEST_DIR = "/data/local/tmp/tmp";
static const char *TEST_FILE = "/data/local/tmp/tmp/test";
static const char *TEST_LINK = "/data/local/tmp/tmp/link";

void ReadlinkatApiTest::SetUpTestCase()
{
}

void ReadlinkatApiTest::TearDownTestCase()
{
}

void ReadlinkatApiTest::SetUp()
{
    mkdir(TEST_DIR, S_IRWXU | S_IRWXG | S_IRWXO);
}

void ReadlinkatApiTest::TearDown()
{
    rmdir(TEST_DIR);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READLINKAT_0100
 * @tc.name   : ReadlinkatNormalReadSuccess_0001
 * @tc.desc   : readlinkat should read the target of a symlink in the specified directory.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ReadlinkatApiTest, ReadlinkatNormalReadSuccess_0001, Function | MediumTest | Level1)
{
    int fd = -1;
    char buf[1024] = {0};
    const char *data = "Hello, World!";
    int dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);
    EXPECT_TRUE(dirFd >= 0);

    fd = openat(dirFd, TEST_FILE, O_RDWR | O_CREAT, 0644);
    write(fd, data, strlen(data));
    close(fd);
    symlink(TEST_FILE, TEST_LINK);
    ssize_t bytesRead = readlinkat(dirFd, TEST_LINK, buf, sizeof(buf) - 1);
    EXPECT_GE(bytesRead, 0);
    EXPECT_STREQ(buf, TEST_FILE);

    close(dirFd);
    unlink(TEST_LINK);
    unlink(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READLINKAT_0200
 * @tc.name   : ReadlinkatInvalidFdFailed_0002
 * @tc.desc   : readlinkat use invalid fd, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ReadlinkatApiTest, ReadlinkatInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    int fd = -1;
    char buf[1024] = {0};
    int dirFd = open(TEST_DIR, O_RDONLY | O_DIRECTORY);
    fd = openat(dirFd, TEST_FILE, O_RDWR | O_CREAT, 0644);
    errno = 0;
    ssize_t bytesWritten = readlinkat(-1, TEST_FILE, buf, sizeof(buf) - 1);
    EXPECT_EQ(bytesWritten, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
    close(dirFd);
    unlink(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READLINKAT_0300
 * @tc.name   : ReadlinkatNonexistentLinkFailed_0003
 * @tc.desc   : readlinkat symlink which is not exist, errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ReadlinkatApiTest, ReadlinkatNonexistentLinkFailed_0003, Function | MediumTest | Level2)
{
    char buf[1024] = {0};
    int dirFd = open(TEST_DIR, O_RDONLY);
    unlinkat(dirFd, TEST_LINK, 0);

    errno = 0;
    ssize_t bytesRead = readlinkat(dirFd, TEST_LINK, buf, sizeof(buf) - 1);
    EXPECT_EQ(bytesRead, -1);
    EXPECT_EQ(errno, ENOENT);

    close(dirFd);
}
