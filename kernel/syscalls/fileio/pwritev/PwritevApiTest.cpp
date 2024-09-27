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
static const char *TEST_FILE = "/data/local/tmp/testfile";

class PwritevApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void PwritevApiTest::SetUp()
{
}
void PwritevApiTest::TearDown()
{
}
void PwritevApiTest::SetUpTestCase()
{
}
void PwritevApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PWRITEV_0100
 * @tc.name   : PwritevNormalWriteSuccess_0001
 * @tc.desc   : pwritev should write data to the file at the specified offset.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(PwritevApiTest, PwritevNormalWriteSuccess_0001, Function | MediumTest | Level1)
{
    char data1[] = "Hello";
    char data2[] = "World!";
    int fd = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);

    struct iovec iov[2] = {
        {.iov_base = data1, .iov_len = sizeof(data1)},
        {.iov_base = data2, .iov_len = sizeof(data2)}
    };

    ssize_t size = pwritev(fd, iov, 2, 0);
    EXPECT_EQ(size, sizeof(data1) + sizeof(data2));

    close(fd);
    unlink(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PWRITEV_0200
 * @tc.name   : PwritevInvalidFdFailed_0002
 * @tc.desc   : pwritev should return -1 and set errno to EBADF for invalid fd, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(PwritevApiTest, PwritevInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    char data[] = "Hello, world!";
    struct iovec iov = {
        .iov_base = data,
        .iov_len = sizeof(data),
    };
    errno = 0;
    ssize_t size = pwritev(-1, &iov, 1, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
}
