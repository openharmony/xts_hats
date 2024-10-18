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
static const int TEST_PORT = 22357;
static const char *TEST_LOCAL_IP = "127.0.0.1";
static int g_serviceFd = -1;
static const int TEST_DELAY_TIME = 100000;

class HatsShutdownTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsShutdownTest::SetUp()
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
void HatsShutdownTest::TearDown()
{
    close(g_serviceFd);
    g_serviceFd = -1;
}
void HatsShutdownTest::SetUpTestCase()
{
}
void HatsShutdownTest::TearDownTestCase()
{
}

static void *ClientConnect(void *args)
{
    int ret;
    int clientFd = -1;
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

    usleep(TEST_DELAY_TIME);

    close(clientFd);
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHUTDOWN_0100
 * @tc.name   : ShutdownSHUT_WRTestSuccess_0001
 * @tc.desc   : shutdown socket fd by SHUT_WR success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsShutdownTest, ShutdownSHUT_WRTestSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    pthread_t thread;
    int acceptFd = -1;
    int32_t backLog = 5;
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);

    ret = listen(g_serviceFd, backLog);
    EXPECT_EQ(ret, 0);

    pthread_create(&thread, nullptr, ClientConnect, nullptr);

    acceptFd = accept(g_serviceFd, reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_TRUE(acceptFd > 0);
    ret = shutdown(g_serviceFd, SHUT_WR);
    EXPECT_EQ(ret, 0);

    close(acceptFd);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHUTDOWN_0200
 * @tc.name   : ShutdownSHUT_RDTestSuccess_0002
 * @tc.desc   : shutdown socket fd by SHUT_RD success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsShutdownTest, ShutdownSHUT_RDTestSuccess_0002, Function | MediumTest | Level1)
{
    int ret;
    pthread_t thread;
    int acceptFd = -1;
    int32_t backLog = 5;
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);

    ret = listen(g_serviceFd, backLog);
    EXPECT_EQ(ret, 0);

    pthread_create(&thread, nullptr, ClientConnect, nullptr);

    acceptFd = accept(g_serviceFd, reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_TRUE(acceptFd > 0);
    ret = shutdown(g_serviceFd, SHUT_RD);
    EXPECT_EQ(ret, 0);

    close(acceptFd);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHUTDOWN_0300
 * @tc.name   : ShutdownSHUT_RDWRTestSuccess_0003
 * @tc.desc   : shutdown socket fd by SHUT_RDWR success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsShutdownTest, ShutdownSHUT_RDWRTestSuccess_0003, Function | MediumTest | Level1)
{
    int ret;
    pthread_t thread;
    int acceptFd = -1;
    int32_t backLog = 5;
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);

    ret = listen(g_serviceFd, backLog);
    EXPECT_EQ(ret, 0);

    pthread_create(&thread, nullptr, ClientConnect, nullptr);

    acceptFd = accept(g_serviceFd, reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_TRUE(acceptFd > 0);

    ret = shutdown(g_serviceFd, SHUT_RDWR);
    EXPECT_EQ(ret, 0);

    close(acceptFd);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHUTDOWN_0400
 * @tc.name   : ShutdownUseInvalidFdFailed_0004
 * @tc.desc   : shutdown socket fd by invalid fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsShutdownTest, ShutdownUseInvalidFdFailed_0004, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = shutdown(g_serviceFd, SHUT_RDWR);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTCONN);

    errno = 0;
    ret = shutdown(BAD_SOCKET_FD, SHUT_RDWR);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}
