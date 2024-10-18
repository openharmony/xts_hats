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
static const int TEST_UDP_PORT = 22356;
static const int TEST_TCP_PORT = 22355;
static const char *TEST_LOCAL_IP = "127.0.0.1";
static const char *TEST_BUFFER = "Hello, world!";
static const char *TEST_EXIT = "exit";
static int g_tcpFd = -1;
static int g_udpFd = -1;

enum ClientType {
    NO_RECV_MSG = 0,
    WAIT_MSG_NULL,
    WAIT_MSG_EXIT,
};

class HatsDataHandleTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsDataHandleTest::SetUp()
{
    int ret;
    int tcpFd = -1;
    int udpFd = -1;
    int32_t optVal = 1;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_TCP_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    tcpFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(tcpFd > 0);

    ret = setsockopt(tcpFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    EXPECT_EQ(ret, 0);

    ret = bind(tcpFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_EQ(ret, 0);

    serAddr.sin_port = htons(TEST_UDP_PORT),
    udpFd = socket(AF_INET, SOCK_DGRAM, 0);
    EXPECT_TRUE(udpFd > 0);

    ret = setsockopt(udpFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    EXPECT_EQ(ret, 0);

    ret = bind(udpFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(serAddr));
    EXPECT_EQ(ret, 0);

    g_tcpFd = tcpFd;
    g_udpFd = udpFd;
}
void HatsDataHandleTest::TearDown()
{
    close(g_tcpFd);
    g_tcpFd = -1;
    close(g_udpFd);
    g_udpFd = -1;
}
void HatsDataHandleTest::SetUpTestCase()
{
}
void HatsDataHandleTest::TearDownTestCase()
{
}

static void *TcpClient(void *args)
{
    int ret;
    ssize_t size;
    char buffer[BUFFER_SIZE] = { 0 };
    int clientFd = -1;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_TCP_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    clientFd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(clientFd > 0);

    ret = connect(clientFd, reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(ret, 0);

    size = send(clientFd, TEST_BUFFER, strlen(TEST_BUFFER), 0);
    EXPECT_EQ(size, strlen(TEST_BUFFER));

    size = recv(clientFd, static_cast<void *>(buffer), BUFFER_SIZE, 0);
    EXPECT_EQ(size, strlen(TEST_EXIT));
    EXPECT_STREQ(buffer, TEST_EXIT);

    close(clientFd);
    return nullptr;
}

static void *UdpClient(void *args)
{
    ssize_t size;
    int clientFd = -1;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_UDP_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };
    ClientType *type = reinterpret_cast<ClientType *>(args);

    clientFd = socket(AF_INET, SOCK_DGRAM, 0);
    EXPECT_TRUE(clientFd > 0);

    size = sendto(clientFd, TEST_BUFFER, strlen(TEST_BUFFER), 0,
        reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(size, strlen(TEST_BUFFER));

    if (*type == WAIT_MSG_EXIT) {
        size = recvfrom(clientFd, static_cast<void *>(buffer), BUFFER_SIZE, 0,
            reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
        EXPECT_EQ(size, strlen(TEST_EXIT));
        EXPECT_STREQ(buffer, TEST_EXIT);
    } else if (*type == WAIT_MSG_NULL) {
        size = recvfrom(clientFd, static_cast<void *>(buffer), BUFFER_SIZE, 0,
            reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
        EXPECT_EQ(size, 0);
    }

    close(clientFd);
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDTO_RECVFROM_0100
 * @tc.name   : SendtoAndRecvFromDataSuccess_0001
 * @tc.desc   : sendto and recvfrom by client and service success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsDataHandleTest, SendtoAndRecvFromDataSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t size;
    pthread_t thread;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);
    ClientType type = WAIT_MSG_EXIT;

    pthread_create(&thread, nullptr, UdpClient, static_cast<void *>(&type));

    size = recvfrom(g_udpFd, static_cast<void *>(buffer), BUFFER_SIZE, 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_EQ(size, strlen(TEST_BUFFER));
    EXPECT_STREQ(buffer, TEST_BUFFER);

    size = sendto(g_udpFd, TEST_EXIT, strlen(TEST_EXIT), 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), addrLen);
    EXPECT_EQ(size, strlen(TEST_EXIT));

    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDTO_RECVFROM_0200
 * @tc.name   : SendtoAndRecvFromInvalidParamsFailed_0002
 * @tc.desc   : sendto and recvfrom use invalid param failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsDataHandleTest, SendtoAndRecvFromInvalidParamsFailed_0002, Function | MediumTest | Level2)
{
    ssize_t size;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);
    int socketFd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    EXPECT_TRUE(socketFd > 0);

    errno = 0;
    size = recvfrom(BAD_SOCKET_FD, static_cast<void *>(buffer), BUFFER_SIZE, 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    size = recvfrom(STDIN_FILENO, static_cast<void *>(buffer), BUFFER_SIZE, 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, ENOTSOCK);

    errno = 0;
    size = recvfrom(socketFd, nullptr, BUFFER_SIZE, 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EAGAIN);

    errno = 0;
    size = recvfrom(socketFd, static_cast<void *>(buffer), 0, 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EAGAIN);

    errno = 0;
    size = sendto(BAD_SOCKET_FD, TEST_EXIT, strlen(TEST_EXIT), 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), addrLen);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    size = sendto(STDIN_FILENO, TEST_EXIT, strlen(TEST_EXIT), 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), addrLen);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, ENOTSOCK);

    errno = 0;
    size = sendto(socketFd, nullptr, BUFFER_SIZE, 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), addrLen);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    size = sendto(socketFd, TEST_EXIT, 0, 0,
        reinterpret_cast<struct sockaddr *>(&dstAddr), addrLen);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDTO_RECVFROM_0300
 * @tc.name   : SendtoFlagTest_0003
 * @tc.desc   : sendto test MSG_DONTWAIT and MSG_CMSG_CLOEXEC flags success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsDataHandleTest, SendtoFlagTest_0003, Function | MediumTest | Level1)
{
    ssize_t size;
    int clientFd = -1;
    struct sockaddr_in serAddr = {
        .sin_family = AF_INET,
        .sin_port = htons(TEST_UDP_PORT),
        .sin_addr = {
            .s_addr = inet_addr(TEST_LOCAL_IP),
        }
    };

    clientFd = socket(AF_INET, SOCK_DGRAM, 0);
    EXPECT_TRUE(clientFd > 0);

    size = sendto(clientFd, TEST_BUFFER, strlen(TEST_BUFFER), MSG_DONTWAIT,
        reinterpret_cast<struct sockaddr *>(&serAddr), sizeof(struct sockaddr_in));
    EXPECT_EQ(size, strlen(TEST_BUFFER));

    close(clientFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDMSG_RECVMSG_0400
 * @tc.name   : RecvmsgSendmsgDataTest_0004
 * @tc.desc   : sendmsg and recvmsg data success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsDataHandleTest, RecvmsgSendmsgDataTest_0004, Function | MediumTest | Level1)
{
    ssize_t size;
    pthread_t thread;
    ClientType type = WAIT_MSG_EXIT;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in dstAddr = { 0 };
    struct iovec io = {
        .iov_base = buffer,
        .iov_len = BUFFER_SIZE,
    };
    struct msghdr msgHdr = {
        .msg_name = &dstAddr,
        .msg_namelen = sizeof(struct sockaddr_in),
        .msg_iov = &io,
        .msg_iovlen = 1,
    };

    pthread_create(&thread, nullptr, UdpClient, static_cast<void *>(&type));

    size = recvmsg(g_udpFd, &msgHdr, 0);EXPECT_EQ(size, strlen(TEST_BUFFER));
    EXPECT_STREQ(static_cast<char *>(msgHdr.msg_iov->iov_base), TEST_BUFFER);

    memcpy_s(msgHdr.msg_iov->iov_base, BUFFER_SIZE, TEST_EXIT, strlen(TEST_EXIT));
    msgHdr.msg_iov->iov_len = strlen(TEST_EXIT);
    size = sendmsg(g_udpFd, &msgHdr, 0);
    EXPECT_EQ(size, strlen(TEST_EXIT));

    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDMSG_RECVMSG_0500
 * @tc.name   : SendmsgRecvmsgInvalidFdFailed_0005
 * @tc.desc   : sendmsg and recvmsg use invalid socket fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsDataHandleTest, SendmsgRecvmsgInvalidFdFailed_0005, Function | MediumTest | Level2)
{
    ssize_t size;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in dstAddr = { 0 };
    struct iovec io = {
        .iov_base = buffer,
        .iov_len = BUFFER_SIZE,
    };
    struct msghdr msgHdr = {
        .msg_name = &dstAddr,
        .msg_namelen = sizeof(struct sockaddr_in),
        .msg_iov = &io,
        .msg_iovlen = 1,
    };

    errno = 0;
    size = recvmsg(BAD_SOCKET_FD, &msgHdr, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
    errno = 0;
    size = sendmsg(BAD_SOCKET_FD, &msgHdr, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    size = recvmsg(STDIN_FILENO, &msgHdr, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, ENOTSOCK);
    errno = 0;
    size = sendmsg(STDIN_FILENO, &msgHdr, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, ENOTSOCK);
}


/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDMSG_RECVMSG_0600
 * @tc.name   : SendmsgRecvmsgMsgNameTest_0006
 * @tc.desc   : send and recvmsg use msg_name nullptr test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsDataHandleTest, SendmsgRecvmsgMsgNameTest_0006, Function | MediumTest | Level2)
{
    ssize_t size;
    pthread_t thread;
    ClientType type = NO_RECV_MSG;
    char buffer[BUFFER_SIZE] = { 0 };
    struct iovec io = {
        .iov_base = buffer,
        .iov_len = BUFFER_SIZE,
    };
    struct msghdr msgHdr = {
        .msg_name = nullptr,
        .msg_namelen = sizeof(struct sockaddr_in),
        .msg_iov = &io,
        .msg_iovlen = 1,
    };

    pthread_create(&thread, nullptr, UdpClient, static_cast<void *>(&type));
    size = recvmsg(g_udpFd, &msgHdr, 0);
    EXPECT_EQ(size, strlen(TEST_BUFFER));
    EXPECT_EQ(msgHdr.msg_name, nullptr);
    errno = 0;
    size = sendmsg(g_udpFd, &msgHdr, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EDESTADDRREQ);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDMSG_RECVMSG_0800
 * @tc.name   : SendmsgRecvmsgErrorNameLenTest_0008
 * @tc.desc   : send and recvmsg use error msg_name length test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsDataHandleTest, SendmsgRecvmsgErrorNameLenTest_0008, Function | MediumTest | Level2)
{
    ssize_t size;
    pthread_t thread;
    ClientType type = NO_RECV_MSG;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in dstAddr = { 0 };
    struct iovec io = {
        .iov_base = buffer,
        .iov_len = BUFFER_SIZE,
    };
    struct msghdr msgHdr = {
        .msg_name = &dstAddr,
        .msg_namelen = 0,
        .msg_iov = &io,
        .msg_iovlen = 1,
    };

    pthread_create(&thread, nullptr, UdpClient, static_cast<void *>(&type));
    errno = 0;
    size = recvmsg(g_udpFd, &msgHdr, 0);
    EXPECT_EQ(size, strlen(TEST_BUFFER));
    EXPECT_EQ(msgHdr.msg_namelen, sizeof(struct sockaddr_in));
    msgHdr.msg_namelen = 0;
    errno = 0;
    size = sendmsg(g_udpFd, &msgHdr, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EDESTADDRREQ);
    pthread_join(thread, nullptr);

    type = WAIT_MSG_NULL;
    pthread_create(&thread, nullptr, UdpClient, static_cast<void *>(&type));
    msgHdr.msg_name = &dstAddr;
    msgHdr.msg_namelen = sizeof(struct sockaddr_in);
    msgHdr.msg_iov = nullptr;
    msgHdr.msg_iovlen = 0;
    size = recvmsg(g_udpFd, &msgHdr, 0);
    EXPECT_EQ(size, 0);
    msgHdr.msg_iov = nullptr;
    msgHdr.msg_iovlen = 0;
    errno = 0;
    size = sendmsg(g_udpFd, &msgHdr, 0);
    EXPECT_EQ(size, 0);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDMSG_RECVMSG_0900
 * @tc.name   : SendmsgRecvmsgMsgIovTest_0009
 * @tc.desc   : send and recvmsg msg_iov nullptr test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsDataHandleTest, SendmsgRecvmsgMsgIovTest_0009, Function | MediumTest | Level2)
{
    ssize_t size;
    pthread_t thread;
    ClientType type = WAIT_MSG_NULL;
    struct sockaddr_in dstAddr = { 0 };
    struct msghdr msgHdr = {
        .msg_name = &dstAddr,
        .msg_namelen = sizeof(struct sockaddr_in),
        .msg_iov = nullptr,
        .msg_iovlen = 0,
    };

    pthread_create(&thread, nullptr, UdpClient, static_cast<void *>(&type));
    size = recvmsg(g_udpFd, &msgHdr, 0);
    EXPECT_EQ(size, 0);
    msgHdr.msg_iov = nullptr;
    msgHdr.msg_iovlen = 0;
    errno = 0;
    size = sendmsg(g_udpFd, &msgHdr, 0);
    EXPECT_EQ(size, 0);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SEND_RECV_1000
 * @tc.name   : SendAndRecvDataSuccess_0010
 * @tc.desc   : send/recv and getsockname by client and service success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsDataHandleTest, SendAndRecvDataSuccess_0010, Function | MediumTest | Level1)
{
    int ret;
    ssize_t size;
    pthread_t thread;
    int acceptFd = -1;
    int32_t backLog = 5;
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);

    ret = listen(g_tcpFd, backLog);
    EXPECT_EQ(ret, 0);

    pthread_create(&thread, nullptr, TcpClient, nullptr);

    acceptFd = accept(g_tcpFd, reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_TRUE(acceptFd > 0);

    // test getsockname get client name success
    ret = getsockname(g_tcpFd, reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_EQ(ret, 0);
    EXPECT_STREQ(inet_ntoa(dstAddr.sin_addr), TEST_LOCAL_IP);

    // test recv success
    size = recv(acceptFd, static_cast<void *>(buffer), BUFFER_SIZE, 0);
    EXPECT_EQ(size, strlen(TEST_BUFFER));
    EXPECT_STREQ(buffer, TEST_BUFFER);

    // test send success
    size = send(acceptFd, TEST_EXIT, strlen(TEST_EXIT), 0);
    EXPECT_EQ(size, strlen(TEST_EXIT));

    close(acceptFd);
    pthread_join(thread, nullptr);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETSOCKNAME_1100
 * @tc.name   : GetsocknameUseInvalidFdFailed_0011
 * @tc.desc   : getsockname use invalid socket fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsDataHandleTest, GetsocknameUseInvalidFdFailed_0011, Function | MediumTest | Level2)
{
    int ret;
    struct sockaddr_in dstAddr = { 0 };
    socklen_t addrLen = sizeof(struct sockaddr_in);

    // use invalid socket fd failed
    errno = 0;
    ret = getsockname(BAD_SOCKET_FD, reinterpret_cast<struct sockaddr *>(&dstAddr), &addrLen);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

