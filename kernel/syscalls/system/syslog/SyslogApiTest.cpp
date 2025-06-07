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
#include <csignal>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <syscall.h>
#include "securec.h"

using namespace testing::ext;

class HatsSyslogTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSyslogTest::SetUp()
{
}

void HatsSyslogTest::TearDown()
{
}

void HatsSyslogTest::SetUpTestCase()
{
}

void HatsSyslogTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0100
 * @tc.name   : SyslogOpenSuccess_0001
 * @tc.desc   : syslog Open the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogOpenSuccess_0001, Function | MediumTest | Level1)
{
    int ret = syscall(__NR_syslog, 1, nullptr, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0200
 * @tc.name   : SyslogCloseSuccess_0002
 * @tc.desc   : syslog Close the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSyslogTest, SyslogCloseSuccess_0002, Function | MediumTest | Level2)
{
    int ret = syscall(__NR_syslog, 0, nullptr, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0300
 * @tc.name   : SyslogSetLogLevelSuccess_0003
 * @tc.desc   : syslog Close the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogSetLogLevelSuccess_0003, Function | MediumTest | Level1)
{
    int ret;
    int i;

    for (i = 1; i < 8; i++) {
        ret = syscall(__NR_syslog, 7, nullptr, i);
        EXPECT_EQ(ret, 0);
    }

    ret = syscall(__NR_syslog, 7, nullptr, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0400
 * @tc.name   : SyslogSetLogLevelFailed_0004
 * @tc.desc   : syslog Close the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogSetLogLevelFailed_0004, Function | MediumTest | Level1)
{
    int ret = syscall(__NR_syslog, 8, nullptr, 9);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0500
 * @tc.name   : SyslogConsoleOffSuccess_0005
 * @tc.desc   : syslog Close the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogConsoleOffSuccess_0005, Function | MediumTest | Level1)
{
    int ret = syscall(__NR_syslog, 6, nullptr, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0600
 * @tc.name   : SyslogConsoleOnSuccess_0006
 * @tc.desc   : syslog Close the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogConsoleOnSuccess_0006, Function | MediumTest | Level1)
{
    int ret = syscall(__NR_syslog, 7, nullptr, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0700
 * @tc.name   : SyslogReadSysLogSuccess_0007
 * @tc.desc   : syslog Close the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogReadSysLogSuccess_0007, Function | MediumTest | Level1)
{
    int bufferSize;
    int readSize;
    char* buffer;

    bufferSize = syscall(__NR_syslog, 10, nullptr, 0);
    EXPECT_GE(bufferSize, 0);

    buffer = (char *)malloc(bufferSize + 1);
    EXPECT_NE(buffer, nullptr);

    readSize = syscall(__NR_syslog, 2, buffer, bufferSize);
    EXPECT_GE(readSize, 0);
    buffer = nullptr;
}
