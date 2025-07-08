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
#include <unistd.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

class HatsGetsockoptTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsGetsockoptTest::SetUp()
{
}
void HatsGetsockoptTest::TearDown()
{
}
void HatsGetsockoptTest::SetUpTestCase()
{
}
void HatsGetsockoptTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETSOCKOPT_0100
 * @tc.name   : GetsockoptSuccess_0001
 * @tc.desc   : Getsockopt test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsGetsockoptTest, GetsockoptSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    struct ucred ucred;
    socklen_t optLen;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    EXPECT_TRUE(fd > 0);

    ret = getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &ucred, &optLen);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(ucred.uid != -1);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETSOCKOPT_0200
 * @tc.name   : GetsockoptSuccess_0002
 * @tc.desc   : getsockopt SO_SNDBUF option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsGetsockoptTest, GetsockoptSuccess_0002, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    int32_t optVal;
    socklen_t optLen;

    fd = socket(AF_INET, SOL_SOCKET, 0);
    EXPECT_TRUE(fd > 0);

    optVal = 0x20000;
    optLen = sizeof(optVal);
    ret = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &optVal, optLen);
    EXPECT_EQ(ret, 0);

    optVal = 0;
    optLen = sizeof(optVal);
    ret = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &optVal, &optLen);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(optVal > 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETSOCKOPT_0300
 * @tc.name   : GetsockoptSuccess_0003
 * @tc.desc   : getsockopt SO_RCVBUF option test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsGetsockoptTest, GetsockoptSuccess_0003, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    int32_t optVal;
    socklen_t optLen;

    fd = socket(AF_INET, SOL_SOCKET, 0);
    EXPECT_TRUE(fd > 0);

    optVal = 0x20000;
    optLen = sizeof(optVal);
    ret = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optVal, optLen);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(optVal > 0);

    optVal = 0;
    optLen = sizeof(optVal);
    ret = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optVal, &optLen);
    EXPECT_EQ(ret, 0);

    close(fd);
}