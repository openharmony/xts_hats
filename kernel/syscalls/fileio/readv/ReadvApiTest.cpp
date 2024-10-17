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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "securec.h"


using namespace testing::ext;
using namespace std;

static const char *TEST_FILE = "/data/local/tmp/readv_test_file.txt";
static const char *TEST_DATA = "Hello World!";
static const int TEST_DATA_LEN = strlen(TEST_DATA);
static const int MAX_LEN = 128;

class ReadvApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ReadvApiTest::SetUp()
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    write(fd, TEST_DATA, TEST_DATA_LEN);
    close(fd);
}
void ReadvApiTest::TearDown()
{
    (void)remove(TEST_FILE);
}
void ReadvApiTest::SetUpTestCase()
{
}
void ReadvApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READV_0100
 * @tc.name   : ReadvFileSuccess_0001
 * @tc.desc   : readv read file Success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ReadvApiTest, ReadvReadEmptyFileSuccess_0001, Function | MediumTest | Level1)
{
    char buffer[MAX_LEN] = { 0 };
    struct iovec iov[2] = {
        {
            .iov_base = buffer,
            .iov_len = TEST_DATA_LEN,
        }, {
            .iov_base = &buffer[TEST_DATA_LEN],
            .iov_len = TEST_DATA_LEN,
        }
    };

    int fd = open(TEST_FILE, O_RDONLY, 0644);
    EXPECT_TRUE(fd > 0);

    ssize_t size = readv(fd, iov, 2);
    EXPECT_EQ(size, TEST_DATA_LEN);
    EXPECT_STREQ(static_cast<char *>(iov[0].iov_base), TEST_DATA);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READV_0200
 * @tc.name   : ReadvInvalidFdFailed_0002
 * @tc.desc   : readv read with invalid fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ReadvApiTest, ReadvInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    char buffer[MAX_LEN] = { 0 };
    struct iovec iov[2] = {
        {
            .iov_base = buffer,
            .iov_len = TEST_DATA_LEN,
        }, {
            .iov_base = &buffer[TEST_DATA_LEN],
            .iov_len = TEST_DATA_LEN,
        }
    };

    errno = 0;
    ssize_t size = readv(-1, iov, 2);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READV_0300
 * @tc.name   : ReadvReadUseTwoBufferSuccess_0003
 * @tc.desc   : readv file use two iov buffer success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ReadvApiTest, ReadvReadUseTwoBufferSuccess_0003, Function | MediumTest | Level1)
{
    ssize_t size;
    const char *content = "this is a test file.";
    char buffer[MAX_LEN] = { 0 };
    struct iovec iov[2] = {
        {
            .iov_base = buffer,
            .iov_len = TEST_DATA_LEN,
        }, {
            .iov_base = &buffer[MAX_LEN / 2],
            .iov_len = strlen(content),
        }
    };

    int fd = open(TEST_FILE, O_RDWR | O_APPEND, 0644);
    write(fd, content, strlen(content));
    close(fd);

    fd = open(TEST_FILE, O_RDWR, 0644);

    // readv content to 2 iovec
    size = readv(fd, iov, 2);
    EXPECT_EQ(size, TEST_DATA_LEN + strlen(content));
    EXPECT_STREQ(static_cast<char *>(iov[0].iov_base), TEST_DATA);
    EXPECT_STREQ(static_cast<char *>(iov[1].iov_base), content);

    close(fd);
}