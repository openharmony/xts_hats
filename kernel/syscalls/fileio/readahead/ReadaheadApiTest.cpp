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
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace testing::ext;

static const char *TEST_FILE = "/data/local/tmp/read_head_test.txt";
static const char *TEST_DATA = "Hello world!";
static const int TEST_DATA_LEN = strlen(TEST_DATA);

class HatsReadaheadTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsReadaheadTest::SetUp()
{
}
void HatsReadaheadTest::TearDown()
{
}
void HatsReadaheadTest::SetUpTestCase()
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    write(fd, TEST_DATA, TEST_DATA_LEN);
    close(fd);
}
void HatsReadaheadTest::TearDownTestCase()
{
    (void)remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READAHEAD_0100
 * @tc.name   : ReadaheadFileSuccess_0001
 * @tc.desc   : Readahead on a valid fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsReadaheadTest, ReadaheadFileSuccess_0001, Function | MediumTest | Level1)
{
    int fd = open(TEST_FILE, O_RDONLY);
    EXPECT_TRUE(fd > 0);
    ssize_t size = readahead(fd, 0, TEST_DATA_LEN);
    EXPECT_EQ(size, 0);

    size = readahead(fd, TEST_DATA_LEN, TEST_DATA_LEN);
    EXPECT_EQ(size, 0);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_READAHEAD_0200
 * @tc.name   : ReadaheadInvalidFdFailed_0002
 * @tc.desc   : Readahead read with invalid fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsReadaheadTest, ReadaheadInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    ssize_t size = readahead(-1, 0, TEST_DATA_LEN);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);

    int fd = open(TEST_FILE, O_WRONLY);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    size = readahead(fd, 0, TEST_DATA_LEN);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
}
