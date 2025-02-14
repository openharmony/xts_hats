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
#include <linux/netlink.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

static const int MAX_SIZE = 128;
static const int ID_ONE = 88;
static const int ID_TWO = 98;

class HatsSocketTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSocketTest::SetUp()
{
}
void HatsSocketTest::TearDown()
{
}
void HatsSocketTest::SetUpTestCase()
{
}
void HatsSocketTest::TearDownTestCase()
{
}

struct SetOptionSupportTest {
    int id;
    int domain;
    int type;
    int protocol;
} g_socketOpt[] = {
    {0,     AF_UNIX, SOCK_STREAM, NETLINK_ROUTE},
    {1,     AF_UNIX, SOCK_STREAM, IPPROTO_IP},
    {2,     AF_UNIX, SOCK_STREAM, IPPROTO_ICMP},
    {3,     AF_UNIX, SOCK_DGRAM, NETLINK_ROUTE},
    {4,     AF_UNIX, SOCK_DGRAM, IPPROTO_IP},
    {5,     AF_UNIX, SOCK_DGRAM, IPPROTO_ICMP},
    {6,     AF_UNIX, SOCK_SEQPACKET, NETLINK_ROUTE},
    {7,     AF_UNIX, SOCK_SEQPACKET, IPPROTO_IP},
    {8,     AF_UNIX, SOCK_SEQPACKET, IPPROTO_ICMP},
    {10,    AF_LOCAL, SOCK_STREAM, NETLINK_ROUTE},
    {11,    AF_LOCAL, SOCK_STREAM, IPPROTO_IP},
    {12,    AF_LOCAL, SOCK_STREAM, IPPROTO_ICMP},
    {13,    AF_LOCAL, SOCK_DGRAM, NETLINK_ROUTE},
    {14,    AF_LOCAL, SOCK_DGRAM, IPPROTO_IP},
    {15,    AF_LOCAL, SOCK_DGRAM, IPPROTO_ICMP},
    {16,    AF_LOCAL, SOCK_SEQPACKET, NETLINK_ROUTE},
    {17,    AF_LOCAL, SOCK_SEQPACKET, IPPROTO_IP},
    {20,    AF_LOCAL, SOCK_SEQPACKET, IPPROTO_ICMP},
    {24,    AF_INET, SOCK_STREAM, NETLINK_ROUTE},
    {25,    AF_INET, SOCK_STREAM, IPPROTO_IP},
    {26,    AF_INET, SOCK_STREAM, IPPROTO_TCP},
    {27,    AF_INET, SOCK_DGRAM, NETLINK_ROUTE},
    {28,    AF_INET, SOCK_DGRAM, IPPROTO_IP},
    {29,    AF_INET, SOCK_DGRAM, IPPROTO_ICMP},
    {30,    AF_INET, SOCK_DGRAM, IPPROTO_UDP},
    {31,    AF_INET, SOCK_RAW, NETLINK_FIREWALL},
    {32,    AF_INET, SOCK_RAW, NETLINK_SOCK_DIAG},
    {33,    AF_INET, SOCK_RAW, NETLINK_NFLOG},
    {34,    AF_INET, SOCK_RAW, NETLINK_AUDIT},
    {35,    AF_INET, SOCK_RAW, NETLINK_FIB_LOOKUP},
    {36,    AF_INET, SOCK_RAW, NETLINK_CONNECTOR},
    {37,    AF_INET, SOCK_RAW, NETLINK_NETFILTER},
    {38,    AF_INET, SOCK_RAW, NETLINK_KOBJECT_UEVENT},
    {39,    AF_INET, SOCK_RAW, NETLINK_GENERIC},
    {40,    AF_INET, SOCK_RAW, IPPROTO_ICMPV6},
    {41,    AF_INET, SOCK_RAW, IPPROTO_ICMP},
    {42,    AF_INET, SOCK_RAW, IPPROTO_TCP},
    {43,    AF_INET, SOCK_RAW, IPPROTO_UDP},
    {44,    AF_INET, SOCK_PACKET, NETLINK_ROUTE},
    {45,    AF_INET, SOCK_PACKET, NETLINK_FIREWALL},
    {46,    AF_INET, SOCK_PACKET, NETLINK_SOCK_DIAG},
    {47,    AF_INET, SOCK_PACKET, NETLINK_NFLOG},
    {48,    AF_INET, SOCK_PACKET, NETLINK_AUDIT},
    {49,    AF_INET, SOCK_PACKET, NETLINK_FIB_LOOKUP},
    {50,    AF_INET, SOCK_PACKET, NETLINK_CONNECTOR},
    {51,    AF_INET, SOCK_PACKET, NETLINK_NETFILTER},
    {52,    AF_INET, SOCK_PACKET, NETLINK_KOBJECT_UEVENT},
    {53,    AF_INET, SOCK_PACKET, NETLINK_GENERIC},
    {54,    AF_INET, SOCK_PACKET, IPPROTO_ICMPV6},
    {55,    AF_INET, SOCK_PACKET, IPPROTO_IP},
    {56,    AF_INET, SOCK_PACKET, IPPROTO_ICMP},
    {57,    AF_INET, SOCK_PACKET, IPPROTO_TCP},
    {58,    AF_INET, SOCK_PACKET, IPPROTO_UDP},
    {59,    AF_INET6, SOCK_STREAM, NETLINK_ROUTE},
    {60,    AF_INET6, SOCK_STREAM, IPPROTO_IP},
    {61,    AF_INET6, SOCK_STREAM, IPPROTO_TCP},
    {62,    AF_INET6, SOCK_DGRAM, NETLINK_ROUTE},
    {63,    AF_INET6, SOCK_DGRAM, IPPROTO_ICMPV6},
    {64,    AF_INET6, SOCK_DGRAM, IPPROTO_IP},
    {65,    AF_INET6, SOCK_DGRAM, IPPROTO_UDP},
    {66,    AF_INET6, SOCK_RAW, NETLINK_FIREWALL},
    {67,    AF_INET6, SOCK_RAW, NETLINK_SOCK_DIAG},
    {68,    AF_INET6, SOCK_RAW, NETLINK_NFLOG},
    {69,    AF_INET6, SOCK_RAW, NETLINK_AUDIT},
    {70,    AF_INET6, SOCK_RAW, NETLINK_FIB_LOOKUP},
    {71,    AF_INET6, SOCK_RAW, NETLINK_CONNECTOR},
    {72,    AF_INET6, SOCK_RAW, NETLINK_NETFILTER},
    {73,    AF_INET6, SOCK_RAW, NETLINK_KOBJECT_UEVENT},
    {74,    AF_INET6, SOCK_RAW, NETLINK_GENERIC},
    {75,    AF_INET6, SOCK_RAW, IPPROTO_ICMPV6},
    {76,    AF_INET6, SOCK_RAW, IPPROTO_ICMP},
    {77,    AF_INET6, SOCK_RAW, IPPROTO_TCP},
    {78,    AF_INET6, SOCK_RAW, IPPROTO_UDP},
    {79,    AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE},
    {80,    AF_NETLINK, SOCK_DGRAM, NETLINK_SOCK_DIAG},
    {81,    AF_NETLINK, SOCK_DGRAM, NETLINK_AUDIT},
    {82,    AF_NETLINK, SOCK_DGRAM, NETLINK_FIB_LOOKUP},
    {84,    AF_NETLINK, SOCK_DGRAM, NETLINK_NETFILTER},
    {85,    AF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT},
    {86,    AF_NETLINK, SOCK_DGRAM, NETLINK_GENERIC},
    {87,    AF_NETLINK, SOCK_DGRAM, IPPROTO_IP},
    {88,    AF_NETLINK, SOCK_DGRAM, NETLINK_XFRM},
    {89,    AF_NETLINK, SOCK_RAW, NETLINK_ROUTE},
    {90,    AF_NETLINK, SOCK_RAW, NETLINK_SOCK_DIAG},
    {91,    AF_NETLINK, SOCK_RAW, NETLINK_AUDIT},
    {92,    AF_NETLINK, SOCK_RAW, NETLINK_FIB_LOOKUP},
    {94,    AF_NETLINK, SOCK_RAW, NETLINK_NETFILTER},
    {95,    AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT},
    {96,    AF_NETLINK, SOCK_RAW, NETLINK_GENERIC},
    {97,    AF_NETLINK, SOCK_RAW, IPPROTO_IP},
    {98,    AF_NETLINK, SOCK_RAW, NETLINK_XFRM},
    {99,    AF_PACKET, SOCK_DGRAM, NETLINK_ROUTE},
    {110,   AF_PACKET, SOCK_DGRAM, NETLINK_FIREWALL},
    {111,   AF_PACKET, SOCK_DGRAM, NETLINK_SOCK_DIAG},
    {112,   AF_PACKET, SOCK_DGRAM, NETLINK_NFLOG},
    {113,   AF_PACKET, SOCK_DGRAM, NETLINK_AUDIT},
    {114,   AF_PACKET, SOCK_DGRAM, NETLINK_FIB_LOOKUP},
    {115,   AF_PACKET, SOCK_DGRAM, NETLINK_CONNECTOR},
    {116,   AF_PACKET, SOCK_DGRAM, NETLINK_NETFILTER},
    {117,   AF_PACKET, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT},
    {118,   AF_PACKET, SOCK_DGRAM, NETLINK_GENERIC},
    {119,   AF_PACKET, SOCK_DGRAM, IPPROTO_ICMPV6},
    {120,   AF_PACKET, SOCK_DGRAM, IPPROTO_IP},
    {121,   AF_PACKET, SOCK_DGRAM, IPPROTO_ICMP},
    {122,   AF_PACKET, SOCK_DGRAM, IPPROTO_TCP},
    {123,   AF_PACKET, SOCK_DGRAM, IPPROTO_UDP},
    {124,   AF_PACKET, SOCK_RAW, NETLINK_ROUTE},
    {125,   AF_PACKET, SOCK_RAW, NETLINK_FIREWALL},
    {126,   AF_PACKET, SOCK_RAW, NETLINK_SOCK_DIAG},
    {127,   AF_PACKET, SOCK_RAW, NETLINK_NFLOG},
    {128,   AF_PACKET, SOCK_RAW, NETLINK_AUDIT},
    {129,   AF_PACKET, SOCK_RAW, NETLINK_FIB_LOOKUP},
    {130,   AF_PACKET, SOCK_RAW, NETLINK_CONNECTOR},
    {131,   AF_PACKET, SOCK_RAW, NETLINK_NETFILTER},
    {132,   AF_PACKET, SOCK_RAW, NETLINK_KOBJECT_UEVENT},
    {133,   AF_PACKET, SOCK_RAW, NETLINK_GENERIC},
    {134,   AF_PACKET, SOCK_RAW, IPPROTO_ICMPV6},
    {135,   AF_PACKET, SOCK_RAW, IPPROTO_IP},
    {136,   AF_PACKET, SOCK_RAW, IPPROTO_ICMP},
    {137,   AF_PACKET, SOCK_RAW, IPPROTO_TCP},
    {138,   AF_PACKET, SOCK_RAW, IPPROTO_UDP},
    {139,   AF_PACKET, SOCK_PACKET, NETLINK_ROUTE},
    {140,   AF_PACKET, SOCK_PACKET, NETLINK_FIREWALL},
    {141,   AF_PACKET, SOCK_PACKET, NETLINK_SOCK_DIAG},
    {142,   AF_PACKET, SOCK_PACKET, NETLINK_NFLOG},
    {143,   AF_PACKET, SOCK_PACKET, NETLINK_AUDIT},
    {144,   AF_PACKET, SOCK_PACKET, NETLINK_FIB_LOOKUP},
    {145,   AF_PACKET, SOCK_PACKET, NETLINK_CONNECTOR},
    {146,   AF_PACKET, SOCK_PACKET, NETLINK_NETFILTER},
    {147,   AF_PACKET, SOCK_PACKET, NETLINK_KOBJECT_UEVENT},
    {148,   AF_PACKET, SOCK_PACKET, NETLINK_GENERIC},
    {149,   AF_PACKET, SOCK_PACKET, IPPROTO_ICMPV6},
    {150,   AF_PACKET, SOCK_PACKET, IPPROTO_IP},
    {151,   AF_PACKET, SOCK_PACKET, IPPROTO_ICMP},
    {152,   AF_PACKET, SOCK_PACKET, IPPROTO_TCP},
    {153,   AF_PACKET, SOCK_PACKET, IPPROTO_UDP},
    {154,   AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, NETLINK_ROUTE},
};

