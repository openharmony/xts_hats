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

class HatsAccept4Test : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsAccept4Test::SetUp()
{
}
void HatsAccept4Test::TearDown()
{
}
void HatsAccept4Test::SetUpTestCase()
{
}
void HatsAccept4Test::TearDownTestCase()
{
}

static const int BAD_SOCKET_FD = -1;
static const int TEST_PORT = 8888;
static const char *TEST_LOCAL_IP = "127.0.0.1";

enum AcceptType {
    GET_NONE = 0,
    GET_CLIENT_SOCKET_ADDR_TEST,
    GET_CLIENT_SOCKET_ADDR_LEN_TEST,
};

static void SocketServiceStart(int *fd)
{
    int ret;
    int socketFd = -1;
    int32_t backLog = 2;
    int32_t optVal = 1;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_TRUE(socketFd > 0);

    ret = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    ASSERT_TRUE(ret == 0);

    ret = bind(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    ASSERT_TRUE(ret == 0);

    ret = listen(socketFd, backLog);
    ASSERT_EQ(ret, 0);

    *fd = socketFd;
}

static void ClientConnect(void)
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
    ret = connect(socketFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, 0);
    close(socketFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ACCEPT4_0100
 * @tc.name   : Accept4ValidSockfdSuccess_0001
 * @tc.desc   : accept4 valid sockfd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsAccept4Test, Accept4ValidSockfdSuccess_0001, Function | MediumTest | Level1)
{
    int pid = -1;
    int socketFd = -1;
    int acceptFd = -1;
    int status = 0;
    struct sockaddr_in cliAddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    SocketServiceStart(&socketFd);
    ASSERT_TRUE(socketFd > 0);
    if ((pid = fork()) == 0) {
        ClientConnect();
        exit(0);
    }
    acceptFd = accept4(socketFd, reinterpret_cast<struct sockaddr *>(&cliAddr), &addrlen, 0);
    EXPECT_TRUE(acceptFd > 0);

    close(acceptFd);
    close(socketFd);
    waitpid(pid, &status, 0);
    EXPECT_TRUE(status == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ACCEPT4_0200
 * @tc.name   : Accept4GetClientAddrSuccess_0002
 * @tc.desc   : accept4 get client addr success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsAccept4Test, Accept4GetClientAddrSuccess_0002, Function | MediumTest | Level1)
{
    int pid = -1;
    int acceptFd = -1;
    int socketFd = -1;
    int status = 0;
    struct sockaddr_in cliAddr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    SocketServiceStart(&socketFd);
    ASSERT_TRUE(socketFd > 0);
    if ((pid = fork()) == 0) {
        ClientConnect();
        exit(0);
    }
    acceptFd = accept4(socketFd, reinterpret_cast<struct sockaddr *>(&cliAddr), &addrlen, 0);
    EXPECT_STREQ(inet_ntoa(cliAddr.sin_addr), TEST_LOCAL_IP);
    close(acceptFd);
    close(socketFd);
    waitpid(pid, &status, 0);
    EXPECT_TRUE(status == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ACCEPT4_0300
 * @tc.name   : Accept4GetClientAddrSuccess_0003
 * @tc.desc   : accept4 get client addr len success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsAccept4Test, Accept4GetClientAddrSuccess_0003, Function | MediumTest | Level1)
{
    int pid = -1;
    int acceptFd = -1;
    int socketFd = -1;
    int status = 0;
    struct sockaddr_in cliAddr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    SocketServiceStart(&socketFd);
    ASSERT_TRUE(socketFd > 0);
    if ((pid = fork()) == 0) {
        ClientConnect();
        exit(0);
    }
    acceptFd = accept4(socketFd, reinterpret_cast<struct sockaddr *>(&cliAddr), &addrlen, 0);
    EXPECT_EQ(addrlen, sizeof(struct sockaddr));
    close(acceptFd);
    close(socketFd);
    waitpid(pid, &status, 0);
    EXPECT_TRUE(status == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ACCEPT4_0400
 * @tc.name   : Accept4InvalidFd_0004
 * @tc.desc   : accept4 use invalid socket fd, return -1, and set errno.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsAccept4Test, Accept4InvalidFd_0004, Function | MediumTest | Level2)
{
    int ret = accept4(BAD_SOCKET_FD, nullptr, nullptr, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
    ret = accept4(STDIN_FILENO, nullptr, nullptr, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTSOCK);
}
