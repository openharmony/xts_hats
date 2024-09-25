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

static const int TEST_PORT = 8888;
static const int TEST_BAD_PORT = 8887;
static const int BAD_SOCKET_FD = -1;
static const char *TEST_LOCAL_IP = "127.0.0.1";
static int g_serviceFd = -1;

class HatsConnectTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsConnectTest::SetUp()
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

    g_serviceFd = socketFd;
}
void HatsConnectTest::TearDown()
{
    close(g_serviceFd);
    g_serviceFd = -1;
}
void HatsConnectTest::SetUpTestCase()
{
}
void HatsConnectTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_CONNECT_0100
 * @tc.name ConnectServiceSuccess_0001
 * @tc.desc connect to local service success.
*/
HWTEST_F(HatsConnectTest, ConnectServiceSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int socketFd = -1;
    int32_t backLog = 5;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };
    ret = listen(g_serviceFd, backLog);
    ASSERT_EQ(ret, 0);

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    errno = 0;
    ret = connect(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    close(socketFd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_CONNECT_0200
 * @tc.name ConnectUseInvalidFdFailed_0002
 * @tc.desc connect to service use invalid socket fd failed.
*/
HWTEST_F(HatsConnectTest, ConnectUseInvalidFdFailed_0002, Function | MediumTest | Level1)
{
    int ret;
    int socketFd = -1;
    int32_t backLog = 5;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };
    ret = listen(g_serviceFd, backLog);
    ASSERT_EQ(ret, 0);

    errno = 0;
    ret = connect(BAD_SOCKET_FD, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    ret = connect(STDIN_FILENO, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTSOCK);
    close(socketFd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_CONNECT_0300
 * @tc.name ConnectUseServiceFdFailed_0003
 * @tc.desc client connect use service socket fd, errno EISCONN.
*/
HWTEST_F(HatsConnectTest, ConnectUseServiceFdFailed_0003, Function | MediumTest | Level2)
{
    int ret;
    int32_t backLog = 5;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };
    ret = listen(g_serviceFd, backLog);
    ASSERT_EQ(ret, 0);

    errno = 0;
    ret = connect(g_serviceFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EISCONN);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_CONNECT_0400
 * @tc.name ConnectNoListenFdFailed_0004
 * @tc.desc connect to socket fd which does not listen, errno ECONNREFUSED.
*/
HWTEST_F(HatsConnectTest, ConnectNoListenFdFailed_0004, Function | MediumTest | Level2)
{
    int ret;
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

    errno = 0;
    ret = connect(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ECONNREFUSED);
    close(socketFd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_CONNECT_0500
 * @tc.name ConnectUseNullAddrFailed_0005
 * @tc.desc connect to nullptr addr, errno EFAULT.
*/
HWTEST_F(HatsConnectTest, ConnectUseNullAddrFailed_0005, Function | MediumTest | Level2)
{
    int ret;
    int socketFd = -1;
    int32_t backLog = 5;
    ret = listen(g_serviceFd, backLog);
    ASSERT_EQ(ret, 0);

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    errno = 0;
    ret = connect(socketFd, nullptr, sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    close(socketFd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_CONNECT_0600
 * @tc.name ConnectWithErrorPortFailed_0006
 * @tc.desc connect to service using error port, errno ECONNREFUSED.
*/
HWTEST_F(HatsConnectTest, ConnectWithErrorPortFailed_0006, Function | MediumTest | Level2)
{
    int ret;
    int socketFd = -1;
    int32_t backLog = 5;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_BAD_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };
    ret = listen(g_serviceFd, backLog);
    ASSERT_EQ(ret, 0);

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    errno = 0;
    ret = connect(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ECONNREFUSED);

    close(socketFd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_CONNECT_0700
 * @tc.name ConnectTestAddrLen_0007
 * @tc.desc connect user addr len >= sizeof(serAddr) success, otherwise errno EINVAL.
*/
HWTEST_F(HatsConnectTest, ConnectTestAddrLen_0007, Function | MediumTest | Level2)
{
    int ret;
    int socketFd = -1;
    int32_t backLog = 5;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };
    ret = listen(g_serviceFd, backLog);
    ASSERT_EQ(ret, 0);

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(socketFd > 0);

    errno = 0;
    ret = connect(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in) - 1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    ret = connect(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in) + 1);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(errno, 0);
    close(socketFd);
}
