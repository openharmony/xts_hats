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

static const int MAX_LEN = 128;
static const char *TEST_FILE = "/data/local/tmp/test_file.txt";

class Pwritev64ApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void Pwritev64ApiTest::SetUp()
{
}
void Pwritev64ApiTest::TearDown()
{
}
void Pwritev64ApiTest::SetUpTestCase()
{
}
void Pwritev64ApiTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_PWRITEV64_0100
 * @tc.name Pwritev64NormalWriteSuccess_0001
 * @tc.desc pwritev64 should write data to the file at the specified offset.
 */
HWTEST_F(Pwritev64ApiTest, Pwritev64NormalWriteSuccess_0001, Function | MediumTest | Level1)
{
    char data1[] = "Hello";
    char data2[] = "World!";
    struct iovec iov[2] = {
        {
            .iov_base = data1,
            .iov_len = sizeof(data1),
        }, {
            .iov_base = data2,
            .iov_len = sizeof(data2),
        }
    };

    int fd = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    ssize_t size = pwritev64(fd, iov, 2, 0);
    EXPECT_EQ(size, sizeof(data1) + sizeof(data2));

    close(fd);
    unlink(TEST_FILE);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_PWRITEV64_0200
 * @tc.name Pwritev64InvalidFdFailed_0002
 * @tc.desc pwritev64 should return -1 and set errno to EBADF for invalid fd.
 */
HWTEST_F(Pwritev64ApiTest, Pwritev64InvalidFdFailed_0002, Function | MediumTest | Level2)
{
    char buffer[MAX_LEN] = { 0 };
    struct iovec iov = {
        .iov_base = buffer,
        .iov_len = sizeof(buffer),
    };
    errno = 0;
    ssize_t size = pwritev64(-1, &iov, 1, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
}
