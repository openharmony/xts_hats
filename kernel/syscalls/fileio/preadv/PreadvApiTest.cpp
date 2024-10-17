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

#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class PreadvApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

private:
};

static const char* TEST_DATA = "Hello, world!";
static const size_t TEST_LEN = strlen(TEST_DATA);
static const int MAX_LEN = 128;
static const char* TEST_FILE = "/data/local/tmp/test_file.txt";
static const char* EMPTY_FILE = "/data/local/tmp/empty_file.txt";

void PreadvApiTest::SetUpTestCase()
{
}

void PreadvApiTest::TearDownTestCase()
{
}

void PreadvApiTest::SetUp()
{
    int fd = open(TEST_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    write(fd, TEST_DATA, TEST_LEN);
    close(fd);
}

void PreadvApiTest::TearDown()
{
    unlink(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PREADV_0100
 * @tc.name   : PreadvReadSuccess_0001
 * @tc.desc   : Test basic read functionality of preadv.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(PreadvApiTest, PreadvReadSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t size;
    int midLen = TEST_LEN / 2;
    char buffer[MAX_LEN] = { 0 };
    struct iovec iov[2] = {
        {
            .iov_base = buffer,
            .iov_len = TEST_LEN,
        }, {
            .iov_base = &buffer[TEST_LEN],
            .iov_len = TEST_LEN,
        }
    };

    int fd = open(TEST_FILE, O_RDONLY);

    // preadv from file start
    size = preadv(fd, iov, 2, 0);
    EXPECT_EQ(size, TEST_LEN);
    EXPECT_STREQ(static_cast<char *>(iov[0].iov_base), TEST_DATA);

    // preadv from file middle
    memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
    size = preadv(fd, iov, 1, midLen);
    EXPECT_EQ(size, TEST_LEN - midLen);
    EXPECT_STREQ(static_cast<char *>(iov[0].iov_base), &TEST_DATA[midLen]);

    // preadv from file end
    memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
    size = preadv(fd, iov, 1, TEST_LEN);
    EXPECT_EQ(size, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PREADV_0200
 * @tc.name   : PreadvInvalidFdFailed_0002
 * @tc.desc   : Test using an invalid file descriptor with preadv.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(PreadvApiTest, PreadvInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    ssize_t size;
    char buffer[MAX_LEN] = { 0 };
    struct iovec iov = {
        .iov_base = buffer,
        .iov_len = sizeof(buffer),
    };
    errno = 0;
    size = preadv(-1, &iov, 1, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PREADV_0300
 * @tc.name   : PreadvReadFromEmptyFileSuccess_0003
 * @tc.desc   : Test reading from an empty file with preadv.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(PreadvApiTest, PreadvReadFromEmptyFileSuccess_0003, Function | MediumTest | Level1)
{
    ssize_t size;
    char buffer[MAX_LEN] = { 0 };
    struct iovec iov = {
        .iov_base = buffer,
        .iov_len = sizeof(buffer),
    };
    int fd = open(EMPTY_FILE, O_CREAT | O_RDWR, 0644);
    size = preadv(fd, &iov, 1, 0);
    EXPECT_EQ(size, 0);
    close(fd);
    unlink(EMPTY_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PREADV_0400
 * @tc.name   : PreadvReadContinuousSuccess_0004
 * @tc.desc   : Test reading from a file with preadv continuously.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(PreadvApiTest, PreadvReadContinuousSuccess_0004, Function | MediumTest | Level1)
{
    ssize_t size;
    const char *context = "this is a test file.";
    char buffer[MAX_LEN] = { 0 };
    struct iovec iov[2] = {
        {
            .iov_base = buffer,
            .iov_len = TEST_LEN,
        }, {
            .iov_base = &buffer[MAX_LEN / 2],
            .iov_len = strlen(context),
        }
    };

    int fd = open(TEST_FILE, O_RDWR | O_APPEND, 0644);
    write(fd, context, strlen(context));

    // preadv context to 2 iovec
    size = preadv(fd, iov, 2, 0);
    EXPECT_EQ(size, TEST_LEN + strlen(context));
    EXPECT_STREQ(static_cast<char *>(iov[0].iov_base), TEST_DATA);
    EXPECT_STREQ(static_cast<char *>(iov[1].iov_base), context);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PREADV2_0500
 * @tc.name   : Preadv2ReadSuccess_0002
 * @tc.desc   : preadv2 read file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(PreadvApiTest, Preadv2ReadSuccess_0002, Function | MediumTest | Level1)
{
    ssize_t size;
    int midLen = TEST_LEN / 2;
    char buffer[MAX_LEN] = { 0 };
    struct iovec iov[2] = {
        {
            .iov_base = buffer,
            .iov_len = TEST_LEN,
        }, {
            .iov_base = &buffer[TEST_LEN],
            .iov_len = TEST_LEN,
        }
    };

    int fd = open(TEST_FILE, O_RDONLY);

    // preadv from file start
    size = preadv(fd, iov, 2, 0);
    EXPECT_EQ(size, TEST_LEN);
    EXPECT_STREQ(static_cast<char *>(iov[0].iov_base), TEST_DATA);

    // preadv from file middle
    memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
    size = preadv2(fd, iov, 1, midLen, 0);
    EXPECT_EQ(size, TEST_LEN - midLen);
    EXPECT_STREQ(static_cast<char *>(iov[0].iov_base), &TEST_DATA[midLen]);

    // preadv from file end
    memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
    size = preadv2(fd, iov, 1, TEST_LEN, 0);
    EXPECT_EQ(size, 0);

    close(fd);
}