struct SocketPairOptionSupportTest {
    int id;
    int domain;
    int type;
} g_socketPairOpt[] = {
    {1,     AF_UNIX,    SOCK_STREAM},
    {2,     AF_UNIX,    SOCK_DGRAM},
    {3,     AF_UNIX,    SOCK_SEQPACKET},
    {4,     AF_UNIX,    SOCK_STREAM | SOCK_NONBLOCK},
    {5,     AF_UNIX,    SOCK_STREAM | SOCK_CLOEXEC},
    {6,     AF_LOCAL,   SOCK_STREAM},
    {7,     AF_LOCAL,   SOCK_DGRAM},
    {8,     AF_LOCAL,   SOCK_SEQPACKET},
    {9,     AF_LOCAL,   SOCK_STREAM | SOCK_NONBLOCK},
    {10,    AF_LOCAL,   SOCK_STREAM | SOCK_CLOEXEC},
};

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKET_0100
 * @tc.name   : SocketCreateFdSuccess_0001
 * @tc.desc   : socket create fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSocketTest, SocketCreateFdSuccess_0001, Function | MediumTest | Level1)
{
    int i;
    int socketFd = -1;
    const string path = "/proc/sys/net/core/xfrm_acq_expires";
    for (i = 0; i < sizeof(g_socketOpt) / sizeof(g_socketOpt[0]); i++) {
        if ((g_socketOpt[i].id == ID_ONE || g_socketOpt[i].id == ID_TWO) && access(path.c_str(), F_OK) == -1) {
            continue;
        }
        socketFd = socket(g_socketOpt[i].domain, g_socketOpt[i].type, g_socketOpt[i].protocol);
        if (socketFd <= 0) {
            printf("id = %d\n", g_socketOpt[i].id);
            printf("err = %s\n", strerror(errno));
        }
        EXPECT_TRUE(socketFd > 0);
        close(socketFd);
    }
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKETPAIR_0200
 * @tc.name   : SocketpairCreateSuccess_0002
 * @tc.desc   : socketpair create success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSocketTest, SocketpairCreateSuccess_0002, Function | MediumTest | Level1)
{
    int i;
    int ret;
    ssize_t size;
    char buf[MAX_SIZE] = {0};
    int socketPair[2];
    const char *content = "Hello, world!";

    for (i = 0; i < sizeof(g_socketPairOpt) / sizeof(g_socketPairOpt[0]); i++) {
        ret = socketpair(g_socketPairOpt[i].domain, g_socketPairOpt[i].type, 0, socketPair);
        if (ret < 0) {
            printf("id = %d, ret = %d\n", g_socketPairOpt[i].id, ret);
        }
        EXPECT_EQ(ret, 0);

        size = write(socketPair[0], content, strlen(content));
        EXPECT_EQ(size, strlen(content));

        read(socketPair[1], buf, MAX_SIZE);
        EXPECT_EQ(size, strlen(content));
        EXPECT_STREQ(buf, content);

        close(socketPair[0]);
        close(socketPair[1]);
    }
}