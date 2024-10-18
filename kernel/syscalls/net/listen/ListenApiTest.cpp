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

class HatsListenTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsListenTest::SetUp()
{
}
void HatsListenTest::TearDown()
{
}
void HatsListenTest::SetUpTestCase()
{
}
void HatsListenTest::TearDownTestCase()
{
}

static const int BAD_SOCKET_FD = -1;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LISTEN_0100
 * @tc.name   : ListenOnUnbindFdSuccess_0001
 * @tc.desc   : listen on the socket fd which does not bind ip and port success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsListenTest, ListenOnUnbindFdSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int backLog = 5;
    int socketFd = -1;

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    ret = listen(socketFd, backLog);
    EXPECT_EQ(ret, 0);

    close(socketFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LISTEN_0200
 * @tc.name   : ListenInvalidSocketFdFailed_0002
 * @tc.desc   : listen on the invalid socket fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsListenTest, ListenInvalidSocketFdFailed_0002, Function | MediumTest | Level2)
{
    int ret;
    int32_t backLog = 5;

    errno = 0;
    ret = listen(BAD_SOCKET_FD, backLog);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    ret = listen(STDIN_FILENO, backLog);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTSOCK);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LISTEN_0300
 * @tc.name   : UnsuportListenSocketFdFailed_0003
 * @tc.desc   : listen on the socket fd which does not support listen failed, errno EOPNOTSUPP.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsListenTest, UnsuportListenSocketFdFailed_0003, Function | MediumTest | Level2)
{
    int ret;
    int socketFd = -1;
    int32_t backLog = 5;

    socketFd = socket(PF_INET, SOCK_DGRAM, 0);
    EXPECT_TRUE(socketFd > 0);

    errno = 0;
    ret = listen(socketFd, backLog);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EOPNOTSUPP);

    close(socketFd);
}
