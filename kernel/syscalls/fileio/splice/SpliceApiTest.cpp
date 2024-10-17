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
#include "securec.h"

using namespace testing::ext;

static const char *TEST_READ_FILE = "/data/local/tmp/splice_read_file.txt";
static const char *TEST_WRITE_FILE = "/data/local/tmp/splice_write_file.txt";
static const char *TEST_DATA = "Hello World!";
static const int TEST_DATA_LEN = strlen(TEST_DATA);
static const int MAX_LEN = 128;

class HatsSpliceTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsSpliceTest::SetUp()
{
    int fd = open(TEST_READ_FILE, O_WRONLY | O_CREAT, 0644);
    write(fd, TEST_DATA, TEST_DATA_LEN);
    close(fd);
}
void HatsSpliceTest::TearDown()
{
    (void)remove(TEST_READ_FILE);
    (void)remove(TEST_WRITE_FILE);
}
void HatsSpliceTest::SetUpTestCase()
{
}
void HatsSpliceTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SPLICE_0100
 * @tc.name   : SpliceMoveFileDataToPipeSuccess_0001
 * @tc.desc   : Splice move data from file to pipe success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSpliceTest, SpliceMoveFileDataToPipeSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    char buffer[MAX_LEN] = { 0 };
    int pipeFd[2];
    int fdIn = open(TEST_READ_FILE, O_RDONLY);
    EXPECT_TRUE(fdIn > 0);

    // move common fd data to pipe success.
    ret = pipe(pipeFd);
    EXPECT_EQ(ret, 0);

    ssize_t size = splice(fdIn, nullptr, pipeFd[1], nullptr, TEST_DATA_LEN, SPLICE_F_MOVE | SPLICE_F_MORE);
    EXPECT_EQ(size, TEST_DATA_LEN);

    size = read(pipeFd[0], buffer, MAX_LEN);
    EXPECT_EQ(size, TEST_DATA_LEN);
    EXPECT_STREQ(buffer, TEST_DATA);

    close(fdIn);
    close(pipeFd[0]);
    close(pipeFd[1]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SPLICE_0200
 * @tc.name   : SpliceMovePipeDataToFileSuccess_0002
 * @tc.desc   : Splice move pipe data to file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSpliceTest, SpliceMoveFileDataToPipeSuccess_0002, Function | MediumTest | Level1)
{
    int ret;
    off64_t offset = 0;
    char buffer[MAX_LEN] = { 0 };
    int pipeFd[2];
    int fdOut = open(TEST_WRITE_FILE, O_RDWR | O_CREAT, 0644);
    EXPECT_TRUE(fdOut > 0);

    // move common fd data to pipe success.
    ret = pipe(pipeFd);
    EXPECT_EQ(ret, 0);
    ssize_t size = write(pipeFd[1], TEST_DATA, TEST_DATA_LEN);
    EXPECT_EQ(size, TEST_DATA_LEN);

    ret = splice(pipeFd[0], nullptr, fdOut, &offset, TEST_DATA_LEN, SPLICE_F_MOVE | SPLICE_F_MORE);
    EXPECT_EQ(ret, TEST_DATA_LEN);

    off_t pos = lseek(fdOut, 0, SEEK_SET);
    EXPECT_EQ(pos, 0);

    size = read(fdOut, buffer, MAX_LEN);
    EXPECT_EQ(size, TEST_DATA_LEN);
    EXPECT_STREQ(buffer, TEST_DATA);

    close(fdOut);
    close(pipeFd[0]);
    close(pipeFd[1]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SPLICE_0300
 * @tc.name   : SpliceInvalidFdFailed_0003
 * @tc.desc   : Splice was provided invalid file fd return failed, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSpliceTest, SpliceInvalidFdFailed_0003, Function | MediumTest | Level2)
{
    int ret;
    off64_t offset = 0;
    int pipeFd[2];

    ret = pipe(pipeFd);
    EXPECT_EQ(ret, 0);
    ssize_t size = write(pipeFd[1], TEST_DATA, TEST_DATA_LEN);
    EXPECT_EQ(size, TEST_DATA_LEN);

    // fdOut is invalid fd, failed
    errno = 0;
    ret = splice(pipeFd[0], nullptr, -1, &offset, TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    // fdIn is invalid fd, failed
    errno = 0;
    ret = splice(-1, &offset, pipeFd[1], nullptr, TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    close(pipeFd[0]);
    close(pipeFd[1]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SPLICE_0400
 * @tc.name   : SpliceNoPipeFdFailed_0004
 * @tc.desc   : Splice neither fds refer to a pipe return failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSpliceTest, SpliceNoPipeFdFailed_0004, Function | MediumTest | Level2)
{
    int ret;
    off64_t offset = 0;
    int fdIn = open(TEST_READ_FILE, O_RDONLY);
    EXPECT_TRUE(fdIn > 0);
    int fdOut = open(TEST_WRITE_FILE, O_RDWR | O_TRUNC | O_CREAT, 0644);
    EXPECT_TRUE(fdOut > 0);
    errno = 0;
    ret = splice(fdIn, &offset, fdOut, &offset, TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fdIn);
    close(fdOut);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SPLICE_0500
 * @tc.name   : SplicePipeOffsetNotNullFailed_0005
 * @tc.desc   : Splice pipeFd offset is not nullptr return failed, errno ESPIPE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSpliceTest, SplicePipeOffsetNotNullFailed_0005, Function | MediumTest | Level2)
{
    int ret;
    off64_t offset = 0;
    int pipeFd[2];
    int fdIn = open(TEST_READ_FILE, O_RDONLY);
    EXPECT_TRUE(fdIn > 0);

    ret = pipe(pipeFd);
    EXPECT_EQ(ret, 0);

    errno = 0;
    ret = splice(fdIn, nullptr, pipeFd[1], &offset, TEST_DATA_LEN, SPLICE_F_MOVE | SPLICE_F_MORE);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESPIPE);

    close(fdIn);
    close(pipeFd[0]);
    close(pipeFd[1]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SPLICE_0600
 * @tc.name   : SpliceFdOpenAsAppendFailed_0006
 * @tc.desc   : Splice file fd was opened in append mode failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSpliceTest, SpliceFdOpenAsAppendFailed_0006, Function | MediumTest | Level2)
{
    int ret;
    int pipeFd[2];
    off64_t offset = 0;

    int fd = open(TEST_WRITE_FILE, O_RDWR | O_APPEND | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);

    ret = pipe(pipeFd);
    EXPECT_EQ(ret, 0);
    ssize_t size = write(pipeFd[1], TEST_DATA, TEST_DATA_LEN);
    EXPECT_EQ(size, TEST_DATA_LEN);

    errno = 0;
    ret = splice(pipeFd[0], nullptr, fd, &offset, TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
    close(pipeFd[0]);
    close(pipeFd[1]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SPLICE_0700
 * @tc.name   : SpliceUseTheSamePipeFdFailed_0007
 * @tc.desc   : Splice fdIn and fdOut from the same pipe failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSpliceTest, SpliceUseTheSamePipeFdFailed_0007, Function | MediumTest | Level2)
{
    int ret;
    int pipeFd[2];

    ret = pipe(pipeFd);
    EXPECT_EQ(ret, 0);
    ssize_t size = write(pipeFd[1], TEST_DATA, TEST_DATA_LEN);
    EXPECT_EQ(size, TEST_DATA_LEN);

    errno = 0;
    ret = splice(pipeFd[0], nullptr, pipeFd[1], nullptr, TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(pipeFd[0]);
    close(pipeFd[1]);
}