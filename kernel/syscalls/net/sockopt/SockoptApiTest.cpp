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
#include <linux/filter.h>
#include <linux/if_ether.h>
#include <linux/in6.h>
#include <linux/netlink.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include "securec.h"

using namespace testing::ext;

static const int BAD_SOCKET_FD = -1;
static const int MAX_SIZE = 1024;


struct SetOptionSupportTest {
    std::string name;
    int domain;
    int type;
    int optLevel;
    int optName;
    int ret;
    int optVal;
    int optLen;
} g_setOpt[] = {
    {"SO_BINDTODEVICE",     AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_BINDTODEVICE,    0,      0,  0},
    {"SO_DEBUG",            AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_DEBUG,           0,      1,  4},
    {"SO_REUSEADDR",        AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_REUSEADDR,       0,      1,  4},
    {"SO_REUSEPORT",        AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_REUSEPORT,       0,      1,  4},
    {"SO_BUSY_POLL",        AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_BUSY_POLL,       0,      1,  4},
    {"SO_DONTROUTE",        AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_DONTROUTE,       0,      1,  4},
    {"SO_BROADCAST",        AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_BROADCAST,       0,      1,  4},
    {"SO_SNDBUFFORCE",      AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_SNDBUFFORCE,     0, 0x40000, 4},
    {"IP_TRANSPARENT",      AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_TRANSPARENT,     0,      1,  4},
    {"SO_RCVBUFFORCE",      AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_RCVBUFFORCE,     0, 0x40000, 4},
    {"TCP_REPAIR",          AF_INET,    SOCK_STREAM,    SOL_TCP,        TCP_REPAIR,         0,      1,      4},
    {"SO_KEEPALIVE",        AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_KEEPALIVE,       0,      1,  4},
    {"SO_OOBINLINE",        AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_OOBINLINE,       0,      1,  4},
    {"SO_NO_CHECK",         AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_NO_CHECK,        0,      1,  4},
    {"SO_PRIORITY",         AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_PRIORITY,        0,      1,  4},
    {"SO_BSDCOMPAT",        AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_BSDCOMPAT,       0,      0,  4},
    {"SO_PASSCRED",         AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_PASSCRED,        0,      1,  4},
    {"SO_TIMESTAMP",        AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_TIMESTAMP,       0,      1,  4},
    {"SO_TIMESTAMPNS",      AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_TIMESTAMPNS,     0,      1,  4},
    {"SO_RCVLOWAT",         AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_RCVLOWAT,        0,      1,  4},
    {"SO_PASSSEC",          AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_PASSSEC,         0,      1,  4},
    {"SO_MARK",             AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_MARK,            0,    100,  4},
    {"SO_RXQ_OVFL",         AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_RXQ_OVFL,        0,      1,  4},
    {"SO_WIFI_STATUS",      AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_WIFI_STATUS,     0,      1,  4},
    {"SO_NOFCS",            AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_NOFCS,           0,      1,  4},
    {"SO_SELECT_ERR_QUEUE", AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_SELECT_ERR_QUEUE, 0,     1,  4},
    {"SO_MAX_PACING_RATE",  AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_MAX_PACING_RATE, 0,      1,  4},
    {"SO_INCOMING_CPU",     AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_INCOMING_CPU,    0,      1,  4},
    {"IP_RECVOPTS",         AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_RECVOPTS,        0,      1,  4},
    {"IP_RECVTOS",          AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_RECVTOS,         0,      1,  4},
    {"IP_RETOPTS",          AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_RETOPTS,         0,      1,  4},
    {"IP_MTU_DISCOVER",     AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_MTU_DISCOVER,    0,      1,  4},
    {"IP_RECVERR",          AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_RECVERR,         0,      1,  4},
    {"IP_FREEBIND",         AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_FREEBIND,        0,      1,  4},
    {"IP_PASSSEC",          AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_PASSSEC,         0,      1,  4},
    {"IP_MINTTL",           AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_MINTTL,          0,      1,  4},
    {"IP_BIND_ADDRESS_NO_PORT", AF_INET, SOCK_STREAM,   IPPROTO_IP,     IP_BIND_ADDRESS_NO_PORT, 0, 1,  4},
    {"IP_RECVORIGDSTADDR",  AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_RECVORIGDSTADDR, 0,      1,  4},
    {"IP_CHECKSUM",         AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_CHECKSUM,        0,      1,  4},
    {"IP_OPTIONS",          AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_OPTIONS,         0,      1,  4},
    {"IP_PKTINFO",          AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_PKTINFO,         0,      1,  4},
    {"IP_RECVTTL",          AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_RECVTTL,         0,      1,  4},
    {"IP_TOS",              AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_TOS,             0,      0,  4},
    {"IP_TTL",              AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_TTL,             0,      1,  4},
    {"IP_MULTICAST_LOOP",   AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_MULTICAST_LOOP,  0,      1,  4},
    {"IP_MULTICAST_ALL",    AF_INET6,   SOCK_STREAM,    IPPROTO_IP,     IP_MULTICAST_ALL,   0,      1,  4},
    {"IPV6_V6ONLY",         AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_V6ONLY,        0,      1,  4},
    {"IPV6_RECVPKTINFO",    AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_RECVPKTINFO,   0,      1,  4},
    {"IPV6_2292PKTINFO",    AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_2292PKTINFO,   0,      1,  4},
    {"IPV6_RECVHOPLIMIT",   AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_RECVHOPLIMIT,  0,      1,  4},
    {"IPV6_2292HOPLIMIT",   AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_2292HOPLIMIT,  0,      1,  4},
    {"IPV6_RECVRTHDR",      AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_RECVRTHDR,     0,      1,  4},
    {"IPV6_2292RTHDR",      AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_2292RTHDR,     0,      1,  4},
    {"IPV6_RECVHOPOPTS",    AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_RECVHOPOPTS,   0,      1,  4},
    {"IPV6_2292HOPOPTS",    AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_2292HOPOPTS,   0,      1,  4},
    {"IPV6_RECVDSTOPTS",    AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_RECVDSTOPTS,   0,      1,  4},
    {"IPV6_2292DSTOPTS",    AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_2292DSTOPTS,   0,      1,  4},
    {"IPV6_TCLASS",         AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_TCLASS,        0,      1,  4},
    {"IPV6_RECVTCLASS",     AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_RECVTCLASS,    0,      1,  4},
    {"IPV6_RECVPATHMTU",    AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_RECVPATHMTU,   0,      1,  4},
    {"IPV6_TRANSPARENT",    AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_TRANSPARENT,   0,      1,  4},
    {"IPV6_RECVORIGDSTADDR", AF_INET6,  SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_RECVORIGDSTADDR, 0,    1,  4},
    {"IPV6_UNICAST_HOPS",   AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_UNICAST_HOPS,   0,     1,  4},
    {"IPV6_MULTICAST_HOPS", AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_MULTICAST_HOPS, 0,     1,  4},
    {"IPV6_MULTICAST_LOOP", AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_MULTICAST_LOOP, 0,     1,  4},
    {"IPV6_MULTICAST_IF",   AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_MULTICAST_IF,   0,     1,  4},
    {"IPV6_MTU_DISCOVER",   AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_MTU_DISCOVER,   0,     1,  4},
    {"IPV6_RECVERR",        AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_RECVERR,        0,     1,  4},
    {"IPV6_ADDR_PREFERENCES", AF_INET6, SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_ADDR_PREFERENCES, 0,   1,  4},
    {"IPV6_MINHOPCOUNT",    AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_MINHOPCOUNT,    0,     1,  4},
    {"IPV6_DONTFRAG",       AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_DONTFRAG,       0,     1,  4},
    {"IPV6_AUTOFLOWLABEL",  AF_INET6,   SOCK_DGRAM,     IPPROTO_IPV6,   IPV6_AUTOFLOWLABEL,  0,     1,  4},
    {"TCP_NODELAY",         AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_NODELAY,         0,     1,  4},
    {"TCP_THIN_LINEAR_TIMEOUTS", AF_INET, SOCK_STREAM,  IPPROTO_TCP, TCP_THIN_LINEAR_TIMEOUTS, 0,   1,  4},
    {"TCP_THIN_DUPACK",     AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_THIN_DUPACK,     0,     0,  4},
    {"TCP_CORK",            AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_CORK,            0,     1,  4},
    {"TCP_KEEPIDLE",        AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_KEEPIDLE,        0,     1,  4},
    {"TCP_KEEPINTVL",       AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_KEEPINTVL,       0,     1,  4},
    {"TCP_KEEPCNT",         AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_KEEPCNT,         0,     1,  4},
    {"TCP_SYNCNT",          AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_SYNCNT,          0,     1,  4},
    {"TCP_SAVE_SYN",        AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_SAVE_SYN,        0,     1,  4},
    {"TCP_LINGER2",         AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_LINGER2,         0,     1,  4},
    {"TCP_DEFER_ACCEPT",    AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_DEFER_ACCEPT,    0,     1,  4},
    {"TCP_WINDOW_CLAMP",    AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_WINDOW_CLAMP,    0,     0,  4},
    {"TCP_QUICKACK",        AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_QUICKACK,        0,     1,  4},
    {"TCP_USER_TIMEOUT",    AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_USER_TIMEOUT,    0,     1,  4},
    {"TCP_FASTOPEN",        AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_FASTOPEN,        0,     1,  4},
    {"TCP_NOTSENT_LOWAT",   AF_INET,    SOCK_STREAM,    IPPROTO_TCP,    TCP_NOTSENT_LOWAT,   0,     1,  4},
    {"TCP_FASTOPEN_CONNECT", AF_INET,   SOCK_STREAM,    IPPROTO_TCP,    TCP_FASTOPEN_CONNECT, 0,    1,  4},
};

