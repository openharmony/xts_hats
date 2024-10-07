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
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *TEST_FILE = "/data/local/tmp/read_test.txt";
static const char *TEST_DATA = "Hello World!";
static const int TEST_LEN = strlen(TEST_DATA);
static const int MAX_LEN = 128;

class ReadApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ReadApiTest::SetUp()
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    write(fd, TEST_DATA, strlen(TEST_DATA));
    close(fd);
}
void ReadApiTest::TearDown()
{
    (void)remove(TEST_FILE);
}
void ReadApiTest::SetUpTestCase()
{
}
void ReadApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READ_0100
 * @tc.name   : ReadFileSuccess_0001
 * @tc.desc   : read file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ReadApiTest, ReadFileSuccess_0001, Function | MediumTest | Level1)
{
    char buffer[MAX_LEN] = { 0 };
    int fd = open(TEST_FILE, O_RDONLY);

    ssize_t size = read(fd, nullptr, 0);
    EXPECT_EQ(size, 0);
    size = read(fd, buffer, MAX_LEN);
    EXPECT_EQ(size, TEST_LEN);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READ_0200
 * @tc.name   : ReadUseInvalidParamFailed_0002
 * @tc.desc   : read use invalid param failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ReadApiTest, ReadUseInvalidParamFailed_0002, Function | MediumTest | Level2)
{
    char buffer[MAX_LEN] = { 0 };
    errno = 0;
    ssize_t size = read(-1, buffer, MAX_LEN);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READ_0300
 * @tc.name   : ReadUseErrorPermissionFdFailed_0003
 * @tc.desc   : read use write only fd failed, errno .
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ReadApiTest, ReadUseErrorPermissionFdFailed_0003, Function | MediumTest | Level2)
{
    char buffer[MAX_LEN] = { 0 };
    int fd = open(TEST_FILE, O_WRONLY);

    ssize_t size = read(fd, buffer, MAX_LEN);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);

    close(fd);
}
