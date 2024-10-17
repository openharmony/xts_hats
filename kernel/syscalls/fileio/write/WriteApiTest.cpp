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

class HatsWriteTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsWriteTest::SetUp()
{
}
void HatsWriteTest::TearDown()
{
}
void HatsWriteTest::SetUpTestCase()
{
}
void HatsWriteTest::TearDownTestCase()
{
}

static const char *WRITE_TEST_FILE = "/data/local/tmp/tryWrite.txt";
static const char *TEST_DATA = "Hello write!";
static const int TEST_DATA_LEN = strlen(TEST_DATA);

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITE_0100
 * @tc.name   : WriteDataToFileSuccess_0001
 * @tc.desc   : write data to file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWriteTest, WriteDataToFileSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int fd;
    char *buf = nullptr;
    buf = new char[TEST_DATA_LEN + 1];

    fd = open(WRITE_TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);

    ret = write(fd, TEST_DATA, TEST_DATA_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN);
    close(fd);

    fd = open(WRITE_TEST_FILE, O_RDONLY);
    EXPECT_TRUE(fd >= 3);
    ret = read(fd, buf, TEST_DATA_LEN);
    close(fd);
    buf[TEST_DATA_LEN] = '\0';
    ret = strcmp(buf, TEST_DATA);
    EXPECT_TRUE(ret == 0);

    remove(WRITE_TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITE_0200
 * @tc.name   : WriteInvalidFdFail_0002
 * @tc.desc   : write to invalid fd fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWriteTest, WriteInvalidFdFail_0002, Function | MediumTest | Level2)
{
    int ret;
    int fd;
    int pipefd[2];

    fd = -1;
    errno = 0;
    ret = write(fd, TEST_DATA, TEST_DATA_LEN);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EBADF);

    ret = pipe(pipefd);
    EXPECT_TRUE(ret == 0);
    errno = 0;
    ret = write(pipefd[0], TEST_DATA, TEST_DATA_LEN);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EBADF);
    close(pipefd[0]);
    close(pipefd[1]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITE_0300
 * @tc.name   : WriteFdIsNotOpenForWriteFail_0003
 * @tc.desc   : write fd is not open for write fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWriteTest, WriteFdIsNotOpenForWriteFail_0003, Function | MediumTest | Level2)
{
    int ret;
    int fd;

    fd = open(WRITE_TEST_FILE, O_RDONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);

    errno = 0;
    ret = write(fd, TEST_DATA, TEST_DATA_LEN);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EBADF);
    close(fd);

    fd = open(WRITE_TEST_FILE, O_APPEND | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);

    errno = 0;
    ret = write(fd, TEST_DATA, TEST_DATA_LEN);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EBADF);
    close(fd);
    remove(WRITE_TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITE_0400
 * @tc.name   : WriteEmptyDataToFileSuccess_0004
 * @tc.desc   : write empty data to file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWriteTest, WriteEmptyDataToFileSuccess_0004, Function | MediumTest | Level1)
{
    int ret;
    int fd;
    
    fd = open(WRITE_TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);

    ret = write(fd, "", 0);
    EXPECT_TRUE(ret == 0);
    close(fd);
    remove(WRITE_TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITE_0500
 * @tc.name   : WriteDataToPipeSuccess_0005
 * @tc.desc   : Write data to pipe success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWriteTest, WriteDataToPipeSuccess_0005, Function | MediumTest | Level1)
{
    int pipefd[2];
    int ret;
    char *buf = nullptr;
    buf = new char[TEST_DATA_LEN + 1];

    ret = pipe(pipefd);
    EXPECT_TRUE(ret == 0);
    ret = write(pipefd[1], TEST_DATA, TEST_DATA_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN);

    ret = read(pipefd[0], buf, TEST_DATA_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN);
    buf[TEST_DATA_LEN] = '\0';
    ret = strcmp(buf, TEST_DATA);
    EXPECT_TRUE(ret == 0);

    close(pipefd[0]);
    close(pipefd[1]);
}
