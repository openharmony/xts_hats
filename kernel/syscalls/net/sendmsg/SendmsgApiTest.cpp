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

class HatsSendmsgTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSendmsgTest::SetUp()
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
void HatsSendmsgTest::TearDown()
{
    close(g_serviceFd);
    g_serviceFd = -1;
}
void HatsSendmsgTest::SetUpTestCase()
{
}
void HatsSendmsgTest::TearDownTestCase()
{
}

static void *ClientConnect(void *args)
{
    int ret;
    ssize_t size;
    int clientFd = -1;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
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

    struct msghdr msg = {0};
    struct iovec iov = {buffer, sizeof(buffer)};
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_name = reinterpret_cast<struct sockaddr *>(&addr);
    msg.msg_namelen = addrlen;

    size = recvmsg(clientFd, &msg, 0);
    EXPECT_TRUE(size >= 0);

    close(clientFd);
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDMSG_0100
 * @tc.name   : SendmsgSuccess_0001
 * @tc.desc   : Sendmsg test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSendmsgTest, SendmsgSuccess_0001, Function | MediumTest | Level1)
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

    struct msghdr msg;
    struct iovec iov;
    char buffer[BUFFER_SIZE];
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg.msg_name = nullptr; // 不需要目标地址
    msg.msg_namelen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = nullptr;
    msg.msg_controllen = 0;

    size = sendmsg(acceptFd, &msg, 0);
    EXPECT_TRUE(size >= 0);

    close(acceptFd);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDMSG_0200
 * @tc.name   : SendmsgSuccess_0002
 * @tc.desc   : Sendmsg test success by MSG_MORE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSendmsgTest, SendmsgSuccess_0002, Function | MediumTest | Level2)
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

    char buf1[] = "Hello, ";
    char buf2[] = "world!";

    struct iovec iov[2];
    iov[0].iov_base = buf1;
    iov[0].iov_len = sizeof(buf1) - 1;
    iov[1].iov_base = buf2;
    iov[1].iov_len = sizeof(buf2) - 1;

    struct msghdr message;
    memset_s(&message, sizeof(message), 0, sizeof(message));
    message.msg_iov = iov;
    message.msg_iovlen = 2;

    size = sendmsg(acceptFd, &message, MSG_MORE);
    EXPECT_TRUE(size >= 0);

    size = sendmsg(acceptFd, &message, 0);
    EXPECT_TRUE(size >= 0);

    close(acceptFd);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDMSG_0300
 * @tc.name   : SendmsgSuccess_0003
 * @tc.desc   : Sendmsg test success by MSG_NOSIGNAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSendmsgTest, SendmsgSuccess_0003, Function | MediumTest | Level2)
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

    char buf[] = "Hello, world!";
    struct iovec iov;
    iov.iov_base = buf;
    iov.iov_len = sizeof(buf) - 1;

    struct msghdr message;
    memset_s(&message, sizeof(message), 0, sizeof(message));
    message.msg_iov = &iov;
    message.msg_iovlen = 1;

    size = sendmsg(acceptFd, &message, MSG_NOSIGNAL);
    EXPECT_TRUE(size >= 0);

    close(acceptFd);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDMSG_0400
 * @tc.name   : SendmsgSuccess_0004
 * @tc.desc   : Sendmsg test success by MSG_OOB.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSendmsgTest, SendmsgSuccess_0004, Function | MediumTest | Level2)
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

    char oobData = '!';
    struct iovec iov;
    iov.iov_base = &oobData;
    iov.iov_len = sizeof(oobData);

    struct msghdr message;
    memset_s(&message, sizeof(message), 0, sizeof(message));
    message.msg_iov = &iov;
    message.msg_iovlen = 1;

    size = sendmsg(acceptFd, &message, MSG_OOB);
    EXPECT_TRUE(size >= 0);

    char normalData[] = "Hello, world!";
    iov.iov_base = normalData;
    iov.iov_len = sizeof(normalData) - 1;

    size = sendmsg(acceptFd, &message, 0);
    EXPECT_TRUE(size >= 0);

    close(acceptFd);
    pthread_join(thread, nullptr);
}