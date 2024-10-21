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
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "securec.h"

using namespace testing::ext;

class HatsWritevTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsWritevTest::SetUp()
{
}
void HatsWritevTest::TearDown()
{
}
void HatsWritevTest::SetUpTestCase()
{
}
void HatsWritevTest::TearDownTestCase()
{
}

static const char *WRITEV_TEST_FILE = "/data/local/tmp/tryWritev.txt";
static const int VEC_LEN = 2;
static const char *TEST_DATA = "Hello writev";
static const int TEST_DATA_LEN = strlen(TEST_DATA);
static const char *TEST_DATA_BUF1 = "Hello ";
static const int BUF1_LEN = strlen(TEST_DATA_BUF1);
static const char *TEST_DATA_BUF2 = "writev";
static const int BUF2_LEN = strlen(TEST_DATA_BUF2);

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITEV_0100
 * @tc.name   : WritevWriteDataToFileSuccess_0001
 * @tc.desc   : writev writes data to file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWritevTest, WritevWriteDataToFileSuccess_0001, Function | MediumTest | Level1)
{
    int fd;
    struct iovec vec[VEC_LEN];
    int ret;
    char *iovBuf1 = nullptr;
    char *iovBuf2 = nullptr;
    char *readBuf = nullptr;
    iovBuf1 = new char[BUF1_LEN];
    memcpy_s(iovBuf1, BUF1_LEN, TEST_DATA_BUF1, BUF1_LEN);
    iovBuf2 = new char[BUF2_LEN];
    memcpy_s(iovBuf2, BUF2_LEN, TEST_DATA_BUF2, BUF2_LEN);
    vec[0].iov_base = iovBuf1;
    vec[0].iov_len = BUF1_LEN;
    vec[1].iov_base = iovBuf2;
    vec[1].iov_len = BUF2_LEN;
    readBuf = new char[TEST_DATA_LEN + 1];

    fd = open(WRITEV_TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);

    ret = writev(fd, vec, VEC_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN);
    close(fd);

    fd = open(WRITEV_TEST_FILE, O_RDONLY);
    EXPECT_TRUE(fd >= 3);
    ret = read(fd, readBuf, TEST_DATA_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN);
    readBuf[TEST_DATA_LEN] = '\0';
    ret = strcmp(readBuf, TEST_DATA);
    EXPECT_TRUE(ret == 0);
    close(fd);
    delete[] iovBuf1;
    delete[] iovBuf2;
    delete[] readBuf;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITEV_0200
 * @tc.name   : WritevWriteToPipeSuccess_0002
 * @tc.desc   : Writev write data to pipe success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWritevTest, WritevWriteToPipeSuccess_0002, Function | MediumTest | Level1)
{
    int pipefd[2];
    struct iovec vec[VEC_LEN];
    int ret;
    char *iovBuf1 = nullptr;
    char *iovBuf2 = nullptr;
    char *readBuf = nullptr;
    iovBuf1 = new char[BUF1_LEN];
    memcpy_s(iovBuf1, BUF1_LEN, TEST_DATA_BUF1, BUF1_LEN);
    iovBuf2 = new char[BUF2_LEN];
    memcpy_s(iovBuf2, BUF2_LEN, TEST_DATA_BUF2, BUF2_LEN);
    vec[0].iov_base = iovBuf1;
    vec[0].iov_len = BUF1_LEN;
    vec[1].iov_base = iovBuf2;
    vec[1].iov_len = BUF2_LEN;
    readBuf = new char[TEST_DATA_LEN + 1];

    ret = pipe(pipefd);
    EXPECT_TRUE(ret == 0);
    ret = writev(pipefd[1], vec, VEC_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN);

    ret = read(pipefd[0], readBuf, TEST_DATA_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN);
    readBuf[TEST_DATA_LEN] = '\0';
    ret = strcmp(readBuf, TEST_DATA);
    EXPECT_TRUE(ret == 0);

    close(pipefd[0]);
    close(pipefd[1]);
    delete[] iovBuf1;
    delete[] iovBuf2;
    delete[] readBuf;
}


