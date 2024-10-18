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
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

static const int BAD_SOCKET_FD = -1;
static const int BUFFER_SIZE = 1024;
static const int TEST_PORT = 22354;
static const char *TEST_LOCAL_IP = "127.0.0.1";
static const char *TEST_EXIT = "exit";
static int g_serviceFd = -1;

class HatsGetInfoTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsGetInfoTest::SetUp()
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
void HatsGetInfoTest::TearDown()
{
    close(g_serviceFd);
    g_serviceFd = -1;
}
void HatsGetInfoTest::SetUpTestCase()
{
}
void HatsGetInfoTest::TearDownTestCase()
{
}

static void *ClientConnect(void *args)
{
    int ret;
    ssize_t size;
    int clientFd = -1;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    clientFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(clientFd > 0);

    ret = connect(clientFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, 0);
    size = recv(clientFd, static_cast<void *>(buffer), BUFFER_SIZE, 0);
    EXPECT_EQ(size, strlen(TEST_EXIT));
    EXPECT_STREQ(buffer, TEST_EXIT);

    close(clientFd);
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETPEERNAME_0100
 * @tc.name   : GetpeernameValidSockfdSuccess_0001
 * @tc.desc   : getpeername valid sockfd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsGetInfoTest, GetpeernameValidSockfdSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    ssize_t size;
    pthread_t thread;
    int acceptFd = -1;
    int32_t backLog = 5;
    struct sockaddr_in dstAddr = { 0 };
    struct sockaddr_in peerAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);
    socklen_t peerLen = sizeof(struct sockaddr_in);

    ret = listen(g_serviceFd, backLog);
    EXPECT_EQ(ret, 0);

    pthread_create(&thread, nullptr, ClientConnect, nullptr);

    acceptFd = accept4(g_serviceFd, reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen, 0);
    EXPECT_TRUE(acceptFd > 0);
    ret = getpeername(acceptFd, reinterpret_cast<struct sockaddr *>(&peerAddr), &peerLen);
    EXPECT_EQ(ret, 0);
    EXPECT_STREQ(inet_ntoa(peerAddr.sin_addr), TEST_LOCAL_IP);
    EXPECT_EQ(peerLen, sizeof(struct sockaddr));

    size = send(acceptFd, TEST_EXIT, strlen(TEST_EXIT), 0);
    EXPECT_EQ(size, strlen(TEST_EXIT));

    close(acceptFd);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETPEERNAME_0200
 * @tc.name   : GetpeernameInvalidSockfdFailed_0002
 * @tc.desc   : getpeername invalid sockfd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsGetInfoTest, GetpeernameInvalidSockfdFailed_0002, Function | MediumTest | Level2)
{
    int ret;
    struct sockaddr_in peerAddr;
    socklen_t peerLen = sizeof(struct sockaddr_in);

    errno = 0;
    ret = getpeername(BAD_SOCKET_FD, reinterpret_cast<struct sockaddr *>(&peerAddr), &peerLen);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    ret = getpeername(STDIN_FILENO, reinterpret_cast<struct sockaddr *>(&peerAddr), &peerLen);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTSOCK);

    errno = 0;
    ret = getpeername(g_serviceFd, reinterpret_cast<struct sockaddr *>(&peerAddr), &peerLen);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTCONN);
}
