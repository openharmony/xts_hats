/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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

static const int BUFFER_SIZE = 1024;
static const int TEST_PORT = 22354;
static const char *TEST_LOCAL_IP = "127.0.0.1";
static int g_serviceFd = -1;

class HatsSendtoTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSendtoTest::SetUp()
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
void HatsSendtoTest::TearDown()
{
    close(g_serviceFd);
    g_serviceFd = -1;
}
void HatsSendtoTest::SetUpTestCase()
{
}
void HatsSendtoTest::TearDownTestCase()
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

    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    size = recvfrom(clientFd, buffer, BUFFER_SIZE, 0, reinterpret_cast<struct sockaddr *>(&clientAddr), &len);
    EXPECT_TRUE(size >= 0);

    close(clientFd);
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDTO_0100
 * @tc.name   : SendtoSuccess_0001
 * @tc.desc   : Sendto test success by MSG_MORE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSendtoTest, SendtoSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    ssize_t size;
    pthread_t thread;
    int acceptFd = -1;
    int32_t backLog = 5;
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);

    ret = listen(g_serviceFd, backLog);
    EXPECT_EQ(ret, 0);

    pthread_create(&thread, nullptr, ClientConnect, nullptr);

    acceptFd = accept4(g_serviceFd, reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen, 0);
    EXPECT_TRUE(acceptFd > 0);

    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    char buf1[] = "Hello, ";
    char buf2[] = "world!";
    // 第一次发送, 使用 MSG_MORE 标志，表示还有后续数据要发送
    size = sendto(acceptFd, buf1, sizeof(buf1) - 1, MSG_MORE,
                  reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_TRUE(size > 0);

    // 第二次发送, 不使用 MSG_MORE，表示数据发送完成
    size = sendto(acceptFd, buf2, sizeof(buf2) - 1, 0,
                  reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_TRUE(size > 0);

    close(acceptFd);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDTO_0200
 * @tc.name   : SendtoSuccess_0002
 * @tc.desc   : Sendto test success by MSG_OOB.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSendtoTest, SendtoSuccess_0002, Function | MediumTest | Level1)
{
    int ret;
    ssize_t size;
    pthread_t thread;
    int acceptFd = -1;
    int32_t backLog = 5;
    const char *message = "Hello, world!";
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    ret = listen(g_serviceFd, backLog);
    EXPECT_EQ(ret, 0);

    pthread_create(&thread, nullptr, ClientConnect, nullptr);

    acceptFd = accept4(g_serviceFd, reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen, 0);
    EXPECT_TRUE(acceptFd > 0);

    size = sendto(acceptFd, message, strlen(message), MSG_OOB,
                  reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_TRUE(size > 0);
    size = sendto(acceptFd, message, strlen(message), 0,
                  reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_TRUE(size >= 0);

    close(acceptFd);
    pthread_join(thread, nullptr);
}