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

const char *TEST_DATA = "Hello tee!";
const int TEST_DATA_LEN = strlen(TEST_DATA);
static const int BUFFER_MAX_LEN = 128;

class HatsTeeTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsTeeTest::SetUp()
{
}
void HatsTeeTest::TearDown()
{
}
void HatsTeeTest::SetUpTestCase()
{
}
void HatsTeeTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TEE_0100
 * @tc.name   : TeeCopyDataSuccess_0001
 * @tc.desc   : Tee copys data from pipe1 to pipe2 success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsTeeTest, TeeCopyDataSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int pipeFd1[2];
    int pipeFd2[2];
    char buffer[BUFFER_MAX_LEN] = { 0 };

    ret = pipe(pipeFd1);
    EXPECT_EQ(ret, 0);
    ret = pipe(pipeFd2);
    EXPECT_EQ(ret, 0);

    ssize_t size = write(pipeFd1[1], TEST_DATA, TEST_DATA_LEN);
    EXPECT_EQ(size, TEST_DATA_LEN);

    size = tee(pipeFd1[0], pipeFd2[1], TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(size, TEST_DATA_LEN);

    size = read(pipeFd2[0], buffer, sizeof(buffer));
    EXPECT_EQ(size, TEST_DATA_LEN);
    EXPECT_STREQ(buffer, TEST_DATA);

    close(pipeFd1[0]);
    close(pipeFd1[1]);
    close(pipeFd2[0]);
    close(pipeFd2[1]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TEE_0200
 * @tc.name   : TeeTestFileFdFail_0002
 * @tc.desc   : Tee fd is file fd instead of pipe fd fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsTeeTest, TeeTestFileFdFail_0002, Function | MediumTest | Level2)
{
    int ret;
    int fd1;
    int fd2;
    int pipeFd[2];

    const char *teeFile1 = "/data/local/tmp/tryTee1.txt";
    const char *teeFile2 = "/data/local/tmp/tryTee2.txt";

    fd1 = open(teeFile1, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd1 >= 0);
    fd2 = open(teeFile2, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd2 >= 0);
    ret = pipe(pipeFd);
    EXPECT_EQ(ret, 0);

    errno = 0;
    ssize_t size = tee(fd1, fd2, TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    size = tee(fd1, pipeFd[1], TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    size = tee(pipeFd[0], fd2, TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EINVAL);

    close(pipeFd[0]);
    close(pipeFd[1]);

    close(fd1);
    close(fd2);
    unlink(teeFile1);
    unlink(teeFile2);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TEE_0300
 * @tc.name   : TeeFdsReferToSamePipeFail_0003
 * @tc.desc   : Tee fds refer to same pipe fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsTeeTest, TeeFdsReferToSamePipeFail_0003, Function | MediumTest | Level2)
{
    int ret;
    int pipeFd[2];

    ret = pipe(pipeFd);
    EXPECT_TRUE(ret == 0);

    errno = 0;
    ssize_t size = tee(pipeFd[0], pipeFd[1], TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EINVAL);

    close(pipeFd[0]);
    close(pipeFd[1]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TEE_0400
 * @tc.name   : TeeInvalidFdFail_0004
 * @tc.desc   : Tee fd is not valid fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsTeeTest, TeeInvalidFdFail_0004, Function | MediumTest | Level2)
{
    errno = 0;
    ssize_t size = tee(-1, -1, TEST_DATA_LEN, SPLICE_F_NONBLOCK);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
}
