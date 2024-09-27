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
#include "securec.h"

using namespace testing::ext;

class HatsBindTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsBindTest::SetUp()
{
}
void HatsBindTest::TearDown()
{
}
void HatsBindTest::SetUpTestCase()
{
}
void HatsBindTest::TearDownTestCase()
{
}

static const int BAD_SOCKET_FD = -1;
static const int TEST_PORT = 8888;
static const char *TEST_LOCAL_IP = "127.0.0.1";
static const char *TEST_BAD_IP = "192.168.10.3";

/*
 * @tc.number : SUB_KERNEL_SYSCALL_BIND_0100
 * @tc.name   : BindSockfdTwice_0001
 * @tc.desc   : bind the same socket fd twice failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsBindTest, BindSockfdTwice_0001, Function | MediumTest | Level2)
{
    int ret;
    int socketFd = -1;
    int32_t optVal = 1;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    ret = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    EXPECT_EQ(ret, 0);

    ret = bind(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_EQ(ret, 0);

    errno = 0;
    ret = bind(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(socketFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_BIND_0200
 * @tc.name   : BindInvalidSockfdFailed_0002
 * @tc.desc   : bind the invalid socket fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsBindTest, BindInvalidSockfdFailed_0002, Function | MediumTest | Level2)
{
    int ret;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    errno = 0;
    ret = bind(BAD_SOCKET_FD, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    ret = bind(STDIN_FILENO, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTSOCK);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_BIND_0300
 * @tc.name   : BindInvalidIPFailed_0003
 * @tc.desc   : bind the invalid ip addr failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsBindTest, BindInvalidIPFailed_0003, Function | MediumTest | Level2)
{
    int ret;
    int32_t optVal = 1;
    int socketFd = -1;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_BAD_IP),
        }
    };

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    ret = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    EXPECT_EQ(ret, 0);

    errno = 0;
    ret = bind(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EADDRNOTAVAIL);

    errno = 0;
    ret = bind(socketFd, nullptr, sizeof(serAddr));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);

    close(socketFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_BIND_0400
 * @tc.name   : BindErrorIPAddrLen_0004
 * @tc.desc   : bind addr len >= sizeof(serAddr) success, otherwise errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsBindTest, BindErrorIPAddrLen_0004, Function | MediumTest | Level2)
{
    int ret;
    int32_t optVal = 1;
    int socketFd = -1;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    ret = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    EXPECT_EQ(ret, 0);

    errno = 0;
    ret = bind(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), -1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    ret = bind(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr) - 1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    ret = bind(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr) + 1);
    EXPECT_EQ(ret, 0);

    close(socketFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_BIND_0500
 * @tc.name   : BindUsedPortFailed_0005
 * @tc.desc   : bind the used port, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsBindTest, BindUsedPortFailed_0005, Function | MediumTest | Level2)
{
    int ret;
    int socketFd = -1;
    int socketFd2 = -1;
    int32_t optVal = 1;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    ret = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    EXPECT_EQ(ret, 0);

    errno = 0;
    ret = bind(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);

    socketFd2 = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    errno = 0;
    ret = bind(socketFd2, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EADDRINUSE);

    close(socketFd);
    close(socketFd2);
}