struct GetOptionOnlySupportTest {
    std::string name;
    int domain;
    int type;
    int optLevel;
    int optName;
    int ret;
    int optVal;
    int optLen;
} g_getOnlyOpt[] = {
    {"SO_TYPE",             AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_TYPE,            0,      1,  4},
    {"SO_PROTOCOL",         AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_PROTOCOL,        0,      6,  4},
    {"SO_DOMAIN",           AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_DOMAIN,          0,      2,  4},
    {"SO_ERROR",            AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_ERROR,           0,      0,  4},
    {"IP_HDRINCL",          AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_HDRINCL,         0,      0,  4},
    {"IP_NODEFRAG",         AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_NODEFRAG,        0,      0,  4},
    {"IP_UNICAST_IF",       AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_UNICAST_IF,      0,      0,  4},
    {"IP_MULTICAST_TTL",    AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_MULTICAST_TTL,   0,      1,  4},
    {"IP_MULTICAST_IF",     AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_MULTICAST_IF,    0,      0,  4},
    {"IPV6_HOPOPTS",        AF_INET6,   SOCK_STREAM,    SOL_IPV6,       IPV6_HOPOPTS,       0,      0,  0},
    {"IPV6_RTHDRDSTOPTS",   AF_INET6,   SOCK_DGRAM,     SOL_IPV6,       IPV6_RTHDRDSTOPTS,  0,      0,  0},
    {"IPV6_RTHDR",          AF_INET6,   SOCK_DGRAM,     SOL_IPV6,       IPV6_RTHDR,         0,      0,  0},
    {"IPV6_DSTOPTS",        AF_INET6,   SOCK_DGRAM,     SOL_IPV6,       IPV6_DSTOPTS,       0,      0,  0},
    {"TCP_MAXSEG",          AF_INET,    SOCK_STREAM,    SOL_TCP,        TCP_MAXSEG,         0,    536,  4},
    {"IP_PKTOPTIONS",       AF_INET,    SOCK_STREAM,    IPPROTO_IP,     IP_PKTOPTIONS,      0,      0,  0},
    {"TCP_INFO",            AF_INET,    SOCK_STREAM,    IPPROTO_IP,     TCP_INFO,           0,      0,   4},
    {"TCP_CC_INFO",         AF_INET,    SOCK_STREAM,    IPPROTO_IP,     TCP_CC_INFO,        0,      0,   4},
    {"SO_SNDLOWAT",         AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_SNDLOWAT,        0,      1,  4},
    {"SO_ACCEPTCONN",       AF_INET,    SOCK_STREAM,    SOL_SOCKET,     SO_ACCEPTCONN,      0,      0,  4},
};

class HatsSockoptTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSockoptTest::SetUp()
{
}
void HatsSockoptTest::TearDown()
{
}
void HatsSockoptTest::SetUpTestCase()
{
}
void HatsSockoptTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKOPT_0100
 * @tc.name   : SetAndGetsockoptSupportOptSuccess_0001
 * @tc.desc   : setsockopt and getsockopt own support option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSockoptTest, SetAndGetsockoptSupportOptSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    int32_t optVal;
    socklen_t optLen;
    for (int32_t i = 0; i < sizeof(g_setOpt) / sizeof(g_setOpt[0]); i++) {
        fd = socket(g_setOpt[i].domain, g_setOpt[i].type, 0);
        EXPECT_TRUE(fd > 0);

        ret = setsockopt(fd, g_setOpt[i].optLevel, g_setOpt[i].optName, &g_setOpt[i].optVal, g_setOpt[i].optLen);
        EXPECT_EQ(ret, g_setOpt[i].ret);

        if (g_setOpt[i].optName == SO_SNDBUFFORCE ||
            g_setOpt[i].optName == SO_RCVBUFFORCE ||
            g_setOpt[i].optName == IPV6_ADDR_PREFERENCES) {
            close(fd);
            continue;
        }
        optVal = 0;
        optLen = g_setOpt[i].optLen;
        ret = getsockopt(fd, g_setOpt[i].optLevel, g_setOpt[i].optName, &optVal, &optLen);
        EXPECT_EQ(ret, g_setOpt[i].ret);
        EXPECT_EQ(optVal, g_setOpt[i].optVal);
        EXPECT_EQ(optLen, g_setOpt[i].optLen);
        close(fd);
    }

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKOPT_0200
 * @tc.name   : SetAndGetsockoptSndAndRcvTestSuccess_0002
 * @tc.desc   : setsockopt and getsockopt SO_SNDBUF and SO_RCVBUF option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSockoptTest, SetAndGetsockoptSndAndRcvTestSuccess_0002, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    int32_t optVal;
    socklen_t optLen;

    fd = socket(AF_INET, SOL_SOCKET, 0);
    EXPECT_TRUE(fd > 0);

    // setsockopt SO_SNDBUF test success
    optVal = 0x20000;
    optLen = sizeof(optVal);
    ret = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &optVal, optLen);
    EXPECT_EQ(ret, 0);

    // getsockopt SO_SNDBUF test success
    optVal = 0;
    optLen = sizeof(optVal);
    ret = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &optVal, &optLen);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(optVal > 0);

    // setsockopt SO_RCVBUF test success
    optVal = 0x20000;
    optLen = sizeof(optVal);
    ret = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optVal, optLen);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(optVal > 0);

    // getsockopt SO_RCVBUF test success
    optVal = 0;
    optLen = sizeof(optVal);
    ret = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optVal, &optLen);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKOPT_0300
 * @tc.name   : SetAndGetsockoptTCP_TIMESTAMPTestSuccess_0003
 * @tc.desc   : setsockopt SO_TIMESTAMPING and getsockopt TCP_TIMESTAMP option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSockoptTest, GetsockoptTCP_TIMESTAMPTestSuccess_0003, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    int32_t optVal;
    socklen_t optLen;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(fd > 0);

    optVal = 1;
    optLen = sizeof(optVal);
    ret = setsockopt(fd, SOL_SOCKET, SO_TIMESTAMPING, &optVal, optLen);
    EXPECT_EQ(ret, 0);

    optVal = 0;
    optLen = sizeof(optVal);
    ret = getsockopt(fd, IPPROTO_TCP, TCP_TIMESTAMP, &optVal, &optLen);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(optVal > 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKOPT_0400
 * @tc.name   : SetAndGetsockoptIPV6_2292PKTOPTIONSTestSuccess_0004
 * @tc.desc   : setsockopt and getsockopt IPV6_2292PKTOPTIONS option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSockoptTest, SetAndGetsockoptIPV6_2292PKTOPTIONSTestSuccess_0004, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    int32_t optVal;
    socklen_t optLen;

    fd = socket(AF_INET6, SOCK_STREAM, 0);
    EXPECT_TRUE(fd > 0);

    optVal = 1;
    optLen = sizeof(optVal);
    ret = setsockopt(fd, IPPROTO_IPV6, IPV6_2292PKTOPTIONS, &optVal, optLen);
    EXPECT_EQ(ret, 0);

    optVal = 0;
    optLen = sizeof(optVal);
    ret = getsockopt(fd, IPPROTO_IPV6, IPV6_2292PKTOPTIONS, &optVal, &optLen);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKOPT_0500
 * @tc.name   : SetAndGetsockoptSO_LINGERTestSuccess_0005
 * @tc.desc   : setsockopt and getsockopt SO_LINGER option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSockoptTest, SetAndGetsockoptSO_LINGERTestSuccess_0005, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    struct linger optVal = {
        .l_onoff = 1,
        .l_linger = 1,
    };
    socklen_t optLen = sizeof(optVal);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(fd > 0);

    // setsockopt SO_LINGER test success
    ret = setsockopt(fd, SOL_SOCKET, SO_LINGER, &optVal, optLen);
    EXPECT_EQ(ret, 0);

    // getsockopt SO_LINGER test success
    memset_s(&optVal, sizeof(optVal), 0, sizeof(optVal));
    optLen = sizeof(optVal);
    ret = getsockopt(fd, SOL_SOCKET, SO_LINGER, &optVal, &optLen);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(optVal.l_onoff, 1);
    EXPECT_EQ(optVal.l_linger, 1);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKOPT_0600
 * @tc.name   : SetAndGetsockoptSO_TIMEOTestSuccess_0006
 * @tc.desc   : setsockopt and getsockopt SO_RCVTIMEO and SO_SNDTIMEO option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSockoptTest, SetAndGetsockoptSO_TIMEOTestSuccess_0006, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    struct timeval optVal = {
        .tv_sec = 1,
        .tv_usec = 1,
    };
    socklen_t optLen = sizeof(optVal);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(fd > 0);

    // setsockopt SO_RCVTIMEO test success
    ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &optVal, optLen);
    EXPECT_EQ(ret, 0);

    // getsockopt SO_RCVTIMEO test success
    memset_s(&optVal, sizeof(optVal), -1, sizeof(optVal));
    optLen = sizeof(optVal);
    ret = getsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &optVal, &optLen);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(optVal.tv_sec != -1);
    EXPECT_TRUE(optVal.tv_usec != -1);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKOPT_0700
 * @tc.name   : SetsockoptSO_FILTERTestSuccess_0007
 * @tc.desc   : setsockopt SO_ATTACH_FILTER and SO_DETACH_FILTER option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSockoptTest, SetsockoptSO_FILTERTestSuccess_0007, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    struct sock_filter code[] = {
        { 0x28, 0, 0, 0x0000000c },
        { 0x15, 0, 8, 0x000086dd },
        { 0x30, 0, 0, 0x00000014 },
        { 0x15, 2, 0, 0x00000084 },
        { 0x15, 1, 0, 0x00000006 },
        { 0x15, 0, 17, 0x00000011 },
        { 0x28, 0, 0, 0x00000036 },
        { 0x15, 14, 0, 0x00000016 },
        { 0x28, 0, 0, 0x00000038 },
        { 0x15, 12, 13, 0x00000016 },
        { 0x15, 0, 12, 0x00000800 },
        { 0x30, 0, 0, 0x00000017 },
        { 0x15, 2, 0, 0x00000084 },
        { 0x15, 1, 0, 0x00000006 },
        { 0x15, 0, 8, 0x00000011 },
        { 0x28, 0, 0, 0x00000014 },
        { 0x45, 6, 0, 0x00001fff },
        { 0xb1, 0, 0, 0x0000000e },
        { 0x48, 0, 0, 0x0000000e },
        { 0x15, 2, 0, 0x00000016 },
        { 0x48, 0, 0, 0x00000010 },
        { 0x15, 0, 1, 0x00000016 },
        { 0x6, 0, 0, 0x00040000 },
        { 0x6, 0, 0, 0x00000000 },
    };
    struct sock_fprog optVal = {
        .len = sizeof(code) / sizeof(code[0]),
        .filter = code,
    };
    socklen_t optLen = sizeof(optVal);

    fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    EXPECT_TRUE(fd > 0);

    // setsockopt SO_ATTACH_FILTER test success
    ret = setsockopt(fd, SOL_SOCKET, SO_ATTACH_FILTER, &optVal, optLen);
    EXPECT_EQ(ret, 0);

    // setsockopt SO_DETACH_FILTER test success
    ret = setsockopt(fd, SOL_SOCKET, SO_DETACH_FILTER, &optVal, optLen);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKOPT_0800
 * @tc.name   : SetsockoptTCP_CONGESTIONTestSuccess_0008
 * @tc.desc   : setsockopt and getsockopt TCP_CONGESTION option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSockoptTest, SetsockoptTCP_CONGESTIONTestSuccess_0008, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    char optVal[MAX_SIZE] = { 0 };
    socklen_t optLen = sizeof(optVal);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_TRUE(fd > 0);

    // setsockopt TCP_CONGESTION test success
    const char *algo = "cubic";
    ret = setsockopt(fd, IPPROTO_TCP, TCP_CONGESTION, algo, strlen(algo));
    EXPECT_EQ(ret, 0);

    // getsockopt get algo success
    ret = getsockopt(fd, IPPROTO_TCP, TCP_CONGESTION, &optVal, &optLen);
    EXPECT_EQ(ret, 0);
    EXPECT_STREQ(optVal, algo);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SOCKOPT_0900
 * @tc.name   : GetsockoptOnlySupportOptSuccess_0009
 * @tc.desc   : getsockopt only support option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSockoptTest, GetsockoptOnlySupportOptSuccess_0009, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    int32_t optVal;
    socklen_t optLen;
    for (int32_t i = 0; i < sizeof(g_getOnlyOpt) / sizeof(g_getOnlyOpt[0]); i++) {
        fd = socket(g_getOnlyOpt[i].domain, g_getOnlyOpt[i].type, 0);
        EXPECT_TRUE(fd > 0);

        optVal = 0;
        optLen = g_getOnlyOpt[i].optLen;
        ret = getsockopt(fd, g_getOnlyOpt[i].optLevel, g_getOnlyOpt[i].optName, &optVal, &optLen);
        EXPECT_EQ(ret, g_getOnlyOpt[i].ret);
        EXPECT_EQ(optVal, g_getOnlyOpt[i].optVal);
        EXPECT_EQ(optLen, g_getOnlyOpt[i].optLen);
        close(fd);
    }

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETSOCKOPT_1000
 * @tc.name   : SetAndGetsockoptInvalidFdFailed_0010
 * @tc.desc   : setsockopt and getsockopt use invalid socket fd test failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSockoptTest, SetAndGetsockoptInvalidFdFailed_0010, Function | MediumTest | Level2)
{
    int ret;
    int32_t optVal = 1;
    socklen_t optLen = sizeof(optVal);

    errno = 0;
    ret = setsockopt(BAD_SOCKET_FD, IPPROTO_TCP, TCP_TIMESTAMP, &optVal, optLen);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    ret = setsockopt(STDIN_FILENO, IPPROTO_TCP, TCP_TIMESTAMP, &optVal, optLen);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTSOCK);

    errno = 0;
    ret = getsockopt(BAD_SOCKET_FD, IPPROTO_TCP, TCP_TIMESTAMP, &optVal, &optLen);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    errno = 0;
    ret = getsockopt(STDIN_FILENO, IPPROTO_TCP, TCP_TIMESTAMP, &optVal, &optLen);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTSOCK);
}