/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITEV_0300
 * @tc.name   : WritevInvalidVecLenFail_0003
 * @tc.desc   : Writv write with invalid iovec length fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWritevTest, WritevInvalidVecLenFail_0003, Function | MediumTest | Level2)
{
    int fd;
    struct iovec vec[VEC_LEN];
    int ret;
    int newVecLen;
    char *iovBuf1 = nullptr;
    char *iovBuf2 = nullptr;
    iovBuf1 = new char[BUF1_LEN];
    ret = memcpy_s(iovBuf1, BUF1_LEN, TEST_DATA_BUF1, BUF1_LEN);
    iovBuf2 = new char[BUF2_LEN];
    ret = memcpy_s(iovBuf2, BUF2_LEN, TEST_DATA_BUF2, BUF2_LEN);
    vec[0].iov_base = iovBuf1;
    vec[0].iov_len = BUF1_LEN;
    vec[1].iov_base = iovBuf2;
    vec[1].iov_len = BUF2_LEN;

    fd = open(WRITEV_TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);

    newVecLen = -1;
    errno = 0;
    ret = writev(fd, vec, newVecLen);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
    delete[] iovBuf1;
    delete[] iovBuf2;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITEV_0400
 * @tc.name   : WritevInvalidFdFail_0004
 * @tc.desc   : writev write data into invalid fd fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWritevTest, WritevInvalidFdFail_0004, Function | MediumTest | Level2)
{
    int fd;
    struct iovec vec[VEC_LEN];
    int ret;
    char *iovBuf1 = nullptr;
    char *iovBuf2 = nullptr;
    iovBuf1 = new char[BUF1_LEN];
    ret = memcpy_s(iovBuf1, BUF1_LEN, TEST_DATA_BUF1, BUF1_LEN);
    iovBuf2 = new char[BUF2_LEN];
    ret = memcpy_s(iovBuf2, BUF2_LEN, TEST_DATA_BUF2, BUF2_LEN);
    vec[0].iov_base = iovBuf1;
    vec[0].iov_len = BUF1_LEN;
    vec[1].iov_base = iovBuf2;
    vec[1].iov_len = BUF2_LEN;

    fd = -1;
    errno = 0;
    ret = writev(fd, vec, VEC_LEN);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EBADF);

    delete[] iovBuf1;
    delete[] iovBuf2;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WRITEV_0500
 * @tc.name   : WritevFdNotInWriteModeFail_0005
 * @tc.desc   : writev fd is not in write mode fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWritevTest, WritevFdNotInWriteModeFail_0005, Function | MediumTest | Level2)
{
    int fd;
    struct iovec vec[VEC_LEN];
    int ret;
    char *iovBuf1 = nullptr;
    char *iovBuf2 = nullptr;
    iovBuf1 = new char[BUF1_LEN];
    ret = memcpy_s(iovBuf1, BUF1_LEN, TEST_DATA_BUF1, BUF1_LEN);
    iovBuf2 = new char[BUF2_LEN];
    ret = memcpy_s(iovBuf2, BUF2_LEN, TEST_DATA_BUF2, BUF2_LEN);
    vec[0].iov_base = iovBuf1;
    vec[0].iov_len = BUF1_LEN;
    vec[1].iov_base = iovBuf2;
    vec[1].iov_len = BUF2_LEN;

    fd = open(WRITEV_TEST_FILE, O_RDONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);
    errno = 0;
    ret = writev(fd, vec, VEC_LEN);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EBADF);
    close(fd);

    fd = open(WRITEV_TEST_FILE, O_APPEND | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);
    errno = 0;
    ret = writev(fd, vec, VEC_LEN);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EBADF);
    close(fd);

    delete[] iovBuf1;
    delete[] iovBuf2;
}
